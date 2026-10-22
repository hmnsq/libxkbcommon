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

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-keysyms.h"

#ضمن "action.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "keymap.h"
#ضمن "keysym.h"
#ضمن "messages-codes.h"
#ضمن "text.h"
#ضمن "utils.h"
#ضمن "util-numbers.h"
#ضمن "util-mem.h"
#ضمن "vmod.h"
#ضمن "xkbcomp-priv.h"
#ضمن "xkbcomp/ast.h"

تعداد key_repeat {
    KEY_REPEAT_UNDEFINED = 0,
    KEY_REPEAT_YES,
    KEY_REPEAT_NO,
    _KEY_REPEAT_NUM_ENTRIES,
};
تعداد { KEY_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(_KEY_REPEAT_NUM_ENTRIES - 1 <=
              (1 << (KEY_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

تعداد group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

تعداد key_field {
    KEY_FIELD_NONE = 0,
    KEY_FIELD_REPEAT = (1 << 0),
    KEY_FIELD_DEFAULT_TYPE = (1 << 1),
    KEY_FIELD_GROUPINFO = (1 << 2),
    KEY_FIELD_VMODMAP = (1 << 3),
    KEY_FIELD_OVERLAY = (1 << 4),
    KEY_FIELD_ALL = KEY_FIELD_REPEAT | KEY_FIELD_DEFAULT_TYPE
                  | KEY_FIELD_GROUPINFO | KEY_FIELD_VMODMAP | KEY_FIELD_OVERLAY,
};
تعداد { KEY_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(KEY_FIELD_ALL <= (1 << (KEY_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

تعريفنوع بنية {
    darray(بنية xkb_level) levels;
    تعداد group_field defined;
    xkb_atom_t type;
} GroupInfo;

تعداد {
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

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_number;
    darray(GroupInfo) groups;
    تعداد xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد key_field defined:KEYINFO_DEFINED_BIT_FIELD;
    تعداد merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    تعداد key_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    اتحاد {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ثابت بنية xkb_key *overlay_key;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_key **overlays_keys;
    };
} KeyInfo;

ساكن فراغ
StealLevelInfo(بنية xkb_level *into, بنية xkb_level *from)
{
    clear_level(into);
    ܐܢ (from->num_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ܐܚܪܢܐ {
        into->s.sym = from->s.sym;
    }
    into->num_syms = from->num_syms;
    from->num_syms = 0;

    ܐܢ (from->num_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ܐܚܪܢܐ {
        into->a.action = from->a.action;
    }
    into->num_actions = from->num_actions;
    from->num_actions = 0;
}

ساكن فراغ
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساكن فراغ
ClearGroupInfo(GroupInfo *groupi)
{
    بنية xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

ساكن فراغ
CopyGroupInfo(GroupInfo *to, ثابت GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    لكل (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        ܐܢ (darray_item(from->levels, j).num_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).num_syms,
                       حجم(xkb_keysym_t));
        }
        ܐܢ (darray_item(from->levels, j).num_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).num_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساكن فراغ
InitKeyInfo(بنية xkb_context *ctx, KeyInfo *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_KEY_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_KEY_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_key = عدم;
}

ساكن فراغ
ClearKeyInfo(KeyInfo *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    ܐܢ (keyi->overlays_alloc)
        حرر(keyi->overlays_keys);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح errorCount;
    unsigned صحيح include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(KeyInfo) keys;
    KeyInfo default_key;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    بنية xkb_mod_set mods;

    بنية xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت بنية xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساكن فراغ
InitSymbolsInfo(SymbolsInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                unsigned صحيح include_depth, ثابت بنية xkb_mod_set *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    InitKeyInfo(keymap_info->keymap.ctx, &info->default_key);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    KeyInfo *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->keys)
        ClearKeyInfo(keyi);
    darray_free(info->keys);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    ClearKeyInfo(&info->default_key);
}

ساكن ثابت حرف *
KeyInfoText(SymbolsInfo *info, KeyInfo *keyi)
{
    ܦܢܐ KeyNameText(info->ctx, keyi->name);
}

ساكن bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t key_name)
{
    /* First find the type of the merged group. */
    ܐܢ (into->type != from->type) {
        ܐܢ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ܐܚܪܢܐ ܐܢ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ܐܚܪܢܐ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ܐܢ (report) {
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

    ܐܢ (darray_empty(from->levels)) {
        InitGroupInfo(from);
        ܦܢܐ true;
    }

    ܐܢ (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        ܦܢܐ true;
    }

    /* Merge the actions and syms. */
    ثابت darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t fromKeysymsCount = 0;
    darray_size_t fromActionsCount = 0;
    لكل (darray_size_t i = 0; i < levels_in_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_item(into->levels, i);
        بنية xkb_level* ثابت fromLevel = &darray_item(from->levels, i);

        ثابت bool fromHasNoKeysym = fromLevel->num_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->num_actions == 0;
        ܐܢ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            استمر;
        }

        ثابت bool intoHasNoKeysym = intoLevel->num_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->num_actions == 0;
        ܐܢ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            fromKeysymsCount++;
            fromActionsCount++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->num_syms > 0 || intoLevel->num_actions > 0);
        أكد(fromLevel->num_syms > 0 || fromLevel->num_actions > 0);

        /* Handle keysyms */
        ܐܢ (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ܐܢ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_SYMBOL,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, KeyNameText(info->ctx, key_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ܐܢ (fromHasNoKeysym) {
                /* No keysym to copy */
            } ܐܚܪܢܐ ܐܢ (clobber) {
                /* Override: copy any defined keysym from `from` */
                ܐܢ (unlikely(fromLevel->num_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ܐܢ (unlikely(intoLevel->num_syms > 1))
                        حرر(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->num_syms = fromLevel->num_syms;
                    fromLevel->num_syms = 0;
                    fromKeysymsCount++;
                } ܐܚܪܢܐ ܐܢ (fromLevel->s.sym != XKB_KEY_NoSymbol) {
                    /* Single defined keysym */
                    ܐܢ (unlikely(intoLevel->num_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->num_syms = 1;
                    fromKeysymsCount++;
                }
            } ܐܚܪܢܐ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ܐܢ (unlikely(intoLevel->num_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ܐܚܪܢܐ ܐܢ (intoLevel->s.sym == XKB_KEY_NoSymbol) {
                    /* Single undefined keysym */
                    ܐܢ (unlikely(fromLevel->num_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->num_syms = fromLevel->num_syms;
                        fromLevel->num_syms = 0;
                    } ܐܚܪܢܐ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->num_syms = 1;
                    }
                    fromKeysymsCount++;
                }
            }
        }

        /* Handle actions */
        ܐܢ (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ܐܢ (report && !(intoHasNoAction || fromHasNoAction)) {
                ܐܢ (unlikely(intoLevel->num_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_KEY_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, KeyNameText(info->ctx, key_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ܐܚܪܢܐ {
                    اتحاد xkb_action *use, *ignore;
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
            ܐܢ (fromHasNoAction) {
                /* No action to copy */
            } ܐܚܪܢܐ ܐܢ (clobber) {
                /* Override: copy any defined action from `from` */
                ܐܢ (unlikely(fromLevel->num_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ܐܢ (unlikely(intoLevel->num_actions > 1))
                        حرر(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->num_actions = fromLevel->num_actions;
                    fromLevel->num_actions = 0;
                    fromActionsCount++;
                } ܐܚܪܢܐ ܐܢ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ܐܢ (unlikely(intoLevel->num_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->num_actions = 1;
                    fromActionsCount++;
                }
            } ܐܚܪܢܐ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ܐܢ (unlikely(intoLevel->num_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ܐܚܪܢܐ ܐܢ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ܐܢ (unlikely(fromLevel->num_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->num_actions = fromLevel->num_actions;
                        fromLevel->num_actions = 0;
                    } ܐܚܪܢܐ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->num_actions = 1;
                    }
                    fromActionsCount++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    بنية xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->num_syms = 0;
        level->num_actions = 0;
        fromKeysymsCount++;
        fromActionsCount++;
    }
    ܐܢ (fromKeysymsCount) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ܐܢ (fromKeysymsCount == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    ܐܢ (fromActionsCount) {
        /* Reset defined actions field if we used no action from `into` */
        ܐܢ (fromActionsCount == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    ܦܢܐ true;
}

ساكن bool
UseNewKeyField(تعداد key_field field, تعداد key_field old, تعداد key_field new,
               bool clobber, bool report, تعداد key_field *collide)
{
    ܐܢ (!(old & field))
        ܦܢܐ (new & field);

    ܐܢ (new & field) {
        ܐܢ (report)
            *collide |= field;

        ܦܢܐ clobber;
    }

    ܦܢܐ false;
}

ساكن bool
overlays_get(ثابت KeyInfo *info, xkb_overlay_index_t bit,
             ثابت بنية xkb_key **key_out)
{
    ܐܢ (bit >= (xkb_overlay_index_t)(حجم(xkb_overlay_mask_t) * CHAR_BIT))
        ܦܢܐ false;
    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    ܐܢ (!(info->overlays & mask)) ܦܢܐ false;
    ܐܢ (key_out) {
        ܐܢ (!info->overlays_alloc) {
            /* Stored inline */
            *key_out = info->overlay_key;
        } ܐܚܪܢܐ {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(low);
            *key_out = info->overlays_keys[index];
        }
    }
    ܦܢܐ true;
}

ساكن bool
overlays_insert(KeyInfo *keyi, xkb_overlay_index_t bit,
                ثابت بنية xkb_key *key)
{
    ܐܢ (bit >= (xkb_overlay_index_t)(حجم(xkb_overlay_mask_t) * CHAR_BIT))
        ܦܢܐ false;

    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    ܐܢ ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ܐܢ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_key = key;
            ܐܢ (!key)
                keyi->overlays_clear = true;
        } ܐܚܪܢܐ {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(low);
            keyi->overlays_keys[index] = key;
        }
        ܦܢܐ true;
    }

    /* Insert new element */
    ܐܢ (!keyi->overlays || (keyi->overlays_clear && !key)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_key = key;
        keyi->overlays |= mask;
        keyi->overlays_clear = !key;
    } ܐܚܪܢܐ ܐܢ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ثابت xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_key ** ثابت tmp = calloc(alloc, حجم(*tmp));
        ܐܢ (!tmp) ܦܢܐ false;

        /* Copy previous entries to the buffer */
        ܐܢ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(low);
            tmp[idx] = keyi->overlay_key;
        } ܐܚܪܢܐ {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(low);
        tmp[idx] = key;

        keyi->overlays_keys = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } ܐܚܪܢܐ {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ثابت xkb_overlay_index_t count =
            (xkb_overlay_index_t)popcount32(overlays);
        ܐܢ (count > keyi->overlays_alloc) {
            ثابت xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(count);
            أكد(alloc >= count);
            ثابت بنية xkb_key ** ثابت tmp = realloc(
                keyi->overlays_keys,
                (size_t)alloc * حجم(*keyi->overlays_keys)
            );
            ܐܢ (!tmp) ܦܢܐ false;
            keyi->overlays_keys = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(low);
        ܐܢ (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ܐܢ (index < count - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_keys + index + 1, keyi->overlays_keys + index,
                    (size_t)(count - 1u - index) * حجم(ثابت بنية xkb_key *));
        }
        keyi->overlays_keys[index] = key;
        keyi->overlays = overlays;
    }

    ܦܢܐ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
merge_overlays(SymbolsInfo *info, KeyInfo *into, KeyInfo *from,
               bool clobber, bool report, تعداد key_field *collide)
{
    ܐܢ (!(from->defined & KEY_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ܐܚܪܢܐ ܐܢ (!(into->defined & KEY_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_keys = from->overlays_keys;
        from->overlays_keys = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= KEY_FIELD_OVERLAY;
    } ܐܚܪܢܐ ܐܢ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ܐܚܪܢܐ {
        ܐܢ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_index_t count =
                (xkb_overlay_index_t)popcount32(result_mask);

            ܐܢ (count == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            KeyInfo *dest = into;
            KeyInfo *src = from;
            ܐܢ (into->overlays_alloc >= count &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ܐܚܪܢܐ ܐܢ (from->overlays_alloc >= count) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ܐܚܪܢܐ ܐܢ (count > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ܐܢ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ܐܢ (!dest->overlays_alloc) {
                    ثابت بنية xkb_key **tmp = calloc(count, حجم(*tmp));
                    ܐܢ (!tmp)
                        ܦܢܐ false;
                    ܐܢ (dest->overlay_key)
                        tmp[0] = dest->overlay_key;
                    dest->overlays_keys = tmp;
                    dest->overlays_alloc = count;
                } ܐܚܪܢܐ {
                    ثابت بنية xkb_key **tmp =
                        realloc(dest->overlays_keys,
                                count * حجم(*dest->overlays_keys));
                    ܐܢ (!tmp)
                        ܦܢܐ false;
                    dest->overlays_keys = tmp;
                    dest->overlays_alloc = count;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ثابت بنية xkb_key **src_keys = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_keys
                    : &src->overlay_key;
            ܟܡܐ (remaining) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                ܐܢ (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_key *src_key = (src_keys)
                    ? *(src_keys++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_key *dest_key = عدم;
                ثابت bool conflict = overlays_get(dest, bit, &dest_key);
                ܐܢ (conflict) {
                    ܐܢ (dest_key == src_key)
                       استمر;
                    ܐܚܪܢܐ ܐܢ (report)
                        *collide |= KEY_FIELD_OVERLAY;
                }

                ܐܢ ((!conflict || clobber) && !overlays_insert(dest, bit, src_key))
                    ܦܢܐ false;
            }

            ܐܢ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ܐܢ (into->overlays_alloc)
                    حرر(into->overlays_keys);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ܐܢ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_keys = from->overlays_keys;
                    from->overlays_keys = عدم;
                    from->overlays_alloc = 0;
                } ܐܚܪܢܐ {
                    into->overlay_key = from->overlay_key;
                }
            }
        } ܐܚܪܢܐ {
            /* Disjoint overlays */

            ܐܢ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_key == from->overlay_key) {
                /* Indentical: do nothing */
                ܦܢܐ true;
            }
            ܐܢ (!(into->overlays & from->overlays)) {
                ܐܢ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_key = from->overlay_key;
                    ܦܢܐ true;
                } ܐܚܪܢܐ ܐܢ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    ܦܢܐ true;
                }
            }
            /* Conflict */
            ܐܢ (report)
                *collide |= KEY_FIELD_OVERLAY;
            ܐܢ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_key = from->overlay_key;
            }
        }
    }
    ܦܢܐ true;
}

ساكن bool
MergeKeys(SymbolsInfo *info, KeyInfo *into, KeyInfo *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    تعداد key_field collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->merge != MERGE_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    ܐܢ (from->merge == MERGE_REPLACE) {
        ClearKeyInfo(into);
        *into = *from;
        InitKeyInfo(info->ctx, from);
        ܦܢܐ true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    لكل (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    لكل (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    ܐܢ (UseNewKeyField(KEY_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= KEY_FIELD_VMODMAP;
    }
    ܐܢ (UseNewKeyField(KEY_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= KEY_FIELD_REPEAT;
    }
    ܐܢ (UseNewKeyField(KEY_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= KEY_FIELD_DEFAULT_TYPE;
    }
    ܐܢ (UseNewKeyField(KEY_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_number = from->out_of_range_group_number;
        into->defined |= KEY_FIELD_GROUPINFO;
    }

    ܐܢ (!merge_overlays(info, into, from, clobber, report, &collide))
        ܦܢܐ false;

    ܐܢ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 KeyNameText(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    ClearKeyInfo(from);
    InitKeyInfo(info->ctx, from);
    ܦܢܐ true;
}

ساكن xkb_atom_t
XkbResolveKeyAlias(ثابت بنية xkb_keymap *keymap, xkb_atom_t name)
{
    ܐܢ (name < keymap->num_key_names) {
        ثابت KeycodeMatch match = keymap->key_names[name];
        ܐܢ (match.found && match.is_alias) {
            ܦܢܐ match.alias.real;
        }
    }
    ܦܢܐ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
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
        ܐܢ (iter->name == keyi->name)
            ܦܢܐ MergeKeys(info, iter, keyi, same_file);

    darray_append(info->keys, *keyi);
    InitKeyInfo(info->ctx, keyi);
    ܦܢܐ true;
}

ساكن bool
AddModMapEntry(SymbolsInfo *info, ثابت ModMapEntry *new)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        ܐܢ ((new->haveSymbol != old->haveSymbol) ||
            (new->haveSymbol && new->u.keySym != old->u.keySym) ||
            (!new->haveSymbol && new->u.keyName != old->u.keyName))
            استمر;

        ܐܢ (new->mods == old->mods)
            ܦܢܐ true;

        ثابت xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        ܐܢ (new->haveSymbol) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.keySym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } ܐܚܪܢܐ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeyNameText(info->ctx, new->u.keyName),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        ܦܢܐ true;
    }

    darray_append(info->modmaps, *new);

    ܦܢܐ true;
}

/***====================================================================***/

ساكن فراغ
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    ܐܢ (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ܦܢܐ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ܐܢ (into->name == عدم) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    لكل (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        ܐܢ (!darray_item(from->group_names, i))
            استمر;

        ܐܢ (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            استمر;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    ܐܢ (darray_empty(into->keys)) {
        into->keys = from->keys;
        darray_init(from->keys);
    }
    ܐܚܪܢܐ {
        KeyInfo *keyi;
        darray_foreach(keyi, from->keys) {
            keyi->merge = merge;
            ܐܢ (!AddKeySymbols(into, keyi, false))
                into->errorCount++;
        }
    }

    ܐܢ (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ܐܚܪܢܐ {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            ܐܢ (!AddModMapEntry(into, mm))
                into->errorCount++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ܐܢ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ܦܢܐ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, حجم(path));
        ܐܢ (!file) {
            info->errorCount += 10;
            ClearSymbolsInfo(&included);
            ܦܢܐ false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        ܐܢ (stmt->modifier) {
            ثابت صحيح count = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            ܐܢ (count <= 0 || *(stmt->modifier + count) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } ܐܚܪܢܐ {
                next_incl.explicit_group--;
            }
        }
        ܐܚܪܢܐ ܐܢ (info->keymap_info->keymap.num_groups != 0 &&
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
        ܐܚܪܢܐ {
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

    ܦܢܐ (info->errorCount == 0);
}

ساكن bool
GetGroupIndex(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
              تعداد group_field field, xkb_layout_index_t *ndx_rtrn)
{
    أكد (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ثابت حرف *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    ܐܢ (arrayNdx == عدم) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ܐܢ (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                ܦܢܐ true;
            }
        }

        ܐܢ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, KeyInfoText(info, keyi), info->max_groups, name);
            ܦܢܐ false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        ܦܢܐ true;
    }

    ܐܢ (ExprResolveGroup(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, KeyInfoText(info, keyi));
        ܦܢܐ false;
    }

    (*ndx_rtrn)--;
    ܐܢ (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    ܦܢܐ true;
}

ساكن bool
AddSymbolsToKey(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
                ExprDef *value)
{
    xkb_layout_index_t ndx = 0;

    ܐܢ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        ܦܢܐ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ܐܢ (value->common.type == STMT_EXPR_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        ܦܢܐ true;
    }

    ܐܢ (value->common.type != STMT_EXPR_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(value->common.type), ndx + 1,
                KeyInfoText(info, keyi));
        ܦܢܐ false;
    }

    ܐܢ (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_KEY_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                KeyInfoText(info, keyi), ndx + 1);
        ܦܢܐ false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (ExprKeysymList *keysymList = (ExprKeysymList *) value;
         keysymList;
         keysymList = (ExprKeysymList *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        ܐܢ (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    ܐܢ (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    ܐܢ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    لكل (ExprKeysymList *keysymList = (ExprKeysymList *) value;
         keysymList && level < nLevels;
         keysymList = (ExprKeysymList *) keysymList->common.next, level++) {
        بنية xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->num_syms == 0);

        ܐܢ (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    KeyInfoText(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            ܦܢܐ false;
        }

        leveli->num_syms = (xkb_keysym_count_t) darray_size(keysymList->syms);
        بدل (leveli->num_syms) {
        حالة 0:
            leveli->s.sym = XKB_KEY_NoSymbol;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_KEY_NoSymbol);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++)
                أكد(leveli->s.syms[k] != XKB_KEY_NoSymbol);
#endif
        }
    }

    ܦܢܐ true;
}

ساكن bool
AddActionsToKey(SymbolsInfo *info, KeyInfo *keyi, ExprDef *arrayNdx,
                ExprDef *value)
{
    xkb_layout_index_t ndx = 0;

    ܐܢ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        ܦܢܐ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ܐܢ (value->common.type == STMT_EXPR_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        ܦܢܐ true;
    }

    ܐܢ (value->common.type != STMT_EXPR_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 value->common.type, ndx, KeyInfoText(info, keyi));
        ܦܢܐ false;
    }

    ܐܢ (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_KEY_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 KeyInfoText(info, keyi), ndx);
        ܦܢܐ false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &value->common; p; p = p->next)
        nLevels++;
    ܐܢ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    لكل (ExprActionList *actionList = (ExprActionList *) value;
         actionList;
         actionList = (ExprActionList *) actionList->common.next, level++) {
        بنية xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->num_actions == 0);

        unsigned صحيح num_actions = 0;
        لكل (ExprDef *act = actionList->actions;
             act; act = (ExprDef *) act->common.next)
            num_actions++;

        ܐܢ (unlikely(num_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    KeyInfoText(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, num_actions);
            ܦܢܐ false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (ExprDef *act = actionList->actions;
             act; act = (ExprDef *) act->common.next) {
            اتحاد xkb_action toAct = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            ܐܢ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        KeyInfoText(info, keyi), ndx + 1, level + 1);
                ܐܢ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    ܦܢܐ false;
                } ܐܚܪܢܐ {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            ܐܢ (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ܐܚܪܢܐ ܐܢ (likely(num_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->num_actions = 1;
                leveli->a.action = toAct;
                اذهب next_level;
            } ܐܚܪܢܐ {
                darray_append(actions, toAct);
            }
        }
        ܐܢ (darray_empty(actions)) {
            leveli->num_actions = 0;
        } ܐܚܪܢܐ ܐܢ (likely(darray_size(actions) > 1)) {
            leveli->num_actions = (xkb_action_count_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_count_t k = 0; k < leveli->num_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } ܐܚܪܢܐ {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(num_actions > darray_size(actions));
            leveli->num_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ܐܢ (leveli->num_actions > 0 || leveli->num_syms > 0)
            nonEmptyLevels = level + 1;
    }

    ܐܢ (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        ܐܢ (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        ܐܚܪܢܐ
            darray_free(groupi->levels);
    }

    ܦܢܐ true;
}

ساكن ثابت LookupEntry repeatEntries[] = {
    { "true", KEY_REPEAT_YES },
    { "yes", KEY_REPEAT_YES },
    { "on", KEY_REPEAT_YES },
    { "false", KEY_REPEAT_NO },
    { "no", KEY_REPEAT_NO },
    { "off", KEY_REPEAT_NO },
    { "default", KEY_REPEAT_UNDEFINED },
    { عدم, 0 }
};

/** Handle overlay<N> = <key> entry */
ساكن bool
ExprResolveOverlayEntry(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *field, ثابت ExprDef *arrayNdx,
                        ثابت ExprDef *expr, KeyInfo *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ثابت بنية xkb_key **key_rtrn)
{
    /* Overlay index */
    ܐܢ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
        ܦܢܐ false;
    }
    static_assert(حجم(xkb_overlay_index_t) == حجم(uint8_t), "");
    ثابت size_t prefix = حجم("overlay") - 1;
    ثابت size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ܐܢ (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        ܦܢܐ false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    بدل (expr->common.type) {
    حالة STMT_EXPR_KEYNAME_LITERAL:
        *key_rtrn = XkbKeyByName(&keymap_info->keymap, expr->key_name.key_name,
                                 false);
        ܐܢ (!*key_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, expr->key_name.key_name),
                    field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
            ܦܢܐ false;
        }
        ܦܢܐ true;
    حالة STMT_EXPR_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_text(keymap_info->keymap.ctx, expr->ident.ident);
        ܐܢ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *key_rtrn = عدم;
            ܦܢܐ true;
        } ܐܚܪܢܐ ܐܢ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *key_rtrn = عدم;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            ܦܢܐ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_VALUE,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, KeyNameText(keymap_info->keymap.ctx, keyi->name));
        ܦܢܐ false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_VALUE,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_EXPR_KEYNAME_LITERAL),
                field, KeyNameText(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(expr->common.type));
        ܦܢܐ false;
    }
}

ساكن bool
SetSymbolsField(SymbolsInfo *info, KeyInfo *keyi, ثابت حرف *field,
                ExprDef *arrayNdx, ExprDef **value_ptr)
{
    ExprDef * restrict ثابت value = *value_ptr;
    ܐܢ (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ܐܢ (!ExprResolveString(info->ctx, value, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            ܦܢܐ false;
        }

        ܐܢ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= KEY_FIELD_DEFAULT_TYPE;
        }
        ܐܚܪܢܐ ܐܢ (ExprResolveGroup(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }
        ܐܚܪܢܐ {
            ndx--;
            ܐܢ (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "symbols")) {
        ܦܢܐ AddSymbolsToKey(info, keyi, arrayNdx, value);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "actions")) {
        ܦܢܐ AddActionsToKey(info, keyi, arrayNdx, value);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        ܐܢ (!ExprResolveModMask(info->ctx, value, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(value->common.type),
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= KEY_FIELD_VMODMAP;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                KeyInfoText(info, keyi));
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                KeyInfoText(info, keyi));
    }
    ܐܚܪܢܐ ܐܢ (istreq_prefix("permanentoverlay", field)) {
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
    ܐܚܪܢܐ ܐܢ (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_key *key = عدم;
        ܐܢ (!ExprResolveOverlayEntry(info->keymap_info, field, arrayNdx,
                                     *value_ptr, keyi, &overlay, &key))
            ܦܢܐ false;
        ܐܢ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            ܦܢܐ true;
        } ܐܚܪܢܐ ܐܢ (key && key->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, KeyInfoText(info, keyi));
        } ܐܚܪܢܐ {
            ثابت بنية xkb_key *prev;
            ܐܢ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ܐܢ (key != prev) {
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
            } ܐܚܪܢܐ ܐܢ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ܐܢ (!overlays_insert(keyi, overlay, key))
                    ܦܢܐ false;
                keyi->defined |= KEY_FIELD_OVERLAY;
            } ܐܚܪܢܐ {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                ܐܢ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ܐܢ (key) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_key = key;
                    } ܐܚܪܢܐ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= KEY_FIELD_OVERLAY;
                } ܐܚܪܢܐ ܐܢ (keyi->overlays) {
                    ܐܢ (key) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                KeyInfoText(info, keyi), keyi->overlays,
                                KeyNameText(info->ctx, keyi->overlay_key->name),
                                overlay + 1, KeyNameText(info->ctx, key->name));
                        ܦܢܐ !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_VALUE_MISMATCH);
                    } ܐܚܪܢܐ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        ܐܢ (!ExprResolveEnum(info->ctx, value, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }

        keyi->repeat = (تعداد key_repeat) val;
        keyi->defined |= KEY_FIELD_REPEAT;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        ܐܢ (!ExprResolveBoolean(info->ctx, value, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        ܐܢ (!ExprResolveBoolean(info->ctx, value, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_VALUE,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ܐܢ (ExprResolveGroup(info->keymap_info, value, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    KeyInfoText(info, keyi));
            ܦܢܐ false;
        }

        ܐܢ (pending) {
            keyi->out_of_range_pending_group = true;
            ثابت darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .expr = *value_ptr,
                    .computed = false,
                    .value = 0,
                }
            );
            *value_ptr = عدم;
            static_assert(حجم(keyi->out_of_range_group_number) ==
                          حجم(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_number = pending_index;
        } ܐܚܪܢܐ {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_number = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= KEY_FIELD_GROUPINFO;
    }
    ܐܚܪܢܐ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_KEY_FIELDS);
    }

    ܦܢܐ true;
}

ساكن bool
SetGroupName(SymbolsInfo *info, ExprDef *arrayNdx, ExprDef *value,
             تعداد merge_mode merge)
{
    ܐܢ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        ܦܢܐ false;
    }

    xkb_layout_index_t group = 0;
    ܐܢ (ExprResolveGroup(info->keymap_info, arrayNdx, false, &group, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        ܦܢܐ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ܐܢ (!ExprResolveString(info->ctx, value, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        ܦܢܐ false;
    }

    xkb_layout_index_t group_to_use;
    ܐܢ (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    ܐܚܪܢܐ ܐܢ (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    ܐܚܪܢܐ {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        ܦܢܐ false;
    }

    ܐܢ (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } ܐܚܪܢܐ {
        ثابت xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        ܐܢ (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (merge != MERGE_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
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

    ܦܢܐ true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    ExprDef *arrayNdx;
    bool ret;

    ܐܢ (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ܦܢܐ false;

    ܐܢ (elem && istreq(elem, "key")) {
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
    ܐܚܪܢܐ ܐܢ (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->value, stmt->merge);
    }
    ܐܚܪܢܐ ܐܢ (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ܐܚܪܢܐ ܐܢ (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ܐܚܪܢܐ ܐܢ (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ܐܚܪܢܐ ܐܢ (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ܐܚܪܢܐ ܐܢ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->value, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } ܐܚܪܢܐ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ܦܢܐ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    ܦܢܐ ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, KeyInfo *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.next) {
        ثابت حرف *field;
        ExprDef *arrayNdx;
        bool ok = true;

        ܐܢ (!def->name) {
            ܐܢ (unlikely(!def->value) || def->value->common.type != STMT_EXPR_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            ܐܚܪܢܐ
                field = "actions";
            arrayNdx = عدم;
        }
        ܐܚܪܢܐ {
            ثابت حرف *elem;
            ok = ExprResolveLhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            ܐܢ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        ܐܢ (unlikely(!def->value)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        ܐܢ (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->value))
            all_valid_entries = false;
    }

    ܦܢܐ all_valid_entries;
}

ساكن bool
SetExplicitGroup(SymbolsInfo *info, KeyInfo *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    ܐܢ (info->explicit_group == XKB_LAYOUT_INVALID)
        ܦܢܐ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ܐܢ (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    ܐܢ (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, KeyInfoText(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    ܐܢ (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    ܦܢܐ true;
}

ساكن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    KeyInfo keyi;

    keyi = info->default_key;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_key.groups);
    لكل (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_key.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->keyName;

    ܐܢ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        AddKeySymbols(info, &keyi, true)) {
        ܦܢܐ true;
    }

    ClearKeyInfo(&keyi);
    info->errorCount++;
    ܦܢܐ false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    بنية xkb_context *ctx = info->ctx;

    ܐܢ (!ExprResolveModMask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        ܦܢܐ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_EXPR_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    لكل (ExprDef *key = def->keys; key; key = (ExprDef *) key->common.next) {
        ܐܢ (key->common.type == STMT_EXPR_KEYNAME_LITERAL) {
            tmp.haveSymbol = false;
            tmp.u.keyName = key->key_name.key_name;
        }
        ܐܚܪܢܐ ܐܢ (key->common.type == STMT_EXPR_KEYSYM_LITERAL) {
            ܐܢ (key->keysym.keysym == XKB_KEY_NoSymbol) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } ܐܚܪܢܐ {
                tmp.haveSymbol = true;
                tmp.u.keySym = key->keysym.keysym;
            }
        }
        ܐܚܪܢܐ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    ܦܢܐ ok;
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_safe(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            اكسر;
        حالة STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            اكسر;
        حالة STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            اكسر;
        حالة STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            اكسر;
        }

        ܐܢ (!ok)
            info->errorCount++;

        ܐܢ (info->errorCount > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
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
ساكن بنية xkb_key *
FindKeyForSymbol(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    ܥܒܕ {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        ܥܒܕ {
            got_one_level = false;
            بنية xkb_key *key;
            xkb_keys_foreach(key, keymap) {
                ܐܢ (group < key->num_groups &&
                    level < XkbKeyNumLevels(key, group)) {
                    got_one_group = got_one_level = true;
                    ثابت xkb_keysym_count_t num_syms =
                        key->groups[group].levels[level].num_syms;
                    ܐܢ (num_syms > 1) {
                        لكل (xkb_keysym_count_t k = 0; k < num_syms; k++) {
                            ܐܢ (key->groups[group].levels[level].s.syms[k] == sym)
                                ܦܢܐ key;
                        }
                    } ܐܚܪܢܐ ܐܢ (num_syms &&
                               key->groups[group].levels[level].s.sym == sym) {
                        ܦܢܐ key;
                    }
                }
            }
            level++;
        } ܟܡܐ (got_one_level);
        group++;
    } ܟܡܐ (got_one_group);

    ܦܢܐ عدم;
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
ساكن xkb_atom_t
FindAutomaticType(بنية xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).num_syms == 0 ? \
        XKB_KEY_NoSymbol : \
     darray_item(groupi->levels, level).num_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    ܐܢ (width == 1 || width <= 0)
        ܦܢܐ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ܐܢ (width == 2) {
        ܐܢ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            ܦܢܐ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ܐܢ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ܦܢܐ xkb_atom_intern_literal(ctx, "KEYPAD");

        ܦܢܐ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ܐܢ (width <= 4) {
        ܐܢ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_KEY_NoSymbol);

            ܐܢ (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                ܦܢܐ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            ܦܢܐ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ܐܢ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ܦܢܐ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        ܦܢܐ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    ܦܢܐ XKB_ATOM_NONE;

#undef GET_SYM
}

ساكن ثابت بنية xkb_key_type *
FindTypeForGroup(بنية xkb_keymap *keymap, KeyInfo *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ܐܢ (type_name == XKB_ATOM_NONE) {
        ܐܢ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ܐܚܪܢܐ {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            ܐܢ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ܐܢ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_KEY_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 KeyNameText(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        اذهب use_default;
    }

    darray_size_t i;
    لكل (i = 0; i < keymap->num_types; i++)
        ܐܢ (keymap->types[i].name == type_name)
            اكسر;

    ܐܢ (i >= keymap->num_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_KEY_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 KeyNameText(keymap->ctx, keyi->name), group + 1);
        اذهب use_default;
    }

    keymap->types[i].required = true;
    ܦܢܐ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    ܦܢܐ &keymap->types[0];
}

ساكن bool
CopySymbolsDefToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                       KeyInfo *keyi)
{
    بنية xkb_key *key;
    GroupInfo *groupi;
    ثابت GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    key = XkbKeyByName(keymap, keyi->name, false);
    ܐܢ (!key) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                KeyInfoText(info, keyi));
        ܦܢܐ false;
    }

    /* Find the range of groups we need. */
    key->num_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & KEY_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        ܐܢ (darray_size(groupi->levels) > 0 || has_explicit_type)
            key->num_groups = i + 1;
        ܐܢ (has_explicit_type)
            key->explicit |= EXPLICIT_TYPES;
    }

    ܐܢ (key->num_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ܐܢ (keyi->defined)
            اذهب key_fields;
        ܐܚܪܢܐ
            ܦܢܐ false;
    }

    darray_resize(keyi->groups, key->num_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ܐܢ (groupi->defined)
            استمر;

        CopyGroupInfo(groupi, group0);
    }

    key->groups = calloc(key->num_groups, حجم(*key->groups));
    أكد(darray_size(keyi->groups) == key->num_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < key->num_groups);
        #endif
        bool explicit_type = false;
        ثابت بنية xkb_key_type * ثابت type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ܐܢ (type->num_levels < darray_size(groupi->levels)) {
            بنية xkb_level *leveli;

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
        بنية xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            بدل (leveli->num_syms) {
            حالة 0:
                leveli->upper = XKB_KEY_NoSymbol;
                اكسر;
            حالة 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                لكل (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++) {
                    ثابت xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    ܐܢ (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        اكسر;
                    }
                }
                ܐܢ (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->num_syms *
                                             حجم(*leveli->s.syms));
                    ܐܢ (!leveli->s.syms)
                        ܦܢܐ false; /* FIXME: better handling? */
                    لكل (xkb_keysym_count_t k = 0; k < leveli->num_syms; k++) {
                        leveli->s.syms[leveli->num_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &key->groups[i].levels, عدم);
        ܐܢ (key->groups[i].type->num_levels > 1 ||
            key->groups[i].levels[0].num_syms > 0) {
            key->groups[i].explicit_symbols = true;
            key->explicit |= EXPLICIT_SYMBOLS;
        }
        ܐܢ (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            key->groups[i].explicit_actions = true;
            key->explicit |= EXPLICIT_INTERP;
        }
        ܐܢ (key->groups[i].explicit_type)
            key->explicit |= EXPLICIT_TYPES;
    }

    key->out_of_range_pending_group = keyi->out_of_range_pending_group;
    key->out_of_range_group_number = keyi->out_of_range_group_number;
    key->out_of_range_group_policy = keyi->out_of_range_group_policy;

key_fields:
    ܐܢ (keyi->defined & KEY_FIELD_VMODMAP) {
        key->vmodmap = keyi->vmodmap;
        key->explicit |= EXPLICIT_VMODMAP;
    }

    ܐܢ (keyi->repeat != KEY_REPEAT_UNDEFINED) {
        key->repeats = (keyi->repeat == KEY_REPEAT_YES);
        key->explicit |= EXPLICIT_REPEAT;
    }

    ܐܢ ((keyi->defined & KEY_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        key->overlays = keyi->overlays;
        ܐܢ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = key->overlays;
            ثابت بنية xkb_key **overlays_keys = keyi->overlays_keys;
            ܟܡܐ (remaining) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                ܐܢ (*overlays_keys) {
                    overlays_keys++;
                } ܐܚܪܢܐ {
                    /* drop current null value (overlayN=none) */
                    key->overlays &= ~lsb;
                }
            }

            ܐܢ (key->overlays) {
                /* Steal */
                key->overlays_keys = keyi->overlays_keys;
                keyi->overlays_keys = عدم;
                keyi->overlays_alloc = 0;

                key->overlays_inline = false;
                key->explicit |= EXPLICIT_OVERLAY;
            }
        } ܐܚܪܢܐ {
            key->overlay_key = keyi->overlay_key;
            key->overlays_inline = true;
            key->explicit |= EXPLICIT_OVERLAY;
        }
    }

    ܦܢܐ true;
}

ساكن bool
CopyModMapDefToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    بنية xkb_key *key;

    ܐܢ (!entry->haveSymbol) {
        key = XkbKeyByName(keymap, entry->u.keyName, true);
        ܐܢ (!key) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    KeyNameText(info->ctx, entry->u.keyName),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ܦܢܐ false;
        }
    }
    ܐܚܪܢܐ {
        key = FindKeyForSymbol(keymap, entry->u.keySym);
        ܐܢ (!key) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_SYMBOL,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.keySym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ܦܢܐ false;
        }
    }

    /* Skip modMap None */
    ܐܢ (entry->mods) {
        key->modmap |= entry->mods;
        entry->keyCode = key->keycode;
    }

    ܦܢܐ true;
}

ساكن bool
CopySymbolsToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    KeyInfo *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->num_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, عدم);

    darray_foreach(keyi, info->keys)
        ܐܢ (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->errorCount++;

    ܐܢ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_key *key;

        xkb_keys_foreach(key, keymap) {
            ܐܢ (key->name == XKB_ATOM_NONE)
                استمر;

            ܐܢ (key->num_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         KeyNameText(info->ctx, key->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        ܐܢ (!CopyModMapDefToKeymap(keymap, info, mm))
            info->errorCount++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->num_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    ܦܢܐ true;
}

bool
CompileSymbols(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ܐܢ (file !=عدم)
        HandleSymbolsFile(&info, file);

    ܐܢ (info.errorCount != 0)
        اذهب err_info;

    ܐܢ (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    ܦܢܐ true;

err_info:
    ClearSymbolsInfo(&info);
    ܦܢܐ false;
}
