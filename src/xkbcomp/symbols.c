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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-keysyms.h"

#અન્તર્ભાવ "action.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-numbers.h"
#અન્તર્ભાવ "util-mem.h"
#અન્તર્ભાવ "vmod.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "xkbcomp/ast.h"

ગણના ચાવી_repeat {
    ચાવી_REPEAT_UNDEFINED = 0,
    ચાવી_REPEAT_YES,
    ચાવી_REPEAT_NO,
    ચાવી_REPEAT_અંક_ENTRIES,
};
ગણના { ચાવી_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(ચાવી_REPEAT_અંક_ENTRIES - 1 <=
              (1 << (ચાવી_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

ગણના group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

ગણના ચાવી_field {
    ચાવી_FIELD_NONE = 0,
    ચાવી_FIELD_REPEAT = (1 << 0),
    ચાવી_FIELD_DEFAULT_TYPE = (1 << 1),
    ચાવી_FIELD_GROUPINFO = (1 << 2),
    ચાવી_FIELD_VMODMAP = (1 << 3),
    ચાવી_FIELD_OVERLAY = (1 << 4),
    ચાવી_FIELD_ALL = ચાવી_FIELD_REPEAT | ચાવી_FIELD_DEFAULT_TYPE
                  | ચાવી_FIELD_GROUPINFO | ચાવી_FIELD_VMODMAP | ચાવી_FIELD_OVERLAY,
};
ગણના { ચાવી_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(ચાવી_FIELD_ALL <= (1 << (ચાવી_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

પ્રકારવ્યાખ્યા સંરચના {
    darray(સંરચના xkb_level) levels;
    ગણના group_field defined;
    xkb_atom_t type;
} GroupInfo;

ગણના {
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
              (unsigned)ચાવી_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)ચાવી_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

પ્રકારવ્યાખ્યા સંરચના {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_સંખ્યા;
    darray(GroupInfo) groups;
    ગણના xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    ગણના ચાવી_field defined:KEYINFO_DEFINED_BIT_FIELD;
    ગણના merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    ગણના ચાવી_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    સંઘ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        અચલ સંરચના xkb_ચાવી *overlay_ચાવી;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        અચલ સંરચના xkb_ચાવી **overlays_ચાવીઓ;
    };
} ચાવી_Info;

સ્થિર શૂન્ય
StealLevelInfo(સંરચના xkb_level *into, સંરચના xkb_level *from)
{
    clear_level(into);
    જો (from->અંક_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } નહીંતર {
        into->s.sym = from->s.sym;
    }
    into->અંક_syms = from->અંક_syms;
    from->અંક_syms = 0;

    જો (from->અંક_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } નહીંતર {
        into->a.action = from->a.action;
    }
    into->અંક_actions = from->અંક_actions;
    from->અંક_actions = 0;
}

સ્થિર શૂન્ય
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, કદ(*groupi));
}

સ્થિર શૂન્ય
ClearGroupInfo(GroupInfo *groupi)
{
    સંરચના xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

સ્થિર શૂન્ય
CopyGroupInfo(GroupInfo *to, અચલ GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    માટે (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        જો (darray_item(from->levels, j).અંક_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).અંક_syms,
                       કદ(xkb_keysym_t));
        }
        જો (darray_item(from->levels, j).અંક_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).અંક_actions,
                       કદ(સંઘ xkb_action));
        }
    }
}

સ્થિર શૂન્ય
Init_ચાવી_Info(સંરચના xkb_context *ctx, ચાવી_Info *keyi)
{
    memset(keyi, 0, કદ(*keyi));
    static_assert(!DEFAULT_ચાવી_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ચાવી_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ચાવી = ખાલી;
}

સ્થિર શૂન્ય
Clear_ચાવી_Info(ચાવી_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    જો (keyi->overlays_alloc)
        મુક્ત(keyi->overlays_ચાવીઓ);
}

/***====================================================================***/

પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર *name;         /* e.g. pc+us+inet(evdev) */
    પૂર્ણાંક error_ગણતરી;
    unsigned પૂર્ણાંક include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(ચાવી_Info) ચાવીઓ;
    ચાવી_Info default_ચાવી;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    સંરચના xkb_mod_set mods;

    સંરચના xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    અચલ સંરચના xkb_keymap_info *keymap_info;
} SymbolsInfo;

સ્થિર શૂન્ય
InitSymbolsInfo(SymbolsInfo *info, અચલ સંરચના xkb_keymap_info *keymap_info,
                unsigned પૂર્ણાંક include_depth, અચલ સંરચના xkb_mod_set *mods)
{
    memset(info, 0, કદ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ચાવી_Info(keymap_info->keymap.ctx, &info->default_ચાવી);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

સ્થિર શૂન્ય
ClearSymbolsInfo(SymbolsInfo *info)
{
    ચાવી_Info *keyi;
    મુક્ત(info->name);
    darray_foreach(keyi, info->ચાવીઓ)
        Clear_ચાવી_Info(keyi);
    darray_free(info->ચાવીઓ);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_ચાવી_Info(&info->default_ચાવી);
}

સ્થિર અચલ અક્ષર *
ચાવી_Info_Text(SymbolsInfo *info, ચાવી_Info *keyi)
{
    પાછા ચાવી_Name_Text(info->ctx, keyi->name);
}

સ્થિર bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t ચાવી_name)
{
    /* First find the type of the merged group. */
    જો (into->type != from->type) {
        જો (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        નહીંતર જો (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        નહીંતર {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            જો (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, ચાવી_Name_Text(info->ctx, ચાવી_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    જો (darray_empty(from->levels)) {
        InitGroupInfo(from);
        પાછા true;
    }

    જો (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        પાછા true;
    }

    /* Merge the actions and syms. */
    અચલ darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_ગણતરી = 0;
    darray_size_t from_Actions_ગણતરી = 0;
    માટે (darray_size_t i = 0; i < levels_in_both; i++) {
        સંરચના xkb_level* અચલ intoLevel = &darray_item(into->levels, i);
        સંરચના xkb_level* અચલ fromLevel = &darray_item(from->levels, i);

        અચલ bool fromHasNoKeysym = fromLevel->અંક_syms == 0;
        અચલ bool fromHasNoAction = fromLevel->અંક_actions == 0;
        જો (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ચાલુ;
        }

        અચલ bool intoHasNoKeysym = intoLevel->અંક_syms == 0;
        અચલ bool intoHasNoAction = intoLevel->અંક_actions == 0;
        જો (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ગણતરી++;
            from_Actions_ગણતરી++;
            ચાલુ;
        }

        /* Possible level conflict */
        પ્રતિજ્ઞા(intoLevel->અંક_syms > 0 || intoLevel->અંક_actions > 0);
        પ્રતિજ્ઞા(fromLevel->અંક_syms > 0 || fromLevel->અંક_actions > 0);

        /* Handle keysyms */
        જો (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            જો (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_પ્રતીક,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, ચાવી_Name_Text(info->ctx, ચાવી_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            જો (fromHasNoKeysym) {
                /* No keysym to copy */
            } નહીંતર જો (clobber) {
                /* Override: copy any defined keysym from `from` */
                જો (unlikely(fromLevel->અંક_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    જો (unlikely(intoLevel->અંક_syms > 1))
                        મુક્ત(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->અંક_syms = fromLevel->અંક_syms;
                    fromLevel->અંક_syms = 0;
                    from_Keysyms_ગણતરી++;
                } નહીંતર જો (fromLevel->s.sym != XKB_ચાવી_No_પ્રતીક) {
                    /* Single defined keysym */
                    જો (unlikely(intoLevel->અંક_syms > 1))
                        મુક્ત(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->અંક_syms = 1;
                    from_Keysyms_ગણતરી++;
                }
            } નહીંતર {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                જો (unlikely(intoLevel->અંક_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } નહીંતર જો (intoLevel->s.sym == XKB_ચાવી_No_પ્રતીક) {
                    /* Single undefined keysym */
                    જો (unlikely(fromLevel->અંક_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->અંક_syms = fromLevel->અંક_syms;
                        fromLevel->અંક_syms = 0;
                    } નહીંતર {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->અંક_syms = 1;
                    }
                    from_Keysyms_ગણતરી++;
                }
            }
        }

        /* Handle actions */
        જો (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            જો (report && !(intoHasNoAction || fromHasNoAction)) {
                જો (unlikely(intoLevel->અંક_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ચાવી_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, ચાવી_Name_Text(info->ctx, ચાવી_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } નહીંતર {
                    સંઘ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                ચાવી_Name_Text(info->ctx, ચાવી_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            જો (fromHasNoAction) {
                /* No action to copy */
            } નહીંતર જો (clobber) {
                /* Override: copy any defined action from `from` */
                જો (unlikely(fromLevel->અંક_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    જો (unlikely(intoLevel->અંક_actions > 1))
                        મુક્ત(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->અંક_actions = fromLevel->અંક_actions;
                    fromLevel->અંક_actions = 0;
                    from_Actions_ગણતરી++;
                } નહીંતર જો (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    જો (unlikely(intoLevel->અંક_actions > 1))
                        મુક્ત(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->અંક_actions = 1;
                    from_Actions_ગણતરી++;
                }
            } નહીંતર {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                જો (unlikely(intoLevel->અંક_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } નહીંતર જો (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    જો (unlikely(fromLevel->અંક_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->અંક_actions = fromLevel->અંક_actions;
                        fromLevel->અંક_actions = 0;
                    } નહીંતર {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->અંક_actions = 1;
                    }
                    from_Actions_ગણતરી++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    સંરચના xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->અંક_syms = 0;
        level->અંક_actions = 0;
        from_Keysyms_ગણતરી++;
        from_Actions_ગણતરી++;
    }
    જો (from_Keysyms_ગણતરી) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        જો (from_Keysyms_ગણતરી == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    જો (from_Actions_ગણતરી) {
        /* Reset defined actions field if we used no action from `into` */
        જો (from_Actions_ગણતરી == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    પાછા true;
}

સ્થિર bool
Use_New_ચાવી_Field(ગણના ચાવી_field field, ગણના ચાવી_field old, ગણના ચાવી_field new,
               bool clobber, bool report, ગણના ચાવી_field *collide)
{
    જો (!(old & field))
        પાછા (new & field);

    જો (new & field) {
        જો (report)
            *collide |= field;

        પાછા clobber;
    }

    પાછા false;
}

સ્થિર bool
overlays_get(અચલ ચાવી_Info *info, xkb_overlay_index_t bit,
             અચલ સંરચના xkb_ચાવી **ચાવી_out)
{
    જો (bit >= (xkb_overlay_index_t)(કદ(xkb_overlay_mask_t) * CHAR_BIT))
        પાછા false;
    અચલ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    જો (!(info->overlays & mask)) પાછા false;
    જો (ચાવી_out) {
        જો (!info->overlays_alloc) {
            /* Stored inline */
            *ચાવી_out = info->overlay_ચાવી;
        } નહીંતર {
            /* Stored in sparse array */
            અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            અચલ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(નીચલું);
            *ચાવી_out = info->overlays_ચાવીઓ[index];
        }
    }
    પાછા true;
}

સ્થિર bool
overlays_ઉમેરો(ચાવી_Info *keyi, xkb_overlay_index_t bit,
                અચલ સંરચના xkb_ચાવી *ચાવી)
{
    જો (bit >= (xkb_overlay_index_t)(કદ(xkb_overlay_mask_t) * CHAR_BIT))
        પાછા false;

    અચલ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    જો ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        જો (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ચાવી = ચાવી;
            જો (!ચાવી)
                keyi->overlays_clear = true;
        } નહીંતર {
            /* Stored in sparse array */
            અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            અચલ xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(નીચલું);
            keyi->overlays_ચાવીઓ[index] = ચાવી;
        }
        પાછા true;
    }

    /* Insert new element */
    જો (!keyi->overlays || (keyi->overlays_clear && !ચાવી)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ચાવી = ચાવી;
        keyi->overlays |= mask;
        keyi->overlays_clear = !ચાવી;
    } નહીંતર જો (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        અચલ xkb_overlay_mask_t overlays = keyi->overlays | mask;
        અચલ xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        અચલ સંરચના xkb_ચાવી ** અચલ tmp = calloc(alloc, કદ(*tmp));
        જો (!tmp) પાછા false;

        /* Copy previous entries to the buffer */
        જો (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            અચલ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(નીચલું);
            tmp[idx] = keyi->overlay_ચાવી;
        } નહીંતર {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        અચલ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(નીચલું);
        tmp[idx] = ચાવી;

        keyi->overlays_ચાવીઓ = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } નહીંતર {
        /* Allocated items: grow heap and shift if relevant */
        અચલ xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        અચલ xkb_overlay_index_t ગણતરી =
            (xkb_overlay_index_t)popcount32(overlays);
        જો (ગણતરી > keyi->overlays_alloc) {
            અચલ xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(ગણતરી);
            પ્રતિજ્ઞા(alloc >= ગણતરી);
            અચલ સંરચના xkb_ચાવી ** અચલ tmp = realloc(
                keyi->overlays_ચાવીઓ,
                (size_t)alloc * કદ(*keyi->overlays_ચાવીઓ)
            );
            જો (!tmp) પાછા false;
            keyi->overlays_ચાવીઓ = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        અચલ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(નીચલું);
        જો (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        જો (index < ગણતરી - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            પ્રતિજ્ઞા(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ચાવીઓ + index + 1, keyi->overlays_ચાવીઓ + index,
                    (size_t)(ગણતરી - 1u - index) * કદ(અચલ સંરચના xkb_ચાવી *));
        }
        keyi->overlays_ચાવીઓ[index] = ચાવી;
        keyi->overlays = overlays;
    }

    પાછા true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
સ્થિર bool
merge_overlays(SymbolsInfo *info, ચાવી_Info *into, ચાવી_Info *from,
               bool clobber, bool report, ગણના ચાવી_field *collide)
{
    જો (!(from->defined & ચાવી_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } નહીંતર જો (!(into->defined & ચાવી_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ચાવીઓ = from->overlays_ચાવીઓ;
        from->overlays_ચાવીઓ = ખાલી;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ચાવી_FIELD_OVERLAY;
    } નહીંતર જો (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } નહીંતર {
        જો (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            અચલ xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            અચલ xkb_overlay_index_t ગણતરી =
                (xkb_overlay_index_t)popcount32(result_mask);

            જો (ગણતરી == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ચાવી_Info *dest = into;
            ચાવી_Info *src = from;
            જો (into->overlays_alloc >= ગણતરી &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } નહીંતર જો (from->overlays_alloc >= ગણતરી) {
                dest = from;
                src = into;
                clobber = !clobber;
            } નહીંતર જો (ગણતરી > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                જો (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                જો (!dest->overlays_alloc) {
                    અચલ સંરચના xkb_ચાવી **tmp = calloc(ગણતરી, કદ(*tmp));
                    જો (!tmp)
                        પાછા false;
                    જો (dest->overlay_ચાવી)
                        tmp[0] = dest->overlay_ચાવી;
                    dest->overlays_ચાવીઓ = tmp;
                    dest->overlays_alloc = ગણતરી;
                } નહીંતર {
                    અચલ સંરચના xkb_ચાવી **tmp =
                        realloc(dest->overlays_ચાવીઓ,
                                ગણતરી * કદ(*dest->overlays_ચાવીઓ));
                    જો (!tmp)
                        પાછા false;
                    dest->overlays_ચાવીઓ = tmp;
                    dest->overlays_alloc = ગણતરી;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            અચલ સંરચના xkb_ચાવી **src_ચાવીઓ = (src->overlays_clear)
                ? ખાલી
                : (src->overlays_alloc)
                    ? src->overlays_ચાવીઓ
                    : &src->overlay_ચાવી;
            જ્યારે (remaining) {
                /* isolate lowest set bit */
                અચલ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                અચલ xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                જો (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                અચલ સંરચના xkb_ચાવી *src_ચાવી = (src_ચાવીઓ)
                    ? *(src_ચાવીઓ++)
                    : ખાલી;

                /* check dest */
                અચલ સંરચના xkb_ચાવી *dest_ચાવી = ખાલી;
                અચલ bool conflict = overlays_get(dest, bit, &dest_ચાવી);
                જો (conflict) {
                    જો (dest_ચાવી == src_ચાવી)
                       ચાલુ;
                    નહીંતર જો (report)
                        *collide |= ચાવી_FIELD_OVERLAY;
                }

                જો ((!conflict || clobber) && !overlays_ઉમેરો(dest, bit, src_ચાવી))
                    પાછા false;
            }

            જો (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                જો (into->overlays_alloc)
                    મુક્ત(into->overlays_ચાવીઓ);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                જો (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ચાવીઓ = from->overlays_ચાવીઓ;
                    from->overlays_ચાવીઓ = ખાલી;
                    from->overlays_alloc = 0;
                } નહીંતર {
                    into->overlay_ચાવી = from->overlay_ચાવી;
                }
            }
        } નહીંતર {
            /* Disjoint overlays */

            જો (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ચાવી == from->overlay_ચાવી) {
                /* Indentical: do nothing */
                પાછા true;
            }
            જો (!(into->overlays & from->overlays)) {
                જો (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ચાવી = from->overlay_ચાવી;
                    પાછા true;
                } નહીંતર જો (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    પાછા true;
                }
            }
            /* Conflict */
            જો (report)
                *collide |= ચાવી_FIELD_OVERLAY;
            જો (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ચાવી = from->overlay_ચાવી;
            }
        }
    }
    પાછા true;
}

સ્થિર bool
Merge_ચાવીઓ(SymbolsInfo *info, ચાવી_Info *into, ચાવી_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    ગણના ચાવી_field collide = 0;
    અચલ પૂર્ણાંક verbosity = xkb_context_get_log_verbosity(info->ctx);
    અચલ bool clobber = (from->merge != MERGE_AUGMENT);
    અચલ bool report = (same_file && verbosity > 0) || verbosity > 9;

    જો (from->merge == MERGE_REPLACE) {
        Clear_ચાવી_Info(into);
        *into = *from;
        Init_ચાવી_Info(info->ctx, from);
        પાછા true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    માટે (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    માટે (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    જો (Use_New_ચાવી_Field(ચાવી_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ચાવી_FIELD_VMODMAP;
    }
    જો (Use_New_ચાવી_Field(ચાવી_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ચાવી_FIELD_REPEAT;
    }
    જો (Use_New_ચાવી_Field(ચાવી_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ચાવી_FIELD_DEFAULT_TYPE;
    }
    જો (Use_New_ચાવી_Field(ચાવી_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_સંખ્યા = from->out_of_range_group_સંખ્યા;
        into->defined |= ચાવી_FIELD_GROUPINFO;
    }

    જો (!merge_overlays(info, into, from, clobber, report, &collide))
        પાછા false;

    જો (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ચાવી_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ચાવી_Info(from);
    Init_ચાવી_Info(info->ctx, from);
    પાછા true;
}

સ્થિર xkb_atom_t
Xkb_Resolve_ચાવી_Alias(અચલ સંરચના xkb_keymap *keymap, xkb_atom_t name)
{
    જો (name < keymap->અંક_ચાવી_names) {
        અચલ KeycodeMatch match = keymap->ચાવી_names[name];
        જો (match.found && match.is_alias) {
            પાછા match.alias.real;
        }
    }
    પાછા name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
સ્થિર bool
Add_ચાવી_Symbols(SymbolsInfo *info, ચાવી_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ચાવી_Alias(&info->keymap_info->keymap, keyi->name);

    ચાવી_Info *iter;
    darray_foreach(iter, info->ચાવીઓ)
        જો (iter->name == keyi->name)
            પાછા Merge_ચાવીઓ(info, iter, keyi, same_file);

    darray_append(info->ચાવીઓ, *keyi);
    Init_ચાવી_Info(info->ctx, keyi);
    પાછા true;
}

સ્થિર bool
AddModMapEntry(SymbolsInfo *info, અચલ ModMapEntry *new)
{
    અચલ bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        જો ((new->have_પ્રતીક != old->have_પ્રતીક) ||
            (new->have_પ્રતીક && new->u.ચાવી_Sym != old->u.ચાવી_Sym) ||
            (!new->have_પ્રતીક && new->u.ચાવી_Name != old->u.ચાવી_Name))
            ચાલુ;

        જો (new->mods == old->mods)
            પાછા true;

        અચલ xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        અચલ xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        જો (new->have_પ્રતીક) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.ચાવી_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } નહીંતર {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ચાવી_Name_Text(info->ctx, new->u.ચાવી_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        પાછા true;
    }

    darray_append(info->modmaps, *new);

    પાછા true;
}

/***====================================================================***/

સ્થિર શૂન્ય
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     ગણના merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    જો (from->error_ગણતરી > 0) {
        into->error_ગણતરી += from->error_ગણતરી;
        પાછા;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    જો (into->name == ખાલી) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    માટે (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        જો (!darray_item(from->group_names, i))
            ચાલુ;

        જો (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            ચાલુ;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    જો (darray_empty(into->ચાવીઓ)) {
        into->ચાવીઓ = from->ચાવીઓ;
        darray_init(from->ચાવીઓ);
    }
    નહીંતર {
        ચાવી_Info *keyi;
        darray_foreach(keyi, from->ચાવીઓ) {
            keyi->merge = merge;
            જો (!Add_ચાવી_Symbols(into, keyi, false))
                into->error_ગણતરી++;
        }
    }

    જો (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    નહીંતર {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            જો (!AddModMapEntry(into, mm))
                into->error_ગણતરી++;
        }
    }
}

સ્થિર શૂન્ય
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

સ્થિર bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    જો (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ગણતરી += 10;
        પાછા false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    અચલ સંરચના parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    માટે (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        અક્ષર path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, કદ(path));
        જો (!file) {
            info->error_ગણતરી += 10;
            ClearSymbolsInfo(&included);
            પાછા false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        જો (stmt->modifier) {
            અચલ પૂર્ણાંક ગણતરી = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            જો (ગણતરી <= 0 || *(stmt->modifier + ગણતરી) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } નહીંતર {
                next_incl.explicit_group--;
            }
        }
        નહીંતર જો (info->keymap_info->keymap.અંક_groups != 0 &&
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
        નહીંતર {
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

    પાછા (info->error_ગણતરી == 0);
}

સ્થિર bool
GetGroupIndex(SymbolsInfo *info, ચાવી_Info *keyi, પદાવલિ_Def *arrayNdx,
              ગણના group_field field, xkb_layout_index_t *ndx_rtrn)
{
    પ્રતિજ્ઞા (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    અચલ અક્ષર *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    જો (arrayNdx == ખાલી) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            જો (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                પાછા true;
            }
        }

        જો (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ચાવી_Info_Text(info, keyi), info->max_groups, name);
            પાછા false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        પાછા true;
    }

    જો (પદાવલિ_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, ખાલી) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ચાવી_Info_Text(info, keyi));
        પાછા false;
    }

    (*ndx_rtrn)--;
    જો (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    પાછા true;
}

સ્થિર bool
Add_Symbols_To_ચાવી(SymbolsInfo *info, ચાવી_Info *keyi, પદાવલિ_Def *arrayNdx,
                પદાવલિ_Def *કિંમત)
{
    xkb_layout_index_t ndx = 0;

    જો (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        પાછા false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    જો (કિંમત->common.type == STMT_પદાવલિ_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        પાછા true;
    }

    જો (કિંમત->common.type != STMT_પદાવલિ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(કિંમત->common.type), ndx + 1,
                ચાવી_Info_Text(info, keyi));
        પાછા false;
    }

    જો (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ચાવી_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ચાવી_Info_Text(info, keyi), ndx + 1);
        પાછા false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    માટે (પદાવલિ_Keysym_List *keysymList = (પદાવલિ_Keysym_List *) કિંમત;
         keysymList;
         keysymList = (પદાવલિ_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        જો (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    જો (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    જો (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    માટે (પદાવલિ_Keysym_List *keysymList = (પદાવલિ_Keysym_List *) કિંમત;
         keysymList && level < nLevels;
         keysymList = (પદાવલિ_Keysym_List *) keysymList->common.next, level++) {
        સંરચના xkb_level *leveli = &darray_item(groupi->levels, level);
        પ્રતિજ્ઞા(leveli->અંક_syms == 0);

        જો (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ચાવી_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            પાછા false;
        }

        leveli->અંક_syms = (xkb_keysym_ગણતરી_t) darray_size(keysymList->syms);
        પસંદગી (leveli->અંક_syms) {
        સ્થિતિ 0:
            leveli->s.sym = XKB_ચાવી_No_પ્રતીક;
            તોડો;
        સ્થિતિ 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            પ્રતિજ્ઞા(leveli->s.sym != XKB_ચાવી_No_પ્રતીક);
            તોડો;
        મૂળભૂત:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ખાલી);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            માટે (xkb_keysym_ગણતરી_t k = 0; k < leveli->અંક_syms; k++)
                પ્રતિજ્ઞા(leveli->s.syms[k] != XKB_ચાવી_No_પ્રતીક);
#endif
        }
    }

    પાછા true;
}

સ્થિર bool
Add_Actions_To_ચાવી(SymbolsInfo *info, ચાવી_Info *keyi, પદાવલિ_Def *arrayNdx,
                પદાવલિ_Def *કિંમત)
{
    xkb_layout_index_t ndx = 0;

    જો (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        પાછા false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    જો (કિંમત->common.type == STMT_પદાવલિ_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        પાછા true;
    }

    જો (કિંમત->common.type != STMT_પદાવલિ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 કિંમત->common.type, ndx, ચાવી_Info_Text(info, keyi));
        પાછા false;
    }

    જો (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ચાવી_Info_Text(info, keyi), ndx);
        પાછા false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    માટે (ParseCommon *p = &કિંમત->common; p; p = p->next)
        nLevels++;
    જો (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    માટે (પદાવલિ_Action_List *actionList = (પદાવલિ_Action_List *) કિંમત;
         actionList;
         actionList = (પદાવલિ_Action_List *) actionList->common.next, level++) {
        સંરચના xkb_level *leveli = &darray_item(groupi->levels, level);
        પ્રતિજ્ઞા(leveli->અંક_actions == 0);

        unsigned પૂર્ણાંક અંક_actions = 0;
        માટે (પદાવલિ_Def *act = actionList->actions;
             act; act = (પદાવલિ_Def *) act->common.next)
            અંક_actions++;

        જો (unlikely(અંક_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ચાવી_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, અંક_actions);
            પાછા false;
        }

        /* Parse actions and add only defined actions */
        darray(સંઘ xkb_action) actions = darray_new();
        માટે (પદાવલિ_Def *act = actionList->actions;
             act; act = (પદાવલિ_Def *) act->common.next) {
            સંઘ xkb_action toAct = { 0 };
            અચલ ગણના xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            જો (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_કિંમત,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ચાવી_Info_Text(info, keyi), ndx + 1, level + 1);
                જો (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    પાછા false;
                } નહીંતર {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            જો (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } નહીંતર જો (likely(અંક_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->અંક_actions = 1;
                leveli->a.action = toAct;
                જાઓ next_level;
            } નહીંતર {
                darray_append(actions, toAct);
            }
        }
        જો (darray_empty(actions)) {
            leveli->અંક_actions = 0;
        } નહીંતર જો (likely(darray_size(actions) > 1)) {
            leveli->અંક_actions = (xkb_action_ગણતરી_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ખાલી);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            માટે (xkb_action_ગણતરી_t k = 0; k < leveli->અંક_actions; k++)
                પ્રતિજ્ઞા(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } નહીંતર {
            /* Unlikely: some actions were dropped and only one remains */
            પ્રતિજ્ઞા(અંક_actions > darray_size(actions));
            leveli->અંક_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            પ્રતિજ્ઞા(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        જો (leveli->અંક_actions > 0 || leveli->અંક_syms > 0)
            nonEmptyLevels = level + 1;
    }

    જો (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        જો (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        નહીંતર
            darray_free(groupi->levels);
    }

    પાછા true;
}

સ્થિર અચલ LookupEntry repeatEntries[] = {
    { "true", ચાવી_REPEAT_YES },
    { "yes", ચાવી_REPEAT_YES },
    { "on", ચાવી_REPEAT_YES },
    { "false", ચાવી_REPEAT_NO },
    { "no", ચાવી_REPEAT_NO },
    { "off", ચાવી_REPEAT_NO },
    { "default", ચાવી_REPEAT_UNDEFINED },
    { ખાલી, 0 }
};

/** Handle overlay<N> = <key> entry */
સ્થિર bool
પદાવલિ_Resolve_Overlay_Entry(અચલ સંરચના xkb_keymap_info *keymap_info,
                        અચલ અક્ષર *field, અચલ પદાવલિ_Def *arrayNdx,
                        અચલ પદાવલિ_Def *પદાવલિ, ચાવી_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        અચલ સંરચના xkb_ચાવી **ચાવી_rtrn)
{
    /* Overlay index */
    જો (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, ચાવી_Name_Text(keymap_info->keymap.ctx, keyi->name));
        પાછા false;
    }
    static_assert(કદ(xkb_overlay_index_t) == કદ(uint8_t), "");
    અચલ size_t prefix = કદ("overlay") - 1;
    અચલ size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    જો (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (પૂર્ણાંક)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, ચાવી_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        પાછા false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_KEYNAME_LITERAL:
        *ચાવી_rtrn = Xkb_ચાવી_By_Name(&keymap_info->keymap, પદાવલિ->ચાવી_name.ચાવી_name,
                                 false);
        જો (!*ચાવી_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, પદાવલિ->ચાવી_name.ચાવી_name),
                    field, ચાવી_Name_Text(keymap_info->keymap.ctx, keyi->name));
            પાછા false;
        }
        પાછા true;
    સ્થિતિ STMT_પદાવલિ_IDENT: {
        અચલ અક્ષર * અચલ id =
            xkb_atom_text(keymap_info->keymap.ctx, પદાવલિ->ident.ident);
        જો (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ચાવી_rtrn = ખાલી;
            પાછા true;
        } નહીંતર જો (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ચાવી_rtrn = ખાલી;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            પાછા true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_કિંમત,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, ચાવી_Name_Text(keymap_info->keymap.ctx, keyi->name));
        પાછા false;
    }
    મૂળભૂત:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_કિંમત,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_પદાવલિ_KEYNAME_LITERAL),
                field, ચાવી_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;
    }
}

સ્થિર bool
SetSymbolsField(SymbolsInfo *info, ચાવી_Info *keyi, અચલ અક્ષર *field,
                પદાવલિ_Def *arrayNdx, પદાવલિ_Def **કિંમત_ptr)
{
    પદાવલિ_Def * restrict અચલ કિંમત = *કિંમત_ptr;
    જો (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        જો (!પદાવલિ_Resolve_String(info->ctx, કિંમત, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            પાછા false;
        }

        જો (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ચાવી_FIELD_DEFAULT_TYPE;
        }
        નહીંતર જો (પદાવલિ_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, ખાલી) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }
        નહીંતર {
            ndx--;
            જો (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                પ્રતિજ્ઞા(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    નહીંતર જો (istreq(field, "symbols")) {
        પાછા Add_Symbols_To_ચાવી(info, keyi, arrayNdx, કિંમત);
    }
    નહીંતર જો (istreq(field, "actions")) {
        પાછા Add_Actions_To_ચાવી(info, keyi, arrayNdx, કિંમત);
    }
    નહીંતર જો (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, કિંમત, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(કિંમત->common.type),
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= ચાવી_FIELD_VMODMAP;
    }
    નહીંતર જો (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ચાવી_Info_Text(info, keyi));
    }
    નહીંતર જો (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ચાવી_Info_Text(info, keyi));
    }
    નહીંતર જો (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ચાવી_Info_Text(info, keyi));
    }
    નહીંતર જો (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        અચલ સંરચના xkb_ચાવી *ચાવી = ખાલી;
        જો (!પદાવલિ_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *કિંમત_ptr, keyi, &overlay, &ચાવી))
            પાછા false;
        જો (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            પાછા true;
        } નહીંતર જો (ચાવી && ચાવી->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ચાવી_Info_Text(info, keyi));
        } નહીંતર {
            અચલ સંરચના xkb_ચાવી *prev;
            જો (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                જો (ચાવી != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ચાવી_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? ચાવી_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ચાવી
                                 ? ચાવી_Name_Text(info->ctx, ચાવી->name)
                                 : "none"));
                }
            } નહીંતર જો (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                જો (!overlays_ઉમેરો(keyi, overlay, ચાવી))
                    પાછા false;
                keyi->defined |= ચાવી_FIELD_OVERLAY;
            } નહીંતર {
                /* Handle disjoint overlays mode */
                અચલ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                જો (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    જો (ચાવી) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_ચાવી = ચાવી;
                    } નહીંતર {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ચાવી_FIELD_OVERLAY;
                } નહીંતર જો (keyi->overlays) {
                    જો (ચાવી) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ચાવી_Info_Text(info, keyi), keyi->overlays,
                                ચાવી_Name_Text(info->ctx, keyi->overlay_ચાવી->name),
                                overlay + 1, ચાવી_Name_Text(info->ctx, ચાવી->name));
                        પાછા !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_કિંમત_MISMATCH);
                    } નહીંતર {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    નહીંતર જો (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        જો (!પદાવલિ_Resolve_Enum(info->ctx, કિંમત, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_કિંમત,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }

        keyi->repeat = (ગણના ચાવી_repeat) val;
        keyi->defined |= ચાવી_FIELD_REPEAT;
    }
    નહીંતર જો (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        જો (!પદાવલિ_Resolve_Boolean(info->ctx, કિંમત, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_કિંમત,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= ચાવી_FIELD_GROUPINFO;
    }
    નહીંતર જો (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        જો (!પદાવલિ_Resolve_Boolean(info->ctx, કિંમત, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_કિંમત,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= ચાવી_FIELD_GROUPINFO;
    }
    નહીંતર જો (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        જો (પદાવલિ_Resolve_Group(info->keymap_info, કિંમત, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ચાવી_Info_Text(info, keyi));
            પાછા false;
        }

        જો (pending) {
            keyi->out_of_range_pending_group = true;
            અચલ darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (સંરચના pending_computation) {
                    .પદાવલિ = *કિંમત_ptr,
                    .computed = false,
                    .કિંમત = 0,
                }
            );
            *કિંમત_ptr = ખાલી;
            static_assert(કદ(keyi->out_of_range_group_સંખ્યા) ==
                          કદ(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_સંખ્યા = pending_index;
        } નહીંતર {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_સંખ્યા = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= ચાવી_FIELD_GROUPINFO;
    }
    નહીંતર {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        પાછા !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ચાવી_FIELDS);
    }

    પાછા true;
}

સ્થિર bool
SetGroupName(SymbolsInfo *info, પદાવલિ_Def *arrayNdx, પદાવલિ_Def *કિંમત,
             ગણના merge_mode merge)
{
    જો (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        પાછા false;
    }

    xkb_layout_index_t group = 0;
    જો (પદાવલિ_Resolve_Group(info->keymap_info, arrayNdx, false, &group, ખાલી) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        પાછા false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    જો (!પદાવલિ_Resolve_String(info->ctx, કિંમત, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        પાછા false;
    }

    xkb_layout_index_t group_to_use;
    જો (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    નહીંતર જો (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    નહીંતર {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        પાછા false;
    }

    જો (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        પ્રતિજ્ઞા(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } નહીંતર {
        અચલ xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        જો (old_name != XKB_ATOM_NONE && old_name != name) {
            અચલ bool replace = (merge != MERGE_AUGMENT);
            અચલ xkb_atom_t use    = (replace ? name : old_name);
            અચલ xkb_atom_t ignore = (replace ? old_name : name);
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

    પાછા true;
}

સ્થિર bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *arrayNdx;
    bool ret;

    જો (!પદાવલિ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        પાછા false;

    જો (elem && istreq(elem, "key")) {
        ચાવી_Info temp = {0};
        Init_ચાવી_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->કિંમત) &&
              Merge_ચાવીઓ(info, &info->default_ચાવી, &temp, true);
        Clear_ચાવી_Info(&temp);
    }
    નહીંતર જો (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->કિંમત, stmt->merge);
    }
    નહીંતર જો (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    નહીંતર જો (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    નહીંતર જો (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    નહીંતર જો (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    નહીંતર જો (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->કિંમત, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } નહીંતર {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        પાછા !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    પાછા ret;
}

સ્થિર bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ચાવી_Info *keyi)
{
    bool all_valid_entries = true;

    માટે (; def; def = (VarDef *) def->common.next) {
        અચલ અક્ષર *field;
        પદાવલિ_Def *arrayNdx;
        bool ok = true;

        જો (!def->name) {
            જો (unlikely(!def->કિંમત) || def->કિંમત->common.type != STMT_પદાવલિ_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            નહીંતર
                field = "actions";
            arrayNdx = ખાલી;
        }
        નહીંતર {
            અચલ અક્ષર *elem;
            ok = પદાવલિ_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            જો (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        જો (unlikely(!def->કિંમત)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        જો (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->કિંમત))
            all_valid_entries = false;
    }

    પાછા all_valid_entries;
}

સ્થિર bool
SetExplicitGroup(SymbolsInfo *info, ચાવી_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    જો (info->explicit_group == XKB_LAYOUT_INVALID)
        પાછા true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        જો (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    જો (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, ચાવી_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    જો (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    પાછા true;
}

સ્થિર bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ચાવી_Info keyi;

    keyi = info->default_ચાવી;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ચાવી.groups);
    માટે (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_ચાવી.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->ચાવી_Name;

    જો (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_ચાવી_Symbols(info, &keyi, true)) {
        પાછા true;
    }

    Clear_ચાવી_Info(&keyi);
    info->error_ગણતરી++;
    પાછા false;
}

સ્થિર bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    સંરચના xkb_context *ctx = info->ctx;

    જો (!પદાવલિ_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        પાછા false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    પ્રતિજ્ઞા(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_પદાવલિ_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    માટે (પદાવલિ_Def *ચાવી = def->ચાવીઓ; ચાવી; ચાવી = (પદાવલિ_Def *) ચાવી->common.next) {
        જો (ચાવી->common.type == STMT_પદાવલિ_KEYNAME_LITERAL) {
            tmp.have_પ્રતીક = false;
            tmp.u.ચાવી_Name = ચાવી->ચાવી_name.ચાવી_name;
        }
        નહીંતર જો (ચાવી->common.type == STMT_પદાવલિ_KEYSYM_LITERAL) {
            જો (ચાવી->keysym.keysym == XKB_ચાવી_No_પ્રતીક) {
                /* Invalid keysym: ignore. Error message already printed */
                ચાલુ;
            } નહીંતર {
                tmp.have_પ્રતીક = true;
                tmp.u.ચાવી_Sym = ચાવી->keysym.keysym;
            }
        }
        નહીંતર {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ચાલુ;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    પાછા ok;
}

સ્થિર શૂન્ય
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    મુક્ત(info->name);
    info->name = strdup_safe(file->name);

    માટે (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        પસંદગી (stmt->type) {
        સ્થિતિ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            તોડો;
        સ્થિતિ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            તોડો;
        સ્થિતિ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            તોડો;
        સ્થિતિ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            તોડો;
        સ્થિતિ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            તોડો;
        સ્થિતિ STMT_UNKNOWN_DECLARATION:
        સ્થિતિ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            તોડો;
        મૂળભૂત:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            તોડો;
        }

        જો (!ok)
            info->error_ગણતરી++;

        જો (info->error_ગણતરી > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            તોડો;
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
સ્થિર સંરચના xkb_ચાવી *
Find_ચાવી_For_પ્રતીક(સંરચના xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    કરો {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        કરો {
            got_one_level = false;
            સંરચના xkb_ચાવી *ચાવી;
            xkb_ચાવીઓ_foreach(ચાવી, keymap) {
                જો (group < ચાવી->અંક_groups &&
                    level < Xkb_ચાવી_અંક_Levels(ચાવી, group)) {
                    got_one_group = got_one_level = true;
                    અચલ xkb_keysym_ગણતરી_t અંક_syms =
                        ચાવી->groups[group].levels[level].અંક_syms;
                    જો (અંક_syms > 1) {
                        માટે (xkb_keysym_ગણતરી_t k = 0; k < અંક_syms; k++) {
                            જો (ચાવી->groups[group].levels[level].s.syms[k] == sym)
                                પાછા ચાવી;
                        }
                    } નહીંતર જો (અંક_syms &&
                               ચાવી->groups[group].levels[level].s.sym == sym) {
                        પાછા ચાવી;
                    }
                }
            }
            level++;
        } જ્યારે (got_one_level);
        group++;
    } જ્યારે (got_one_group);

    પાછા ખાલી;
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
સ્થિર xkb_atom_t
FindAutomaticType(સંરચના xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    અચલ xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).અંક_syms == 0 ? \
        XKB_ચાવી_No_પ્રતીક : \
     darray_item(groupi->levels, level).અંક_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    જો (width == 1 || width <= 0)
        પાછા xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    જો (width == 2) {
        જો (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            પાછા xkb_atom_intern_literal(ctx, "ALPHABETIC");

        જો (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            પાછા xkb_atom_intern_literal(ctx, "KEYPAD");

        પાછા xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    જો (width <= 4) {
        જો (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_ચાવી_No_પ્રતીક);

            જો (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                પાછા xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            પાછા xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        જો (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            પાછા xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        પાછા xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    પાછા XKB_ATOM_NONE;

#undef GET_SYM
}

સ્થિર અચલ સંરચના xkb_ચાવી_type *
FindTypeForGroup(સંરચના xkb_keymap *keymap, ચાવી_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    જો (type_name == XKB_ATOM_NONE) {
        જો (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        નહીંતર {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            જો (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    જો (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ચાવી_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ચાવી_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        જાઓ use_default;
    }

    darray_size_t i;
    માટે (i = 0; i < keymap->અંક_types; i++)
        જો (keymap->types[i].name == type_name)
            તોડો;

    જો (i >= keymap->અંક_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ચાવી_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 ચાવી_Name_Text(keymap->ctx, keyi->name), group + 1);
        જાઓ use_default;
    }

    keymap->types[i].required = true;
    પાછા &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    પાછા &keymap->types[0];
}

સ્થિર bool
CopySymbolsDefToKeymap(સંરચના xkb_keymap *keymap, SymbolsInfo *info,
                       ચાવી_Info *keyi)
{
    સંરચના xkb_ચાવી *ચાવી;
    GroupInfo *groupi;
    અચલ GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ચાવી = Xkb_ચાવી_By_Name(keymap, keyi->name, false);
    જો (!ચાવી) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ચાવી_Info_Text(info, keyi));
        પાછા false;
    }

    /* Find the range of groups we need. */
    ચાવી->અંક_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        અચલ bool has_explicit_type = (keyi->defined & ચાવી_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        જો (darray_size(groupi->levels) > 0 || has_explicit_type)
            ચાવી->અંક_groups = i + 1;
        જો (has_explicit_type)
            ચાવી->explicit |= EXPLICIT_TYPES;
    }

    જો (ચાવી->અંક_groups <= 0) {
        /* A key with no group may still have other fields defined */
        જો (keyi->defined)
            જાઓ ચાવી_fields;
        નહીંતર
            પાછા false;
    }

    darray_resize(keyi->groups, ચાવી->અંક_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        જો (groupi->defined)
            ચાલુ;

        CopyGroupInfo(groupi, group0);
    }

    ચાવી->groups = calloc(ચાવી->અંક_groups, કદ(*ચાવી->groups));
    પ્રતિજ્ઞા(darray_size(keyi->groups) == ચાવી->અંક_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ચાવી->અંક_groups);
        #endif
        bool explicit_type = false;
        અચલ સંરચના xkb_ચાવી_type * અચલ type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        જો (type->અંક_levels < darray_size(groupi->levels)) {
            સંરચના xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->અંક_levels,
                    ચાવી_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->અંક_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->અંક_levels);

        ચાવી->groups[i].explicit_type = explicit_type;
        ચાવી->groups[i].type = type;
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
        સંરચના xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            પસંદગી (leveli->અંક_syms) {
            સ્થિતિ 0:
                leveli->upper = XKB_ચાવી_No_પ્રતીક;
                તોડો;
            સ્થિતિ 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                તોડો;
            મૂળભૂત:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                માટે (xkb_keysym_ગણતરી_t k = 0; k < leveli->અંક_syms; k++) {
                    અચલ xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    જો (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        તોડો;
                    }
                }
                જો (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->અંક_syms *
                                             કદ(*leveli->s.syms));
                    જો (!leveli->s.syms)
                        પાછા false; /* FIXME: better handling? */
                    માટે (xkb_keysym_ગણતરી_t k = 0; k < leveli->અંક_syms; k++) {
                        leveli->s.syms[leveli->અંક_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &ચાવી->groups[i].levels, ખાલી);
        જો (ચાવી->groups[i].type->અંક_levels > 1 ||
            ચાવી->groups[i].levels[0].અંક_syms > 0) {
            ચાવી->groups[i].explicit_symbols = true;
            ચાવી->explicit |= EXPLICIT_SYMBOLS;
        }
        જો (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            ચાવી->groups[i].explicit_actions = true;
            ચાવી->explicit |= EXPLICIT_INTERP;
        }
        જો (ચાવી->groups[i].explicit_type)
            ચાવી->explicit |= EXPLICIT_TYPES;
    }

    ચાવી->out_of_range_pending_group = keyi->out_of_range_pending_group;
    ચાવી->out_of_range_group_સંખ્યા = keyi->out_of_range_group_સંખ્યા;
    ચાવી->out_of_range_group_policy = keyi->out_of_range_group_policy;

ચાવી_fields:
    જો (keyi->defined & ચાવી_FIELD_VMODMAP) {
        ચાવી->vmodmap = keyi->vmodmap;
        ચાવી->explicit |= EXPLICIT_VMODMAP;
    }

    જો (keyi->repeat != ચાવી_REPEAT_UNDEFINED) {
        ચાવી->repeats = (keyi->repeat == ચાવી_REPEAT_YES);
        ચાવી->explicit |= EXPLICIT_REPEAT;
    }

    જો ((keyi->defined & ચાવી_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ચાવી->overlays = keyi->overlays;
        જો (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = ચાવી->overlays;
            અચલ સંરચના xkb_ચાવી **overlays_ચાવીઓ = keyi->overlays_ચાવીઓ;
            જ્યારે (remaining) {
                /* isolate lowest set bit */
                અચલ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                જો (*overlays_ચાવીઓ) {
                    overlays_ચાવીઓ++;
                } નહીંતર {
                    /* drop current null value (overlayN=none) */
                    ચાવી->overlays &= ~lsb;
                }
            }

            જો (ચાવી->overlays) {
                /* Steal */
                ચાવી->overlays_ચાવીઓ = keyi->overlays_ચાવીઓ;
                keyi->overlays_ચાવીઓ = ખાલી;
                keyi->overlays_alloc = 0;

                ચાવી->overlays_inline = false;
                ચાવી->explicit |= EXPLICIT_OVERLAY;
            }
        } નહીંતર {
            ચાવી->overlay_ચાવી = keyi->overlay_ચાવી;
            ચાવી->overlays_inline = true;
            ચાવી->explicit |= EXPLICIT_OVERLAY;
        }
    }

    પાછા true;
}

સ્થિર bool
CopyModMapDefToKeymap(સંરચના xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    સંરચના xkb_ચાવી *ચાવી;

    જો (!entry->have_પ્રતીક) {
        ચાવી = Xkb_ચાવી_By_Name(keymap, entry->u.ચાવી_Name, true);
        જો (!ચાવી) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ચાવી_Name_Text(info->ctx, entry->u.ચાવી_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            પાછા false;
        }
    }
    નહીંતર {
        ચાવી = Find_ચાવી_For_પ્રતીક(keymap, entry->u.ચાવી_Sym);
        જો (!ચાવી) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_પ્રતીક,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.ચાવી_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            પાછા false;
        }
    }

    /* Skip modMap None */
    જો (entry->mods) {
        ચાવી->modmap |= entry->mods;
        entry->ચાવી_Code = ચાવી->keycode;
    }

    પાછા true;
}

સ્થિર bool
CopySymbolsToKeymap(સંરચના xkb_keymap *keymap, SymbolsInfo *info)
{
    ચાવી_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->અંક_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, ખાલી);

    darray_foreach(keyi, info->ચાવીઓ)
        જો (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_ગણતરી++;

    જો (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        સંરચના xkb_ચાવી *ચાવી;

        xkb_ચાવીઓ_foreach(ચાવી, keymap) {
            જો (ચાવી->name == XKB_ATOM_NONE)
                ચાલુ;

            જો (ચાવી->અંક_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         ચાવી_Name_Text(info->ctx, ચાવી->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        જો (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_ગણતરી++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->અંક_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    પાછા true;
}

bool
CompileSymbols(XkbFile *file, સંરચના xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    જો (file !=ખાલી)
        HandleSymbolsFile(&info, file);

    જો (info.error_ગણતરી != 0)
        જાઓ err_info;

    જો (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        જાઓ err_info;

    ClearSymbolsInfo(&info);
    પાછા true;

err_info:
    ClearSymbolsInfo(&info);
    પાછા false;
}
