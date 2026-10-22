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

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"

#अन्तर्भाव "action.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "vmod.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/ast.h"

गणना कुञ्चिका_repeat {
    कुञ्चिका_REPEAT_UNDEFINED = 0,
    कुञ्चिका_REPEAT_YES,
    कुञ्चिका_REPEAT_NO,
    कुञ्चिका_REPEAT_अङ्कः_ENTRIES,
};
गणना { कुञ्चिका_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(कुञ्चिका_REPEAT_अङ्कः_ENTRIES - 1 <=
              (1 << (कुञ्चिका_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

गणना group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

गणना कुञ्चिका_field {
    कुञ्चिका_FIELD_NONE = 0,
    कुञ्चिका_FIELD_REPEAT = (1 << 0),
    कुञ्चिका_FIELD_DEFAULT_TYPE = (1 << 1),
    कुञ्चिका_FIELD_GROUPINFO = (1 << 2),
    कुञ्चिका_FIELD_VMODMAP = (1 << 3),
    कुञ्चिका_FIELD_OVERLAY = (1 << 4),
    कुञ्चिका_FIELD_ALL = कुञ्चिका_FIELD_REPEAT | कुञ्चिका_FIELD_DEFAULT_TYPE
                  | कुञ्चिका_FIELD_GROUPINFO | कुञ्चिका_FIELD_VMODMAP | कुञ्चिका_FIELD_OVERLAY,
};
गणना { कुञ्चिका_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(कुञ्चिका_FIELD_ALL <= (1 << (कुञ्चिका_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

प्रकारनिर्देश संरचना {
    darray(संरचना xkb_level) levels;
    गणना group_field defined;
    xkb_atom_t type;
} GroupInfo;

गणना {
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
              (unsigned)कुञ्चिका_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)कुञ्चिका_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

प्रकारनिर्देश संरचना {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_सङ्ख्या;
    darray(GroupInfo) groups;
    गणना xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    गणना कुञ्चिका_field defined:KEYINFO_DEFINED_BIT_FIELD;
    गणना merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    गणना कुञ्चिका_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    सङ्घ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ध्रुव संरचना xkb_कुञ्चिका *overlay_कुञ्चिका;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः;
    };
} कुञ्चिका_Info;

स्थिर शून्य
StealLevelInfo(संरचना xkb_level *into, संरचना xkb_level *from)
{
    clear_level(into);
    जइ (from->अङ्कः_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अण्णहा {
        into->s.sym = from->s.sym;
    }
    into->अङ्कः_syms = from->अङ्कः_syms;
    from->अङ्कः_syms = 0;

    जइ (from->अङ्कः_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अण्णहा {
        into->a.action = from->a.action;
    }
    into->अङ्कः_actions = from->अङ्कः_actions;
    from->अङ्कः_actions = 0;
}

स्थिर शून्य
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, परिमाण(*groupi));
}

स्थिर शून्य
ClearGroupInfo(GroupInfo *groupi)
{
    संरचना xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

स्थिर शून्य
CopyGroupInfo(GroupInfo *to, ध्रुव GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    प्रति (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        जइ (darray_item(from->levels, j).अङ्कः_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).अङ्कः_syms,
                       परिमाण(xkb_keysym_t));
        }
        जइ (darray_item(from->levels, j).अङ्कः_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).अङ्कः_actions,
                       परिमाण(सङ्घ xkb_action));
        }
    }
}

स्थिर शून्य
Init_कुञ्चिका_Info(संरचना xkb_context *ctx, कुञ्चिका_Info *keyi)
{
    memset(keyi, 0, परिमाण(*keyi));
    static_assert(!DEFAULT_कुञ्चिका_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_कुञ्चिका_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_कुञ्चिका = रिक्त;
}

स्थिर शून्य
Clear_कुञ्चिका_Info(कुञ्चिका_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    जइ (keyi->overlays_alloc)
        मुक्त(keyi->overlays_कुञ्चिकाः);
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    वर्ण *name;         /* e.g. pc+us+inet(evdev) */
    पूर्णाङ्क error_गणनम्;
    unsigned पूर्णाङ्क include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(कुञ्चिका_Info) कुञ्चिकाः;
    कुञ्चिका_Info default_कुञ्चिका;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    संरचना xkb_mod_set mods;

    संरचना xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} SymbolsInfo;

स्थिर शून्य
InitSymbolsInfo(SymbolsInfo *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                unsigned पूर्णाङ्क include_depth, ध्रुव संरचना xkb_mod_set *mods)
{
    memset(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_कुञ्चिका_Info(keymap_info->keymap.ctx, &info->default_कुञ्चिका);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

स्थिर शून्य
ClearSymbolsInfo(SymbolsInfo *info)
{
    कुञ्चिका_Info *keyi;
    मुक्त(info->name);
    darray_foreach(keyi, info->कुञ्चिकाः)
        Clear_कुञ्चिका_Info(keyi);
    darray_free(info->कुञ्चिकाः);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_कुञ्चिका_Info(&info->default_कुञ्चिका);
}

स्थिर ध्रुव वर्ण *
कुञ्चिका_Info_Text(SymbolsInfo *info, कुञ्चिका_Info *keyi)
{
    प्रत्यागम कुञ्चिका_Name_Text(info->ctx, keyi->name);
}

स्थिर bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t कुञ्चिका_name)
{
    /* First find the type of the merged group. */
    जइ (into->type != from->type) {
        जइ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अण्णहा जइ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अण्णहा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            जइ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    जइ (darray_empty(from->levels)) {
        InitGroupInfo(from);
        प्रत्यागम true;
    }

    जइ (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        प्रत्यागम true;
    }

    /* Merge the actions and syms. */
    ध्रुव darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_गणनम् = 0;
    darray_size_t from_Actions_गणनम् = 0;
    प्रति (darray_size_t i = 0; i < levels_in_both; i++) {
        संरचना xkb_level* ध्रुव intoLevel = &darray_item(into->levels, i);
        संरचना xkb_level* ध्रुव fromLevel = &darray_item(from->levels, i);

        ध्रुव bool fromHasNoKeysym = fromLevel->अङ्कः_syms == 0;
        ध्रुव bool fromHasNoAction = fromLevel->अङ्कः_actions == 0;
        जइ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            अनुवर्त;
        }

        ध्रुव bool intoHasNoKeysym = intoLevel->अङ्कः_syms == 0;
        ध्रुव bool intoHasNoAction = intoLevel->अङ्कः_actions == 0;
        जइ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_गणनम्++;
            from_Actions_गणनम्++;
            अनुवर्त;
        }

        /* Possible level conflict */
        प्रतिज्ञा(intoLevel->अङ्कः_syms > 0 || intoLevel->अङ्कः_actions > 0);
        प्रतिज्ञा(fromLevel->अङ्कः_syms > 0 || fromLevel->अङ्कः_actions > 0);

        /* Handle keysyms */
        जइ (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            जइ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_चिह्नम्,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            जइ (fromHasNoKeysym) {
                /* No keysym to copy */
            } अण्णहा जइ (clobber) {
                /* Override: copy any defined keysym from `from` */
                जइ (unlikely(fromLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    जइ (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                    fromLevel->अङ्कः_syms = 0;
                    from_Keysyms_गणनम्++;
                } अण्णहा जइ (fromLevel->s.sym != XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single defined keysym */
                    जइ (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अङ्कः_syms = 1;
                    from_Keysyms_गणनम्++;
                }
            } अण्णहा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                जइ (unlikely(intoLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अण्णहा जइ (intoLevel->s.sym == XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single undefined keysym */
                    जइ (unlikely(fromLevel->अङ्कः_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                        fromLevel->अङ्कः_syms = 0;
                    } अण्णहा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अङ्कः_syms = 1;
                    }
                    from_Keysyms_गणनम्++;
                }
            }
        }

        /* Handle actions */
        जइ (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            जइ (report && !(intoHasNoAction || fromHasNoAction)) {
                जइ (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अण्णहा {
                    सङ्घ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            जइ (fromHasNoAction) {
                /* No action to copy */
            } अण्णहा जइ (clobber) {
                /* Override: copy any defined action from `from` */
                जइ (unlikely(fromLevel->अङ्कः_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    जइ (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                    fromLevel->अङ्कः_actions = 0;
                    from_Actions_गणनम्++;
                } अण्णहा जइ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    जइ (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अङ्कः_actions = 1;
                    from_Actions_गणनम्++;
                }
            } अण्णहा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                जइ (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अण्णहा जइ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    जइ (unlikely(fromLevel->अङ्कः_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                        fromLevel->अङ्कः_actions = 0;
                    } अण्णहा {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->अङ्कः_actions = 1;
                    }
                    from_Actions_गणनम्++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    संरचना xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->अङ्कः_syms = 0;
        level->अङ्कः_actions = 0;
        from_Keysyms_गणनम्++;
        from_Actions_गणनम्++;
    }
    जइ (from_Keysyms_गणनम्) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        जइ (from_Keysyms_गणनम् == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    जइ (from_Actions_गणनम्) {
        /* Reset defined actions field if we used no action from `into` */
        जइ (from_Actions_गणनम् == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    प्रत्यागम true;
}

स्थिर bool
Use_New_कुञ्चिका_Field(गणना कुञ्चिका_field field, गणना कुञ्चिका_field old, गणना कुञ्चिका_field new,
               bool clobber, bool report, गणना कुञ्चिका_field *collide)
{
    जइ (!(old & field))
        प्रत्यागम (new & field);

    जइ (new & field) {
        जइ (report)
            *collide |= field;

        प्रत्यागम clobber;
    }

    प्रत्यागम false;
}

स्थिर bool
overlays_get(ध्रुव कुञ्चिका_Info *info, xkb_overlay_index_t bit,
             ध्रुव संरचना xkb_कुञ्चिका **कुञ्चिका_out)
{
    जइ (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * CHAR_BIT))
        प्रत्यागम false;
    ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    जइ (!(info->overlays & mask)) प्रत्यागम false;
    जइ (कुञ्चिका_out) {
        जइ (!info->overlays_alloc) {
            /* Stored inline */
            *कुञ्चिका_out = info->overlay_कुञ्चिका;
        } अण्णहा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ध्रुव xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(नीचः);
            *कुञ्चिका_out = info->overlays_कुञ्चिकाः[index];
        }
    }
    प्रत्यागम true;
}

स्थिर bool
overlays_निवेशय(कुञ्चिका_Info *keyi, xkb_overlay_index_t bit,
                ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    जइ (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * CHAR_BIT))
        प्रत्यागम false;

    ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    जइ ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        जइ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_कुञ्चिका = कुञ्चिका;
            जइ (!कुञ्चिका)
                keyi->overlays_clear = true;
        } अण्णहा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ध्रुव xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(नीचः);
            keyi->overlays_कुञ्चिकाः[index] = कुञ्चिका;
        }
        प्रत्यागम true;
    }

    /* Insert new element */
    जइ (!keyi->overlays || (keyi->overlays_clear && !कुञ्चिका)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_कुञ्चिका = कुञ्चिका;
        keyi->overlays |= mask;
        keyi->overlays_clear = !कुञ्चिका;
    } अण्णहा जइ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ध्रुव xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ध्रुव xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = calloc(alloc, परिमाण(*tmp));
        जइ (!tmp) प्रत्यागम false;

        /* Copy previous entries to the buffer */
        जइ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ध्रुव xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(नीचः);
            tmp[idx] = keyi->overlay_कुञ्चिका;
        } अण्णहा {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ध्रुव xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(नीचः);
        tmp[idx] = कुञ्चिका;

        keyi->overlays_कुञ्चिकाः = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } अण्णहा {
        /* Allocated items: grow heap and shift if relevant */
        ध्रुव xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ध्रुव xkb_overlay_index_t गणनम् =
            (xkb_overlay_index_t)popcount32(overlays);
        जइ (गणनम् > keyi->overlays_alloc) {
            ध्रुव xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(गणनम्);
            प्रतिज्ञा(alloc >= गणनम्);
            ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = realloc(
                keyi->overlays_कुञ्चिकाः,
                (size_t)alloc * परिमाण(*keyi->overlays_कुञ्चिकाः)
            );
            जइ (!tmp) प्रत्यागम false;
            keyi->overlays_कुञ्चिकाः = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ध्रुव xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(नीचः);
        जइ (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        जइ (index < गणनम् - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            प्रतिज्ञा(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_कुञ्चिकाः + index + 1, keyi->overlays_कुञ्चिकाः + index,
                    (size_t)(गणनम् - 1u - index) * परिमाण(ध्रुव संरचना xkb_कुञ्चिका *));
        }
        keyi->overlays_कुञ्चिकाः[index] = कुञ्चिका;
        keyi->overlays = overlays;
    }

    प्रत्यागम true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
स्थिर bool
merge_overlays(SymbolsInfo *info, कुञ्चिका_Info *into, कुञ्चिका_Info *from,
               bool clobber, bool report, गणना कुञ्चिका_field *collide)
{
    जइ (!(from->defined & कुञ्चिका_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अण्णहा जइ (!(into->defined & कुञ्चिका_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
        from->overlays_कुञ्चिकाः = रिक्त;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= कुञ्चिका_FIELD_OVERLAY;
    } अण्णहा जइ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अण्णहा {
        जइ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ध्रुव xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ध्रुव xkb_overlay_index_t गणनम् =
                (xkb_overlay_index_t)popcount32(result_mask);

            जइ (गणनम् == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            कुञ्चिका_Info *dest = into;
            कुञ्चिका_Info *src = from;
            जइ (into->overlays_alloc >= गणनम् &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अण्णहा जइ (from->overlays_alloc >= गणनम्) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अण्णहा जइ (गणनम् > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                जइ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                जइ (!dest->overlays_alloc) {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp = calloc(गणनम्, परिमाण(*tmp));
                    जइ (!tmp)
                        प्रत्यागम false;
                    जइ (dest->overlay_कुञ्चिका)
                        tmp[0] = dest->overlay_कुञ्चिका;
                    dest->overlays_कुञ्चिकाः = tmp;
                    dest->overlays_alloc = गणनम्;
                } अण्णहा {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp =
                        realloc(dest->overlays_कुञ्चिकाः,
                                गणनम् * परिमाण(*dest->overlays_कुञ्चिकाः));
                    जइ (!tmp)
                        प्रत्यागम false;
                    dest->overlays_कुञ्चिकाः = tmp;
                    dest->overlays_alloc = गणनम्;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ध्रुव संरचना xkb_कुञ्चिका **src_कुञ्चिकाः = (src->overlays_clear)
                ? रिक्त
                : (src->overlays_alloc)
                    ? src->overlays_कुञ्चिकाः
                    : &src->overlay_कुञ्चिका;
            जाव (remaining) {
                /* isolate lowest set bit */
                ध्रुव xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                ध्रुव xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                जइ (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ध्रुव संरचना xkb_कुञ्चिका *src_कुञ्चिका = (src_कुञ्चिकाः)
                    ? *(src_कुञ्चिकाः++)
                    : रिक्त;

                /* check dest */
                ध्रुव संरचना xkb_कुञ्चिका *dest_कुञ्चिका = रिक्त;
                ध्रुव bool conflict = overlays_get(dest, bit, &dest_कुञ्चिका);
                जइ (conflict) {
                    जइ (dest_कुञ्चिका == src_कुञ्चिका)
                       अनुवर्त;
                    अण्णहा जइ (report)
                        *collide |= कुञ्चिका_FIELD_OVERLAY;
                }

                जइ ((!conflict || clobber) && !overlays_निवेशय(dest, bit, src_कुञ्चिका))
                    प्रत्यागम false;
            }

            जइ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                जइ (into->overlays_alloc)
                    मुक्त(into->overlays_कुञ्चिकाः);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                जइ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
                    from->overlays_कुञ्चिकाः = रिक्त;
                    from->overlays_alloc = 0;
                } अण्णहा {
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                }
            }
        } अण्णहा {
            /* Disjoint overlays */

            जइ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_कुञ्चिका == from->overlay_कुञ्चिका) {
                /* Indentical: do nothing */
                प्रत्यागम true;
            }
            जइ (!(into->overlays & from->overlays)) {
                जइ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                    प्रत्यागम true;
                } अण्णहा जइ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    प्रत्यागम true;
                }
            }
            /* Conflict */
            जइ (report)
                *collide |= कुञ्चिका_FIELD_OVERLAY;
            जइ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
            }
        }
    }
    प्रत्यागम true;
}

स्थिर bool
Merge_कुञ्चिकाः(SymbolsInfo *info, कुञ्चिका_Info *into, कुञ्चिका_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    गणना कुञ्चिका_field collide = 0;
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool clobber = (from->merge != MERGE_AUGMENT);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;

    जइ (from->merge == MERGE_REPLACE) {
        Clear_कुञ्चिका_Info(into);
        *into = *from;
        Init_कुञ्चिका_Info(info->ctx, from);
        प्रत्यागम true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    प्रति (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    प्रति (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    जइ (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= कुञ्चिका_FIELD_VMODMAP;
    }
    जइ (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= कुञ्चिका_FIELD_REPEAT;
    }
    जइ (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= कुञ्चिका_FIELD_DEFAULT_TYPE;
    }
    जइ (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_सङ्ख्या = from->out_of_range_group_सङ्ख्या;
        into->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }

    जइ (!merge_overlays(info, into, from, clobber, report, &collide))
        प्रत्यागम false;

    जइ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 कुञ्चिका_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_कुञ्चिका_Info(from);
    Init_कुञ्चिका_Info(info->ctx, from);
    प्रत्यागम true;
}

स्थिर xkb_atom_t
Xkb_Resolve_कुञ्चिका_Alias(ध्रुव संरचना xkb_keymap *keymap, xkb_atom_t name)
{
    जइ (name < keymap->अङ्कः_कुञ्चिका_names) {
        ध्रुव KeycodeMatch match = keymap->कुञ्चिका_names[name];
        जइ (match.found && match.is_alias) {
            प्रत्यागम match.alias.real;
        }
    }
    प्रत्यागम name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
स्थिर bool
Add_कुञ्चिका_Symbols(SymbolsInfo *info, कुञ्चिका_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_कुञ्चिका_Alias(&info->keymap_info->keymap, keyi->name);

    कुञ्चिका_Info *iter;
    darray_foreach(iter, info->कुञ्चिकाः)
        जइ (iter->name == keyi->name)
            प्रत्यागम Merge_कुञ्चिकाः(info, iter, keyi, same_file);

    darray_append(info->कुञ्चिकाः, *keyi);
    Init_कुञ्चिका_Info(info->ctx, keyi);
    प्रत्यागम true;
}

स्थिर bool
AddModMapEntry(SymbolsInfo *info, ध्रुव ModMapEntry *new)
{
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        जइ ((new->have_चिह्नम् != old->have_चिह्नम्) ||
            (new->have_चिह्नम् && new->u.कुञ्चिका_Sym != old->u.कुञ्चिका_Sym) ||
            (!new->have_चिह्नम् && new->u.कुञ्चिका_Name != old->u.कुञ्चिका_Name))
            अनुवर्त;

        जइ (new->mods == old->mods)
            प्रत्यागम true;

        ध्रुव xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ध्रुव xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        जइ (new->have_चिह्नम्) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.कुञ्चिका_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } अण्णहा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    कुञ्चिका_Name_Text(info->ctx, new->u.कुञ्चिका_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        प्रत्यागम true;
    }

    darray_append(info->modmaps, *new);

    प्रत्यागम true;
}

/***====================================================================***/

स्थिर शून्य
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     गणना merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    जइ (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        प्रत्यागम;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    जइ (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    प्रति (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        जइ (!darray_item(from->group_names, i))
            अनुवर्त;

        जइ (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            अनुवर्त;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    जइ (darray_empty(into->कुञ्चिकाः)) {
        into->कुञ्चिकाः = from->कुञ्चिकाः;
        darray_init(from->कुञ्चिकाः);
    }
    अण्णहा {
        कुञ्चिका_Info *keyi;
        darray_foreach(keyi, from->कुञ्चिकाः) {
            keyi->merge = merge;
            जइ (!Add_कुञ्चिका_Symbols(into, keyi, false))
                into->error_गणनम्++;
        }
    }

    जइ (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अण्णहा {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            जइ (!AddModMapEntry(into, mm))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    जइ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गणनम् += 10;
        प्रत्यागम false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, परिमाण(path));
        जइ (!file) {
            info->error_गणनम् += 10;
            ClearSymbolsInfo(&included);
            प्रत्यागम false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        जइ (stmt->modifier) {
            ध्रुव पूर्णाङ्क गणनम् = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            जइ (गणनम् <= 0 || *(stmt->modifier + गणनम्) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } अण्णहा {
                next_incl.explicit_group--;
            }
        }
        अण्णहा जइ (info->keymap_info->keymap.अङ्कः_groups != 0 &&
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
        अण्णहा {
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

    प्रत्यागम (info->error_गणनम् == 0);
}

स्थिर bool
GetGroupIndex(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
              गणना group_field field, xkb_layout_index_t *ndx_rtrn)
{
    प्रतिज्ञा (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ध्रुव वर्ण *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    जइ (arrayNdx == रिक्त) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            जइ (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                प्रत्यागम true;
            }
        }

        जइ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, कुञ्चिका_Info_Text(info, keyi), info->max_groups, name);
            प्रत्यागम false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        प्रत्यागम true;
    }

    जइ (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    (*ndx_rtrn)--;
    जइ (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    प्रत्यागम true;
}

स्थिर bool
Add_Symbols_To_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_index_t ndx = 0;

    जइ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        प्रत्यागम false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    जइ (मानम्->common.type == STMT_व्यञ्जकम्_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        प्रत्यागम true;
    }

    जइ (मानम्->common.type != STMT_व्यञ्जकम्_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(मानम्->common.type), ndx + 1,
                कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    जइ (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_कुञ्चिका_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                कुञ्चिका_Info_Text(info, keyi), ndx + 1);
        प्रत्यागम false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    प्रति (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        जइ (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    जइ (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    जइ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    प्रति (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList && level < nLevels;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्कः_syms == 0);

        जइ (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            प्रत्यागम false;
        }

        leveli->अङ्कः_syms = (xkb_keysym_गणनम्_t) darray_size(keysymList->syms);
        विकल्प (leveli->अङ्कः_syms) {
        पक्ष 0:
            leveli->s.sym = XKB_कुञ्चिका_No_चिह्नम्;
            भङ्ग;
        पक्ष 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            प्रतिज्ञा(leveli->s.sym != XKB_कुञ्चिका_No_चिह्नम्);
            भङ्ग;
        स्वभाव:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, रिक्त);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++)
                प्रतिज्ञा(leveli->s.syms[k] != XKB_कुञ्चिका_No_चिह्नम्);
#endif
        }
    }

    प्रत्यागम true;
}

स्थिर bool
Add_Actions_To_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_index_t ndx = 0;

    जइ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        प्रत्यागम false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    जइ (मानम्->common.type == STMT_व्यञ्जकम्_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        प्रत्यागम true;
    }

    जइ (मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मानम्->common.type, ndx, कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    जइ (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 कुञ्चिका_Info_Text(info, keyi), ndx);
        प्रत्यागम false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    प्रति (ParseCommon *p = &मानम्->common; p; p = p->next)
        nLevels++;
    जइ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    प्रति (व्यञ्जकम्_Action_List *actionList = (व्यञ्जकम्_Action_List *) मानम्;
         actionList;
         actionList = (व्यञ्जकम्_Action_List *) actionList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्कः_actions == 0);

        unsigned पूर्णाङ्क अङ्कः_actions = 0;
        प्रति (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.next)
            अङ्कः_actions++;

        जइ (unlikely(अङ्कः_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, अङ्कः_actions);
            प्रत्यागम false;
        }

        /* Parse actions and add only defined actions */
        darray(सङ्घ xkb_action) actions = darray_new();
        प्रति (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.next) {
            सङ्घ xkb_action toAct = { 0 };
            ध्रुव गणना xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            जइ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1);
                जइ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    प्रत्यागम false;
                } अण्णहा {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            जइ (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अण्णहा जइ (likely(अङ्कः_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अङ्कः_actions = 1;
                leveli->a.action = toAct;
                गच्छ next_level;
            } अण्णहा {
                darray_append(actions, toAct);
            }
        }
        जइ (darray_empty(actions)) {
            leveli->अङ्कः_actions = 0;
        } अण्णहा जइ (likely(darray_size(actions) > 1)) {
            leveli->अङ्कः_actions = (xkb_action_गणनम्_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, रिक्त);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            प्रति (xkb_action_गणनम्_t k = 0; k < leveli->अङ्कः_actions; k++)
                प्रतिज्ञा(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } अण्णहा {
            /* Unlikely: some actions were dropped and only one remains */
            प्रतिज्ञा(अङ्कः_actions > darray_size(actions));
            leveli->अङ्कः_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            प्रतिज्ञा(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        जइ (leveli->अङ्कः_actions > 0 || leveli->अङ्कः_syms > 0)
            nonEmptyLevels = level + 1;
    }

    जइ (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        जइ (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        अण्णहा
            darray_free(groupi->levels);
    }

    प्रत्यागम true;
}

स्थिर ध्रुव LookupEntry repeatEntries[] = {
    { "true", कुञ्चिका_REPEAT_YES },
    { "yes", कुञ्चिका_REPEAT_YES },
    { "on", कुञ्चिका_REPEAT_YES },
    { "false", कुञ्चिका_REPEAT_NO },
    { "no", कुञ्चिका_REPEAT_NO },
    { "off", कुञ्चिका_REPEAT_NO },
    { "default", कुञ्चिका_REPEAT_UNDEFINED },
    { रिक्त, 0 }
};

/** Handle overlay<N> = <key> entry */
स्थिर bool
व्यञ्जकम्_Resolve_Overlay_Entry(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव वर्ण *field, ध्रुव व्यञ्जकम्_Def *arrayNdx,
                        ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, कुञ्चिका_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ध्रुव संरचना xkb_कुञ्चिका **कुञ्चिका_rtrn)
{
    /* Overlay index */
    जइ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        प्रत्यागम false;
    }
    static_assert(परिमाण(xkb_overlay_index_t) == परिमाण(uint8_t), "");
    ध्रुव size_t prefix = परिमाण("overlay") - 1;
    ध्रुव size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    जइ (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (पूर्णाङ्क)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        प्रत्यागम false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
        *कुञ्चिका_rtrn = Xkb_कुञ्चिका_By_Name(&keymap_info->keymap, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name,
                                 false);
        जइ (!*कुञ्चिका_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name),
                    field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
            प्रत्यागम false;
        }
        प्रत्यागम true;
    पक्ष STMT_व्यञ्जकम्_IDENT: {
        ध्रुव वर्ण * ध्रुव id =
            xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->ident.ident);
        जइ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *कुञ्चिका_rtrn = रिक्त;
            प्रत्यागम true;
        } अण्णहा जइ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *कुञ्चिका_rtrn = रिक्त;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            प्रत्यागम true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        प्रत्यागम false;
    }
    स्वभाव:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_व्यञ्जकम्_KEYNAME_LITERAL),
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;
    }
}

स्थिर bool
SetSymbolsField(SymbolsInfo *info, कुञ्चिका_Info *keyi, ध्रुव वर्ण *field,
                व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def **मानम्_ptr)
{
    व्यञ्जकम्_Def * restrict ध्रुव मानम् = *मानम्_ptr;
    जइ (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        जइ (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            प्रत्यागम false;
        }

        जइ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= कुञ्चिका_FIELD_DEFAULT_TYPE;
        }
        अण्णहा जइ (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, रिक्त) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }
        अण्णहा {
            ndx--;
            जइ (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                प्रतिज्ञा(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    अण्णहा जइ (istreq(field, "symbols")) {
        प्रत्यागम Add_Symbols_To_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अण्णहा जइ (istreq(field, "actions")) {
        प्रत्यागम Add_Actions_To_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अण्णहा जइ (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        जइ (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, मानम्, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(मानम्->common.type),
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= कुञ्चिका_FIELD_VMODMAP;
    }
    अण्णहा जइ (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अण्णहा जइ (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अण्णहा जइ (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अण्णहा जइ (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = रिक्त;
        जइ (!व्यञ्जकम्_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *मानम्_ptr, keyi, &overlay, &कुञ्चिका))
            प्रत्यागम false;
        जइ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            प्रत्यागम true;
        } अण्णहा जइ (कुञ्चिका && कुञ्चिका->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, कुञ्चिका_Info_Text(info, keyi));
        } अण्णहा {
            ध्रुव संरचना xkb_कुञ्चिका *prev;
            जइ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                जइ (कुञ्चिका != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             कुञ्चिका_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? कुञ्चिका_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (कुञ्चिका
                                 ? कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name)
                                 : "none"));
                }
            } अण्णहा जइ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                जइ (!overlays_निवेशय(keyi, overlay, कुञ्चिका))
                    प्रत्यागम false;
                keyi->defined |= कुञ्चिका_FIELD_OVERLAY;
            } अण्णहा {
                /* Handle disjoint overlays mode */
                ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                जइ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    जइ (कुञ्चिका) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_कुञ्चिका = कुञ्चिका;
                    } अण्णहा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= कुञ्चिका_FIELD_OVERLAY;
                } अण्णहा जइ (keyi->overlays) {
                    जइ (कुञ्चिका) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                कुञ्चिका_Info_Text(info, keyi), keyi->overlays,
                                कुञ्चिका_Name_Text(info->ctx, keyi->overlay_कुञ्चिका->name),
                                overlay + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
                        प्रत्यागम !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_मानम्_MISMATCH);
                    } अण्णहा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अण्णहा जइ (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        जइ (!व्यञ्जकम्_Resolve_Enum(info->ctx, मानम्, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->repeat = (गणना कुञ्चिका_repeat) val;
        keyi->defined |= कुञ्चिका_FIELD_REPEAT;
    }
    अण्णहा जइ (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        जइ (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अण्णहा जइ (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        जइ (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अण्णहा जइ (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        जइ (व्यञ्जकम्_Resolve_Group(info->keymap_info, मानम्, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        जइ (pending) {
            keyi->out_of_range_pending_group = true;
            ध्रुव darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (संरचना pending_computation) {
                    .व्यञ्जकम् = *मानम्_ptr,
                    .computed = false,
                    .मानम् = 0,
                }
            );
            *मानम्_ptr = रिक्त;
            static_assert(परिमाण(keyi->out_of_range_group_सङ्ख्या) ==
                          परिमाण(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_सङ्ख्या = pending_index;
        } अण्णहा {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_सङ्ख्या = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अण्णहा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_UNKNOWN_कुञ्चिका_FIELDS);
    }

    प्रत्यागम true;
}

स्थिर bool
SetGroupName(SymbolsInfo *info, व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्,
             गणना merge_mode merge)
{
    जइ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        प्रत्यागम false;
    }

    xkb_layout_index_t group = 0;
    जइ (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false, &group, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        प्रत्यागम false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    जइ (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        प्रत्यागम false;
    }

    xkb_layout_index_t group_to_use;
    जइ (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    अण्णहा जइ (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    अण्णहा {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        प्रत्यागम false;
    }

    जइ (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        प्रतिज्ञा(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } अण्णहा {
        ध्रुव xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        जइ (old_name != XKB_ATOM_NONE && old_name != name) {
            ध्रुव bool replace = (merge != MERGE_AUGMENT);
            ध्रुव xkb_atom_t use    = (replace ? name : old_name);
            ध्रुव xkb_atom_t ignore = (replace ? old_name : name);
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

    प्रत्यागम true;
}

स्थिर bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;
    bool ret;

    जइ (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        प्रत्यागम false;

    जइ (elem && istreq(elem, "key")) {
        कुञ्चिका_Info temp = {0};
        Init_कुञ्चिका_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->मानम्) &&
              Merge_कुञ्चिकाः(info, &info->default_कुञ्चिका, &temp, true);
        Clear_कुञ्चिका_Info(&temp);
    }
    अण्णहा जइ (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->मानम्, stmt->merge);
    }
    अण्णहा जइ (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अण्णहा जइ (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अण्णहा जइ (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अण्णहा जइ (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अण्णहा जइ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->मानम्, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अण्णहा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    प्रत्यागम ret;
}

स्थिर bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, कुञ्चिका_Info *keyi)
{
    bool all_valid_entries = true;

    प्रति (; def; def = (VarDef *) def->common.next) {
        ध्रुव वर्ण *field;
        व्यञ्जकम्_Def *arrayNdx;
        bool ok = true;

        जइ (!def->name) {
            जइ (unlikely(!def->मानम्) || def->मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            अण्णहा
                field = "actions";
            arrayNdx = रिक्त;
        }
        अण्णहा {
            ध्रुव वर्ण *elem;
            ok = व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            जइ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        जइ (unlikely(!def->मानम्)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        जइ (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->मानम्))
            all_valid_entries = false;
    }

    प्रत्यागम all_valid_entries;
}

स्थिर bool
SetExplicitGroup(SymbolsInfo *info, कुञ्चिका_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    जइ (info->explicit_group == XKB_LAYOUT_INVALID)
        प्रत्यागम true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        जइ (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    जइ (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, कुञ्चिका_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    जइ (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    प्रत्यागम true;
}

स्थिर bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    कुञ्चिका_Info keyi;

    keyi = info->default_कुञ्चिका;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_कुञ्चिका.groups);
    प्रति (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_कुञ्चिका.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->कुञ्चिका_Name;

    जइ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_कुञ्चिका_Symbols(info, &keyi, true)) {
        प्रत्यागम true;
    }

    Clear_कुञ्चिका_Info(&keyi);
    info->error_गणनम्++;
    प्रत्यागम false;
}

स्थिर bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    संरचना xkb_context *ctx = info->ctx;

    जइ (!व्यञ्जकम्_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        प्रत्यागम false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    प्रतिज्ञा(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_व्यञ्जकम्_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    प्रति (व्यञ्जकम्_Def *कुञ्चिका = def->कुञ्चिकाः; कुञ्चिका; कुञ्चिका = (व्यञ्जकम्_Def *) कुञ्चिका->common.next) {
        जइ (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYNAME_LITERAL) {
            tmp.have_चिह्नम् = false;
            tmp.u.कुञ्चिका_Name = कुञ्चिका->कुञ्चिका_name.कुञ्चिका_name;
        }
        अण्णहा जइ (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYSYM_LITERAL) {
            जइ (कुञ्चिका->keysym.keysym == XKB_कुञ्चिका_No_चिह्नम्) {
                /* Invalid keysym: ignore. Error message already printed */
                अनुवर्त;
            } अण्णहा {
                tmp.have_चिह्नम् = true;
                tmp.u.कुञ्चिका_Sym = कुञ्चिका->keysym.keysym;
            }
        }
        अण्णहा {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            अनुवर्त;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    प्रत्यागम ok;
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            भङ्ग;
        पक्ष STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            भङ्ग;
        पक्ष STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            भङ्ग;
        पक्ष STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            भङ्ग;
        स्वभाव:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        जइ (!ok)
            info->error_गणनम्++;

        जइ (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
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
स्थिर संरचना xkb_कुञ्चिका *
Find_कुञ्चिका_For_चिह्नम्(संरचना xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    करेहि {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        करेहि {
            got_one_level = false;
            संरचना xkb_कुञ्चिका *कुञ्चिका;
            xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
                जइ (group < कुञ्चिका->अङ्कः_groups &&
                    level < Xkb_कुञ्चिका_अङ्कः_Levels(कुञ्चिका, group)) {
                    got_one_group = got_one_level = true;
                    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms =
                        कुञ्चिका->groups[group].levels[level].अङ्कः_syms;
                    जइ (अङ्कः_syms > 1) {
                        प्रति (xkb_keysym_गणनम्_t k = 0; k < अङ्कः_syms; k++) {
                            जइ (कुञ्चिका->groups[group].levels[level].s.syms[k] == sym)
                                प्रत्यागम कुञ्चिका;
                        }
                    } अण्णहा जइ (अङ्कः_syms &&
                               कुञ्चिका->groups[group].levels[level].s.sym == sym) {
                        प्रत्यागम कुञ्चिका;
                    }
                }
            }
            level++;
        } जाव (got_one_level);
        group++;
    } जाव (got_one_group);

    प्रत्यागम रिक्त;
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
स्थिर xkb_atom_t
FindAutomaticType(संरचना xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ध्रुव xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).अङ्कः_syms == 0 ? \
        XKB_कुञ्चिका_No_चिह्नम् : \
     darray_item(groupi->levels, level).अङ्कः_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    जइ (width == 1 || width <= 0)
        प्रत्यागम xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    जइ (width == 2) {
        जइ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "ALPHABETIC");

        जइ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "KEYPAD");

        प्रत्यागम xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    जइ (width <= 4) {
        जइ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_कुञ्चिका_No_चिह्नम्);

            जइ (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        जइ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    प्रत्यागम XKB_ATOM_NONE;

#undef GET_SYM
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type *
FindTypeForGroup(संरचना xkb_keymap *keymap, कुञ्चिका_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    जइ (type_name == XKB_ATOM_NONE) {
        जइ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अण्णहा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            जइ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    जइ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_कुञ्चिका_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        गच्छ use_default;
    }

    darray_size_t i;
    प्रति (i = 0; i < keymap->अङ्कः_types; i++)
        जइ (keymap->types[i].name == type_name)
            भङ्ग;

    जइ (i >= keymap->अङ्कः_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_कुञ्चिका_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), group + 1);
        गच्छ use_default;
    }

    keymap->types[i].required = true;
    प्रत्यागम &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    प्रत्यागम &keymap->types[0];
}

स्थिर bool
CopySymbolsDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                       कुञ्चिका_Info *keyi)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;
    GroupInfo *groupi;
    ध्रुव GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    कुञ्चिका = Xkb_कुञ्चिका_By_Name(keymap, keyi->name, false);
    जइ (!कुञ्चिका) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    /* Find the range of groups we need. */
    कुञ्चिका->अङ्कः_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ध्रुव bool has_explicit_type = (keyi->defined & कुञ्चिका_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        जइ (darray_size(groupi->levels) > 0 || has_explicit_type)
            कुञ्चिका->अङ्कः_groups = i + 1;
        जइ (has_explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    जइ (कुञ्चिका->अङ्कः_groups <= 0) {
        /* A key with no group may still have other fields defined */
        जइ (keyi->defined)
            गच्छ कुञ्चिका_fields;
        अण्णहा
            प्रत्यागम false;
    }

    darray_resize(keyi->groups, कुञ्चिका->अङ्कः_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        जइ (groupi->defined)
            अनुवर्त;

        CopyGroupInfo(groupi, group0);
    }

    कुञ्चिका->groups = calloc(कुञ्चिका->अङ्कः_groups, परिमाण(*कुञ्चिका->groups));
    प्रतिज्ञा(darray_size(keyi->groups) == कुञ्चिका->अङ्कः_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < कुञ्चिका->अङ्कः_groups);
        #endif
        bool explicit_type = false;
        ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        जइ (type->अङ्कः_levels < darray_size(groupi->levels)) {
            संरचना xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->अङ्कः_levels,
                    कुञ्चिका_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->अङ्कः_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->अङ्कः_levels);

        कुञ्चिका->groups[i].explicit_type = explicit_type;
        कुञ्चिका->groups[i].type = type;
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
        संरचना xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            विकल्प (leveli->अङ्कः_syms) {
            पक्ष 0:
                leveli->upper = XKB_कुञ्चिका_No_चिह्नम्;
                भङ्ग;
            पक्ष 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                भङ्ग;
            स्वभाव:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                    ध्रुव xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    जइ (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        भङ्ग;
                    }
                }
                जइ (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->अङ्कः_syms *
                                             परिमाण(*leveli->s.syms));
                    जइ (!leveli->s.syms)
                        प्रत्यागम false; /* FIXME: better handling? */
                    प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                        leveli->s.syms[leveli->अङ्कः_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &कुञ्चिका->groups[i].levels, रिक्त);
        जइ (कुञ्चिका->groups[i].type->अङ्कः_levels > 1 ||
            कुञ्चिका->groups[i].levels[0].अङ्कः_syms > 0) {
            कुञ्चिका->groups[i].explicit_symbols = true;
            कुञ्चिका->explicit |= EXPLICIT_SYMBOLS;
        }
        जइ (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            कुञ्चिका->groups[i].explicit_actions = true;
            कुञ्चिका->explicit |= EXPLICIT_INTERP;
        }
        जइ (कुञ्चिका->groups[i].explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    कुञ्चिका->out_of_range_pending_group = keyi->out_of_range_pending_group;
    कुञ्चिका->out_of_range_group_सङ्ख्या = keyi->out_of_range_group_सङ्ख्या;
    कुञ्चिका->out_of_range_group_policy = keyi->out_of_range_group_policy;

कुञ्चिका_fields:
    जइ (keyi->defined & कुञ्चिका_FIELD_VMODMAP) {
        कुञ्चिका->vmodmap = keyi->vmodmap;
        कुञ्चिका->explicit |= EXPLICIT_VMODMAP;
    }

    जइ (keyi->repeat != कुञ्चिका_REPEAT_UNDEFINED) {
        कुञ्चिका->repeats = (keyi->repeat == कुञ्चिका_REPEAT_YES);
        कुञ्चिका->explicit |= EXPLICIT_REPEAT;
    }

    जइ ((keyi->defined & कुञ्चिका_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        कुञ्चिका->overlays = keyi->overlays;
        जइ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = कुञ्चिका->overlays;
            ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
            जाव (remaining) {
                /* isolate lowest set bit */
                ध्रुव xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                जइ (*overlays_कुञ्चिकाः) {
                    overlays_कुञ्चिकाः++;
                } अण्णहा {
                    /* drop current null value (overlayN=none) */
                    कुञ्चिका->overlays &= ~lsb;
                }
            }

            जइ (कुञ्चिका->overlays) {
                /* Steal */
                कुञ्चिका->overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
                keyi->overlays_कुञ्चिकाः = रिक्त;
                keyi->overlays_alloc = 0;

                कुञ्चिका->overlays_inline = false;
                कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
            }
        } अण्णहा {
            कुञ्चिका->overlay_कुञ्चिका = keyi->overlay_कुञ्चिका;
            कुञ्चिका->overlays_inline = true;
            कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
        }
    }

    प्रत्यागम true;
}

स्थिर bool
CopyModMapDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;

    जइ (!entry->have_चिह्नम्) {
        कुञ्चिका = Xkb_कुञ्चिका_By_Name(keymap, entry->u.कुञ्चिका_Name, true);
        जइ (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    कुञ्चिका_Name_Text(info->ctx, entry->u.कुञ्चिका_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            प्रत्यागम false;
        }
    }
    अण्णहा {
        कुञ्चिका = Find_कुञ्चिका_For_चिह्नम्(keymap, entry->u.कुञ्चिका_Sym);
        जइ (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_चिह्नम्,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.कुञ्चिका_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            प्रत्यागम false;
        }
    }

    /* Skip modMap None */
    जइ (entry->mods) {
        कुञ्चिका->modmap |= entry->mods;
        entry->कुञ्चिका_Code = कुञ्चिका->keycode;
    }

    प्रत्यागम true;
}

स्थिर bool
CopySymbolsToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info)
{
    कुञ्चिका_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->अङ्कः_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, रिक्त);

    darray_foreach(keyi, info->कुञ्चिकाः)
        जइ (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_गणनम्++;

    जइ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
            जइ (कुञ्चिका->name == XKB_ATOM_NONE)
                अनुवर्त;

            जइ (कुञ्चिका->अङ्कः_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        जइ (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_गणनम्++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अङ्कः_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    प्रत्यागम true;
}

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    जइ (file !=रिक्त)
        HandleSymbolsFile(&info, file);

    जइ (info.error_गणनम् != 0)
        गच्छ err_info;

    जइ (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearSymbolsInfo(&info);
    प्रत्यागम true;

err_info:
    ClearSymbolsInfo(&info);
    प्रत्यागम false;
}
