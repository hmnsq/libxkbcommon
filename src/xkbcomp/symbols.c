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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"

#ಅನ್ತರ್ಭಾವ "action.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "vmod.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"

ಗಣನಾ ಕೀಲಿ_repeat {
    ಕೀಲಿ_REPEAT_UNDEFINED = 0,
    ಕೀಲಿ_REPEAT_YES,
    ಕೀಲಿ_REPEAT_NO,
    ಕೀಲಿ_REPEAT_ಅಂಕೆ_ENTRIES,
};
ಗಣನಾ { ಕೀಲಿ_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(ಕೀಲಿ_REPEAT_ಅಂಕೆ_ENTRIES - 1 <=
              (1 << (ಕೀಲಿ_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

ಗಣನಾ group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

ಗಣನಾ ಕೀಲಿ_field {
    ಕೀಲಿ_FIELD_NONE = 0,
    ಕೀಲಿ_FIELD_REPEAT = (1 << 0),
    ಕೀಲಿ_FIELD_DEFAULT_TYPE = (1 << 1),
    ಕೀಲಿ_FIELD_GROUPINFO = (1 << 2),
    ಕೀಲಿ_FIELD_VMODMAP = (1 << 3),
    ಕೀಲಿ_FIELD_OVERLAY = (1 << 4),
    ಕೀಲಿ_FIELD_ALL = ಕೀಲಿ_FIELD_REPEAT | ಕೀಲಿ_FIELD_DEFAULT_TYPE
                  | ಕೀಲಿ_FIELD_GROUPINFO | ಕೀಲಿ_FIELD_VMODMAP | ಕೀಲಿ_FIELD_OVERLAY,
};
ಗಣನಾ { ಕೀಲಿ_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(ಕೀಲಿ_FIELD_ALL <= (1 << (ಕೀಲಿ_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    darray(ಸಂರಚನಾ xkb_level) levels;
    ಗಣನಾ group_field defined;
    xkb_atom_t type;
} GroupInfo;

ಗಣನಾ {
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
              (unsigned)ಕೀಲಿ_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)ಕೀಲಿ_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_ಸಂಖ್ಯೆ;
    darray(GroupInfo) groups;
    ಗಣನಾ xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    ಗಣನಾ ಕೀಲಿ_field defined:KEYINFO_DEFINED_BIT_FIELD;
    ಗಣನಾ merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    ಗಣನಾ ಕೀಲಿ_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    ಸಙ್ಘ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು;
    };
} ಕೀಲಿ_Info;

ಸ್ಥಿರ ಶೂನ್ಯ
StealLevelInfo(ಸಂರಚನಾ xkb_level *into, ಸಂರಚನಾ xkb_level *from)
{
    clear_level(into);
    ಯದಿ (from->ಅಂಕೆ_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ಅನ್ಯಥಾ {
        into->s.sym = from->s.sym;
    }
    into->ಅಂಕೆ_syms = from->ಅಂಕೆ_syms;
    from->ಅಂಕೆ_syms = 0;

    ಯದಿ (from->ಅಂಕೆ_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ಅನ್ಯಥಾ {
        into->a.action = from->a.action;
    }
    into->ಅಂಕೆ_actions = from->ಅಂಕೆ_actions;
    from->ಅಂಕೆ_actions = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, ಪರಿಮಾಣ(*groupi));
}

ಸ್ಥಿರ ಶೂನ್ಯ
ClearGroupInfo(GroupInfo *groupi)
{
    ಸಂರಚನಾ xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

ಸ್ಥಿರ ಶೂನ್ಯ
CopyGroupInfo(GroupInfo *to, ಧ್ರುವ GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    ಪ್ರತಿ (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        ಯದಿ (darray_item(from->levels, j).ಅಂಕೆ_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).ಅಂಕೆ_syms,
                       ಪರಿಮಾಣ(xkb_keysym_t));
        }
        ಯದಿ (darray_item(from->levels, j).ಅಂಕೆ_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).ಅಂಕೆ_actions,
                       ಪರಿಮಾಣ(ಸಙ್ಘ xkb_action));
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_Info(ಸಂರಚನಾ xkb_context *ctx, ಕೀಲಿ_Info *keyi)
{
    memset(keyi, 0, ಪರಿಮಾಣ(*keyi));
    static_assert(!DEFAULT_ಕೀಲಿ_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ಕೀಲಿ_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ಕೀಲಿ = ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Info(ಕೀಲಿ_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    ಯದಿ (keyi->overlays_alloc)
        ಮುಕ್ತ(keyi->overlays_ಕೀಲಿಗಳು);
}

/***====================================================================***/

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ *name;         /* e.g. pc+us+inet(evdev) */
    ಪೂರ್ಣಾಙ್ಕ error_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(ಕೀಲಿ_Info) ಕೀಲಿಗಳು;
    ಕೀಲಿ_Info default_ಕೀಲಿ;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    ಸಂರಚನಾ xkb_mod_set mods;

    ಸಂರಚನಾ xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info;
} SymbolsInfo;

ಸ್ಥಿರ ಶೂನ್ಯ
InitSymbolsInfo(SymbolsInfo *info, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                unsigned ಪೂರ್ಣಾಙ್ಕ include_depth, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods)
{
    memset(info, 0, ಪರಿಮಾಣ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ಕೀಲಿ_Info(keymap_info->keymap.ctx, &info->default_ಕೀಲಿ);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ಕೀಲಿ_Info *keyi;
    ಮುಕ್ತ(info->name);
    darray_foreach(keyi, info->ಕೀಲಿಗಳು)
        Clear_ಕೀಲಿ_Info(keyi);
    darray_free(info->ಕೀಲಿಗಳು);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_ಕೀಲಿ_Info(&info->default_ಕೀಲಿ);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
ಕೀಲಿ_Info_Text(SymbolsInfo *info, ಕೀಲಿ_Info *keyi)
{
    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_Name_Text(info->ctx, keyi->name);
}

ಸ್ಥಿರ bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t ಕೀಲಿ_name)
{
    /* First find the type of the merged group. */
    ಯದಿ (into->type != from->type) {
        ಯದಿ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ಅನ್ಯಥಾ ಯದಿ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ಅನ್ಯಥಾ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ಯದಿ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    ಯದಿ (darray_empty(from->levels)) {
        InitGroupInfo(from);
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /* Merge the actions and syms. */
    ಧ್ರುವ darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_ಎಣಿಕೆ = 0;
    darray_size_t from_Actions_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (darray_size_t i = 0; i < levels_in_both; i++) {
        ಸಂರಚನಾ xkb_level* ಧ್ರುವ intoLevel = &darray_item(into->levels, i);
        ಸಂರಚನಾ xkb_level* ಧ್ರುವ fromLevel = &darray_item(from->levels, i);

        ಧ್ರುವ bool fromHasNoKeysym = fromLevel->ಅಂಕೆ_syms == 0;
        ಧ್ರುವ bool fromHasNoAction = fromLevel->ಅಂಕೆ_actions == 0;
        ಯದಿ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ಅನುವರ್ತ;
        }

        ಧ್ರುವ bool intoHasNoKeysym = intoLevel->ಅಂಕೆ_syms == 0;
        ಧ್ರುವ bool intoHasNoAction = intoLevel->ಅಂಕೆ_actions == 0;
        ಯದಿ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ಎಣಿಕೆ++;
            from_Actions_ಎಣಿಕೆ++;
            ಅನುವರ್ತ;
        }

        /* Possible level conflict */
        ಪ್ರತಿಜ್ಞಾ(intoLevel->ಅಂಕೆ_syms > 0 || intoLevel->ಅಂಕೆ_actions > 0);
        ಪ್ರತಿಜ್ಞಾ(fromLevel->ಅಂಕೆ_syms > 0 || fromLevel->ಅಂಕೆ_actions > 0);

        /* Handle keysyms */
        ಯದಿ (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ಯದಿ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ಚಿಹ್ನೆ,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ಯದಿ (fromHasNoKeysym) {
                /* No keysym to copy */
            } ಅನ್ಯಥಾ ಯದಿ (clobber) {
                /* Override: copy any defined keysym from `from` */
                ಯದಿ (unlikely(fromLevel->ಅಂಕೆ_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_syms > 1))
                        ಮುಕ್ತ(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->ಅಂಕೆ_syms = fromLevel->ಅಂಕೆ_syms;
                    fromLevel->ಅಂಕೆ_syms = 0;
                    from_Keysyms_ಎಣಿಕೆ++;
                } ಅನ್ಯಥಾ ಯದಿ (fromLevel->s.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                    /* Single defined keysym */
                    ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_syms > 1))
                        ಮುಕ್ತ(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->ಅಂಕೆ_syms = 1;
                    from_Keysyms_ಎಣಿಕೆ++;
                }
            } ಅನ್ಯಥಾ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ಅನ್ಯಥಾ ಯದಿ (intoLevel->s.sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                    /* Single undefined keysym */
                    ಯದಿ (unlikely(fromLevel->ಅಂಕೆ_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->ಅಂಕೆ_syms = fromLevel->ಅಂಕೆ_syms;
                        fromLevel->ಅಂಕೆ_syms = 0;
                    } ಅನ್ಯಥಾ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->ಅಂಕೆ_syms = 1;
                    }
                    from_Keysyms_ಎಣಿಕೆ++;
                }
            }
        }

        /* Handle actions */
        ಯದಿ (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ಯದಿ (report && !(intoHasNoAction || fromHasNoAction)) {
                ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ಅನ್ಯಥಾ {
                    ಸಙ್ಘ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            ಯದಿ (fromHasNoAction) {
                /* No action to copy */
            } ಅನ್ಯಥಾ ಯದಿ (clobber) {
                /* Override: copy any defined action from `from` */
                ಯದಿ (unlikely(fromLevel->ಅಂಕೆ_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_actions > 1))
                        ಮುಕ್ತ(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->ಅಂಕೆ_actions = fromLevel->ಅಂಕೆ_actions;
                    fromLevel->ಅಂಕೆ_actions = 0;
                    from_Actions_ಎಣಿಕೆ++;
                } ಅನ್ಯಥಾ ಯದಿ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_actions > 1))
                        ಮುಕ್ತ(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->ಅಂಕೆ_actions = 1;
                    from_Actions_ಎಣಿಕೆ++;
                }
            } ಅನ್ಯಥಾ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ಯದಿ (unlikely(intoLevel->ಅಂಕೆ_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ಅನ್ಯಥಾ ಯದಿ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ಯದಿ (unlikely(fromLevel->ಅಂಕೆ_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->ಅಂಕೆ_actions = fromLevel->ಅಂಕೆ_actions;
                        fromLevel->ಅಂಕೆ_actions = 0;
                    } ಅನ್ಯಥಾ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->ಅಂಕೆ_actions = 1;
                    }
                    from_Actions_ಎಣಿಕೆ++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ಸಂರಚನಾ xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->ಅಂಕೆ_syms = 0;
        level->ಅಂಕೆ_actions = 0;
        from_Keysyms_ಎಣಿಕೆ++;
        from_Actions_ಎಣಿಕೆ++;
    }
    ಯದಿ (from_Keysyms_ಎಣಿಕೆ) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ಯದಿ (from_Keysyms_ಎಣಿಕೆ == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    ಯದಿ (from_Actions_ಎಣಿಕೆ) {
        /* Reset defined actions field if we used no action from `into` */
        ಯದಿ (from_Actions_ಎಣಿಕೆ == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Use_New_ಕೀಲಿ_Field(ಗಣನಾ ಕೀಲಿ_field field, ಗಣನಾ ಕೀಲಿ_field old, ಗಣನಾ ಕೀಲಿ_field new,
               bool clobber, bool report, ಗಣನಾ ಕೀಲಿ_field *collide)
{
    ಯದಿ (!(old & field))
        ಪ್ರತ್ಯಾಗಮ (new & field);

    ಯದಿ (new & field) {
        ಯದಿ (report)
            *collide |= field;

        ಪ್ರತ್ಯಾಗಮ clobber;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
overlays_get(ಧ್ರುವ ಕೀಲಿ_Info *info, xkb_overlay_index_t bit,
             ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **ಕೀಲಿ_out)
{
    ಯದಿ (bit >= (xkb_overlay_index_t)(ಪರಿಮಾಣ(xkb_overlay_mask_t) * CHAR_BIT))
        ಪ್ರತ್ಯಾಗಮ false;
    ಧ್ರುವ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    ಯದಿ (!(info->overlays & mask)) ಪ್ರತ್ಯಾಗಮ false;
    ಯದಿ (ಕೀಲಿ_out) {
        ಯದಿ (!info->overlays_alloc) {
            /* Stored inline */
            *ಕೀಲಿ_out = info->overlay_ಕೀಲಿ;
        } ಅನ್ಯಥಾ {
            /* Stored in sparse array */
            ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ಧ್ರುವ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
            *ಕೀಲಿ_out = info->overlays_ಕೀಲಿಗಳು[index];
        }
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
overlays_ಸೇರಿಸು(ಕೀಲಿ_Info *keyi, xkb_overlay_index_t bit,
                ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಯದಿ (bit >= (xkb_overlay_index_t)(ಪರಿಮಾಣ(xkb_overlay_mask_t) * CHAR_BIT))
        ಪ್ರತ್ಯಾಗಮ false;

    ಧ್ರುವ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    ಯದಿ ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ಯದಿ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
            ಯದಿ (!ಕೀಲಿ)
                keyi->overlays_clear = true;
        } ಅನ್ಯಥಾ {
            /* Stored in sparse array */
            ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ಧ್ರುವ xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
            keyi->overlays_ಕೀಲಿಗಳು[index] = ಕೀಲಿ;
        }
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /* Insert new element */
    ಯದಿ (!keyi->overlays || (keyi->overlays_clear && !ಕೀಲಿ)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
        keyi->overlays |= mask;
        keyi->overlays_clear = !ಕೀಲಿ;
    } ಅನ್ಯಥಾ ಯದಿ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ಧ್ರುವ xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ಧ್ರುವ xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ ** ಧ್ರುವ tmp = calloc(alloc, ಪರಿಮಾಣ(*tmp));
        ಯದಿ (!tmp) ಪ್ರತ್ಯಾಗಮ false;

        /* Copy previous entries to the buffer */
        ಯದಿ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ಧ್ರುವ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
            tmp[idx] = keyi->overlay_ಕೀಲಿ;
        } ಅನ್ಯಥಾ {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ಧ್ರುವ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
        tmp[idx] = ಕೀಲಿ;

        keyi->overlays_ಕೀಲಿಗಳು = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } ಅನ್ಯಥಾ {
        /* Allocated items: grow heap and shift if relevant */
        ಧ್ರುವ xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ಧ್ರುವ xkb_overlay_index_t ಎಣಿಕೆ =
            (xkb_overlay_index_t)popcount32(overlays);
        ಯದಿ (ಎಣಿಕೆ > keyi->overlays_alloc) {
            ಧ್ರುವ xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(ಎಣಿಕೆ);
            ಪ್ರತಿಜ್ಞಾ(alloc >= ಎಣಿಕೆ);
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ ** ಧ್ರುವ tmp = realloc(
                keyi->overlays_ಕೀಲಿಗಳು,
                (size_t)alloc * ಪರಿಮಾಣ(*keyi->overlays_ಕೀಲಿಗಳು)
            );
            ಯದಿ (!tmp) ಪ್ರತ್ಯಾಗಮ false;
            keyi->overlays_ಕೀಲಿಗಳು = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ಧ್ರುವ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
        ಯದಿ (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ಯದಿ (index < ಎಣಿಕೆ - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ಪ್ರತಿಜ್ಞಾ(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ಕೀಲಿಗಳು + index + 1, keyi->overlays_ಕೀಲಿಗಳು + index,
                    (size_t)(ಎಣಿಕೆ - 1u - index) * ಪರಿಮಾಣ(ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *));
        }
        keyi->overlays_ಕೀಲಿಗಳು[index] = ಕೀಲಿ;
        keyi->overlays = overlays;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ಸ್ಥಿರ bool
merge_overlays(SymbolsInfo *info, ಕೀಲಿ_Info *into, ಕೀಲಿ_Info *from,
               bool clobber, bool report, ಗಣನಾ ಕೀಲಿ_field *collide)
{
    ಯದಿ (!(from->defined & ಕೀಲಿ_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ಅನ್ಯಥಾ ಯದಿ (!(into->defined & ಕೀಲಿ_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ಕೀಲಿಗಳು = from->overlays_ಕೀಲಿಗಳು;
        from->overlays_ಕೀಲಿಗಳು = ರಿಕ್ತ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ಕೀಲಿ_FIELD_OVERLAY;
    } ಅನ್ಯಥಾ ಯದಿ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ಅನ್ಯಥಾ {
        ಯದಿ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ಧ್ರುವ xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ಧ್ರುವ xkb_overlay_index_t ಎಣಿಕೆ =
                (xkb_overlay_index_t)popcount32(result_mask);

            ಯದಿ (ಎಣಿಕೆ == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ಕೀಲಿ_Info *dest = into;
            ಕೀಲಿ_Info *src = from;
            ಯದಿ (into->overlays_alloc >= ಎಣಿಕೆ &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ಅನ್ಯಥಾ ಯದಿ (from->overlays_alloc >= ಎಣಿಕೆ) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ಅನ್ಯಥಾ ಯದಿ (ಎಣಿಕೆ > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ಯದಿ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ಯದಿ (!dest->overlays_alloc) {
                    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **tmp = calloc(ಎಣಿಕೆ, ಪರಿಮಾಣ(*tmp));
                    ಯದಿ (!tmp)
                        ಪ್ರತ್ಯಾಗಮ false;
                    ಯದಿ (dest->overlay_ಕೀಲಿ)
                        tmp[0] = dest->overlay_ಕೀಲಿ;
                    dest->overlays_ಕೀಲಿಗಳು = tmp;
                    dest->overlays_alloc = ಎಣಿಕೆ;
                } ಅನ್ಯಥಾ {
                    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **tmp =
                        realloc(dest->overlays_ಕೀಲಿಗಳು,
                                ಎಣಿಕೆ * ಪರಿಮಾಣ(*dest->overlays_ಕೀಲಿಗಳು));
                    ಯದಿ (!tmp)
                        ಪ್ರತ್ಯಾಗಮ false;
                    dest->overlays_ಕೀಲಿಗಳು = tmp;
                    dest->overlays_alloc = ಎಣಿಕೆ;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **src_ಕೀಲಿಗಳು = (src->overlays_clear)
                ? ರಿಕ್ತ
                : (src->overlays_alloc)
                    ? src->overlays_ಕೀಲಿಗಳು
                    : &src->overlay_ಕೀಲಿ;
            ಯಾವತ್ (remaining) {
                /* isolate lowest set bit */
                ಧ್ರುವ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                ಧ್ರುವ xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                ಯದಿ (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *src_ಕೀಲಿ = (src_ಕೀಲಿಗಳು)
                    ? *(src_ಕೀಲಿಗಳು++)
                    : ರಿಕ್ತ;

                /* check dest */
                ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *dest_ಕೀಲಿ = ರಿಕ್ತ;
                ಧ್ರುವ bool conflict = overlays_get(dest, bit, &dest_ಕೀಲಿ);
                ಯದಿ (conflict) {
                    ಯದಿ (dest_ಕೀಲಿ == src_ಕೀಲಿ)
                       ಅನುವರ್ತ;
                    ಅನ್ಯಥಾ ಯದಿ (report)
                        *collide |= ಕೀಲಿ_FIELD_OVERLAY;
                }

                ಯದಿ ((!conflict || clobber) && !overlays_ಸೇರಿಸು(dest, bit, src_ಕೀಲಿ))
                    ಪ್ರತ್ಯಾಗಮ false;
            }

            ಯದಿ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ಯದಿ (into->overlays_alloc)
                    ಮುಕ್ತ(into->overlays_ಕೀಲಿಗಳು);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ಯದಿ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ಕೀಲಿಗಳು = from->overlays_ಕೀಲಿಗಳು;
                    from->overlays_ಕೀಲಿಗಳು = ರಿಕ್ತ;
                    from->overlays_alloc = 0;
                } ಅನ್ಯಥಾ {
                    into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
                }
            }
        } ಅನ್ಯಥಾ {
            /* Disjoint overlays */

            ಯದಿ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ಕೀಲಿ == from->overlay_ಕೀಲಿ) {
                /* Indentical: do nothing */
                ಪ್ರತ್ಯಾಗಮ true;
            }
            ಯದಿ (!(into->overlays & from->overlays)) {
                ಯದಿ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
                    ಪ್ರತ್ಯಾಗಮ true;
                } ಅನ್ಯಥಾ ಯದಿ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    ಪ್ರತ್ಯಾಗಮ true;
                }
            }
            /* Conflict */
            ಯದಿ (report)
                *collide |= ಕೀಲಿ_FIELD_OVERLAY;
            ಯದಿ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Merge_ಕೀಲಿಗಳು(SymbolsInfo *info, ಕೀಲಿ_Info *into, ಕೀಲಿ_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    ಗಣನಾ ಕೀಲಿ_field collide = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಧ್ರುವ bool clobber = (from->merge != MERGE_AUGMENT);
    ಧ್ರುವ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ಯದಿ (from->merge == MERGE_REPLACE) {
        Clear_ಕೀಲಿ_Info(into);
        *into = *from;
        Init_ಕೀಲಿ_Info(info->ctx, from);
        ಪ್ರತ್ಯಾಗಮ true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    ಪ್ರತಿ (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    ಪ್ರತಿ (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    ಯದಿ (Use_New_ಕೀಲಿ_Field(ಕೀಲಿ_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ಕೀಲಿ_FIELD_VMODMAP;
    }
    ಯದಿ (Use_New_ಕೀಲಿ_Field(ಕೀಲಿ_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ಕೀಲಿ_FIELD_REPEAT;
    }
    ಯದಿ (Use_New_ಕೀಲಿ_Field(ಕೀಲಿ_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ಕೀಲಿ_FIELD_DEFAULT_TYPE;
    }
    ಯದಿ (Use_New_ಕೀಲಿ_Field(ಕೀಲಿ_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_ಸಂಖ್ಯೆ = from->out_of_range_group_ಸಂಖ್ಯೆ;
        into->defined |= ಕೀಲಿ_FIELD_GROUPINFO;
    }

    ಯದಿ (!merge_overlays(info, into, from, clobber, report, &collide))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ಕೀಲಿ_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ಕೀಲಿ_Info(from);
    Init_ಕೀಲಿ_Info(info->ctx, from);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ xkb_atom_t
Xkb_Resolve_ಕೀಲಿ_Alias(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, xkb_atom_t name)
{
    ಯದಿ (name < keymap->ಅಂಕೆ_ಕೀಲಿ_names) {
        ಧ್ರುವ KeycodeMatch match = keymap->ಕೀಲಿ_names[name];
        ಯದಿ (match.found && match.is_alias) {
            ಪ್ರತ್ಯಾಗಮ match.alias.real;
        }
    }
    ಪ್ರತ್ಯಾಗಮ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ಸ್ಥಿರ bool
Add_ಕೀಲಿ_Symbols(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ಕೀಲಿ_Alias(&info->keymap_info->keymap, keyi->name);

    ಕೀಲಿ_Info *iter;
    darray_foreach(iter, info->ಕೀಲಿಗಳು)
        ಯದಿ (iter->name == keyi->name)
            ಪ್ರತ್ಯಾಗಮ Merge_ಕೀಲಿಗಳು(info, iter, keyi, same_file);

    darray_append(info->ಕೀಲಿಗಳು, *keyi);
    Init_ಕೀಲಿ_Info(info->ctx, keyi);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
AddModMapEntry(SymbolsInfo *info, ಧ್ರುವ ModMapEntry *new)
{
    ಧ್ರುವ bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        ಯದಿ ((new->have_ಚಿಹ್ನೆ != old->have_ಚಿಹ್ನೆ) ||
            (new->have_ಚಿಹ್ನೆ && new->u.ಕೀಲಿ_Sym != old->u.ಕೀಲಿ_Sym) ||
            (!new->have_ಚಿಹ್ನೆ && new->u.ಕೀಲಿ_Name != old->u.ಕೀಲಿ_Name))
            ಅನುವರ್ತ;

        ಯದಿ (new->mods == old->mods)
            ಪ್ರತ್ಯಾಗಮ true;

        ಧ್ರುವ xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ಧ್ರುವ xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        ಯದಿ (new->have_ಚಿಹ್ನೆ) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.ಕೀಲಿ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } ಅನ್ಯಥಾ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ಕೀಲಿ_Name_Text(info->ctx, new->u.ಕೀಲಿ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    darray_append(info->modmaps, *new);

    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     ಗಣನಾ merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    ಯದಿ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ಯದಿ (into->name == ರಿಕ್ತ) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    ಪ್ರತಿ (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        ಯದಿ (!darray_item(from->group_names, i))
            ಅನುವರ್ತ;

        ಯದಿ (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            ಅನುವರ್ತ;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    ಯದಿ (darray_empty(into->ಕೀಲಿಗಳು)) {
        into->ಕೀಲಿಗಳು = from->ಕೀಲಿಗಳು;
        darray_init(from->ಕೀಲಿಗಳು);
    }
    ಅನ್ಯಥಾ {
        ಕೀಲಿ_Info *keyi;
        darray_foreach(keyi, from->ಕೀಲಿಗಳು) {
            keyi->merge = merge;
            ಯದಿ (!Add_ಕೀಲಿ_Symbols(into, keyi, false))
                into->error_ಎಣಿಕೆ++;
        }
    }

    ಯದಿ (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ಅನ್ಯಥಾ {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            ಯದಿ (!AddModMapEntry(into, mm))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ಸ್ಥಿರ bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ಯದಿ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ಎಣಿಕೆ += 10;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಪ್ರತಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        ವರ್ಣ path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, ಪರಿಮಾಣ(path));
        ಯದಿ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            ClearSymbolsInfo(&included);
            ಪ್ರತ್ಯಾಗಮ false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        ಯದಿ (stmt->modifier) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            ಯದಿ (ಎಣಿಕೆ <= 0 || *(stmt->modifier + ಎಣಿಕೆ) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } ಅನ್ಯಥಾ {
                next_incl.explicit_group--;
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (info->keymap_info->keymap.ಅಂಕೆ_groups != 0 &&
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
        ಅನ್ಯಥಾ {
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

    ಪ್ರತ್ಯಾಗಮ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
GetGroupIndex(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
              ಗಣನಾ group_field field, xkb_layout_index_t *ndx_rtrn)
{
    ಪ್ರತಿಜ್ಞಾ (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ಧ್ರುವ ವರ್ಣ *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    ಯದಿ (arrayNdx == ರಿಕ್ತ) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ಯದಿ (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                ಪ್ರತ್ಯಾಗಮ true;
            }
        }

        ಯದಿ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ಕೀಲಿ_Info_Text(info, keyi), info->max_groups, name);
            ಪ್ರತ್ಯಾಗಮ false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, ರಿಕ್ತ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ಕೀಲಿ_Info_Text(info, keyi));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    (*ndx_rtrn)--;
    ಯದಿ (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Add_Symbols_To_ಕೀಲಿ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_layout_index_t ndx = 0;

    ಯದಿ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        ಪ್ರತ್ಯಾಗಮ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(ಮೌಲ್ಯ->common.type), ndx + 1,
                ಕೀಲಿ_Info_Text(info, keyi));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ಕೀಲಿ_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ಕೀಲಿ_Info_Text(info, keyi), ndx + 1);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) ಮೌಲ್ಯ;
         keysymList;
         keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        ಯದಿ (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    ಯದಿ (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    ಯದಿ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) ಮೌಲ್ಯ;
         keysymList && level < nLevels;
         keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) keysymList->common.next, level++) {
        ಸಂರಚನಾ xkb_level *leveli = &darray_item(groupi->levels, level);
        ಪ್ರತಿಜ್ಞಾ(leveli->ಅಂಕೆ_syms == 0);

        ಯದಿ (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ಕೀಲಿ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        leveli->ಅಂಕೆ_syms = (xkb_keysym_ಎಣಿಕೆ_t) darray_size(keysymList->syms);
        ವಿಕಲ್ಪ (leveli->ಅಂಕೆ_syms) {
        ಪಕ್ಷ 0:
            leveli->s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
            ಭಙ್ಗ;
        ಪಕ್ಷ 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            ಪ್ರತಿಜ್ಞಾ(leveli->s.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ರಿಕ್ತ);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            ಪ್ರತಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++)
                ಪ್ರತಿಜ್ಞಾ(leveli->s.syms[k] != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
#endif
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Add_Actions_To_ಕೀಲಿ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_layout_index_t ndx = 0;

    ಯದಿ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        ಪ್ರತ್ಯಾಗಮ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 ಮೌಲ್ಯ->common.type, ndx, ಕೀಲಿ_Info_Text(info, keyi));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ಕೀಲಿ_Info_Text(info, keyi), ndx);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    ಪ್ರತಿ (ParseCommon *p = &ಮೌಲ್ಯ->common; p; p = p->next)
        nLevels++;
    ಯದಿ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Action_List *actionList = (ಅಭಿವ್ಯಕ್ತಿ_Action_List *) ಮೌಲ್ಯ;
         actionList;
         actionList = (ಅಭಿವ್ಯಕ್ತಿ_Action_List *) actionList->common.next, level++) {
        ಸಂರಚನಾ xkb_level *leveli = &darray_item(groupi->levels, level);
        ಪ್ರತಿಜ್ಞಾ(leveli->ಅಂಕೆ_actions == 0);

        unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_actions = 0;
        ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = actionList->actions;
             act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.next)
            ಅಂಕೆ_actions++;

        ಯದಿ (unlikely(ಅಂಕೆ_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ಕೀಲಿ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, ಅಂಕೆ_actions);
            ಪ್ರತ್ಯಾಗಮ false;
        }

        /* Parse actions and add only defined actions */
        darray(ಸಙ್ಘ xkb_action) actions = darray_new();
        ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = actionList->actions;
             act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.next) {
            ಸಙ್ಘ xkb_action toAct = { 0 };
            ಧ್ರುವ ಗಣನಾ xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            ಯದಿ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ಕೀಲಿ_Info_Text(info, keyi), ndx + 1, level + 1);
                ಯದಿ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    ಪ್ರತ್ಯಾಗಮ false;
                } ಅನ್ಯಥಾ {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            ಯದಿ (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ಅನ್ಯಥಾ ಯದಿ (likely(ಅಂಕೆ_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->ಅಂಕೆ_actions = 1;
                leveli->a.action = toAct;
                ಗಚ್ಛ next_level;
            } ಅನ್ಯಥಾ {
                darray_append(actions, toAct);
            }
        }
        ಯದಿ (darray_empty(actions)) {
            leveli->ಅಂಕೆ_actions = 0;
        } ಅನ್ಯಥಾ ಯದಿ (likely(darray_size(actions) > 1)) {
            leveli->ಅಂಕೆ_actions = (xkb_action_ಎಣಿಕೆ_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ರಿಕ್ತ);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_actions; k++)
                ಪ್ರತಿಜ್ಞಾ(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } ಅನ್ಯಥಾ {
            /* Unlikely: some actions were dropped and only one remains */
            ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_actions > darray_size(actions));
            leveli->ಅಂಕೆ_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            ಪ್ರತಿಜ್ಞಾ(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ಯದಿ (leveli->ಅಂಕೆ_actions > 0 || leveli->ಅಂಕೆ_syms > 0)
            nonEmptyLevels = level + 1;
    }

    ಯದಿ (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        ಯದಿ (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        ಅನ್ಯಥಾ
            darray_free(groupi->levels);
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಧ್ರುವ LookupEntry repeatEntries[] = {
    { "true", ಕೀಲಿ_REPEAT_YES },
    { "yes", ಕೀಲಿ_REPEAT_YES },
    { "on", ಕೀಲಿ_REPEAT_YES },
    { "false", ಕೀಲಿ_REPEAT_NO },
    { "no", ಕೀಲಿ_REPEAT_NO },
    { "off", ಕೀಲಿ_REPEAT_NO },
    { "default", ಕೀಲಿ_REPEAT_UNDEFINED },
    { ರಿಕ್ತ, 0 }
};

/** Handle overlay<N> = <key> entry */
ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Overlay_Entry(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                        ಧ್ರುವ ವರ್ಣ *field, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಕೀಲಿ_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **ಕೀಲಿ_rtrn)
{
    /* Overlay index */
    ಯದಿ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, ಕೀಲಿ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        ಪ್ರತ್ಯಾಗಮ false;
    }
    static_assert(ಪರಿಮಾಣ(xkb_overlay_index_t) == ಪರಿಮಾಣ(uint8_t), "");
    ಧ್ರುವ size_t prefix = ಪರಿಮಾಣ("overlay") - 1;
    ಧ್ರುವ size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ಯದಿ (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (ಪೂರ್ಣಾಙ್ಕ)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, ಕೀಲಿ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        ಪ್ರತ್ಯಾಗಮ false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
        *ಕೀಲಿ_rtrn = Xkb_ಕೀಲಿ_By_Name(&keymap_info->keymap, ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name,
                                 false);
        ಯದಿ (!*ಕೀಲಿ_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name),
                    field, ಕೀಲಿ_Name_Text(keymap_info->keymap.ctx, keyi->name));
            ಪ್ರತ್ಯಾಗಮ false;
        }
        ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT: {
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ id =
            xkb_atom_text(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಯದಿ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ಕೀಲಿ_rtrn = ರಿಕ್ತ;
            ಪ್ರತ್ಯಾಗಮ true;
        } ಅನ್ಯಥಾ ಯದಿ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ಕೀಲಿ_rtrn = ರಿಕ್ತ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            ಪ್ರತ್ಯಾಗಮ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, ಕೀಲಿ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಸ್ವಭಾವ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL),
                field, ಕೀಲಿ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;
    }
}

ಸ್ಥಿರ bool
SetSymbolsField(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಧ್ರುವ ವರ್ಣ *field,
                ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def * restrict ಧ್ರುವ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;
    ಯದಿ (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            ಪ್ರತ್ಯಾಗಮ false;
        }

        ಯದಿ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ಕೀಲಿ_FIELD_DEFAULT_TYPE;
        }
        ಅನ್ಯಥಾ ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, ರಿಕ್ತ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }
        ಅನ್ಯಥಾ {
            ndx--;
            ಯದಿ (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ಪ್ರತಿಜ್ಞಾ(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "symbols")) {
        ಪ್ರತ್ಯಾಗಮ Add_Symbols_To_ಕೀಲಿ(info, keyi, arrayNdx, ಮೌಲ್ಯ);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "actions")) {
        ಪ್ರತ್ಯಾಗಮ Add_Actions_To_ಕೀಲಿ(info, keyi, arrayNdx, ಮೌಲ್ಯ);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, ಮೌಲ್ಯ, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(ಮೌಲ್ಯ->common.type),
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= ಕೀಲಿ_FIELD_VMODMAP;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ಕೀಲಿ_Info_Text(info, keyi));
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ಕೀಲಿ_Info_Text(info, keyi));
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ಕೀಲಿ_Info_Text(info, keyi));
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = ರಿಕ್ತ;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *ಮೌಲ್ಯ_ptr, keyi, &overlay, &ಕೀಲಿ))
            ಪ್ರತ್ಯಾಗಮ false;
        ಯದಿ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            ಪ್ರತ್ಯಾಗಮ true;
        } ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ && ಕೀಲಿ->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ಕೀಲಿ_Info_Text(info, keyi));
        } ಅನ್ಯಥಾ {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *prev;
            ಯದಿ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ಯದಿ (ಕೀಲಿ != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ಕೀಲಿ_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? ಕೀಲಿ_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ಕೀಲಿ
                                 ? ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ->name)
                                 : "none"));
                }
            } ಅನ್ಯಥಾ ಯದಿ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ಯದಿ (!overlays_ಸೇರಿಸು(keyi, overlay, ಕೀಲಿ))
                    ಪ್ರತ್ಯಾಗಮ false;
                keyi->defined |= ಕೀಲಿ_FIELD_OVERLAY;
            } ಅನ್ಯಥಾ {
                /* Handle disjoint overlays mode */
                ಧ್ರುವ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                ಯದಿ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ಯದಿ (ಕೀಲಿ) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
                    } ಅನ್ಯಥಾ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ಕೀಲಿ_FIELD_OVERLAY;
                } ಅನ್ಯಥಾ ಯದಿ (keyi->overlays) {
                    ಯದಿ (ಕೀಲಿ) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ಕೀಲಿ_Info_Text(info, keyi), keyi->overlays,
                                ಕೀಲಿ_Name_Text(info->ctx, keyi->overlay_ಕೀಲಿ->name),
                                overlay + 1, ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ->name));
                        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_ಮೌಲ್ಯ_MISMATCH);
                    } ಅನ್ಯಥಾ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(info->ctx, ಮೌಲ್ಯ, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        keyi->repeat = (ಗಣನಾ ಕೀಲಿ_repeat) val;
        keyi->defined |= ಕೀಲಿ_FIELD_REPEAT;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= ಕೀಲಿ_FIELD_GROUPINFO;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= ಕೀಲಿ_FIELD_GROUPINFO;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info->keymap_info, ಮೌಲ್ಯ, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ಕೀಲಿ_Info_Text(info, keyi));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        ಯದಿ (pending) {
            keyi->out_of_range_pending_group = true;
            ಧ್ರುವ darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ಸಂರಚನಾ pending_computation) {
                    .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                    .computed = false,
                    .ಮೌಲ್ಯ = 0,
                }
            );
            *ಮೌಲ್ಯ_ptr = ರಿಕ್ತ;
            static_assert(ಪರಿಮಾಣ(keyi->out_of_range_group_ಸಂಖ್ಯೆ) ==
                          ಪರಿಮಾಣ(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_ಸಂಖ್ಯೆ = pending_index;
        } ಅನ್ಯಥಾ {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_ಸಂಖ್ಯೆ = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= ಕೀಲಿ_FIELD_GROUPINFO;
    }
    ಅನ್ಯಥಾ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ಕೀಲಿ_FIELDS);
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
SetGroupName(SymbolsInfo *info, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
             ಗಣನಾ merge_mode merge)
{
    ಯದಿ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_layout_index_t group = 0;
    ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info->keymap_info, arrayNdx, false, &group, ರಿಕ್ತ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_layout_index_t group_to_use;
    ಯದಿ (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    ಅನ್ಯಥಾ ಯದಿ (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    ಅನ್ಯಥಾ {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        ಪ್ರತಿಜ್ಞಾ(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } ಅನ್ಯಥಾ {
        ಧ್ರುವ xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        ಯದಿ (old_name != XKB_ATOM_NONE && old_name != name) {
            ಧ್ರುವ bool replace = (merge != MERGE_AUGMENT);
            ಧ್ರುವ xkb_atom_t use    = (replace ? name : old_name);
            ಧ್ರುವ xkb_atom_t ignore = (replace ? old_name : name);
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

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
    bool ret;

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (elem && istreq(elem, "key")) {
        ಕೀಲಿ_Info temp = {0};
        Init_ಕೀಲಿ_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->ಮೌಲ್ಯ) &&
              Merge_ಕೀಲಿಗಳು(info, &info->default_ಕೀಲಿ, &temp, true);
        Clear_ಕೀಲಿ_Info(&temp);
    }
    ಅನ್ಯಥಾ ಯದಿ (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->ಮೌಲ್ಯ, stmt->merge);
    }
    ಅನ್ಯಥಾ ಯದಿ (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ಅನ್ಯಥಾ ಯದಿ (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ಅನ್ಯಥಾ ಯದಿ (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ಅನ್ಯಥಾ ಯದಿ (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ಅನ್ಯಥಾ ಯದಿ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->ಮೌಲ್ಯ, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } ಅನ್ಯಥಾ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ಕೀಲಿ_Info *keyi)
{
    bool all_valid_entries = true;

    ಪ್ರತಿ (; def; def = (VarDef *) def->common.next) {
        ಧ್ರುವ ವರ್ಣ *field;
        ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
        bool ok = true;

        ಯದಿ (!def->name) {
            ಯದಿ (unlikely(!def->ಮೌಲ್ಯ) || def->ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            ಅನ್ಯಥಾ
                field = "actions";
            arrayNdx = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ {
            ಧ್ರುವ ವರ್ಣ *elem;
            ok = ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            ಯದಿ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        ಯದಿ (unlikely(!def->ಮೌಲ್ಯ)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        ಯದಿ (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->ಮೌಲ್ಯ))
            all_valid_entries = false;
    }

    ಪ್ರತ್ಯಾಗಮ all_valid_entries;
}

ಸ್ಥಿರ bool
SetExplicitGroup(SymbolsInfo *info, ಕೀಲಿ_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    ಯದಿ (info->explicit_group == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ಯದಿ (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    ಯದಿ (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, ಕೀಲಿ_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    ಯದಿ (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ಕೀಲಿ_Info keyi;

    keyi = info->default_ಕೀಲಿ;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ಕೀಲಿ.groups);
    ಪ್ರತಿ (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_ಕೀಲಿ.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->ಕೀಲಿ_Name;

    ಯದಿ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_ಕೀಲಿ_Symbols(info, &keyi, true)) {
        ಪ್ರತ್ಯಾಗಮ true;
    }

    Clear_ಕೀಲಿ_Info(&keyi);
    info->error_ಎಣಿಕೆ++;
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    ಸಂರಚನಾ xkb_context *ctx = info->ctx;

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ಪ್ರತಿಜ್ಞಾ(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿ = def->ಕೀಲಿಗಳು; ಕೀಲಿ; ಕೀಲಿ = (ಅಭಿವ್ಯಕ್ತಿ_Def *) ಕೀಲಿ->common.next) {
        ಯದಿ (ಕೀಲಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL) {
            tmp.have_ಚಿಹ್ನೆ = false;
            tmp.u.ಕೀಲಿ_Name = ಕೀಲಿ->ಕೀಲಿ_name.ಕೀಲಿ_name;
        }
        ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL) {
            ಯದಿ (ಕೀಲಿ->keysym.keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                /* Invalid keysym: ignore. Error message already printed */
                ಅನುವರ್ತ;
            } ಅನ್ಯಥಾ {
                tmp.have_ಚಿಹ್ನೆ = true;
                tmp.u.ಕೀಲಿ_Sym = ಕೀಲಿ->keysym.keysym;
            }
        }
        ಅನ್ಯಥಾ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ಅನುವರ್ತ;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    ಮುಕ್ತ(info->name);
    info->name = strdup_safe(file->name);

    ಪ್ರತಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ವಿಕಲ್ಪ (stmt->type) {
        ಪಕ್ಷ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_UNKNOWN_DECLARATION:
        ಪಕ್ಷ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ಭಙ್ಗ;
        }

        ಯದಿ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಯದಿ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            ಭಙ್ಗ;
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
ಸ್ಥಿರ ಸಂರಚನಾ xkb_ಕೀಲಿ *
Find_ಕೀಲಿ_For_ಚಿಹ್ನೆ(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    ಕುರು {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        ಕುರು {
            got_one_level = false;
            ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
            xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
                ಯದಿ (group < ಕೀಲಿ->ಅಂಕೆ_groups &&
                    level < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, group)) {
                    got_one_group = got_one_level = true;
                    ಧ್ರುವ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms =
                        ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_syms;
                    ಯದಿ (ಅಂಕೆ_syms > 1) {
                        ಪ್ರತಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < ಅಂಕೆ_syms; k++) {
                            ಯದಿ (ಕೀಲಿ->groups[group].levels[level].s.syms[k] == sym)
                                ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
                        }
                    } ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_syms &&
                               ಕೀಲಿ->groups[group].levels[level].s.sym == sym) {
                        ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
                    }
                }
            }
            level++;
        } ಯಾವತ್ (got_one_level);
        group++;
    } ಯಾವತ್ (got_one_group);

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
ಸ್ಥಿರ xkb_atom_t
FindAutomaticType(ಸಂರಚನಾ xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ಧ್ರುವ xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).ಅಂಕೆ_syms == 0 ? \
        XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ : \
     darray_item(groupi->levels, level).ಅಂಕೆ_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    ಯದಿ (width == 1 || width <= 0)
        ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ಯದಿ (width == 2) {
        ಯದಿ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ಯದಿ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "KEYPAD");

        ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ಯದಿ (width <= 4) {
        ಯದಿ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);

            ಯದಿ (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ಯದಿ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        ಪ್ರತ್ಯಾಗಮ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;

#undef GET_SYM
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *
FindTypeForGroup(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ಯದಿ (type_name == XKB_ATOM_NONE) {
        ಯದಿ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ಅನ್ಯಥಾ {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            ಯದಿ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ಯದಿ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ಕೀಲಿ_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ಕೀಲಿ_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        ಗಚ್ಛ use_default;
    }

    darray_size_t i;
    ಪ್ರತಿ (i = 0; i < keymap->ಅಂಕೆ_types; i++)
        ಯದಿ (keymap->types[i].name == type_name)
            ಭಙ್ಗ;

    ಯದಿ (i >= keymap->ಅಂಕೆ_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ಕೀಲಿ_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 ಕೀಲಿ_Name_Text(keymap->ctx, keyi->name), group + 1);
        ಗಚ್ಛ use_default;
    }

    keymap->types[i].required = true;
    ಪ್ರತ್ಯಾಗಮ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    ಪ್ರತ್ಯಾಗಮ &keymap->types[0];
}

ಸ್ಥಿರ bool
CopySymbolsDefToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, SymbolsInfo *info,
                       ಕೀಲಿ_Info *keyi)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    GroupInfo *groupi;
    ಧ್ರುವ GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ಕೀಲಿ = Xkb_ಕೀಲಿ_By_Name(keymap, keyi->name, false);
    ಯದಿ (!ಕೀಲಿ) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ಕೀಲಿ_Info_Text(info, keyi));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /* Find the range of groups we need. */
    ಕೀಲಿ->ಅಂಕೆ_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ಧ್ರುವ bool has_explicit_type = (keyi->defined & ಕೀಲಿ_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        ಯದಿ (darray_size(groupi->levels) > 0 || has_explicit_type)
            ಕೀಲಿ->ಅಂಕೆ_groups = i + 1;
        ಯದಿ (has_explicit_type)
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
    }

    ಯದಿ (ಕೀಲಿ->ಅಂಕೆ_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ಯದಿ (keyi->defined)
            ಗಚ್ಛ ಕೀಲಿ_fields;
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ false;
    }

    darray_resize(keyi->groups, ಕೀಲಿ->ಅಂಕೆ_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ಯದಿ (groupi->defined)
            ಅನುವರ್ತ;

        CopyGroupInfo(groupi, group0);
    }

    ಕೀಲಿ->groups = calloc(ಕೀಲಿ->ಅಂಕೆ_groups, ಪರಿಮಾಣ(*ಕೀಲಿ->groups));
    ಪ್ರತಿಜ್ಞಾ(darray_size(keyi->groups) == ಕೀಲಿ->ಅಂಕೆ_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ಕೀಲಿ->ಅಂಕೆ_groups);
        #endif
        bool explicit_type = false;
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type * ಧ್ರುವ type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ಯದಿ (type->ಅಂಕೆ_levels < darray_size(groupi->levels)) {
            ಸಂರಚನಾ xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->ಅಂಕೆ_levels,
                    ಕೀಲಿ_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->ಅಂಕೆ_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->ಅಂಕೆ_levels);

        ಕೀಲಿ->groups[i].explicit_type = explicit_type;
        ಕೀಲಿ->groups[i].type = type;
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
        ಸಂರಚನಾ xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            ವಿಕಲ್ಪ (leveli->ಅಂಕೆ_syms) {
            ಪಕ್ಷ 0:
                leveli->upper = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                ಭಙ್ಗ;
            ಪಕ್ಷ 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                ಭಙ್ಗ;
            ಸ್ವಭಾವ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                ಪ್ರತಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++) {
                    ಧ್ರುವ xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    ಯದಿ (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        ಭಙ್ಗ;
                    }
                }
                ಯದಿ (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->ಅಂಕೆ_syms *
                                             ಪರಿಮಾಣ(*leveli->s.syms));
                    ಯದಿ (!leveli->s.syms)
                        ಪ್ರತ್ಯಾಗಮ false; /* FIXME: better handling? */
                    ಪ್ರತಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++) {
                        leveli->s.syms[leveli->ಅಂಕೆ_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &ಕೀಲಿ->groups[i].levels, ರಿಕ್ತ);
        ಯದಿ (ಕೀಲಿ->groups[i].type->ಅಂಕೆ_levels > 1 ||
            ಕೀಲಿ->groups[i].levels[0].ಅಂಕೆ_syms > 0) {
            ಕೀಲಿ->groups[i].explicit_symbols = true;
            ಕೀಲಿ->explicit |= EXPLICIT_SYMBOLS;
        }
        ಯದಿ (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            ಕೀಲಿ->groups[i].explicit_actions = true;
            ಕೀಲಿ->explicit |= EXPLICIT_INTERP;
        }
        ಯದಿ (ಕೀಲಿ->groups[i].explicit_type)
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
    }

    ಕೀಲಿ->out_of_range_pending_group = keyi->out_of_range_pending_group;
    ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ = keyi->out_of_range_group_ಸಂಖ್ಯೆ;
    ಕೀಲಿ->out_of_range_group_policy = keyi->out_of_range_group_policy;

ಕೀಲಿ_fields:
    ಯದಿ (keyi->defined & ಕೀಲಿ_FIELD_VMODMAP) {
        ಕೀಲಿ->vmodmap = keyi->vmodmap;
        ಕೀಲಿ->explicit |= EXPLICIT_VMODMAP;
    }

    ಯದಿ (keyi->repeat != ಕೀಲಿ_REPEAT_UNDEFINED) {
        ಕೀಲಿ->repeats = (keyi->repeat == ಕೀಲಿ_REPEAT_YES);
        ಕೀಲಿ->explicit |= EXPLICIT_REPEAT;
    }

    ಯದಿ ((keyi->defined & ಕೀಲಿ_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ಕೀಲಿ->overlays = keyi->overlays;
        ಯದಿ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = ಕೀಲಿ->overlays;
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು = keyi->overlays_ಕೀಲಿಗಳು;
            ಯಾವತ್ (remaining) {
                /* isolate lowest set bit */
                ಧ್ರುವ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                ಯದಿ (*overlays_ಕೀಲಿಗಳು) {
                    overlays_ಕೀಲಿಗಳು++;
                } ಅನ್ಯಥಾ {
                    /* drop current null value (overlayN=none) */
                    ಕೀಲಿ->overlays &= ~lsb;
                }
            }

            ಯದಿ (ಕೀಲಿ->overlays) {
                /* Steal */
                ಕೀಲಿ->overlays_ಕೀಲಿಗಳು = keyi->overlays_ಕೀಲಿಗಳು;
                keyi->overlays_ಕೀಲಿಗಳು = ರಿಕ್ತ;
                keyi->overlays_alloc = 0;

                ಕೀಲಿ->overlays_inline = false;
                ಕೀಲಿ->explicit |= EXPLICIT_OVERLAY;
            }
        } ಅನ್ಯಥಾ {
            ಕೀಲಿ->overlay_ಕೀಲಿ = keyi->overlay_ಕೀಲಿ;
            ಕೀಲಿ->overlays_inline = true;
            ಕೀಲಿ->explicit |= EXPLICIT_OVERLAY;
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
CopyModMapDefToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

    ಯದಿ (!entry->have_ಚಿಹ್ನೆ) {
        ಕೀಲಿ = Xkb_ಕೀಲಿ_By_Name(keymap, entry->u.ಕೀಲಿ_Name, true);
        ಯದಿ (!ಕೀಲಿ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ಕೀಲಿ_Name_Text(info->ctx, entry->u.ಕೀಲಿ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }
    ಅನ್ಯಥಾ {
        ಕೀಲಿ = Find_ಕೀಲಿ_For_ಚಿಹ್ನೆ(keymap, entry->u.ಕೀಲಿ_Sym);
        ಯದಿ (!ಕೀಲಿ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_ಚಿಹ್ನೆ,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.ಕೀಲಿ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

    /* Skip modMap None */
    ಯದಿ (entry->mods) {
        ಕೀಲಿ->modmap |= entry->mods;
        entry->ಕೀಲಿ_Code = ಕೀಲಿ->keycode;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
CopySymbolsToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, SymbolsInfo *info)
{
    ಕೀಲಿ_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->ಅಂಕೆ_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, ರಿಕ್ತ);

    darray_foreach(keyi, info->ಕೀಲಿಗಳು)
        ಯದಿ (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_ಎಣಿಕೆ++;

    ಯದಿ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

        xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
            ಯದಿ (ಕೀಲಿ->name == XKB_ATOM_NONE)
                ಅನುವರ್ತ;

            ಯದಿ (ಕೀಲಿ->ಅಂಕೆ_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         ಕೀಲಿ_Name_Text(info->ctx, ಕೀಲಿ->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        ಯದಿ (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_ಎಣಿಕೆ++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->ಅಂಕೆ_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
CompileSymbols(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ಯದಿ (file !=ರಿಕ್ತ)
        HandleSymbolsFile(&info, file);

    ಯದಿ (info.error_ಎಣಿಕೆ != 0)
        ಗಚ್ಛ err_info;

    ಯದಿ (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        ಗಚ್ಛ err_info;

    ClearSymbolsInfo(&info);
    ಪ್ರತ್ಯಾಗಮ true;

err_info:
    ClearSymbolsInfo(&info);
    ಪ್ರತ್ಯಾಗಮ false;
}
