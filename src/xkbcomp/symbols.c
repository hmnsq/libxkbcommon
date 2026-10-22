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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"

#അന്തര്ഭാവ "action.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-numbers.h"
#അന്തര്ഭാവ "util-mem.h"
#അന്തര്ഭാവ "vmod.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"

എണ്ണൽ താക്കോൽ_repeat {
    താക്കോൽ_REPEAT_UNDEFINED = 0,
    താക്കോൽ_REPEAT_YES,
    താക്കോൽ_REPEAT_NO,
    താക്കോൽ_REPEAT_അക്കം_ENTRIES,
};
എണ്ണൽ { താക്കോൽ_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(താക്കോൽ_REPEAT_അക്കം_ENTRIES - 1 <=
              (1 << (താക്കോൽ_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

എണ്ണൽ group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

എണ്ണൽ താക്കോൽ_field {
    താക്കോൽ_FIELD_NONE = 0,
    താക്കോൽ_FIELD_REPEAT = (1 << 0),
    താക്കോൽ_FIELD_DEFAULT_TYPE = (1 << 1),
    താക്കോൽ_FIELD_GROUPINFO = (1 << 2),
    താക്കോൽ_FIELD_VMODMAP = (1 << 3),
    താക്കോൽ_FIELD_OVERLAY = (1 << 4),
    താക്കോൽ_FIELD_ALL = താക്കോൽ_FIELD_REPEAT | താക്കോൽ_FIELD_DEFAULT_TYPE
                  | താക്കോൽ_FIELD_GROUPINFO | താക്കോൽ_FIELD_VMODMAP | താക്കോൽ_FIELD_OVERLAY,
};
എണ്ണൽ { താക്കോൽ_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(താക്കോൽ_FIELD_ALL <= (1 << (താക്കോൽ_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

തരനിർവചനം ഘടന {
    darray(ഘടന xkb_level) levels;
    എണ്ണൽ group_field defined;
    xkb_atom_t type;
} GroupInfo;

എണ്ണൽ {
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
              (unsigned)താക്കോൽ_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)താക്കോൽ_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

തരനിർവചനം ഘടന {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_സംഖ്യ;
    darray(GroupInfo) groups;
    എണ്ണൽ xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    എണ്ണൽ താക്കോൽ_field defined:KEYINFO_DEFINED_BIT_FIELD;
    എണ്ണൽ merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    എണ്ണൽ താക്കോൽ_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    സംഘം {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        സ്ഥിരം ഘടന xkb_താക്കോൽ *overlay_താക്കോൽ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        സ്ഥിരം ഘടന xkb_താക്കോൽ **overlays_താക്കോലുകൾ;
    };
} താക്കോൽ_Info;

സ്ഥിതം ശൂന്യം
StealLevelInfo(ഘടന xkb_level *into, ഘടന xkb_level *from)
{
    clear_level(into);
    എങ്കിൽ (from->അക്കം_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } അല്ലെങ്കിൽ {
        into->s.sym = from->s.sym;
    }
    into->അക്കം_syms = from->അക്കം_syms;
    from->അക്കം_syms = 0;

    എങ്കിൽ (from->അക്കം_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } അല്ലെങ്കിൽ {
        into->a.action = from->a.action;
    }
    into->അക്കം_actions = from->അക്കം_actions;
    from->അക്കം_actions = 0;
}

സ്ഥിതം ശൂന്യം
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, വലുപ്പം(*groupi));
}

സ്ഥിതം ശൂന്യം
ClearGroupInfo(GroupInfo *groupi)
{
    ഘടന xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

സ്ഥിതം ശൂന്യം
CopyGroupInfo(GroupInfo *to, സ്ഥിരം GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    വേണ്ടി (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        എങ്കിൽ (darray_item(from->levels, j).അക്കം_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).അക്കം_syms,
                       വലുപ്പം(xkb_keysym_t));
        }
        എങ്കിൽ (darray_item(from->levels, j).അക്കം_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).അക്കം_actions,
                       വലുപ്പം(സംഘം xkb_action));
        }
    }
}

സ്ഥിതം ശൂന്യം
Init_താക്കോൽ_Info(ഘടന xkb_context *ctx, താക്കോൽ_Info *keyi)
{
    memset(keyi, 0, വലുപ്പം(*keyi));
    static_assert(!DEFAULT_താക്കോൽ_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_താക്കോൽ_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_താക്കോൽ = ശൂന്യ;
}

സ്ഥിതം ശൂന്യം
Clear_താക്കോൽ_Info(താക്കോൽ_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    എങ്കിൽ (keyi->overlays_alloc)
        സ്വതന്ത്രം(keyi->overlays_താക്കോലുകൾ);
}

/***====================================================================***/

തരനിർവചനം ഘടന {
    അക്ഷരം *name;         /* e.g. pc+us+inet(evdev) */
    പൂർണ്ണസംഖ്യ error_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(താക്കോൽ_Info) താക്കോലുകൾ;
    താക്കോൽ_Info default_താക്കോൽ;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    ഘടന xkb_mod_set mods;

    ഘടന xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    സ്ഥിരം ഘടന xkb_keymap_info *keymap_info;
} SymbolsInfo;

സ്ഥിതം ശൂന്യം
InitSymbolsInfo(SymbolsInfo *info, സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                unsigned പൂർണ്ണസംഖ്യ include_depth, സ്ഥിരം ഘടന xkb_mod_set *mods)
{
    memset(info, 0, വലുപ്പം(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_താക്കോൽ_Info(keymap_info->keymap.ctx, &info->default_താക്കോൽ);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

സ്ഥിതം ശൂന്യം
ClearSymbolsInfo(SymbolsInfo *info)
{
    താക്കോൽ_Info *keyi;
    സ്വതന്ത്രം(info->name);
    darray_foreach(keyi, info->താക്കോലുകൾ)
        Clear_താക്കോൽ_Info(keyi);
    darray_free(info->താക്കോലുകൾ);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_താക്കോൽ_Info(&info->default_താക്കോൽ);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
താക്കോൽ_Info_Text(SymbolsInfo *info, താക്കോൽ_Info *keyi)
{
    തിരികെ താക്കോൽ_Name_Text(info->ctx, keyi->name);
}

സ്ഥിതം bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t താക്കോൽ_name)
{
    /* First find the type of the merged group. */
    എങ്കിൽ (into->type != from->type) {
        എങ്കിൽ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        അല്ലെങ്കിൽ എങ്കിൽ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        അല്ലെങ്കിൽ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            എങ്കിൽ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, താക്കോൽ_Name_Text(info->ctx, താക്കോൽ_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    എങ്കിൽ (darray_empty(from->levels)) {
        InitGroupInfo(from);
        തിരികെ true;
    }

    എങ്കിൽ (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        തിരികെ true;
    }

    /* Merge the actions and syms. */
    സ്ഥിരം darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_എണ്ണം = 0;
    darray_size_t from_Actions_എണ്ണം = 0;
    വേണ്ടി (darray_size_t i = 0; i < levels_in_both; i++) {
        ഘടന xkb_level* സ്ഥിരം intoLevel = &darray_item(into->levels, i);
        ഘടന xkb_level* സ്ഥിരം fromLevel = &darray_item(from->levels, i);

        സ്ഥിരം bool fromHasNoKeysym = fromLevel->അക്കം_syms == 0;
        സ്ഥിരം bool fromHasNoAction = fromLevel->അക്കം_actions == 0;
        എങ്കിൽ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            തുടരുക;
        }

        സ്ഥിരം bool intoHasNoKeysym = intoLevel->അക്കം_syms == 0;
        സ്ഥിരം bool intoHasNoAction = intoLevel->അക്കം_actions == 0;
        എങ്കിൽ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_എണ്ണം++;
            from_Actions_എണ്ണം++;
            തുടരുക;
        }

        /* Possible level conflict */
        പ്രതിജ്ഞാ(intoLevel->അക്കം_syms > 0 || intoLevel->അക്കം_actions > 0);
        പ്രതിജ്ഞാ(fromLevel->അക്കം_syms > 0 || fromLevel->അക്കം_actions > 0);

        /* Handle keysyms */
        എങ്കിൽ (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            എങ്കിൽ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_ചിഹ്നം,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, താക്കോൽ_Name_Text(info->ctx, താക്കോൽ_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            എങ്കിൽ (fromHasNoKeysym) {
                /* No keysym to copy */
            } അല്ലെങ്കിൽ എങ്കിൽ (clobber) {
                /* Override: copy any defined keysym from `from` */
                എങ്കിൽ (unlikely(fromLevel->അക്കം_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    എങ്കിൽ (unlikely(intoLevel->അക്കം_syms > 1))
                        സ്വതന്ത്രം(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->അക്കം_syms = fromLevel->അക്കം_syms;
                    fromLevel->അക്കം_syms = 0;
                    from_Keysyms_എണ്ണം++;
                } അല്ലെങ്കിൽ എങ്കിൽ (fromLevel->s.sym != XKB_താക്കോൽ_No_ചിഹ്നം) {
                    /* Single defined keysym */
                    എങ്കിൽ (unlikely(intoLevel->അക്കം_syms > 1))
                        സ്വതന്ത്രം(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->അക്കം_syms = 1;
                    from_Keysyms_എണ്ണം++;
                }
            } അല്ലെങ്കിൽ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                എങ്കിൽ (unlikely(intoLevel->അക്കം_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } അല്ലെങ്കിൽ എങ്കിൽ (intoLevel->s.sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
                    /* Single undefined keysym */
                    എങ്കിൽ (unlikely(fromLevel->അക്കം_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->അക്കം_syms = fromLevel->അക്കം_syms;
                        fromLevel->അക്കം_syms = 0;
                    } അല്ലെങ്കിൽ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->അക്കം_syms = 1;
                    }
                    from_Keysyms_എണ്ണം++;
                }
            }
        }

        /* Handle actions */
        എങ്കിൽ (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            എങ്കിൽ (report && !(intoHasNoAction || fromHasNoAction)) {
                എങ്കിൽ (unlikely(intoLevel->അക്കം_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, താക്കോൽ_Name_Text(info->ctx, താക്കോൽ_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } അല്ലെങ്കിൽ {
                    സംഘം xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                താക്കോൽ_Name_Text(info->ctx, താക്കോൽ_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            എങ്കിൽ (fromHasNoAction) {
                /* No action to copy */
            } അല്ലെങ്കിൽ എങ്കിൽ (clobber) {
                /* Override: copy any defined action from `from` */
                എങ്കിൽ (unlikely(fromLevel->അക്കം_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    എങ്കിൽ (unlikely(intoLevel->അക്കം_actions > 1))
                        സ്വതന്ത്രം(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->അക്കം_actions = fromLevel->അക്കം_actions;
                    fromLevel->അക്കം_actions = 0;
                    from_Actions_എണ്ണം++;
                } അല്ലെങ്കിൽ എങ്കിൽ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    എങ്കിൽ (unlikely(intoLevel->അക്കം_actions > 1))
                        സ്വതന്ത്രം(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->അക്കം_actions = 1;
                    from_Actions_എണ്ണം++;
                }
            } അല്ലെങ്കിൽ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                എങ്കിൽ (unlikely(intoLevel->അക്കം_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } അല്ലെങ്കിൽ എങ്കിൽ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    എങ്കിൽ (unlikely(fromLevel->അക്കം_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->അക്കം_actions = fromLevel->അക്കം_actions;
                        fromLevel->അക്കം_actions = 0;
                    } അല്ലെങ്കിൽ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->അക്കം_actions = 1;
                    }
                    from_Actions_എണ്ണം++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ഘടന xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->അക്കം_syms = 0;
        level->അക്കം_actions = 0;
        from_Keysyms_എണ്ണം++;
        from_Actions_എണ്ണം++;
    }
    എങ്കിൽ (from_Keysyms_എണ്ണം) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        എങ്കിൽ (from_Keysyms_എണ്ണം == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    എങ്കിൽ (from_Actions_എണ്ണം) {
        /* Reset defined actions field if we used no action from `into` */
        എങ്കിൽ (from_Actions_എണ്ണം == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    തിരികെ true;
}

സ്ഥിതം bool
Use_New_താക്കോൽ_Field(എണ്ണൽ താക്കോൽ_field field, എണ്ണൽ താക്കോൽ_field old, എണ്ണൽ താക്കോൽ_field new,
               bool clobber, bool report, എണ്ണൽ താക്കോൽ_field *collide)
{
    എങ്കിൽ (!(old & field))
        തിരികെ (new & field);

    എങ്കിൽ (new & field) {
        എങ്കിൽ (report)
            *collide |= field;

        തിരികെ clobber;
    }

    തിരികെ false;
}

സ്ഥിതം bool
overlays_get(സ്ഥിരം താക്കോൽ_Info *info, xkb_overlay_index_t bit,
             സ്ഥിരം ഘടന xkb_താക്കോൽ **താക്കോൽ_out)
{
    എങ്കിൽ (bit >= (xkb_overlay_index_t)(വലുപ്പം(xkb_overlay_mask_t) * CHAR_BIT))
        തിരികെ false;
    സ്ഥിരം xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    എങ്കിൽ (!(info->overlays & mask)) തിരികെ false;
    എങ്കിൽ (താക്കോൽ_out) {
        എങ്കിൽ (!info->overlays_alloc) {
            /* Stored inline */
            *താക്കോൽ_out = info->overlay_താക്കോൽ;
        } അല്ലെങ്കിൽ {
            /* Stored in sparse array */
            സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            സ്ഥിരം xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
            *താക്കോൽ_out = info->overlays_താക്കോലുകൾ[index];
        }
    }
    തിരികെ true;
}

സ്ഥിതം bool
overlays_തിരുകുക(താക്കോൽ_Info *keyi, xkb_overlay_index_t bit,
                സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    എങ്കിൽ (bit >= (xkb_overlay_index_t)(വലുപ്പം(xkb_overlay_mask_t) * CHAR_BIT))
        തിരികെ false;

    സ്ഥിരം xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    എങ്കിൽ ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        എങ്കിൽ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_താക്കോൽ = താക്കോൽ;
            എങ്കിൽ (!താക്കോൽ)
                keyi->overlays_clear = true;
        } അല്ലെങ്കിൽ {
            /* Stored in sparse array */
            സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            സ്ഥിരം xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
            keyi->overlays_താക്കോലുകൾ[index] = താക്കോൽ;
        }
        തിരികെ true;
    }

    /* Insert new element */
    എങ്കിൽ (!keyi->overlays || (keyi->overlays_clear && !താക്കോൽ)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_താക്കോൽ = താക്കോൽ;
        keyi->overlays |= mask;
        keyi->overlays_clear = !താക്കോൽ;
    } അല്ലെങ്കിൽ എങ്കിൽ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        സ്ഥിരം xkb_overlay_mask_t overlays = keyi->overlays | mask;
        സ്ഥിരം xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        സ്ഥിരം ഘടന xkb_താക്കോൽ ** സ്ഥിരം tmp = calloc(alloc, വലുപ്പം(*tmp));
        എങ്കിൽ (!tmp) തിരികെ false;

        /* Copy previous entries to the buffer */
        എങ്കിൽ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            സ്ഥിരം xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
            tmp[idx] = keyi->overlay_താക്കോൽ;
        } അല്ലെങ്കിൽ {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        സ്ഥിരം xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
        tmp[idx] = താക്കോൽ;

        keyi->overlays_താക്കോലുകൾ = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } അല്ലെങ്കിൽ {
        /* Allocated items: grow heap and shift if relevant */
        സ്ഥിരം xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        സ്ഥിരം xkb_overlay_index_t എണ്ണം =
            (xkb_overlay_index_t)popcount32(overlays);
        എങ്കിൽ (എണ്ണം > keyi->overlays_alloc) {
            സ്ഥിരം xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(എണ്ണം);
            പ്രതിജ്ഞാ(alloc >= എണ്ണം);
            സ്ഥിരം ഘടന xkb_താക്കോൽ ** സ്ഥിരം tmp = realloc(
                keyi->overlays_താക്കോലുകൾ,
                (size_t)alloc * വലുപ്പം(*keyi->overlays_താക്കോലുകൾ)
            );
            എങ്കിൽ (!tmp) തിരികെ false;
            keyi->overlays_താക്കോലുകൾ = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        സ്ഥിരം xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
        എങ്കിൽ (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        എങ്കിൽ (index < എണ്ണം - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            പ്രതിജ്ഞാ(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_താക്കോലുകൾ + index + 1, keyi->overlays_താക്കോലുകൾ + index,
                    (size_t)(എണ്ണം - 1u - index) * വലുപ്പം(സ്ഥിരം ഘടന xkb_താക്കോൽ *));
        }
        keyi->overlays_താക്കോലുകൾ[index] = താക്കോൽ;
        keyi->overlays = overlays;
    }

    തിരികെ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
സ്ഥിതം bool
merge_overlays(SymbolsInfo *info, താക്കോൽ_Info *into, താക്കോൽ_Info *from,
               bool clobber, bool report, എണ്ണൽ താക്കോൽ_field *collide)
{
    എങ്കിൽ (!(from->defined & താക്കോൽ_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } അല്ലെങ്കിൽ എങ്കിൽ (!(into->defined & താക്കോൽ_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_താക്കോലുകൾ = from->overlays_താക്കോലുകൾ;
        from->overlays_താക്കോലുകൾ = ശൂന്യ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= താക്കോൽ_FIELD_OVERLAY;
    } അല്ലെങ്കിൽ എങ്കിൽ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            സ്ഥിരം xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            സ്ഥിരം xkb_overlay_index_t എണ്ണം =
                (xkb_overlay_index_t)popcount32(result_mask);

            എങ്കിൽ (എണ്ണം == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            താക്കോൽ_Info *dest = into;
            താക്കോൽ_Info *src = from;
            എങ്കിൽ (into->overlays_alloc >= എണ്ണം &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } അല്ലെങ്കിൽ എങ്കിൽ (from->overlays_alloc >= എണ്ണം) {
                dest = from;
                src = into;
                clobber = !clobber;
            } അല്ലെങ്കിൽ എങ്കിൽ (എണ്ണം > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                എങ്കിൽ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                എങ്കിൽ (!dest->overlays_alloc) {
                    സ്ഥിരം ഘടന xkb_താക്കോൽ **tmp = calloc(എണ്ണം, വലുപ്പം(*tmp));
                    എങ്കിൽ (!tmp)
                        തിരികെ false;
                    എങ്കിൽ (dest->overlay_താക്കോൽ)
                        tmp[0] = dest->overlay_താക്കോൽ;
                    dest->overlays_താക്കോലുകൾ = tmp;
                    dest->overlays_alloc = എണ്ണം;
                } അല്ലെങ്കിൽ {
                    സ്ഥിരം ഘടന xkb_താക്കോൽ **tmp =
                        realloc(dest->overlays_താക്കോലുകൾ,
                                എണ്ണം * വലുപ്പം(*dest->overlays_താക്കോലുകൾ));
                    എങ്കിൽ (!tmp)
                        തിരികെ false;
                    dest->overlays_താക്കോലുകൾ = tmp;
                    dest->overlays_alloc = എണ്ണം;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            സ്ഥിരം ഘടന xkb_താക്കോൽ **src_താക്കോലുകൾ = (src->overlays_clear)
                ? ശൂന്യ
                : (src->overlays_alloc)
                    ? src->overlays_താക്കോലുകൾ
                    : &src->overlay_താക്കോൽ;
            ആകുമ്പോൾ (remaining) {
                /* isolate lowest set bit */
                സ്ഥിരം xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                സ്ഥിരം xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                എങ്കിൽ (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                സ്ഥിരം ഘടന xkb_താക്കോൽ *src_താക്കോൽ = (src_താക്കോലുകൾ)
                    ? *(src_താക്കോലുകൾ++)
                    : ശൂന്യ;

                /* check dest */
                സ്ഥിരം ഘടന xkb_താക്കോൽ *dest_താക്കോൽ = ശൂന്യ;
                സ്ഥിരം bool conflict = overlays_get(dest, bit, &dest_താക്കോൽ);
                എങ്കിൽ (conflict) {
                    എങ്കിൽ (dest_താക്കോൽ == src_താക്കോൽ)
                       തുടരുക;
                    അല്ലെങ്കിൽ എങ്കിൽ (report)
                        *collide |= താക്കോൽ_FIELD_OVERLAY;
                }

                എങ്കിൽ ((!conflict || clobber) && !overlays_തിരുകുക(dest, bit, src_താക്കോൽ))
                    തിരികെ false;
            }

            എങ്കിൽ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                എങ്കിൽ (into->overlays_alloc)
                    സ്വതന്ത്രം(into->overlays_താക്കോലുകൾ);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                എങ്കിൽ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_താക്കോലുകൾ = from->overlays_താക്കോലുകൾ;
                    from->overlays_താക്കോലുകൾ = ശൂന്യ;
                    from->overlays_alloc = 0;
                } അല്ലെങ്കിൽ {
                    into->overlay_താക്കോൽ = from->overlay_താക്കോൽ;
                }
            }
        } അല്ലെങ്കിൽ {
            /* Disjoint overlays */

            എങ്കിൽ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_താക്കോൽ == from->overlay_താക്കോൽ) {
                /* Indentical: do nothing */
                തിരികെ true;
            }
            എങ്കിൽ (!(into->overlays & from->overlays)) {
                എങ്കിൽ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_താക്കോൽ = from->overlay_താക്കോൽ;
                    തിരികെ true;
                } അല്ലെങ്കിൽ എങ്കിൽ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    തിരികെ true;
                }
            }
            /* Conflict */
            എങ്കിൽ (report)
                *collide |= താക്കോൽ_FIELD_OVERLAY;
            എങ്കിൽ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_താക്കോൽ = from->overlay_താക്കോൽ;
            }
        }
    }
    തിരികെ true;
}

സ്ഥിതം bool
Merge_താക്കോലുകൾ(SymbolsInfo *info, താക്കോൽ_Info *into, താക്കോൽ_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    എണ്ണൽ താക്കോൽ_field collide = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ verbosity = xkb_context_get_log_verbosity(info->ctx);
    സ്ഥിരം bool clobber = (from->merge != MERGE_AUGMENT);
    സ്ഥിരം bool report = (same_file && verbosity > 0) || verbosity > 9;

    എങ്കിൽ (from->merge == MERGE_REPLACE) {
        Clear_താക്കോൽ_Info(into);
        *into = *from;
        Init_താക്കോൽ_Info(info->ctx, from);
        തിരികെ true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    വേണ്ടി (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    വേണ്ടി (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    എങ്കിൽ (Use_New_താക്കോൽ_Field(താക്കോൽ_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= താക്കോൽ_FIELD_VMODMAP;
    }
    എങ്കിൽ (Use_New_താക്കോൽ_Field(താക്കോൽ_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= താക്കോൽ_FIELD_REPEAT;
    }
    എങ്കിൽ (Use_New_താക്കോൽ_Field(താക്കോൽ_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= താക്കോൽ_FIELD_DEFAULT_TYPE;
    }
    എങ്കിൽ (Use_New_താക്കോൽ_Field(താക്കോൽ_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_സംഖ്യ = from->out_of_range_group_സംഖ്യ;
        into->defined |= താക്കോൽ_FIELD_GROUPINFO;
    }

    എങ്കിൽ (!merge_overlays(info, into, from, clobber, report, &collide))
        തിരികെ false;

    എങ്കിൽ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 താക്കോൽ_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_താക്കോൽ_Info(from);
    Init_താക്കോൽ_Info(info->ctx, from);
    തിരികെ true;
}

സ്ഥിതം xkb_atom_t
Xkb_Resolve_താക്കോൽ_Alias(സ്ഥിരം ഘടന xkb_keymap *keymap, xkb_atom_t name)
{
    എങ്കിൽ (name < keymap->അക്കം_താക്കോൽ_names) {
        സ്ഥിരം KeycodeMatch match = keymap->താക്കോൽ_names[name];
        എങ്കിൽ (match.found && match.is_alias) {
            തിരികെ match.alias.real;
        }
    }
    തിരികെ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
സ്ഥിതം bool
Add_താക്കോൽ_Symbols(SymbolsInfo *info, താക്കോൽ_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_താക്കോൽ_Alias(&info->keymap_info->keymap, keyi->name);

    താക്കോൽ_Info *iter;
    darray_foreach(iter, info->താക്കോലുകൾ)
        എങ്കിൽ (iter->name == keyi->name)
            തിരികെ Merge_താക്കോലുകൾ(info, iter, keyi, same_file);

    darray_append(info->താക്കോലുകൾ, *keyi);
    Init_താക്കോൽ_Info(info->ctx, keyi);
    തിരികെ true;
}

സ്ഥിതം bool
AddModMapEntry(SymbolsInfo *info, സ്ഥിരം ModMapEntry *new)
{
    സ്ഥിരം bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        എങ്കിൽ ((new->have_ചിഹ്നം != old->have_ചിഹ്നം) ||
            (new->have_ചിഹ്നം && new->u.താക്കോൽ_Sym != old->u.താക്കോൽ_Sym) ||
            (!new->have_ചിഹ്നം && new->u.താക്കോൽ_Name != old->u.താക്കോൽ_Name))
            തുടരുക;

        എങ്കിൽ (new->mods == old->mods)
            തിരികെ true;

        സ്ഥിരം xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        സ്ഥിരം xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        എങ്കിൽ (new->have_ചിഹ്നം) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.താക്കോൽ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } അല്ലെങ്കിൽ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    താക്കോൽ_Name_Text(info->ctx, new->u.താക്കോൽ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        തിരികെ true;
    }

    darray_append(info->modmaps, *new);

    തിരികെ true;
}

/***====================================================================***/

സ്ഥിതം ശൂന്യം
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     എണ്ണൽ merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    എങ്കിൽ (from->error_എണ്ണം > 0) {
        into->error_എണ്ണം += from->error_എണ്ണം;
        തിരികെ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    എങ്കിൽ (into->name == ശൂന്യ) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    വേണ്ടി (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        എങ്കിൽ (!darray_item(from->group_names, i))
            തുടരുക;

        എങ്കിൽ (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            തുടരുക;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    എങ്കിൽ (darray_empty(into->താക്കോലുകൾ)) {
        into->താക്കോലുകൾ = from->താക്കോലുകൾ;
        darray_init(from->താക്കോലുകൾ);
    }
    അല്ലെങ്കിൽ {
        താക്കോൽ_Info *keyi;
        darray_foreach(keyi, from->താക്കോലുകൾ) {
            keyi->merge = merge;
            എങ്കിൽ (!Add_താക്കോൽ_Symbols(into, keyi, false))
                into->error_എണ്ണം++;
        }
    }

    എങ്കിൽ (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    അല്ലെങ്കിൽ {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            എങ്കിൽ (!AddModMapEntry(into, mm))
                into->error_എണ്ണം++;
        }
    }
}

സ്ഥിതം ശൂന്യം
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

സ്ഥിതം bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    എങ്കിൽ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_എണ്ണം += 10;
        തിരികെ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    വേണ്ടി (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        അക്ഷരം path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, വലുപ്പം(path));
        എങ്കിൽ (!file) {
            info->error_എണ്ണം += 10;
            ClearSymbolsInfo(&included);
            തിരികെ false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        എങ്കിൽ (stmt->modifier) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            എങ്കിൽ (എണ്ണം <= 0 || *(stmt->modifier + എണ്ണം) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } അല്ലെങ്കിൽ {
                next_incl.explicit_group--;
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (info->keymap_info->keymap.അക്കം_groups != 0 &&
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
        അല്ലെങ്കിൽ {
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

    തിരികെ (info->error_എണ്ണം == 0);
}

സ്ഥിതം bool
GetGroupIndex(SymbolsInfo *info, താക്കോൽ_Info *keyi, വ്യഞ്ജകം_Def *arrayNdx,
              എണ്ണൽ group_field field, xkb_layout_index_t *ndx_rtrn)
{
    പ്രതിജ്ഞാ (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    സ്ഥിരം അക്ഷരം *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    എങ്കിൽ (arrayNdx == ശൂന്യ) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            എങ്കിൽ (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                തിരികെ true;
            }
        }

        എങ്കിൽ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, താക്കോൽ_Info_Text(info, keyi), info->max_groups, name);
            തിരികെ false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        തിരികെ true;
    }

    എങ്കിൽ (വ്യഞ്ജകം_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, ശൂന്യ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, താക്കോൽ_Info_Text(info, keyi));
        തിരികെ false;
    }

    (*ndx_rtrn)--;
    എങ്കിൽ (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    തിരികെ true;
}

സ്ഥിതം bool
Add_Symbols_To_താക്കോൽ(SymbolsInfo *info, താക്കോൽ_Info *keyi, വ്യഞ്ജകം_Def *arrayNdx,
                വ്യഞ്ജകം_Def *മൂല്യം)
{
    xkb_layout_index_t ndx = 0;

    എങ്കിൽ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        തിരികെ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        തിരികെ true;
    }

    എങ്കിൽ (മൂല്യം->common.type != STMT_വ്യഞ്ജകം_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(മൂല്യം->common.type), ndx + 1,
                താക്കോൽ_Info_Text(info, keyi));
        തിരികെ false;
    }

    എങ്കിൽ (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_താക്കോൽ_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                താക്കോൽ_Info_Text(info, keyi), ndx + 1);
        തിരികെ false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    വേണ്ടി (വ്യഞ്ജകം_Keysym_List *keysymList = (വ്യഞ്ജകം_Keysym_List *) മൂല്യം;
         keysymList;
         keysymList = (വ്യഞ്ജകം_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        എങ്കിൽ (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    എങ്കിൽ (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    എങ്കിൽ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    വേണ്ടി (വ്യഞ്ജകം_Keysym_List *keysymList = (വ്യഞ്ജകം_Keysym_List *) മൂല്യം;
         keysymList && level < nLevels;
         keysymList = (വ്യഞ്ജകം_Keysym_List *) keysymList->common.next, level++) {
        ഘടന xkb_level *leveli = &darray_item(groupi->levels, level);
        പ്രതിജ്ഞാ(leveli->അക്കം_syms == 0);

        എങ്കിൽ (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    താക്കോൽ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            തിരികെ false;
        }

        leveli->അക്കം_syms = (xkb_keysym_എണ്ണം_t) darray_size(keysymList->syms);
        തിരഞ്ഞെടുപ്പ് (leveli->അക്കം_syms) {
        അവസ്ഥ 0:
            leveli->s.sym = XKB_താക്കോൽ_No_ചിഹ്നം;
            നിർത്തുക;
        അവസ്ഥ 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            പ്രതിജ്ഞാ(leveli->s.sym != XKB_താക്കോൽ_No_ചിഹ്നം);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ശൂന്യ);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            വേണ്ടി (xkb_keysym_എണ്ണം_t k = 0; k < leveli->അക്കം_syms; k++)
                പ്രതിജ്ഞാ(leveli->s.syms[k] != XKB_താക്കോൽ_No_ചിഹ്നം);
#endif
        }
    }

    തിരികെ true;
}

സ്ഥിതം bool
Add_Actions_To_താക്കോൽ(SymbolsInfo *info, താക്കോൽ_Info *keyi, വ്യഞ്ജകം_Def *arrayNdx,
                വ്യഞ്ജകം_Def *മൂല്യം)
{
    xkb_layout_index_t ndx = 0;

    എങ്കിൽ (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        തിരികെ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        തിരികെ true;
    }

    എങ്കിൽ (മൂല്യം->common.type != STMT_വ്യഞ്ജകം_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 മൂല്യം->common.type, ndx, താക്കോൽ_Info_Text(info, keyi));
        തിരികെ false;
    }

    എങ്കിൽ (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 താക്കോൽ_Info_Text(info, keyi), ndx);
        തിരികെ false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    വേണ്ടി (ParseCommon *p = &മൂല്യം->common; p; p = p->next)
        nLevels++;
    എങ്കിൽ (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    വേണ്ടി (വ്യഞ്ജകം_Action_List *actionList = (വ്യഞ്ജകം_Action_List *) മൂല്യം;
         actionList;
         actionList = (വ്യഞ്ജകം_Action_List *) actionList->common.next, level++) {
        ഘടന xkb_level *leveli = &darray_item(groupi->levels, level);
        പ്രതിജ്ഞാ(leveli->അക്കം_actions == 0);

        unsigned പൂർണ്ണസംഖ്യ അക്കം_actions = 0;
        വേണ്ടി (വ്യഞ്ജകം_Def *act = actionList->actions;
             act; act = (വ്യഞ്ജകം_Def *) act->common.next)
            അക്കം_actions++;

        എങ്കിൽ (unlikely(അക്കം_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    താക്കോൽ_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, അക്കം_actions);
            തിരികെ false;
        }

        /* Parse actions and add only defined actions */
        darray(സംഘം xkb_action) actions = darray_new();
        വേണ്ടി (വ്യഞ്ജകം_Def *act = actionList->actions;
             act; act = (വ്യഞ്ജകം_Def *) act->common.next) {
            സംഘം xkb_action toAct = { 0 };
            സ്ഥിരം എണ്ണൽ xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            എങ്കിൽ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_മൂല്യം,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        താക്കോൽ_Info_Text(info, keyi), ndx + 1, level + 1);
                എങ്കിൽ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    തിരികെ false;
                } അല്ലെങ്കിൽ {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            എങ്കിൽ (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } അല്ലെങ്കിൽ എങ്കിൽ (likely(അക്കം_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->അക്കം_actions = 1;
                leveli->a.action = toAct;
                പോകുക next_level;
            } അല്ലെങ്കിൽ {
                darray_append(actions, toAct);
            }
        }
        എങ്കിൽ (darray_empty(actions)) {
            leveli->അക്കം_actions = 0;
        } അല്ലെങ്കിൽ എങ്കിൽ (likely(darray_size(actions) > 1)) {
            leveli->അക്കം_actions = (xkb_action_എണ്ണം_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ശൂന്യ);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < leveli->അക്കം_actions; k++)
                പ്രതിജ്ഞാ(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } അല്ലെങ്കിൽ {
            /* Unlikely: some actions were dropped and only one remains */
            പ്രതിജ്ഞാ(അക്കം_actions > darray_size(actions));
            leveli->അക്കം_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            പ്രതിജ്ഞാ(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        എങ്കിൽ (leveli->അക്കം_actions > 0 || leveli->അക്കം_syms > 0)
            nonEmptyLevels = level + 1;
    }

    എങ്കിൽ (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        എങ്കിൽ (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        അല്ലെങ്കിൽ
            darray_free(groupi->levels);
    }

    തിരികെ true;
}

സ്ഥിതം സ്ഥിരം LookupEntry repeatEntries[] = {
    { "true", താക്കോൽ_REPEAT_YES },
    { "yes", താക്കോൽ_REPEAT_YES },
    { "on", താക്കോൽ_REPEAT_YES },
    { "false", താക്കോൽ_REPEAT_NO },
    { "no", താക്കോൽ_REPEAT_NO },
    { "off", താക്കോൽ_REPEAT_NO },
    { "default", താക്കോൽ_REPEAT_UNDEFINED },
    { ശൂന്യ, 0 }
};

/** Handle overlay<N> = <key> entry */
സ്ഥിതം bool
വ്യഞ്ജകം_Resolve_Overlay_Entry(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                        സ്ഥിരം അക്ഷരം *field, സ്ഥിരം വ്യഞ്ജകം_Def *arrayNdx,
                        സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, താക്കോൽ_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        സ്ഥിരം ഘടന xkb_താക്കോൽ **താക്കോൽ_rtrn)
{
    /* Overlay index */
    എങ്കിൽ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, താക്കോൽ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        തിരികെ false;
    }
    static_assert(വലുപ്പം(xkb_overlay_index_t) == വലുപ്പം(uint8_t), "");
    സ്ഥിരം size_t prefix = വലുപ്പം("overlay") - 1;
    സ്ഥിരം size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    എങ്കിൽ (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (പൂർണ്ണസംഖ്യ)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, താക്കോൽ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        തിരികെ false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYNAME_LITERAL:
        *താക്കോൽ_rtrn = Xkb_താക്കോൽ_By_Name(&keymap_info->keymap, വ്യഞ്ജകം->താക്കോൽ_name.താക്കോൽ_name,
                                 false);
        എങ്കിൽ (!*താക്കോൽ_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, വ്യഞ്ജകം->താക്കോൽ_name.താക്കോൽ_name),
                    field, താക്കോൽ_Name_Text(keymap_info->keymap.ctx, keyi->name));
            തിരികെ false;
        }
        തിരികെ true;
    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT: {
        സ്ഥിരം അക്ഷരം * സ്ഥിരം id =
            xkb_atom_text(keymap_info->keymap.ctx, വ്യഞ്ജകം->ident.ident);
        എങ്കിൽ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *താക്കോൽ_rtrn = ശൂന്യ;
            തിരികെ true;
        } അല്ലെങ്കിൽ എങ്കിൽ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *താക്കോൽ_rtrn = ശൂന്യ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            തിരികെ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_മൂല്യം,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, താക്കോൽ_Name_Text(keymap_info->keymap.ctx, keyi->name));
        തിരികെ false;
    }
    സ്ഥിരസ്ഥിതി:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_മൂല്യം,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_വ്യഞ്ജകം_KEYNAME_LITERAL),
                field, താക്കോൽ_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;
    }
}

സ്ഥിതം bool
SetSymbolsField(SymbolsInfo *info, താക്കോൽ_Info *keyi, സ്ഥിരം അക്ഷരം *field,
                വ്യഞ്ജകം_Def *arrayNdx, വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    വ്യഞ്ജകം_Def * restrict സ്ഥിരം മൂല്യം = *മൂല്യം_ptr;
    എങ്കിൽ (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_String(info->ctx, മൂല്യം, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            തിരികെ false;
        }

        എങ്കിൽ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= താക്കോൽ_FIELD_DEFAULT_TYPE;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (വ്യഞ്ജകം_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, ശൂന്യ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }
        അല്ലെങ്കിൽ {
            ndx--;
            എങ്കിൽ (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                പ്രതിജ്ഞാ(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "symbols")) {
        തിരികെ Add_Symbols_To_താക്കോൽ(info, keyi, arrayNdx, മൂല്യം);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "actions")) {
        തിരികെ Add_Actions_To_താക്കോൽ(info, keyi, arrayNdx, മൂല്യം);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, മൂല്യം, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(മൂല്യം->common.type),
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= താക്കോൽ_FIELD_VMODMAP;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                താക്കോൽ_Info_Text(info, keyi));
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                താക്കോൽ_Info_Text(info, keyi));
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                താക്കോൽ_Info_Text(info, keyi));
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = ശൂന്യ;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *മൂല്യം_ptr, keyi, &overlay, &താക്കോൽ))
            തിരികെ false;
        എങ്കിൽ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            തിരികെ true;
        } അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ && താക്കോൽ->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, താക്കോൽ_Info_Text(info, keyi));
        } അല്ലെങ്കിൽ {
            സ്ഥിരം ഘടന xkb_താക്കോൽ *prev;
            എങ്കിൽ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                എങ്കിൽ (താക്കോൽ != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             താക്കോൽ_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? താക്കോൽ_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (താക്കോൽ
                                 ? താക്കോൽ_Name_Text(info->ctx, താക്കോൽ->name)
                                 : "none"));
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                എങ്കിൽ (!overlays_തിരുകുക(keyi, overlay, താക്കോൽ))
                    തിരികെ false;
                keyi->defined |= താക്കോൽ_FIELD_OVERLAY;
            } അല്ലെങ്കിൽ {
                /* Handle disjoint overlays mode */
                സ്ഥിരം xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                എങ്കിൽ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    എങ്കിൽ (താക്കോൽ) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_താക്കോൽ = താക്കോൽ;
                    } അല്ലെങ്കിൽ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= താക്കോൽ_FIELD_OVERLAY;
                } അല്ലെങ്കിൽ എങ്കിൽ (keyi->overlays) {
                    എങ്കിൽ (താക്കോൽ) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                താക്കോൽ_Info_Text(info, keyi), keyi->overlays,
                                താക്കോൽ_Name_Text(info->ctx, keyi->overlay_താക്കോൽ->name),
                                overlay + 1, താക്കോൽ_Name_Text(info->ctx, താക്കോൽ->name));
                        തിരികെ !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_മൂല്യം_MISMATCH);
                    } അല്ലെങ്കിൽ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Enum(info->ctx, മൂല്യം, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_മൂല്യം,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }

        keyi->repeat = (എണ്ണൽ താക്കോൽ_repeat) val;
        keyi->defined |= താക്കോൽ_FIELD_REPEAT;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Boolean(info->ctx, മൂല്യം, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_മൂല്യം,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= താക്കോൽ_FIELD_GROUPINFO;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Boolean(info->ctx, മൂല്യം, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_മൂല്യം,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= താക്കോൽ_FIELD_GROUPINFO;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        എങ്കിൽ (വ്യഞ്ജകം_Resolve_Group(info->keymap_info, മൂല്യം, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    താക്കോൽ_Info_Text(info, keyi));
            തിരികെ false;
        }

        എങ്കിൽ (pending) {
            keyi->out_of_range_pending_group = true;
            സ്ഥിരം darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ഘടന pending_computation) {
                    .വ്യഞ്ജകം = *മൂല്യം_ptr,
                    .computed = false,
                    .മൂല്യം = 0,
                }
            );
            *മൂല്യം_ptr = ശൂന്യ;
            static_assert(വലുപ്പം(keyi->out_of_range_group_സംഖ്യ) ==
                          വലുപ്പം(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_സംഖ്യ = pending_index;
        } അല്ലെങ്കിൽ {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_സംഖ്യ = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= താക്കോൽ_FIELD_GROUPINFO;
    }
    അല്ലെങ്കിൽ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        തിരികെ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_താക്കോൽ_FIELDS);
    }

    തിരികെ true;
}

സ്ഥിതം bool
SetGroupName(SymbolsInfo *info, വ്യഞ്ജകം_Def *arrayNdx, വ്യഞ്ജകം_Def *മൂല്യം,
             എണ്ണൽ merge_mode merge)
{
    എങ്കിൽ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        തിരികെ false;
    }

    xkb_layout_index_t group = 0;
    എങ്കിൽ (വ്യഞ്ജകം_Resolve_Group(info->keymap_info, arrayNdx, false, &group, ശൂന്യ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        തിരികെ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_String(info->ctx, മൂല്യം, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        തിരികെ false;
    }

    xkb_layout_index_t group_to_use;
    എങ്കിൽ (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    അല്ലെങ്കിൽ {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        തിരികെ false;
    }

    എങ്കിൽ (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        പ്രതിജ്ഞാ(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } അല്ലെങ്കിൽ {
        സ്ഥിരം xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        എങ്കിൽ (old_name != XKB_ATOM_NONE && old_name != name) {
            സ്ഥിരം bool replace = (merge != MERGE_AUGMENT);
            സ്ഥിരം xkb_atom_t use    = (replace ? name : old_name);
            സ്ഥിരം xkb_atom_t ignore = (replace ? old_name : name);
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

    തിരികെ true;
}

സ്ഥിതം bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *arrayNdx;
    bool ret;

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        തിരികെ false;

    എങ്കിൽ (elem && istreq(elem, "key")) {
        താക്കോൽ_Info temp = {0};
        Init_താക്കോൽ_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->മൂല്യം) &&
              Merge_താക്കോലുകൾ(info, &info->default_താക്കോൽ, &temp, true);
        Clear_താക്കോൽ_Info(&temp);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->മൂല്യം, stmt->merge);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->മൂല്യം, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } അല്ലെങ്കിൽ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        തിരികെ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    തിരികെ ret;
}

സ്ഥിതം bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, താക്കോൽ_Info *keyi)
{
    bool all_valid_entries = true;

    വേണ്ടി (; def; def = (VarDef *) def->common.next) {
        സ്ഥിരം അക്ഷരം *field;
        വ്യഞ്ജകം_Def *arrayNdx;
        bool ok = true;

        എങ്കിൽ (!def->name) {
            എങ്കിൽ (unlikely(!def->മൂല്യം) || def->മൂല്യം->common.type != STMT_വ്യഞ്ജകം_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            അല്ലെങ്കിൽ
                field = "actions";
            arrayNdx = ശൂന്യ;
        }
        അല്ലെങ്കിൽ {
            സ്ഥിരം അക്ഷരം *elem;
            ok = വ്യഞ്ജകം_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            എങ്കിൽ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        എങ്കിൽ (unlikely(!def->മൂല്യം)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        എങ്കിൽ (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->മൂല്യം))
            all_valid_entries = false;
    }

    തിരികെ all_valid_entries;
}

സ്ഥിതം bool
SetExplicitGroup(SymbolsInfo *info, താക്കോൽ_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    എങ്കിൽ (info->explicit_group == XKB_LAYOUT_INVALID)
        തിരികെ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        എങ്കിൽ (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    എങ്കിൽ (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, താക്കോൽ_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    എങ്കിൽ (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    തിരികെ true;
}

സ്ഥിതം bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    താക്കോൽ_Info keyi;

    keyi = info->default_താക്കോൽ;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_താക്കോൽ.groups);
    വേണ്ടി (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_താക്കോൽ.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->താക്കോൽ_Name;

    എങ്കിൽ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_താക്കോൽ_Symbols(info, &keyi, true)) {
        തിരികെ true;
    }

    Clear_താക്കോൽ_Info(&keyi);
    info->error_എണ്ണം++;
    തിരികെ false;
}

സ്ഥിതം bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    ഘടന xkb_context *ctx = info->ctx;

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        തിരികെ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    പ്രതിജ്ഞാ(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_വ്യഞ്ജകം_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    വേണ്ടി (വ്യഞ്ജകം_Def *താക്കോൽ = def->താക്കോലുകൾ; താക്കോൽ; താക്കോൽ = (വ്യഞ്ജകം_Def *) താക്കോൽ->common.next) {
        എങ്കിൽ (താക്കോൽ->common.type == STMT_വ്യഞ്ജകം_KEYNAME_LITERAL) {
            tmp.have_ചിഹ്നം = false;
            tmp.u.താക്കോൽ_Name = താക്കോൽ->താക്കോൽ_name.താക്കോൽ_name;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->common.type == STMT_വ്യഞ്ജകം_KEYSYM_LITERAL) {
            എങ്കിൽ (താക്കോൽ->keysym.keysym == XKB_താക്കോൽ_No_ചിഹ്നം) {
                /* Invalid keysym: ignore. Error message already printed */
                തുടരുക;
            } അല്ലെങ്കിൽ {
                tmp.have_ചിഹ്നം = true;
                tmp.u.താക്കോൽ_Sym = താക്കോൽ->keysym.keysym;
            }
        }
        അല്ലെങ്കിൽ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            തുടരുക;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    തിരികെ ok;
}

സ്ഥിതം ശൂന്യം
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    സ്വതന്ത്രം(info->name);
    info->name = strdup_safe(file->name);

    വേണ്ടി (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        തിരഞ്ഞെടുപ്പ് (stmt->type) {
        അവസ്ഥ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_UNKNOWN_DECLARATION:
        അവസ്ഥ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            നിർത്തുക;
        }

        എങ്കിൽ (!ok)
            info->error_എണ്ണം++;

        എങ്കിൽ (info->error_എണ്ണം > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            നിർത്തുക;
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
സ്ഥിതം ഘടന xkb_താക്കോൽ *
Find_താക്കോൽ_For_ചിഹ്നം(ഘടന xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    ചെയ്യുക {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        ചെയ്യുക {
            got_one_level = false;
            ഘടന xkb_താക്കോൽ *താക്കോൽ;
            xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
                എങ്കിൽ (group < താക്കോൽ->അക്കം_groups &&
                    level < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, group)) {
                    got_one_group = got_one_level = true;
                    സ്ഥിരം xkb_keysym_എണ്ണം_t അക്കം_syms =
                        താക്കോൽ->groups[group].levels[level].അക്കം_syms;
                    എങ്കിൽ (അക്കം_syms > 1) {
                        വേണ്ടി (xkb_keysym_എണ്ണം_t k = 0; k < അക്കം_syms; k++) {
                            എങ്കിൽ (താക്കോൽ->groups[group].levels[level].s.syms[k] == sym)
                                തിരികെ താക്കോൽ;
                        }
                    } അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_syms &&
                               താക്കോൽ->groups[group].levels[level].s.sym == sym) {
                        തിരികെ താക്കോൽ;
                    }
                }
            }
            level++;
        } ആകുമ്പോൾ (got_one_level);
        group++;
    } ആകുമ്പോൾ (got_one_group);

    തിരികെ ശൂന്യ;
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
സ്ഥിതം xkb_atom_t
FindAutomaticType(ഘടന xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    സ്ഥിരം xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).അക്കം_syms == 0 ? \
        XKB_താക്കോൽ_No_ചിഹ്നം : \
     darray_item(groupi->levels, level).അക്കം_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    എങ്കിൽ (width == 1 || width <= 0)
        തിരികെ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    എങ്കിൽ (width == 2) {
        എങ്കിൽ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            തിരികെ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        എങ്കിൽ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            തിരികെ xkb_atom_intern_literal(ctx, "KEYPAD");

        തിരികെ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    എങ്കിൽ (width <= 4) {
        എങ്കിൽ (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_താക്കോൽ_No_ചിഹ്നം);

            എങ്കിൽ (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                തിരികെ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            തിരികെ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        എങ്കിൽ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            തിരികെ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        തിരികെ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    തിരികെ XKB_ATOM_NONE;

#undef GET_SYM
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ_type *
FindTypeForGroup(ഘടന xkb_keymap *keymap, താക്കോൽ_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    എങ്കിൽ (type_name == XKB_ATOM_NONE) {
        എങ്കിൽ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        അല്ലെങ്കിൽ {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            എങ്കിൽ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    എങ്കിൽ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_താക്കോൽ_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 താക്കോൽ_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        പോകുക use_default;
    }

    darray_size_t i;
    വേണ്ടി (i = 0; i < keymap->അക്കം_types; i++)
        എങ്കിൽ (keymap->types[i].name == type_name)
            നിർത്തുക;

    എങ്കിൽ (i >= keymap->അക്കം_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_താക്കോൽ_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 താക്കോൽ_Name_Text(keymap->ctx, keyi->name), group + 1);
        പോകുക use_default;
    }

    keymap->types[i].required = true;
    തിരികെ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    തിരികെ &keymap->types[0];
}

സ്ഥിതം bool
CopySymbolsDefToKeymap(ഘടന xkb_keymap *keymap, SymbolsInfo *info,
                       താക്കോൽ_Info *keyi)
{
    ഘടന xkb_താക്കോൽ *താക്കോൽ;
    GroupInfo *groupi;
    സ്ഥിരം GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    താക്കോൽ = Xkb_താക്കോൽ_By_Name(keymap, keyi->name, false);
    എങ്കിൽ (!താക്കോൽ) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                താക്കോൽ_Info_Text(info, keyi));
        തിരികെ false;
    }

    /* Find the range of groups we need. */
    താക്കോൽ->അക്കം_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        സ്ഥിരം bool has_explicit_type = (keyi->defined & താക്കോൽ_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        എങ്കിൽ (darray_size(groupi->levels) > 0 || has_explicit_type)
            താക്കോൽ->അക്കം_groups = i + 1;
        എങ്കിൽ (has_explicit_type)
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
    }

    എങ്കിൽ (താക്കോൽ->അക്കം_groups <= 0) {
        /* A key with no group may still have other fields defined */
        എങ്കിൽ (keyi->defined)
            പോകുക താക്കോൽ_fields;
        അല്ലെങ്കിൽ
            തിരികെ false;
    }

    darray_resize(keyi->groups, താക്കോൽ->അക്കം_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        എങ്കിൽ (groupi->defined)
            തുടരുക;

        CopyGroupInfo(groupi, group0);
    }

    താക്കോൽ->groups = calloc(താക്കോൽ->അക്കം_groups, വലുപ്പം(*താക്കോൽ->groups));
    പ്രതിജ്ഞാ(darray_size(keyi->groups) == താക്കോൽ->അക്കം_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < താക്കോൽ->അക്കം_groups);
        #endif
        bool explicit_type = false;
        സ്ഥിരം ഘടന xkb_താക്കോൽ_type * സ്ഥിരം type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        എങ്കിൽ (type->അക്കം_levels < darray_size(groupi->levels)) {
            ഘടന xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->അക്കം_levels,
                    താക്കോൽ_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->അക്കം_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->അക്കം_levels);

        താക്കോൽ->groups[i].explicit_type = explicit_type;
        താക്കോൽ->groups[i].type = type;
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
        ഘടന xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            തിരഞ്ഞെടുപ്പ് (leveli->അക്കം_syms) {
            അവസ്ഥ 0:
                leveli->upper = XKB_താക്കോൽ_No_ചിഹ്നം;
                നിർത്തുക;
            അവസ്ഥ 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                വേണ്ടി (xkb_keysym_എണ്ണം_t k = 0; k < leveli->അക്കം_syms; k++) {
                    സ്ഥിരം xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    എങ്കിൽ (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        നിർത്തുക;
                    }
                }
                എങ്കിൽ (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->അക്കം_syms *
                                             വലുപ്പം(*leveli->s.syms));
                    എങ്കിൽ (!leveli->s.syms)
                        തിരികെ false; /* FIXME: better handling? */
                    വേണ്ടി (xkb_keysym_എണ്ണം_t k = 0; k < leveli->അക്കം_syms; k++) {
                        leveli->s.syms[leveli->അക്കം_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &താക്കോൽ->groups[i].levels, ശൂന്യ);
        എങ്കിൽ (താക്കോൽ->groups[i].type->അക്കം_levels > 1 ||
            താക്കോൽ->groups[i].levels[0].അക്കം_syms > 0) {
            താക്കോൽ->groups[i].explicit_symbols = true;
            താക്കോൽ->explicit |= EXPLICIT_SYMBOLS;
        }
        എങ്കിൽ (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            താക്കോൽ->groups[i].explicit_actions = true;
            താക്കോൽ->explicit |= EXPLICIT_INTERP;
        }
        എങ്കിൽ (താക്കോൽ->groups[i].explicit_type)
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
    }

    താക്കോൽ->out_of_range_pending_group = keyi->out_of_range_pending_group;
    താക്കോൽ->out_of_range_group_സംഖ്യ = keyi->out_of_range_group_സംഖ്യ;
    താക്കോൽ->out_of_range_group_policy = keyi->out_of_range_group_policy;

താക്കോൽ_fields:
    എങ്കിൽ (keyi->defined & താക്കോൽ_FIELD_VMODMAP) {
        താക്കോൽ->vmodmap = keyi->vmodmap;
        താക്കോൽ->explicit |= EXPLICIT_VMODMAP;
    }

    എങ്കിൽ (keyi->repeat != താക്കോൽ_REPEAT_UNDEFINED) {
        താക്കോൽ->repeats = (keyi->repeat == താക്കോൽ_REPEAT_YES);
        താക്കോൽ->explicit |= EXPLICIT_REPEAT;
    }

    എങ്കിൽ ((keyi->defined & താക്കോൽ_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        താക്കോൽ->overlays = keyi->overlays;
        എങ്കിൽ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = താക്കോൽ->overlays;
            സ്ഥിരം ഘടന xkb_താക്കോൽ **overlays_താക്കോലുകൾ = keyi->overlays_താക്കോലുകൾ;
            ആകുമ്പോൾ (remaining) {
                /* isolate lowest set bit */
                സ്ഥിരം xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                എങ്കിൽ (*overlays_താക്കോലുകൾ) {
                    overlays_താക്കോലുകൾ++;
                } അല്ലെങ്കിൽ {
                    /* drop current null value (overlayN=none) */
                    താക്കോൽ->overlays &= ~lsb;
                }
            }

            എങ്കിൽ (താക്കോൽ->overlays) {
                /* Steal */
                താക്കോൽ->overlays_താക്കോലുകൾ = keyi->overlays_താക്കോലുകൾ;
                keyi->overlays_താക്കോലുകൾ = ശൂന്യ;
                keyi->overlays_alloc = 0;

                താക്കോൽ->overlays_inline = false;
                താക്കോൽ->explicit |= EXPLICIT_OVERLAY;
            }
        } അല്ലെങ്കിൽ {
            താക്കോൽ->overlay_താക്കോൽ = keyi->overlay_താക്കോൽ;
            താക്കോൽ->overlays_inline = true;
            താക്കോൽ->explicit |= EXPLICIT_OVERLAY;
        }
    }

    തിരികെ true;
}

സ്ഥിതം bool
CopyModMapDefToKeymap(ഘടന xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    ഘടന xkb_താക്കോൽ *താക്കോൽ;

    എങ്കിൽ (!entry->have_ചിഹ്നം) {
        താക്കോൽ = Xkb_താക്കോൽ_By_Name(keymap, entry->u.താക്കോൽ_Name, true);
        എങ്കിൽ (!താക്കോൽ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    താക്കോൽ_Name_Text(info->ctx, entry->u.താക്കോൽ_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            തിരികെ false;
        }
    }
    അല്ലെങ്കിൽ {
        താക്കോൽ = Find_താക്കോൽ_For_ചിഹ്നം(keymap, entry->u.താക്കോൽ_Sym);
        എങ്കിൽ (!താക്കോൽ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_ചിഹ്നം,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.താക്കോൽ_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            തിരികെ false;
        }
    }

    /* Skip modMap None */
    എങ്കിൽ (entry->mods) {
        താക്കോൽ->modmap |= entry->mods;
        entry->താക്കോൽ_Code = താക്കോൽ->keycode;
    }

    തിരികെ true;
}

സ്ഥിതം bool
CopySymbolsToKeymap(ഘടന xkb_keymap *keymap, SymbolsInfo *info)
{
    താക്കോൽ_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->അക്കം_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, ശൂന്യ);

    darray_foreach(keyi, info->താക്കോലുകൾ)
        എങ്കിൽ (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_എണ്ണം++;

    എങ്കിൽ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ഘടന xkb_താക്കോൽ *താക്കോൽ;

        xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
            എങ്കിൽ (താക്കോൽ->name == XKB_ATOM_NONE)
                തുടരുക;

            എങ്കിൽ (താക്കോൽ->അക്കം_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         താക്കോൽ_Name_Text(info->ctx, താക്കോൽ->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        എങ്കിൽ (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_എണ്ണം++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->അക്കം_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    തിരികെ true;
}

bool
CompileSymbols(XkbFile *file, ഘടന xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    എങ്കിൽ (file !=ശൂന്യ)
        HandleSymbolsFile(&info, file);

    എങ്കിൽ (info.error_എണ്ണം != 0)
        പോകുക err_info;

    എങ്കിൽ (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        പോകുക err_info;

    ClearSymbolsInfo(&info);
    തിരികെ true;

err_info:
    ClearSymbolsInfo(&info);
    തിരികെ false;
}
