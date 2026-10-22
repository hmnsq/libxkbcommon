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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-keysyms.h"

#অন্তর্ভাব "action.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-numbers.h"
#অন্তর্ভাব "util-mem.h"
#অন্তর্ভাব "vmod.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "xkbcomp/ast.h"

গণনা চাবি_repeat {
    চাবি_REPEAT_UNDEFINED = 0,
    চাবি_REPEAT_YES,
    চাবি_REPEAT_NO,
    চাবি_REPEAT_অঙ্ক_ENTRIES,
};
গণনা { চাবি_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(চাবি_REPEAT_অঙ্ক_ENTRIES - 1 <=
              (1 << (চাবি_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

গণনা group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

গণনা চাবি_field {
    চাবি_FIELD_NONE = 0,
    চাবি_FIELD_REPEAT = (1 << 0),
    চাবি_FIELD_DEFAULT_TYPE = (1 << 1),
    চাবি_FIELD_GROUPINFO = (1 << 2),
    চাবি_FIELD_VMODMAP = (1 << 3),
    চাবি_FIELD_OVERLAY = (1 << 4),
    চাবি_FIELD_ALL = চাবি_FIELD_REPEAT | চাবি_FIELD_DEFAULT_TYPE
                  | চাবি_FIELD_GROUPINFO | চাবি_FIELD_VMODMAP | চাবি_FIELD_OVERLAY,
};
গণনা { চাবি_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(চাবি_FIELD_ALL <= (1 << (চাবি_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

প্রকারনির্দেশ সংরচনা {
    darray(সংরচনা xkb_level) levels;
    গণনা group_field defined;
    xkb_atom_t type;
} GroupInfo;

গণনা {
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
              (unsigned)চাবি_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)চাবি_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

প্রকারনির্দেশ সংরচনা {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_সংখ্যা;
    darray(GroupInfo) groups;
    গণনা xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    গণনা চাবি_field defined:KEYINFO_DEFINED_BIT_FIELD;
    গণনা merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    গণনা চাবি_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    সঙ্ঘ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ধ্রুব সংরচনা xkb_চাবি *overlay_চাবি;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ধ্রুব সংরচনা xkb_চাবি **overlays_চাবিগুলি;
    };
} চাবি_Info;

স্থির শূন্য
StealLevelInfo(সংরচনা xkb_level *into, সংরচনা xkb_level *from)
{
    clear_level(into);
    যদি (from->অঙ্ক_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } অন্যথা {
        into->s.sym = from->s.sym;
    }
    into->অঙ্ক_syms = from->অঙ্ক_syms;
    from->অঙ্ক_syms = 0;

    যদি (from->অঙ্ক_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } অন্যথা {
        into->a.action = from->a.action;
    }
    into->অঙ্ক_actions = from->অঙ্ক_actions;
    from->অঙ্ক_actions = 0;
}

স্থির শূন্য
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, পরিমাণ(*groupi));
}

স্থির শূন্য
ClearGroupInfo(GroupInfo *groupi)
{
    সংরচনা xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

স্থির শূন্য
CopyGroupInfo(GroupInfo *to, ধ্রুব GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    প্রতি (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        যদি (darray_item(from->levels, j).অঙ্ক_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).অঙ্ক_syms,
                       পরিমাণ(xkb_keysym_t));
        }
        যদি (darray_item(from->levels, j).অঙ্ক_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).অঙ্ক_actions,
                       পরিমাণ(সঙ্ঘ xkb_action));
        }
    }
}

স্থির শূন্য
Init_চাবি_Info(সংরচনা xkb_context *ctx, চাবি_Info *keyi)
{
    memset(keyi, 0, পরিমাণ(*keyi));
    static_assert(!DEFAULT_চাবি_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_চাবি_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_চাবি = রিক্ত;
}

স্থির শূন্য
Clear_চাবি_Info(চাবি_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    যদি (keyi->overlays_alloc)
        মুক্ত(keyi->overlays_চাবিগুলি);
}

/***====================================================================***/

প্রকারনির্দেশ সংরচনা {
    বর্ণ *name;         /* e.g. pc+us+inet(evdev) */
    পূর্ণাঙ্ক error_গুনতি;
    unsigned পূর্ণাঙ্ক include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(চাবি_Info) চাবিগুলি;
    চাবি_Info default_চাবি;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    সংরচনা xkb_mod_set mods;

    সংরচনা xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ধ্রুব সংরচনা xkb_keymap_info *keymap_info;
} SymbolsInfo;

স্থির শূন্য
InitSymbolsInfo(SymbolsInfo *info, ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                unsigned পূর্ণাঙ্ক include_depth, ধ্রুব সংরচনা xkb_mod_set *mods)
{
    memset(info, 0, পরিমাণ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_চাবি_Info(keymap_info->keymap.ctx, &info->default_চাবি);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

স্থির শূন্য
ClearSymbolsInfo(SymbolsInfo *info)
{
    চাবি_Info *keyi;
    মুক্ত(info->name);
    darray_foreach(keyi, info->চাবিগুলি)
        Clear_চাবি_Info(keyi);
    darray_free(info->চাবিগুলি);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_চাবি_Info(&info->default_চাবি);
}

স্থির ধ্রুব বর্ণ *
চাবি_Info_Text(SymbolsInfo *info, চাবি_Info *keyi)
{
    প্রত্যাগম চাবি_Name_Text(info->ctx, keyi->name);
}

স্থির bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t চাবি_name)
{
    /* First find the type of the merged group. */
    যদি (into->type != from->type) {
        যদি (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        অন্যথা যদি (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        অন্যথা {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            যদি (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, চাবি_Name_Text(info->ctx, চাবি_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    যদি (darray_empty(from->levels)) {
        InitGroupInfo(from);
        প্রত্যাগম true;
    }

    যদি (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        প্রত্যাগম true;
    }

    /* Merge the actions and syms. */
    ধ্রুব darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_গুনতি = 0;
    darray_size_t from_Actions_গুনতি = 0;
    প্রতি (darray_size_t i = 0; i < levels_in_both; i++) {
        সংরচনা xkb_level* ধ্রুব intoLevel = &darray_item(into->levels, i);
        সংরচনা xkb_level* ধ্রুব fromLevel = &darray_item(from->levels, i);

        ধ্রুব bool fromHasNoKeysym = fromLevel->অঙ্ক_syms == 0;
        ধ্রুব bool fromHasNoAction = fromLevel->অঙ্ক_actions == 0;
        যদি (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            অনুবর্ত;
        }

        ধ্রুব bool intoHasNoKeysym = intoLevel->অঙ্ক_syms == 0;
        ধ্রুব bool intoHasNoAction = intoLevel->অঙ্ক_actions == 0;
        যদি (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_গুনতি++;
            from_Actions_গুনতি++;
            অনুবর্ত;
        }

        /* Possible level conflict */
        প্রতিজ্ঞা(intoLevel->অঙ্ক_syms > 0 || intoLevel->অঙ্ক_actions > 0);
        প্রতিজ্ঞা(fromLevel->অঙ্ক_syms > 0 || fromLevel->অঙ্ক_actions > 0);

        /* Handle keysyms */
        যদি (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            যদি (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_প্রতীক,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, চাবি_Name_Text(info->ctx, চাবি_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            যদি (fromHasNoKeysym) {
                /* No keysym to copy */
            } অন্যথা যদি (clobber) {
                /* Override: copy any defined keysym from `from` */
                যদি (unlikely(fromLevel->অঙ্ক_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    যদি (unlikely(intoLevel->অঙ্ক_syms > 1))
                        মুক্ত(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->অঙ্ক_syms = fromLevel->অঙ্ক_syms;
                    fromLevel->অঙ্ক_syms = 0;
                    from_Keysyms_গুনতি++;
                } অন্যথা যদি (fromLevel->s.sym != XKB_চাবি_No_প্রতীক) {
                    /* Single defined keysym */
                    যদি (unlikely(intoLevel->অঙ্ক_syms > 1))
                        মুক্ত(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->অঙ্ক_syms = 1;
                    from_Keysyms_গুনতি++;
                }
            } অন্যথা {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                যদি (unlikely(intoLevel->অঙ্ক_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } অন্যথা যদি (intoLevel->s.sym == XKB_চাবি_No_প্রতীক) {
                    /* Single undefined keysym */
                    যদি (unlikely(fromLevel->অঙ্ক_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->অঙ্ক_syms = fromLevel->অঙ্ক_syms;
                        fromLevel->অঙ্ক_syms = 0;
                    } অন্যথা {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->অঙ্ক_syms = 1;
                    }
                    from_Keysyms_গুনতি++;
                }
            }
        }

        /* Handle actions */
        যদি (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            যদি (report && !(intoHasNoAction || fromHasNoAction)) {
                যদি (unlikely(intoLevel->অঙ্ক_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_চাবি_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, চাবি_Name_Text(info->ctx, চাবি_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } অন্যথা {
                    সঙ্ঘ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                চাবি_Name_Text(info->ctx, চাবি_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            যদি (fromHasNoAction) {
                /* No action to copy */
            } অন্যথা যদি (clobber) {
                /* Override: copy any defined action from `from` */
                যদি (unlikely(fromLevel->অঙ্ক_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    যদি (unlikely(intoLevel->অঙ্ক_actions > 1))
                        মুক্ত(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->অঙ্ক_actions = fromLevel->অঙ্ক_actions;
                    fromLevel->অঙ্ক_actions = 0;
                    from_Actions_গুনতি++;
                } অন্যথা যদি (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    যদি (unlikely(intoLevel->অঙ্ক_actions > 1))
                        মুক্ত(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->অঙ্ক_actions = 1;
                    from_Actions_গুনতি++;
                }
            } অন্যথা {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                যদি (unlikely(intoLevel->অঙ্ক_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } অন্যথা যদি (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    যদি (unlikely(fromLevel->অঙ্ক_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->অঙ্ক_actions = fromLevel->অঙ্ক_actions;
                        fromLevel->অঙ্ক_actions = 0;
                    } অন্যথা {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->অঙ্ক_actions = 1;
                    }
                    from_Actions_গুনতি++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    সংরচনা xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->অঙ্ক_syms = 0;
        level->অঙ্ক_actions = 0;
        from_Keysyms_গুনতি++;
        from_Actions_গুনতি++;
    }
    যদি (from_Keysyms_গুনতি) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        যদি (from_Keysyms_গুনতি == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    যদি (from_Actions_গুনতি) {
        /* Reset defined actions field if we used no action from `into` */
        যদি (from_Actions_গুনতি == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    প্রত্যাগম true;
}

স্থির bool
Use_New_চাবি_Field(গণনা চাবি_field field, গণনা চাবি_field old, গণনা চাবি_field new,
               bool clobber, bool report, গণনা চাবি_field *collide)
{
    যদি (!(old & field))
        প্রত্যাগম (new & field);

    যদি (new & field) {
        যদি (report)
            *collide |= field;

        প্রত্যাগম clobber;
    }

    প্রত্যাগম false;
}

স্থির bool
overlays_get(ধ্রুব চাবি_Info *info, xkb_overlay_index_t bit,
             ধ্রুব সংরচনা xkb_চাবি **চাবি_out)
{
    যদি (bit >= (xkb_overlay_index_t)(পরিমাণ(xkb_overlay_mask_t) * CHAR_BIT))
        প্রত্যাগম false;
    ধ্রুব xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    যদি (!(info->overlays & mask)) প্রত্যাগম false;
    যদি (চাবি_out) {
        যদি (!info->overlays_alloc) {
            /* Stored inline */
            *চাবি_out = info->overlay_চাবি;
        } অন্যথা {
            /* Stored in sparse array */
            ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ধ্রুব xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(নিচের);
            *চাবি_out = info->overlays_চাবিগুলি[index];
        }
    }
    প্রত্যাগম true;
}

স্থির bool
overlays_ঢোকাও(চাবি_Info *keyi, xkb_overlay_index_t bit,
                ধ্রুব সংরচনা xkb_চাবি *চাবি)
{
    যদি (bit >= (xkb_overlay_index_t)(পরিমাণ(xkb_overlay_mask_t) * CHAR_BIT))
        প্রত্যাগম false;

    ধ্রুব xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    যদি ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        যদি (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_চাবি = চাবি;
            যদি (!চাবি)
                keyi->overlays_clear = true;
        } অন্যথা {
            /* Stored in sparse array */
            ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ধ্রুব xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(নিচের);
            keyi->overlays_চাবিগুলি[index] = চাবি;
        }
        প্রত্যাগম true;
    }

    /* Insert new element */
    যদি (!keyi->overlays || (keyi->overlays_clear && !চাবি)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_চাবি = চাবি;
        keyi->overlays |= mask;
        keyi->overlays_clear = !চাবি;
    } অন্যথা যদি (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ধ্রুব xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ধ্রুব xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ধ্রুব সংরচনা xkb_চাবি ** ধ্রুব tmp = calloc(alloc, পরিমাণ(*tmp));
        যদি (!tmp) প্রত্যাগম false;

        /* Copy previous entries to the buffer */
        যদি (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ধ্রুব xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(নিচের);
            tmp[idx] = keyi->overlay_চাবি;
        } অন্যথা {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ধ্রুব xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(নিচের);
        tmp[idx] = চাবি;

        keyi->overlays_চাবিগুলি = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } অন্যথা {
        /* Allocated items: grow heap and shift if relevant */
        ধ্রুব xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ধ্রুব xkb_overlay_index_t গুনতি =
            (xkb_overlay_index_t)popcount32(overlays);
        যদি (গুনতি > keyi->overlays_alloc) {
            ধ্রুব xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(গুনতি);
            প্রতিজ্ঞা(alloc >= গুনতি);
            ধ্রুব সংরচনা xkb_চাবি ** ধ্রুব tmp = realloc(
                keyi->overlays_চাবিগুলি,
                (size_t)alloc * পরিমাণ(*keyi->overlays_চাবিগুলি)
            );
            যদি (!tmp) প্রত্যাগম false;
            keyi->overlays_চাবিগুলি = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ধ্রুব xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(নিচের);
        যদি (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        যদি (index < গুনতি - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            প্রতিজ্ঞা(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_চাবিগুলি + index + 1, keyi->overlays_চাবিগুলি + index,
                    (size_t)(গুনতি - 1u - index) * পরিমাণ(ধ্রুব সংরচনা xkb_চাবি *));
        }
        keyi->overlays_চাবিগুলি[index] = চাবি;
        keyi->overlays = overlays;
    }

    প্রত্যাগম true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
স্থির bool
merge_overlays(SymbolsInfo *info, চাবি_Info *into, চাবি_Info *from,
               bool clobber, bool report, গণনা চাবি_field *collide)
{
    যদি (!(from->defined & চাবি_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } অন্যথা যদি (!(into->defined & চাবি_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_চাবিগুলি = from->overlays_চাবিগুলি;
        from->overlays_চাবিগুলি = রিক্ত;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= চাবি_FIELD_OVERLAY;
    } অন্যথা যদি (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } অন্যথা {
        যদি (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ধ্রুব xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ধ্রুব xkb_overlay_index_t গুনতি =
                (xkb_overlay_index_t)popcount32(result_mask);

            যদি (গুনতি == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            চাবি_Info *dest = into;
            চাবি_Info *src = from;
            যদি (into->overlays_alloc >= গুনতি &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } অন্যথা যদি (from->overlays_alloc >= গুনতি) {
                dest = from;
                src = into;
                clobber = !clobber;
            } অন্যথা যদি (গুনতি > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                যদি (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                যদি (!dest->overlays_alloc) {
                    ধ্রুব সংরচনা xkb_চাবি **tmp = calloc(গুনতি, পরিমাণ(*tmp));
                    যদি (!tmp)
                        প্রত্যাগম false;
                    যদি (dest->overlay_চাবি)
                        tmp[0] = dest->overlay_চাবি;
                    dest->overlays_চাবিগুলি = tmp;
                    dest->overlays_alloc = গুনতি;
                } অন্যথা {
                    ধ্রুব সংরচনা xkb_চাবি **tmp =
                        realloc(dest->overlays_চাবিগুলি,
                                গুনতি * পরিমাণ(*dest->overlays_চাবিগুলি));
                    যদি (!tmp)
                        প্রত্যাগম false;
                    dest->overlays_চাবিগুলি = tmp;
                    dest->overlays_alloc = গুনতি;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ধ্রুব সংরচনা xkb_চাবি **src_চাবিগুলি = (src->overlays_clear)
                ? রিক্ত
                : (src->overlays_alloc)
                    ? src->overlays_চাবিগুলি
                    : &src->overlay_চাবি;
            যাবত্ (remaining) {
                /* isolate lowest set bit */
                ধ্রুব xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                ধ্রুব xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                যদি (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ধ্রুব সংরচনা xkb_চাবি *src_চাবি = (src_চাবিগুলি)
                    ? *(src_চাবিগুলি++)
                    : রিক্ত;

                /* check dest */
                ধ্রুব সংরচনা xkb_চাবি *dest_চাবি = রিক্ত;
                ধ্রুব bool conflict = overlays_get(dest, bit, &dest_চাবি);
                যদি (conflict) {
                    যদি (dest_চাবি == src_চাবি)
                       অনুবর্ত;
                    অন্যথা যদি (report)
                        *collide |= চাবি_FIELD_OVERLAY;
                }

                যদি ((!conflict || clobber) && !overlays_ঢোকাও(dest, bit, src_চাবি))
                    প্রত্যাগম false;
            }

            যদি (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                যদি (into->overlays_alloc)
                    মুক্ত(into->overlays_চাবিগুলি);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                যদি (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_চাবিগুলি = from->overlays_চাবিগুলি;
                    from->overlays_চাবিগুলি = রিক্ত;
                    from->overlays_alloc = 0;
                } অন্যথা {
                    into->overlay_চাবি = from->overlay_চাবি;
                }
            }
        } অন্যথা {
            /* Disjoint overlays */

            যদি (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_চাবি == from->overlay_চাবি) {
                /* Indentical: do nothing */
                প্রত্যাগম true;
            }
            যদি (!(into->overlays & from->overlays)) {
                যদি (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_চাবি = from->overlay_চাবি;
                    প্রত্যাগম true;
                } অন্যথা যদি (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    প্রত্যাগম true;
                }
            }
            /* Conflict */
            যদি (report)
                *collide |= চাবি_FIELD_OVERLAY;
            যদি (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_চাবি = from->overlay_চাবি;
            }
        }
    }
    প্রত্যাগম true;
}

স্থির bool
Merge_চাবিগুলি(SymbolsInfo *info, চাবি_Info *into, চাবি_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    গণনা চাবি_field collide = 0;
    ধ্রুব পূর্ণাঙ্ক verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুব bool clobber = (from->merge != MERGE_AUGMENT);
    ধ্রুব bool report = (same_file && verbosity > 0) || verbosity > 9;

    যদি (from->merge == MERGE_REPLACE) {
        Clear_চাবি_Info(into);
        *into = *from;
        Init_চাবি_Info(info->ctx, from);
        প্রত্যাগম true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    প্রতি (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    প্রতি (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    যদি (Use_New_চাবি_Field(চাবি_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= চাবি_FIELD_VMODMAP;
    }
    যদি (Use_New_চাবি_Field(চাবি_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= চাবি_FIELD_REPEAT;
    }
    যদি (Use_New_চাবি_Field(চাবি_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= চাবি_FIELD_DEFAULT_TYPE;
    }
    যদি (Use_New_চাবি_Field(চাবি_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_সংখ্যা = from->out_of_range_group_সংখ্যা;
        into->defined |= চাবি_FIELD_GROUPINFO;
    }

    যদি (!merge_overlays(info, into, from, clobber, report, &collide))
        প্রত্যাগম false;

    যদি (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 চাবি_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_চাবি_Info(from);
    Init_চাবি_Info(info->ctx, from);
    প্রত্যাগম true;
}

স্থির xkb_atom_t
Xkb_Resolve_চাবি_Alias(ধ্রুব সংরচনা xkb_keymap *keymap, xkb_atom_t name)
{
    যদি (name < keymap->অঙ্ক_চাবি_names) {
        ধ্রুব KeycodeMatch match = keymap->চাবি_names[name];
        যদি (match.found && match.is_alias) {
            প্রত্যাগম match.alias.real;
        }
    }
    প্রত্যাগম name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
স্থির bool
Add_চাবি_Symbols(SymbolsInfo *info, চাবি_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_চাবি_Alias(&info->keymap_info->keymap, keyi->name);

    চাবি_Info *iter;
    darray_foreach(iter, info->চাবিগুলি)
        যদি (iter->name == keyi->name)
            প্রত্যাগম Merge_চাবিগুলি(info, iter, keyi, same_file);

    darray_append(info->চাবিগুলি, *keyi);
    Init_চাবি_Info(info->ctx, keyi);
    প্রত্যাগম true;
}

স্থির bool
AddModMapEntry(SymbolsInfo *info, ধ্রুব ModMapEntry *new)
{
    ধ্রুব bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        যদি ((new->have_প্রতীক != old->have_প্রতীক) ||
            (new->have_প্রতীক && new->u.চাবি_Sym != old->u.চাবি_Sym) ||
            (!new->have_প্রতীক && new->u.চাবি_Name != old->u.চাবি_Name))
            অনুবর্ত;

        যদি (new->mods == old->mods)
            প্রত্যাগম true;

        ধ্রুব xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ধ্রুব xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        যদি (new->have_প্রতীক) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.চাবি_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } অন্যথা {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    চাবি_Name_Text(info->ctx, new->u.চাবি_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        প্রত্যাগম true;
    }

    darray_append(info->modmaps, *new);

    প্রত্যাগম true;
}

/***====================================================================***/

স্থির শূন্য
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     গণনা merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    যদি (from->error_গুনতি > 0) {
        into->error_গুনতি += from->error_গুনতি;
        প্রত্যাগম;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    যদি (into->name == রিক্ত) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    প্রতি (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        যদি (!darray_item(from->group_names, i))
            অনুবর্ত;

        যদি (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            অনুবর্ত;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    যদি (darray_empty(into->চাবিগুলি)) {
        into->চাবিগুলি = from->চাবিগুলি;
        darray_init(from->চাবিগুলি);
    }
    অন্যথা {
        চাবি_Info *keyi;
        darray_foreach(keyi, from->চাবিগুলি) {
            keyi->merge = merge;
            যদি (!Add_চাবি_Symbols(into, keyi, false))
                into->error_গুনতি++;
        }
    }

    যদি (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    অন্যথা {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            যদি (!AddModMapEntry(into, mm))
                into->error_গুনতি++;
        }
    }
}

স্থির শূন্য
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

স্থির bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    যদি (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_গুনতি += 10;
        প্রত্যাগম false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    প্রতি (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        বর্ণ path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, পরিমাণ(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            ClearSymbolsInfo(&included);
            প্রত্যাগম false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        যদি (stmt->modifier) {
            ধ্রুব পূর্ণাঙ্ক গুনতি = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            যদি (গুনতি <= 0 || *(stmt->modifier + গুনতি) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } অন্যথা {
                next_incl.explicit_group--;
            }
        }
        অন্যথা যদি (info->keymap_info->keymap.অঙ্ক_groups != 0 &&
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
        অন্যথা {
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

    প্রত্যাগম (info->error_গুনতি == 0);
}

স্থির bool
GetGroupIndex(SymbolsInfo *info, চাবি_Info *keyi, রাশিমালা_Def *arrayNdx,
              গণনা group_field field, xkb_layout_index_t *ndx_rtrn)
{
    প্রতিজ্ঞা (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ধ্রুব বর্ণ *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    যদি (arrayNdx == রিক্ত) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            যদি (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                প্রত্যাগম true;
            }
        }

        যদি (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, চাবি_Info_Text(info, keyi), info->max_groups, name);
            প্রত্যাগম false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        প্রত্যাগম true;
    }

    যদি (রাশিমালা_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, রিক্ত) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, চাবি_Info_Text(info, keyi));
        প্রত্যাগম false;
    }

    (*ndx_rtrn)--;
    যদি (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    প্রত্যাগম true;
}

স্থির bool
Add_Symbols_To_চাবি(SymbolsInfo *info, চাবি_Info *keyi, রাশিমালা_Def *arrayNdx,
                রাশিমালা_Def *মান)
{
    xkb_layout_index_t ndx = 0;

    যদি (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        প্রত্যাগম false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    যদি (মান->common.type == STMT_রাশিমালা_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        প্রত্যাগম true;
    }

    যদি (মান->common.type != STMT_রাশিমালা_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(মান->common.type), ndx + 1,
                চাবি_Info_Text(info, keyi));
        প্রত্যাগম false;
    }

    যদি (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_চাবি_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                চাবি_Info_Text(info, keyi), ndx + 1);
        প্রত্যাগম false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    প্রতি (রাশিমালা_Keysym_List *keysymList = (রাশিমালা_Keysym_List *) মান;
         keysymList;
         keysymList = (রাশিমালা_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        যদি (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    যদি (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    যদি (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    প্রতি (রাশিমালা_Keysym_List *keysymList = (রাশিমালা_Keysym_List *) মান;
         keysymList && level < nLevels;
         keysymList = (রাশিমালা_Keysym_List *) keysymList->common.next, level++) {
        সংরচনা xkb_level *leveli = &darray_item(groupi->levels, level);
        প্রতিজ্ঞা(leveli->অঙ্ক_syms == 0);

        যদি (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    চাবি_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            প্রত্যাগম false;
        }

        leveli->অঙ্ক_syms = (xkb_keysym_গুনতি_t) darray_size(keysymList->syms);
        বিকল্প (leveli->অঙ্ক_syms) {
        পক্ষ 0:
            leveli->s.sym = XKB_চাবি_No_প্রতীক;
            ভঙ্গ;
        পক্ষ 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            প্রতিজ্ঞা(leveli->s.sym != XKB_চাবি_No_প্রতীক);
            ভঙ্গ;
        স্বভাব:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, রিক্ত);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            প্রতি (xkb_keysym_গুনতি_t k = 0; k < leveli->অঙ্ক_syms; k++)
                প্রতিজ্ঞা(leveli->s.syms[k] != XKB_চাবি_No_প্রতীক);
#endif
        }
    }

    প্রত্যাগম true;
}

স্থির bool
Add_Actions_To_চাবি(SymbolsInfo *info, চাবি_Info *keyi, রাশিমালা_Def *arrayNdx,
                রাশিমালা_Def *মান)
{
    xkb_layout_index_t ndx = 0;

    যদি (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        প্রত্যাগম false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    যদি (মান->common.type == STMT_রাশিমালা_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        প্রত্যাগম true;
    }

    যদি (মান->common.type != STMT_রাশিমালা_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 মান->common.type, ndx, চাবি_Info_Text(info, keyi));
        প্রত্যাগম false;
    }

    যদি (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_চাবি_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 চাবি_Info_Text(info, keyi), ndx);
        প্রত্যাগম false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    প্রতি (ParseCommon *p = &মান->common; p; p = p->next)
        nLevels++;
    যদি (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    প্রতি (রাশিমালা_Action_List *actionList = (রাশিমালা_Action_List *) মান;
         actionList;
         actionList = (রাশিমালা_Action_List *) actionList->common.next, level++) {
        সংরচনা xkb_level *leveli = &darray_item(groupi->levels, level);
        প্রতিজ্ঞা(leveli->অঙ্ক_actions == 0);

        unsigned পূর্ণাঙ্ক অঙ্ক_actions = 0;
        প্রতি (রাশিমালা_Def *act = actionList->actions;
             act; act = (রাশিমালা_Def *) act->common.next)
            অঙ্ক_actions++;

        যদি (unlikely(অঙ্ক_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    চাবি_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, অঙ্ক_actions);
            প্রত্যাগম false;
        }

        /* Parse actions and add only defined actions */
        darray(সঙ্ঘ xkb_action) actions = darray_new();
        প্রতি (রাশিমালা_Def *act = actionList->actions;
             act; act = (রাশিমালা_Def *) act->common.next) {
            সঙ্ঘ xkb_action toAct = { 0 };
            ধ্রুব গণনা xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            যদি (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_মান,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        চাবি_Info_Text(info, keyi), ndx + 1, level + 1);
                যদি (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    প্রত্যাগম false;
                } অন্যথা {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            যদি (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } অন্যথা যদি (likely(অঙ্ক_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->অঙ্ক_actions = 1;
                leveli->a.action = toAct;
                গচ্ছ next_level;
            } অন্যথা {
                darray_append(actions, toAct);
            }
        }
        যদি (darray_empty(actions)) {
            leveli->অঙ্ক_actions = 0;
        } অন্যথা যদি (likely(darray_size(actions) > 1)) {
            leveli->অঙ্ক_actions = (xkb_action_গুনতি_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, রিক্ত);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            প্রতি (xkb_action_গুনতি_t k = 0; k < leveli->অঙ্ক_actions; k++)
                প্রতিজ্ঞা(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } অন্যথা {
            /* Unlikely: some actions were dropped and only one remains */
            প্রতিজ্ঞা(অঙ্ক_actions > darray_size(actions));
            leveli->অঙ্ক_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            প্রতিজ্ঞা(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        যদি (leveli->অঙ্ক_actions > 0 || leveli->অঙ্ক_syms > 0)
            nonEmptyLevels = level + 1;
    }

    যদি (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        যদি (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        অন্যথা
            darray_free(groupi->levels);
    }

    প্রত্যাগম true;
}

স্থির ধ্রুব LookupEntry repeatEntries[] = {
    { "true", চাবি_REPEAT_YES },
    { "yes", চাবি_REPEAT_YES },
    { "on", চাবি_REPEAT_YES },
    { "false", চাবি_REPEAT_NO },
    { "no", চাবি_REPEAT_NO },
    { "off", চাবি_REPEAT_NO },
    { "default", চাবি_REPEAT_UNDEFINED },
    { রিক্ত, 0 }
};

/** Handle overlay<N> = <key> entry */
স্থির bool
রাশিমালা_Resolve_Overlay_Entry(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                        ধ্রুব বর্ণ *field, ধ্রুব রাশিমালা_Def *arrayNdx,
                        ধ্রুব রাশিমালা_Def *রাশিমালা, চাবি_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ধ্রুব সংরচনা xkb_চাবি **চাবি_rtrn)
{
    /* Overlay index */
    যদি (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, চাবি_Name_Text(keymap_info->keymap.ctx, keyi->name));
        প্রত্যাগম false;
    }
    static_assert(পরিমাণ(xkb_overlay_index_t) == পরিমাণ(uint8_t), "");
    ধ্রুব size_t prefix = পরিমাণ("overlay") - 1;
    ধ্রুব size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    যদি (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (পূর্ণাঙ্ক)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, চাবি_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        প্রত্যাগম false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_KEYNAME_LITERAL:
        *চাবি_rtrn = Xkb_চাবি_By_Name(&keymap_info->keymap, রাশিমালা->চাবি_name.চাবি_name,
                                 false);
        যদি (!*চাবি_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, রাশিমালা->চাবি_name.চাবি_name),
                    field, চাবি_Name_Text(keymap_info->keymap.ctx, keyi->name));
            প্রত্যাগম false;
        }
        প্রত্যাগম true;
    পক্ষ STMT_রাশিমালা_IDENT: {
        ধ্রুব বর্ণ * ধ্রুব id =
            xkb_atom_text(keymap_info->keymap.ctx, রাশিমালা->ident.ident);
        যদি (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *চাবি_rtrn = রিক্ত;
            প্রত্যাগম true;
        } অন্যথা যদি (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *চাবি_rtrn = রিক্ত;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            প্রত্যাগম true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_মান,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, চাবি_Name_Text(keymap_info->keymap.ctx, keyi->name));
        প্রত্যাগম false;
    }
    স্বভাব:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_মান,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_রাশিমালা_KEYNAME_LITERAL),
                field, চাবি_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;
    }
}

স্থির bool
SetSymbolsField(SymbolsInfo *info, চাবি_Info *keyi, ধ্রুব বর্ণ *field,
                রাশিমালা_Def *arrayNdx, রাশিমালা_Def **মান_ptr)
{
    রাশিমালা_Def * restrict ধ্রুব মান = *মান_ptr;
    যদি (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        যদি (!রাশিমালা_Resolve_String(info->ctx, মান, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            প্রত্যাগম false;
        }

        যদি (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= চাবি_FIELD_DEFAULT_TYPE;
        }
        অন্যথা যদি (রাশিমালা_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, রিক্ত) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }
        অন্যথা {
            ndx--;
            যদি (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                প্রতিজ্ঞা(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    অন্যথা যদি (istreq(field, "symbols")) {
        প্রত্যাগম Add_Symbols_To_চাবি(info, keyi, arrayNdx, মান);
    }
    অন্যথা যদি (istreq(field, "actions")) {
        প্রত্যাগম Add_Actions_To_চাবি(info, keyi, arrayNdx, মান);
    }
    অন্যথা যদি (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, মান, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(মান->common.type),
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= চাবি_FIELD_VMODMAP;
    }
    অন্যথা যদি (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                চাবি_Info_Text(info, keyi));
    }
    অন্যথা যদি (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                চাবি_Info_Text(info, keyi));
    }
    অন্যথা যদি (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                চাবি_Info_Text(info, keyi));
    }
    অন্যথা যদি (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ধ্রুব সংরচনা xkb_চাবি *চাবি = রিক্ত;
        যদি (!রাশিমালা_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *মান_ptr, keyi, &overlay, &চাবি))
            প্রত্যাগম false;
        যদি (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            প্রত্যাগম true;
        } অন্যথা যদি (চাবি && চাবি->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, চাবি_Info_Text(info, keyi));
        } অন্যথা {
            ধ্রুব সংরচনা xkb_চাবি *prev;
            যদি (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                যদি (চাবি != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             চাবি_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? চাবি_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (চাবি
                                 ? চাবি_Name_Text(info->ctx, চাবি->name)
                                 : "none"));
                }
            } অন্যথা যদি (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                যদি (!overlays_ঢোকাও(keyi, overlay, চাবি))
                    প্রত্যাগম false;
                keyi->defined |= চাবি_FIELD_OVERLAY;
            } অন্যথা {
                /* Handle disjoint overlays mode */
                ধ্রুব xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                যদি (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    যদি (চাবি) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_চাবি = চাবি;
                    } অন্যথা {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= চাবি_FIELD_OVERLAY;
                } অন্যথা যদি (keyi->overlays) {
                    যদি (চাবি) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                চাবি_Info_Text(info, keyi), keyi->overlays,
                                চাবি_Name_Text(info->ctx, keyi->overlay_চাবি->name),
                                overlay + 1, চাবি_Name_Text(info->ctx, চাবি->name));
                        প্রত্যাগম !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_মান_MISMATCH);
                    } অন্যথা {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    অন্যথা যদি (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        যদি (!রাশিমালা_Resolve_Enum(info->ctx, মান, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_মান,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }

        keyi->repeat = (গণনা চাবি_repeat) val;
        keyi->defined |= চাবি_FIELD_REPEAT;
    }
    অন্যথা যদি (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        যদি (!রাশিমালা_Resolve_Boolean(info->ctx, মান, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_মান,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= চাবি_FIELD_GROUPINFO;
    }
    অন্যথা যদি (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        যদি (!রাশিমালা_Resolve_Boolean(info->ctx, মান, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_মান,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= চাবি_FIELD_GROUPINFO;
    }
    অন্যথা যদি (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        যদি (রাশিমালা_Resolve_Group(info->keymap_info, মান, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    চাবি_Info_Text(info, keyi));
            প্রত্যাগম false;
        }

        যদি (pending) {
            keyi->out_of_range_pending_group = true;
            ধ্রুব darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (সংরচনা pending_computation) {
                    .রাশিমালা = *মান_ptr,
                    .computed = false,
                    .মান = 0,
                }
            );
            *মান_ptr = রিক্ত;
            static_assert(পরিমাণ(keyi->out_of_range_group_সংখ্যা) ==
                          পরিমাণ(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_সংখ্যা = pending_index;
        } অন্যথা {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_সংখ্যা = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= চাবি_FIELD_GROUPINFO;
    }
    অন্যথা {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        প্রত্যাগম !(info->keymap_info->strict & PARSER_NO_UNKNOWN_চাবি_FIELDS);
    }

    প্রত্যাগম true;
}

স্থির bool
SetGroupName(SymbolsInfo *info, রাশিমালা_Def *arrayNdx, রাশিমালা_Def *মান,
             গণনা merge_mode merge)
{
    যদি (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        প্রত্যাগম false;
    }

    xkb_layout_index_t group = 0;
    যদি (রাশিমালা_Resolve_Group(info->keymap_info, arrayNdx, false, &group, রিক্ত) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        প্রত্যাগম false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    যদি (!রাশিমালা_Resolve_String(info->ctx, মান, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        প্রত্যাগম false;
    }

    xkb_layout_index_t group_to_use;
    যদি (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    অন্যথা যদি (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    অন্যথা {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        প্রত্যাগম false;
    }

    যদি (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        প্রতিজ্ঞা(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } অন্যথা {
        ধ্রুব xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        যদি (old_name != XKB_ATOM_NONE && old_name != name) {
            ধ্রুব bool replace = (merge != MERGE_AUGMENT);
            ধ্রুব xkb_atom_t use    = (replace ? name : old_name);
            ধ্রুব xkb_atom_t ignore = (replace ? old_name : name);
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

    প্রত্যাগম true;
}

স্থির bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ধ্রুব বর্ণ *elem, *field;
    রাশিমালা_Def *arrayNdx;
    bool ret;

    যদি (!রাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        প্রত্যাগম false;

    যদি (elem && istreq(elem, "key")) {
        চাবি_Info temp = {0};
        Init_চাবি_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->মান) &&
              Merge_চাবিগুলি(info, &info->default_চাবি, &temp, true);
        Clear_চাবি_Info(&temp);
    }
    অন্যথা যদি (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->মান, stmt->merge);
    }
    অন্যথা যদি (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    অন্যথা যদি (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    অন্যথা যদি (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    অন্যথা যদি (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    অন্যথা যদি (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->মান, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } অন্যথা {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        প্রত্যাগম !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    প্রত্যাগম ret;
}

স্থির bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, চাবি_Info *keyi)
{
    bool all_valid_entries = true;

    প্রতি (; def; def = (VarDef *) def->common.next) {
        ধ্রুব বর্ণ *field;
        রাশিমালা_Def *arrayNdx;
        bool ok = true;

        যদি (!def->name) {
            যদি (unlikely(!def->মান) || def->মান->common.type != STMT_রাশিমালা_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            অন্যথা
                field = "actions";
            arrayNdx = রিক্ত;
        }
        অন্যথা {
            ধ্রুব বর্ণ *elem;
            ok = রাশিমালা_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            যদি (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        যদি (unlikely(!def->মান)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        যদি (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->মান))
            all_valid_entries = false;
    }

    প্রত্যাগম all_valid_entries;
}

স্থির bool
SetExplicitGroup(SymbolsInfo *info, চাবি_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    যদি (info->explicit_group == XKB_LAYOUT_INVALID)
        প্রত্যাগম true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        যদি (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    যদি (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, চাবি_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    যদি (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    প্রত্যাগম true;
}

স্থির bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    চাবি_Info keyi;

    keyi = info->default_চাবি;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_চাবি.groups);
    প্রতি (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_চাবি.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->চাবি_Name;

    যদি (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_চাবি_Symbols(info, &keyi, true)) {
        প্রত্যাগম true;
    }

    Clear_চাবি_Info(&keyi);
    info->error_গুনতি++;
    প্রত্যাগম false;
}

স্থির bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    সংরচনা xkb_context *ctx = info->ctx;

    যদি (!রাশিমালা_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        প্রত্যাগম false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    প্রতিজ্ঞা(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_রাশিমালা_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    প্রতি (রাশিমালা_Def *চাবি = def->চাবিগুলি; চাবি; চাবি = (রাশিমালা_Def *) চাবি->common.next) {
        যদি (চাবি->common.type == STMT_রাশিমালা_KEYNAME_LITERAL) {
            tmp.have_প্রতীক = false;
            tmp.u.চাবি_Name = চাবি->চাবি_name.চাবি_name;
        }
        অন্যথা যদি (চাবি->common.type == STMT_রাশিমালা_KEYSYM_LITERAL) {
            যদি (চাবি->keysym.keysym == XKB_চাবি_No_প্রতীক) {
                /* Invalid keysym: ignore. Error message already printed */
                অনুবর্ত;
            } অন্যথা {
                tmp.have_প্রতীক = true;
                tmp.u.চাবি_Sym = চাবি->keysym.keysym;
            }
        }
        অন্যথা {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            অনুবর্ত;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    প্রত্যাগম ok;
}

স্থির শূন্য
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    প্রতি (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        বিকল্প (stmt->type) {
        পক্ষ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ভঙ্গ;
        পক্ষ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_UNKNOWN_DECLARATION:
        পক্ষ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ভঙ্গ;
        স্বভাব:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ভঙ্গ;
        }

        যদি (!ok)
            info->error_গুনতি++;

        যদি (info->error_গুনতি > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            ভঙ্গ;
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
স্থির সংরচনা xkb_চাবি *
Find_চাবি_For_প্রতীক(সংরচনা xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    কুরু {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        কুরু {
            got_one_level = false;
            সংরচনা xkb_চাবি *চাবি;
            xkb_চাবিগুলি_foreach(চাবি, keymap) {
                যদি (group < চাবি->অঙ্ক_groups &&
                    level < Xkb_চাবি_অঙ্ক_Levels(চাবি, group)) {
                    got_one_group = got_one_level = true;
                    ধ্রুব xkb_keysym_গুনতি_t অঙ্ক_syms =
                        চাবি->groups[group].levels[level].অঙ্ক_syms;
                    যদি (অঙ্ক_syms > 1) {
                        প্রতি (xkb_keysym_গুনতি_t k = 0; k < অঙ্ক_syms; k++) {
                            যদি (চাবি->groups[group].levels[level].s.syms[k] == sym)
                                প্রত্যাগম চাবি;
                        }
                    } অন্যথা যদি (অঙ্ক_syms &&
                               চাবি->groups[group].levels[level].s.sym == sym) {
                        প্রত্যাগম চাবি;
                    }
                }
            }
            level++;
        } যাবত্ (got_one_level);
        group++;
    } যাবত্ (got_one_group);

    প্রত্যাগম রিক্ত;
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
স্থির xkb_atom_t
FindAutomaticType(সংরচনা xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ধ্রুব xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).অঙ্ক_syms == 0 ? \
        XKB_চাবি_No_প্রতীক : \
     darray_item(groupi->levels, level).অঙ্ক_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    যদি (width == 1 || width <= 0)
        প্রত্যাগম xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    যদি (width == 2) {
        যদি (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            প্রত্যাগম xkb_atom_intern_literal(ctx, "ALPHABETIC");

        যদি (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            প্রত্যাগম xkb_atom_intern_literal(ctx, "KEYPAD");

        প্রত্যাগম xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    যদি (width <= 4) {
        যদি (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_চাবি_No_প্রতীক);

            যদি (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                প্রত্যাগম xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            প্রত্যাগম xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        যদি (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            প্রত্যাগম xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        প্রত্যাগম xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    প্রত্যাগম XKB_ATOM_NONE;

#undef GET_SYM
}

স্থির ধ্রুব সংরচনা xkb_চাবি_type *
FindTypeForGroup(সংরচনা xkb_keymap *keymap, চাবি_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    যদি (type_name == XKB_ATOM_NONE) {
        যদি (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        অন্যথা {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            যদি (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    যদি (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_চাবি_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 চাবি_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        গচ্ছ use_default;
    }

    darray_size_t i;
    প্রতি (i = 0; i < keymap->অঙ্ক_types; i++)
        যদি (keymap->types[i].name == type_name)
            ভঙ্গ;

    যদি (i >= keymap->অঙ্ক_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_চাবি_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 চাবি_Name_Text(keymap->ctx, keyi->name), group + 1);
        গচ্ছ use_default;
    }

    keymap->types[i].required = true;
    প্রত্যাগম &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    প্রত্যাগম &keymap->types[0];
}

স্থির bool
CopySymbolsDefToKeymap(সংরচনা xkb_keymap *keymap, SymbolsInfo *info,
                       চাবি_Info *keyi)
{
    সংরচনা xkb_চাবি *চাবি;
    GroupInfo *groupi;
    ধ্রুব GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    চাবি = Xkb_চাবি_By_Name(keymap, keyi->name, false);
    যদি (!চাবি) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                চাবি_Info_Text(info, keyi));
        প্রত্যাগম false;
    }

    /* Find the range of groups we need. */
    চাবি->অঙ্ক_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ধ্রুব bool has_explicit_type = (keyi->defined & চাবি_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        যদি (darray_size(groupi->levels) > 0 || has_explicit_type)
            চাবি->অঙ্ক_groups = i + 1;
        যদি (has_explicit_type)
            চাবি->explicit |= EXPLICIT_TYPES;
    }

    যদি (চাবি->অঙ্ক_groups <= 0) {
        /* A key with no group may still have other fields defined */
        যদি (keyi->defined)
            গচ্ছ চাবি_fields;
        অন্যথা
            প্রত্যাগম false;
    }

    darray_resize(keyi->groups, চাবি->অঙ্ক_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        যদি (groupi->defined)
            অনুবর্ত;

        CopyGroupInfo(groupi, group0);
    }

    চাবি->groups = calloc(চাবি->অঙ্ক_groups, পরিমাণ(*চাবি->groups));
    প্রতিজ্ঞা(darray_size(keyi->groups) == চাবি->অঙ্ক_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < চাবি->অঙ্ক_groups);
        #endif
        bool explicit_type = false;
        ধ্রুব সংরচনা xkb_চাবি_type * ধ্রুব type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        যদি (type->অঙ্ক_levels < darray_size(groupi->levels)) {
            সংরচনা xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->অঙ্ক_levels,
                    চাবি_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->অঙ্ক_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->অঙ্ক_levels);

        চাবি->groups[i].explicit_type = explicit_type;
        চাবি->groups[i].type = type;
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
        সংরচনা xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            বিকল্প (leveli->অঙ্ক_syms) {
            পক্ষ 0:
                leveli->upper = XKB_চাবি_No_প্রতীক;
                ভঙ্গ;
            পক্ষ 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                ভঙ্গ;
            স্বভাব:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                প্রতি (xkb_keysym_গুনতি_t k = 0; k < leveli->অঙ্ক_syms; k++) {
                    ধ্রুব xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    যদি (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        ভঙ্গ;
                    }
                }
                যদি (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->অঙ্ক_syms *
                                             পরিমাণ(*leveli->s.syms));
                    যদি (!leveli->s.syms)
                        প্রত্যাগম false; /* FIXME: better handling? */
                    প্রতি (xkb_keysym_গুনতি_t k = 0; k < leveli->অঙ্ক_syms; k++) {
                        leveli->s.syms[leveli->অঙ্ক_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &চাবি->groups[i].levels, রিক্ত);
        যদি (চাবি->groups[i].type->অঙ্ক_levels > 1 ||
            চাবি->groups[i].levels[0].অঙ্ক_syms > 0) {
            চাবি->groups[i].explicit_symbols = true;
            চাবি->explicit |= EXPLICIT_SYMBOLS;
        }
        যদি (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            চাবি->groups[i].explicit_actions = true;
            চাবি->explicit |= EXPLICIT_INTERP;
        }
        যদি (চাবি->groups[i].explicit_type)
            চাবি->explicit |= EXPLICIT_TYPES;
    }

    চাবি->out_of_range_pending_group = keyi->out_of_range_pending_group;
    চাবি->out_of_range_group_সংখ্যা = keyi->out_of_range_group_সংখ্যা;
    চাবি->out_of_range_group_policy = keyi->out_of_range_group_policy;

চাবি_fields:
    যদি (keyi->defined & চাবি_FIELD_VMODMAP) {
        চাবি->vmodmap = keyi->vmodmap;
        চাবি->explicit |= EXPLICIT_VMODMAP;
    }

    যদি (keyi->repeat != চাবি_REPEAT_UNDEFINED) {
        চাবি->repeats = (keyi->repeat == চাবি_REPEAT_YES);
        চাবি->explicit |= EXPLICIT_REPEAT;
    }

    যদি ((keyi->defined & চাবি_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        চাবি->overlays = keyi->overlays;
        যদি (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = চাবি->overlays;
            ধ্রুব সংরচনা xkb_চাবি **overlays_চাবিগুলি = keyi->overlays_চাবিগুলি;
            যাবত্ (remaining) {
                /* isolate lowest set bit */
                ধ্রুব xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                যদি (*overlays_চাবিগুলি) {
                    overlays_চাবিগুলি++;
                } অন্যথা {
                    /* drop current null value (overlayN=none) */
                    চাবি->overlays &= ~lsb;
                }
            }

            যদি (চাবি->overlays) {
                /* Steal */
                চাবি->overlays_চাবিগুলি = keyi->overlays_চাবিগুলি;
                keyi->overlays_চাবিগুলি = রিক্ত;
                keyi->overlays_alloc = 0;

                চাবি->overlays_inline = false;
                চাবি->explicit |= EXPLICIT_OVERLAY;
            }
        } অন্যথা {
            চাবি->overlay_চাবি = keyi->overlay_চাবি;
            চাবি->overlays_inline = true;
            চাবি->explicit |= EXPLICIT_OVERLAY;
        }
    }

    প্রত্যাগম true;
}

স্থির bool
CopyModMapDefToKeymap(সংরচনা xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    সংরচনা xkb_চাবি *চাবি;

    যদি (!entry->have_প্রতীক) {
        চাবি = Xkb_চাবি_By_Name(keymap, entry->u.চাবি_Name, true);
        যদি (!চাবি) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    চাবি_Name_Text(info->ctx, entry->u.চাবি_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            প্রত্যাগম false;
        }
    }
    অন্যথা {
        চাবি = Find_চাবি_For_প্রতীক(keymap, entry->u.চাবি_Sym);
        যদি (!চাবি) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_প্রতীক,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.চাবি_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            প্রত্যাগম false;
        }
    }

    /* Skip modMap None */
    যদি (entry->mods) {
        চাবি->modmap |= entry->mods;
        entry->চাবি_Code = চাবি->keycode;
    }

    প্রত্যাগম true;
}

স্থির bool
CopySymbolsToKeymap(সংরচনা xkb_keymap *keymap, SymbolsInfo *info)
{
    চাবি_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->অঙ্ক_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, রিক্ত);

    darray_foreach(keyi, info->চাবিগুলি)
        যদি (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_গুনতি++;

    যদি (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        সংরচনা xkb_চাবি *চাবি;

        xkb_চাবিগুলি_foreach(চাবি, keymap) {
            যদি (চাবি->name == XKB_ATOM_NONE)
                অনুবর্ত;

            যদি (চাবি->অঙ্ক_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         চাবি_Name_Text(info->ctx, চাবি->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        যদি (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_গুনতি++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->অঙ্ক_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    প্রত্যাগম true;
}

bool
CompileSymbols(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    যদি (file !=রিক্ত)
        HandleSymbolsFile(&info, file);

    যদি (info.error_গুনতি != 0)
        গচ্ছ err_info;

    যদি (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        গচ্ছ err_info;

    ClearSymbolsInfo(&info);
    প্রত্যাগম true;

err_info:
    ClearSymbolsInfo(&info);
    প্রত্যাগম false;
}
