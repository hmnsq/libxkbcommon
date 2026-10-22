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

गणना साँचो_repeat {
    साँचो_REPEAT_UNDEFINED = 0,
    साँचो_REPEAT_YES,
    साँचो_REPEAT_NO,
    साँचो_REPEAT_अङ्क_ENTRIES,
};
गणना { साँचो_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(साँचो_REPEAT_अङ्क_ENTRIES - 1 <=
              (1 << (साँचो_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

गणना group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

गणना साँचो_field {
    साँचो_FIELD_NONE = 0,
    साँचो_FIELD_REPEAT = (1 << 0),
    साँचो_FIELD_DEFAULT_TYPE = (1 << 1),
    साँचो_FIELD_GROUPINFO = (1 << 2),
    साँचो_FIELD_VMODMAP = (1 << 3),
    साँचो_FIELD_OVERLAY = (1 << 4),
    साँचो_FIELD_ALL = साँचो_FIELD_REPEAT | साँचो_FIELD_DEFAULT_TYPE
                  | साँचो_FIELD_GROUPINFO | साँचो_FIELD_VMODMAP | साँचो_FIELD_OVERLAY,
};
गणना { साँचो_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(साँचो_FIELD_ALL <= (1 << (साँचो_FIELD_MIN_WIDTH - 1)) - 1,
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
static_assert((अचिन्हित)KEYINFO_DEFINED_BIT_FIELD >=
              (अचिन्हित)साँचो_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_MERGE_BIT_FIELD >=
              (अचिन्हित)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_REPEAT_BIT_FIELD >=
              (अचिन्हित)साँचो_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

प्रकारनिर्देश संरचना {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_सङ्ख्या;
    darray(GroupInfo) groups;
    गणना xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    गणना साँचो_field defined:KEYINFO_DEFINED_BIT_FIELD;
    गणना merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    गणना साँचो_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
        अचर संरचना xkb_साँचो *overlay_साँचो;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        अचर संरचना xkb_साँचो **overlays_साँचोहरू;
    };
} साँचो_Info;

स्थिर शून्य
StealLevelInfo(संरचना xkb_level *into, संरचना xkb_level *from)
{
    clear_level(into);
    यदि (from->अङ्क_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अन्यथा {
        into->s.sym = from->s.sym;
    }
    into->अङ्क_syms = from->अङ्क_syms;
    from->अङ्क_syms = 0;

    यदि (from->अङ्क_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अन्यथा {
        into->a.action = from->a.action;
    }
    into->अङ्क_actions = from->अङ्क_actions;
    from->अङ्क_actions = 0;
}

स्थिर शून्य
InitGroupInfo(GroupInfo *groupi)
{
    स_रखो(groupi, 0, परिमाण(*groupi));
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
CopyGroupInfo(GroupInfo *to, अचर GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    लागि (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        यदि (darray_item(from->levels, j).अङ्क_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).अङ्क_syms,
                       परिमाण(xkb_keysym_t));
        }
        यदि (darray_item(from->levels, j).अङ्क_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).अङ्क_actions,
                       परिमाण(सङ्घ xkb_action));
        }
    }
}

स्थिर शून्य
Init_साँचो_Info(संरचना xkb_context *ctx, साँचो_Info *keyi)
{
    स_रखो(keyi, 0, परिमाण(*keyi));
    static_assert(!DEFAULT_साँचो_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_साँचो_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_साँचो = रिक्त;
}

स्थिर शून्य
Clear_साँचो_Info(साँचो_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    यदि (keyi->overlays_alloc)
        मुक्त(keyi->overlays_साँचोहरू);
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    वर्ण *name;         /* e.g. pc+us+inet(evdev) */
    पूर्णाङ्क error_गन्ती;
    अचिन्हित पूर्णाङ्क include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(साँचो_Info) साँचोहरू;
    साँचो_Info default_साँचो;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    संरचना xkb_mod_set mods;

    संरचना xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    अचर संरचना xkb_keymap_info *keymap_info;
} SymbolsInfo;

स्थिर शून्य
InitSymbolsInfo(SymbolsInfo *info, अचर संरचना xkb_keymap_info *keymap_info,
                अचिन्हित पूर्णाङ्क include_depth, अचर संरचना xkb_mod_set *mods)
{
    स_रखो(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_साँचो_Info(keymap_info->keymap.ctx, &info->default_साँचो);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

स्थिर शून्य
ClearSymbolsInfo(SymbolsInfo *info)
{
    साँचो_Info *keyi;
    मुक्त(info->name);
    darray_foreach(keyi, info->साँचोहरू)
        Clear_साँचो_Info(keyi);
    darray_free(info->साँचोहरू);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_साँचो_Info(&info->default_साँचो);
}

स्थिर अचर वर्ण *
साँचो_Info_Text(SymbolsInfo *info, साँचो_Info *keyi)
{
    फर्काऊ साँचो_Name_Text(info->ctx, keyi->name);
}

स्थिर bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t साँचो_name)
{
    /* First find the type of the merged group. */
    यदि (into->type != from->type) {
        यदि (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अन्यथा यदि (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अन्यथा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            यदि (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, साँचो_Name_Text(info->ctx, साँचो_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    यदि (darray_empty(from->levels)) {
        InitGroupInfo(from);
        फर्काऊ true;
    }

    यदि (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        फर्काऊ true;
    }

    /* Merge the actions and syms. */
    अचर darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_गन्ती = 0;
    darray_size_t from_Actions_गन्ती = 0;
    लागि (darray_size_t i = 0; i < levels_in_both; i++) {
        संरचना xkb_level* अचर intoLevel = &darray_item(into->levels, i);
        संरचना xkb_level* अचर fromLevel = &darray_item(from->levels, i);

        अचर bool fromHasNoKeysym = fromLevel->अङ्क_syms == 0;
        अचर bool fromHasNoAction = fromLevel->अङ्क_actions == 0;
        यदि (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            जारी;
        }

        अचर bool intoHasNoKeysym = intoLevel->अङ्क_syms == 0;
        अचर bool intoHasNoAction = intoLevel->अङ्क_actions == 0;
        यदि (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_गन्ती++;
            from_Actions_गन्ती++;
            जारी;
        }

        /* Possible level conflict */
        प्रतिज्ञा(intoLevel->अङ्क_syms > 0 || intoLevel->अङ्क_actions > 0);
        प्रतिज्ञा(fromLevel->अङ्क_syms > 0 || fromLevel->अङ्क_actions > 0);

        /* Handle keysyms */
        यदि (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            यदि (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_चिह्न,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, साँचो_Name_Text(info->ctx, साँचो_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            यदि (fromHasNoKeysym) {
                /* No keysym to copy */
            } अन्यथा यदि (clobber) {
                /* Override: copy any defined keysym from `from` */
                यदि (unlikely(fromLevel->अङ्क_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    यदि (unlikely(intoLevel->अङ्क_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अङ्क_syms = fromLevel->अङ्क_syms;
                    fromLevel->अङ्क_syms = 0;
                    from_Keysyms_गन्ती++;
                } अन्यथा यदि (fromLevel->s.sym != XKB_साँचो_No_चिह्न) {
                    /* Single defined keysym */
                    यदि (unlikely(intoLevel->अङ्क_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अङ्क_syms = 1;
                    from_Keysyms_गन्ती++;
                }
            } अन्यथा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                यदि (unlikely(intoLevel->अङ्क_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा यदि (intoLevel->s.sym == XKB_साँचो_No_चिह्न) {
                    /* Single undefined keysym */
                    यदि (unlikely(fromLevel->अङ्क_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अङ्क_syms = fromLevel->अङ्क_syms;
                        fromLevel->अङ्क_syms = 0;
                    } अन्यथा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अङ्क_syms = 1;
                    }
                    from_Keysyms_गन्ती++;
                }
            }
        }

        /* Handle actions */
        यदि (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            यदि (report && !(intoHasNoAction || fromHasNoAction)) {
                यदि (unlikely(intoLevel->अङ्क_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_साँचो_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, साँचो_Name_Text(info->ctx, साँचो_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अन्यथा {
                    सङ्घ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                साँचो_Name_Text(info->ctx, साँचो_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            यदि (fromHasNoAction) {
                /* No action to copy */
            } अन्यथा यदि (clobber) {
                /* Override: copy any defined action from `from` */
                यदि (unlikely(fromLevel->अङ्क_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    यदि (unlikely(intoLevel->अङ्क_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अङ्क_actions = fromLevel->अङ्क_actions;
                    fromLevel->अङ्क_actions = 0;
                    from_Actions_गन्ती++;
                } अन्यथा यदि (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    यदि (unlikely(intoLevel->अङ्क_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अङ्क_actions = 1;
                    from_Actions_गन्ती++;
                }
            } अन्यथा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                यदि (unlikely(intoLevel->अङ्क_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा यदि (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    यदि (unlikely(fromLevel->अङ्क_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अङ्क_actions = fromLevel->अङ्क_actions;
                        fromLevel->अङ्क_actions = 0;
                    } अन्यथा {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->अङ्क_actions = 1;
                    }
                    from_Actions_गन्ती++;
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
        level->अङ्क_syms = 0;
        level->अङ्क_actions = 0;
        from_Keysyms_गन्ती++;
        from_Actions_गन्ती++;
    }
    यदि (from_Keysyms_गन्ती) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        यदि (from_Keysyms_गन्ती == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    यदि (from_Actions_गन्ती) {
        /* Reset defined actions field if we used no action from `into` */
        यदि (from_Actions_गन्ती == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    फर्काऊ true;
}

स्थिर bool
Use_New_साँचो_Field(गणना साँचो_field field, गणना साँचो_field old, गणना साँचो_field new,
               bool clobber, bool report, गणना साँचो_field *collide)
{
    यदि (!(old & field))
        फर्काऊ (new & field);

    यदि (new & field) {
        यदि (report)
            *collide |= field;

        फर्काऊ clobber;
    }

    फर्काऊ false;
}

स्थिर bool
overlays_get(अचर साँचो_Info *info, xkb_overlay_index_t bit,
             अचर संरचना xkb_साँचो **साँचो_out)
{
    यदि (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * अक्षर_बिट))
        फर्काऊ false;
    अचर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    यदि (!(info->overlays & mask)) फर्काऊ false;
    यदि (साँचो_out) {
        यदि (!info->overlays_alloc) {
            /* Stored inline */
            *साँचो_out = info->overlay_साँचो;
        } अन्यथा {
            /* Stored in sparse array */
            अचर xkb_overlay_mask_t तल्लो = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            अचर xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(तल्लो);
            *साँचो_out = info->overlays_साँचोहरू[index];
        }
    }
    फर्काऊ true;
}

स्थिर bool
overlays_घुसाउ(साँचो_Info *keyi, xkb_overlay_index_t bit,
                अचर संरचना xkb_साँचो *साँचो)
{
    यदि (bit >= (xkb_overlay_index_t)(परिमाण(xkb_overlay_mask_t) * अक्षर_बिट))
        फर्काऊ false;

    अचर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    यदि ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        यदि (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_साँचो = साँचो;
            यदि (!साँचो)
                keyi->overlays_clear = true;
        } अन्यथा {
            /* Stored in sparse array */
            अचर xkb_overlay_mask_t तल्लो = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            अचर xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(तल्लो);
            keyi->overlays_साँचोहरू[index] = साँचो;
        }
        फर्काऊ true;
    }

    /* Insert new element */
    यदि (!keyi->overlays || (keyi->overlays_clear && !साँचो)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_साँचो = साँचो;
        keyi->overlays |= mask;
        keyi->overlays_clear = !साँचो;
    } अन्यथा यदि (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        अचर xkb_overlay_mask_t overlays = keyi->overlays | mask;
        अचर xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        अचर संरचना xkb_साँचो ** अचर tmp = सुस्मृति(alloc, परिमाण(*tmp));
        यदि (!tmp) फर्काऊ false;

        /* Copy previous entries to the buffer */
        यदि (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            अचर xkb_overlay_mask_t तल्लो = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            अचर xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(तल्लो);
            tmp[idx] = keyi->overlay_साँचो;
        } अन्यथा {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        अचर xkb_overlay_mask_t तल्लो = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        अचर xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(तल्लो);
        tmp[idx] = साँचो;

        keyi->overlays_साँचोहरू = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } अन्यथा {
        /* Allocated items: grow heap and shift if relevant */
        अचर xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        अचर xkb_overlay_index_t गन्ती =
            (xkb_overlay_index_t)popcount32(overlays);
        यदि (गन्ती > keyi->overlays_alloc) {
            अचर xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(गन्ती);
            प्रतिज्ञा(alloc >= गन्ती);
            अचर संरचना xkb_साँचो ** अचर tmp = पुनः_स्मृति(
                keyi->overlays_साँचोहरू,
                (माप_प्रकार)alloc * परिमाण(*keyi->overlays_साँचोहरू)
            );
            यदि (!tmp) फर्काऊ false;
            keyi->overlays_साँचोहरू = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        अचर xkb_overlay_mask_t तल्लो = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        अचर xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(तल्लो);
        यदि (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        यदि (index < गन्ती - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            प्रतिज्ञा(index + 1 < keyi->overlays_alloc);
            स_हटाओ(keyi->overlays_साँचोहरू + index + 1, keyi->overlays_साँचोहरू + index,
                    (माप_प्रकार)(गन्ती - 1u - index) * परिमाण(अचर संरचना xkb_साँचो *));
        }
        keyi->overlays_साँचोहरू[index] = साँचो;
        keyi->overlays = overlays;
    }

    फर्काऊ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
स्थिर bool
merge_overlays(SymbolsInfo *info, साँचो_Info *into, साँचो_Info *from,
               bool clobber, bool report, गणना साँचो_field *collide)
{
    यदि (!(from->defined & साँचो_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अन्यथा यदि (!(into->defined & साँचो_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_साँचोहरू = from->overlays_साँचोहरू;
        from->overlays_साँचोहरू = रिक्त;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= साँचो_FIELD_OVERLAY;
    } अन्यथा यदि (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अन्यथा {
        यदि (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            अचर xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            अचर xkb_overlay_index_t गन्ती =
                (xkb_overlay_index_t)popcount32(result_mask);

            यदि (गन्ती == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            साँचो_Info *dest = into;
            साँचो_Info *src = from;
            यदि (into->overlays_alloc >= गन्ती &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अन्यथा यदि (from->overlays_alloc >= गन्ती) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अन्यथा यदि (गन्ती > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                यदि (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                यदि (!dest->overlays_alloc) {
                    अचर संरचना xkb_साँचो **tmp = सुस्मृति(गन्ती, परिमाण(*tmp));
                    यदि (!tmp)
                        फर्काऊ false;
                    यदि (dest->overlay_साँचो)
                        tmp[0] = dest->overlay_साँचो;
                    dest->overlays_साँचोहरू = tmp;
                    dest->overlays_alloc = गन्ती;
                } अन्यथा {
                    अचर संरचना xkb_साँचो **tmp =
                        पुनः_स्मृति(dest->overlays_साँचोहरू,
                                गन्ती * परिमाण(*dest->overlays_साँचोहरू));
                    यदि (!tmp)
                        फर्काऊ false;
                    dest->overlays_साँचोहरू = tmp;
                    dest->overlays_alloc = गन्ती;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            अचर संरचना xkb_साँचो **src_साँचोहरू = (src->overlays_clear)
                ? रिक्त
                : (src->overlays_alloc)
                    ? src->overlays_साँचोहरू
                    : &src->overlay_साँचो;
            जबसम्म (remaining) {
                /* isolate lowest set bit */
                अचर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                /* get its index */
                अचर xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                यदि (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                अचर संरचना xkb_साँचो *src_साँचो = (src_साँचोहरू)
                    ? *(src_साँचोहरू++)
                    : रिक्त;

                /* check dest */
                अचर संरचना xkb_साँचो *dest_साँचो = रिक्त;
                अचर bool conflict = overlays_get(dest, bit, &dest_साँचो);
                यदि (conflict) {
                    यदि (dest_साँचो == src_साँचो)
                       जारी;
                    अन्यथा यदि (report)
                        *collide |= साँचो_FIELD_OVERLAY;
                }

                यदि ((!conflict || clobber) && !overlays_घुसाउ(dest, bit, src_साँचो))
                    फर्काऊ false;
            }

            यदि (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                यदि (into->overlays_alloc)
                    मुक्त(into->overlays_साँचोहरू);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                यदि (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_साँचोहरू = from->overlays_साँचोहरू;
                    from->overlays_साँचोहरू = रिक्त;
                    from->overlays_alloc = 0;
                } अन्यथा {
                    into->overlay_साँचो = from->overlay_साँचो;
                }
            }
        } अन्यथा {
            /* Disjoint overlays */

            यदि (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_साँचो == from->overlay_साँचो) {
                /* Indentical: do nothing */
                फर्काऊ true;
            }
            यदि (!(into->overlays & from->overlays)) {
                यदि (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_साँचो = from->overlay_साँचो;
                    फर्काऊ true;
                } अन्यथा यदि (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    फर्काऊ true;
                }
            }
            /* Conflict */
            यदि (report)
                *collide |= साँचो_FIELD_OVERLAY;
            यदि (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_साँचो = from->overlay_साँचो;
            }
        }
    }
    फर्काऊ true;
}

स्थिर bool
Merge_साँचोहरू(SymbolsInfo *info, साँचो_Info *into, साँचो_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    गणना साँचो_field collide = 0;
    अचर पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    अचर bool clobber = (from->merge != MERGE_AUGMENT);
    अचर bool report = (same_file && verbosity > 0) || verbosity > 9;

    यदि (from->merge == MERGE_REPLACE) {
        Clear_साँचो_Info(into);
        *into = *from;
        Init_साँचो_Info(info->ctx, from);
        फर्काऊ true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    लागि (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    लागि (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    यदि (Use_New_साँचो_Field(साँचो_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= साँचो_FIELD_VMODMAP;
    }
    यदि (Use_New_साँचो_Field(साँचो_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= साँचो_FIELD_REPEAT;
    }
    यदि (Use_New_साँचो_Field(साँचो_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= साँचो_FIELD_DEFAULT_TYPE;
    }
    यदि (Use_New_साँचो_Field(साँचो_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_सङ्ख्या = from->out_of_range_group_सङ्ख्या;
        into->defined |= साँचो_FIELD_GROUPINFO;
    }

    यदि (!merge_overlays(info, into, from, clobber, report, &collide))
        फर्काऊ false;

    यदि (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 साँचो_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_साँचो_Info(from);
    Init_साँचो_Info(info->ctx, from);
    फर्काऊ true;
}

स्थिर xkb_atom_t
Xkb_Resolve_साँचो_Alias(अचर संरचना xkb_keymap *keymap, xkb_atom_t name)
{
    यदि (name < keymap->अङ्क_साँचो_names) {
        अचर KeycodeMatch match = keymap->साँचो_names[name];
        यदि (match.found && match.is_alias) {
            फर्काऊ match.alias.real;
        }
    }
    फर्काऊ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
स्थिर bool
Add_साँचो_Symbols(SymbolsInfo *info, साँचो_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_साँचो_Alias(&info->keymap_info->keymap, keyi->name);

    साँचो_Info *iter;
    darray_foreach(iter, info->साँचोहरू)
        यदि (iter->name == keyi->name)
            फर्काऊ Merge_साँचोहरू(info, iter, keyi, same_file);

    darray_append(info->साँचोहरू, *keyi);
    Init_साँचो_Info(info->ctx, keyi);
    फर्काऊ true;
}

स्थिर bool
AddModMapEntry(SymbolsInfo *info, अचर ModMapEntry *new)
{
    अचर bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        यदि ((new->have_चिह्न != old->have_चिह्न) ||
            (new->have_चिह्न && new->u.साँचो_Sym != old->u.साँचो_Sym) ||
            (!new->have_चिह्न && new->u.साँचो_Name != old->u.साँचो_Name))
            जारी;

        यदि (new->mods == old->mods)
            फर्काऊ true;

        अचर xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        अचर xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        यदि (new->have_चिह्न) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.साँचो_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } अन्यथा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    साँचो_Name_Text(info->ctx, new->u.साँचो_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        फर्काऊ true;
    }

    darray_append(info->modmaps, *new);

    फर्काऊ true;
}

/***====================================================================***/

स्थिर शून्य
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     गणना merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    यदि (from->error_गन्ती > 0) {
        into->error_गन्ती += from->error_गन्ती;
        फर्काऊ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    लागि (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        यदि (!darray_item(from->group_names, i))
            जारी;

        यदि (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            जारी;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    यदि (darray_empty(into->साँचोहरू)) {
        into->साँचोहरू = from->साँचोहरू;
        darray_init(from->साँचोहरू);
    }
    अन्यथा {
        साँचो_Info *keyi;
        darray_foreach(keyi, from->साँचोहरू) {
            keyi->merge = merge;
            यदि (!Add_साँचो_Symbols(into, keyi, false))
                into->error_गन्ती++;
        }
    }

    यदि (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अन्यथा {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            यदि (!AddModMapEntry(into, mm))
                into->error_गन्ती++;
        }
    }
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    यदि (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गन्ती += 10;
        फर्काऊ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    अचर संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    लागि (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, परिमाण(path));
        यदि (!file) {
            info->error_गन्ती += 10;
            ClearSymbolsInfo(&included);
            फर्काऊ false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        यदि (stmt->modifier) {
            अचर पूर्णाङ्क गन्ती = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            यदि (गन्ती <= 0 || *(stmt->modifier + गन्ती) != '\0' ||
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
        अन्यथा यदि (info->keymap_info->keymap.अङ्क_groups != 0 &&
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

    फर्काऊ (info->error_गन्ती == 0);
}

स्थिर bool
GetGroupIndex(SymbolsInfo *info, साँचो_Info *keyi, अभिव्यक्ति_Def *arrayNdx,
              गणना group_field field, xkb_layout_index_t *ndx_rtrn)
{
    प्रतिज्ञा (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    अचर वर्ण *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    यदि (arrayNdx == रिक्त) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            यदि (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                फर्काऊ true;
            }
        }

        यदि (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, साँचो_Info_Text(info, keyi), info->max_groups, name);
            फर्काऊ false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        फर्काऊ true;
    }

    यदि (अभिव्यक्ति_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, साँचो_Info_Text(info, keyi));
        फर्काऊ false;
    }

    (*ndx_rtrn)--;
    यदि (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    फर्काऊ true;
}

स्थिर bool
Add_Symbols_To_साँचो(SymbolsInfo *info, साँचो_Info *keyi, अभिव्यक्ति_Def *arrayNdx,
                अभिव्यक्ति_Def *मान)
{
    xkb_layout_index_t ndx = 0;

    यदि (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        फर्काऊ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    यदि (मान->common.type == STMT_अभिव्यक्ति_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        फर्काऊ true;
    }

    यदि (मान->common.type != STMT_अभिव्यक्ति_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(मान->common.type), ndx + 1,
                साँचो_Info_Text(info, keyi));
        फर्काऊ false;
    }

    यदि (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_साँचो_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                साँचो_Info_Text(info, keyi), ndx + 1);
        फर्काऊ false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    लागि (अभिव्यक्ति_Keysym_List *keysymList = (अभिव्यक्ति_Keysym_List *) मान;
         keysymList;
         keysymList = (अभिव्यक्ति_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        यदि (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    यदि (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    यदि (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    लागि (अभिव्यक्ति_Keysym_List *keysymList = (अभिव्यक्ति_Keysym_List *) मान;
         keysymList && level < nLevels;
         keysymList = (अभिव्यक्ति_Keysym_List *) keysymList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्क_syms == 0);

        यदि (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    साँचो_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            फर्काऊ false;
        }

        leveli->अङ्क_syms = (xkb_keysym_गन्ती_t) darray_size(keysymList->syms);
        विकल्प (leveli->अङ्क_syms) {
        पक्ष 0:
            leveli->s.sym = XKB_साँचो_No_चिह्न;
            रोक;
        पक्ष 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            प्रतिज्ञा(leveli->s.sym != XKB_साँचो_No_चिह्न);
            रोक;
        स्वभाव:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, रिक्त);
#ifndef न_संशोधन
            /* Canonical list: all NoSymbol were dropped */
            लागि (xkb_keysym_गन्ती_t k = 0; k < leveli->अङ्क_syms; k++)
                प्रतिज्ञा(leveli->s.syms[k] != XKB_साँचो_No_चिह्न);
#endif
        }
    }

    फर्काऊ true;
}

स्थिर bool
Add_Actions_To_साँचो(SymbolsInfo *info, साँचो_Info *keyi, अभिव्यक्ति_Def *arrayNdx,
                अभिव्यक्ति_Def *मान)
{
    xkb_layout_index_t ndx = 0;

    यदि (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        फर्काऊ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    यदि (मान->common.type == STMT_अभिव्यक्ति_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        फर्काऊ true;
    }

    यदि (मान->common.type != STMT_अभिव्यक्ति_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मान->common.type, ndx, साँचो_Info_Text(info, keyi));
        फर्काऊ false;
    }

    यदि (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_साँचो_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 साँचो_Info_Text(info, keyi), ndx);
        फर्काऊ false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    लागि (ParseCommon *p = &मान->common; p; p = p->next)
        nLevels++;
    यदि (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    लागि (अभिव्यक्ति_Action_List *actionList = (अभिव्यक्ति_Action_List *) मान;
         actionList;
         actionList = (अभिव्यक्ति_Action_List *) actionList->common.next, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->levels, level);
        प्रतिज्ञा(leveli->अङ्क_actions == 0);

        अचिन्हित पूर्णाङ्क अङ्क_actions = 0;
        लागि (अभिव्यक्ति_Def *act = actionList->actions;
             act; act = (अभिव्यक्ति_Def *) act->common.next)
            अङ्क_actions++;

        यदि (unlikely(अङ्क_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    साँचो_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, अङ्क_actions);
            फर्काऊ false;
        }

        /* Parse actions and add only defined actions */
        darray(सङ्घ xkb_action) actions = darray_new();
        लागि (अभिव्यक्ति_Def *act = actionList->actions;
             act; act = (अभिव्यक्ति_Def *) act->common.next) {
            सङ्घ xkb_action toAct = { 0 };
            अचर गणना xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            यदि (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मान,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        साँचो_Info_Text(info, keyi), ndx + 1, level + 1);
                यदि (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    फर्काऊ false;
                } अन्यथा {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            यदि (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अन्यथा यदि (likely(अङ्क_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अङ्क_actions = 1;
                leveli->a.action = toAct;
                गच्छ next_level;
            } अन्यथा {
                darray_append(actions, toAct);
            }
        }
        यदि (darray_empty(actions)) {
            leveli->अङ्क_actions = 0;
        } अन्यथा यदि (likely(darray_size(actions) > 1)) {
            leveli->अङ्क_actions = (xkb_action_गन्ती_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, रिक्त);
#ifndef न_संशोधन
            /* Canonical list: all NoAction() were dropped */
            लागि (xkb_action_गन्ती_t k = 0; k < leveli->अङ्क_actions; k++)
                प्रतिज्ञा(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } अन्यथा {
            /* Unlikely: some actions were dropped and only one remains */
            प्रतिज्ञा(अङ्क_actions > darray_size(actions));
            leveli->अङ्क_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            प्रतिज्ञा(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        यदि (leveli->अङ्क_actions > 0 || leveli->अङ्क_syms > 0)
            nonEmptyLevels = level + 1;
    }

    यदि (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        यदि (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        अन्यथा
            darray_free(groupi->levels);
    }

    फर्काऊ true;
}

स्थिर अचर LookupEntry repeatEntries[] = {
    { "true", साँचो_REPEAT_YES },
    { "yes", साँचो_REPEAT_YES },
    { "on", साँचो_REPEAT_YES },
    { "false", साँचो_REPEAT_NO },
    { "no", साँचो_REPEAT_NO },
    { "off", साँचो_REPEAT_NO },
    { "default", साँचो_REPEAT_UNDEFINED },
    { रिक्त, 0 }
};

/** Handle overlay<N> = <key> entry */
स्थिर bool
अभिव्यक्ति_Resolve_Overlay_Entry(अचर संरचना xkb_keymap_info *keymap_info,
                        अचर वर्ण *field, अचर अभिव्यक्ति_Def *arrayNdx,
                        अचर अभिव्यक्ति_Def *अभिव्यक्ति, साँचो_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        अचर संरचना xkb_साँचो **साँचो_rtrn)
{
    /* Overlay index */
    यदि (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, साँचो_Name_Text(keymap_info->keymap.ctx, keyi->name));
        फर्काऊ false;
    }
    static_assert(परिमाण(xkb_overlay_index_t) == परिमाण(uint8_t), "");
    अचर माप_प्रकार prefix = परिमाण("overlay") - 1;
    अचर माप_प्रकार len = म_माप(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    यदि (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (पूर्णाङ्क)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, साँचो_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        फर्काऊ false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_KEYNAME_LITERAL:
        *साँचो_rtrn = Xkb_साँचो_By_Name(&keymap_info->keymap, अभिव्यक्ति->साँचो_name.साँचो_name,
                                 false);
        यदि (!*साँचो_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, अभिव्यक्ति->साँचो_name.साँचो_name),
                    field, साँचो_Name_Text(keymap_info->keymap.ctx, keyi->name));
            फर्काऊ false;
        }
        फर्काऊ true;
    पक्ष STMT_अभिव्यक्ति_IDENT: {
        अचर वर्ण * अचर id =
            xkb_atom_text(keymap_info->keymap.ctx, अभिव्यक्ति->ident.ident);
        यदि (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *साँचो_rtrn = रिक्त;
            फर्काऊ true;
        } अन्यथा यदि (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *साँचो_rtrn = रिक्त;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            फर्काऊ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मान,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, साँचो_Name_Text(keymap_info->keymap.ctx, keyi->name));
        फर्काऊ false;
    }
    स्वभाव:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मान,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_अभिव्यक्ति_KEYNAME_LITERAL),
                field, साँचो_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;
    }
}

स्थिर bool
SetSymbolsField(SymbolsInfo *info, साँचो_Info *keyi, अचर वर्ण *field,
                अभिव्यक्ति_Def *arrayNdx, अभिव्यक्ति_Def **मान_ptr)
{
    अभिव्यक्ति_Def * restrict अचर मान = *मान_ptr;
    यदि (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        यदि (!अभिव्यक्ति_Resolve_String(info->ctx, मान, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            फर्काऊ false;
        }

        यदि (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= साँचो_FIELD_DEFAULT_TYPE;
        }
        अन्यथा यदि (अभिव्यक्ति_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, रिक्त) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }
        अन्यथा {
            ndx--;
            यदि (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                प्रतिज्ञा(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    अन्यथा यदि (istreq(field, "symbols")) {
        फर्काऊ Add_Symbols_To_साँचो(info, keyi, arrayNdx, मान);
    }
    अन्यथा यदि (istreq(field, "actions")) {
        फर्काऊ Add_Actions_To_साँचो(info, keyi, arrayNdx, मान);
    }
    अन्यथा यदि (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        यदि (!अभिव्यक्ति_Resolve_Mod_Mask(info->ctx, मान, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(मान->common.type),
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= साँचो_FIELD_VMODMAP;
    }
    अन्यथा यदि (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                साँचो_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                साँचो_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                साँचो_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        अचर संरचना xkb_साँचो *साँचो = रिक्त;
        यदि (!अभिव्यक्ति_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *मान_ptr, keyi, &overlay, &साँचो))
            फर्काऊ false;
        यदि (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            फर्काऊ true;
        } अन्यथा यदि (साँचो && साँचो->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, साँचो_Info_Text(info, keyi));
        } अन्यथा {
            अचर संरचना xkb_साँचो *prev;
            यदि (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                यदि (साँचो != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             साँचो_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? साँचो_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (साँचो
                                 ? साँचो_Name_Text(info->ctx, साँचो->name)
                                 : "none"));
                }
            } अन्यथा यदि (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                यदि (!overlays_घुसाउ(keyi, overlay, साँचो))
                    फर्काऊ false;
                keyi->defined |= साँचो_FIELD_OVERLAY;
            } अन्यथा {
                /* Handle disjoint overlays mode */
                अचर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                यदि (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    यदि (साँचो) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_साँचो = साँचो;
                    } अन्यथा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= साँचो_FIELD_OVERLAY;
                } अन्यथा यदि (keyi->overlays) {
                    यदि (साँचो) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                साँचो_Info_Text(info, keyi), keyi->overlays,
                                साँचो_Name_Text(info->ctx, keyi->overlay_साँचो->name),
                                overlay + 1, साँचो_Name_Text(info->ctx, साँचो->name));
                        फर्काऊ !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_मान_MISMATCH);
                    } अन्यथा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अन्यथा यदि (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        यदि (!अभिव्यक्ति_Resolve_Enum(info->ctx, मान, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मान,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }

        keyi->repeat = (गणना साँचो_repeat) val;
        keyi->defined |= साँचो_FIELD_REPEAT;
    }
    अन्यथा यदि (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        यदि (!अभिव्यक्ति_Resolve_Boolean(info->ctx, मान, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मान,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= साँचो_FIELD_GROUPINFO;
    }
    अन्यथा यदि (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        यदि (!अभिव्यक्ति_Resolve_Boolean(info->ctx, मान, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मान,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= साँचो_FIELD_GROUPINFO;
    }
    अन्यथा यदि (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        यदि (अभिव्यक्ति_Resolve_Group(info->keymap_info, मान, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    साँचो_Info_Text(info, keyi));
            फर्काऊ false;
        }

        यदि (pending) {
            keyi->out_of_range_pending_group = true;
            अचर darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (संरचना pending_computation) {
                    .अभिव्यक्ति = *मान_ptr,
                    .computed = false,
                    .मान = 0,
                }
            );
            *मान_ptr = रिक्त;
            static_assert(परिमाण(keyi->out_of_range_group_सङ्ख्या) ==
                          परिमाण(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_सङ्ख्या = pending_index;
        } अन्यथा {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_सङ्ख्या = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= साँचो_FIELD_GROUPINFO;
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        फर्काऊ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_साँचो_FIELDS);
    }

    फर्काऊ true;
}

स्थिर bool
SetGroupName(SymbolsInfo *info, अभिव्यक्ति_Def *arrayNdx, अभिव्यक्ति_Def *मान,
             गणना merge_mode merge)
{
    यदि (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        फर्काऊ false;
    }

    xkb_layout_index_t group = 0;
    यदि (अभिव्यक्ति_Resolve_Group(info->keymap_info, arrayNdx, false, &group, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        फर्काऊ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    यदि (!अभिव्यक्ति_Resolve_String(info->ctx, मान, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        फर्काऊ false;
    }

    xkb_layout_index_t group_to_use;
    यदि (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    अन्यथा यदि (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    अन्यथा {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        फर्काऊ false;
    }

    यदि (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        प्रतिज्ञा(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } अन्यथा {
        अचर xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        यदि (old_name != XKB_ATOM_NONE && old_name != name) {
            अचर bool replace = (merge != MERGE_AUGMENT);
            अचर xkb_atom_t use    = (replace ? name : old_name);
            अचर xkb_atom_t ignore = (replace ? old_name : name);
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

    फर्काऊ true;
}

स्थिर bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    अचर वर्ण *elem, *field;
    अभिव्यक्ति_Def *arrayNdx;
    bool ret;

    यदि (!अभिव्यक्ति_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        फर्काऊ false;

    यदि (elem && istreq(elem, "key")) {
        साँचो_Info temp = {0};
        Init_साँचो_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->मान) &&
              Merge_साँचोहरू(info, &info->default_साँचो, &temp, true);
        Clear_साँचो_Info(&temp);
    }
    अन्यथा यदि (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->मान, stmt->merge);
    }
    अन्यथा यदि (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अन्यथा यदि (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->मान, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        फर्काऊ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    फर्काऊ ret;
}

स्थिर bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, साँचो_Info *keyi)
{
    bool all_valid_entries = true;

    लागि (; def; def = (VarDef *) def->common.next) {
        अचर वर्ण *field;
        अभिव्यक्ति_Def *arrayNdx;
        bool ok = true;

        यदि (!def->name) {
            यदि (unlikely(!def->मान) || def->मान->common.type != STMT_अभिव्यक्ति_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            अन्यथा
                field = "actions";
            arrayNdx = रिक्त;
        }
        अन्यथा {
            अचर वर्ण *elem;
            ok = अभिव्यक्ति_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            यदि (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        यदि (unlikely(!def->मान)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        यदि (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->मान))
            all_valid_entries = false;
    }

    फर्काऊ all_valid_entries;
}

स्थिर bool
SetExplicitGroup(SymbolsInfo *info, साँचो_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    यदि (info->explicit_group == XKB_LAYOUT_INVALID)
        फर्काऊ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        यदि (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    यदि (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, साँचो_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    यदि (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    फर्काऊ true;
}

स्थिर bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    साँचो_Info keyi;

    keyi = info->default_साँचो;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_साँचो.groups);
    लागि (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_साँचो.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->साँचो_Name;

    यदि (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_साँचो_Symbols(info, &keyi, true)) {
        फर्काऊ true;
    }

    Clear_साँचो_Info(&keyi);
    info->error_गन्ती++;
    फर्काऊ false;
}

स्थिर bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    संरचना xkb_context *ctx = info->ctx;

    यदि (!अभिव्यक्ति_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        फर्काऊ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    प्रतिज्ञा(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_अभिव्यक्ति_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    लागि (अभिव्यक्ति_Def *साँचो = def->साँचोहरू; साँचो; साँचो = (अभिव्यक्ति_Def *) साँचो->common.next) {
        यदि (साँचो->common.type == STMT_अभिव्यक्ति_KEYNAME_LITERAL) {
            tmp.have_चिह्न = false;
            tmp.u.साँचो_Name = साँचो->साँचो_name.साँचो_name;
        }
        अन्यथा यदि (साँचो->common.type == STMT_अभिव्यक्ति_KEYSYM_LITERAL) {
            यदि (साँचो->keysym.keysym == XKB_साँचो_No_चिह्न) {
                /* Invalid keysym: ignore. Error message already printed */
                जारी;
            } अन्यथा {
                tmp.have_चिह्न = true;
                tmp.u.साँचो_Sym = साँचो->keysym.keysym;
            }
        }
        अन्यथा {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            जारी;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    फर्काऊ ok;
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    लागि (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            रोक;
        पक्ष STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            रोक;
        पक्ष STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            रोक;
        पक्ष STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            रोक;
        पक्ष STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            रोक;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            रोक;
        स्वभाव:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            रोक;
        }

        यदि (!ok)
            info->error_गन्ती++;

        यदि (info->error_गन्ती > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            रोक;
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
स्थिर संरचना xkb_साँचो *
Find_साँचो_For_चिह्न(संरचना xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    गर {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        गर {
            got_one_level = false;
            संरचना xkb_साँचो *साँचो;
            xkb_साँचोहरू_foreach(साँचो, keymap) {
                यदि (group < साँचो->अङ्क_groups &&
                    level < Xkb_साँचो_अङ्क_Levels(साँचो, group)) {
                    got_one_group = got_one_level = true;
                    अचर xkb_keysym_गन्ती_t अङ्क_syms =
                        साँचो->groups[group].levels[level].अङ्क_syms;
                    यदि (अङ्क_syms > 1) {
                        लागि (xkb_keysym_गन्ती_t k = 0; k < अङ्क_syms; k++) {
                            यदि (साँचो->groups[group].levels[level].s.syms[k] == sym)
                                फर्काऊ साँचो;
                        }
                    } अन्यथा यदि (अङ्क_syms &&
                               साँचो->groups[group].levels[level].s.sym == sym) {
                        फर्काऊ साँचो;
                    }
                }
            }
            level++;
        } जबसम्म (got_one_level);
        group++;
    } जबसम्म (got_one_group);

    फर्काऊ रिक्त;
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
    अचर xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).अङ्क_syms == 0 ? \
        XKB_साँचो_No_चिह्न : \
     darray_item(groupi->levels, level).अङ्क_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    यदि (width == 1 || width <= 0)
        फर्काऊ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    यदि (width == 2) {
        यदि (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            फर्काऊ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        यदि (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            फर्काऊ xkb_atom_intern_literal(ctx, "KEYPAD");

        फर्काऊ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    यदि (width <= 4) {
        यदि (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_साँचो_No_चिह्न);

            यदि (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                फर्काऊ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            फर्काऊ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        यदि (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            फर्काऊ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        फर्काऊ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    फर्काऊ XKB_ATOM_NONE;

#undef GET_SYM
}

स्थिर अचर संरचना xkb_साँचो_type *
FindTypeForGroup(संरचना xkb_keymap *keymap, साँचो_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    यदि (type_name == XKB_ATOM_NONE) {
        यदि (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अन्यथा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            यदि (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    यदि (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_साँचो_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 साँचो_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        गच्छ use_default;
    }

    darray_size_t i;
    लागि (i = 0; i < keymap->अङ्क_types; i++)
        यदि (keymap->types[i].name == type_name)
            रोक;

    यदि (i >= keymap->अङ्क_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_साँचो_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 साँचो_Name_Text(keymap->ctx, keyi->name), group + 1);
        गच्छ use_default;
    }

    keymap->types[i].required = true;
    फर्काऊ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    फर्काऊ &keymap->types[0];
}

स्थिर bool
CopySymbolsDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                       साँचो_Info *keyi)
{
    संरचना xkb_साँचो *साँचो;
    GroupInfo *groupi;
    अचर GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    साँचो = Xkb_साँचो_By_Name(keymap, keyi->name, false);
    यदि (!साँचो) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                साँचो_Info_Text(info, keyi));
        फर्काऊ false;
    }

    /* Find the range of groups we need. */
    साँचो->अङ्क_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        अचर bool has_explicit_type = (keyi->defined & साँचो_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        यदि (darray_size(groupi->levels) > 0 || has_explicit_type)
            साँचो->अङ्क_groups = i + 1;
        यदि (has_explicit_type)
            साँचो->explicit |= EXPLICIT_TYPES;
    }

    यदि (साँचो->अङ्क_groups <= 0) {
        /* A key with no group may still have other fields defined */
        यदि (keyi->defined)
            गच्छ साँचो_fields;
        अन्यथा
            फर्काऊ false;
    }

    darray_resize(keyi->groups, साँचो->अङ्क_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        यदि (groupi->defined)
            जारी;

        CopyGroupInfo(groupi, group0);
    }

    साँचो->groups = सुस्मृति(साँचो->अङ्क_groups, परिमाण(*साँचो->groups));
    प्रतिज्ञा(darray_size(keyi->groups) == साँचो->अङ्क_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < साँचो->अङ्क_groups);
        #endif
        bool explicit_type = false;
        अचर संरचना xkb_साँचो_type * अचर type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        यदि (type->अङ्क_levels < darray_size(groupi->levels)) {
            संरचना xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->अङ्क_levels,
                    साँचो_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->अङ्क_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->अङ्क_levels);

        साँचो->groups[i].explicit_type = explicit_type;
        साँचो->groups[i].type = type;
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
            विकल्प (leveli->अङ्क_syms) {
            पक्ष 0:
                leveli->upper = XKB_साँचो_No_चिह्न;
                रोक;
            पक्ष 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                रोक;
            स्वभाव:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                लागि (xkb_keysym_गन्ती_t k = 0; k < leveli->अङ्क_syms; k++) {
                    अचर xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    यदि (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        रोक;
                    }
                }
                यदि (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = पुनः_स्मृति(leveli->s.syms,
                                             (माप_प्रकार) 2 * leveli->अङ्क_syms *
                                             परिमाण(*leveli->s.syms));
                    यदि (!leveli->s.syms)
                        फर्काऊ false; /* FIXME: better handling? */
                    लागि (xkb_keysym_गन्ती_t k = 0; k < leveli->अङ्क_syms; k++) {
                        leveli->s.syms[leveli->अङ्क_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &साँचो->groups[i].levels, रिक्त);
        यदि (साँचो->groups[i].type->अङ्क_levels > 1 ||
            साँचो->groups[i].levels[0].अङ्क_syms > 0) {
            साँचो->groups[i].explicit_symbols = true;
            साँचो->explicit |= EXPLICIT_SYMBOLS;
        }
        यदि (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            साँचो->groups[i].explicit_actions = true;
            साँचो->explicit |= EXPLICIT_INTERP;
        }
        यदि (साँचो->groups[i].explicit_type)
            साँचो->explicit |= EXPLICIT_TYPES;
    }

    साँचो->out_of_range_pending_group = keyi->out_of_range_pending_group;
    साँचो->out_of_range_group_सङ्ख्या = keyi->out_of_range_group_सङ्ख्या;
    साँचो->out_of_range_group_policy = keyi->out_of_range_group_policy;

साँचो_fields:
    यदि (keyi->defined & साँचो_FIELD_VMODMAP) {
        साँचो->vmodmap = keyi->vmodmap;
        साँचो->explicit |= EXPLICIT_VMODMAP;
    }

    यदि (keyi->repeat != साँचो_REPEAT_UNDEFINED) {
        साँचो->repeats = (keyi->repeat == साँचो_REPEAT_YES);
        साँचो->explicit |= EXPLICIT_REPEAT;
    }

    यदि ((keyi->defined & साँचो_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        साँचो->overlays = keyi->overlays;
        यदि (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = साँचो->overlays;
            अचर संरचना xkb_साँचो **overlays_साँचोहरू = keyi->overlays_साँचोहरू;
            जबसम्म (remaining) {
                /* isolate lowest set bit */
                अचर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                यदि (*overlays_साँचोहरू) {
                    overlays_साँचोहरू++;
                } अन्यथा {
                    /* drop current null value (overlayN=none) */
                    साँचो->overlays &= ~lsb;
                }
            }

            यदि (साँचो->overlays) {
                /* Steal */
                साँचो->overlays_साँचोहरू = keyi->overlays_साँचोहरू;
                keyi->overlays_साँचोहरू = रिक्त;
                keyi->overlays_alloc = 0;

                साँचो->overlays_inline = false;
                साँचो->explicit |= EXPLICIT_OVERLAY;
            }
        } अन्यथा {
            साँचो->overlay_साँचो = keyi->overlay_साँचो;
            साँचो->overlays_inline = true;
            साँचो->explicit |= EXPLICIT_OVERLAY;
        }
    }

    फर्काऊ true;
}

स्थिर bool
CopyModMapDefToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    संरचना xkb_साँचो *साँचो;

    यदि (!entry->have_चिह्न) {
        साँचो = Xkb_साँचो_By_Name(keymap, entry->u.साँचो_Name, true);
        यदि (!साँचो) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    साँचो_Name_Text(info->ctx, entry->u.साँचो_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            फर्काऊ false;
        }
    }
    अन्यथा {
        साँचो = Find_साँचो_For_चिह्न(keymap, entry->u.साँचो_Sym);
        यदि (!साँचो) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_चिह्न,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.साँचो_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            फर्काऊ false;
        }
    }

    /* Skip modMap None */
    यदि (entry->mods) {
        साँचो->modmap |= entry->mods;
        entry->साँचो_Code = साँचो->keycode;
    }

    फर्काऊ true;
}

स्थिर bool
CopySymbolsToKeymap(संरचना xkb_keymap *keymap, SymbolsInfo *info)
{
    साँचो_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->अङ्क_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, रिक्त);

    darray_foreach(keyi, info->साँचोहरू)
        यदि (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_गन्ती++;

    यदि (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        संरचना xkb_साँचो *साँचो;

        xkb_साँचोहरू_foreach(साँचो, keymap) {
            यदि (साँचो->name == XKB_ATOM_NONE)
                जारी;

            यदि (साँचो->अङ्क_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         साँचो_Name_Text(info->ctx, साँचो->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        यदि (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_गन्ती++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अङ्क_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    फर्काऊ true;
}

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    यदि (file !=रिक्त)
        HandleSymbolsFile(&info, file);

    यदि (info.error_गन्ती != 0)
        गच्छ err_info;

    यदि (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearSymbolsInfo(&info);
    फर्काऊ true;

err_info:
    ClearSymbolsInfo(&info);
    फर्काऊ false;
}
