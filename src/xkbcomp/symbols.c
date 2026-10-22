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

#شامل "config.h"

#شامل <limits.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon-keysyms.h"

#شامل "action.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "keymap.h"
#شامل "keysym.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "utils.h"
#شامل "util-numbers.h"
#شامل "util-mem.h"
#شامل "vmod.h"
#شامل "xkbcomp-priv.h"
#شامل "xkbcomp/ast.h"

شمارش کلید_repeat {
    کلید_REPEAT_UNDEFINED = 0,
    کلید_REPEAT_YES,
    کلید_REPEAT_NO,
    کلید_REPEAT_رقم_ENTRIES,
};
شمارش { کلید_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(کلید_REPEAT_رقم_ENTRIES - 1 <=
              (1 << (کلید_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

شمارش group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

شمارش کلید_field {
    کلید_FIELD_NONE = 0,
    کلید_FIELD_REPEAT = (1 << 0),
    کلید_FIELD_DEFAULT_TYPE = (1 << 1),
    کلید_FIELD_GROUPINFO = (1 << 2),
    کلید_FIELD_VMODMAP = (1 << 3),
    کلید_FIELD_OVERLAY = (1 << 4),
    کلید_FIELD_ALL = کلید_FIELD_REPEAT | کلید_FIELD_DEFAULT_TYPE
                  | کلید_FIELD_GROUPINFO | کلید_FIELD_VMODMAP | کلید_FIELD_OVERLAY,
};
شمارش { کلید_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(کلید_FIELD_ALL <= (1 << (کلید_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

تعریفنوع ساختار {
    darray(ساختار xkb_level) levels;
    شمارش group_field defined;
    xkb_atom_t type;
} GroupInfo;

شمارش {
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
              (unsigned)کلید_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)کلید_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

تعریفنوع ساختار {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_عدد;
    darray(GroupInfo) groups;
    شمارش xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    شمارش کلید_field defined:KEYINFO_DEFINED_BIT_FIELD;
    شمارش merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    شمارش کلید_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    اجتماع {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ثابت ساختار xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت ساختار xkb_کلید **overlays_کلیدها;
    };
} کلید_Info;

ایستا تهی
StealLevelInfo(ساختار xkb_level *into, ساختار xkb_level *from)
{
    clear_level(into);
    اگر (from->رقم_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } وگرنه {
        into->s.sym = from->s.sym;
    }
    into->رقم_syms = from->رقم_syms;
    from->رقم_syms = 0;

    اگر (from->رقم_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } وگرنه {
        into->a.action = from->a.action;
    }
    into->رقم_actions = from->رقم_actions;
    from->رقم_actions = 0;
}

ایستا تهی
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, اندازه(*groupi));
}

ایستا تهی
ClearGroupInfo(GroupInfo *groupi)
{
    ساختار xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

ایستا تهی
CopyGroupInfo(GroupInfo *to, ثابت GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    برای (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        اگر (darray_item(from->levels, j).رقم_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).رقم_syms,
                       اندازه(xkb_keysym_t));
        }
        اگر (darray_item(from->levels, j).رقم_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).رقم_actions,
                       اندازه(اجتماع xkb_action));
        }
    }
}

ایستا تهی
Init_کلید_Info(ساختار xkb_context *ctx, کلید_Info *keyi)
{
    memset(keyi, 0, اندازه(*keyi));
    static_assert(!DEFAULT_کلید_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_کلید_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_کلید = پوچ;
}

ایستا تهی
Clear_کلید_Info(کلید_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    اگر (keyi->overlays_alloc)
        آزاد(keyi->overlays_کلیدها);
}

/***====================================================================***/

تعریفنوع ساختار {
    نویسه *name;         /* e.g. pc+us+inet(evdev) */
    صحیح error_شمار;
    unsigned صحیح include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(کلید_Info) کلیدها;
    کلید_Info default_کلید;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    ساختار xkb_mod_set mods;

    ساختار xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت ساختار xkb_keymap_info *keymap_info;
} SymbolsInfo;

ایستا تهی
InitSymbolsInfo(SymbolsInfo *info, ثابت ساختار xkb_keymap_info *keymap_info,
                unsigned صحیح include_depth, ثابت ساختار xkb_mod_set *mods)
{
    memset(info, 0, اندازه(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_کلید_Info(keymap_info->keymap.ctx, &info->default_کلید);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

ایستا تهی
ClearSymbolsInfo(SymbolsInfo *info)
{
    کلید_Info *keyi;
    آزاد(info->name);
    darray_foreach(keyi, info->کلیدها)
        Clear_کلید_Info(keyi);
    darray_free(info->کلیدها);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_کلید_Info(&info->default_کلید);
}

ایستا ثابت نویسه *
کلید_Info_Text(SymbolsInfo *info, کلید_Info *keyi)
{
    بازگشت کلید_Name_Text(info->ctx, keyi->name);
}

ایستا bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t کلید_name)
{
    /* First find the type of the merged group. */
    اگر (into->type != from->type) {
        اگر (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        وگرنه اگر (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        وگرنه {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            اگر (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, کلید_Name_Text(info->ctx, کلید_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    اگر (darray_empty(from->levels)) {
        InitGroupInfo(from);
        بازگشت true;
    }

    اگر (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        بازگشت true;
    }

    /* Merge the actions and syms. */
    ثابت darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_شمار = 0;
    darray_size_t from_Actions_شمار = 0;
    برای (darray_size_t i = 0; i < levels_in_both; i++) {
        ساختار xkb_level* ثابت intoLevel = &darray_item(into->levels, i);
        ساختار xkb_level* ثابت fromLevel = &darray_item(from->levels, i);

        ثابت bool fromHasNoKeysym = fromLevel->رقم_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->رقم_actions == 0;
        اگر (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ادامه;
        }

        ثابت bool intoHasNoKeysym = intoLevel->رقم_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->رقم_actions == 0;
        اگر (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_شمار++;
            from_Actions_شمار++;
            ادامه;
        }

        /* Possible level conflict */
        أكد(intoLevel->رقم_syms > 0 || intoLevel->رقم_actions > 0);
        أكد(fromLevel->رقم_syms > 0 || fromLevel->رقم_actions > 0);

        /* Handle keysyms */
        اگر (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            اگر (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_نماد,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, کلید_Name_Text(info->ctx, کلید_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            اگر (fromHasNoKeysym) {
                /* No keysym to copy */
            } وگرنه اگر (clobber) {
                /* Override: copy any defined keysym from `from` */
                اگر (unlikely(fromLevel->رقم_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    اگر (unlikely(intoLevel->رقم_syms > 1))
                        آزاد(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->رقم_syms = fromLevel->رقم_syms;
                    fromLevel->رقم_syms = 0;
                    from_Keysyms_شمار++;
                } وگرنه اگر (fromLevel->s.sym != XKB_کلید_No_نماد) {
                    /* Single defined keysym */
                    اگر (unlikely(intoLevel->رقم_syms > 1))
                        آزاد(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->رقم_syms = 1;
                    from_Keysyms_شمار++;
                }
            } وگرنه {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                اگر (unlikely(intoLevel->رقم_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وگرنه اگر (intoLevel->s.sym == XKB_کلید_No_نماد) {
                    /* Single undefined keysym */
                    اگر (unlikely(fromLevel->رقم_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->رقم_syms = fromLevel->رقم_syms;
                        fromLevel->رقم_syms = 0;
                    } وگرنه {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->رقم_syms = 1;
                    }
                    from_Keysyms_شمار++;
                }
            }
        }

        /* Handle actions */
        اگر (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            اگر (report && !(intoHasNoAction || fromHasNoAction)) {
                اگر (unlikely(intoLevel->رقم_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, کلید_Name_Text(info->ctx, کلید_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } وگرنه {
                    اجتماع xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                کلید_Name_Text(info->ctx, کلید_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            اگر (fromHasNoAction) {
                /* No action to copy */
            } وگرنه اگر (clobber) {
                /* Override: copy any defined action from `from` */
                اگر (unlikely(fromLevel->رقم_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    اگر (unlikely(intoLevel->رقم_actions > 1))
                        آزاد(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->رقم_actions = fromLevel->رقم_actions;
                    fromLevel->رقم_actions = 0;
                    from_Actions_شمار++;
                } وگرنه اگر (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    اگر (unlikely(intoLevel->رقم_actions > 1))
                        آزاد(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->رقم_actions = 1;
                    from_Actions_شمار++;
                }
            } وگرنه {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                اگر (unlikely(intoLevel->رقم_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وگرنه اگر (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    اگر (unlikely(fromLevel->رقم_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->رقم_actions = fromLevel->رقم_actions;
                        fromLevel->رقم_actions = 0;
                    } وگرنه {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->رقم_actions = 1;
                    }
                    from_Actions_شمار++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ساختار xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->رقم_syms = 0;
        level->رقم_actions = 0;
        from_Keysyms_شمار++;
        from_Actions_شمار++;
    }
    اگر (from_Keysyms_شمار) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        اگر (from_Keysyms_شمار == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    اگر (from_Actions_شمار) {
        /* Reset defined actions field if we used no action from `into` */
        اگر (from_Actions_شمار == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    بازگشت true;
}

ایستا bool
Use_New_کلید_Field(شمارش کلید_field field, شمارش کلید_field old, شمارش کلید_field new,
               bool clobber, bool report, شمارش کلید_field *collide)
{
    اگر (!(old & field))
        بازگشت (new & field);

    اگر (new & field) {
        اگر (report)
            *collide |= field;

        بازگشت clobber;
    }

    بازگشت false;
}

ایستا bool
overlays_get(ثابت کلید_Info *info, xkb_overlay_index_t bit,
             ثابت ساختار xkb_کلید **کلید_out)
{
    اگر (bit >= (xkb_overlay_index_t)(اندازه(xkb_overlay_mask_t) * CHAR_BIT))
        بازگشت false;
    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    اگر (!(info->overlays & mask)) بازگشت false;
    اگر (کلید_out) {
        اگر (!info->overlays_alloc) {
            /* Stored inline */
            *کلید_out = info->overlay_کلید;
        } وگرنه {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(کم);
            *کلید_out = info->overlays_کلیدها[index];
        }
    }
    بازگشت true;
}

ایستا bool
overlays_درج(کلید_Info *keyi, xkb_overlay_index_t bit,
                ثابت ساختار xkb_کلید *کلید)
{
    اگر (bit >= (xkb_overlay_index_t)(اندازه(xkb_overlay_mask_t) * CHAR_BIT))
        بازگشت false;

    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    اگر ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        اگر (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_کلید = کلید;
            اگر (!کلید)
                keyi->overlays_clear = true;
        } وگرنه {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(کم);
            keyi->overlays_کلیدها[index] = کلید;
        }
        بازگشت true;
    }

    /* Insert new element */
    اگر (!keyi->overlays || (keyi->overlays_clear && !کلید)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_کلید = کلید;
        keyi->overlays |= mask;
        keyi->overlays_clear = !کلید;
    } وگرنه اگر (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ثابت xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت ساختار xkb_کلید ** ثابت tmp = calloc(alloc, اندازه(*tmp));
        اگر (!tmp) بازگشت false;

        /* Copy previous entries to the buffer */
        اگر (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(کم);
            tmp[idx] = keyi->overlay_کلید;
        } وگرنه {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(کم);
        tmp[idx] = کلید;

        keyi->overlays_کلیدها = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وگرنه {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ثابت xkb_overlay_index_t شمار =
            (xkb_overlay_index_t)popcount32(overlays);
        اگر (شمار > keyi->overlays_alloc) {
            ثابت xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(شمار);
            أكد(alloc >= شمار);
            ثابت ساختار xkb_کلید ** ثابت tmp = realloc(
                keyi->overlays_کلیدها,
                (size_t)alloc * اندازه(*keyi->overlays_کلیدها)
            );
            اگر (!tmp) بازگشت false;
            keyi->overlays_کلیدها = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(کم);
        اگر (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        اگر (index < شمار - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_کلیدها + index + 1, keyi->overlays_کلیدها + index,
                    (size_t)(شمار - 1u - index) * اندازه(ثابت ساختار xkb_کلید *));
        }
        keyi->overlays_کلیدها[index] = کلید;
        keyi->overlays = overlays;
    }

    بازگشت true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ایستا bool
merge_overlays(SymbolsInfo *info, کلید_Info *into, کلید_Info *from,
               bool clobber, bool report, شمارش کلید_field *collide)
{
    اگر (!(from->defined & کلید_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وگرنه اگر (!(into->defined & کلید_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_کلیدها = from->overlays_کلیدها;
        from->overlays_کلیدها = پوچ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= کلید_FIELD_OVERLAY;
    } وگرنه اگر (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وگرنه {
        اگر (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_index_t شمار =
                (xkb_overlay_index_t)popcount32(result_mask);

            اگر (شمار == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            کلید_Info *dest = into;
            کلید_Info *src = from;
            اگر (into->overlays_alloc >= شمار &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وگرنه اگر (from->overlays_alloc >= شمار) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وگرنه اگر (شمار > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                اگر (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                اگر (!dest->overlays_alloc) {
                    ثابت ساختار xkb_کلید **tmp = calloc(شمار, اندازه(*tmp));
                    اگر (!tmp)
                        بازگشت false;
                    اگر (dest->overlay_کلید)
                        tmp[0] = dest->overlay_کلید;
                    dest->overlays_کلیدها = tmp;
                    dest->overlays_alloc = شمار;
                } وگرنه {
                    ثابت ساختار xkb_کلید **tmp =
                        realloc(dest->overlays_کلیدها,
                                شمار * اندازه(*dest->overlays_کلیدها));
                    اگر (!tmp)
                        بازگشت false;
                    dest->overlays_کلیدها = tmp;
                    dest->overlays_alloc = شمار;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ثابت ساختار xkb_کلید **src_کلیدها = (src->overlays_clear)
                ? پوچ
                : (src->overlays_alloc)
                    ? src->overlays_کلیدها
                    : &src->overlay_کلید;
            وقتی (remaining) {
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
                اگر (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ثابت ساختار xkb_کلید *src_کلید = (src_کلیدها)
                    ? *(src_کلیدها++)
                    : پوچ;

                /* check dest */
                ثابت ساختار xkb_کلید *dest_کلید = پوچ;
                ثابت bool conflict = overlays_get(dest, bit, &dest_کلید);
                اگر (conflict) {
                    اگر (dest_کلید == src_کلید)
                       ادامه;
                    وگرنه اگر (report)
                        *collide |= کلید_FIELD_OVERLAY;
                }

                اگر ((!conflict || clobber) && !overlays_درج(dest, bit, src_کلید))
                    بازگشت false;
            }

            اگر (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                اگر (into->overlays_alloc)
                    آزاد(into->overlays_کلیدها);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                اگر (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_کلیدها = from->overlays_کلیدها;
                    from->overlays_کلیدها = پوچ;
                    from->overlays_alloc = 0;
                } وگرنه {
                    into->overlay_کلید = from->overlay_کلید;
                }
            }
        } وگرنه {
            /* Disjoint overlays */

            اگر (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_کلید == from->overlay_کلید) {
                /* Indentical: do nothing */
                بازگشت true;
            }
            اگر (!(into->overlays & from->overlays)) {
                اگر (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_کلید = from->overlay_کلید;
                    بازگشت true;
                } وگرنه اگر (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    بازگشت true;
                }
            }
            /* Conflict */
            اگر (report)
                *collide |= کلید_FIELD_OVERLAY;
            اگر (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_کلید = from->overlay_کلید;
            }
        }
    }
    بازگشت true;
}

ایستا bool
Merge_کلیدها(SymbolsInfo *info, کلید_Info *into, کلید_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    شمارش کلید_field collide = 0;
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->merge != MERGE_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    اگر (from->merge == MERGE_REPLACE) {
        Clear_کلید_Info(into);
        *into = *from;
        Init_کلید_Info(info->ctx, from);
        بازگشت true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    برای (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    برای (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    اگر (Use_New_کلید_Field(کلید_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= کلید_FIELD_VMODMAP;
    }
    اگر (Use_New_کلید_Field(کلید_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= کلید_FIELD_REPEAT;
    }
    اگر (Use_New_کلید_Field(کلید_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= کلید_FIELD_DEFAULT_TYPE;
    }
    اگر (Use_New_کلید_Field(کلید_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_عدد = from->out_of_range_group_عدد;
        into->defined |= کلید_FIELD_GROUPINFO;
    }

    اگر (!merge_overlays(info, into, from, clobber, report, &collide))
        بازگشت false;

    اگر (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 کلید_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_کلید_Info(from);
    Init_کلید_Info(info->ctx, from);
    بازگشت true;
}

ایستا xkb_atom_t
Xkb_Resolve_کلید_Alias(ثابت ساختار xkb_keymap *keymap, xkb_atom_t name)
{
    اگر (name < keymap->رقم_کلید_names) {
        ثابت KeycodeMatch match = keymap->کلید_names[name];
        اگر (match.found && match.is_alias) {
            بازگشت match.alias.real;
        }
    }
    بازگشت name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ایستا bool
Add_کلید_Symbols(SymbolsInfo *info, کلید_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_کلید_Alias(&info->keymap_info->keymap, keyi->name);

    کلید_Info *iter;
    darray_foreach(iter, info->کلیدها)
        اگر (iter->name == keyi->name)
            بازگشت Merge_کلیدها(info, iter, keyi, same_file);

    darray_append(info->کلیدها, *keyi);
    Init_کلید_Info(info->ctx, keyi);
    بازگشت true;
}

ایستا bool
AddModMapEntry(SymbolsInfo *info, ثابت ModMapEntry *new)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        اگر ((new->have_نماد != old->have_نماد) ||
            (new->have_نماد && new->u.کلید_Sym != old->u.کلید_Sym) ||
            (!new->have_نماد && new->u.کلید_Name != old->u.کلید_Name))
            ادامه;

        اگر (new->mods == old->mods)
            بازگشت true;

        ثابت xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        اگر (new->have_نماد) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.کلید_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } وگرنه {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    کلید_Name_Text(info->ctx, new->u.کلید_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        بازگشت true;
    }

    darray_append(info->modmaps, *new);

    بازگشت true;
}

/***====================================================================***/

ایستا تهی
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     شمارش merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    اگر (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        بازگشت;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    اگر (into->name == پوچ) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    برای (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        اگر (!darray_item(from->group_names, i))
            ادامه;

        اگر (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            ادامه;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    اگر (darray_empty(into->کلیدها)) {
        into->کلیدها = from->کلیدها;
        darray_init(from->کلیدها);
    }
    وگرنه {
        کلید_Info *keyi;
        darray_foreach(keyi, from->کلیدها) {
            keyi->merge = merge;
            اگر (!Add_کلید_Symbols(into, keyi, false))
                into->error_شمار++;
        }
    }

    اگر (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وگرنه {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            اگر (!AddModMapEntry(into, mm))
                into->error_شمار++;
        }
    }
}

ایستا تهی
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ایستا bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    اگر (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_شمار += 10;
        بازگشت false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساختار parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برای (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        نویسه path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, اندازه(path));
        اگر (!file) {
            info->error_شمار += 10;
            ClearSymbolsInfo(&included);
            بازگشت false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        اگر (stmt->modifier) {
            ثابت صحیح شمار = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            اگر (شمار <= 0 || *(stmt->modifier + شمار) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } وگرنه {
                next_incl.explicit_group--;
            }
        }
        وگرنه اگر (info->keymap_info->keymap.رقم_groups != 0 &&
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
        وگرنه {
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

    بازگشت (info->error_شمار == 0);
}

ایستا bool
GetGroupIndex(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
              شمارش group_field field, xkb_layout_index_t *ndx_rtrn)
{
    أكد (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ثابت نویسه *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    اگر (arrayNdx == پوچ) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            اگر (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                بازگشت true;
            }
        }

        اگر (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, کلید_Info_Text(info, keyi), info->max_groups, name);
            بازگشت false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        بازگشت true;
    }

    اگر (عبارت_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, پوچ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, کلید_Info_Text(info, keyi));
        بازگشت false;
    }

    (*ndx_rtrn)--;
    اگر (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    بازگشت true;
}

ایستا bool
Add_Symbols_To_کلید(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
                عبارت_Def *مقدار)
{
    xkb_layout_index_t ndx = 0;

    اگر (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        بازگشت false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    اگر (مقدار->common.type == STMT_عبارت_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        بازگشت true;
    }

    اگر (مقدار->common.type != STMT_عبارت_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(مقدار->common.type), ndx + 1,
                کلید_Info_Text(info, keyi));
        بازگشت false;
    }

    اگر (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_کلید_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                کلید_Info_Text(info, keyi), ndx + 1);
        بازگشت false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    برای (عبارت_Keysym_List *keysymList = (عبارت_Keysym_List *) مقدار;
         keysymList;
         keysymList = (عبارت_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        اگر (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    اگر (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    اگر (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    برای (عبارت_Keysym_List *keysymList = (عبارت_Keysym_List *) مقدار;
         keysymList && level < nLevels;
         keysymList = (عبارت_Keysym_List *) keysymList->common.next, level++) {
        ساختار xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->رقم_syms == 0);

        اگر (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            بازگشت false;
        }

        leveli->رقم_syms = (xkb_keysym_شمار_t) darray_size(keysymList->syms);
        انتخاب (leveli->رقم_syms) {
        حالت 0:
            leveli->s.sym = XKB_کلید_No_نماد;
            بشکن;
        حالت 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_کلید_No_نماد);
            بشکن;
        پیشفرض:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, پوچ);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            برای (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++)
                أكد(leveli->s.syms[k] != XKB_کلید_No_نماد);
#endif
        }
    }

    بازگشت true;
}

ایستا bool
Add_Actions_To_کلید(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
                عبارت_Def *مقدار)
{
    xkb_layout_index_t ndx = 0;

    اگر (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        بازگشت false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    اگر (مقدار->common.type == STMT_عبارت_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        بازگشت true;
    }

    اگر (مقدار->common.type != STMT_عبارت_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 مقدار->common.type, ndx, کلید_Info_Text(info, keyi));
        بازگشت false;
    }

    اگر (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 کلید_Info_Text(info, keyi), ndx);
        بازگشت false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    برای (ParseCommon *p = &مقدار->common; p; p = p->next)
        nLevels++;
    اگر (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    برای (عبارت_Action_List *actionList = (عبارت_Action_List *) مقدار;
         actionList;
         actionList = (عبارت_Action_List *) actionList->common.next, level++) {
        ساختار xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->رقم_actions == 0);

        unsigned صحیح رقم_actions = 0;
        برای (عبارت_Def *act = actionList->actions;
             act; act = (عبارت_Def *) act->common.next)
            رقم_actions++;

        اگر (unlikely(رقم_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, رقم_actions);
            بازگشت false;
        }

        /* Parse actions and add only defined actions */
        darray(اجتماع xkb_action) actions = darray_new();
        برای (عبارت_Def *act = actionList->actions;
             act; act = (عبارت_Def *) act->common.next) {
            اجتماع xkb_action toAct = { 0 };
            ثابت شمارش xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            اگر (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        کلید_Info_Text(info, keyi), ndx + 1, level + 1);
                اگر (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    بازگشت false;
                } وگرنه {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            اگر (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } وگرنه اگر (likely(رقم_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->رقم_actions = 1;
                leveli->a.action = toAct;
                برو next_level;
            } وگرنه {
                darray_append(actions, toAct);
            }
        }
        اگر (darray_empty(actions)) {
            leveli->رقم_actions = 0;
        } وگرنه اگر (likely(darray_size(actions) > 1)) {
            leveli->رقم_actions = (xkb_action_شمار_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, پوچ);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            برای (xkb_action_شمار_t k = 0; k < leveli->رقم_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } وگرنه {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(رقم_actions > darray_size(actions));
            leveli->رقم_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        اگر (leveli->رقم_actions > 0 || leveli->رقم_syms > 0)
            nonEmptyLevels = level + 1;
    }

    اگر (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        اگر (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        وگرنه
            darray_free(groupi->levels);
    }

    بازگشت true;
}

ایستا ثابت LookupEntry repeatEntries[] = {
    { "true", کلید_REPEAT_YES },
    { "yes", کلید_REPEAT_YES },
    { "on", کلید_REPEAT_YES },
    { "false", کلید_REPEAT_NO },
    { "no", کلید_REPEAT_NO },
    { "off", کلید_REPEAT_NO },
    { "default", کلید_REPEAT_UNDEFINED },
    { پوچ, 0 }
};

/** Handle overlay<N> = <key> entry */
ایستا bool
عبارت_Resolve_Overlay_Entry(ثابت ساختار xkb_keymap_info *keymap_info,
                        ثابت نویسه *field, ثابت عبارت_Def *arrayNdx,
                        ثابت عبارت_Def *عبارت, کلید_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ثابت ساختار xkb_کلید **کلید_rtrn)
{
    /* Overlay index */
    اگر (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, کلید_Name_Text(keymap_info->keymap.ctx, keyi->name));
        بازگشت false;
    }
    static_assert(اندازه(xkb_overlay_index_t) == اندازه(uint8_t), "");
    ثابت size_t prefix = اندازه("overlay") - 1;
    ثابت size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    اگر (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (صحیح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, کلید_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        بازگشت false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_KEYNAME_LITERAL:
        *کلید_rtrn = Xkb_کلید_By_Name(&keymap_info->keymap, عبارت->کلید_name.کلید_name,
                                 false);
        اگر (!*کلید_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, عبارت->کلید_name.کلید_name),
                    field, کلید_Name_Text(keymap_info->keymap.ctx, keyi->name));
            بازگشت false;
        }
        بازگشت true;
    حالت STMT_عبارت_IDENT: {
        ثابت نویسه * ثابت id =
            xkb_atom_text(keymap_info->keymap.ctx, عبارت->ident.ident);
        اگر (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *کلید_rtrn = پوچ;
            بازگشت true;
        } وگرنه اگر (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *کلید_rtrn = پوچ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            بازگشت true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_مقدار,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, کلید_Name_Text(keymap_info->keymap.ctx, keyi->name));
        بازگشت false;
    }
    پیشفرض:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_مقدار,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_عبارت_KEYNAME_LITERAL),
                field, کلید_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;
    }
}

ایستا bool
SetSymbolsField(SymbolsInfo *info, کلید_Info *keyi, ثابت نویسه *field,
                عبارت_Def *arrayNdx, عبارت_Def **مقدار_ptr)
{
    عبارت_Def * restrict ثابت مقدار = *مقدار_ptr;
    اگر (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        اگر (!عبارت_Resolve_String(info->ctx, مقدار, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            بازگشت false;
        }

        اگر (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= کلید_FIELD_DEFAULT_TYPE;
        }
        وگرنه اگر (عبارت_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, پوچ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }
        وگرنه {
            ndx--;
            اگر (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    وگرنه اگر (istreq(field, "symbols")) {
        بازگشت Add_Symbols_To_کلید(info, keyi, arrayNdx, مقدار);
    }
    وگرنه اگر (istreq(field, "actions")) {
        بازگشت Add_Actions_To_کلید(info, keyi, arrayNdx, مقدار);
    }
    وگرنه اگر (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        اگر (!عبارت_Resolve_Mod_Mask(info->ctx, مقدار, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(مقدار->common.type),
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= کلید_FIELD_VMODMAP;
    }
    وگرنه اگر (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                کلید_Info_Text(info, keyi));
    }
    وگرنه اگر (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                کلید_Info_Text(info, keyi));
    }
    وگرنه اگر (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                کلید_Info_Text(info, keyi));
    }
    وگرنه اگر (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ثابت ساختار xkb_کلید *کلید = پوچ;
        اگر (!عبارت_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *مقدار_ptr, keyi, &overlay, &کلید))
            بازگشت false;
        اگر (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            بازگشت true;
        } وگرنه اگر (کلید && کلید->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, کلید_Info_Text(info, keyi));
        } وگرنه {
            ثابت ساختار xkb_کلید *prev;
            اگر (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                اگر (کلید != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             کلید_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? کلید_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (کلید
                                 ? کلید_Name_Text(info->ctx, کلید->name)
                                 : "none"));
                }
            } وگرنه اگر (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                اگر (!overlays_درج(keyi, overlay, کلید))
                    بازگشت false;
                keyi->defined |= کلید_FIELD_OVERLAY;
            } وگرنه {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                اگر (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    اگر (کلید) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_کلید = کلید;
                    } وگرنه {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= کلید_FIELD_OVERLAY;
                } وگرنه اگر (keyi->overlays) {
                    اگر (کلید) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                کلید_Info_Text(info, keyi), keyi->overlays,
                                کلید_Name_Text(info->ctx, keyi->overlay_کلید->name),
                                overlay + 1, کلید_Name_Text(info->ctx, کلید->name));
                        بازگشت !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_مقدار_MISMATCH);
                    } وگرنه {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وگرنه اگر (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        اگر (!عبارت_Resolve_Enum(info->ctx, مقدار, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }

        keyi->repeat = (شمارش کلید_repeat) val;
        keyi->defined |= کلید_FIELD_REPEAT;
    }
    وگرنه اگر (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        اگر (!عبارت_Resolve_Boolean(info->ctx, مقدار, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= کلید_FIELD_GROUPINFO;
    }
    وگرنه اگر (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        اگر (!عبارت_Resolve_Boolean(info->ctx, مقدار, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= کلید_FIELD_GROUPINFO;
    }
    وگرنه اگر (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        اگر (عبارت_Resolve_Group(info->keymap_info, مقدار, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    کلید_Info_Text(info, keyi));
            بازگشت false;
        }

        اگر (pending) {
            keyi->out_of_range_pending_group = true;
            ثابت darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ساختار pending_computation) {
                    .عبارت = *مقدار_ptr,
                    .computed = false,
                    .مقدار = 0,
                }
            );
            *مقدار_ptr = پوچ;
            static_assert(اندازه(keyi->out_of_range_group_عدد) ==
                          اندازه(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_عدد = pending_index;
        } وگرنه {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_عدد = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= کلید_FIELD_GROUPINFO;
    }
    وگرنه {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        بازگشت !(info->keymap_info->strict & PARSER_NO_UNKNOWN_کلید_FIELDS);
    }

    بازگشت true;
}

ایستا bool
SetGroupName(SymbolsInfo *info, عبارت_Def *arrayNdx, عبارت_Def *مقدار,
             شمارش merge_mode merge)
{
    اگر (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        بازگشت false;
    }

    xkb_layout_index_t group = 0;
    اگر (عبارت_Resolve_Group(info->keymap_info, arrayNdx, false, &group, پوچ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        بازگشت false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    اگر (!عبارت_Resolve_String(info->ctx, مقدار, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        بازگشت false;
    }

    xkb_layout_index_t group_to_use;
    اگر (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    وگرنه اگر (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    وگرنه {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        بازگشت false;
    }

    اگر (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } وگرنه {
        ثابت xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        اگر (old_name != XKB_ATOM_NONE && old_name != name) {
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

    بازگشت true;
}

ایستا bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت نویسه *elem, *field;
    عبارت_Def *arrayNdx;
    bool ret;

    اگر (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        بازگشت false;

    اگر (elem && istreq(elem, "key")) {
        کلید_Info temp = {0};
        Init_کلید_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->مقدار) &&
              Merge_کلیدها(info, &info->default_کلید, &temp, true);
        Clear_کلید_Info(&temp);
    }
    وگرنه اگر (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->مقدار, stmt->merge);
    }
    وگرنه اگر (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وگرنه اگر (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وگرنه اگر (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وگرنه اگر (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وگرنه اگر (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->مقدار, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } وگرنه {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        بازگشت !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    بازگشت ret;
}

ایستا bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, کلید_Info *keyi)
{
    bool all_valid_entries = true;

    برای (; def; def = (VarDef *) def->common.next) {
        ثابت نویسه *field;
        عبارت_Def *arrayNdx;
        bool ok = true;

        اگر (!def->name) {
            اگر (unlikely(!def->مقدار) || def->مقدار->common.type != STMT_عبارت_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            وگرنه
                field = "actions";
            arrayNdx = پوچ;
        }
        وگرنه {
            ثابت نویسه *elem;
            ok = عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            اگر (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        اگر (unlikely(!def->مقدار)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        اگر (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->مقدار))
            all_valid_entries = false;
    }

    بازگشت all_valid_entries;
}

ایستا bool
SetExplicitGroup(SymbolsInfo *info, کلید_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    اگر (info->explicit_group == XKB_LAYOUT_INVALID)
        بازگشت true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        اگر (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    اگر (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, کلید_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    اگر (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    بازگشت true;
}

ایستا bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    کلید_Info keyi;

    keyi = info->default_کلید;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_کلید.groups);
    برای (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_کلید.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->کلید_Name;

    اگر (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_کلید_Symbols(info, &keyi, true)) {
        بازگشت true;
    }

    Clear_کلید_Info(&keyi);
    info->error_شمار++;
    بازگشت false;
}

ایستا bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    ساختار xkb_context *ctx = info->ctx;

    اگر (!عبارت_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        بازگشت false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_عبارت_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    برای (عبارت_Def *کلید = def->کلیدها; کلید; کلید = (عبارت_Def *) کلید->common.next) {
        اگر (کلید->common.type == STMT_عبارت_KEYNAME_LITERAL) {
            tmp.have_نماد = false;
            tmp.u.کلید_Name = کلید->کلید_name.کلید_name;
        }
        وگرنه اگر (کلید->common.type == STMT_عبارت_KEYSYM_LITERAL) {
            اگر (کلید->keysym.keysym == XKB_کلید_No_نماد) {
                /* Invalid keysym: ignore. Error message already printed */
                ادامه;
            } وگرنه {
                tmp.have_نماد = true;
                tmp.u.کلید_Sym = کلید->keysym.keysym;
            }
        }
        وگرنه {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ادامه;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    بازگشت ok;
}

ایستا تهی
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_safe(file->name);

    برای (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            بشکن;
        حالت STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            بشکن;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            بشکن;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            بشکن;
        حالت STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            بشکن;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            بشکن;
        پیشفرض:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            بشکن;
        }

        اگر (!ok)
            info->error_شمار++;

        اگر (info->error_شمار > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            بشکن;
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
ایستا ساختار xkb_کلید *
Find_کلید_For_نماد(ساختار xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    انجامبده {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        انجامبده {
            got_one_level = false;
            ساختار xkb_کلید *کلید;
            xkb_کلیدها_foreach(کلید, keymap) {
                اگر (group < کلید->رقم_groups &&
                    level < Xkb_کلید_رقم_Levels(کلید, group)) {
                    got_one_group = got_one_level = true;
                    ثابت xkb_keysym_شمار_t رقم_syms =
                        کلید->groups[group].levels[level].رقم_syms;
                    اگر (رقم_syms > 1) {
                        برای (xkb_keysym_شمار_t k = 0; k < رقم_syms; k++) {
                            اگر (کلید->groups[group].levels[level].s.syms[k] == sym)
                                بازگشت کلید;
                        }
                    } وگرنه اگر (رقم_syms &&
                               کلید->groups[group].levels[level].s.sym == sym) {
                        بازگشت کلید;
                    }
                }
            }
            level++;
        } وقتی (got_one_level);
        group++;
    } وقتی (got_one_group);

    بازگشت پوچ;
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
ایستا xkb_atom_t
FindAutomaticType(ساختار xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).رقم_syms == 0 ? \
        XKB_کلید_No_نماد : \
     darray_item(groupi->levels, level).رقم_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    اگر (width == 1 || width <= 0)
        بازگشت xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    اگر (width == 2) {
        اگر (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            بازگشت xkb_atom_intern_literal(ctx, "ALPHABETIC");

        اگر (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            بازگشت xkb_atom_intern_literal(ctx, "KEYPAD");

        بازگشت xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    اگر (width <= 4) {
        اگر (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_کلید_No_نماد);

            اگر (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                بازگشت xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            بازگشت xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        اگر (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            بازگشت xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        بازگشت xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    بازگشت XKB_ATOM_NONE;

#undef GET_SYM
}

ایستا ثابت ساختار xkb_کلید_type *
FindTypeForGroup(ساختار xkb_keymap *keymap, کلید_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    اگر (type_name == XKB_ATOM_NONE) {
        اگر (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وگرنه {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            اگر (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    اگر (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_کلید_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 کلید_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        برو use_default;
    }

    darray_size_t i;
    برای (i = 0; i < keymap->رقم_types; i++)
        اگر (keymap->types[i].name == type_name)
            بشکن;

    اگر (i >= keymap->رقم_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_کلید_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 کلید_Name_Text(keymap->ctx, keyi->name), group + 1);
        برو use_default;
    }

    keymap->types[i].required = true;
    بازگشت &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    بازگشت &keymap->types[0];
}

ایستا bool
CopySymbolsDefToKeymap(ساختار xkb_keymap *keymap, SymbolsInfo *info,
                       کلید_Info *keyi)
{
    ساختار xkb_کلید *کلید;
    GroupInfo *groupi;
    ثابت GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    کلید = Xkb_کلید_By_Name(keymap, keyi->name, false);
    اگر (!کلید) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                کلید_Info_Text(info, keyi));
        بازگشت false;
    }

    /* Find the range of groups we need. */
    کلید->رقم_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & کلید_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        اگر (darray_size(groupi->levels) > 0 || has_explicit_type)
            کلید->رقم_groups = i + 1;
        اگر (has_explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    اگر (کلید->رقم_groups <= 0) {
        /* A key with no group may still have other fields defined */
        اگر (keyi->defined)
            برو کلید_fields;
        وگرنه
            بازگشت false;
    }

    darray_resize(keyi->groups, کلید->رقم_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        اگر (groupi->defined)
            ادامه;

        CopyGroupInfo(groupi, group0);
    }

    کلید->groups = calloc(کلید->رقم_groups, اندازه(*کلید->groups));
    أكد(darray_size(keyi->groups) == کلید->رقم_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < کلید->رقم_groups);
        #endif
        bool explicit_type = false;
        ثابت ساختار xkb_کلید_type * ثابت type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        اگر (type->رقم_levels < darray_size(groupi->levels)) {
            ساختار xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->رقم_levels,
                    کلید_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->رقم_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->رقم_levels);

        کلید->groups[i].explicit_type = explicit_type;
        کلید->groups[i].type = type;
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
        ساختار xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            انتخاب (leveli->رقم_syms) {
            حالت 0:
                leveli->upper = XKB_کلید_No_نماد;
                بشکن;
            حالت 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                بشکن;
            پیشفرض:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                برای (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++) {
                    ثابت xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    اگر (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        بشکن;
                    }
                }
                اگر (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->رقم_syms *
                                             اندازه(*leveli->s.syms));
                    اگر (!leveli->s.syms)
                        بازگشت false; /* FIXME: better handling? */
                    برای (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++) {
                        leveli->s.syms[leveli->رقم_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &کلید->groups[i].levels, پوچ);
        اگر (کلید->groups[i].type->رقم_levels > 1 ||
            کلید->groups[i].levels[0].رقم_syms > 0) {
            کلید->groups[i].explicit_symbols = true;
            کلید->explicit |= EXPLICIT_SYMBOLS;
        }
        اگر (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            کلید->groups[i].explicit_actions = true;
            کلید->explicit |= EXPLICIT_INTERP;
        }
        اگر (کلید->groups[i].explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    کلید->out_of_range_pending_group = keyi->out_of_range_pending_group;
    کلید->out_of_range_group_عدد = keyi->out_of_range_group_عدد;
    کلید->out_of_range_group_policy = keyi->out_of_range_group_policy;

کلید_fields:
    اگر (keyi->defined & کلید_FIELD_VMODMAP) {
        کلید->vmodmap = keyi->vmodmap;
        کلید->explicit |= EXPLICIT_VMODMAP;
    }

    اگر (keyi->repeat != کلید_REPEAT_UNDEFINED) {
        کلید->repeats = (keyi->repeat == کلید_REPEAT_YES);
        کلید->explicit |= EXPLICIT_REPEAT;
    }

    اگر ((keyi->defined & کلید_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        کلید->overlays = keyi->overlays;
        اگر (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = کلید->overlays;
            ثابت ساختار xkb_کلید **overlays_کلیدها = keyi->overlays_کلیدها;
            وقتی (remaining) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                اگر (*overlays_کلیدها) {
                    overlays_کلیدها++;
                } وگرنه {
                    /* drop current null value (overlayN=none) */
                    کلید->overlays &= ~lsb;
                }
            }

            اگر (کلید->overlays) {
                /* Steal */
                کلید->overlays_کلیدها = keyi->overlays_کلیدها;
                keyi->overlays_کلیدها = پوچ;
                keyi->overlays_alloc = 0;

                کلید->overlays_inline = false;
                کلید->explicit |= EXPLICIT_OVERLAY;
            }
        } وگرنه {
            کلید->overlay_کلید = keyi->overlay_کلید;
            کلید->overlays_inline = true;
            کلید->explicit |= EXPLICIT_OVERLAY;
        }
    }

    بازگشت true;
}

ایستا bool
CopyModMapDefToKeymap(ساختار xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    ساختار xkb_کلید *کلید;

    اگر (!entry->have_نماد) {
        کلید = Xkb_کلید_By_Name(keymap, entry->u.کلید_Name, true);
        اگر (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    کلید_Name_Text(info->ctx, entry->u.کلید_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            بازگشت false;
        }
    }
    وگرنه {
        کلید = Find_کلید_For_نماد(keymap, entry->u.کلید_Sym);
        اگر (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_نماد,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.کلید_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            بازگشت false;
        }
    }

    /* Skip modMap None */
    اگر (entry->mods) {
        کلید->modmap |= entry->mods;
        entry->کلید_Code = کلید->keycode;
    }

    بازگشت true;
}

ایستا bool
CopySymbolsToKeymap(ساختار xkb_keymap *keymap, SymbolsInfo *info)
{
    کلید_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->رقم_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, پوچ);

    darray_foreach(keyi, info->کلیدها)
        اگر (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_شمار++;

    اگر (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ساختار xkb_کلید *کلید;

        xkb_کلیدها_foreach(کلید, keymap) {
            اگر (کلید->name == XKB_ATOM_NONE)
                ادامه;

            اگر (کلید->رقم_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         کلید_Name_Text(info->ctx, کلید->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        اگر (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_شمار++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->رقم_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    بازگشت true;
}

bool
CompileSymbols(XkbFile *file, ساختار xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    اگر (file !=پوچ)
        HandleSymbolsFile(&info, file);

    اگر (info.error_شمار != 0)
        برو err_info;

    اگر (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        برو err_info;

    ClearSymbolsInfo(&info);
    بازگشت true;

err_info:
    ClearSymbolsInfo(&info);
    بازگشت false;
}
