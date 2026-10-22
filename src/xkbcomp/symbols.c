/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Ran Benita <ran234@gmail.com>
 */

#include "config.h"

#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-status.h"
#include "xkbcommon/xkbcommon-keysyms.h"

#include "action.h"
#include "context.h"
#include "darray.h"
#include "expr.h"
#include "include.h"
#include "keymap.h"
#include "keysym.h"
#include "messages-codes.h"
#include "text.h"
#include "utils.h"
#include "util-numbers.h"
#include "util-mem.h"
#include "vmod.h"
#include "xkbcomp-priv.h"
#include "xkbcomp/ast.h"

enum key_repeat {
    KEY_REPEAT_UNDEFINED = 0,
    KEY_REPEAT_YES,
    KEY_REPEAT_NO,
    _KEY_REPEAT_NUM_ENTRIES,
};
enum { KEY_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(_KEY_REPEAT_NUM_ENTRIES - 1 <=
              (1 << (KEY_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

enum group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

enum key_field {
    KEY_FIELD_NONE = 0,
    KEY_FIELD_REPEAT = (1 << 0),
    KEY_FIELD_DEFAULT_TYPE = (1 << 1),
    KEY_FIELD_GROUPINFO = (1 << 2),
    KEY_FIELD_VMODMAP = (1 << 3),
    KEY_FIELD_OVERLAY = (1 << 4),
    KEY_FIELD_ALL = KEY_FIELD_REPEAT | KEY_FIELD_DEFAULT_TYPE
                  | KEY_FIELD_GROUPINFO | KEY_FIELD_VMODMAP | KEY_FIELD_OVERLAY,
};
enum { KEY_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(KEY_FIELD_ALL <= (1 << (KEY_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

typedef struct {
    darray(struct xkb_level) levels;
    enum group_field defined;
    xkb_atom_t type;
} GroupInfo;

enum {
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH = 4 /* 3 bits + sign */,
    KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD = 8,
    KEYINFO_DEFINED_BIT_FIELD = 16,
    KEYINFO_MERGE_BIT_FIELD = 8,
    KEYINFO_REPEAT_BIT_FIELD = 8,
};
static_assert(
    XKB_LAYOUT_OUT_OF_RANGE_WRAP < XKB_LAYOUT_OUT_OF_RANGE_CLAMP &&
    XKB_LAYOUT_OUT_OF_RANGE_CLAMP < XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_OUT_OF_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD >=
              XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH, "");
static_assert((unsigned)KEYINFO_DEFINED_BIT_FIELD >=
              (unsigned)KEY_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)KEY_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

typedef struct {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_number;
    darray(GroupInfo) groups;
    enum xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    enum key_field defined:KEYINFO_DEFINED_BIT_FIELD;
    enum merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    enum key_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

    bool out_of_range_pending_group:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_index_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_mask_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    union {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        const struct xkb_key *overlay_key;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        const struct xkb_key **overlays_keys;
    };
} KeyInfo;

static void
StealLevelInfo(struct xkb_level *into, struct xkb_level *from)
{
    clear_level(into);
    ако (from->num_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } иначе {
        into->s.sym = from->s.sym;
    }
    into->num_syms = from->num_syms;
    from->num_syms = 0;

    ако (from->num_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } иначе {
        into->a.action = from->a.action;
    }
    into->num_actions = from->num_actions;
    from->num_actions = 0;
}

static void
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, sizeof(*groupi));
}

static void
ClearGroupInfo(GroupInfo *groupi)
{
    struct xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

static void
CopyGroupInfo(GroupInfo *to, const GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    за (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        ако (darray_item(from->levels, j).num_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).num_syms,
                       sizeof(xkb_keysym_t));
        }
        ако (darray_item(from->levels, j).num_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).num_actions,
                       sizeof(union xkb_action));
        }
    }
}

static void
InitKeyInfo(struct xkb_context *ctx, KeyInfo *keyi)
{
    memset(keyi, 0, sizeof(*keyi));
    static_assert(!DEFAULT_KEY_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_KEY_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_key = NULL;
}

static void
ClearKeyInfo(KeyInfo *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    ако (keyi->overlays_alloc)
        free(keyi->overlays_keys);
}

/***====================================================================***/

typedef struct {
    char *name;         /* e.g. pc+us+inet(evdev) */
    цяло errorCount;
    unsigned цяло include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(KeyInfo) keys;
    KeyInfo default_key;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    struct xkb_mod_set mods;

    struct xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    const struct xkb_keymap_info *keymap_info;
} SymbolsInfo;

static void
InitSymbolsInfo(SymbolsInfo *info, const struct xkb_keymap_info *keymap_info,
                unsigned цяло include_depth, const struct xkb_mod_set *mods)
{
    memset(info, 0, sizeof(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    InitKeyInfo(keymap_info->keymap.ctx, &info->default_key);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

static void
ClearSymbolsInfo(SymbolsInfo *info)
{
    KeyInfo *keyi;
    free(info->name);
    darray_foreach(keyi, info->keys)
        ClearKeyInfo(keyi);
    darray_free(info->keys);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    ClearKeyInfo(&info->default_key);
}

static const char *
KeyInfoText(SymbolsInfo *info, KeyInfo *keyi)
{
    върни KeyNameText(info->ctx, keyi->name);
}

static bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t key_name)
{
    /* First find the type of the merged group. */
    ако (into->type != from->type) {
        ако (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        иначе ако (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        иначе {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ако (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, KeyNameText(info->ctx, key_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    ако (darray_empty(from->levels)) {
        InitGroupInfo(from);
        върни true;
    }

    ако (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        върни true;
    }

    /* Merge the actions and syms. */
    const darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t fromKeysymsCount = 0;
    darray_size_t fromActionsCount = 0;
    за (darray_size_t i = 0; i < levels_in_both; i++) {
        struct xkb_level* const intoLevel = &darray_item(into->levels, i);
        struct xkb_level* const fromLevel = &darray_item(from->levels, i);

        const bool fromHasNoKeysym = fromLevel->num_syms == 0;
        const bool fromHasNoAction = fromLevel->num_actions == 0;
        ако (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            continue;
        }

        const bool intoHasNoKeysym = intoLevel->num_syms == 0;
        const bool intoHasNoAction = intoLevel->num_actions == 0;
        ако (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            fromKeysymsCount++;
            fromActionsCount++;
            continue;
        }

        /* Possible level conflict */
        assert(intoLevel->num_syms > 0 || intoLevel->num_actions > 0);
        assert(fromLevel->num_syms > 0 || fromLevel->num_actions > 0);

        /* Handle keysyms */
        ако (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ако (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_SYMBOL,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, KeyNameText(info->ctx, key_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ако (fromHasNoKeysym) {
                /* No keysym to copy */
            } иначе ако (clobber) {
                /* Override: copy any defined keysym from `from` */
                ако (unlikely(fromLevel->num_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ако (unlikely(intoLevel->num_syms > 1))
                        free(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->num_syms = fromLevel->num_syms;
                    fromLevel->num_syms = 0;
                    fromKeysymsCount++;
                } иначе ако (fromLevel->s.sym != XKB_KEY_NoSymbol) {
                    /* Single defined keysym */
                    ако (unlikely(intoLevel->num_syms > 1))
                        free(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->num_syms = 1;
                    fromKeysymsCount++;
                }
            } иначе {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ако (unlikely(intoLevel->num_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } иначе ако (intoLevel->s.sym == XKB_KEY_NoSymbol) {
                    /* Single undefined keysym */
                    ако (unlikely(fromLevel->num_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->num_syms = fromLevel->num_syms;
                        fromLevel->num_syms = 0;
                    } иначе {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->num_syms = 1;
                    }
                    fromKeysymsCount++;
                }
            }
        }

        /* Handle actions */
        ако (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ако (report && !(intoHasNoAction || fromHasNoAction)) {
                ако (unlikely(intoLevel->num_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_KEY_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, KeyNameText(info->ctx, key_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } иначе {
                    union xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                KeyNameText(info->ctx, key_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            ако (fromHasNoAction) {
                /* No action to copy */
            } иначе ако (clobber) {
                /* Override: copy any defined action from `from` */
                ако (unlikely(fromLevel->num_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ако (unlikely(intoLevel->num_actions > 1))
                        free(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->num_actions = fromLevel->num_actions;
                    fromLevel->num_actions = 0;
                    fromActionsCount++;
                } иначе ако (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ако (unlikely(intoLevel->num_actions > 1))
                        free(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->num_actions = 1;
                    fromActionsCount++;
                }
            } иначе {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ако (unlikely(intoLevel->num_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } иначе ако (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ако (unlikely(fromLevel->num_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->num_actions = fromLevel->num_actions;
                        fromLevel->num_actions = 0;
                    } иначе {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->num_actions = 1;
                    }
                    fromActionsCount++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    struct xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->num_syms = 0;
        level->num_actions = 0;
        fromKeysymsCount++;
        fromActionsCount++;
    }
    ако (fromKeysymsCount) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ако (fromKeysymsCount == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    ако (fromActionsCount) {
        /* Reset defined actions field if we used no action from `into` */
        ако (fromActionsCount == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    върни true;
}

static bool
UseNewKeyField(enum key_field field, enum key_field old, enum key_field new,
               bool clobber, bool report, enum key_field *collide)
{
    ако (!(old & field))
        върни (new & field);

    ако (new & field) {
        ако (report)
            *collide |= field;

        върни clobber;
    }

    върни false;
}

static bool
overlays_get(const KeyInfo *info, xkb_overlay_index_t bit,
             const struct xkb_key **key_out)
{
    ако (bit >= (xkb_overlay_index_t)(sizeof(xkb_overlay_mask_t) * CHAR_BIT))
        върни false;
    const xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    ако (!(info->overlays & mask)) върни false;
    ако (key_out) {
        ако (!info->overlays_alloc) {
            /* Stored inline */
            *key_out = info->overlay_key;
        } иначе {
            /* Stored in sparse array */
            const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            const xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(low);
            *key_out = info->overlays_keys[index];
        }
    }
    върни true;
}

static bool
overlays_insert(KeyInfo *keyi, xkb_overlay_index_t bit,
                const struct xkb_key *key)
{
    ако (bit >= (xkb_overlay_index_t)(sizeof(xkb_overlay_mask_t) * CHAR_BIT))
        върни false;

    const xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    ако ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ако (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_key = key;
            ако (!key)
                keyi->overlays_clear = true;
        } иначе {
            /* Stored in sparse array */
            const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            const xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(low);
            keyi->overlays_keys[index] = key;
        }
        върни true;
    }

    /* Insert new element */
    ако (!keyi->overlays || (keyi->overlays_clear && !key)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_key = key;
        keyi->overlays |= mask;
        keyi->overlays_clear = !key;
    } иначе ако (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        const xkb_overlay_mask_t overlays = keyi->overlays | mask;
        const xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        const struct xkb_key ** const tmp = calloc(alloc, sizeof(*tmp));
        ако (!tmp) върни false;

        /* Copy previous entries to the buffer */
        ако (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            const xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(low);
            tmp[idx] = keyi->overlay_key;
        } иначе {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        const xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(low);
        tmp[idx] = key;

        keyi->overlays_keys = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } иначе {
        /* Allocated items: grow heap and shift if relevant */
        const xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        const xkb_overlay_index_t count =
            (xkb_overlay_index_t)popcount32(overlays);
        ако (count > keyi->overlays_alloc) {
            const xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(count);
            assert(alloc >= count);
            const struct xkb_key ** const tmp = realloc(
                keyi->overlays_keys,
                (size_t)alloc * sizeof(*keyi->overlays_keys)
            );
            ако (!tmp) върни false;
            keyi->overlays_keys = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        const xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(low);
        ако (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ако (index < count - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            assert(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_keys + index + 1, keyi->overlays_keys + index,
                    (size_t)(count - 1u - index) * sizeof(const struct xkb_key *));
        }
        keyi->overlays_keys[index] = key;
        keyi->overlays = overlays;
    }

    върни true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
static bool
merge_overlays(SymbolsInfo *info, KeyInfo *into, KeyInfo *from,
               bool clobber, bool report, enum key_field *collide)
{
    ако (!(from->defined & KEY_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } иначе ако (!(into->defined & KEY_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_keys = from->overlays_keys;
        from->overlays_keys = NULL;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= KEY_FIELD_OVERLAY;
    } иначе ако (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } иначе {
        ако (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            const xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            const xkb_overlay_index_t count =
                (xkb_overlay_index_t)popcount32(result_mask);

            ако (count == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            KeyInfo *dest = into;
            KeyInfo *src = from;
            ако (into->overlays_alloc >= count &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } иначе ако (from->overlays_alloc >= count) {
                dest = from;
                src = into;
                clobber = !clobber;
            } иначе ако (count > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ако (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ако (!dest->overlays_alloc) {
                    const struct xkb_key **tmp = calloc(count, sizeof(*tmp));
                    ако (!tmp)
                        върни false;
                    ако (dest->overlay_key)
                        tmp[0] = dest->overlay_key;
                    dest->overlays_keys = tmp;
                    dest->overlays_alloc = count;
                } иначе {
                    const struct xkb_key **tmp =
                        realloc(dest->overlays_keys,
                                count * sizeof(*dest->overlays_keys));
                    ако (!tmp)
                        върни false;
                    dest->overlays_keys = tmp;
                    dest->overlays_alloc = count;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            const struct xkb_key **src_keys = (src->overlays_clear)
                ? NULL
                : (src->overlays_alloc)
                    ? src->overlays_keys
                    : &src->overlay_key;
            докато (remaining) {
                /* isolate lowest set bit */
                const xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                const xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                ако (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                const struct xkb_key *src_key = (src_keys)
                    ? *(src_keys++)
                    : NULL;

                /* check dest */
                const struct xkb_key *dest_key = NULL;
                const bool conflict = overlays_get(dest, bit, &dest_key);
                ако (conflict) {
                    ако (dest_key == src_key)
                       continue;
                    иначе ако (report)
                        *collide |= KEY_FIELD_OVERLAY;
                }

                ако ((!conflict || clobber) && !overlays_insert(dest, bit, src_key))
                    върни false;
            }

            ако (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ако (into->overlays_alloc)
                    free(into->overlays_keys);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ако (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_keys = from->overlays_keys;
                    from->overlays_keys = NULL;
                    from->overlays_alloc = 0;
                } иначе {
                    into->overlay_key = from->overlay_key;
                }
            }
        } иначе {
            /* Disjoint overlays */

            ако (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_key == from->overlay_key) {
                /* Indentical: do nothing */
                върни true;
            }
            ако (!(into->overlays & from->overlays)) {
                ако (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_key = from->overlay_key;
                    върни true;
                } иначе ако (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    върни true;
                }
            }
            /* Conflict */
            ако (report)
                *collide |= KEY_FIELD_OVERLAY;
            ако (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_key = from->overlay_key;
            }
        }
    }
    върни true;
}

static bool
MergeKeys(SymbolsInfo *info, KeyInfo *into, KeyInfo *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    enum key_field collide = 0;
    const цяло verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool clobber = (from->merge != MERGE_AUGMENT);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;

    ако (from->merge == MERGE_REPLACE) {
        ClearKeyInfo(into);
        *into = *from;
        InitKeyInfo(info->ctx, from);
        върни true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    за (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    за (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    ако (UseNewKeyField(KEY_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= KEY_FIELD_VMODMAP;
    }
    ако (UseNewKeyField(KEY_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= KEY_FIELD_REPEAT;
    }
    ако (UseNewKeyField(KEY_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= KEY_FIELD_DEFAULT_TYPE;
    }
    ако (UseNewKeyField(KEY_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_number = from->out_of_range_group_number;
        into->defined |= KEY_FIELD_GROUPINFO;
    }

    ако (!merge_overlays(info, into, from, clobber, report, &collide))
        върни false;

    ако (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 KeyNameText(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    ClearKeyInfo(from);
    InitKeyInfo(info->ctx, from);
    върни true;
}

static xkb_atom_t
XkbResolveKeyAlias(const struct xkb_keymap *keymap, xkb_atom_t name)
{
    ако (name < keymap->num_key_names) {
        const KeycodeMatch match = keymap->key_names[name];
        ако (match.found && match.is_alias) {
            върни match.alias.real;
        }
    }
    върни name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
static bool
AddKeySymbols(SymbolsInfo *info, KeyInfo *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = XkbResolveKeyAlias(&info->keymap_info->keymap, keyi->name);

    KeyInfo *iter;
    darray_foreach(iter, info->keys)
        ако (iter->name == keyi->name)
            върни MergeKeys(info, iter, keyi, same_file);

    darray_append(info->keys, *keyi);
    InitKeyInfo(info->ctx, keyi);
    върни true;
}

static bool
AddModMapEntry(SymbolsInfo *info, const ModMapEntry *new)
{
    const bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        ако ((new->haveSymbol != old->haveSymbol) ||
            (new->haveSymbol && new->u.keySym != old->u.keySym) ||
            (!new->haveSymbol && new->u.keyName != old->u.keyName))
            continue;

        ако (new->mods == old->mods)
            върни true;

        const xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        const xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        ако (new->haveSymbol) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.keySym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } иначе {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeyNameText(info->ctx, new->u.keyName),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        върни true;
    }

    darray_append(info->modmaps, *new);

    върни true;
}

/***====================================================================***/

static void
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     enum merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    ако (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        върни;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ако (into->name == NULL) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    за (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        ако (!darray_item(from->group_names, i))
            continue;

        ако (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            continue;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    ако (darray_empty(into->keys)) {
        into->keys = from->keys;
        darray_init(from->keys);
    }
    иначе {
        KeyInfo *keyi;
        darray_foreach(keyi, from->keys) {
            keyi->merge = merge;
            ако (!AddKeySymbols(into, keyi, false))
                into->errorCount++;
        }
    }

    ако (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    иначе {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            ако (!AddModMapEntry(into, mm))
                into->errorCount++;
        }
    }
}

static void
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

static bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ако (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        върни false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    const struct parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    за (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        char path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, sizeof(path));
        ако (!file) {
            info->errorCount += 10;
            ClearSymbolsInfo(&included);
            върни false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        ако (stmt->modifier) {
            const цяло count = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            ако (count <= 0 || *(stmt->modifier + count) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } иначе {
                next_incl.explicit_group--;
            }
        }
        иначе ако (info->keymap_info->keymap.num_groups != 0 &&
                 next_incl.include_depth == 1) {
            /*
             * If keymap is the result of RMLVO resolution and we are at the
             * first include depth, transform e.g. `pc` into `pc:1` in order to
             * force only one group per key using the explicit group.
             *
             * NOTE: X11’s xkbcomp does not apply this rule.
             *
             * WARNING: If this feature is removed, then the entries `Last` in
             * the `groupIndexNames` and `groupMaskNames` LUT should initially
             * be inactive.
             */
            next_incl.explicit_group = 0;
        }
        иначе {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            next_incl.explicit_group = info->explicit_group;
        }

        HandleSymbolsFile(&next_incl, file);

        MergeIncludedSymbols(&included, &next_incl, stmt->merge);

        ClearSymbolsInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedSymbols(info, &included, include->merge);
    ClearSymbolsInfo(&included);

    върни (info->errorCount == 0);
}

static bool
GetGroupIndex(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
              enum group_field field, xkb_layout_index_t *ndx_rtrn)
{
    assert (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    const char *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    ако (arrayNdx == NULL) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ако (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                върни true;
            }
        }

        ако (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, KeyInfoText(info, keyi), info->max_groups, name);
            върни false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        върни true;
    }

    ако (ExprResolveGroup(info->keymap_info, arrayNdx, false, ndx_rtrn, NULL) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, KeyInfoText(info, keyi));
        върни false;
    }

    (*ndx_rtrn)--;
    ако (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    върни true;
}

static bool
AddSymbolsToKey(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
                ExprDef *value)
{
    xkb_layout_index_t ndx = 0;

    ако (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        върни false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ако (value->common.type == STMT_EXPR_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        върни true;
    }

    ако (value->common.type != STMT_EXPR_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(value->common.type), ndx + 1,
                KeyInfoText(info, keyi));
        върни false;
    }

    ако (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_KEY_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                KeyInfoText(info, keyi), ndx + 1);
        върни false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    за (ExprKeysymList *keysymList = (ExprKeysymList *) value;
         keysymList;
         keysymList = (ExprKeysymList *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        ако (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    ако (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    ако (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    за (ExprKeysymList *keysymList = (ExprKeysymList *) value;
         keysymList && level < nLevels;
         keysymList = (ExprKeysymList *) keysymList->common.next, level++) {
        struct xkb_level *leveli = &darray_item(groupi->levels, level);
        assert(leveli->num_syms == 0);

        ако (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    KeyInfoText(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            върни false;
        }

        leveli->num_syms = (xkb_keysym_count_t) darray_size(keysymList->syms);
        switch (leveli->num_syms) {
        case 0:
            leveli->s.sym = XKB_KEY_NoSymbol;
            break;
        case 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            assert(leveli->s.sym != XKB_KEY_NoSymbol);
            break;
        default:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, NULL);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            за (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++)
                assert(leveli->s.syms[k] != XKB_KEY_NoSymbol);
#endif
        }
    }

    върни true;
}

static bool
AddActionsToKey(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
                ExprDef *value)
{
    xkb_layout_index_t ndx = 0;

    ако (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        върни false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ако (value->common.type == STMT_EXPR_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        върни true;
    }

    ако (value->common.type != STMT_EXPR_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 value->common.type, ndx, KeyInfoText(info, keyi));
        върни false;
    }

    ако (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_KEY_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 KeyInfoText(info, keyi), ndx);
        върни false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    за (ParseCommon *p = &value->common; p; p = p->next)
        nLevels++;
    ако (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    за (ExprActionList *actionList = (ExprActionList *) value;
         actionList;
         actionList = (ExprActionList *) actionList->common.next, level++) {
        struct xkb_level *leveli = &darray_item(groupi->levels, level);
        assert(leveli->num_actions == 0);

        unsigned цяло num_actions = 0;
        за (ExprDef *act = actionList->actions;
             act; act = (ExprDef *) act->common.next)
            num_actions++;

        ако (unlikely(num_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    KeyInfoText(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, num_actions);
            върни false;
        }

        /* Parse actions and add only defined actions */
        darray(union xkb_action) actions = darray_new();
        за (ExprDef *act = actionList->actions;
             act; act = (ExprDef *) act->common.next) {
            union xkb_action toAct = { 0 };
            const enum xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            ако (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        KeyInfoText(info, keyi), ndx + 1, level + 1);
                ако (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    върни false;
                } иначе {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            ако (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } иначе ако (likely(num_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->num_actions = 1;
                leveli->a.action = toAct;
                goto next_level;
            } иначе {
                darray_append(actions, toAct);
            }
        }
        ако (darray_empty(actions)) {
            leveli->num_actions = 0;
        } иначе ако (likely(darray_size(actions) > 1)) {
            leveli->num_actions = (xkb_action_count_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, NULL);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            за (xkb_action_count_t k = 0; k < leveli->num_actions; k++)
                assert(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } иначе {
            /* Unlikely: some actions were dropped and only one remains */
            assert(num_actions > darray_size(actions));
            leveli->num_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            assert(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ако (leveli->num_actions > 0 || leveli->num_syms > 0)
            nonEmptyLevels = level + 1;
    }

    ако (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        ако (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        иначе
            darray_free(groupi->levels);
    }

    върни true;
}

static const LookupEntry repeatEntries[] = {
    { "true", KEY_REPEAT_YES },
    { "yes", KEY_REPEAT_YES },
    { "on", KEY_REPEAT_YES },
    { "false", KEY_REPEAT_NO },
    { "no", KEY_REPEAT_NO },
    { "off", KEY_REPEAT_NO },
    { "default", KEY_REPEAT_UNDEFINED },
    { NULL, 0 }
};

/** Handle overlay<N> = <key> entry */
static bool
ExprResolveOverlayEntry(const struct xkb_keymap_info *keymap_info,
                        const char *field, const ExprDef *arrayNdx,
                        const ExprDef *expr, KeyInfo *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        const struct xkb_key **key_rtrn)
{
    /* Overlay index */
    ако (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
        върни false;
    }
    static_assert(sizeof(xkb_overlay_index_t) == sizeof(uint8_t), "");
    const size_t prefix = sizeof("overlay") - 1;
    const size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ако (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (цяло)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        върни false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    switch (expr->common.type) {
    case STMT_EXPR_KEYNAME_LITERAL:
        *key_rtrn = XkbKeyByName(&keymap_info->keymap, expr->key_name.key_name,
                                 false);
        ако (!*key_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, expr->key_name.key_name),
                    field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
            върни false;
        }
        върни true;
    case STMT_EXPR_IDENT: {
        const char * const id =
            xkb_atom_text(keymap_info->keymap.ctx, expr->ident.ident);
        ако (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *key_rtrn = NULL;
            върни true;
        } иначе ако (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *key_rtrn = NULL;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            върни true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_VALUE,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
        върни false;
    }
    default:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_VALUE,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_EXPR_KEYNAME_LITERAL),
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(expr->common.type));
        върни false;
    }
}

static bool
SetSymbolsField(SymbolsInfo *info, KeyInfo *keyi, const char *field,
                ExprDef *arrayNdx, ExprDef **value_ptr)
{
    ExprDef * restrict const value = *value_ptr;
    ако (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ако (!ExprResolveString(info->ctx, value, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            върни false;
        }

        ако (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= KEY_FIELD_DEFAULT_TYPE;
        }
        иначе ако (ExprResolveGroup(info->keymap_info, arrayNdx, false,
                                  &ndx, NULL) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    KeyInfoText(info, keyi));
            върни false;
        }
        иначе {
            ndx--;
            ако (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                assert(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    иначе ако (istreq(field, "symbols")) {
        върни AddSymbolsToKey(info, keyi, arrayNdx, value);
    }
    иначе ако (istreq(field, "actions")) {
        върни AddActionsToKey(info, keyi, arrayNdx, value);
    }
    иначе ако (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        ако (!ExprResolveModMask(info->ctx, value, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(value->common.type),
                    KeyInfoText(info, keyi));
            върни false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= KEY_FIELD_VMODMAP;
    }
    иначе ако (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                KeyInfoText(info, keyi));
    }
    иначе ако (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                KeyInfoText(info, keyi));
    }
    иначе ако (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                KeyInfoText(info, keyi));
    }
    иначе ако (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        const struct xkb_key *key = NULL;
        ако (!ExprResolveOverlayEntry(info->keymap_info, field, arrayNdx,
                                     *value_ptr, keyi, &overlay, &key))
            върни false;
        ако (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            върни true;
        } иначе ако (key && key->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, KeyInfoText(info, keyi));
        } иначе {
            const struct xkb_key *prev;
            ако (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ако (key != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             KeyInfoText(info, keyi), overlay + 1,
                             (prev
                                 ? KeyNameText(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (key
                                 ? KeyNameText(info->ctx, key->name)
                                 : "none"));
                }
            } иначе ако (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ако (!overlays_insert(keyi, overlay, key))
                    върни false;
                keyi->defined |= KEY_FIELD_OVERLAY;
            } иначе {
                /* Handle disjoint overlays mode */
                const xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                ако (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ако (key) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_key = key;
                    } иначе {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= KEY_FIELD_OVERLAY;
                } иначе ако (keyi->overlays) {
                    ако (key) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                KeyInfoText(info, keyi), keyi->overlays,
                                KeyNameText(info->ctx, keyi->overlay_key->name),
                                overlay + 1, KeyNameText(info->ctx, key->name));
                        върни !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_VALUE_MISMATCH);
                    } иначе {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    иначе ако (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        ако (!ExprResolveEnum(info->ctx, value, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    KeyInfoText(info, keyi));
            върни false;
        }

        keyi->repeat = (enum key_repeat) val;
        keyi->defined |= KEY_FIELD_REPEAT;
    }
    иначе ако (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        ако (!ExprResolveBoolean(info->ctx, value, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    KeyInfoText(info, keyi));
            върни false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    иначе ако (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        ако (!ExprResolveBoolean(info->ctx, value, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    KeyInfoText(info, keyi));
            върни false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    иначе ако (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ако (ExprResolveGroup(info->keymap_info, value, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    KeyInfoText(info, keyi));
            върни false;
        }

        ако (pending) {
            keyi->out_of_range_pending_group = true;
            const darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (struct pending_computation) {
                    .expr = *value_ptr,
                    .computed = false,
                    .value = 0,
                }
            );
            *value_ptr = NULL;
            static_assert(sizeof(keyi->out_of_range_group_number) ==
                          sizeof(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_number = pending_index;
        } иначе {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_number = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    иначе {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        върни !(info->keymap_info->strict & PARSER_NO_UNKNOWN_KEY_FIELDS);
    }

    върни true;
}

static bool
SetGroupName(SymbolsInfo *info, ExprDef *arrayNdx, ExprDef *value,
             enum merge_mode merge)
{
    ако (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        върни false;
    }

    xkb_layout_index_t group = 0;
    ако (ExprResolveGroup(info->keymap_info, arrayNdx, false, &group, NULL) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        върни false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ако (!ExprResolveString(info->ctx, value, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        върни false;
    }

    xkb_layout_index_t group_to_use;
    ако (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    иначе ако (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    иначе {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        върни false;
    }

    ако (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        assert(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } иначе {
        const xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        ако (old_name != XKB_ATOM_NONE && old_name != name) {
            const bool replace = (merge != MERGE_AUGMENT);
            const xkb_atom_t use    = (replace ? name : old_name);
            const xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     group_to_use, info->name,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
            name = use;
        }
    }
    darray_item(info->group_names, group_to_use) = name;

    върни true;
}

static bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *arrayNdx;
    bool ret;

    ако (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        върни false;

    ако (elem && istreq(elem, "key")) {
        KeyInfo temp = {0};
        InitKeyInfo(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->value) &&
              MergeKeys(info, &info->default_key, &temp, true);
        ClearKeyInfo(&temp);
    }
    иначе ако (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->value, stmt->merge);
    }
    иначе ако (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    иначе ако (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    иначе ако (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    иначе ако (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    иначе ако (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->value, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } иначе {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        върни !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    върни ret;
}

static bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, KeyInfo *keyi)
{
    bool all_valid_entries = true;

    за (; def; def = (VarDef *) def->common.next) {
        const char *field;
        ExprDef *arrayNdx;
        bool ok = true;

        ако (!def->name) {
            ако (unlikely(!def->value) || def->value->common.type != STMT_EXPR_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            иначе
                field = "actions";
            arrayNdx = NULL;
        }
        иначе {
            const char *elem;
            ok = ExprResolveLhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            ако (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        ако (unlikely(!def->value)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        ако (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->value))
            all_valid_entries = false;
    }

    върни all_valid_entries;
}

static bool
SetExplicitGroup(SymbolsInfo *info, KeyInfo *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    ако (info->explicit_group == XKB_LAYOUT_INVALID)
        върни true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ако (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    ако (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, KeyInfoText(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    ако (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    върни true;
}

static bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    KeyInfo keyi;

    keyi = info->default_key;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_key.groups);
    за (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_key.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->keyName;

    ако (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        AddKeySymbols(info, &keyi, true)) {
        върни true;
    }

    ClearKeyInfo(&keyi);
    info->errorCount++;
    върни false;
}

static bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    struct xkb_context *ctx = info->ctx;

    ако (!ExprResolveModMask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        върни false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    assert(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_EXPR_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    за (ExprDef *key = def->keys; key; key = (ExprDef *) key->common.next) {
        ако (key->common.type == STMT_EXPR_KEYNAME_LITERAL) {
            tmp.haveSymbol = false;
            tmp.u.keyName = key->key_name.key_name;
        }
        иначе ако (key->common.type == STMT_EXPR_KEYSYM_LITERAL) {
            ако (key->keysym.keysym == XKB_KEY_NoSymbol) {
                /* Invalid keysym: ignore. Error message already printed */
                continue;
            } иначе {
                tmp.haveSymbol = true;
                tmp.u.keySym = key->keysym.keysym;
            }
        }
        иначе {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            continue;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    върни ok;
}

static void
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    за (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            break;
        case STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            break;
        case STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            break;
        case STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            break;
        case STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            break;
        case STMT_UNKNOWN_DECLARATION:
        case STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            break;
        default:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            break;
        }

        ако (!ok)
            info->errorCount++;

        ако (info->errorCount > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            break;
        }
    }
}

/**
 * Given a keysym @sym, return a key which generates it, or NULL.
 * This is used for example in a modifier map definition, such as:
 *      modifier_map Lock           { Caps_Lock };
 * where we want to add the Lock modifier to the modmap of the key
 * which matches the keysym Caps_Lock.
 * Since there can be many keys which generates the keysym, the key
 * is chosen first by lowest group in which the keysym appears, than
 * by lowest level and than by lowest key code.
 */
static struct xkb_key *
FindKeyForSymbol(struct xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    do {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        do {
            got_one_level = false;
            struct xkb_key *key;
            xkb_keys_foreach(key, keymap) {
                ако (group < key->num_groups &&
                    level < XkbKeyNumLevels(key, group)) {
                    got_one_group = got_one_level = true;
                    const xkb_keysym_count_t num_syms =
                        key->groups[group].levels[level].num_syms;
                    ако (num_syms > 1) {
                        за (xkb_keysym_count_t k = 0; k < num_syms; k++) {
                            ако (key->groups[group].levels[level].s.syms[k] == sym)
                                върни key;
                        }
                    } иначе ако (num_syms &&
                               key->groups[group].levels[level].s.sym == sym) {
                        върни key;
                    }
                }
            }
            level++;
        } докато (got_one_level);
        group++;
    } докато (got_one_group);

    върни NULL;
}

/*
 * Find an appropriate type for a group and return its name.
 *
 * Simple recipe:
 * - ONE_LEVEL for width 0/1
 * - ALPHABETIC for 2 shift levels, with lower/upercase keysyms
 * - KEYPAD for keypad keys.
 * - TWO_LEVEL for other 2 shift level keys.
 * and the same for four level keys.
 *
 * FIXME: Decide how to handle multiple-syms-per-level, and do it.
 */
static xkb_atom_t
FindAutomaticType(struct xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    const xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).num_syms == 0 ? \
        XKB_KEY_NoSymbol : \
     darray_item(groupi->levels, level).num_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    ако (width == 1 || width <= 0)
        върни xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ако (width == 2) {
        ако (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            върни xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ако (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            върни xkb_atom_intern_literal(ctx, "KEYPAD");

        върни xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ако (width <= 4) {
        ако (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_KEY_NoSymbol);

            ако (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                върни xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            върни xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ако (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            върни xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        върни xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    върни XKB_ATOM_NONE;

#undef GET_SYM
}

static const struct xkb_key_type *
FindTypeForGroup(struct xkb_keymap *keymap, KeyInfo *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ако (type_name == XKB_ATOM_NONE) {
        ако (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        иначе {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            ако (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ако (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_KEY_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 KeyNameText(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        goto use_default;
    }

    darray_size_t i;
    за (i = 0; i < keymap->num_types; i++)
        ако (keymap->types[i].name == type_name)
            break;

    ако (i >= keymap->num_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_KEY_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 KeyNameText(keymap->ctx, keyi->name), group + 1);
        goto use_default;
    }

    keymap->types[i].required = true;
    върни &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    върни &keymap->types[0];
}

static bool
CopySymbolsDefToKeymap(struct xkb_keymap *keymap, SymbolsInfo *info,
                       KeyInfo *keyi)
{
    struct xkb_key *key;
    GroupInfo *groupi;
    const GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    key = XkbKeyByName(keymap, keyi->name, false);
    ако (!key) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                KeyInfoText(info, keyi));
        върни false;
    }

    /* Find the range of groups we need. */
    key->num_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        const bool has_explicit_type = (keyi->defined & KEY_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        ако (darray_size(groupi->levels) > 0 || has_explicit_type)
            key->num_groups = i + 1;
        ако (has_explicit_type)
            key->explicit |= EXPLICIT_TYPES;
    }

    ако (key->num_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ако (keyi->defined)
            goto key_fields;
        иначе
            върни false;
    }

    darray_resize(keyi->groups, key->num_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ако (groupi->defined)
            continue;

        CopyGroupInfo(groupi, group0);
    }

    key->groups = calloc(key->num_groups, sizeof(*key->groups));
    assert(darray_size(keyi->groups) == key->num_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < key->num_groups);
        #endif
        bool explicit_type = false;
        const struct xkb_key_type * const type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ако (type->num_levels < darray_size(groupi->levels)) {
            struct xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->num_levels,
                    KeyInfoText(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->num_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->num_levels);

        key->groups[i].explicit_type = explicit_type;
        key->groups[i].type = type;
    }

    /* Copy levels. */
    darray_enumerate(i, groupi, keyi->groups) {
        /*
         * Compute the capitalization transformation of the keysyms.
         * This is necessary because `xkb_state_key_get_syms()` returns an
         * immutable array and does not use a buffer, so we must store them.
         * We apply only simple capitalization rules, so the keysyms count is
         * unchanged.
         */
        struct xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            switch (leveli->num_syms) {
            case 0:
                leveli->upper = XKB_KEY_NoSymbol;
                break;
            case 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                break;
            default:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                за (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++) {
                    const xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    ако (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        break;
                    }
                }
                ако (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->num_syms *
                                             sizeof(*leveli->s.syms));
                    ако (!leveli->s.syms)
                        върни false; /* FIXME: better handling? */
                    за (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++) {
                        leveli->s.syms[leveli->num_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &key->groups[i].levels, NULL);
        ако (key->groups[i].type->num_levels > 1 ||
            key->groups[i].levels[0].num_syms > 0) {
            key->groups[i].explicit_symbols = true;
            key->explicit |= EXPLICIT_SYMBOLS;
        }
        ако (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            key->groups[i].explicit_actions = true;
            key->explicit |= EXPLICIT_INTERP;
        }
        ако (key->groups[i].explicit_type)
            key->explicit |= EXPLICIT_TYPES;
    }

    key->out_of_range_pending_group = keyi->out_of_range_pending_group;
    key->out_of_range_group_number = keyi->out_of_range_group_number;
    key->out_of_range_group_policy = keyi->out_of_range_group_policy;

key_fields:
    ако (keyi->defined & KEY_FIELD_VMODMAP) {
        key->vmodmap = keyi->vmodmap;
        key->explicit |= EXPLICIT_VMODMAP;
    }

    ако (keyi->repeat != KEY_REPEAT_UNDEFINED) {
        key->repeats = (keyi->repeat == KEY_REPEAT_YES);
        key->explicit |= EXPLICIT_REPEAT;
    }

    ако ((keyi->defined & KEY_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        key->overlays = keyi->overlays;
        ако (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = key->overlays;
            const struct xkb_key **overlays_keys = keyi->overlays_keys;
            докато (remaining) {
                /* isolate lowest set bit */
                const xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                ако (*overlays_keys) {
                    overlays_keys++;
                } иначе {
                    /* drop current null value (overlayN=none) */
                    key->overlays &= ~lsb;
                }
            }

            ако (key->overlays) {
                /* Steal */
                key->overlays_keys = keyi->overlays_keys;
                keyi->overlays_keys = NULL;
                keyi->overlays_alloc = 0;

                key->overlays_inline = false;
                key->explicit |= EXPLICIT_OVERLAY;
            }
        } иначе {
            key->overlay_key = keyi->overlay_key;
            key->overlays_inline = true;
            key->explicit |= EXPLICIT_OVERLAY;
        }
    }

    върни true;
}

static bool
CopyModMapDefToKeymap(struct xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    struct xkb_key *key;

    ако (!entry->haveSymbol) {
        key = XkbKeyByName(keymap, entry->u.keyName, true);
        ако (!key) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    KeyNameText(info->ctx, entry->u.keyName),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            върни false;
        }
    }
    иначе {
        key = FindKeyForSymbol(keymap, entry->u.keySym);
        ако (!key) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_SYMBOL,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.keySym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            върни false;
        }
    }

    /* Skip modMap None */
    ако (entry->mods) {
        key->modmap |= entry->mods;
        entry->keyCode = key->keycode;
    }

    върни true;
}

static bool
CopySymbolsToKeymap(struct xkb_keymap *keymap, SymbolsInfo *info)
{
    KeyInfo *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->num_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, NULL);

    darray_foreach(keyi, info->keys)
        ако (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->errorCount++;

    ако (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        struct xkb_key *key;

        xkb_keys_foreach(key, keymap) {
            ако (key->name == XKB_ATOM_NONE)
                continue;

            ако (key->num_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         KeyNameText(info->ctx, key->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        ако (!CopyModMapDefToKeymap(keymap, info, mm))
            info->errorCount++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->num_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    върни true;
}

bool
CompileSymbols(XkbFile *file, struct xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ако (file !=NULL)
        HandleSymbolsFile(&info, file);

    ако (info.errorCount != 0)
        goto err_info;

    ако (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearSymbolsInfo(&info);
    върни true;

err_info:
    ClearSymbolsInfo(&info);
    върни false;
}
