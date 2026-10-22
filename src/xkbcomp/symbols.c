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

गणना किल्ली_repeat {
    किल्ली_REPEAT_UNDEFINED = 0,
    किल्ली_REPEAT_YES,
    किल्ली_REPEAT_NO,
    किल्ली_REPEAT_अंक_ENTRIES,
};
गणना { किल्ली_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(किल्ली_REPEAT_अंक_ENTRIES - 1 <=
              (1 << (किल्ली_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

गणना group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

गणना किल्ली_field {
    किल्ली_FIELD_NONE = 0,
    किल्ली_FIELD_REPEAT = (1 << 0),
    किल्ली_FIELD_DEFAULT_TYPE = (1 << 1),
    किल्ली_FIELD_GROUPINFO = (1 << 2),
    किल्ली_FIELD_VMODMAP = (1 << 3),
    किल्ली_FIELD_OVERLAY = (1 << 4),
    किल्ली_FIELD_ALL = किल्ली_FIELD_REPEAT | किल्ली_FIELD_DEFAULT_TYPE
                  | किल्ली_FIELD_GROUPINFO | किल्ली_FIELD_VMODMAP | किल्ली_FIELD_OVERLAY,
};
गणना { किल्ली_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(किल्ली_FIELD_ALL <= (1 << (किल्ली_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

प्रकारव्याख्या संरचना {
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
static_assert((अचिन्हित)KEYINFO_DEFINED_BIT_FIELD >=
              (अचिन्हित)किल्ली_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_MERGE_BIT_FIELD >=
              (अचिन्हित)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_REPEAT_BIT_FIELD >=
              (अचिन्हित)किल्ली_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

प्रकारव्याख्या संरचना {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_संख्या;
    darray(GroupInfo) groups;
    गणना xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    गणना किल्ली_field defined:KEYINFO_DEFINED_BIT_FIELD;
    गणना merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    गणना किल्ली_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    संघ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        स्थिरांक संरचना xkb_किल्ली *overlay_किल्ली;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        स्थिरांक संरचना xkb_किल्ली **overlays_किल्ल्या;
    };
} किल्ली_Info;

स्थिर शून्य
StealLevelInfo(संरचना xkb_level *into, संरचना xkb_level *from)
{
    clear_level(into);
    जर (from->अंक_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अन्यथा {
        into->s.sym = from->s.sym;
    }
    into->अंक_syms = from->अंक_syms;
    from->अंक_syms = 0;

    जर (from->अंक_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अन्यथा {
        into->a.action = from->a.action;
    }
    into->अंक_actions = from->अंक_actions;
    from->अंक_actions = 0;
}

स्थिर शून्य
InitGroupInfo(GroupInfo *groupi)
{
    स_रखो(groupi, 0, आकार(*groupi));
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
CopyGroupInfo(GroupInfo *to, स्थिरांक GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    साठी (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        जर (darray_item(from->levels, j).अंक_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).अंक_syms,
                       आकार(xkb_keysym_t));
        }
        जर (darray_item(from->levels, j).अंक_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).अंक_actions,
                       आकार(संघ xkb_action));
        }
    }
}

स्थिर शून्य
Init_किल्ली_Info(संरचना xkb_context *ctx, किल्ली_Info *keyi)
{
    स_रखो(keyi, 0, आकार(*keyi));
    static_assert(!DEFAULT_किल्ली_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_किल्ली_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_किल्ली = रिक्त;
}

स्थिर शून्य
Clear_किल्ली_Info(किल्ली_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    जर (keyi->overlays_alloc)
        मुक्त(keyi->overlays_किल्ल्या);
}

/***====================================================================***/

प्रकारव्याख्या संरचना {
    अक्षर *name;         /* e.g. pc+us+inet(evdev) */
    पूर्णांक error_मोजणी;
    अचिन्हित पूर्णांक include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(किल्ली_Info) किल्ल्या;
    किल्ली_Info default_किल्ली;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    संरचना xkb_mod_set mods;

    संरचना xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    स्थिरांक संरचना xkb_keymap_info *keymap_info;
} SymbolsInfo;

स्थिर शून्य
InitSymbolsInfo(SymbolsInfo *info, स्थिरांक संरचना xkb_keymap_info *keymap_info,
                अचिन्हित पूर्णांक include_depth, स्थिरांक संरचना xkb_mod_set *mods)
{
    स_रखो(info, 0, आकार(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_किल्ली_Info(keymap_info->keymap.ctx, &info->default_किल्ली);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

स्थिर शून्य
ClearSymbolsInfo(SymbolsInfo *info)
{
    किल्ली_Info *keyi;
    मुक्त(info->name);
    darray_foreach(keyi, info->किल्ल्या)
        Clear_किल्ली_Info(keyi);
    darray_free(info->किल्ल्या);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_किल्ली_Info(&info->default_किल्ली);
}

स्थिर स्थिरांक अक्षर *
किल्ली_Info_Text(SymbolsInfo *info, किल्ली_Info *keyi)
{
    परत किल्ली_Name_Text(info->ctx, keyi->name);
}

स्थिर bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t किल्ली_name)
{
    /* First find the type of the merged group. */
    जर (into->type != from->type) {
        जर (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अन्यथा जर (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अन्यथा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            जर (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, किल्ली_Name_Text(info->ctx, किल्ली_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    जर (darray_empty(from->levels)) {
        InitGroupInfo(from);
        परत true;
    }

    जर (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        परत true;
    }

    /* Merge the actions and syms. */
    स्थिरांक darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_मोजणी = 0;
    darray_size_t from_Actions_मोजणी = 0;
    साठी (darray_size_t i = 0; i < levels_in_both; i++) {
        संरचना xkb_level* स्थिरांक intoLevel = &darray_item(into->levels, i);
        संरचना xkb_level* स्थिरांक fromLevel = &darray_item(from->levels, i);

        स्थिरांक bool fromHasNoKeysym = fromLevel->अंक_syms == 0;
        स्थिरांक bool fromHasNoAction = fromLevel->अंक_actions == 0;
        जर (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            सुरूठेवा;
        }

        स्थिरांक bool intoHasNoKeysym = intoLevel->अंक_syms == 0;
        स्थिरांक bool intoHasNoAction = intoLevel->अंक_actions == 0;
        जर (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_मोजणी++;
            from_Actions_मोजणी++;
            सुरूठेवा;
        }

        /* Possible level conflict */
        प्रतिज्ञा(intoLevel->अंक_syms > 0 || intoLevel->अंक_actions > 0);
        प्रतिज्ञा(fromLevel->अंक_syms > 0 || fromLevel->अंक_actions > 0);

        /* Handle keysyms */
        जर (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            जर (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_चिन्ह,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, किल्ली_Name_Text(info->ctx, किल्ली_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            जर (fromHasNoKeysym) {
                /* No keysym to copy */
            } अन्यथा जर (clobber) {
                /* Override: copy any defined keysym from `from` */
                जर (unlikely(fromLevel->अंक_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    जर (unlikely(intoLevel->अंक_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अंक_syms = fromLevel->अंक_syms;
                    fromLevel->अंक_syms = 0;
                    from_Keysyms_मोजणी++;
                } अन्यथा जर (fromLevel->s.sym != XKB_किल्ली_No_चिन्ह) {
                    /* Single defined keysym */
                    जर (unlikely(intoLevel->अंक_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अंक_syms = 1;
                    from_Keysyms_मोजणी++;
                }
            } अन्यथा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                जर (unlikely(intoLevel->अंक_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा जर (intoLevel->s.sym == XKB_किल्ली_No_चिन्ह) {
                    /* Single undefined keysym */
                    जर (unlikely(fromLevel->अंक_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अंक_syms = fromLevel->अंक_syms;
                        fromLevel->अंक_syms = 0;
                    } अन्यथा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अंक_syms = 1;
                    }
                    from_Keysyms_मोजणी++;
                }
            }
        }

        /* Handle actions */
        जर (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            जर (report && !(intoHasNoAction || fromHasNoAction)) {
                जर (unlikely(intoLevel->अंक_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_किल्ली_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, किल्ली_Name_Text(info->ctx, किल्ली_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अन्यथा {
                    संघ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                किल्ली_Name_Text(info->ctx, किल्ली_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            जर (fromHasNoAction) {
                /* No action to copy */
            } अन्यथा जर (clobber) {
                /* Override: copy any defined action from `from` */
                जर (unlikely(fromLevel->अंक_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    जर (unlikely(intoLevel->अंक_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अंक_actions = fromLevel->अंक_actions;
                    fromLevel->अंक_actions = 0;
                    from_Actions_मोजणी++;
                } अन्यथा जर (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    जर (unlikely(intoLevel->अंक_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अंक_actions = 1;
                    from_Actions_मोजणी++;
                }
            } अन्यथा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                जर (unlikely(intoLevel->अंक_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा जर (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    जर (unlikely(fromLevel->अंक_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अंक_actions = fromLevel->अंक_actions;
                        fromLevel->अंक_actions = 0;
                    } अन्यथा {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->अंक_actions = 1;
                    }
                    from_Actions_मोजणी++;
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
        level->अंक_syms = 0;
        level->अंक_actions = 0;
        from_Keysyms_मोजणी++;
        from_Actions_मोजणी++;
    }
    जर (from_Keysyms_मोजणी) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        जर (from_Keysyms_मोजणी == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    जर (from_Actions_मोजणी) {
        /* Reset defined actions field if we used no action from `into` */
        जर (from_Actions_मोजणी == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    परत true;
}

स्थिर bool
Use_New_किल्ली_Field(गणना किल्ली_field field, गणना किल्ली_field old, गणना किल्ली_field new,
               bool clobber, bool report, गणना किल्ली_field *collide)
{
    जर (!(old & field))
        परत (new & field);

    जर (new & field) {
        जर (report)
            *collide |= field;

        परत clobber;
    }

    परत false;
}

स्थिर bool
overlays_get(स्थिरांक किल्ली_Info *info, xkb_overlay_index_t bit,
             स्थिरांक संरचना xkb_किल्ली **किल्ली_out)
{
    जर (bit >= (xkb_overlay_index_t)(आकार(xkb_overlay_mask_t) * अक्षर_बिट))
        परत false;
    स्थिरांक xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    जर (!(info->overlays & mask)) परत false;
    जर (किल्ली_out) {
        जर (!info->overlays_alloc) {
            /* Stored inline */
            *किल्ली_out = info->overlay_किल्ली;
        } अन्यथा {
            /* Stored in sparse array */
            स्थिरांक xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            स्थिरांक xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(खालचा);
            *किल्ली_out = info->overlays_किल्ल्या[index];
        }
    }
    परत true;
}

स्थिर bool
overlays_घाला(किल्ली_Info *keyi, xkb_overlay_index_t bit,
                स्थिरांक संरचना xkb_किल्ली *किल्ली)
{
    जर (bit >= (xkb_overlay_index_t)(आकार(xkb_overlay_mask_t) * अक्षर_बिट))
        परत false;

    स्थिरांक xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    जर ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        जर (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_किल्ली = किल्ली;
            जर (!किल्ली)
                keyi->overlays_clear = true;
        } अन्यथा {
            /* Stored in sparse array */
            स्थिरांक xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            स्थिरांक xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(खालचा);
            keyi->overlays_किल्ल्या[index] = किल्ली;
        }
        परत true;
    }

    /* Insert new element */
    जर (!keyi->overlays || (keyi->overlays_clear && !किल्ली)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_किल्ली = किल्ली;
        keyi->overlays |= mask;
        keyi->overlays_clear = !किल्ली;
    } अन्यथा जर (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        स्थिरांक xkb_overlay_mask_t overlays = keyi->overlays | mask;
        स्थिरांक xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        स्थिरांक संरचना xkb_किल्ली ** स्थिरांक tmp = सुस्मृति(alloc, आकार(*tmp));
        जर (!tmp) परत false;

        /* Copy previous entries to the buffer */
        जर (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            स्थिरांक xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            स्थिरांक xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(खालचा);
            tmp[idx] = keyi->overlay_किल्ली;
        } अन्यथा {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        स्थिरांक xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        स्थिरांक xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(खालचा);
        tmp[idx] = किल्ली;

        keyi->overlays_किल्ल्या = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } अन्यथा {
        /* Allocated items: grow heap and shift if relevant */
        स्थिरांक xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        स्थिरांक xkb_overlay_index_t मोजणी =
            (xkb_overlay_index_t)popcount32(overlays);
        जर (मोजणी > keyi->overlays_alloc) {
            स्थिरांक xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(मोजणी);
            प्रतिज्ञा(alloc >= मोजणी);
            स्थिरांक संरचना xkb_किल्ली ** स्थिरांक tmp = पुनः_स्मृति(
                keyi->overlays_किल्ल्या,
                (माप_प्रकार)alloc * आकार(*keyi->overlays_किल्ल्या)
            );
            जर (!tmp) परत false;
            keyi->overlays_किल्ल्या = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        स्थिरांक xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        स्थिरांक xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(खालचा);
        जर (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        जर (index < मोजणी - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            प्रतिज्ञा(index + 1 < keyi->overlays_alloc);
            स_हटाओ(keyi->overlays_किल्ल्या + index + 1, keyi->overlays_किल्ल्या + index,
                    (माप_प्रकार)(मोजणी - 1u - index) * आकार(स्थिरांक संरचना xkb_किल्ली *));
        }
        keyi->overlays_किल्ल्या[index] = किल्ली;
        keyi->overlays = overlays;
    }

    परत true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
स्थिर bool
merge_overlays(SymbolsInfo *info, किल्ली_Info *into, किल्ली_Info *from,
               bool clobber, bool report, गणना किल्ली_field *collide)
{
    जर (!(from->defined & किल्ली_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अन्यथा जर (!(into->defined & किल्ली_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_किल्ल्या = from->overlays_किल्ल्या;
        from->overlays_किल्ल्या = रिक्त;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= किल्ली_FIELD_OVERLAY;
    } अन्यथा जर (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अन्यथा {
        जर (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            स्थिरांक xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            स्थिरांक xkb_overlay_index_t मोजणी =
                (xkb_overlay_index_t)popcount32(result_mask);

            जर (मोजणी == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            किल्ली_Info *dest = into;
            किल्ली_Info *src = from;
            जर (into->overlays_alloc >= मोजणी &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अन्यथा जर (from->overlays_alloc >= मोजणी) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अन्यथा जर (मोजणी > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                जर (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                जर (!dest->overlays_alloc) {
                    स्थिरांक संरचना xkb_किल्ली **tmp = सुस्मृति(मोजणी, आकार(*tmp));
                    जर (!tmp)
                        परत false;
                    जर (dest->overlay_किल्ली)
                        tmp[0] = dest->overlay_किल्ली;
                    dest->overlays_किल्ल्या = tmp;
                    dest->overlays_alloc = मोजणी;
                } अन्यथा {
                    स्थिरांक संरचना xkb_किल्ली **tmp =
                        पुनः_स्मृति(dest->overlays_किल्ल्या,
                                मोजणी * आकार(*dest->overlays_किल्ल्या));
                    जर (!tmp)
                        परत false;
                    dest->overlays_किल्ल्या = tmp;
                    dest->overlays_alloc = मोजणी;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            स्थिरांक संरचना xkb_किल्ली **src_किल्ल्या = (src->overlays_clear)
                ? रिक्त
                : (src->overlays_alloc)
                    ? src->overlays_किल्ल्या
                    : &src->overlay_किल्ली;
            पर्यंत (remaining) {
                /* isolate lowest set bit */
                स्थिरांक xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                /* get its index */
                स्थिरांक xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                जर (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                स्थिरांक संरचना xkb_किल्ली *src_किल्ली = (src_किल्ल्या)
                    ? *(src_किल्ल्या++)
                    : रिक्त;

                /* check dest */
                स्थिरांक संरचना xkb_किल्ली *dest_किल्ली = रिक्त;
                स्थिरांक bool conflict = overlays_get(dest, bit, &dest_किल्ली);
                जर (conflict) {
                    जर (dest_किल्ली == src_किल्ली)
                       सुरूठेवा;
                    अन्यथा जर (report)
                        *collide |= किल्ली_FIELD_OVERLAY;
                }

                जर ((!conflict || clobber) && !overlays_घाला(dest, bit, src_किल्ली))
                    परत false;
            }

            जर (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                जर (into->overlays_alloc)
                    मुक्त(into->overlays_किल्ल्या);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                जर (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_किल्ल्या = from->overlays_किल्ल्या;
                    from->overlays_किल्ल्या = रिक्त;
                    from->overlays_alloc = 0;
                } अन्यथा {
                    into->overlay_किल्ली = from->overlay_किल्ली;
                }
            }
        } अन्यथा {
            /* Disjoint overlays */

            जर (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_किल्ली == from->overlay_किल्ली) {
                /* Indentical: do nothing */
                परत true;
            }
            जर (!(into->overlays & from->overlays)) {
                जर (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_किल्ली = from->overlay_किल्ली;
                    परत true;
                } अन्यथा जर (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    परत true;
                }
            }
            /* Conflict */
            जर (report)
                *collide |= किल्ली_FIELD_OVERLAY;
            जर (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_किल्ली = from->overlay_किल्ली;
            }
        }
    }
    परत true;
}

स्थिर bool
Merge_किल्ल्या(SymbolsInfo *info, किल्ली_Info *into, किल्ली_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    गणना किल्ली_field collide = 0;
    स्थिरांक पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिरांक bool clobber = (from->merge != MERGE_AUGMENT);
    स्थिरांक bool report = (same_file && verbosity > 0) || verbosity > 9;

    जर (from->merge == MERGE_REPLACE) {
        Clear_किल्ली_Info(into);
        *into = *from;
        Init_किल्ली_Info(info->ctx, from);
        परत true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    साठी (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    साठी (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    जर (Use_New_किल्ली_Field(किल्ली_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= किल्ली_FIELD_VMODMAP;
    }
    जर (Use_New_किल्ली_Field(किल्ली_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= किल्ली_FIELD_REPEAT;
    }
    जर (Use_New_किल्ली_Field(किल्ली_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= किल्ली_FIELD_DEFAULT_TYPE;
    }
    जर (Use_New_किल्ली_Field(किल्ली_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_संख्या = from->out_of_range_group_संख्या;
        into->defined |= किल्ली_FIELD_GROUPINFO;
    }

    जर (!merge_overlays(info, into, from, clobber, report, &collide))
        परत false;

    जर (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 किल्ली_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_किल्ली_Info(from);
    Init_किल्ली_Info(info->ctx, from);
    परत true;
}

स्थिर xkb_atom_t
Xkb_Resolve_किल्ली_Alias(स्थिरांक संरचना xkb_keymap *keymap, xkb_atom_t name)
{
    जर (name < keymap->अंक_किल्ली_names) {
        स्थिरांक KeycodeMatch match = keymap->किल्ली_names[name];
        जर (match.found && match.is_alias) {
            परत match.alias.real;
        }
    }
    परत name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
स्थिर bool
Add_किल्ली_Symbols(SymbolsInfo *info, किल्ली_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_किल्ली_Alias(&info->keymap_info->keymap, keyi->name);

    किल्ली_Info *iter;
    darray_foreach(iter, info->किल्ल्या)
        जर (iter->name == keyi->name)
            परत Merge_किल्ल्या(info, iter, keyi, same_file);

    darray_append(info->किल्ल्या, *keyi);
    Init_किल्ली_Info(info->ctx, keyi);
    परत true;
}

स्थिर bool
AddModMapEntry(SymbolsInfo *info, स्थिरांक ModMapEntry *new)
{
    स्थिरांक bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        जर ((new->have_चिन्ह != old->have_चिन्ह) ||
            (new->have_चिन्ह && new->u.किल्ली_Sym != old->u.किल्ली_Sym) ||
            (!new->have_चिन्ह && new->u.किल्ली_Name != old->u.किल्ली_Name))
            सुरूठेवा;

        जर (new->mods == old->mods)
            परत true;

        स्थिरांक xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        स्थिरांक xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        जर (new->have_चिन्ह) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.किल्ली_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } अन्यथा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    किल्ली_Name_Text(info->ctx, new->u.किल्ली_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        परत true;
    }

    darray_append(info->modmaps, *new);

    परत true;
}

/***====================================================================***/

स्थिर शून्य
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     गणना merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    जर (from->error_मोजणी > 0) {
        into->error_मोजणी += from->error_मोजणी;
        परत;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    जर (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    साठी (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        जर (!darray_item(from->group_names, i))
            सुरूठेवा;

        जर (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            सुरूठेवा;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    जर (darray_empty(into->किल्ल्या)) {
        into->किल्ल्या = from->किल्ल्या;
        darray_init(from->किल्ल्या);
    }
    अन्यथा {
        किल्ली_Info *keyi;
        darray_foreach(keyi, from->किल्ल्या) {
            keyi->merge = merge;
            जर (!Add_किल्ली_Symbols(into, keyi, false))
                into->error_मोजणी++;
        }
    }

    जर (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अन्यथा {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            जर (!AddModMapEntry(into, mm))
                into->error_मोजणी++;
        }
    }
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    जर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_मोजणी += 10;
        परत false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    स्थिरांक संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    साठी (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, आकार(path));
        जर (!file) {
            info->error_मोजणी += 10;
            ClearSymbolsInfo(&included);
            परत false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        जर (stmt->modifier) {
            स्थिरांक पूर्णांक मोजणी = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            जर (मोजणी <= 0 || *(stmt->modifier + मोजणी) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } अन्यथा {
                next_incl.explicit_group--;
            }
        }
        अन्यथा जर (info->keymap_info->keymap.अंक_groups != 0 &&
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
        अन्यथा {
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

    परत (info->error_मोजणी == 0);
}

स्थिर bool
GetGroupIndex(SymbolsInfo *info, किल्ली_Info *keyi, अभिव्यक्ती_Def *arrayNdx,
              गणना group_field field, xkb_layout_index_t *ndx_rtrn)
{
    प्रतिज्ञा (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    स्थिरांक अक्षर *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    जर (arrayNdx == रिक्त) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            जर (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                परत true;
            }
        }

        जर (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, किल्ली_Info_Text(info, keyi), info->max_groups, name);
            परत false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        परत true;
    }

    जर (अभिव्यक्ती_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, किल्ली_Info_Text(info, keyi));
        परत false;
    }

    (*ndx_rtrn)--;
    जर (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    परत true;
}

स्थिर bool
Add_Symbols_To_किल्ली(SymbolsInfo *info, किल्ली_Info *keyi, अभिव्यक्ती_Def *arrayNdx,
                अभिव्यक्ती_Def *मूल्य)
{
    xkb_layout_index_t ndx = 0;

    जर (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        परत false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    जर (मूल्य->common.type == STMT_अभिव्यक्ती_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        परत true;
    }

    जर (मूल्य->common.type != STMT_अभिव्यक्ती_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(मूल्य->common.type), ndx + 1,
                किल्ली_Info_Text(info, keyi));
        परत false;
    }

    जर (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_किल्ली_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                किल्ली_Info_Text(info, keyi), ndx + 1);
        परत false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    साठी (अभिव्यक्ती_Keysym_List *keysymList = (अभिव्यक्ती_Keysym_List *) मूल्य;
         keysymList;
         keysymList = (अभिव्यक्ती_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        जर (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    जर (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    जर (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    साठी (अभिव्यक्ती_Keysym_List *keysymList = (अभिव्यक्ती_Keysym_List *) मूल्य;
         keysymList && level < nLevels;
         keysymList = (अभिव्यक्ती_Keysym_List *) keysymList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अंक_syms == 0);

        जर (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    किल्ली_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            परत false;
        }

        leveli->अंक_syms = (xkb_keysym_मोजणी_t) darray_size(keysymList->syms);
        निवड (leveli->अंक_syms) {
        स्थिती 0:
            leveli->s.sym = XKB_किल्ली_No_चिन्ह;
            थांबा;
        स्थिती 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            प्रतिज्ञा(leveli->s.sym != XKB_किल्ली_No_चिन्ह);
            थांबा;
        पूर्वनिर्धारित:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, रिक्त);
#ifndef न_संशोधन
            /* Canonical list: all NoSymbol were dropped */
            साठी (xkb_keysym_मोजणी_t k = 0; k < leveli->अंक_syms; k++)
                प्रतिज्ञा(leveli->s.syms[k] != XKB_किल्ली_No_चिन्ह);
#endif
        }
    }

    परत true;
}

स्थिर bool
Add_Actions_To_किल्ली(SymbolsInfo *info, किल्ली_Info *keyi, अभिव्यक्ती_Def *arrayNdx,
                अभिव्यक्ती_Def *मूल्य)
{
    xkb_layout_index_t ndx = 0;

    जर (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        परत false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    जर (मूल्य->common.type == STMT_अभिव्यक्ती_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        परत true;
    }

    जर (मूल्य->common.type != STMT_अभिव्यक्ती_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मूल्य->common.type, ndx, किल्ली_Info_Text(info, keyi));
        परत false;
    }

    जर (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 किल्ली_Info_Text(info, keyi), ndx);
        परत false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    साठी (ParseCommon *p = &मूल्य->common; p; p = p->next)
        nLevels++;
    जर (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    साठी (अभिव्यक्ती_Action_List *actionList = (अभिव्यक्ती_Action_List *) मूल्य;
         actionList;
         actionList = (अभिव्यक्ती_Action_List *) actionList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अंक_actions == 0);

        अचिन्हित पूर्णांक अंक_actions = 0;
        साठी (अभिव्यक्ती_Def *act = actionList->actions;
             act; act = (अभिव्यक्ती_Def *) act->common.next)
            अंक_actions++;

        जर (unlikely(अंक_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    किल्ली_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, अंक_actions);
            परत false;
        }

        /* Parse actions and add only defined actions */
        darray(संघ xkb_action) actions = darray_new();
        साठी (अभिव्यक्ती_Def *act = actionList->actions;
             act; act = (अभिव्यक्ती_Def *) act->common.next) {
            संघ xkb_action toAct = { 0 };
            स्थिरांक गणना xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            जर (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        किल्ली_Info_Text(info, keyi), ndx + 1, level + 1);
                जर (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    परत false;
                } अन्यथा {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            जर (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अन्यथा जर (likely(अंक_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अंक_actions = 1;
                leveli->a.action = toAct;
                जा next_level;
            } अन्यथा {
                darray_append(actions, toAct);
            }
        }
        जर (darray_empty(actions)) {
            leveli->अंक_actions = 0;
        } अन्यथा जर (likely(darray_size(actions) > 1)) {
            leveli->अंक_actions = (xkb_action_मोजणी_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, रिक्त);
#ifndef न_संशोधन
            /* Canonical list: all NoAction() were dropped */
            साठी (xkb_action_मोजणी_t k = 0; k < leveli->अंक_actions; k++)
                प्रतिज्ञा(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } अन्यथा {
            /* Unlikely: some actions were dropped and only one remains */
            प्रतिज्ञा(अंक_actions > darray_size(actions));
            leveli->अंक_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            प्रतिज्ञा(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        जर (leveli->अंक_actions > 0 || leveli->अंक_syms > 0)
            nonEmptyLevels = level + 1;
    }

    जर (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        जर (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        अन्यथा
            darray_free(groupi->levels);
    }

    परत true;
}

स्थिर स्थिरांक LookupEntry repeatEntries[] = {
    { "true", किल्ली_REPEAT_YES },
    { "yes", किल्ली_REPEAT_YES },
    { "on", किल्ली_REPEAT_YES },
    { "false", किल्ली_REPEAT_NO },
    { "no", किल्ली_REPEAT_NO },
    { "off", किल्ली_REPEAT_NO },
    { "default", किल्ली_REPEAT_UNDEFINED },
    { रिक्त, 0 }
};

/** Handle overlay<N> = <key> entry */
स्थिर bool
अभिव्यक्ती_Resolve_Overlay_Entry(स्थिरांक संरचना xkb_keymap_info *keymap_info,
                        स्थिरांक अक्षर *field, स्थिरांक अभिव्यक्ती_Def *arrayNdx,
                        स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती, किल्ली_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        स्थिरांक संरचना xkb_किल्ली **किल्ली_rtrn)
{
    /* Overlay index */
    जर (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, किल्ली_Name_Text(keymap_info->keymap.ctx, keyi->name));
        परत false;
    }
    static_assert(आकार(xkb_overlay_index_t) == आकार(uint8_t), "");
    स्थिरांक माप_प्रकार prefix = आकार("overlay") - 1;
    स्थिरांक माप_प्रकार len = म_माप(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    जर (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (पूर्णांक)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, किल्ली_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        परत false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_KEYNAME_LITERAL:
        *किल्ली_rtrn = Xkb_किल्ली_By_Name(&keymap_info->keymap, अभिव्यक्ती->किल्ली_name.किल्ली_name,
                                 false);
        जर (!*किल्ली_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, अभिव्यक्ती->किल्ली_name.किल्ली_name),
                    field, किल्ली_Name_Text(keymap_info->keymap.ctx, keyi->name));
            परत false;
        }
        परत true;
    स्थिती STMT_अभिव्यक्ती_IDENT: {
        स्थिरांक अक्षर * स्थिरांक id =
            xkb_atom_text(keymap_info->keymap.ctx, अभिव्यक्ती->ident.ident);
        जर (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *किल्ली_rtrn = रिक्त;
            परत true;
        } अन्यथा जर (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *किल्ली_rtrn = रिक्त;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            परत true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मूल्य,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, किल्ली_Name_Text(keymap_info->keymap.ctx, keyi->name));
        परत false;
    }
    पूर्वनिर्धारित:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मूल्य,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_अभिव्यक्ती_KEYNAME_LITERAL),
                field, किल्ली_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;
    }
}

स्थिर bool
SetSymbolsField(SymbolsInfo *info, किल्ली_Info *keyi, स्थिरांक अक्षर *field,
                अभिव्यक्ती_Def *arrayNdx, अभिव्यक्ती_Def **मूल्य_ptr)
{
    अभिव्यक्ती_Def * restrict स्थिरांक मूल्य = *मूल्य_ptr;
    जर (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        जर (!अभिव्यक्ती_Resolve_String(info->ctx, मूल्य, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            परत false;
        }

        जर (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= किल्ली_FIELD_DEFAULT_TYPE;
        }
        अन्यथा जर (अभिव्यक्ती_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, रिक्त) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }
        अन्यथा {
            ndx--;
            जर (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                प्रतिज्ञा(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    अन्यथा जर (istreq(field, "symbols")) {
        परत Add_Symbols_To_किल्ली(info, keyi, arrayNdx, मूल्य);
    }
    अन्यथा जर (istreq(field, "actions")) {
        परत Add_Actions_To_किल्ली(info, keyi, arrayNdx, मूल्य);
    }
    अन्यथा जर (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(मूल्य->common.type),
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= किल्ली_FIELD_VMODMAP;
    }
    अन्यथा जर (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                किल्ली_Info_Text(info, keyi));
    }
    अन्यथा जर (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                किल्ली_Info_Text(info, keyi));
    }
    अन्यथा जर (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                किल्ली_Info_Text(info, keyi));
    }
    अन्यथा जर (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        स्थिरांक संरचना xkb_किल्ली *किल्ली = रिक्त;
        जर (!अभिव्यक्ती_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *मूल्य_ptr, keyi, &overlay, &किल्ली))
            परत false;
        जर (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            परत true;
        } अन्यथा जर (किल्ली && किल्ली->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, किल्ली_Info_Text(info, keyi));
        } अन्यथा {
            स्थिरांक संरचना xkb_किल्ली *prev;
            जर (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                जर (किल्ली != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             किल्ली_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? किल्ली_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (किल्ली
                                 ? किल्ली_Name_Text(info->ctx, किल्ली->name)
                                 : "none"));
                }
            } अन्यथा जर (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                जर (!overlays_घाला(keyi, overlay, किल्ली))
                    परत false;
                keyi->defined |= किल्ली_FIELD_OVERLAY;
            } अन्यथा {
                /* Handle disjoint overlays mode */
                स्थिरांक xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                जर (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    जर (किल्ली) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_किल्ली = किल्ली;
                    } अन्यथा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= किल्ली_FIELD_OVERLAY;
                } अन्यथा जर (keyi->overlays) {
                    जर (किल्ली) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                किल्ली_Info_Text(info, keyi), keyi->overlays,
                                किल्ली_Name_Text(info->ctx, keyi->overlay_किल्ली->name),
                                overlay + 1, किल्ली_Name_Text(info->ctx, किल्ली->name));
                        परत !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_मूल्य_MISMATCH);
                    } अन्यथा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अन्यथा जर (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        जर (!अभिव्यक्ती_Resolve_Enum(info->ctx, मूल्य, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }

        keyi->repeat = (गणना किल्ली_repeat) val;
        keyi->defined |= किल्ली_FIELD_REPEAT;
    }
    अन्यथा जर (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        जर (!अभिव्यक्ती_Resolve_Boolean(info->ctx, मूल्य, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= किल्ली_FIELD_GROUPINFO;
    }
    अन्यथा जर (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        जर (!अभिव्यक्ती_Resolve_Boolean(info->ctx, मूल्य, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= किल्ली_FIELD_GROUPINFO;
    }
    अन्यथा जर (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        जर (अभिव्यक्ती_Resolve_Group(info->keymap_info, मूल्य, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    किल्ली_Info_Text(info, keyi));
            परत false;
        }

        जर (pending) {
            keyi->out_of_range_pending_group = true;
            स्थिरांक darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (संरचना pending_computation) {
                    .अभिव्यक्ती = *मूल्य_ptr,
                    .computed = false,
                    .मूल्य = 0,
                }
            );
            *मूल्य_ptr = रिक्त;
            static_assert(आकार(keyi->out_of_range_group_संख्या) ==
                          आकार(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_संख्या = pending_index;
        } अन्यथा {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_संख्या = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= किल्ली_FIELD_GROUPINFO;
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        परत !(info->keymap_info->strict & PARSER_NO_UNKNOWN_किल्ली_FIELDS);
    }

    परत true;
}

स्थिर bool
SetGroupName(SymbolsInfo *info, अभिव्यक्ती_Def *arrayNdx, अभिव्यक्ती_Def *मूल्य,
             गणना merge_mode merge)
{
    जर (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        परत false;
    }

    xkb_layout_index_t group = 0;
    जर (अभिव्यक्ती_Resolve_Group(info->keymap_info, arrayNdx, false, &group, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        परत false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    जर (!अभिव्यक्ती_Resolve_String(info->ctx, मूल्य, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        परत false;
    }

    xkb_layout_index_t group_to_use;
    जर (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    अन्यथा जर (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    अन्यथा {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        परत false;
    }

    जर (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        प्रतिज्ञा(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } अन्यथा {
        स्थिरांक xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        जर (old_name != XKB_ATOM_NONE && old_name != name) {
            स्थिरांक bool replace = (merge != MERGE_AUGMENT);
            स्थिरांक xkb_atom_t use    = (replace ? name : old_name);
            स्थिरांक xkb_atom_t ignore = (replace ? old_name : name);
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

    परत true;
}

स्थिर bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    स्थिरांक अक्षर *elem, *field;
    अभिव्यक्ती_Def *arrayNdx;
    bool ret;

    जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        परत false;

    जर (elem && istreq(elem, "key")) {
        किल्ली_Info temp = {0};
        Init_किल्ली_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->मूल्य) &&
              Merge_किल्ल्या(info, &info->default_किल्ली, &temp, true);
        Clear_किल्ली_Info(&temp);
    }
    अन्यथा जर (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->मूल्य, stmt->merge);
    }
    अन्यथा जर (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा जर (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा जर (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा जर (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अन्यथा जर (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->मूल्य, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        परत !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    परत ret;
}

स्थिर bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, किल्ली_Info *keyi)
{
    bool all_valid_entries = true;

    साठी (; def; def = (VarDef *) def->common.next) {
        स्थिरांक अक्षर *field;
        अभिव्यक्ती_Def *arrayNdx;
        bool ok = true;

        जर (!def->name) {
            जर (unlikely(!def->मूल्य) || def->मूल्य->common.type != STMT_अभिव्यक्ती_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            अन्यथा
                field = "actions";
            arrayNdx = रिक्त;
        }
        अन्यथा {
            स्थिरांक अक्षर *elem;
            ok = अभिव्यक्ती_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            जर (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        जर (unlikely(!def->मूल्य)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        जर (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->मूल्य))
            all_valid_entries = false;
    }

    परत all_valid_entries;
}

स्थिर bool
SetExplicitGroup(SymbolsInfo *info, किल्ली_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    जर (info->explicit_group == XKB_LAYOUT_INVALID)
        परत true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        जर (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    जर (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, किल्ली_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    जर (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    परत true;
}

स्थिर bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    किल्ली_Info keyi;

    keyi = info->default_किल्ली;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_किल्ली.groups);
    साठी (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_किल्ली.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->किल्ली_Name;

    जर (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_किल्ली_Symbols(info, &keyi, true)) {
        परत true;
    }

    Clear_किल्ली_Info(&keyi);
    info->error_मोजणी++;
    परत false;
}

स्थिर bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    संरचना xkb_context *ctx = info->ctx;

    जर (!अभिव्यक्ती_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        परत false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    प्रतिज्ञा(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_अभिव्यक्ती_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    साठी (अभिव्यक्ती_Def *किल्ली = def->किल्ल्या; किल्ली; किल्ली = (अभिव्यक्ती_Def *) किल्ली->common.next) {
        जर (किल्ली->common.type == STMT_अभिव्यक्ती_KEYNAME_LITERAL) {
            tmp.have_चिन्ह = false;
            tmp.u.किल्ली_Name = किल्ली->किल्ली_name.किल्ली_name;
        }
        अन्यथा जर (किल्ली->common.type == STMT_अभिव्यक्ती_KEYSYM_LITERAL) {
            जर (किल्ली->keysym.keysym == XKB_किल्ली_No_चिन्ह) {
                /* Invalid keysym: ignore. Error message already printed */
                सुरूठेवा;
            } अन्यथा {
                tmp.have_चिन्ह = true;
                tmp.u.किल्ली_Sym = किल्ली->keysym.keysym;
            }
        }
        अन्यथा {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            सुरूठेवा;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    परत ok;
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    साठी (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        निवड (stmt->type) {
        स्थिती STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            थांबा;
        स्थिती STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            थांबा;
        स्थिती STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            थांबा;
        स्थिती STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            थांबा;
        स्थिती STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            थांबा;
        स्थिती STMT_UNKNOWN_DECLARATION:
        स्थिती STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            थांबा;
        पूर्वनिर्धारित:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            थांबा;
        }

        जर (!ok)
            info->error_मोजणी++;

        जर (info->error_मोजणी > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            थांबा;
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
स्थिर संरचना xkb_किल्ली *
Find_किल्ली_For_चिन्ह(संरचना xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    करा {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        करा {
            got_one_level = false;
            संरचना xkb_किल्ली *किल्ली;
            xkb_किल्ल्या_foreach(किल्ली, keymap) {
                जर (group < किल्ली->अंक_groups &&
                    level < Xkb_किल्ली_अंक_Levels(किल्ली, group)) {
                    got_one_group = got_one_level = true;
                    स्थिरांक xkb_keysym_मोजणी_t अंक_syms =
                        किल्ली->groups[group].levels[level].अंक_syms;
                    जर (अंक_syms > 1) {
                        साठी (xkb_keysym_मोजणी_t k = 0; k < अंक_syms; k++) {
                            जर (किल्ली->groups[group].levels[level].s.syms[k] == sym)
                                परत किल्ली;
                        }
                    } अन्यथा जर (अंक_syms &&
                               किल्ली->groups[group].levels[level].s.sym == sym) {
                        परत किल्ली;
                    }
                }
            }
            level++;
        } पर्यंत (got_one_level);
        group++;
    } पर्यंत (got_one_group);

    परत रिक्त;
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
    स्थिरांक xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).अंक_syms == 0 ? \
        XKB_किल्ली_No_चिन्ह : \
     darray_item(groupi->levels, level).अंक_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    जर (width == 1 || width <= 0)
        परत xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    जर (width == 2) {
        जर (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            परत xkb_atom_intern_literal(ctx, "ALPHABETIC");

        जर (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            परत xkb_atom_intern_literal(ctx, "KEYPAD");

        परत xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    जर (width <= 4) {
        जर (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_किल्ली_No_चिन्ह);

            जर (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                परत xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            परत xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        जर (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            परत xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        परत xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    परत XKB_ATOM_NONE;

#undef GET_SYM
}

स्थिर स्थिरांक संरचना xkb_किल्ली_type *
FindTypeForGroup(संरचना xkb_keymap *keymap, किल्ली_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    जर (type_name == XKB_ATOM_NONE) {
        जर (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अन्यथा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            जर (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    जर (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_किल्ली_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 किल्ली_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        जा use_default;
    }

    darray_size_t i;
    साठी (i = 0; i < keymap->अंक_types; i++)
        जर (keymap->types[i].name == type_name)
            थांबा;

    जर (i >= keymap->अंक_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_किल्ली_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 किल्ली_Name_Text(keymap->ctx, keyi->name), group + 1);
        जा use_default;
    }

    keymap->types[i].required = true;
    परत &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    परत &keymap->types[0];
}

स्थिर bool
CopySymbolsDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                       किल्ली_Info *keyi)
{
    संरचना xkb_किल्ली *किल्ली;
    GroupInfo *groupi;
    स्थिरांक GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    किल्ली = Xkb_किल्ली_By_Name(keymap, keyi->name, false);
    जर (!किल्ली) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                किल्ली_Info_Text(info, keyi));
        परत false;
    }

    /* Find the range of groups we need. */
    किल्ली->अंक_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        स्थिरांक bool has_explicit_type = (keyi->defined & किल्ली_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        जर (darray_size(groupi->levels) > 0 || has_explicit_type)
            किल्ली->अंक_groups = i + 1;
        जर (has_explicit_type)
            किल्ली->explicit |= EXPLICIT_TYPES;
    }

    जर (किल्ली->अंक_groups <= 0) {
        /* A key with no group may still have other fields defined */
        जर (keyi->defined)
            जा किल्ली_fields;
        अन्यथा
            परत false;
    }

    darray_resize(keyi->groups, किल्ली->अंक_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        जर (groupi->defined)
            सुरूठेवा;

        CopyGroupInfo(groupi, group0);
    }

    किल्ली->groups = सुस्मृति(किल्ली->अंक_groups, आकार(*किल्ली->groups));
    प्रतिज्ञा(darray_size(keyi->groups) == किल्ली->अंक_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < किल्ली->अंक_groups);
        #endif
        bool explicit_type = false;
        स्थिरांक संरचना xkb_किल्ली_type * स्थिरांक type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        जर (type->अंक_levels < darray_size(groupi->levels)) {
            संरचना xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->अंक_levels,
                    किल्ली_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->अंक_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->अंक_levels);

        किल्ली->groups[i].explicit_type = explicit_type;
        किल्ली->groups[i].type = type;
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
            निवड (leveli->अंक_syms) {
            स्थिती 0:
                leveli->upper = XKB_किल्ली_No_चिन्ह;
                थांबा;
            स्थिती 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                थांबा;
            पूर्वनिर्धारित:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                साठी (xkb_keysym_मोजणी_t k = 0; k < leveli->अंक_syms; k++) {
                    स्थिरांक xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    जर (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        थांबा;
                    }
                }
                जर (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = पुनः_स्मृति(leveli->s.syms,
                                             (माप_प्रकार) 2 * leveli->अंक_syms *
                                             आकार(*leveli->s.syms));
                    जर (!leveli->s.syms)
                        परत false; /* FIXME: better handling? */
                    साठी (xkb_keysym_मोजणी_t k = 0; k < leveli->अंक_syms; k++) {
                        leveli->s.syms[leveli->अंक_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &किल्ली->groups[i].levels, रिक्त);
        जर (किल्ली->groups[i].type->अंक_levels > 1 ||
            किल्ली->groups[i].levels[0].अंक_syms > 0) {
            किल्ली->groups[i].explicit_symbols = true;
            किल्ली->explicit |= EXPLICIT_SYMBOLS;
        }
        जर (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            किल्ली->groups[i].explicit_actions = true;
            किल्ली->explicit |= EXPLICIT_INTERP;
        }
        जर (किल्ली->groups[i].explicit_type)
            किल्ली->explicit |= EXPLICIT_TYPES;
    }

    किल्ली->out_of_range_pending_group = keyi->out_of_range_pending_group;
    किल्ली->out_of_range_group_संख्या = keyi->out_of_range_group_संख्या;
    किल्ली->out_of_range_group_policy = keyi->out_of_range_group_policy;

किल्ली_fields:
    जर (keyi->defined & किल्ली_FIELD_VMODMAP) {
        किल्ली->vmodmap = keyi->vmodmap;
        किल्ली->explicit |= EXPLICIT_VMODMAP;
    }

    जर (keyi->repeat != किल्ली_REPEAT_UNDEFINED) {
        किल्ली->repeats = (keyi->repeat == किल्ली_REPEAT_YES);
        किल्ली->explicit |= EXPLICIT_REPEAT;
    }

    जर ((keyi->defined & किल्ली_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        किल्ली->overlays = keyi->overlays;
        जर (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = किल्ली->overlays;
            स्थिरांक संरचना xkb_किल्ली **overlays_किल्ल्या = keyi->overlays_किल्ल्या;
            पर्यंत (remaining) {
                /* isolate lowest set bit */
                स्थिरांक xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                जर (*overlays_किल्ल्या) {
                    overlays_किल्ल्या++;
                } अन्यथा {
                    /* drop current null value (overlayN=none) */
                    किल्ली->overlays &= ~lsb;
                }
            }

            जर (किल्ली->overlays) {
                /* Steal */
                किल्ली->overlays_किल्ल्या = keyi->overlays_किल्ल्या;
                keyi->overlays_किल्ल्या = रिक्त;
                keyi->overlays_alloc = 0;

                किल्ली->overlays_inline = false;
                किल्ली->explicit |= EXPLICIT_OVERLAY;
            }
        } अन्यथा {
            किल्ली->overlay_किल्ली = keyi->overlay_किल्ली;
            किल्ली->overlays_inline = true;
            किल्ली->explicit |= EXPLICIT_OVERLAY;
        }
    }

    परत true;
}

स्थिर bool
CopyModMapDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    संरचना xkb_किल्ली *किल्ली;

    जर (!entry->have_चिन्ह) {
        किल्ली = Xkb_किल्ली_By_Name(keymap, entry->u.किल्ली_Name, true);
        जर (!किल्ली) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    किल्ली_Name_Text(info->ctx, entry->u.किल्ली_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            परत false;
        }
    }
    अन्यथा {
        किल्ली = Find_किल्ली_For_चिन्ह(keymap, entry->u.किल्ली_Sym);
        जर (!किल्ली) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_चिन्ह,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.किल्ली_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            परत false;
        }
    }

    /* Skip modMap None */
    जर (entry->mods) {
        किल्ली->modmap |= entry->mods;
        entry->किल्ली_Code = किल्ली->keycode;
    }

    परत true;
}

स्थिर bool
CopySymbolsToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info)
{
    किल्ली_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->अंक_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, रिक्त);

    darray_foreach(keyi, info->किल्ल्या)
        जर (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_मोजणी++;

    जर (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        संरचना xkb_किल्ली *किल्ली;

        xkb_किल्ल्या_foreach(किल्ली, keymap) {
            जर (किल्ली->name == XKB_ATOM_NONE)
                सुरूठेवा;

            जर (किल्ली->अंक_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         किल्ली_Name_Text(info->ctx, किल्ली->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        जर (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_मोजणी++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अंक_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    परत true;
}

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    जर (file !=रिक्त)
        HandleSymbolsFile(&info, file);

    जर (info.error_मोजणी != 0)
        जा err_info;

    जर (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        जा err_info;

    ClearSymbolsInfo(&info);
    परत true;

err_info:
    ClearSymbolsInfo(&info);
    परत false;
}
