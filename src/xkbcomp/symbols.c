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

تعداد ئاچقۇچ_repeat {
    ئاچقۇچ_REPEAT_UNDEFINED = 0,
    ئاچقۇچ_REPEAT_YES,
    ئاچقۇچ_REPEAT_NO,
    ئاچقۇچ_REPEAT_رەقەم_ENTRIES,
};
تعداد { ئاچقۇچ_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(ئاچقۇچ_REPEAT_رەقەم_ENTRIES - 1 <=
              (1 << (ئاچقۇچ_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

تعداد group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

تعداد ئاچقۇچ_field {
    ئاچقۇچ_FIELD_NONE = 0,
    ئاچقۇچ_FIELD_REPEAT = (1 << 0),
    ئاچقۇچ_FIELD_DEFAULT_TYPE = (1 << 1),
    ئاچقۇچ_FIELD_GROUPINFO = (1 << 2),
    ئاچقۇچ_FIELD_VMODMAP = (1 << 3),
    ئاچقۇچ_FIELD_OVERLAY = (1 << 4),
    ئاچقۇچ_FIELD_ALL = ئاچقۇچ_FIELD_REPEAT | ئاچقۇچ_FIELD_DEFAULT_TYPE
                  | ئاچقۇچ_FIELD_GROUPINFO | ئاچقۇچ_FIELD_VMODMAP | ئاچقۇچ_FIELD_OVERLAY,
};
تعداد { ئاچقۇچ_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(ئاچقۇچ_FIELD_ALL <= (1 << (ئاچقۇچ_FIELD_MIN_WIDTH - 1)) - 1,
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
              (unsigned)ئاچقۇچ_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)ئاچقۇچ_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_سان;
    darray(GroupInfo) groups;
    تعداد xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد ئاچقۇچ_field defined:KEYINFO_DEFINED_BIT_FIELD;
    تعداد merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    تعداد ئاچقۇچ_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
        ثابت بنية xkb_ئاچقۇچ *overlay_ئاچقۇچ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_ئاچقۇچ **overlays_ئاچقۇچلار;
    };
} ئاچقۇچ_Info;

ساكن فراغ
StealLevelInfo(بنية xkb_level *into, بنية xkb_level *from)
{
    clear_level(into);
    إذا (from->رەقەم_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } وإلا {
        into->s.sym = from->s.sym;
    }
    into->رەقەم_syms = from->رەقەم_syms;
    from->رەقەم_syms = 0;

    إذا (from->رەقەم_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } وإلا {
        into->a.action = from->a.action;
    }
    into->رەقەم_actions = from->رەقەم_actions;
    from->رەقەم_actions = 0;
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
        إذا (darray_item(from->levels, j).رەقەم_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).رەقەم_syms,
                       حجم(xkb_keysym_t));
        }
        إذا (darray_item(from->levels, j).رەقەم_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).رەقەم_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساكن فراغ
Init_ئاچقۇچ_Info(بنية xkb_context *ctx, ئاچقۇچ_Info *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_ئاچقۇچ_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ئاچقۇچ_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ئاچقۇچ = عدم;
}

ساكن فراغ
Clear_ئاچقۇچ_Info(ئاچقۇچ_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    إذا (keyi->overlays_alloc)
        حرر(keyi->overlays_ئاچقۇچلار);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح error_سانى;
    unsigned صحيح include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(ئاچقۇچ_Info) ئاچقۇچلار;
    ئاچقۇچ_Info default_ئاچقۇچ;
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
    Init_ئاچقۇچ_Info(keymap_info->keymap.ctx, &info->default_ئاچقۇچ);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ئاچقۇچ_Info *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->ئاچقۇچلار)
        Clear_ئاچقۇچ_Info(keyi);
    darray_free(info->ئاچقۇچلار);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_ئاچقۇچ_Info(&info->default_ئاچقۇچ);
}

ساكن ثابت حرف *
ئاچقۇچ_Info_Text(SymbolsInfo *info, ئاچقۇچ_Info *keyi)
{
    أرجع ئاچقۇچ_Name_Text(info->ctx, keyi->name);
}

ساكن bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t ئاچقۇچ_name)
{
    /* First find the type of the merged group. */
    إذا (into->type != from->type) {
        إذا (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        وإلا إذا (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        وإلا {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    إذا (darray_empty(from->levels)) {
        InitGroupInfo(from);
        أرجع true;
    }

    إذا (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        أرجع true;
    }

    /* Merge the actions and syms. */
    ثابت darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_سانى = 0;
    darray_size_t from_Actions_سانى = 0;
    لكل (darray_size_t i = 0; i < levels_in_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_item(into->levels, i);
        بنية xkb_level* ثابت fromLevel = &darray_item(from->levels, i);

        ثابت bool fromHasNoKeysym = fromLevel->رەقەم_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->رەقەم_actions == 0;
        إذا (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            استمر;
        }

        ثابت bool intoHasNoKeysym = intoLevel->رەقەم_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->رەقەم_actions == 0;
        إذا (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_سانى++;
            from_Actions_سانى++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->رەقەم_syms > 0 || intoLevel->رەقەم_actions > 0);
        أكد(fromLevel->رەقەم_syms > 0 || fromLevel->رەقەم_actions > 0);

        /* Handle keysyms */
        إذا (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            إذا (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_بەلگە,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            إذا (fromHasNoKeysym) {
                /* No keysym to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined keysym from `from` */
                إذا (unlikely(fromLevel->رەقەم_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->رەقەم_syms > 1))
                        حرر(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->رەقەم_syms = fromLevel->رەقەم_syms;
                    fromLevel->رەقەم_syms = 0;
                    from_Keysyms_سانى++;
                } وإلا إذا (fromLevel->s.sym != XKB_ئاچقۇچ_No_بەلگە) {
                    /* Single defined keysym */
                    إذا (unlikely(intoLevel->رەقەم_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->رەقەم_syms = 1;
                    from_Keysyms_سانى++;
                }
            } وإلا {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->رەقەم_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->s.sym == XKB_ئاچقۇچ_No_بەلگە) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->رەقەم_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->رەقەم_syms = fromLevel->رەقەم_syms;
                        fromLevel->رەقەم_syms = 0;
                    } وإلا {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->رەقەم_syms = 1;
                    }
                    from_Keysyms_سانى++;
                }
            }
        }

        /* Handle actions */
        إذا (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            إذا (report && !(intoHasNoAction || fromHasNoAction)) {
                إذا (unlikely(intoLevel->رەقەم_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } وإلا {
                    اتحاد xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            إذا (fromHasNoAction) {
                /* No action to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined action from `from` */
                إذا (unlikely(fromLevel->رەقەم_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->رەقەم_actions > 1))
                        حرر(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->رەقەم_actions = fromLevel->رەقەم_actions;
                    fromLevel->رەقەم_actions = 0;
                    from_Actions_سانى++;
                } وإلا إذا (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    إذا (unlikely(intoLevel->رەقەم_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->رەقەم_actions = 1;
                    from_Actions_سانى++;
                }
            } وإلا {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->رەقەم_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->رەقەم_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->رەقەم_actions = fromLevel->رەقەم_actions;
                        fromLevel->رەقەم_actions = 0;
                    } وإلا {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->رەقەم_actions = 1;
                    }
                    from_Actions_سانى++;
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
        level->رەقەم_syms = 0;
        level->رەقەم_actions = 0;
        from_Keysyms_سانى++;
        from_Actions_سانى++;
    }
    إذا (from_Keysyms_سانى) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        إذا (from_Keysyms_سانى == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    إذا (from_Actions_سانى) {
        /* Reset defined actions field if we used no action from `into` */
        إذا (from_Actions_سانى == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    أرجع true;
}

ساكن bool
Use_New_ئاچقۇچ_Field(تعداد ئاچقۇچ_field field, تعداد ئاچقۇچ_field old, تعداد ئاچقۇچ_field new,
               bool clobber, bool report, تعداد ئاچقۇچ_field *collide)
{
    إذا (!(old & field))
        أرجع (new & field);

    إذا (new & field) {
        إذا (report)
            *collide |= field;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
overlays_get(ثابت ئاچقۇچ_Info *info, xkb_overlay_index_t bit,
             ثابت بنية xkb_ئاچقۇچ **ئاچقۇچ_out)
{
    إذا (bit >= (xkb_overlay_index_t)(حجم(xkb_overlay_mask_t) * CHAR_BIT))
        أرجع false;
    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    إذا (!(info->overlays & mask)) أرجع false;
    إذا (ئاچقۇچ_out) {
        إذا (!info->overlays_alloc) {
            /* Stored inline */
            *ئاچقۇچ_out = info->overlay_ئاچقۇچ;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t تۆۋەن = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(تۆۋەن);
            *ئاچقۇچ_out = info->overlays_ئاچقۇچلار[index];
        }
    }
    أرجع true;
}

ساكن bool
overlays_قىستۇرۇش(ئاچقۇچ_Info *keyi, xkb_overlay_index_t bit,
                ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ)
{
    إذا (bit >= (xkb_overlay_index_t)(حجم(xkb_overlay_mask_t) * CHAR_BIT))
        أرجع false;

    ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    إذا ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        إذا (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ئاچقۇچ = ئاچقۇچ;
            إذا (!ئاچقۇچ)
                keyi->overlays_clear = true;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_mask_t تۆۋەن = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(تۆۋەن);
            keyi->overlays_ئاچقۇچلار[index] = ئاچقۇچ;
        }
        أرجع true;
    }

    /* Insert new element */
    إذا (!keyi->overlays || (keyi->overlays_clear && !ئاچقۇچ)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ئاچقۇچ = ئاچقۇچ;
        keyi->overlays |= mask;
        keyi->overlays_clear = !ئاچقۇچ;
    } وإلا إذا (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ثابت xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_ئاچقۇچ ** ثابت tmp = calloc(alloc, حجم(*tmp));
        إذا (!tmp) أرجع false;

        /* Copy previous entries to the buffer */
        إذا (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_mask_t تۆۋەن = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(تۆۋەن);
            tmp[idx] = keyi->overlay_ئاچقۇچ;
        } وإلا {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_mask_t تۆۋەن = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(تۆۋەن);
        tmp[idx] = ئاچقۇچ;

        keyi->overlays_ئاچقۇچلار = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وإلا {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ثابت xkb_overlay_index_t سانى =
            (xkb_overlay_index_t)popcount32(overlays);
        إذا (سانى > keyi->overlays_alloc) {
            ثابت xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(سانى);
            أكد(alloc >= سانى);
            ثابت بنية xkb_ئاچقۇچ ** ثابت tmp = realloc(
                keyi->overlays_ئاچقۇچلار,
                (size_t)alloc * حجم(*keyi->overlays_ئاچقۇچلار)
            );
            إذا (!tmp) أرجع false;
            keyi->overlays_ئاچقۇچلار = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_mask_t تۆۋەن = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ثابت xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(تۆۋەن);
        إذا (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        إذا (index < سانى - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ئاچقۇچلار + index + 1, keyi->overlays_ئاچقۇچلار + index,
                    (size_t)(سانى - 1u - index) * حجم(ثابت بنية xkb_ئاچقۇچ *));
        }
        keyi->overlays_ئاچقۇچلار[index] = ئاچقۇچ;
        keyi->overlays = overlays;
    }

    أرجع true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
merge_overlays(SymbolsInfo *info, ئاچقۇچ_Info *into, ئاچقۇچ_Info *from,
               bool clobber, bool report, تعداد ئاچقۇچ_field *collide)
{
    إذا (!(from->defined & ئاچقۇچ_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وإلا إذا (!(into->defined & ئاچقۇچ_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ئاچقۇچلار = from->overlays_ئاچقۇچلار;
        from->overlays_ئاچقۇچلار = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ئاچقۇچ_FIELD_OVERLAY;
    } وإلا إذا (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وإلا {
        إذا (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_index_t سانى =
                (xkb_overlay_index_t)popcount32(result_mask);

            إذا (سانى == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ئاچقۇچ_Info *dest = into;
            ئاچقۇچ_Info *src = from;
            إذا (into->overlays_alloc >= سانى &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وإلا إذا (from->overlays_alloc >= سانى) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وإلا إذا (سانى > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                إذا (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                إذا (!dest->overlays_alloc) {
                    ثابت بنية xkb_ئاچقۇچ **tmp = calloc(سانى, حجم(*tmp));
                    إذا (!tmp)
                        أرجع false;
                    إذا (dest->overlay_ئاچقۇچ)
                        tmp[0] = dest->overlay_ئاچقۇچ;
                    dest->overlays_ئاچقۇچلار = tmp;
                    dest->overlays_alloc = سانى;
                } وإلا {
                    ثابت بنية xkb_ئاچقۇچ **tmp =
                        realloc(dest->overlays_ئاچقۇچلار,
                                سانى * حجم(*dest->overlays_ئاچقۇچلار));
                    إذا (!tmp)
                        أرجع false;
                    dest->overlays_ئاچقۇچلار = tmp;
                    dest->overlays_alloc = سانى;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ثابت بنية xkb_ئاچقۇچ **src_ئاچقۇچلار = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_ئاچقۇچلار
                    : &src->overlay_ئاچقۇچ;
            طالما (remaining) {
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
                إذا (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_ئاچقۇچ *src_ئاچقۇچ = (src_ئاچقۇچلار)
                    ? *(src_ئاچقۇچلار++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_ئاچقۇچ *dest_ئاچقۇچ = عدم;
                ثابت bool conflict = overlays_get(dest, bit, &dest_ئاچقۇچ);
                إذا (conflict) {
                    إذا (dest_ئاچقۇچ == src_ئاچقۇچ)
                       استمر;
                    وإلا إذا (report)
                        *collide |= ئاچقۇچ_FIELD_OVERLAY;
                }

                إذا ((!conflict || clobber) && !overlays_قىستۇرۇش(dest, bit, src_ئاچقۇچ))
                    أرجع false;
            }

            إذا (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                إذا (into->overlays_alloc)
                    حرر(into->overlays_ئاچقۇچلار);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                إذا (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ئاچقۇچلار = from->overlays_ئاچقۇچلار;
                    from->overlays_ئاچقۇچلار = عدم;
                    from->overlays_alloc = 0;
                } وإلا {
                    into->overlay_ئاچقۇچ = from->overlay_ئاچقۇچ;
                }
            }
        } وإلا {
            /* Disjoint overlays */

            إذا (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ئاچقۇچ == from->overlay_ئاچقۇچ) {
                /* Indentical: do nothing */
                أرجع true;
            }
            إذا (!(into->overlays & from->overlays)) {
                إذا (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ئاچقۇچ = from->overlay_ئاچقۇچ;
                    أرجع true;
                } وإلا إذا (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    أرجع true;
                }
            }
            /* Conflict */
            إذا (report)
                *collide |= ئاچقۇچ_FIELD_OVERLAY;
            إذا (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ئاچقۇچ = from->overlay_ئاچقۇچ;
            }
        }
    }
    أرجع true;
}

ساكن bool
Merge_ئاچقۇچلار(SymbolsInfo *info, ئاچقۇچ_Info *into, ئاچقۇچ_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    تعداد ئاچقۇچ_field collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->merge != MERGE_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (from->merge == MERGE_REPLACE) {
        Clear_ئاچقۇچ_Info(into);
        *into = *from;
        Init_ئاچقۇچ_Info(info->ctx, from);
        أرجع true;
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

    إذا (Use_New_ئاچقۇچ_Field(ئاچقۇچ_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ئاچقۇچ_FIELD_VMODMAP;
    }
    إذا (Use_New_ئاچقۇچ_Field(ئاچقۇچ_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ئاچقۇچ_FIELD_REPEAT;
    }
    إذا (Use_New_ئاچقۇچ_Field(ئاچقۇچ_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ئاچقۇچ_FIELD_DEFAULT_TYPE;
    }
    إذا (Use_New_ئاچقۇچ_Field(ئاچقۇچ_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_سان = from->out_of_range_group_سان;
        into->defined |= ئاچقۇچ_FIELD_GROUPINFO;
    }

    إذا (!merge_overlays(info, into, from, clobber, report, &collide))
        أرجع false;

    إذا (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ئاچقۇچ_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ئاچقۇچ_Info(from);
    Init_ئاچقۇچ_Info(info->ctx, from);
    أرجع true;
}

ساكن xkb_atom_t
Xkb_Resolve_ئاچقۇچ_Alias(ثابت بنية xkb_keymap *keymap, xkb_atom_t name)
{
    إذا (name < keymap->رەقەم_ئاچقۇچ_names) {
        ثابت KeycodeMatch match = keymap->ئاچقۇچ_names[name];
        إذا (match.found && match.is_alias) {
            أرجع match.alias.real;
        }
    }
    أرجع name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
Add_ئاچقۇچ_Symbols(SymbolsInfo *info, ئاچقۇچ_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ئاچقۇچ_Alias(&info->keymap_info->keymap, keyi->name);

    ئاچقۇچ_Info *iter;
    darray_foreach(iter, info->ئاچقۇچلار)
        إذا (iter->name == keyi->name)
            أرجع Merge_ئاچقۇچلار(info, iter, keyi, same_file);

    darray_append(info->ئاچقۇچلار, *keyi);
    Init_ئاچقۇچ_Info(info->ctx, keyi);
    أرجع true;
}

ساكن bool
AddModMapEntry(SymbolsInfo *info, ثابت ModMapEntry *new)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        إذا ((new->have_بەلگە != old->have_بەلگە) ||
            (new->have_بەلگە && new->u.ئاچقۇچ_Sym != old->u.ئاچقۇچ_Sym) ||
            (!new->have_بەلگە && new->u.ئاچقۇچ_Name != old->u.ئاچقۇچ_Name))
            استمر;

        إذا (new->mods == old->mods)
            أرجع true;

        ثابت xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        إذا (new->have_بەلگە) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.ئاچقۇچ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } وإلا {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ئاچقۇچ_Name_Text(info->ctx, new->u.ئاچقۇچ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        أرجع true;
    }

    darray_append(info->modmaps, *new);

    أرجع true;
}

/***====================================================================***/

ساكن فراغ
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    إذا (from->error_سانى > 0) {
        into->error_سانى += from->error_سانى;
        أرجع;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    لكل (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        إذا (!darray_item(from->group_names, i))
            استمر;

        إذا (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            استمر;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    إذا (darray_empty(into->ئاچقۇچلار)) {
        into->ئاچقۇچلار = from->ئاچقۇچلار;
        darray_init(from->ئاچقۇچلار);
    }
    وإلا {
        ئاچقۇچ_Info *keyi;
        darray_foreach(keyi, from->ئاچقۇچلار) {
            keyi->merge = merge;
            إذا (!Add_ئاچقۇچ_Symbols(into, keyi, false))
                into->error_سانى++;
        }
    }

    إذا (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وإلا {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            إذا (!AddModMapEntry(into, mm))
                into->error_سانى++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    إذا (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_سانى += 10;
        أرجع false;
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
        إذا (!file) {
            info->error_سانى += 10;
            ClearSymbolsInfo(&included);
            أرجع false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        إذا (stmt->modifier) {
            ثابت صحيح سانى = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            إذا (سانى <= 0 || *(stmt->modifier + سانى) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } وإلا {
                next_incl.explicit_group--;
            }
        }
        وإلا إذا (info->keymap_info->keymap.رەقەم_groups != 0 &&
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
        وإلا {
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

    أرجع (info->error_سانى == 0);
}

ساكن bool
GetGroupIndex(SymbolsInfo *info, ئاچقۇچ_Info *keyi, ئىپادە_Def *arrayNdx,
              تعداد group_field field, xkb_layout_index_t *ndx_rtrn)
{
    أكد (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ثابت حرف *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    إذا (arrayNdx == عدم) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            إذا (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                أرجع true;
            }
        }

        إذا (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ئاچقۇچ_Info_Text(info, keyi), info->max_groups, name);
            أرجع false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        أرجع true;
    }

    إذا (ئىپادە_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ئاچقۇچ_Info_Text(info, keyi));
        أرجع false;
    }

    (*ndx_rtrn)--;
    إذا (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    أرجع true;
}

ساكن bool
Add_Symbols_To_ئاچقۇچ(SymbolsInfo *info, ئاچقۇچ_Info *keyi, ئىپادە_Def *arrayNdx,
                ئىپادە_Def *قىممەت)
{
    xkb_layout_index_t ndx = 0;

    إذا (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        أرجع false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    إذا (قىممەت->common.type == STMT_ئىپادە_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        أرجع true;
    }

    إذا (قىممەت->common.type != STMT_ئىپادە_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(قىممەت->common.type), ndx + 1,
                ئاچقۇچ_Info_Text(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ئاچقۇچ_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ئاچقۇچ_Info_Text(info, keyi), ndx + 1);
        أرجع false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (ئىپادە_Keysym_List *keysymList = (ئىپادە_Keysym_List *) قىممەت;
         keysymList;
         keysymList = (ئىپادە_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        إذا (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    إذا (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    إذا (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    لكل (ئىپادە_Keysym_List *keysymList = (ئىپادە_Keysym_List *) قىممەت;
         keysymList && level < nLevels;
         keysymList = (ئىپادە_Keysym_List *) keysymList->common.next, level++) {
        بنية xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->رەقەم_syms == 0);

        إذا (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ئاچقۇچ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            أرجع false;
        }

        leveli->رەقەم_syms = (xkb_keysym_سانى_t) darray_size(keysymList->syms);
        بدل (leveli->رەقەم_syms) {
        حالة 0:
            leveli->s.sym = XKB_ئاچقۇچ_No_بەلگە;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_ئاچقۇچ_No_بەلگە);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_سانى_t k = 0; k < leveli->رەقەم_syms; k++)
                أكد(leveli->s.syms[k] != XKB_ئاچقۇچ_No_بەلگە);
#endif
        }
    }

    أرجع true;
}

ساكن bool
Add_Actions_To_ئاچقۇچ(SymbolsInfo *info, ئاچقۇچ_Info *keyi, ئىپادە_Def *arrayNdx,
                ئىپادە_Def *قىممەت)
{
    xkb_layout_index_t ndx = 0;

    إذا (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        أرجع false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    إذا (قىممەت->common.type == STMT_ئىپادە_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        أرجع true;
    }

    إذا (قىممەت->common.type != STMT_ئىپادە_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 قىممەت->common.type, ndx, ئاچقۇچ_Info_Text(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ئاچقۇچ_Info_Text(info, keyi), ndx);
        أرجع false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &قىممەت->common; p; p = p->next)
        nLevels++;
    إذا (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    لكل (ئىپادە_Action_List *actionList = (ئىپادە_Action_List *) قىممەت;
         actionList;
         actionList = (ئىپادە_Action_List *) actionList->common.next, level++) {
        بنية xkb_level *leveli = &darray_item(groupi->levels, level);
        أكد(leveli->رەقەم_actions == 0);

        unsigned صحيح رەقەم_actions = 0;
        لكل (ئىپادە_Def *act = actionList->actions;
             act; act = (ئىپادە_Def *) act->common.next)
            رەقەم_actions++;

        إذا (unlikely(رەقەم_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ئاچقۇچ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, رەقەم_actions);
            أرجع false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (ئىپادە_Def *act = actionList->actions;
             act; act = (ئىپادە_Def *) act->common.next) {
            اتحاد xkb_action toAct = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            إذا (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_قىممەت,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ئاچقۇچ_Info_Text(info, keyi), ndx + 1, level + 1);
                إذا (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    أرجع false;
                } وإلا {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            إذا (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } وإلا إذا (likely(رەقەم_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->رەقەم_actions = 1;
                leveli->a.action = toAct;
                اذهب next_level;
            } وإلا {
                darray_append(actions, toAct);
            }
        }
        إذا (darray_empty(actions)) {
            leveli->رەقەم_actions = 0;
        } وإلا إذا (likely(darray_size(actions) > 1)) {
            leveli->رەقەم_actions = (xkb_action_سانى_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_سانى_t k = 0; k < leveli->رەقەم_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } وإلا {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(رەقەم_actions > darray_size(actions));
            leveli->رەقەم_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        إذا (leveli->رەقەم_actions > 0 || leveli->رەقەم_syms > 0)
            nonEmptyLevels = level + 1;
    }

    إذا (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        إذا (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        وإلا
            darray_free(groupi->levels);
    }

    أرجع true;
}

ساكن ثابت LookupEntry repeatEntries[] = {
    { "true", ئاچقۇچ_REPEAT_YES },
    { "yes", ئاچقۇچ_REPEAT_YES },
    { "on", ئاچقۇچ_REPEAT_YES },
    { "false", ئاچقۇچ_REPEAT_NO },
    { "no", ئاچقۇچ_REPEAT_NO },
    { "off", ئاچقۇچ_REPEAT_NO },
    { "default", ئاچقۇچ_REPEAT_UNDEFINED },
    { عدم, 0 }
};

/** Handle overlay<N> = <key> entry */
ساكن bool
ئىپادە_Resolve_Overlay_Entry(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *field, ثابت ئىپادە_Def *arrayNdx,
                        ثابت ئىپادە_Def *ئىپادە, ئاچقۇچ_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ثابت بنية xkb_ئاچقۇچ **ئاچقۇچ_rtrn)
{
    /* Overlay index */
    إذا (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, ئاچقۇچ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    static_assert(حجم(xkb_overlay_index_t) == حجم(uint8_t), "");
    ثابت size_t prefix = حجم("overlay") - 1;
    ثابت size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    إذا (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, ئاچقۇچ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        أرجع false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_KEYNAME_LITERAL:
        *ئاچقۇچ_rtrn = Xkb_ئاچقۇچ_By_Name(&keymap_info->keymap, ئىپادە->ئاچقۇچ_name.ئاچقۇچ_name,
                                 false);
        إذا (!*ئاچقۇچ_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, ئىپادە->ئاچقۇچ_name.ئاچقۇچ_name),
                    field, ئاچقۇچ_Name_Text(keymap_info->keymap.ctx, keyi->name));
            أرجع false;
        }
        أرجع true;
    حالة STMT_ئىپادە_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_text(keymap_info->keymap.ctx, ئىپادە->ident.ident);
        إذا (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ئاچقۇچ_rtrn = عدم;
            أرجع true;
        } وإلا إذا (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ئاچقۇچ_rtrn = عدم;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            أرجع true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قىممەت,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, ئاچقۇچ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قىممەت,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_ئىپادە_KEYNAME_LITERAL),
                field, ئاچقۇچ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;
    }
}

ساكن bool
SetSymbolsField(SymbolsInfo *info, ئاچقۇچ_Info *keyi, ثابت حرف *field,
                ئىپادە_Def *arrayNdx, ئىپادە_Def **قىممەت_ptr)
{
    ئىپادە_Def * restrict ثابت قىممەت = *قىممەت_ptr;
    إذا (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        إذا (!ئىپادە_Resolve_String(info->ctx, قىممەت, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            أرجع false;
        }

        إذا (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ئاچقۇچ_FIELD_DEFAULT_TYPE;
        }
        وإلا إذا (ئىپادە_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }
        وإلا {
            ndx--;
            إذا (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    وإلا إذا (istreq(field, "symbols")) {
        أرجع Add_Symbols_To_ئاچقۇچ(info, keyi, arrayNdx, قىممەت);
    }
    وإلا إذا (istreq(field, "actions")) {
        أرجع Add_Actions_To_ئاچقۇچ(info, keyi, arrayNdx, قىممەت);
    }
    وإلا إذا (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        إذا (!ئىپادە_Resolve_Mod_Mask(info->ctx, قىممەت, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(قىممەت->common.type),
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= ئاچقۇچ_FIELD_VMODMAP;
    }
    وإلا إذا (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ئاچقۇچ_Info_Text(info, keyi));
    }
    وإلا إذا (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ئاچقۇچ_Info_Text(info, keyi));
    }
    وإلا إذا (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ئاچقۇچ_Info_Text(info, keyi));
    }
    وإلا إذا (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ = عدم;
        إذا (!ئىپادە_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *قىممەت_ptr, keyi, &overlay, &ئاچقۇچ))
            أرجع false;
        إذا (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            أرجع true;
        } وإلا إذا (ئاچقۇچ && ئاچقۇچ->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ئاچقۇچ_Info_Text(info, keyi));
        } وإلا {
            ثابت بنية xkb_ئاچقۇچ *prev;
            إذا (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                إذا (ئاچقۇچ != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ئاچقۇچ_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ئاچقۇچ_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? ئاچقۇچ_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ئاچقۇچ
                                 ? ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ->name)
                                 : "none"));
                }
            } وإلا إذا (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                إذا (!overlays_قىستۇرۇش(keyi, overlay, ئاچقۇچ))
                    أرجع false;
                keyi->defined |= ئاچقۇچ_FIELD_OVERLAY;
            } وإلا {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                إذا (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    إذا (ئاچقۇچ) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_ئاچقۇچ = ئاچقۇچ;
                    } وإلا {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ئاچقۇچ_FIELD_OVERLAY;
                } وإلا إذا (keyi->overlays) {
                    إذا (ئاچقۇچ) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ئاچقۇچ_Info_Text(info, keyi), keyi->overlays,
                                ئاچقۇچ_Name_Text(info->ctx, keyi->overlay_ئاچقۇچ->name),
                                overlay + 1, ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ->name));
                        أرجع !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_قىممەت_MISMATCH);
                    } وإلا {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وإلا إذا (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        إذا (!ئىپادە_Resolve_Enum(info->ctx, قىممەت, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قىممەت,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }

        keyi->repeat = (تعداد ئاچقۇچ_repeat) val;
        keyi->defined |= ئاچقۇچ_FIELD_REPEAT;
    }
    وإلا إذا (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        إذا (!ئىپادە_Resolve_Boolean(info->ctx, قىممەت, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قىممەت,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= ئاچقۇچ_FIELD_GROUPINFO;
    }
    وإلا إذا (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        إذا (!ئىپادە_Resolve_Boolean(info->ctx, قىممەت, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قىممەت,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= ئاچقۇچ_FIELD_GROUPINFO;
    }
    وإلا إذا (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        إذا (ئىپادە_Resolve_Group(info->keymap_info, قىممەت, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ئاچقۇچ_Info_Text(info, keyi));
            أرجع false;
        }

        إذا (pending) {
            keyi->out_of_range_pending_group = true;
            ثابت darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .ئىپادە = *قىممەت_ptr,
                    .computed = false,
                    .قىممەت = 0,
                }
            );
            *قىممەت_ptr = عدم;
            static_assert(حجم(keyi->out_of_range_group_سان) ==
                          حجم(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_سان = pending_index;
        } وإلا {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_سان = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= ئاچقۇچ_FIELD_GROUPINFO;
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ئاچقۇچ_FIELDS);
    }

    أرجع true;
}

ساكن bool
SetGroupName(SymbolsInfo *info, ئىپادە_Def *arrayNdx, ئىپادە_Def *قىممەت,
             تعداد merge_mode merge)
{
    إذا (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        أرجع false;
    }

    xkb_layout_index_t group = 0;
    إذا (ئىپادە_Resolve_Group(info->keymap_info, arrayNdx, false, &group, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!ئىپادە_Resolve_String(info->ctx, قىممەت, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        أرجع false;
    }

    xkb_layout_index_t group_to_use;
    إذا (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    وإلا إذا (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    وإلا {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        أرجع false;
    }

    إذا (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } وإلا {
        ثابت xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        إذا (old_name != XKB_ATOM_NONE && old_name != name) {
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

    أرجع true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    ئىپادە_Def *arrayNdx;
    bool ret;

    إذا (!ئىپادە_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        أرجع false;

    إذا (elem && istreq(elem, "key")) {
        ئاچقۇچ_Info temp = {0};
        Init_ئاچقۇچ_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->قىممەت) &&
              Merge_ئاچقۇچلار(info, &info->default_ئاچقۇچ, &temp, true);
        Clear_ئاچقۇچ_Info(&temp);
    }
    وإلا إذا (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->قىممەت, stmt->merge);
    }
    وإلا إذا (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وإلا إذا (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->قىممەت, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    أرجع ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ئاچقۇچ_Info *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.next) {
        ثابت حرف *field;
        ئىپادە_Def *arrayNdx;
        bool ok = true;

        إذا (!def->name) {
            إذا (unlikely(!def->قىممەت) || def->قىممەت->common.type != STMT_ئىپادە_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            وإلا
                field = "actions";
            arrayNdx = عدم;
        }
        وإلا {
            ثابت حرف *elem;
            ok = ئىپادە_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            إذا (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        إذا (unlikely(!def->قىممەت)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        إذا (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->قىممەت))
            all_valid_entries = false;
    }

    أرجع all_valid_entries;
}

ساكن bool
SetExplicitGroup(SymbolsInfo *info, ئاچقۇچ_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    إذا (info->explicit_group == XKB_LAYOUT_INVALID)
        أرجع true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        إذا (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    إذا (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, ئاچقۇچ_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    إذا (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    أرجع true;
}

ساكن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ئاچقۇچ_Info keyi;

    keyi = info->default_ئاچقۇچ;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ئاچقۇچ.groups);
    لكل (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_ئاچقۇچ.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->ئاچقۇچ_Name;

    إذا (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_ئاچقۇچ_Symbols(info, &keyi, true)) {
        أرجع true;
    }

    Clear_ئاچقۇچ_Info(&keyi);
    info->error_سانى++;
    أرجع false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    بنية xkb_context *ctx = info->ctx;

    إذا (!ئىپادە_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        أرجع false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_ئىپادە_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    لكل (ئىپادە_Def *ئاچقۇچ = def->ئاچقۇچلار; ئاچقۇچ; ئاچقۇچ = (ئىپادە_Def *) ئاچقۇچ->common.next) {
        إذا (ئاچقۇچ->common.type == STMT_ئىپادە_KEYNAME_LITERAL) {
            tmp.have_بەلگە = false;
            tmp.u.ئاچقۇچ_Name = ئاچقۇچ->ئاچقۇچ_name.ئاچقۇچ_name;
        }
        وإلا إذا (ئاچقۇچ->common.type == STMT_ئىپادە_KEYSYM_LITERAL) {
            إذا (ئاچقۇچ->keysym.keysym == XKB_ئاچقۇچ_No_بەلگە) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } وإلا {
                tmp.have_بەلگە = true;
                tmp.u.ئاچقۇچ_Sym = ئاچقۇچ->keysym.keysym;
            }
        }
        وإلا {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    أرجع ok;
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

        إذا (!ok)
            info->error_سانى++;

        إذا (info->error_سانى > 10) {
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
ساكن بنية xkb_ئاچقۇچ *
Find_ئاچقۇچ_For_بەلگە(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    افعل {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        افعل {
            got_one_level = false;
            بنية xkb_ئاچقۇچ *ئاچقۇچ;
            xkb_ئاچقۇچلار_foreach(ئاچقۇچ, keymap) {
                إذا (group < ئاچقۇچ->رەقەم_groups &&
                    level < Xkb_ئاچقۇچ_رەقەم_Levels(ئاچقۇچ, group)) {
                    got_one_group = got_one_level = true;
                    ثابت xkb_keysym_سانى_t رەقەم_syms =
                        ئاچقۇچ->groups[group].levels[level].رەقەم_syms;
                    إذا (رەقەم_syms > 1) {
                        لكل (xkb_keysym_سانى_t k = 0; k < رەقەم_syms; k++) {
                            إذا (ئاچقۇچ->groups[group].levels[level].s.syms[k] == sym)
                                أرجع ئاچقۇچ;
                        }
                    } وإلا إذا (رەقەم_syms &&
                               ئاچقۇچ->groups[group].levels[level].s.sym == sym) {
                        أرجع ئاچقۇچ;
                    }
                }
            }
            level++;
        } طالما (got_one_level);
        group++;
    } طالما (got_one_group);

    أرجع عدم;
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
    (darray_item(groupi->levels, level).رەقەم_syms == 0 ? \
        XKB_ئاچقۇچ_No_بەلگە : \
     darray_item(groupi->levels, level).رەقەم_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    إذا (width == 1 || width <= 0)
        أرجع xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    إذا (width == 2) {
        إذا (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            أرجع xkb_atom_intern_literal(ctx, "ALPHABETIC");

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    إذا (width <= 4) {
        إذا (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_ئاچقۇچ_No_بەلگە);

            إذا (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    أرجع XKB_ATOM_NONE;

#undef GET_SYM
}

ساكن ثابت بنية xkb_ئاچقۇچ_type *
FindTypeForGroup(بنية xkb_keymap *keymap, ئاچقۇچ_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    إذا (type_name == XKB_ATOM_NONE) {
        إذا (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وإلا {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            إذا (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    إذا (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ئاچقۇچ_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ئاچقۇچ_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        اذهب use_default;
    }

    darray_size_t i;
    لكل (i = 0; i < keymap->رەقەم_types; i++)
        إذا (keymap->types[i].name == type_name)
            اكسر;

    إذا (i >= keymap->رەقەم_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ئاچقۇچ_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 ئاچقۇچ_Name_Text(keymap->ctx, keyi->name), group + 1);
        اذهب use_default;
    }

    keymap->types[i].required = true;
    أرجع &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    أرجع &keymap->types[0];
}

ساكن bool
CopySymbolsDefToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                       ئاچقۇچ_Info *keyi)
{
    بنية xkb_ئاچقۇچ *ئاچقۇچ;
    GroupInfo *groupi;
    ثابت GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ئاچقۇچ = Xkb_ئاچقۇچ_By_Name(keymap, keyi->name, false);
    إذا (!ئاچقۇچ) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ئاچقۇچ_Info_Text(info, keyi));
        أرجع false;
    }

    /* Find the range of groups we need. */
    ئاچقۇچ->رەقەم_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & ئاچقۇچ_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        إذا (darray_size(groupi->levels) > 0 || has_explicit_type)
            ئاچقۇچ->رەقەم_groups = i + 1;
        إذا (has_explicit_type)
            ئاچقۇچ->explicit |= EXPLICIT_TYPES;
    }

    إذا (ئاچقۇچ->رەقەم_groups <= 0) {
        /* A key with no group may still have other fields defined */
        إذا (keyi->defined)
            اذهب ئاچقۇچ_fields;
        وإلا
            أرجع false;
    }

    darray_resize(keyi->groups, ئاچقۇچ->رەقەم_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        إذا (groupi->defined)
            استمر;

        CopyGroupInfo(groupi, group0);
    }

    ئاچقۇچ->groups = calloc(ئاچقۇچ->رەقەم_groups, حجم(*ئاچقۇچ->groups));
    أكد(darray_size(keyi->groups) == ئاچقۇچ->رەقەم_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ئاچقۇچ->رەقەم_groups);
        #endif
        bool explicit_type = false;
        ثابت بنية xkb_ئاچقۇچ_type * ثابت type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        إذا (type->رەقەم_levels < darray_size(groupi->levels)) {
            بنية xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->رەقەم_levels,
                    ئاچقۇچ_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->رەقەم_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->رەقەم_levels);

        ئاچقۇچ->groups[i].explicit_type = explicit_type;
        ئاچقۇچ->groups[i].type = type;
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
            بدل (leveli->رەقەم_syms) {
            حالة 0:
                leveli->upper = XKB_ئاچقۇچ_No_بەلگە;
                اكسر;
            حالة 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                لكل (xkb_keysym_سانى_t k = 0; k < leveli->رەقەم_syms; k++) {
                    ثابت xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    إذا (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        اكسر;
                    }
                }
                إذا (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->رەقەم_syms *
                                             حجم(*leveli->s.syms));
                    إذا (!leveli->s.syms)
                        أرجع false; /* FIXME: better handling? */
                    لكل (xkb_keysym_سانى_t k = 0; k < leveli->رەقەم_syms; k++) {
                        leveli->s.syms[leveli->رەقەم_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &ئاچقۇچ->groups[i].levels, عدم);
        إذا (ئاچقۇچ->groups[i].type->رەقەم_levels > 1 ||
            ئاچقۇچ->groups[i].levels[0].رەقەم_syms > 0) {
            ئاچقۇچ->groups[i].explicit_symbols = true;
            ئاچقۇچ->explicit |= EXPLICIT_SYMBOLS;
        }
        إذا (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            ئاچقۇچ->groups[i].explicit_actions = true;
            ئاچقۇچ->explicit |= EXPLICIT_INTERP;
        }
        إذا (ئاچقۇچ->groups[i].explicit_type)
            ئاچقۇچ->explicit |= EXPLICIT_TYPES;
    }

    ئاچقۇچ->out_of_range_pending_group = keyi->out_of_range_pending_group;
    ئاچقۇچ->out_of_range_group_سان = keyi->out_of_range_group_سان;
    ئاچقۇچ->out_of_range_group_policy = keyi->out_of_range_group_policy;

ئاچقۇچ_fields:
    إذا (keyi->defined & ئاچقۇچ_FIELD_VMODMAP) {
        ئاچقۇچ->vmodmap = keyi->vmodmap;
        ئاچقۇچ->explicit |= EXPLICIT_VMODMAP;
    }

    إذا (keyi->repeat != ئاچقۇچ_REPEAT_UNDEFINED) {
        ئاچقۇچ->repeats = (keyi->repeat == ئاچقۇچ_REPEAT_YES);
        ئاچقۇچ->explicit |= EXPLICIT_REPEAT;
    }

    إذا ((keyi->defined & ئاچقۇچ_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ئاچقۇچ->overlays = keyi->overlays;
        إذا (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = ئاچقۇچ->overlays;
            ثابت بنية xkb_ئاچقۇچ **overlays_ئاچقۇچلار = keyi->overlays_ئاچقۇچلار;
            طالما (remaining) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                إذا (*overlays_ئاچقۇچلار) {
                    overlays_ئاچقۇچلار++;
                } وإلا {
                    /* drop current null value (overlayN=none) */
                    ئاچقۇچ->overlays &= ~lsb;
                }
            }

            إذا (ئاچقۇچ->overlays) {
                /* Steal */
                ئاچقۇچ->overlays_ئاچقۇچلار = keyi->overlays_ئاچقۇچلار;
                keyi->overlays_ئاچقۇچلار = عدم;
                keyi->overlays_alloc = 0;

                ئاچقۇچ->overlays_inline = false;
                ئاچقۇچ->explicit |= EXPLICIT_OVERLAY;
            }
        } وإلا {
            ئاچقۇچ->overlay_ئاچقۇچ = keyi->overlay_ئاچقۇچ;
            ئاچقۇچ->overlays_inline = true;
            ئاچقۇچ->explicit |= EXPLICIT_OVERLAY;
        }
    }

    أرجع true;
}

ساكن bool
CopyModMapDefToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    بنية xkb_ئاچقۇچ *ئاچقۇچ;

    إذا (!entry->have_بەلگە) {
        ئاچقۇچ = Xkb_ئاچقۇچ_By_Name(keymap, entry->u.ئاچقۇچ_Name, true);
        إذا (!ئاچقۇچ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ئاچقۇچ_Name_Text(info->ctx, entry->u.ئاچقۇچ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            أرجع false;
        }
    }
    وإلا {
        ئاچقۇچ = Find_ئاچقۇچ_For_بەلگە(keymap, entry->u.ئاچقۇچ_Sym);
        إذا (!ئاچقۇچ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_بەلگە,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.ئاچقۇچ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            أرجع false;
        }
    }

    /* Skip modMap None */
    إذا (entry->mods) {
        ئاچقۇچ->modmap |= entry->mods;
        entry->ئاچقۇچ_Code = ئاچقۇچ->keycode;
    }

    أرجع true;
}

ساكن bool
CopySymbolsToKeymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    ئاچقۇچ_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->رەقەم_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, عدم);

    darray_foreach(keyi, info->ئاچقۇچلار)
        إذا (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_سانى++;

    إذا (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_ئاچقۇچ *ئاچقۇچ;

        xkb_ئاچقۇچلار_foreach(ئاچقۇچ, keymap) {
            إذا (ئاچقۇچ->name == XKB_ATOM_NONE)
                استمر;

            إذا (ئاچقۇچ->رەقەم_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         ئاچقۇچ_Name_Text(info->ctx, ئاچقۇچ->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        إذا (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_سانى++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->رەقەم_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    أرجع true;
}

bool
CompileSymbols(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    إذا (file !=عدم)
        HandleSymbolsFile(&info, file);

    إذا (info.error_سانى != 0)
        اذهب err_info;

    إذا (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    أرجع true;

err_info:
    ClearSymbolsInfo(&info);
    أرجع false;
}
