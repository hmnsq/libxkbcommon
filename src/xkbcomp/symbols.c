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

#समावेश "config.h"

#समावेश <limits.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "xkbcommon/xkbcommon-keysyms.h"

#समावेश "action.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "expr.h"
#समावेश "include.h"
#समावेश "keymap.h"
#समावेश "keysym.h"
#समावेश "messages-codes.h"
#समावेश "text.h"
#समावेश "utils.h"
#समावेश "util-numbers.h"
#समावेश "util-mem.h"
#समावेश "vmod.h"
#समावेश "xkbcomp-priv.h"
#समावेश "xkbcomp/ast.h"

क्रमागत कुंजी_repeat {
    कुंजी_REPEAT_UNDEFINED = 0,
    कुंजी_REPEAT_YES,
    कुंजी_REPEAT_NO,
    कुंजी_REPEAT_अंक_ENTRIES,
};
क्रमागत { कुंजी_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(कुंजी_REPEAT_अंक_ENTRIES - 1 <=
              (1 << (कुंजी_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

क्रमागत group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

क्रमागत कुंजी_field {
    कुंजी_FIELD_NONE = 0,
    कुंजी_FIELD_REPEAT = (1 << 0),
    कुंजी_FIELD_DEFAULT_TYPE = (1 << 1),
    कुंजी_FIELD_GROUPINFO = (1 << 2),
    कुंजी_FIELD_VMODMAP = (1 << 3),
    कुंजी_FIELD_OVERLAY = (1 << 4),
    कुंजी_FIELD_ALL = कुंजी_FIELD_REPEAT | कुंजी_FIELD_DEFAULT_TYPE
                  | कुंजी_FIELD_GROUPINFO | कुंजी_FIELD_VMODMAP | कुंजी_FIELD_OVERLAY,
};
क्रमागत { कुंजी_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(कुंजी_FIELD_ALL <= (1 << (कुंजी_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

प्रकार काष्ठा {
    darray(काष्ठा xkb_level) levels;
    क्रमागत group_field defined;
    xkb_atom_t type;
} GroupInfo;

क्रमागत {
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
              (अचिन्हित)कुंजी_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_MERGE_BIT_FIELD >=
              (अचिन्हित)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((अचिन्हित)KEYINFO_REPEAT_BIT_FIELD >=
              (अचिन्हित)कुंजी_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

प्रकार काष्ठा {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_सूचकांक_t out_of_range_group_संख्या;
    darray(GroupInfo) groups;
    क्रमागत xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    क्रमागत कुंजी_field defined:KEYINFO_DEFINED_BIT_FIELD;
    क्रमागत merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    क्रमागत कुंजी_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

    bool out_of_range_pending_group:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_सूचकांक_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_mask_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    जोड़ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        स्थिर काष्ठा xkb_कुंजी *overlay_कुंजी;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        स्थिर काष्ठा xkb_कुंजी **overlays_कुंजियाँ;
    };
} कुंजी_Info;

अटल व्योम
StealLevelInfo(काष्ठा xkb_level *into, काष्ठा xkb_level *from)
{
    clear_level(into);
    अगर (from->अंक_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अन्यथा {
        into->s.sym = from->s.sym;
    }
    into->अंक_syms = from->अंक_syms;
    from->अंक_syms = 0;

    अगर (from->अंक_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अन्यथा {
        into->a.action = from->a.action;
    }
    into->अंक_actions = from->अंक_actions;
    from->अंक_actions = 0;
}

अटल व्योम
InitGroupInfo(GroupInfo *groupi)
{
    स_रखो(groupi, 0, माप(*groupi));
}

अटल व्योम
ClearGroupInfo(GroupInfo *groupi)
{
    काष्ठा xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

अटल व्योम
CopyGroupInfo(GroupInfo *to, स्थिर GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    क्रम (xkb_level_सूचकांक_t j = 0; j < darray_आकार(to->levels); j++) {
        अगर (darray_item(from->levels, j).अंक_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).अंक_syms,
                       माप(xkb_keysym_t));
        }
        अगर (darray_item(from->levels, j).अंक_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).अंक_actions,
                       माप(जोड़ xkb_action));
        }
    }
}

अटल व्योम
Init_कुंजी_Info(काष्ठा xkb_context *ctx, कुंजी_Info *keyi)
{
    स_रखो(keyi, 0, माप(*keyi));
    static_assert(!DEFAULT_कुंजी_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_कुंजी_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_कुंजी = शून्य;
}

अटल व्योम
Clear_कुंजी_Info(कुंजी_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    अगर (keyi->overlays_alloc)
        मुक्त(keyi->overlays_कुंजियाँ);
}

/***====================================================================***/

प्रकार काष्ठा {
    अक्षर *name;         /* e.g. pc+us+inet(evdev) */
    पूर्णांक error_गिनती;
    अचिन्हित पूर्णांक include_depth;
    xkb_layout_सूचकांक_t explicit_group;
    xkb_layout_सूचकांक_t max_groups;
    darray(कुंजी_Info) कुंजियाँ;
    कुंजी_Info default_कुंजी;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    काष्ठा xkb_mod_set mods;

    काष्ठा xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    स्थिर काष्ठा xkb_keymap_info *keymap_info;
} SymbolsInfo;

अटल व्योम
InitSymbolsInfo(SymbolsInfo *info, स्थिर काष्ठा xkb_keymap_info *keymap_info,
                अचिन्हित पूर्णांक include_depth, स्थिर काष्ठा xkb_mod_set *mods)
{
    स_रखो(info, 0, माप(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_कुंजी_Info(keymap_info->keymap.ctx, &info->default_कुंजी);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

अटल व्योम
ClearSymbolsInfo(SymbolsInfo *info)
{
    कुंजी_Info *keyi;
    मुक्त(info->name);
    darray_foreach(keyi, info->कुंजियाँ)
        Clear_कुंजी_Info(keyi);
    darray_free(info->कुंजियाँ);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_कुंजी_Info(&info->default_कुंजी);
}

अटल स्थिर अक्षर *
कुंजी_Info_Text(SymbolsInfo *info, कुंजी_Info *keyi)
{
    वापस कुंजी_Name_Text(info->ctx, keyi->name);
}

अटल bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_सूचकांक_t group, xkb_atom_t कुंजी_name)
{
    /* First find the type of the merged group. */
    अगर (into->type != from->type) {
        अगर (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अन्यथा अगर (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अन्यथा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            अगर (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, कुंजी_Name_Text(info->ctx, कुंजी_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    अगर (darray_empty(from->levels)) {
        InitGroupInfo(from);
        वापस true;
    }

    अगर (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        वापस true;
    }

    /* Merge the actions and syms. */
    स्थिर darray_आकार_t levels_in_both =
        MIN(darray_आकार(into->levels), darray_आकार(from->levels));
    darray_आकार_t from_Keysyms_गिनती = 0;
    darray_आकार_t from_Actions_गिनती = 0;
    क्रम (darray_आकार_t i = 0; i < levels_in_both; i++) {
        काष्ठा xkb_level* स्थिर intoLevel = &darray_item(into->levels, i);
        काष्ठा xkb_level* स्थिर fromLevel = &darray_item(from->levels, i);

        स्थिर bool fromHasNoKeysym = fromLevel->अंक_syms == 0;
        स्थिर bool fromHasNoAction = fromLevel->अंक_actions == 0;
        अगर (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            जारी;
        }

        स्थिर bool intoHasNoKeysym = intoLevel->अंक_syms == 0;
        स्थिर bool intoHasNoAction = intoLevel->अंक_actions == 0;
        अगर (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_गिनती++;
            from_Actions_गिनती++;
            जारी;
        }

        /* Possible level conflict */
        निश्चित(intoLevel->अंक_syms > 0 || intoLevel->अंक_actions > 0);
        निश्चित(fromLevel->अंक_syms > 0 || fromLevel->अंक_actions > 0);

        /* Handle keysyms */
        अगर (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            अगर (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_प्रतीक,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, कुंजी_Name_Text(info->ctx, कुंजी_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            अगर (fromHasNoKeysym) {
                /* No keysym to copy */
            } अन्यथा अगर (clobber) {
                /* Override: copy any defined keysym from `from` */
                अगर (unlikely(fromLevel->अंक_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    अगर (unlikely(intoLevel->अंक_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अंक_syms = fromLevel->अंक_syms;
                    fromLevel->अंक_syms = 0;
                    from_Keysyms_गिनती++;
                } अन्यथा अगर (fromLevel->s.sym != XKB_कुंजी_No_प्रतीक) {
                    /* Single defined keysym */
                    अगर (unlikely(intoLevel->अंक_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अंक_syms = 1;
                    from_Keysyms_गिनती++;
                }
            } अन्यथा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                अगर (unlikely(intoLevel->अंक_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा अगर (intoLevel->s.sym == XKB_कुंजी_No_प्रतीक) {
                    /* Single undefined keysym */
                    अगर (unlikely(fromLevel->अंक_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अंक_syms = fromLevel->अंक_syms;
                        fromLevel->अंक_syms = 0;
                    } अन्यथा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अंक_syms = 1;
                    }
                    from_Keysyms_गिनती++;
                }
            }
        }

        /* Handle actions */
        अगर (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            अगर (report && !(intoHasNoAction || fromHasNoAction)) {
                अगर (unlikely(intoLevel->अंक_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_कुंजी_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, कुंजी_Name_Text(info->ctx, कुंजी_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अन्यथा {
                    जोड़ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                कुंजी_Name_Text(info->ctx, कुंजी_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            अगर (fromHasNoAction) {
                /* No action to copy */
            } अन्यथा अगर (clobber) {
                /* Override: copy any defined action from `from` */
                अगर (unlikely(fromLevel->अंक_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    अगर (unlikely(intoLevel->अंक_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अंक_actions = fromLevel->अंक_actions;
                    fromLevel->अंक_actions = 0;
                    from_Actions_गिनती++;
                } अन्यथा अगर (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    अगर (unlikely(intoLevel->अंक_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अंक_actions = 1;
                    from_Actions_गिनती++;
                }
            } अन्यथा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                अगर (unlikely(intoLevel->अंक_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा अगर (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    अगर (unlikely(fromLevel->अंक_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अंक_actions = fromLevel->अंक_actions;
                        fromLevel->अंक_actions = 0;
                    } अन्यथा {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->अंक_actions = 1;
                    }
                    from_Actions_गिनती++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    काष्ठा xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->अंक_syms = 0;
        level->अंक_actions = 0;
        from_Keysyms_गिनती++;
        from_Actions_गिनती++;
    }
    अगर (from_Keysyms_गिनती) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        अगर (from_Keysyms_गिनती == darray_आकार(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    अगर (from_Actions_गिनती) {
        /* Reset defined actions field if we used no action from `into` */
        अगर (from_Actions_गिनती == darray_आकार(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    वापस true;
}

अटल bool
Use_New_कुंजी_Field(क्रमागत कुंजी_field field, क्रमागत कुंजी_field old, क्रमागत कुंजी_field new,
               bool clobber, bool report, क्रमागत कुंजी_field *collide)
{
    अगर (!(old & field))
        वापस (new & field);

    अगर (new & field) {
        अगर (report)
            *collide |= field;

        वापस clobber;
    }

    वापस false;
}

अटल bool
overlays_get(स्थिर कुंजी_Info *info, xkb_overlay_सूचकांक_t bit,
             स्थिर काष्ठा xkb_कुंजी **कुंजी_out)
{
    अगर (bit >= (xkb_overlay_सूचकांक_t)(माप(xkb_overlay_mask_t) * अक्षर_बिट))
        वापस false;
    स्थिर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    अगर (!(info->overlays & mask)) वापस false;
    अगर (कुंजी_out) {
        अगर (!info->overlays_alloc) {
            /* Stored inline */
            *कुंजी_out = info->overlay_कुंजी;
        } अन्यथा {
            /* Stored in sparse array */
            स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            स्थिर xkb_overlay_सूचकांक_t सूचकांक = (xkb_overlay_सूचकांक_t)popcount32(निचला);
            *कुंजी_out = info->overlays_कुंजियाँ[सूचकांक];
        }
    }
    वापस true;
}

अटल bool
overlays_जोड़ो(कुंजी_Info *keyi, xkb_overlay_सूचकांक_t bit,
                स्थिर काष्ठा xkb_कुंजी *कुंजी)
{
    अगर (bit >= (xkb_overlay_सूचकांक_t)(माप(xkb_overlay_mask_t) * अक्षर_बिट))
        वापस false;

    स्थिर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    अगर ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        अगर (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_कुंजी = कुंजी;
            अगर (!कुंजी)
                keyi->overlays_clear = true;
        } अन्यथा {
            /* Stored in sparse array */
            स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            स्थिर xkb_overlay_सूचकांक_t सूचकांक =
                (xkb_overlay_सूचकांक_t)popcount32(निचला);
            keyi->overlays_कुंजियाँ[सूचकांक] = कुंजी;
        }
        वापस true;
    }

    /* Insert new element */
    अगर (!keyi->overlays || (keyi->overlays_clear && !कुंजी)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_कुंजी = कुंजी;
        keyi->overlays |= mask;
        keyi->overlays_clear = !कुंजी;
    } अन्यथा अगर (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        स्थिर xkb_overlay_mask_t overlays = keyi->overlays | mask;
        स्थिर xkb_overlay_सूचकांक_t alloc =
            (xkb_overlay_सूचकांक_t)popcount32(overlays);

        /* Alloc sparse array */
        स्थिर काष्ठा xkb_कुंजी ** स्थिर tmp = सुस्मृति(alloc, माप(*tmp));
        अगर (!tmp) वापस false;

        /* Copy previous entries to the buffer */
        अगर (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            स्थिर xkb_overlay_सूचकांक_t idx = (xkb_overlay_सूचकांक_t)popcount32(निचला);
            tmp[idx] = keyi->overlay_कुंजी;
        } अन्यथा {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        स्थिर xkb_overlay_सूचकांक_t idx = (xkb_overlay_सूचकांक_t)popcount32(निचला);
        tmp[idx] = कुंजी;

        keyi->overlays_कुंजियाँ = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } अन्यथा {
        /* Allocated items: grow heap and shift if relevant */
        स्थिर xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        स्थिर xkb_overlay_सूचकांक_t गिनती =
            (xkb_overlay_सूचकांक_t)popcount32(overlays);
        अगर (गिनती > keyi->overlays_alloc) {
            स्थिर xkb_overlay_सूचकांक_t alloc =
                (xkb_overlay_सूचकांक_t)अग्र_pow2(गिनती);
            निश्चित(alloc >= गिनती);
            स्थिर काष्ठा xkb_कुंजी ** स्थिर tmp = पुनः_स्मृति(
                keyi->overlays_कुंजियाँ,
                (माप_प्रकार)alloc * माप(*keyi->overlays_कुंजियाँ)
            );
            अगर (!tmp) वापस false;
            keyi->overlays_कुंजियाँ = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        स्थिर xkb_overlay_सूचकांक_t सूचकांक = (xkb_overlay_सूचकांक_t)popcount32(निचला);
        अगर (सूचकांक >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        अगर (सूचकांक < गिनती - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            निश्चित(सूचकांक + 1 < keyi->overlays_alloc);
            स_हटाओ(keyi->overlays_कुंजियाँ + सूचकांक + 1, keyi->overlays_कुंजियाँ + सूचकांक,
                    (माप_प्रकार)(गिनती - 1u - सूचकांक) * माप(स्थिर काष्ठा xkb_कुंजी *));
        }
        keyi->overlays_कुंजियाँ[सूचकांक] = कुंजी;
        keyi->overlays = overlays;
    }

    वापस true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
अटल bool
merge_overlays(SymbolsInfo *info, कुंजी_Info *into, कुंजी_Info *from,
               bool clobber, bool report, क्रमागत कुंजी_field *collide)
{
    अगर (!(from->defined & कुंजी_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अन्यथा अगर (!(into->defined & कुंजी_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_कुंजियाँ = from->overlays_कुंजियाँ;
        from->overlays_कुंजियाँ = शून्य;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= कुंजी_FIELD_OVERLAY;
    } अन्यथा अगर (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अन्यथा {
        अगर (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            स्थिर xkb_overlay_mask_t परिणाम_mask =
                (into->overlays | from->overlays);
            स्थिर xkb_overlay_सूचकांक_t गिनती =
                (xkb_overlay_सूचकांक_t)popcount32(परिणाम_mask);

            अगर (गिनती == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            कुंजी_Info *dest = into;
            कुंजी_Info *src = from;
            अगर (into->overlays_alloc >= गिनती &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अन्यथा अगर (from->overlays_alloc >= गिनती) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अन्यथा अगर (गिनती > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                अगर (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                अगर (!dest->overlays_alloc) {
                    स्थिर काष्ठा xkb_कुंजी **tmp = सुस्मृति(गिनती, माप(*tmp));
                    अगर (!tmp)
                        वापस false;
                    अगर (dest->overlay_कुंजी)
                        tmp[0] = dest->overlay_कुंजी;
                    dest->overlays_कुंजियाँ = tmp;
                    dest->overlays_alloc = गिनती;
                } अन्यथा {
                    स्थिर काष्ठा xkb_कुंजी **tmp =
                        पुनः_स्मृति(dest->overlays_कुंजियाँ,
                                गिनती * माप(*dest->overlays_कुंजियाँ));
                    अगर (!tmp)
                        वापस false;
                    dest->overlays_कुंजियाँ = tmp;
                    dest->overlays_alloc = गिनती;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            स्थिर काष्ठा xkb_कुंजी **src_कुंजियाँ = (src->overlays_clear)
                ? शून्य
                : (src->overlays_alloc)
                    ? src->overlays_कुंजियाँ
                    : &src->overlay_कुंजी;
            जबतक (remaining) {
                /* isolate lowest set bit */
                स्थिर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                /* get its index */
                स्थिर xkb_overlay_सूचकांक_t bit =
                    (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                अगर (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                स्थिर काष्ठा xkb_कुंजी *src_कुंजी = (src_कुंजियाँ)
                    ? *(src_कुंजियाँ++)
                    : शून्य;

                /* check dest */
                स्थिर काष्ठा xkb_कुंजी *dest_कुंजी = शून्य;
                स्थिर bool conflict = overlays_get(dest, bit, &dest_कुंजी);
                अगर (conflict) {
                    अगर (dest_कुंजी == src_कुंजी)
                       जारी;
                    अन्यथा अगर (report)
                        *collide |= कुंजी_FIELD_OVERLAY;
                }

                अगर ((!conflict || clobber) && !overlays_जोड़ो(dest, bit, src_कुंजी))
                    वापस false;
            }

            अगर (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                अगर (into->overlays_alloc)
                    मुक्त(into->overlays_कुंजियाँ);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                अगर (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_कुंजियाँ = from->overlays_कुंजियाँ;
                    from->overlays_कुंजियाँ = शून्य;
                    from->overlays_alloc = 0;
                } अन्यथा {
                    into->overlay_कुंजी = from->overlay_कुंजी;
                }
            }
        } अन्यथा {
            /* Disjoint overlays */

            अगर (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_कुंजी == from->overlay_कुंजी) {
                /* Indentical: do nothing */
                वापस true;
            }
            अगर (!(into->overlays & from->overlays)) {
                अगर (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_कुंजी = from->overlay_कुंजी;
                    वापस true;
                } अन्यथा अगर (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    वापस true;
                }
            }
            /* Conflict */
            अगर (report)
                *collide |= कुंजी_FIELD_OVERLAY;
            अगर (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_कुंजी = from->overlay_कुंजी;
            }
        }
    }
    वापस true;
}

अटल bool
Merge_कुंजियाँ(SymbolsInfo *info, कुंजी_Info *into, कुंजी_Info *from, bool same_file)
{
    xkb_layout_सूचकांक_t i;
    xkb_layout_सूचकांक_t groups_in_both;
    क्रमागत कुंजी_field collide = 0;
    स्थिर पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिर bool clobber = (from->merge != MERGE_AUGMENT);
    स्थिर bool report = (same_file && verbosity > 0) || verbosity > 9;

    अगर (from->merge == MERGE_REPLACE) {
        Clear_कुंजी_Info(into);
        *into = *from;
        Init_कुंजी_Info(info->ctx, from);
        वापस true;
    }

    groups_in_both = MIN(darray_आकार(into->groups), darray_आकार(from->groups));
    क्रम (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    क्रम (i = groups_in_both; i < darray_आकार(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    अगर (Use_New_कुंजी_Field(कुंजी_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= कुंजी_FIELD_VMODMAP;
    }
    अगर (Use_New_कुंजी_Field(कुंजी_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= कुंजी_FIELD_REPEAT;
    }
    अगर (Use_New_कुंजी_Field(कुंजी_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= कुंजी_FIELD_DEFAULT_TYPE;
    }
    अगर (Use_New_कुंजी_Field(कुंजी_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_संख्या = from->out_of_range_group_संख्या;
        into->defined |= कुंजी_FIELD_GROUPINFO;
    }

    अगर (!merge_overlays(info, into, from, clobber, report, &collide))
        वापस false;

    अगर (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 कुंजी_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_कुंजी_Info(from);
    Init_कुंजी_Info(info->ctx, from);
    वापस true;
}

अटल xkb_atom_t
Xkb_Resolve_कुंजी_Alias(स्थिर काष्ठा xkb_keymap *keymap, xkb_atom_t name)
{
    अगर (name < keymap->अंक_कुंजी_names) {
        स्थिर KeycodeMatch match = keymap->कुंजी_names[name];
        अगर (match.found && match.is_alias) {
            वापस match.alias.real;
        }
    }
    वापस name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
अटल bool
Add_कुंजी_Symbols(SymbolsInfo *info, कुंजी_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_कुंजी_Alias(&info->keymap_info->keymap, keyi->name);

    कुंजी_Info *iter;
    darray_foreach(iter, info->कुंजियाँ)
        अगर (iter->name == keyi->name)
            वापस Merge_कुंजियाँ(info, iter, keyi, same_file);

    darray_append(info->कुंजियाँ, *keyi);
    Init_कुंजी_Info(info->ctx, keyi);
    वापस true;
}

अटल bool
AddModMapEntry(SymbolsInfo *info, स्थिर ModMapEntry *new)
{
    स्थिर bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        अगर ((new->have_प्रतीक != old->have_प्रतीक) ||
            (new->have_प्रतीक && new->u.कुंजी_Sym != old->u.कुंजी_Sym) ||
            (!new->have_प्रतीक && new->u.कुंजी_Name != old->u.कुंजी_Name))
            जारी;

        अगर (new->mods == old->mods)
            वापस true;

        स्थिर xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        स्थिर xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        अगर (new->have_प्रतीक) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.कुंजी_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } अन्यथा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    कुंजी_Name_Text(info->ctx, new->u.कुंजी_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        वापस true;
    }

    darray_append(info->modmaps, *new);

    वापस true;
}

/***====================================================================***/

अटल व्योम
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     क्रमागत merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_सूचकांक_t group_names_in_both;

    अगर (from->error_गिनती > 0) {
        into->error_गिनती += from->error_गिनती;
        वापस;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    अगर (into->name == शून्य) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_आकार(into->group_names),
                              darray_आकार(from->group_names));
    क्रम (xkb_layout_सूचकांक_t i = 0; i < group_names_in_both; i++) {
        अगर (!darray_item(from->group_names, i))
            जारी;

        अगर (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            जारी;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    अगर (darray_empty(into->कुंजियाँ)) {
        into->कुंजियाँ = from->कुंजियाँ;
        darray_init(from->कुंजियाँ);
    }
    अन्यथा {
        कुंजी_Info *keyi;
        darray_foreach(keyi, from->कुंजियाँ) {
            keyi->merge = merge;
            अगर (!Add_कुंजी_Symbols(into, keyi, false))
                into->error_गिनती++;
        }
    }

    अगर (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अन्यथा {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            अगर (!AddModMapEntry(into, mm))
                into->error_गिनती++;
        }
    }
}

अटल व्योम
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

अटल bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    अगर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गिनती += 10;
        वापस false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    स्थिर काष्ठा parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    क्रम (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        SymbolsInfo अग्र_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, माप(path));
        अगर (!file) {
            info->error_गिनती += 10;
            ClearSymbolsInfo(&included);
            वापस false;
        }

        InitSymbolsInfo(&अग्र_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        अगर (stmt->modifier) {
            स्थिर पूर्णांक गिनती = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &अग्र_incl.explicit_group);
            अगर (गिनती <= 0 || *(stmt->modifier + गिनती) != '\0' ||
                अग्र_incl.explicit_group == 0 ||
                अग्र_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                अग्र_incl.explicit_group = info->explicit_group;
            } अन्यथा {
                अग्र_incl.explicit_group--;
            }
        }
        अन्यथा अगर (info->keymap_info->keymap.अंक_groups != 0 &&
                 अग्र_incl.include_depth == 1) {
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
            अग्र_incl.explicit_group = 0;
        }
        अन्यथा {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            अग्र_incl.explicit_group = info->explicit_group;
        }

        HandleSymbolsFile(&अग्र_incl, file);

        MergeIncludedSymbols(&included, &अग्र_incl, stmt->merge);

        ClearSymbolsInfo(&अग्र_incl);
        FreeXkbFile(file);
    }

    MergeIncludedSymbols(info, &included, include->merge);
    ClearSymbolsInfo(&included);

    वापस (info->error_गिनती == 0);
}

अटल bool
Get_Group_सूचकांक(SymbolsInfo *info, कुंजी_Info *keyi, व्यंजक_Def *arrayNdx,
              क्रमागत group_field field, xkb_layout_सूचकांक_t *ndx_rtrn)
{
    निश्चित (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    स्थिर अक्षर *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    अगर (arrayNdx == शून्य) {
        xkb_layout_सूचकांक_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            अगर (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                वापस true;
            }
        }

        अगर (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, कुंजी_Info_Text(info, keyi), info->max_groups, name);
            वापस false;
        }

        darray_resize0(keyi->groups, darray_आकार(keyi->groups) + 1);
        *ndx_rtrn = darray_आकार(keyi->groups) - 1;
        वापस true;
    }

    अगर (व्यंजक_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, शून्य) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, कुंजी_Info_Text(info, keyi));
        वापस false;
    }

    (*ndx_rtrn)--;
    अगर (*ndx_rtrn >= darray_आकार(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    वापस true;
}

अटल bool
Add_Symbols_To_कुंजी(SymbolsInfo *info, कुंजी_Info *keyi, व्यंजक_Def *arrayNdx,
                व्यंजक_Def *मूल्य)
{
    xkb_layout_सूचकांक_t ndx = 0;

    अगर (!Get_Group_सूचकांक(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        वापस false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    अगर (मूल्य->common.type == STMT_व्यंजक_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        वापस true;
    }

    अगर (मूल्य->common.type != STMT_व्यंजक_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(मूल्य->common.type), ndx + 1,
                कुंजी_Info_Text(info, keyi));
        वापस false;
    }

    अगर (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_कुंजी_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                कुंजी_Info_Text(info, keyi), ndx + 1);
        वापस false;
    }

    xkb_level_सूचकांक_t nLevels = 0;
    xkb_level_सूचकांक_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    क्रम (व्यंजक_Keysym_List *keysymList = (व्यंजक_Keysym_List *) मूल्य;
         keysymList;
         keysymList = (व्यंजक_Keysym_List *) keysymList->common.अग्र) {
        nLevels++;
        /* Drop trailing NoSymbol */
        अगर (darray_आकार(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    अगर (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    अगर (darray_आकार(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_सूचकांक_t level = 0;
    क्रम (व्यंजक_Keysym_List *keysymList = (व्यंजक_Keysym_List *) मूल्य;
         keysymList && level < nLevels;
         keysymList = (व्यंजक_Keysym_List *) keysymList->common.अग्र, level++) {
        काष्ठा xkb_level *leveli = &darray_item(groupi->levels, level);
        निश्चित(leveli->अंक_syms == 0);

        अगर (unlikely(darray_आकार(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुंजी_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_आकार(keysymList->syms));
            वापस false;
        }

        leveli->अंक_syms = (xkb_keysym_गिनती_t) darray_आकार(keysymList->syms);
        चयन (leveli->अंक_syms) {
        हाल 0:
            leveli->s.sym = XKB_कुंजी_No_प्रतीक;
            अवरोध;
        हाल 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            निश्चित(leveli->s.sym != XKB_कुंजी_No_प्रतीक);
            अवरोध;
        शेष:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, शून्य);
#अगर_अघोषित न_संशोधन
            /* Canonical list: all NoSymbol were dropped */
            क्रम (xkb_keysym_गिनती_t k = 0; k < leveli->अंक_syms; k++)
                निश्चित(leveli->s.syms[k] != XKB_कुंजी_No_प्रतीक);
#पूर्ण_अगर
        }
    }

    वापस true;
}

अटल bool
Add_Actions_To_कुंजी(SymbolsInfo *info, कुंजी_Info *keyi, व्यंजक_Def *arrayNdx,
                व्यंजक_Def *मूल्य)
{
    xkb_layout_सूचकांक_t ndx = 0;

    अगर (!Get_Group_सूचकांक(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        वापस false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    अगर (मूल्य->common.type == STMT_व्यंजक_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        वापस true;
    }

    अगर (मूल्य->common.type != STMT_व्यंजक_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मूल्य->common.type, ndx, कुंजी_Info_Text(info, keyi));
        वापस false;
    }

    अगर (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 कुंजी_Info_Text(info, keyi), ndx);
        वापस false;
    }

    xkb_level_सूचकांक_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    क्रम (ParseCommon *p = &मूल्य->common; p; p = p->अग्र)
        nLevels++;
    अगर (darray_आकार(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_सूचकांक_t level = 0;
    xkb_level_सूचकांक_t nonEmptyLevels = 0;
    क्रम (व्यंजक_Action_List *actionList = (व्यंजक_Action_List *) मूल्य;
         actionList;
         actionList = (व्यंजक_Action_List *) actionList->common.अग्र, level++) {
        काष्ठा xkb_level *leveli = &darray_item(groupi->levels, level);
        निश्चित(leveli->अंक_actions == 0);

        अचिन्हित पूर्णांक अंक_actions = 0;
        क्रम (व्यंजक_Def *act = actionList->actions;
             act; act = (व्यंजक_Def *) act->common.अग्र)
            अंक_actions++;

        अगर (unlikely(अंक_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुंजी_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, अंक_actions);
            वापस false;
        }

        /* Parse actions and add only defined actions */
        darray(जोड़ xkb_action) actions = darray_new();
        क्रम (व्यंजक_Def *act = actionList->actions;
             act; act = (व्यंजक_Def *) act->common.अग्र) {
            जोड़ xkb_action toAct = { 0 };
            स्थिर क्रमागत xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            अगर (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        कुंजी_Info_Text(info, keyi), ndx + 1, level + 1);
                अगर (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    वापस false;
                } अन्यथा {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            अगर (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अन्यथा अगर (likely(अंक_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अंक_actions = 1;
                leveli->a.action = toAct;
                जाओ अग्र_level;
            } अन्यथा {
                darray_append(actions, toAct);
            }
        }
        अगर (darray_empty(actions)) {
            leveli->अंक_actions = 0;
        } अन्यथा अगर (likely(darray_आकार(actions) > 1)) {
            leveli->अंक_actions = (xkb_action_गिनती_t) darray_आकार(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, शून्य);
#अगर_अघोषित न_संशोधन
            /* Canonical list: all NoAction() were dropped */
            क्रम (xkb_action_गिनती_t k = 0; k < leveli->अंक_actions; k++)
                निश्चित(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#पूर्ण_अगर
        } अन्यथा {
            /* Unlikely: some actions were dropped and only one remains */
            निश्चित(अंक_actions > darray_आकार(actions));
            leveli->अंक_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            निश्चित(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
अग्र_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        अगर (leveli->अंक_actions > 0 || leveli->अंक_syms > 0)
            nonEmptyLevels = level + 1;
    }

    अगर (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_आकार(groupi->levels) = nonEmptyLevels;
        अगर (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        अन्यथा
            darray_free(groupi->levels);
    }

    वापस true;
}

अटल स्थिर LookupEntry repeatEntries[] = {
    { "true", कुंजी_REPEAT_YES },
    { "yes", कुंजी_REPEAT_YES },
    { "on", कुंजी_REPEAT_YES },
    { "false", कुंजी_REPEAT_NO },
    { "no", कुंजी_REPEAT_NO },
    { "off", कुंजी_REPEAT_NO },
    { "default", कुंजी_REPEAT_UNDEFINED },
    { शून्य, 0 }
};

/** Handle overlay<N> = <key> entry */
अटल bool
व्यंजक_Resolve_Overlay_Entry(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                        स्थिर अक्षर *field, स्थिर व्यंजक_Def *arrayNdx,
                        स्थिर व्यंजक_Def *व्यंजक, कुंजी_Info *keyi,
                        xkb_overlay_सूचकांक_t *overlay_rtrn,
                        स्थिर काष्ठा xkb_कुंजी **कुंजी_rtrn)
{
    /* Overlay index */
    अगर (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, कुंजी_Name_Text(keymap_info->keymap.ctx, keyi->name));
        वापस false;
    }
    static_assert(माप(xkb_overlay_सूचकांक_t) == माप(uint8_t), "");
    स्थिर माप_प्रकार prefix = माप("overlay") - 1;
    स्थिर माप_प्रकार len = म_माप(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    अगर (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (पूर्णांक)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_सूचकांक,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, कुंजी_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        वापस false;
    }
    *overlay_rtrn = (xkb_overlay_सूचकांक_t)raw_overlay - 1;

    /* Overlay key */
    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_KEYNAME_LITERAL:
        *कुंजी_rtrn = Xkb_कुंजी_By_Name(&keymap_info->keymap, व्यंजक->कुंजी_name.कुंजी_name,
                                 false);
        अगर (!*कुंजी_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, व्यंजक->कुंजी_name.कुंजी_name),
                    field, कुंजी_Name_Text(keymap_info->keymap.ctx, keyi->name));
            वापस false;
        }
        वापस true;
    हाल STMT_व्यंजक_IDENT: {
        स्थिर अक्षर * स्थिर id =
            xkb_atom_text(keymap_info->keymap.ctx, व्यंजक->ident.ident);
        अगर (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *कुंजी_rtrn = शून्य;
            वापस true;
        } अन्यथा अगर (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *कुंजी_rtrn = शून्य;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            वापस true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मूल्य,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, कुंजी_Name_Text(keymap_info->keymap.ctx, keyi->name));
        वापस false;
    }
    शेष:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मूल्य,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_व्यंजक_KEYNAME_LITERAL),
                field, कुंजी_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;
    }
}

अटल bool
SetSymbolsField(SymbolsInfo *info, कुंजी_Info *keyi, स्थिर अक्षर *field,
                व्यंजक_Def *arrayNdx, व्यंजक_Def **मूल्य_ptr)
{
    व्यंजक_Def * restrict स्थिर मूल्य = *मूल्य_ptr;
    अगर (istreq(field, "type")) {
        xkb_layout_सूचकांक_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        अगर (!व्यंजक_Resolve_String(info->ctx, मूल्य, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            वापस false;
        }

        अगर (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= कुंजी_FIELD_DEFAULT_TYPE;
        }
        अन्यथा अगर (व्यंजक_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, शून्य) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }
        अन्यथा {
            ndx--;
            अगर (ndx >= darray_आकार(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                निश्चित(darray_आकार(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    अन्यथा अगर (istreq(field, "symbols")) {
        वापस Add_Symbols_To_कुंजी(info, keyi, arrayNdx, मूल्य);
    }
    अन्यथा अगर (istreq(field, "actions")) {
        वापस Add_Actions_To_कुंजी(info, keyi, arrayNdx, मूल्य);
    }
    अन्यथा अगर (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(मूल्य->common.type),
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= कुंजी_FIELD_VMODMAP;
    }
    अन्यथा अगर (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                कुंजी_Info_Text(info, keyi));
    }
    अन्यथा अगर (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                कुंजी_Info_Text(info, keyi));
    }
    अन्यथा अगर (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                कुंजी_Info_Text(info, keyi));
    }
    अन्यथा अगर (istreq_prefix("overlay", field)) {
        xkb_overlay_सूचकांक_t overlay = XKB_OVERLAY_INVALID;
        स्थिर काष्ठा xkb_कुंजी *कुंजी = शून्य;
        अगर (!व्यंजक_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *मूल्य_ptr, keyi, &overlay, &कुंजी))
            वापस false;
        अगर (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            वापस true;
        } अन्यथा अगर (कुंजी && कुंजी->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, कुंजी_Info_Text(info, keyi));
        } अन्यथा {
            स्थिर काष्ठा xkb_कुंजी *पूर्व;
            अगर (overlays_get(keyi, overlay, &पूर्व)) {
                /* Duplicate entry */
                अगर (कुंजी != पूर्व) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             कुंजी_Info_Text(info, keyi), overlay + 1,
                             (पूर्व
                                 ? कुंजी_Name_Text(info->ctx, पूर्व->name)
                                 : "none"),
                             overlay + 1,
                             (कुंजी
                                 ? कुंजी_Name_Text(info->ctx, कुंजी->name)
                                 : "none"));
                }
            } अन्यथा अगर (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                अगर (!overlays_जोड़ो(keyi, overlay, कुंजी))
                    वापस false;
                keyi->defined |= कुंजी_FIELD_OVERLAY;
            } अन्यथा {
                /* Handle disjoint overlays mode */
                स्थिर xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                अगर (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    अगर (कुंजी) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_कुंजी = कुंजी;
                    } अन्यथा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= कुंजी_FIELD_OVERLAY;
                } अन्यथा अगर (keyi->overlays) {
                    अगर (कुंजी) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                कुंजी_Info_Text(info, keyi), keyi->overlays,
                                कुंजी_Name_Text(info->ctx, keyi->overlay_कुंजी->name),
                                overlay + 1, कुंजी_Name_Text(info->ctx, कुंजी->name));
                        वापस !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_मूल्य_MISMATCH);
                    } अन्यथा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अन्यथा अगर (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        अगर (!व्यंजक_Resolve_Enum(info->ctx, मूल्य, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }

        keyi->repeat = (क्रमागत कुंजी_repeat) val;
        keyi->defined |= कुंजी_FIELD_REPEAT;
    }
    अन्यथा अगर (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        अगर (!व्यंजक_Resolve_Boolean(info->ctx, मूल्य, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= कुंजी_FIELD_GROUPINFO;
    }
    अन्यथा अगर (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        अगर (!व्यंजक_Resolve_Boolean(info->ctx, मूल्य, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मूल्य,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= कुंजी_FIELD_GROUPINFO;
    }
    अन्यथा अगर (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_सूचकांक_t grp = 0;
        bool pending = false;

        // TODO: recover?
        अगर (व्यंजक_Resolve_Group(info->keymap_info, मूल्य, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    कुंजी_Info_Text(info, keyi));
            वापस false;
        }

        अगर (pending) {
            keyi->out_of_range_pending_group = true;
            स्थिर darray_आकार_t pending_सूचकांक =
                darray_आकार(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (काष्ठा pending_computation) {
                    .व्यंजक = *मूल्य_ptr,
                    .computed = false,
                    .मूल्य = 0,
                }
            );
            *मूल्य_ptr = शून्य;
            static_assert(माप(keyi->out_of_range_group_संख्या) ==
                          माप(pending_सूचकांक),
                          "Cannot save pending computation");
            keyi->out_of_range_group_संख्या = pending_सूचकांक;
        } अन्यथा {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_संख्या = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= कुंजी_FIELD_GROUPINFO;
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        वापस !(info->keymap_info->strict & PARSER_NO_UNKNOWN_कुंजी_FIELDS);
    }

    वापस true;
}

अटल bool
SetGroupName(SymbolsInfo *info, व्यंजक_Def *arrayNdx, व्यंजक_Def *मूल्य,
             क्रमागत merge_mode merge)
{
    अगर (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_सूचकांक,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        वापस false;
    }

    xkb_layout_सूचकांक_t group = 0;
    अगर (व्यंजक_Resolve_Group(info->keymap_info, arrayNdx, false, &group, शून्य) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        वापस false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    अगर (!व्यंजक_Resolve_String(info->ctx, मूल्य, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        वापस false;
    }

    xkb_layout_सूचकांक_t group_to_use;
    अगर (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    अन्यथा अगर (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    अन्यथा {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        वापस false;
    }

    अगर (group_to_use >= darray_आकार(info->group_names)) {
        /* Avoid clang-tidy false positive */
        निश्चित(darray_आकार(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } अन्यथा {
        स्थिर xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        अगर (old_name != XKB_ATOM_NONE && old_name != name) {
            स्थिर bool replace = (merge != MERGE_AUGMENT);
            स्थिर xkb_atom_t use    = (replace ? name : old_name);
            स्थिर xkb_atom_t ignore = (replace ? old_name : name);
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

    वापस true;
}

अटल bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *arrayNdx;
    bool ret;

    अगर (!व्यंजक_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        वापस false;

    अगर (elem && istreq(elem, "key")) {
        कुंजी_Info अस्थायी = {0};
        Init_कुंजी_Info(info->ctx, &अस्थायी);
        /* Do not replace the whole key, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &अस्थायी, field, arrayNdx, &stmt->मूल्य) &&
              Merge_कुंजियाँ(info, &info->default_कुंजी, &अस्थायी, true);
        Clear_कुंजी_Info(&अस्थायी);
    }
    अन्यथा अगर (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->मूल्य, stmt->merge);
    }
    अन्यथा अगर (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा अगर (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा अगर (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा अगर (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अन्यथा अगर (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->मूल्य, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        वापस !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    वापस ret;
}

अटल bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, कुंजी_Info *keyi)
{
    bool all_valid_entries = true;

    क्रम (; def; def = (VarDef *) def->common.अग्र) {
        स्थिर अक्षर *field;
        व्यंजक_Def *arrayNdx;
        bool ok = true;

        अगर (!def->name) {
            अगर (unlikely(!def->मूल्य) || def->मूल्य->common.type != STMT_व्यंजक_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            अन्यथा
                field = "actions";
            arrayNdx = शून्य;
        }
        अन्यथा {
            स्थिर अक्षर *elem;
            ok = व्यंजक_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            अगर (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        अगर (unlikely(!def->मूल्य)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        अगर (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->मूल्य))
            all_valid_entries = false;
    }

    वापस all_valid_entries;
}

अटल bool
SetExplicitGroup(SymbolsInfo *info, कुंजी_Info *keyi)
{
    xkb_layout_सूचकांक_t i;
    GroupInfo *groupi;
    bool warn = false;

    अगर (info->explicit_group == XKB_LAYOUT_INVALID)
        वापस true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        अगर (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    अगर (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, कुंजी_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    अगर (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    वापस true;
}

अटल bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    कुंजी_Info keyi;

    keyi = info->default_कुंजी;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_कुंजी.groups);
    क्रम (xkb_layout_सूचकांक_t i = 0; i < darray_आकार(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_कुंजी.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->कुंजी_Name;

    अगर (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_कुंजी_Symbols(info, &keyi, true)) {
        वापस true;
    }

    Clear_कुंजी_Info(&keyi);
    info->error_गिनती++;
    वापस false;
}

अटल bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    काष्ठा xkb_context *ctx = info->ctx;

    अगर (!व्यंजक_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        वापस false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    निश्चित(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_व्यंजक_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    क्रम (व्यंजक_Def *कुंजी = def->कुंजियाँ; कुंजी; कुंजी = (व्यंजक_Def *) कुंजी->common.अग्र) {
        अगर (कुंजी->common.type == STMT_व्यंजक_KEYNAME_LITERAL) {
            tmp.have_प्रतीक = false;
            tmp.u.कुंजी_Name = कुंजी->कुंजी_name.कुंजी_name;
        }
        अन्यथा अगर (कुंजी->common.type == STMT_व्यंजक_KEYSYM_LITERAL) {
            अगर (कुंजी->keysym.keysym == XKB_कुंजी_No_प्रतीक) {
                /* Invalid keysym: ignore. Error message already printed */
                जारी;
            } अन्यथा {
                tmp.have_प्रतीक = true;
                tmp.u.कुंजी_Sym = कुंजी->keysym.keysym;
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
    वापस ok;
}

अटल व्योम
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    क्रम (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        चयन (stmt->type) {
        हाल STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            अवरोध;
        हाल STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            अवरोध;
        हाल STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            अवरोध;
        हाल STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            अवरोध;
        हाल STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            अवरोध;
        हाल STMT_UNKNOWN_DECLARATION:
        हाल STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            अवरोध;
        शेष:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            अवरोध;
        }

        अगर (!ok)
            info->error_गिनती++;

        अगर (info->error_गिनती > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            अवरोध;
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
अटल काष्ठा xkb_कुंजी *
Find_कुंजी_For_प्रतीक(काष्ठा xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_सूचकांक_t group = 0;
    करो {
        xkb_level_सूचकांक_t level = 0;
        got_one_group = false;
        bool got_one_level;
        करो {
            got_one_level = false;
            काष्ठा xkb_कुंजी *कुंजी;
            xkb_कुंजियाँ_foreach(कुंजी, keymap) {
                अगर (group < कुंजी->अंक_groups &&
                    level < Xkb_कुंजी_अंक_Levels(कुंजी, group)) {
                    got_one_group = got_one_level = true;
                    स्थिर xkb_keysym_गिनती_t अंक_syms =
                        कुंजी->groups[group].levels[level].अंक_syms;
                    अगर (अंक_syms > 1) {
                        क्रम (xkb_keysym_गिनती_t k = 0; k < अंक_syms; k++) {
                            अगर (कुंजी->groups[group].levels[level].s.syms[k] == sym)
                                वापस कुंजी;
                        }
                    } अन्यथा अगर (अंक_syms &&
                               कुंजी->groups[group].levels[level].s.sym == sym) {
                        वापस कुंजी;
                    }
                }
            }
            level++;
        } जबतक (got_one_level);
        group++;
    } जबतक (got_one_group);

    वापस शून्य;
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
अटल xkb_atom_t
FindAutomaticType(काष्ठा xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    स्थिर xkb_level_सूचकांक_t width = darray_आकार(groupi->levels);

#घोषणा GET_SYM(level) \
    (darray_item(groupi->levels, level).अंक_syms == 0 ? \
        XKB_कुंजी_No_प्रतीक : \
     darray_item(groupi->levels, level).अंक_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    अगर (width == 1 || width <= 0)
        वापस xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    अगर (width == 2) {
        अगर (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            वापस xkb_atom_intern_literal(ctx, "ALPHABETIC");

        अगर (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            वापस xkb_atom_intern_literal(ctx, "KEYPAD");

        वापस xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    अगर (width <= 4) {
        अगर (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_कुंजी_No_प्रतीक);

            अगर (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                वापस xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            वापस xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        अगर (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            वापस xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        वापस xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    वापस XKB_ATOM_NONE;

#अघोषित GET_SYM
}

अटल स्थिर काष्ठा xkb_कुंजी_type *
FindTypeForGroup(काष्ठा xkb_keymap *keymap, कुंजी_Info *keyi,
                 xkb_layout_सूचकांक_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    अगर (type_name == XKB_ATOM_NONE) {
        अगर (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अन्यथा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            अगर (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    अगर (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_कुंजी_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 कुंजी_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_आकार(groupi->levels));
        जाओ use_default;
    }

    darray_आकार_t i;
    क्रम (i = 0; i < keymap->अंक_types; i++)
        अगर (keymap->types[i].name == type_name)
            अवरोध;

    अगर (i >= keymap->अंक_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_कुंजी_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 कुंजी_Name_Text(keymap->ctx, keyi->name), group + 1);
        जाओ use_default;
    }

    keymap->types[i].required = true;
    वापस &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    वापस &keymap->types[0];
}

अटल bool
CopySymbolsDefToKeymap(काष्ठा xkb_keymap *keymap, SymbolsInfo *info,
                       कुंजी_Info *keyi)
{
    काष्ठा xkb_कुंजी *कुंजी;
    GroupInfo *groupi;
    स्थिर GroupInfo *group0;
    xkb_layout_सूचकांक_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    कुंजी = Xkb_कुंजी_By_Name(keymap, keyi->name, false);
    अगर (!कुंजी) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                कुंजी_Info_Text(info, keyi));
        वापस false;
    }

    /* Find the range of groups we need. */
    कुंजी->अंक_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        स्थिर bool has_explicit_type = (keyi->defined & कुंजी_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        अगर (darray_आकार(groupi->levels) > 0 || has_explicit_type)
            कुंजी->अंक_groups = i + 1;
        अगर (has_explicit_type)
            कुंजी->explicit |= EXPLICIT_TYPES;
    }

    अगर (कुंजी->अंक_groups <= 0) {
        /* A key with no group may still have other fields defined */
        अगर (keyi->defined)
            जाओ कुंजी_fields;
        अन्यथा
            वापस false;
    }

    darray_resize(keyi->groups, कुंजी->अंक_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        अगर (groupi->defined)
            जारी;

        CopyGroupInfo(groupi, group0);
    }

    कुंजी->groups = सुस्मृति(कुंजी->अंक_groups, माप(*कुंजी->groups));
    निश्चित(darray_आकार(keyi->groups) == कुंजी->अंक_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #अगर_घोषित __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < कुंजी->अंक_groups);
        #पूर्ण_अगर
        bool explicit_type = false;
        स्थिर काष्ठा xkb_कुंजी_type * स्थिर type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        अगर (type->अंक_levels < darray_आकार(groupi->levels)) {
            काष्ठा xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->अंक_levels,
                    कुंजी_Info_Text(info, keyi),
                    darray_आकार(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->अंक_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->अंक_levels);

        कुंजी->groups[i].explicit_type = explicit_type;
        कुंजी->groups[i].type = type;
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
        काष्ठा xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            चयन (leveli->अंक_syms) {
            हाल 0:
                leveli->upper = XKB_कुंजी_No_प्रतीक;
                अवरोध;
            हाल 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                अवरोध;
            शेष:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                क्रम (xkb_keysym_गिनती_t k = 0; k < leveli->अंक_syms; k++) {
                    स्थिर xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    अगर (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        अवरोध;
                    }
                }
                अगर (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = पुनः_स्मृति(leveli->s.syms,
                                             (माप_प्रकार) 2 * leveli->अंक_syms *
                                             माप(*leveli->s.syms));
                    अगर (!leveli->s.syms)
                        वापस false; /* FIXME: better handling? */
                    क्रम (xkb_keysym_गिनती_t k = 0; k < leveli->अंक_syms; k++) {
                        leveli->s.syms[leveli->अंक_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &कुंजी->groups[i].levels, शून्य);
        अगर (कुंजी->groups[i].type->अंक_levels > 1 ||
            कुंजी->groups[i].levels[0].अंक_syms > 0) {
            कुंजी->groups[i].explicit_symbols = true;
            कुंजी->explicit |= EXPLICIT_SYMBOLS;
        }
        अगर (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            कुंजी->groups[i].explicit_actions = true;
            कुंजी->explicit |= EXPLICIT_INTERP;
        }
        अगर (कुंजी->groups[i].explicit_type)
            कुंजी->explicit |= EXPLICIT_TYPES;
    }

    कुंजी->out_of_range_pending_group = keyi->out_of_range_pending_group;
    कुंजी->out_of_range_group_संख्या = keyi->out_of_range_group_संख्या;
    कुंजी->out_of_range_group_policy = keyi->out_of_range_group_policy;

कुंजी_fields:
    अगर (keyi->defined & कुंजी_FIELD_VMODMAP) {
        कुंजी->vmodmap = keyi->vmodmap;
        कुंजी->explicit |= EXPLICIT_VMODMAP;
    }

    अगर (keyi->repeat != कुंजी_REPEAT_UNDEFINED) {
        कुंजी->repeats = (keyi->repeat == कुंजी_REPEAT_YES);
        कुंजी->explicit |= EXPLICIT_REPEAT;
    }

    अगर ((keyi->defined & कुंजी_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        कुंजी->overlays = keyi->overlays;
        अगर (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = कुंजी->overlays;
            स्थिर काष्ठा xkb_कुंजी **overlays_कुंजियाँ = keyi->overlays_कुंजियाँ;
            जबतक (remaining) {
                /* isolate lowest set bit */
                स्थिर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                अगर (*overlays_कुंजियाँ) {
                    overlays_कुंजियाँ++;
                } अन्यथा {
                    /* drop current null value (overlayN=none) */
                    कुंजी->overlays &= ~lsb;
                }
            }

            अगर (कुंजी->overlays) {
                /* Steal */
                कुंजी->overlays_कुंजियाँ = keyi->overlays_कुंजियाँ;
                keyi->overlays_कुंजियाँ = शून्य;
                keyi->overlays_alloc = 0;

                कुंजी->overlays_inline = false;
                कुंजी->explicit |= EXPLICIT_OVERLAY;
            }
        } अन्यथा {
            कुंजी->overlay_कुंजी = keyi->overlay_कुंजी;
            कुंजी->overlays_inline = true;
            कुंजी->explicit |= EXPLICIT_OVERLAY;
        }
    }

    वापस true;
}

अटल bool
CopyModMapDefToKeymap(काष्ठा xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    काष्ठा xkb_कुंजी *कुंजी;

    अगर (!entry->have_प्रतीक) {
        कुंजी = Xkb_कुंजी_By_Name(keymap, entry->u.कुंजी_Name, true);
        अगर (!कुंजी) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    कुंजी_Name_Text(info->ctx, entry->u.कुंजी_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            वापस false;
        }
    }
    अन्यथा {
        कुंजी = Find_कुंजी_For_प्रतीक(keymap, entry->u.कुंजी_Sym);
        अगर (!कुंजी) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_प्रतीक,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.कुंजी_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            वापस false;
        }
    }

    /* Skip modMap None */
    अगर (entry->mods) {
        कुंजी->modmap |= entry->mods;
        entry->कुंजी_Code = कुंजी->keycode;
    }

    वापस true;
}

अटल bool
CopySymbolsToKeymap(काष्ठा xkb_keymap *keymap, SymbolsInfo *info)
{
    कुंजी_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->अंक_group_names = (xkb_layout_सूचकांक_t)darray_आकार(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, शून्य);

    darray_foreach(keyi, info->कुंजियाँ)
        अगर (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_गिनती++;

    अगर (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        काष्ठा xkb_कुंजी *कुंजी;

        xkb_कुंजियाँ_foreach(कुंजी, keymap) {
            अगर (कुंजी->name == XKB_ATOM_NONE)
                जारी;

            अगर (कुंजी->अंक_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         कुंजी_Name_Text(info->ctx, कुंजी->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        अगर (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_गिनती++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अंक_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    वापस true;
}

bool
CompileSymbols(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    अगर (file !=शून्य)
        HandleSymbolsFile(&info, file);

    अगर (info.error_गिनती != 0)
        जाओ err_info;

    अगर (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        जाओ err_info;

    ClearSymbolsInfo(&info);
    वापस true;

err_info:
    ClearSymbolsInfo(&info);
    वापस false;
}
