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
    सचे (from->अङ्कः_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अञ्ञथा {
        into->s.sym = from->s.sym;
    }
    into->अङ्कः_syms = from->अङ्कः_syms;
    from->अङ्कः_syms = 0;

    सचे (from->अङ्कः_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अञ्ञथा {
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
    अत्थाय (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        सचे (darray_item(from->levels, j).अङ्कः_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).अङ्कः_syms,
                       परिमाण(xkb_keysym_t));
        }
        सचे (darray_item(from->levels, j).अङ्कः_actions > 1) {
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
    सचे (keyi->overlays_alloc)
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
    पटिनिवत्त कुञ्चिका_Name_Text(info->ctx, keyi->name);
}

स्थिर bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t कुञ्चिका_name)
{
    /* First find the type of the merged group. */
    सचे (into->type != from->type) {
        सचे (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अञ्ञथा सचे (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अञ्ञथा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            सचे (report) {
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

    सचे (darray_empty(from->levels)) {
        InitGroupInfo(from);
        पटिनिवत्त true;
    }

    सचे (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        पटिनिवत्त true;
    }

    /* Merge the actions and syms. */
    ध्रुव darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_गणनम् = 0;
    darray_size_t from_Actions_गणनम् = 0;
    अत्थाय (darray_size_t i = 0; i < levels_in_both; i++) {
        संरचना xkb_level* ध्रुव intoLevel = &darray_item(into->levels, i);
        संरचना xkb_level* ध्रुव fromLevel = &darray_item(from->levels, i);

        ध्रुव bool fromHasNoKeysym = fromLevel->अङ्कः_syms == 0;
        ध्रुव bool fromHasNoAction = fromLevel->अङ्कः_actions == 0;
        सचे (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            अनुवर्त;
        }

        ध्रुव bool intoHasNoKeysym = intoLevel->अङ्कः_syms == 0;
        ध्रुव bool intoHasNoAction = intoLevel->अङ्कः_actions == 0;
        सचे (intoHasNoKeysym && intoHasNoAction) {
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
        सचे (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            सचे (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_चिह्नम्,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            सचे (fromHasNoKeysym) {
                /* No keysym to copy */
            } अञ्ञथा सचे (clobber) {
                /* Override: copy any defined keysym from `from` */
                सचे (unlikely(fromLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    सचे (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                    fromLevel->अङ्कः_syms = 0;
                    from_Keysyms_गणनम्++;
                } अञ्ञथा सचे (fromLevel->s.sym != XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single defined keysym */
                    सचे (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अङ्कः_syms = 1;
                    from_Keysyms_गणनम्++;
                }
            } अञ्ञथा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                सचे (unlikely(intoLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अञ्ञथा सचे (intoLevel->s.sym == XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single undefined keysym */
                    सचे (unlikely(fromLevel->अङ्कः_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                        fromLevel->अङ्कः_syms = 0;
                    } अञ्ञथा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अङ्कः_syms = 1;
                    }
                    from_Keysyms_गणनम्++;
                }
            }
        }

        /* Handle actions */
        सचे (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            सचे (report && !(intoHasNoAction || fromHasNoAction)) {
                सचे (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अञ्ञथा {
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
            सचे (fromHasNoAction) {
                /* No action to copy */
            } अञ्ञथा सचे (clobber) {
                /* Override: copy any defined action from `from` */
                सचे (unlikely(fromLevel->अङ्कः_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    सचे (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                    fromLevel->अङ्कः_actions = 0;
                    from_Actions_गणनम्++;
                } अञ्ञथा सचे (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    सचे (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अङ्कः_actions = 1;
                    from_Actions_गणनम्++;
                }
            } अञ्ञथा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                सचे (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अञ्ञथा सचे (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    सचे (unlikely(fromLevel->अङ्कः_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                        fromLevel->अङ्कः_actions = 0;
                    } अञ्ञथा {
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
    सचे (from_Keysyms_गणनम्) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        सचे (from_Keysyms_गणनम् == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    सचे (from_Actions_गणनम्) {
        /* Reset defined actions field if we used no action from `into` */
        सचे (from_Actions_गणनम् == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    पटिनिवत्त true;
}

स्थिर bool
Use_New_कुञ्चिका_Field(गणना कुञ्चिका_field field, गणना कुञ्चिका_field old, गणना कुञ्चिका_field new,
               bool clobber, bool report, गणना कुञ्चिका_field *collide)
{
    सचे (!(old & field))
        पटिनिवत्त (new & field);

    सचे (new & field) {
        सचे (report)
            *collide |= field;

        पटिनिवत्त clobber;
    }

    पटिनिवत्त false;
}

स्थिर bool
overlays_get(ध्रुव कुञ्चिका_Info *info, xkb_overlay_index_t bit,
             ध्रुव संरचना xkb_कुञ्चिका **कुञ्चिका_out)
{
    सचे (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * CHAR_BIT))
        पटिनिवत्त false;
    ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    सचे (!(info->overlays & mask)) पटिनिवत्त false;
    सचे (कुञ्चिका_out) {
        सचे (!info->overlays_alloc) {
            /* Stored inline */
            *कुञ्चिका_out = info->overlay_कुञ्चिका;
        } अञ्ञथा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ध्रुव xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(नीचः);
            *कुञ्चिका_out = info->overlays_कुञ्चिकाः[index];
        }
    }
    पटिनिवत्त true;
}

स्थिर bool
overlays_निवेशय(कुञ्चिका_Info *keyi, xkb_overlay_index_t bit,
                ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    सचे (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * CHAR_BIT))
        पटिनिवत्त false;

    ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    सचे ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        सचे (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_कुञ्चिका = कुञ्चिका;
            सचे (!कुञ्चिका)
                keyi->overlays_clear = true;
        } अञ्ञथा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ध्रुव xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(नीचः);
            keyi->overlays_कुञ्चिकाः[index] = कुञ्चिका;
        }
        पटिनिवत्त true;
    }

    /* Insert new element */
    सचे (!keyi->overlays || (keyi->overlays_clear && !कुञ्चिका)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_कुञ्चिका = कुञ्चिका;
        keyi->overlays |= mask;
        keyi->overlays_clear = !कुञ्चिका;
    } अञ्ञथा सचे (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ध्रुव xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ध्रुव xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = calloc(alloc, परिमाण(*tmp));
        सचे (!tmp) पटिनिवत्त false;

        /* Copy previous entries to the buffer */
        सचे (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ध्रुव xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(नीचः);
            tmp[idx] = keyi->overlay_कुञ्चिका;
        } अञ्ञथा {
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
    } अञ्ञथा {
        /* Allocated items: grow heap and shift if relevant */
        ध्रुव xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ध्रुव xkb_overlay_index_t गणनम् =
            (xkb_overlay_index_t)popcount32(overlays);
        सचे (गणनम् > keyi->overlays_alloc) {
            ध्रुव xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(गणनम्);
            प्रतिज्ञा(alloc >= गणनम्);
            ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = realloc(
                keyi->overlays_कुञ्चिकाः,
                (size_t)alloc * परिमाण(*keyi->overlays_कुञ्चिकाः)
            );
            सचे (!tmp) पटिनिवत्त false;
            keyi->overlays_कुञ्चिकाः = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ध्रुव xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(नीचः);
        सचे (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        सचे (index < गणनम् - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            प्रतिज्ञा(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_कुञ्चिकाः + index + 1, keyi->overlays_कुञ्चिकाः + index,
                    (size_t)(गणनम् - 1u - index) * परिमाण(ध्रुव संरचना xkb_कुञ्चिका *));
        }
        keyi->overlays_कुञ्चिकाः[index] = कुञ्चिका;
        keyi->overlays = overlays;
    }

    पटिनिवत्त true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
स्थिर bool
merge_overlays(SymbolsInfo *info, कुञ्चिका_Info *into, कुञ्चिका_Info *from,
               bool clobber, bool report, गणना कुञ्चिका_field *collide)
{
    सचे (!(from->defined & कुञ्चिका_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अञ्ञथा सचे (!(into->defined & कुञ्चिका_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
        from->overlays_कुञ्चिकाः = रिक्त;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= कुञ्चिका_FIELD_OVERLAY;
    } अञ्ञथा सचे (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अञ्ञथा {
        सचे (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ध्रुव xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ध्रुव xkb_overlay_index_t गणनम् =
                (xkb_overlay_index_t)popcount32(result_mask);

            सचे (गणनम् == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            कुञ्चिका_Info *dest = into;
            कुञ्चिका_Info *src = from;
            सचे (into->overlays_alloc >= गणनम् &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अञ्ञथा सचे (from->overlays_alloc >= गणनम्) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अञ्ञथा सचे (गणनम् > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                सचे (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                सचे (!dest->overlays_alloc) {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp = calloc(गणनम्, परिमाण(*tmp));
                    सचे (!tmp)
                        पटिनिवत्त false;
                    सचे (dest->overlay_कुञ्चिका)
                        tmp[0] = dest->overlay_कुञ्चिका;
                    dest->overlays_कुञ्चिकाः = tmp;
                    dest->overlays_alloc = गणनम्;
                } अञ्ञथा {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp =
                        realloc(dest->overlays_कुञ्चिकाः,
                                गणनम् * परिमाण(*dest->overlays_कुञ्चिकाः));
                    सचे (!tmp)
                        पटिनिवत्त false;
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
            याव (remaining) {
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
                सचे (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ध्रुव संरचना xkb_कुञ्चिका *src_कुञ्चिका = (src_कुञ्चिकाः)
                    ? *(src_कुञ्चिकाः++)
                    : रिक्त;

                /* check dest */
                ध्रुव संरचना xkb_कुञ्चिका *dest_कुञ्चिका = रिक्त;
                ध्रुव bool conflict = overlays_get(dest, bit, &dest_कुञ्चिका);
                सचे (conflict) {
                    सचे (dest_कुञ्चिका == src_कुञ्चिका)
                       अनुवर्त;
                    अञ्ञथा सचे (report)
                        *collide |= कुञ्चिका_FIELD_OVERLAY;
                }

                सचे ((!conflict || clobber) && !overlays_निवेशय(dest, bit, src_कुञ्चिका))
                    पटिनिवत्त false;
            }

            सचे (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                सचे (into->overlays_alloc)
                    मुक्त(into->overlays_कुञ्चिकाः);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                सचे (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
                    from->overlays_कुञ्चिकाः = रिक्त;
                    from->overlays_alloc = 0;
                } अञ्ञथा {
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                }
            }
        } अञ्ञथा {
            /* Disjoint overlays */

            सचे (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_कुञ्चिका == from->overlay_कुञ्चिका) {
                /* Indentical: do nothing */
                पटिनिवत्त true;
            }
            सचे (!(into->overlays & from->overlays)) {
                सचे (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                    पटिनिवत्त true;
                } अञ्ञथा सचे (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    पटिनिवत्त true;
                }
            }
            /* Conflict */
            सचे (report)
                *collide |= कुञ्चिका_FIELD_OVERLAY;
            सचे (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
            }
        }
    }
    पटिनिवत्त true;
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

    सचे (from->merge == MERGE_REPLACE) {
        Clear_कुञ्चिका_Info(into);
        *into = *from;
        Init_कुञ्चिका_Info(info->ctx, from);
        पटिनिवत्त true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    अत्थाय (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    अत्थाय (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    सचे (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= कुञ्चिका_FIELD_VMODMAP;
    }
    सचे (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= कुञ्चिका_FIELD_REPEAT;
    }
    सचे (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= कुञ्चिका_FIELD_DEFAULT_TYPE;
    }
    सचे (Use_New_कुञ्चिका_Field(कुञ्चिका_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_सङ्ख्या = from->out_of_range_group_सङ्ख्या;
        into->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }

    सचे (!merge_overlays(info, into, from, clobber, report, &collide))
        पटिनिवत्त false;

    सचे (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 कुञ्चिका_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_कुञ्चिका_Info(from);
    Init_कुञ्चिका_Info(info->ctx, from);
    पटिनिवत्त true;
}

स्थिर xkb_atom_t
Xkb_Resolve_कुञ्चिका_Alias(ध्रुव संरचना xkb_keymap *keymap, xkb_atom_t name)
{
    सचे (name < keymap->अङ्कः_कुञ्चिका_names) {
        ध्रुव KeycodeMatch match = keymap->कुञ्चिका_names[name];
        सचे (match.found && match.is_alias) {
            पटिनिवत्त match.alias.real;
        }
    }
    पटिनिवत्त name;
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
        सचे (iter->name == keyi->name)
            पटिनिवत्त Merge_कुञ्चिकाः(info, iter, keyi, same_file);

    darray_append(info->कुञ्चिकाः, *keyi);
    Init_कुञ्चिका_Info(info->ctx, keyi);
    पटिनिवत्त true;
}

स्थिर bool
AddModMapEntry(SymbolsInfo *info, ध्रुव ModMapEntry *new)
{
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        सचे ((new->have_चिह्नम् != old->have_चिह्नम्) ||
            (new->have_चिह्नम् && new->u.कुञ्चिका_Sym != old->u.कुञ्चिका_Sym) ||
            (!new->have_चिह्नम् && new->u.कुञ्चिका_Name != old->u.कुञ्चिका_Name))
            अनुवर्त;

        सचे (new->mods == old->mods)
            पटिनिवत्त true;

        ध्रुव xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ध्रुव xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        सचे (new->have_चिह्नम्) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.कुञ्चिका_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } अञ्ञथा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    कुञ्चिका_Name_Text(info->ctx, new->u.कुञ्चिका_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        पटिनिवत्त true;
    }

    darray_append(info->modmaps, *new);

    पटिनिवत्त true;
}

/***====================================================================***/

स्थिर शून्य
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     गणना merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    सचे (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        पटिनिवत्त;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    सचे (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    अत्थाय (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        सचे (!darray_item(from->group_names, i))
            अनुवर्त;

        सचे (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            अनुवर्त;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    सचे (darray_empty(into->कुञ्चिकाः)) {
        into->कुञ्चिकाः = from->कुञ्चिकाः;
        darray_init(from->कुञ्चिकाः);
    }
    अञ्ञथा {
        कुञ्चिका_Info *keyi;
        darray_foreach(keyi, from->कुञ्चिकाः) {
            keyi->merge = merge;
            सचे (!Add_कुञ्चिका_Symbols(into, keyi, false))
                into->error_गणनम्++;
        }
    }

    सचे (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अञ्ञथा {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            सचे (!AddModMapEntry(into, mm))
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

    सचे (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गणनम् += 10;
        पटिनिवत्त false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    अत्थाय (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, परिमाण(path));
        सचे (!file) {
            info->error_गणनम् += 10;
            ClearSymbolsInfo(&included);
            पटिनिवत्त false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        सचे (stmt->modifier) {
            ध्रुव पूर्णाङ्क गणनम् = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            सचे (गणनम् <= 0 || *(stmt->modifier + गणनम्) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } अञ्ञथा {
                next_incl.explicit_group--;
            }
        }
        अञ्ञथा सचे (info->keymap_info->keymap.अङ्कः_groups != 0 &&
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
        अञ्ञथा {
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

    पटिनिवत्त (info->error_गणनम् == 0);
}

स्थिर bool
GetGroupIndex(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
              गणना group_field field, xkb_layout_index_t *ndx_rtrn)
{
    प्रतिज्ञा (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ध्रुव वर्ण *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    सचे (arrayNdx == रिक्त) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            सचे (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                पटिनिवत्त true;
            }
        }

        सचे (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, कुञ्चिका_Info_Text(info, keyi), info->max_groups, name);
            पटिनिवत्त false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        पटिनिवत्त true;
    }

    सचे (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, कुञ्चिका_Info_Text(info, keyi));
        पटिनिवत्त false;
    }

    (*ndx_rtrn)--;
    सचे (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    पटिनिवत्त true;
}

स्थिर bool
Add_Symbols_To_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_index_t ndx = 0;

    सचे (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        पटिनिवत्त false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    सचे (मानम्->common.type == STMT_व्यञ्जकम्_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        पटिनिवत्त true;
    }

    सचे (मानम्->common.type != STMT_व्यञ्जकम्_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(मानम्->common.type), ndx + 1,
                कुञ्चिका_Info_Text(info, keyi));
        पटिनिवत्त false;
    }

    सचे (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_कुञ्चिका_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                कुञ्चिका_Info_Text(info, keyi), ndx + 1);
        पटिनिवत्त false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    अत्थाय (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        सचे (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    सचे (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    सचे (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    अत्थाय (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList && level < nLevels;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्कः_syms == 0);

        सचे (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            पटिनिवत्त false;
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
            अत्थाय (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++)
                प्रतिज्ञा(leveli->s.syms[k] != XKB_कुञ्चिका_No_चिह्नम्);
#endif
        }
    }

    पटिनिवत्त true;
}

स्थिर bool
Add_Actions_To_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_index_t ndx = 0;

    सचे (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        पटिनिवत्त false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    सचे (मानम्->common.type == STMT_व्यञ्जकम्_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        पटिनिवत्त true;
    }

    सचे (मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मानम्->common.type, ndx, कुञ्चिका_Info_Text(info, keyi));
        पटिनिवत्त false;
    }

    सचे (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 कुञ्चिका_Info_Text(info, keyi), ndx);
        पटिनिवत्त false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    अत्थाय (ParseCommon *p = &मानम्->common; p; p = p->next)
        nLevels++;
    सचे (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    अत्थाय (व्यञ्जकम्_Action_List *actionList = (व्यञ्जकम्_Action_List *) मानम्;
         actionList;
         actionList = (व्यञ्जकम्_Action_List *) actionList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्कः_actions == 0);

        unsigned पूर्णाङ्क अङ्कः_actions = 0;
        अत्थाय (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.next)
            अङ्कः_actions++;

        सचे (unlikely(अङ्कः_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, अङ्कः_actions);
            पटिनिवत्त false;
        }

        /* Parse actions and add only defined actions */
        darray(सङ्घ xkb_action) actions = darray_new();
        अत्थाय (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.next) {
            सङ्घ xkb_action toAct = { 0 };
            ध्रुव गणना xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            सचे (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1);
                सचे (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    पटिनिवत्त false;
                } अञ्ञथा {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            सचे (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अञ्ञथा सचे (likely(अङ्कः_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अङ्कः_actions = 1;
                leveli->a.action = toAct;
                गच्छ next_level;
            } अञ्ञथा {
                darray_append(actions, toAct);
            }
        }
        सचे (darray_empty(actions)) {
            leveli->अङ्कः_actions = 0;
        } अञ्ञथा सचे (likely(darray_size(actions) > 1)) {
            leveli->अङ्कः_actions = (xkb_action_गणनम्_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, रिक्त);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            अत्थाय (xkb_action_गणनम्_t k = 0; k < leveli->अङ्कः_actions; k++)
                प्रतिज्ञा(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } अञ्ञथा {
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
        सचे (leveli->अङ्कः_actions > 0 || leveli->अङ्कः_syms > 0)
            nonEmptyLevels = level + 1;
    }

    सचे (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        सचे (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        अञ्ञथा
            darray_free(groupi->levels);
    }

    पटिनिवत्त true;
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
    सचे (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        पटिनिवत्त false;
    }
    static_assert(परिमाण(xkb_overlay_index_t) == परिमाण(uint8_t), "");
    ध्रुव size_t prefix = परिमाण("overlay") - 1;
    ध्रुव size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    सचे (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (पूर्णाङ्क)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        पटिनिवत्त false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
        *कुञ्चिका_rtrn = Xkb_कुञ्चिका_By_Name(&keymap_info->keymap, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name,
                                 false);
        सचे (!*कुञ्चिका_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name),
                    field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
            पटिनिवत्त false;
        }
        पटिनिवत्त true;
    पक्ष STMT_व्यञ्जकम्_IDENT: {
        ध्रुव वर्ण * ध्रुव id =
            xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->ident.ident);
        सचे (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *कुञ्चिका_rtrn = रिक्त;
            पटिनिवत्त true;
        } अञ्ञथा सचे (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *कुञ्चिका_rtrn = रिक्त;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            पटिनिवत्त true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        पटिनिवत्त false;
    }
    स्वभाव:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_व्यञ्जकम्_KEYNAME_LITERAL),
                field, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;
    }
}

स्थिर bool
SetSymbolsField(SymbolsInfo *info, कुञ्चिका_Info *keyi, ध्रुव वर्ण *field,
                व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def **मानम्_ptr)
{
    व्यञ्जकम्_Def * restrict ध्रुव मानम् = *मानम्_ptr;
    सचे (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        सचे (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            पटिनिवत्त false;
        }

        सचे (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= कुञ्चिका_FIELD_DEFAULT_TYPE;
        }
        अञ्ञथा सचे (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, रिक्त) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }
        अञ्ञथा {
            ndx--;
            सचे (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                प्रतिज्ञा(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    अञ्ञथा सचे (istreq(field, "symbols")) {
        पटिनिवत्त Add_Symbols_To_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अञ्ञथा सचे (istreq(field, "actions")) {
        पटिनिवत्त Add_Actions_To_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अञ्ञथा सचे (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, मानम्, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(मानम्->common.type),
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= कुञ्चिका_FIELD_VMODMAP;
    }
    अञ्ञथा सचे (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अञ्ञथा सचे (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अञ्ञथा सचे (istreq_prefix("permanentoverlay", field)) {
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
    अञ्ञथा सचे (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = रिक्त;
        सचे (!व्यञ्जकम्_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *मानम्_ptr, keyi, &overlay, &कुञ्चिका))
            पटिनिवत्त false;
        सचे (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            पटिनिवत्त true;
        } अञ्ञथा सचे (कुञ्चिका && कुञ्चिका->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, कुञ्चिका_Info_Text(info, keyi));
        } अञ्ञथा {
            ध्रुव संरचना xkb_कुञ्चिका *prev;
            सचे (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                सचे (कुञ्चिका != prev) {
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
            } अञ्ञथा सचे (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                सचे (!overlays_निवेशय(keyi, overlay, कुञ्चिका))
                    पटिनिवत्त false;
                keyi->defined |= कुञ्चिका_FIELD_OVERLAY;
            } अञ्ञथा {
                /* Handle disjoint overlays mode */
                ध्रुव xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                सचे (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    सचे (कुञ्चिका) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_कुञ्चिका = कुञ्चिका;
                    } अञ्ञथा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= कुञ्चिका_FIELD_OVERLAY;
                } अञ्ञथा सचे (keyi->overlays) {
                    सचे (कुञ्चिका) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                कुञ्चिका_Info_Text(info, keyi), keyi->overlays,
                                कुञ्चिका_Name_Text(info->ctx, keyi->overlay_कुञ्चिका->name),
                                overlay + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
                        पटिनिवत्त !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_मानम्_MISMATCH);
                    } अञ्ञथा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अञ्ञथा सचे (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        सचे (!व्यञ्जकम्_Resolve_Enum(info->ctx, मानम्, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }

        keyi->repeat = (गणना कुञ्चिका_repeat) val;
        keyi->defined |= कुञ्चिका_FIELD_REPEAT;
    }
    अञ्ञथा सचे (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        सचे (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अञ्ञथा सचे (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        सचे (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अञ्ञथा सचे (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        सचे (व्यञ्जकम्_Resolve_Group(info->keymap_info, मानम्, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            पटिनिवत्त false;
        }

        सचे (pending) {
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
        } अञ्ञथा {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_सङ्ख्या = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= कुञ्चिका_FIELD_GROUPINFO;
    }
    अञ्ञथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_UNKNOWN_कुञ्चिका_FIELDS);
    }

    पटिनिवत्त true;
}

स्थिर bool
SetGroupName(SymbolsInfo *info, व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्,
             गणना merge_mode merge)
{
    सचे (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        पटिनिवत्त false;
    }

    xkb_layout_index_t group = 0;
    सचे (व्यञ्जकम्_Resolve_Group(info->keymap_info, arrayNdx, false, &group, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        पटिनिवत्त false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    सचे (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        पटिनिवत्त false;
    }

    xkb_layout_index_t group_to_use;
    सचे (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    अञ्ञथा सचे (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    अञ्ञथा {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        पटिनिवत्त false;
    }

    सचे (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        प्रतिज्ञा(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } अञ्ञथा {
        ध्रुव xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        सचे (old_name != XKB_ATOM_NONE && old_name != name) {
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

    पटिनिवत्त true;
}

स्थिर bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;
    bool ret;

    सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        पटिनिवत्त false;

    सचे (elem && istreq(elem, "key")) {
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
    अञ्ञथा सचे (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->मानम्, stmt->merge);
    }
    अञ्ञथा सचे (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अञ्ञथा सचे (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अञ्ञथा सचे (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अञ्ञथा सचे (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अञ्ञथा सचे (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->मानम्, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अञ्ञथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        पटिनिवत्त !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    पटिनिवत्त ret;
}

स्थिर bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, कुञ्चिका_Info *keyi)
{
    bool all_valid_entries = true;

    अत्थाय (; def; def = (VarDef *) def->common.next) {
        ध्रुव वर्ण *field;
        व्यञ्जकम्_Def *arrayNdx;
        bool ok = true;

        सचे (!def->name) {
            सचे (unlikely(!def->मानम्) || def->मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            अञ्ञथा
                field = "actions";
            arrayNdx = रिक्त;
        }
        अञ्ञथा {
            ध्रुव वर्ण *elem;
            ok = व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            सचे (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        सचे (unlikely(!def->मानम्)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        सचे (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->मानम्))
            all_valid_entries = false;
    }

    पटिनिवत्त all_valid_entries;
}

स्थिर bool
SetExplicitGroup(SymbolsInfo *info, कुञ्चिका_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    सचे (info->explicit_group == XKB_LAYOUT_INVALID)
        पटिनिवत्त true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        सचे (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    सचे (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, कुञ्चिका_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    सचे (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    पटिनिवत्त true;
}

स्थिर bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    कुञ्चिका_Info keyi;

    keyi = info->default_कुञ्चिका;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_कुञ्चिका.groups);
    अत्थाय (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_कुञ्चिका.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->कुञ्चिका_Name;

    सचे (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_कुञ्चिका_Symbols(info, &keyi, true)) {
        पटिनिवत्त true;
    }

    Clear_कुञ्चिका_Info(&keyi);
    info->error_गणनम्++;
    पटिनिवत्त false;
}

स्थिर bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    संरचना xkb_context *ctx = info->ctx;

    सचे (!व्यञ्जकम्_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        पटिनिवत्त false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    प्रतिज्ञा(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_व्यञ्जकम्_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    अत्थाय (व्यञ्जकम्_Def *कुञ्चिका = def->कुञ्चिकाः; कुञ्चिका; कुञ्चिका = (व्यञ्जकम्_Def *) कुञ्चिका->common.next) {
        सचे (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYNAME_LITERAL) {
            tmp.have_चिह्नम् = false;
            tmp.u.कुञ्चिका_Name = कुञ्चिका->कुञ्चिका_name.कुञ्चिका_name;
        }
        अञ्ञथा सचे (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYSYM_LITERAL) {
            सचे (कुञ्चिका->keysym.keysym == XKB_कुञ्चिका_No_चिह्नम्) {
                /* Invalid keysym: ignore. Error message already printed */
                अनुवर्त;
            } अञ्ञथा {
                tmp.have_चिह्नम् = true;
                tmp.u.कुञ्चिका_Sym = कुञ्चिका->keysym.keysym;
            }
        }
        अञ्ञथा {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            अनुवर्त;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    पटिनिवत्त ok;
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    अत्थाय (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
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

        सचे (!ok)
            info->error_गणनम्++;

        सचे (info->error_गणनम् > 10) {
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
    करोहि {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        करोहि {
            got_one_level = false;
            संरचना xkb_कुञ्चिका *कुञ्चिका;
            xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
                सचे (group < कुञ्चिका->अङ्कः_groups &&
                    level < Xkb_कुञ्चिका_अङ्कः_Levels(कुञ्चिका, group)) {
                    got_one_group = got_one_level = true;
                    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms =
                        कुञ्चिका->groups[group].levels[level].अङ्कः_syms;
                    सचे (अङ्कः_syms > 1) {
                        अत्थाय (xkb_keysym_गणनम्_t k = 0; k < अङ्कः_syms; k++) {
                            सचे (कुञ्चिका->groups[group].levels[level].s.syms[k] == sym)
                                पटिनिवत्त कुञ्चिका;
                        }
                    } अञ्ञथा सचे (अङ्कः_syms &&
                               कुञ्चिका->groups[group].levels[level].s.sym == sym) {
                        पटिनिवत्त कुञ्चिका;
                    }
                }
            }
            level++;
        } याव (got_one_level);
        group++;
    } याव (got_one_group);

    पटिनिवत्त रिक्त;
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

    सचे (width == 1 || width <= 0)
        पटिनिवत्त xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    सचे (width == 2) {
        सचे (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            पटिनिवत्त xkb_atom_intern_literal(ctx, "ALPHABETIC");

        सचे (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            पटिनिवत्त xkb_atom_intern_literal(ctx, "KEYPAD");

        पटिनिवत्त xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    सचे (width <= 4) {
        सचे (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_कुञ्चिका_No_चिह्नम्);

            सचे (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                पटिनिवत्त xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            पटिनिवत्त xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        सचे (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            पटिनिवत्त xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        पटिनिवत्त xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    पटिनिवत्त XKB_ATOM_NONE;

#undef GET_SYM
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type *
FindTypeForGroup(संरचना xkb_keymap *keymap, कुञ्चिका_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    सचे (type_name == XKB_ATOM_NONE) {
        सचे (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अञ्ञथा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            सचे (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    सचे (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_कुञ्चिका_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        गच्छ use_default;
    }

    darray_size_t i;
    अत्थाय (i = 0; i < keymap->अङ्कः_types; i++)
        सचे (keymap->types[i].name == type_name)
            भङ्ग;

    सचे (i >= keymap->अङ्कः_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_कुञ्चिका_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), group + 1);
        गच्छ use_default;
    }

    keymap->types[i].required = true;
    पटिनिवत्त &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    पटिनिवत्त &keymap->types[0];
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
    सचे (!कुञ्चिका) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                कुञ्चिका_Info_Text(info, keyi));
        पटिनिवत्त false;
    }

    /* Find the range of groups we need. */
    कुञ्चिका->अङ्कः_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ध्रुव bool has_explicit_type = (keyi->defined & कुञ्चिका_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        सचे (darray_size(groupi->levels) > 0 || has_explicit_type)
            कुञ्चिका->अङ्कः_groups = i + 1;
        सचे (has_explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    सचे (कुञ्चिका->अङ्कः_groups <= 0) {
        /* A key with no group may still have other fields defined */
        सचे (keyi->defined)
            गच्छ कुञ्चिका_fields;
        अञ्ञथा
            पटिनिवत्त false;
    }

    darray_resize(keyi->groups, कुञ्चिका->अङ्कः_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        सचे (groupi->defined)
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
        सचे (type->अङ्कः_levels < darray_size(groupi->levels)) {
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
                अत्थाय (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                    ध्रुव xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    सचे (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        भङ्ग;
                    }
                }
                सचे (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->अङ्कः_syms *
                                             परिमाण(*leveli->s.syms));
                    सचे (!leveli->s.syms)
                        पटिनिवत्त false; /* FIXME: better handling? */
                    अत्थाय (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                        leveli->s.syms[leveli->अङ्कः_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &कुञ्चिका->groups[i].levels, रिक्त);
        सचे (कुञ्चिका->groups[i].type->अङ्कः_levels > 1 ||
            कुञ्चिका->groups[i].levels[0].अङ्कः_syms > 0) {
            कुञ्चिका->groups[i].explicit_symbols = true;
            कुञ्चिका->explicit |= EXPLICIT_SYMBOLS;
        }
        सचे (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            कुञ्चिका->groups[i].explicit_actions = true;
            कुञ्चिका->explicit |= EXPLICIT_INTERP;
        }
        सचे (कुञ्चिका->groups[i].explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    कुञ्चिका->out_of_range_pending_group = keyi->out_of_range_pending_group;
    कुञ्चिका->out_of_range_group_सङ्ख्या = keyi->out_of_range_group_सङ्ख्या;
    कुञ्चिका->out_of_range_group_policy = keyi->out_of_range_group_policy;

कुञ्चिका_fields:
    सचे (keyi->defined & कुञ्चिका_FIELD_VMODMAP) {
        कुञ्चिका->vmodmap = keyi->vmodmap;
        कुञ्चिका->explicit |= EXPLICIT_VMODMAP;
    }

    सचे (keyi->repeat != कुञ्चिका_REPEAT_UNDEFINED) {
        कुञ्चिका->repeats = (keyi->repeat == कुञ्चिका_REPEAT_YES);
        कुञ्चिका->explicit |= EXPLICIT_REPEAT;
    }

    सचे ((keyi->defined & कुञ्चिका_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        कुञ्चिका->overlays = keyi->overlays;
        सचे (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = कुञ्चिका->overlays;
            ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
            याव (remaining) {
                /* isolate lowest set bit */
                ध्रुव xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                सचे (*overlays_कुञ्चिकाः) {
                    overlays_कुञ्चिकाः++;
                } अञ्ञथा {
                    /* drop current null value (overlayN=none) */
                    कुञ्चिका->overlays &= ~lsb;
                }
            }

            सचे (कुञ्चिका->overlays) {
                /* Steal */
                कुञ्चिका->overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
                keyi->overlays_कुञ्चिकाः = रिक्त;
                keyi->overlays_alloc = 0;

                कुञ्चिका->overlays_inline = false;
                कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
            }
        } अञ्ञथा {
            कुञ्चिका->overlay_कुञ्चिका = keyi->overlay_कुञ्चिका;
            कुञ्चिका->overlays_inline = true;
            कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
        }
    }

    पटिनिवत्त true;
}

स्थिर bool
CopyModMapDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;

    सचे (!entry->have_चिह्नम्) {
        कुञ्चिका = Xkb_कुञ्चिका_By_Name(keymap, entry->u.कुञ्चिका_Name, true);
        सचे (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    कुञ्चिका_Name_Text(info->ctx, entry->u.कुञ्चिका_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            पटिनिवत्त false;
        }
    }
    अञ्ञथा {
        कुञ्चिका = Find_कुञ्चिका_For_चिह्नम्(keymap, entry->u.कुञ्चिका_Sym);
        सचे (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_चिह्नम्,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.कुञ्चिका_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            पटिनिवत्त false;
        }
    }

    /* Skip modMap None */
    सचे (entry->mods) {
        कुञ्चिका->modmap |= entry->mods;
        entry->कुञ्चिका_Code = कुञ्चिका->keycode;
    }

    पटिनिवत्त true;
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
        सचे (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_गणनम्++;

    सचे (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
            सचे (कुञ्चिका->name == XKB_ATOM_NONE)
                अनुवर्त;

            सचे (कुञ्चिका->अङ्कः_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        सचे (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_गणनम्++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अङ्कः_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    पटिनिवत्त true;
}

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    सचे (file !=रिक्त)
        HandleSymbolsFile(&info, file);

    सचे (info.error_गणनम् != 0)
        गच्छ err_info;

    सचे (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearSymbolsInfo(&info);
    पटिनिवत्त true;

err_info:
    ClearSymbolsInfo(&info);
    पटिनिवत्त false;
}
