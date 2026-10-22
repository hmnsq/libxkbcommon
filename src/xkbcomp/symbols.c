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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"

#ଅନ୍ତର୍ଭାଵ "action.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"
#ଅନ୍ତର୍ଭାଵ "vmod.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"

ଗଣନା ଚାବି_repeat {
    ଚାବି_REPEAT_UNDEFINED = 0,
    ଚାବି_REPEAT_YES,
    ଚାବି_REPEAT_NO,
    ଚାବି_REPEAT_ଅଙ୍କ_ENTRIES,
};
ଗଣନା { ଚାବି_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(ଚାବି_REPEAT_ଅଙ୍କ_ENTRIES - 1 <=
              (1 << (ଚାବି_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

ଗଣନା group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

ଗଣନା ଚାବି_field {
    ଚାବି_FIELD_NONE = 0,
    ଚାବି_FIELD_REPEAT = (1 << 0),
    ଚାବି_FIELD_DEFAULT_TYPE = (1 << 1),
    ଚାବି_FIELD_GROUPINFO = (1 << 2),
    ଚାବି_FIELD_VMODMAP = (1 << 3),
    ଚାବି_FIELD_OVERLAY = (1 << 4),
    ଚାବି_FIELD_ALL = ଚାବି_FIELD_REPEAT | ଚାବି_FIELD_DEFAULT_TYPE
                  | ଚାବି_FIELD_GROUPINFO | ଚାବି_FIELD_VMODMAP | ଚାବି_FIELD_OVERLAY,
};
ଗଣନା { ଚାବି_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(ଚାବି_FIELD_ALL <= (1 << (ଚାବି_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    darray(ସଂରଚନା xkb_level) levels;
    ଗଣନା group_field defined;
    xkb_atom_t type;
} GroupInfo;

ଗଣନା {
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
              (unsigned)ଚାବି_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)ଚାବି_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_ସଂଖ୍ୟା;
    darray(GroupInfo) groups;
    ଗଣନା xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    ଗଣନା ଚାବି_field defined:KEYINFO_DEFINED_BIT_FIELD;
    ଗଣନା merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    ଗଣନା ଚାବି_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

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
    ସଂଘ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ଅଚଳ ସଂରଚନା xkb_ଚାବି *overlay_ଚାବି;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ଅଚଳ ସଂରଚନା xkb_ଚାବି **overlays_ଚାବିଗୁଡ଼ିକ;
    };
} ଚାବି_Info;

ସ୍ଥିର ଶୂନ୍ୟ
StealLevelInfo(ସଂରଚନା xkb_level *into, ସଂରଚନା xkb_level *from)
{
    clear_level(into);
    ଯଦି (from->ଅଙ୍କ_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ଅନ୍ୟଥା {
        into->s.sym = from->s.sym;
    }
    into->ଅଙ୍କ_syms = from->ଅଙ୍କ_syms;
    from->ଅଙ୍କ_syms = 0;

    ଯଦି (from->ଅଙ୍କ_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ଅନ୍ୟଥା {
        into->a.action = from->a.action;
    }
    into->ଅଙ୍କ_actions = from->ଅଙ୍କ_actions;
    from->ଅଙ୍କ_actions = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, ଆକାର(*groupi));
}

ସ୍ଥିର ଶୂନ୍ୟ
ClearGroupInfo(GroupInfo *groupi)
{
    ସଂରଚନା xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

ସ୍ଥିର ଶୂନ୍ୟ
CopyGroupInfo(GroupInfo *to, ଅଚଳ GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    ପାଇଁ (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        ଯଦି (darray_item(from->levels, j).ଅଙ୍କ_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).ଅଙ୍କ_syms,
                       ଆକାର(xkb_keysym_t));
        }
        ଯଦି (darray_item(from->levels, j).ଅଙ୍କ_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).ଅଙ୍କ_actions,
                       ଆକାର(ସଂଘ xkb_action));
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
Init_ଚାବି_Info(ସଂରଚନା xkb_context *ctx, ଚାବି_Info *keyi)
{
    memset(keyi, 0, ଆକାର(*keyi));
    static_assert(!DEFAULT_ଚାବି_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ଚାବି_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ଚାବି = ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ
Clear_ଚାବି_Info(ଚାବି_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    ଯଦି (keyi->overlays_alloc)
        ମୁକ୍ତ(keyi->overlays_ଚାବିଗୁଡ଼ିକ);
}

/***====================================================================***/

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର *name;         /* e.g. pc+us+inet(evdev) */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(ଚାବି_Info) ଚାବିଗୁଡ଼ିକ;
    ଚାବି_Info default_ଚାବି;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    ସଂରଚନା xkb_mod_set mods;

    ସଂରଚନା xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info;
} SymbolsInfo;

ସ୍ଥିର ଶୂନ୍ୟ
InitSymbolsInfo(SymbolsInfo *info, ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth, ଅଚଳ ସଂରଚନା xkb_mod_set *mods)
{
    memset(info, 0, ଆକାର(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ଚାବି_Info(keymap_info->keymap.ctx, &info->default_ଚାବି);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

ସ୍ଥିର ଶୂନ୍ୟ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ଚାବି_Info *keyi;
    ମୁକ୍ତ(info->name);
    darray_foreach(keyi, info->ଚାବିଗୁଡ଼ିକ)
        Clear_ଚାବି_Info(keyi);
    darray_free(info->ଚାବିଗୁଡ଼ିକ);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_ଚାବି_Info(&info->default_ଚାବି);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
ଚାବି_Info_Text(SymbolsInfo *info, ଚାବି_Info *keyi)
{
    ଫେରନ୍ତୁ ଚାବି_Name_Text(info->ctx, keyi->name);
}

ସ୍ଥିର bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t ଚାବି_name)
{
    /* First find the type of the merged group. */
    ଯଦି (into->type != from->type) {
        ଯଦି (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ଅନ୍ୟଥା ଯଦି (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ଅନ୍ୟଥା {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ଯଦି (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, ଚାବି_Name_Text(info->ctx, ଚାବି_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    ଯଦି (darray_empty(from->levels)) {
        InitGroupInfo(from);
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        ଫେରନ୍ତୁ true;
    }

    /* Merge the actions and syms. */
    ଅଚଳ darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_ଗଣନା = 0;
    darray_size_t from_Actions_ଗଣନା = 0;
    ପାଇଁ (darray_size_t i = 0; i < levels_in_both; i++) {
        ସଂରଚନା xkb_level* ଅଚଳ intoLevel = &darray_item(into->levels, i);
        ସଂରଚନା xkb_level* ଅଚଳ fromLevel = &darray_item(from->levels, i);

        ଅଚଳ bool fromHasNoKeysym = fromLevel->ଅଙ୍କ_syms == 0;
        ଅଚଳ bool fromHasNoAction = fromLevel->ଅଙ୍କ_actions == 0;
        ଯଦି (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ଜାରି;
        }

        ଅଚଳ bool intoHasNoKeysym = intoLevel->ଅଙ୍କ_syms == 0;
        ଅଚଳ bool intoHasNoAction = intoLevel->ଅଙ୍କ_actions == 0;
        ଯଦି (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ଗଣନା++;
            from_Actions_ଗଣନା++;
            ଜାରି;
        }

        /* Possible level conflict */
        ପ୍ରତିଜ୍ଞା(intoLevel->ଅଙ୍କ_syms > 0 || intoLevel->ଅଙ୍କ_actions > 0);
        ପ୍ରତିଜ୍ଞା(fromLevel->ଅଙ୍କ_syms > 0 || fromLevel->ଅଙ୍କ_actions > 0);

        /* Handle keysyms */
        ଯଦି (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ଯଦି (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_ପ୍ରତୀକ,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, ଚାବି_Name_Text(info->ctx, ଚାବି_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ଯଦି (fromHasNoKeysym) {
                /* No keysym to copy */
            } ଅନ୍ୟଥା ଯଦି (clobber) {
                /* Override: copy any defined keysym from `from` */
                ଯଦି (unlikely(fromLevel->ଅଙ୍କ_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ଯଦି (unlikely(intoLevel->ଅଙ୍କ_syms > 1))
                        ମୁକ୍ତ(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->ଅଙ୍କ_syms = fromLevel->ଅଙ୍କ_syms;
                    fromLevel->ଅଙ୍କ_syms = 0;
                    from_Keysyms_ଗଣନା++;
                } ଅନ୍ୟଥା ଯଦି (fromLevel->s.sym != XKB_ଚାବି_No_ପ୍ରତୀକ) {
                    /* Single defined keysym */
                    ଯଦି (unlikely(intoLevel->ଅଙ୍କ_syms > 1))
                        ମୁକ୍ତ(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->ଅଙ୍କ_syms = 1;
                    from_Keysyms_ଗଣନା++;
                }
            } ଅନ୍ୟଥା {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ଯଦି (unlikely(intoLevel->ଅଙ୍କ_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ଅନ୍ୟଥା ଯଦି (intoLevel->s.sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
                    /* Single undefined keysym */
                    ଯଦି (unlikely(fromLevel->ଅଙ୍କ_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->ଅଙ୍କ_syms = fromLevel->ଅଙ୍କ_syms;
                        fromLevel->ଅଙ୍କ_syms = 0;
                    } ଅନ୍ୟଥା {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->ଅଙ୍କ_syms = 1;
                    }
                    from_Keysyms_ଗଣନା++;
                }
            }
        }

        /* Handle actions */
        ଯଦି (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ଯଦି (report && !(intoHasNoAction || fromHasNoAction)) {
                ଯଦି (unlikely(intoLevel->ଅଙ୍କ_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, ଚାବି_Name_Text(info->ctx, ଚାବି_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ଅନ୍ୟଥା {
                    ସଂଘ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                ଚାବି_Name_Text(info->ctx, ଚାବି_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            ଯଦି (fromHasNoAction) {
                /* No action to copy */
            } ଅନ୍ୟଥା ଯଦି (clobber) {
                /* Override: copy any defined action from `from` */
                ଯଦି (unlikely(fromLevel->ଅଙ୍କ_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ଯଦି (unlikely(intoLevel->ଅଙ୍କ_actions > 1))
                        ମୁକ୍ତ(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->ଅଙ୍କ_actions = fromLevel->ଅଙ୍କ_actions;
                    fromLevel->ଅଙ୍କ_actions = 0;
                    from_Actions_ଗଣନା++;
                } ଅନ୍ୟଥା ଯଦି (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ଯଦି (unlikely(intoLevel->ଅଙ୍କ_actions > 1))
                        ମୁକ୍ତ(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->ଅଙ୍କ_actions = 1;
                    from_Actions_ଗଣନା++;
                }
            } ଅନ୍ୟଥା {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ଯଦି (unlikely(intoLevel->ଅଙ୍କ_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ଅନ୍ୟଥା ଯଦି (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ଯଦି (unlikely(fromLevel->ଅଙ୍କ_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->ଅଙ୍କ_actions = fromLevel->ଅଙ୍କ_actions;
                        fromLevel->ଅଙ୍କ_actions = 0;
                    } ଅନ୍ୟଥା {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->ଅଙ୍କ_actions = 1;
                    }
                    from_Actions_ଗଣନା++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ସଂରଚନା xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->ଅଙ୍କ_syms = 0;
        level->ଅଙ୍କ_actions = 0;
        from_Keysyms_ଗଣନା++;
        from_Actions_ଗଣନା++;
    }
    ଯଦି (from_Keysyms_ଗଣନା) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ଯଦି (from_Keysyms_ଗଣନା == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    ଯଦି (from_Actions_ଗଣନା) {
        /* Reset defined actions field if we used no action from `into` */
        ଯଦି (from_Actions_ଗଣନା == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Use_New_ଚାବି_Field(ଗଣନା ଚାବି_field field, ଗଣନା ଚାବି_field old, ଗଣନା ଚାବି_field new,
               bool clobber, bool report, ଗଣନା ଚାବି_field *collide)
{
    ଯଦି (!(old & field))
        ଫେରନ୍ତୁ (new & field);

    ଯଦି (new & field) {
        ଯଦି (report)
            *collide |= field;

        ଫେରନ୍ତୁ clobber;
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
overlays_get(ଅଚଳ ଚାବି_Info *info, xkb_overlay_index_t bit,
             ଅଚଳ ସଂରଚନା xkb_ଚାବି **ଚାବି_out)
{
    ଯଦି (bit >= (xkb_overlay_index_t)(ଆକାର(xkb_overlay_mask_t) * CHAR_BIT))
        ଫେରନ୍ତୁ false;
    ଅଚଳ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    ଯଦି (!(info->overlays & mask)) ଫେରନ୍ତୁ false;
    ଯଦି (ଚାବି_out) {
        ଯଦି (!info->overlays_alloc) {
            /* Stored inline */
            *ଚାବି_out = info->overlay_ଚାବି;
        } ଅନ୍ୟଥା {
            /* Stored in sparse array */
            ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            ଅଚଳ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(ତଳ);
            *ଚାବି_out = info->overlays_ଚାବିଗୁଡ଼ିକ[index];
        }
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
overlays_ଭର୍ତ୍ତି(ଚାବି_Info *keyi, xkb_overlay_index_t bit,
                ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    ଯଦି (bit >= (xkb_overlay_index_t)(ଆକାର(xkb_overlay_mask_t) * CHAR_BIT))
        ଫେରନ୍ତୁ false;

    ଅଚଳ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    ଯଦି ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ଯଦି (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ଚାବି = ଚାବି;
            ଯଦି (!ଚାବି)
                keyi->overlays_clear = true;
        } ଅନ୍ୟଥା {
            /* Stored in sparse array */
            ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            ଅଚଳ xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(ତଳ);
            keyi->overlays_ଚାବିଗୁଡ଼ିକ[index] = ଚାବି;
        }
        ଫେରନ୍ତୁ true;
    }

    /* Insert new element */
    ଯଦି (!keyi->overlays || (keyi->overlays_clear && !ଚାବି)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ଚାବି = ଚାବି;
        keyi->overlays |= mask;
        keyi->overlays_clear = !ଚାବି;
    } ଅନ୍ୟଥା ଯଦି (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ଅଚଳ xkb_overlay_mask_t overlays = keyi->overlays | mask;
        ଅଚଳ xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        ଅଚଳ ସଂରଚନା xkb_ଚାବି ** ଅଚଳ tmp = calloc(alloc, ଆକାର(*tmp));
        ଯଦି (!tmp) ଫେରନ୍ତୁ false;

        /* Copy previous entries to the buffer */
        ଯଦି (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            ଅଚଳ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(ତଳ);
            tmp[idx] = keyi->overlay_ଚାବି;
        } ଅନ୍ୟଥା {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ଅଚଳ xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(ତଳ);
        tmp[idx] = ଚାବି;

        keyi->overlays_ଚାବିଗୁଡ଼ିକ = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } ଅନ୍ୟଥା {
        /* Allocated items: grow heap and shift if relevant */
        ଅଚଳ xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        ଅଚଳ xkb_overlay_index_t ଗଣନା_2 =
            (xkb_overlay_index_t)popcount32(overlays);
        ଯଦି (ଗଣନା_2 > keyi->overlays_alloc) {
            ଅଚଳ xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(ଗଣନା_2);
            ପ୍ରତିଜ୍ଞା(alloc >= ଗଣନା_2);
            ଅଚଳ ସଂରଚନା xkb_ଚାବି ** ଅଚଳ tmp = realloc(
                keyi->overlays_ଚାବିଗୁଡ଼ିକ,
                (size_t)alloc * ଆକାର(*keyi->overlays_ଚାବିଗୁଡ଼ିକ)
            );
            ଯଦି (!tmp) ଫେରନ୍ତୁ false;
            keyi->overlays_ଚାବିଗୁଡ଼ିକ = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        ଅଚଳ xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(ତଳ);
        ଯଦି (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ଯଦି (index < ଗଣନା_2 - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ପ୍ରତିଜ୍ଞା(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ଚାବିଗୁଡ଼ିକ + index + 1, keyi->overlays_ଚାବିଗୁଡ଼ିକ + index,
                    (size_t)(ଗଣନା_2 - 1u - index) * ଆକାର(ଅଚଳ ସଂରଚନା xkb_ଚାବି *));
        }
        keyi->overlays_ଚାବିଗୁଡ଼ିକ[index] = ଚାବି;
        keyi->overlays = overlays;
    }

    ଫେରନ୍ତୁ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ସ୍ଥିର bool
merge_overlays(SymbolsInfo *info, ଚାବି_Info *into, ଚାବି_Info *from,
               bool clobber, bool report, ଗଣନା ଚାବି_field *collide)
{
    ଯଦି (!(from->defined & ଚାବି_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ଅନ୍ୟଥା ଯଦି (!(into->defined & ଚାବି_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ଚାବିଗୁଡ଼ିକ = from->overlays_ଚାବିଗୁଡ଼ିକ;
        from->overlays_ଚାବିଗୁଡ଼ିକ = ଖାଲି;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ଚାବି_FIELD_OVERLAY;
    } ଅନ୍ୟଥା ଯଦି (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ଅନ୍ୟଥା {
        ଯଦି (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ଅଚଳ xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            ଅଚଳ xkb_overlay_index_t ଗଣନା_2 =
                (xkb_overlay_index_t)popcount32(result_mask);

            ଯଦି (ଗଣନା_2 == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ଚାବି_Info *dest = into;
            ଚାବି_Info *src = from;
            ଯଦି (into->overlays_alloc >= ଗଣନା_2 &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ଅନ୍ୟଥା ଯଦି (from->overlays_alloc >= ଗଣନା_2) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ଅନ୍ୟଥା ଯଦି (ଗଣନା_2 > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ଯଦି (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ଯଦି (!dest->overlays_alloc) {
                    ଅଚଳ ସଂରଚନା xkb_ଚାବି **tmp = calloc(ଗଣନା_2, ଆକାର(*tmp));
                    ଯଦି (!tmp)
                        ଫେରନ୍ତୁ false;
                    ଯଦି (dest->overlay_ଚାବି)
                        tmp[0] = dest->overlay_ଚାବି;
                    dest->overlays_ଚାବିଗୁଡ଼ିକ = tmp;
                    dest->overlays_alloc = ଗଣନା_2;
                } ଅନ୍ୟଥା {
                    ଅଚଳ ସଂରଚନା xkb_ଚାବି **tmp =
                        realloc(dest->overlays_ଚାବିଗୁଡ଼ିକ,
                                ଗଣନା_2 * ଆକାର(*dest->overlays_ଚାବିଗୁଡ଼ିକ));
                    ଯଦି (!tmp)
                        ଫେରନ୍ତୁ false;
                    dest->overlays_ଚାବିଗୁଡ଼ିକ = tmp;
                    dest->overlays_alloc = ଗଣନା_2;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            ଅଚଳ ସଂରଚନା xkb_ଚାବି **src_ଚାବିଗୁଡ଼ିକ = (src->overlays_clear)
                ? ଖାଲି
                : (src->overlays_alloc)
                    ? src->overlays_ଚାବିଗୁଡ଼ିକ
                    : &src->overlay_ଚାବି;
            ଯେତେବେଳେ (remaining) {
                /* isolate lowest set bit */
                ଅଚଳ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                ଅଚଳ xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                ଯଦି (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ଅଚଳ ସଂରଚନା xkb_ଚାବି *src_ଚାବି = (src_ଚାବିଗୁଡ଼ିକ)
                    ? *(src_ଚାବିଗୁଡ଼ିକ++)
                    : ଖାଲି;

                /* check dest */
                ଅଚଳ ସଂରଚନା xkb_ଚାବି *dest_ଚାବି = ଖାଲି;
                ଅଚଳ bool conflict = overlays_get(dest, bit, &dest_ଚାବି);
                ଯଦି (conflict) {
                    ଯଦି (dest_ଚାବି == src_ଚାବି)
                       ଜାରି;
                    ଅନ୍ୟଥା ଯଦି (report)
                        *collide |= ଚାବି_FIELD_OVERLAY;
                }

                ଯଦି ((!conflict || clobber) && !overlays_ଭର୍ତ୍ତି(dest, bit, src_ଚାବି))
                    ଫେରନ୍ତୁ false;
            }

            ଯଦି (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ଯଦି (into->overlays_alloc)
                    ମୁକ୍ତ(into->overlays_ଚାବିଗୁଡ଼ିକ);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ଯଦି (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ଚାବିଗୁଡ଼ିକ = from->overlays_ଚାବିଗୁଡ଼ିକ;
                    from->overlays_ଚାବିଗୁଡ଼ିକ = ଖାଲି;
                    from->overlays_alloc = 0;
                } ଅନ୍ୟଥା {
                    into->overlay_ଚାବି = from->overlay_ଚାବି;
                }
            }
        } ଅନ୍ୟଥା {
            /* Disjoint overlays */

            ଯଦି (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ଚାବି == from->overlay_ଚାବି) {
                /* Indentical: do nothing */
                ଫେରନ୍ତୁ true;
            }
            ଯଦି (!(into->overlays & from->overlays)) {
                ଯଦି (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ଚାବି = from->overlay_ଚାବି;
                    ଫେରନ୍ତୁ true;
                } ଅନ୍ୟଥା ଯଦି (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    ଫେରନ୍ତୁ true;
                }
            }
            /* Conflict */
            ଯଦି (report)
                *collide |= ଚାବି_FIELD_OVERLAY;
            ଯଦି (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ଚାବି = from->overlay_ଚାବି;
            }
        }
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Merge_ଚାବିଗୁଡ଼ିକ(SymbolsInfo *info, ଚାବି_Info *into, ଚାବି_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    ଗଣନା ଚାବି_field collide = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = xkb_context_get_log_verbosity(info->ctx);
    ଅଚଳ bool clobber = (from->merge != MERGE_AUGMENT);
    ଅଚଳ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ଯଦି (from->merge == MERGE_REPLACE) {
        Clear_ଚାବି_Info(into);
        *into = *from;
        Init_ଚାବି_Info(info->ctx, from);
        ଫେରନ୍ତୁ true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    ପାଇଁ (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    ପାଇଁ (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    ଯଦି (Use_New_ଚାବି_Field(ଚାବି_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ଚାବି_FIELD_VMODMAP;
    }
    ଯଦି (Use_New_ଚାବି_Field(ଚାବି_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ଚାବି_FIELD_REPEAT;
    }
    ଯଦି (Use_New_ଚାବି_Field(ଚାବି_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ଚାବି_FIELD_DEFAULT_TYPE;
    }
    ଯଦି (Use_New_ଚାବି_Field(ଚାବି_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_ସଂଖ୍ୟା = from->out_of_range_group_ସଂଖ୍ୟା;
        into->defined |= ଚାବି_FIELD_GROUPINFO;
    }

    ଯଦି (!merge_overlays(info, into, from, clobber, report, &collide))
        ଫେରନ୍ତୁ false;

    ଯଦି (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ଚାବି_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ଚାବି_Info(from);
    Init_ଚାବି_Info(info->ctx, from);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର xkb_atom_t
Xkb_Resolve_ଚାବି_Alias(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, xkb_atom_t name)
{
    ଯଦି (name < keymap->ଅଙ୍କ_ଚାବି_names) {
        ଅଚଳ KeycodeMatch match = keymap->ଚାବି_names[name];
        ଯଦି (match.found && match.is_alias) {
            ଫେରନ୍ତୁ match.alias.real;
        }
    }
    ଫେରନ୍ତୁ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ସ୍ଥିର bool
Add_ଚାବି_Symbols(SymbolsInfo *info, ଚାବି_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ଚାବି_Alias(&info->keymap_info->keymap, keyi->name);

    ଚାବି_Info *iter;
    darray_foreach(iter, info->ଚାବିଗୁଡ଼ିକ)
        ଯଦି (iter->name == keyi->name)
            ଫେରନ୍ତୁ Merge_ଚାବିଗୁଡ଼ିକ(info, iter, keyi, same_file);

    darray_append(info->ଚାବିଗୁଡ଼ିକ, *keyi);
    Init_ଚାବି_Info(info->ctx, keyi);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
AddModMapEntry(SymbolsInfo *info, ଅଚଳ ModMapEntry *new)
{
    ଅଚଳ bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        ଯଦି ((new->have_ପ୍ରତୀକ != old->have_ପ୍ରତୀକ) ||
            (new->have_ପ୍ରତୀକ && new->u.ଚାବି_Sym != old->u.ଚାବି_Sym) ||
            (!new->have_ପ୍ରତୀକ && new->u.ଚାବି_Name != old->u.ଚାବି_Name))
            ଜାରି;

        ଯଦି (new->mods == old->mods)
            ଫେରନ୍ତୁ true;

        ଅଚଳ xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        ଅଚଳ xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        ଯଦି (new->have_ପ୍ରତୀକ) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.ଚାବି_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } ଅନ୍ୟଥା {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ଚାବି_Name_Text(info->ctx, new->u.ଚାବି_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        ଫେରନ୍ତୁ true;
    }

    darray_append(info->modmaps, *new);

    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

ସ୍ଥିର ଶୂନ୍ୟ
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     ଗଣନା merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    ଯଦି (from->error_ଗଣନା > 0) {
        into->error_ଗଣନା += from->error_ଗଣନା;
        ଫେରନ୍ତୁ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ଯଦି (into->name == ଖାଲି) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    ପାଇଁ (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        ଯଦି (!darray_item(from->group_names, i))
            ଜାରି;

        ଯଦି (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            ଜାରି;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    ଯଦି (darray_empty(into->ଚାବିଗୁଡ଼ିକ)) {
        into->ଚାବିଗୁଡ଼ିକ = from->ଚାବିଗୁଡ଼ିକ;
        darray_init(from->ଚାବିଗୁଡ଼ିକ);
    }
    ଅନ୍ୟଥା {
        ଚାବି_Info *keyi;
        darray_foreach(keyi, from->ଚାବିଗୁଡ଼ିକ) {
            keyi->merge = merge;
            ଯଦି (!Add_ଚାବି_Symbols(into, keyi, false))
                into->error_ଗଣନା++;
        }
    }

    ଯଦି (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ଅନ୍ୟଥା {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            ଯଦି (!AddModMapEntry(into, mm))
                into->error_ଗଣନା++;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ସ୍ଥିର bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ଯଦି (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ଗଣନା += 10;
        ଫେରନ୍ତୁ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ପାଇଁ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        ଅକ୍ଷର path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, ଆକାର(path));
        ଯଦି (!file) {
            info->error_ଗଣନା += 10;
            ClearSymbolsInfo(&included);
            ଫେରନ୍ତୁ false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        ଯଦି (stmt->modifier) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            ଯଦି (ଗଣନା_2 <= 0 || *(stmt->modifier + ଗଣନା_2) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } ଅନ୍ୟଥା {
                next_incl.explicit_group--;
            }
        }
        ଅନ୍ୟଥା ଯଦି (info->keymap_info->keymap.ଅଙ୍କ_groups != 0 &&
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
        ଅନ୍ୟଥା {
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

    ଫେରନ୍ତୁ (info->error_ଗଣନା == 0);
}

ସ୍ଥିର bool
GetGroupIndex(SymbolsInfo *info, ଚାବି_Info *keyi, ପଦସମଷ୍ଟି_Def *arrayNdx,
              ଗଣନା group_field field, xkb_layout_index_t *ndx_rtrn)
{
    ପ୍ରତିଜ୍ଞା (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    ଅଚଳ ଅକ୍ଷର *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    ଯଦି (arrayNdx == ଖାଲି) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ଯଦି (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                ଫେରନ୍ତୁ true;
            }
        }

        ଯଦି (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ଚାବି_Info_Text(info, keyi), info->max_groups, name);
            ଫେରନ୍ତୁ false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (ପଦସମଷ୍ଟି_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, ଖାଲି) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ଚାବି_Info_Text(info, keyi));
        ଫେରନ୍ତୁ false;
    }

    (*ndx_rtrn)--;
    ଯଦି (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Add_Symbols_To_ଚାବି(SymbolsInfo *info, ଚାବି_Info *keyi, ପଦସମଷ୍ଟି_Def *arrayNdx,
                ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    xkb_layout_index_t ndx = 0;

    ଯଦି (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        ଫେରନ୍ତୁ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(ମୂଲ୍ୟ->common.type), ndx + 1,
                ଚାବି_Info_Text(info, keyi));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ଚାବି_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ଚାବି_Info_Text(info, keyi), ndx + 1);
        ଫେରନ୍ତୁ false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    ପାଇଁ (ପଦସମଷ୍ଟି_Keysym_List *keysymList = (ପଦସମଷ୍ଟି_Keysym_List *) ମୂଲ୍ୟ;
         keysymList;
         keysymList = (ପଦସମଷ୍ଟି_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        ଯଦି (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    ଯଦି (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    ଯଦି (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    ପାଇଁ (ପଦସମଷ୍ଟି_Keysym_List *keysymList = (ପଦସମଷ୍ଟି_Keysym_List *) ମୂଲ୍ୟ;
         keysymList && level < nLevels;
         keysymList = (ପଦସମଷ୍ଟି_Keysym_List *) keysymList->common.next, level++) {
        ସଂରଚନା xkb_level *leveli = &darray_item(groupi->levels, level);
        ପ୍ରତିଜ୍ଞା(leveli->ଅଙ୍କ_syms == 0);

        ଯଦି (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ଚାବି_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            ଫେରନ୍ତୁ false;
        }

        leveli->ଅଙ୍କ_syms = (xkb_keysym_ଗଣନା_t) darray_size(keysymList->syms);
        ବାଛ (leveli->ଅଙ୍କ_syms) {
        ସ୍ଥିତି 0:
            leveli->s.sym = XKB_ଚାବି_No_ପ୍ରତୀକ;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            ପ୍ରତିଜ୍ଞା(leveli->s.sym != XKB_ଚାବି_No_ପ୍ରତୀକ);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ଖାଲି);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            ପାଇଁ (xkb_keysym_ଗଣନା_t k = 0; k < leveli->ଅଙ୍କ_syms; k++)
                ପ୍ରତିଜ୍ଞା(leveli->s.syms[k] != XKB_ଚାବି_No_ପ୍ରତୀକ);
#endif
        }
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Add_Actions_To_ଚାବି(SymbolsInfo *info, ଚାବି_Info *keyi, ପଦସମଷ୍ଟି_Def *arrayNdx,
                ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    xkb_layout_index_t ndx = 0;

    ଯଦି (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        ଫେରନ୍ତୁ false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 ମୂଲ୍ୟ->common.type, ndx, ଚାବି_Info_Text(info, keyi));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ଚାବି_Info_Text(info, keyi), ndx);
        ଫେରନ୍ତୁ false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    ପାଇଁ (ParseCommon *p = &ମୂଲ୍ୟ->common; p; p = p->next)
        nLevels++;
    ଯଦି (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    ପାଇଁ (ପଦସମଷ୍ଟି_Action_List *actionList = (ପଦସମଷ୍ଟି_Action_List *) ମୂଲ୍ୟ;
         actionList;
         actionList = (ପଦସମଷ୍ଟି_Action_List *) actionList->common.next, level++) {
        ସଂରଚନା xkb_level *leveli = &darray_item(groupi->levels, level);
        ପ୍ରତିଜ୍ଞା(leveli->ଅଙ୍କ_actions == 0);

        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_actions = 0;
        ପାଇଁ (ପଦସମଷ୍ଟି_Def *act = actionList->actions;
             act; act = (ପଦସମଷ୍ଟି_Def *) act->common.next)
            ଅଙ୍କ_actions++;

        ଯଦି (unlikely(ଅଙ୍କ_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ଚାବି_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, ଅଙ୍କ_actions);
            ଫେରନ୍ତୁ false;
        }

        /* Parse actions and add only defined actions */
        darray(ସଂଘ xkb_action) actions = darray_new();
        ପାଇଁ (ପଦସମଷ୍ଟି_Def *act = actionList->actions;
             act; act = (ପଦସମଷ୍ଟି_Def *) act->common.next) {
            ସଂଘ xkb_action toAct = { 0 };
            ଅଚଳ ଗଣନା xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            ଯଦି (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ଚାବି_Info_Text(info, keyi), ndx + 1, level + 1);
                ଯଦି (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    ଫେରନ୍ତୁ false;
                } ଅନ୍ୟଥା {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            ଯଦି (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ଅନ୍ୟଥା ଯଦି (likely(ଅଙ୍କ_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->ଅଙ୍କ_actions = 1;
                leveli->a.action = toAct;
                ଯାଅ next_level;
            } ଅନ୍ୟଥା {
                darray_append(actions, toAct);
            }
        }
        ଯଦି (darray_empty(actions)) {
            leveli->ଅଙ୍କ_actions = 0;
        } ଅନ୍ୟଥା ଯଦି (likely(darray_size(actions) > 1)) {
            leveli->ଅଙ୍କ_actions = (xkb_action_ଗଣନା_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ଖାଲି);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < leveli->ଅଙ୍କ_actions; k++)
                ପ୍ରତିଜ୍ଞା(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } ଅନ୍ୟଥା {
            /* Unlikely: some actions were dropped and only one remains */
            ପ୍ରତିଜ୍ଞା(ଅଙ୍କ_actions > darray_size(actions));
            leveli->ଅଙ୍କ_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            ପ୍ରତିଜ୍ଞା(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ଯଦି (leveli->ଅଙ୍କ_actions > 0 || leveli->ଅଙ୍କ_syms > 0)
            nonEmptyLevels = level + 1;
    }

    ଯଦି (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        ଯଦି (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        ଅନ୍ୟଥା
            darray_free(groupi->levels);
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଅଚଳ LookupEntry repeatEntries[] = {
    { "true", ଚାବି_REPEAT_YES },
    { "yes", ଚାବି_REPEAT_YES },
    { "on", ଚାବି_REPEAT_YES },
    { "false", ଚାବି_REPEAT_NO },
    { "no", ଚାବି_REPEAT_NO },
    { "off", ଚାବି_REPEAT_NO },
    { "default", ଚାବି_REPEAT_UNDEFINED },
    { ଖାଲି, 0 }
};

/** Handle overlay<N> = <key> entry */
ସ୍ଥିର bool
ପଦସମଷ୍ଟି_Resolve_Overlay_Entry(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                        ଅଚଳ ଅକ୍ଷର *field, ଅଚଳ ପଦସମଷ୍ଟି_Def *arrayNdx,
                        ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, ଚାବି_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        ଅଚଳ ସଂରଚନା xkb_ଚାବି **ଚାବି_rtrn)
{
    /* Overlay index */
    ଯଦି (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, ଚାବି_Name_Text(keymap_info->keymap.ctx, keyi->name));
        ଫେରନ୍ତୁ false;
    }
    static_assert(ଆକାର(xkb_overlay_index_t) == ଆକାର(uint8_t), "");
    ଅଚଳ size_t prefix = ଆକାର("overlay") - 1;
    ଅଚଳ size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ଯଦି (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, ଚାବି_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        ଫେରନ୍ତୁ false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL:
        *ଚାବି_rtrn = Xkb_ଚାବି_By_Name(&keymap_info->keymap, ପଦସମଷ୍ଟି->ଚାବି_name.ଚାବି_name,
                                 false);
        ଯଦି (!*ଚାବି_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, ପଦସମଷ୍ଟି->ଚାବି_name.ଚାବି_name),
                    field, ଚାବି_Name_Text(keymap_info->keymap.ctx, keyi->name));
            ଫେରନ୍ତୁ false;
        }
        ଫେରନ୍ତୁ true;
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT: {
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ id =
            xkb_atom_text(keymap_info->keymap.ctx, ପଦସମଷ୍ଟି->ident.ident);
        ଯଦି (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ଚାବି_rtrn = ଖାଲି;
            ଫେରନ୍ତୁ true;
        } ଅନ୍ୟଥା ଯଦି (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ଚାବି_rtrn = ଖାଲି;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            ଫେରନ୍ତୁ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, ଚାବି_Name_Text(keymap_info->keymap.ctx, keyi->name));
        ଫେରନ୍ତୁ false;
    }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL),
                field, ଚାବି_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;
    }
}

ସ୍ଥିର bool
SetSymbolsField(SymbolsInfo *info, ଚାବି_Info *keyi, ଅଚଳ ଅକ୍ଷର *field,
                ପଦସମଷ୍ଟି_Def *arrayNdx, ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ପଦସମଷ୍ଟି_Def * restrict ଅଚଳ ମୂଲ୍ୟ = *ମୂଲ୍ୟ_ptr;
    ଯଦି (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_String(info->ctx, ମୂଲ୍ୟ, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            ଫେରନ୍ତୁ false;
        }

        ଯଦି (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ଚାବି_FIELD_DEFAULT_TYPE;
        }
        ଅନ୍ୟଥା ଯଦି (ପଦସମଷ୍ଟି_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, ଖାଲି) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }
        ଅନ୍ୟଥା {
            ndx--;
            ଯଦି (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ପ୍ରତିଜ୍ଞା(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "symbols")) {
        ଫେରନ୍ତୁ Add_Symbols_To_ଚାବି(info, keyi, arrayNdx, ମୂଲ୍ୟ);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "actions")) {
        ଫେରନ୍ତୁ Add_Actions_To_ଚାବି(info, keyi, arrayNdx, ମୂଲ୍ୟ);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, ମୂଲ୍ୟ, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(ମୂଲ୍ୟ->common.type),
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= ଚାବି_FIELD_VMODMAP;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ଚାବି_Info_Text(info, keyi));
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ଚାବି_Info_Text(info, keyi));
    }
    ଅନ୍ୟଥା ଯଦି (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ଚାବି_Info_Text(info, keyi));
    }
    ଅନ୍ୟଥା ଯଦି (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = ଖାଲି;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *ମୂଲ୍ୟ_ptr, keyi, &overlay, &ଚାବି))
            ଫେରନ୍ତୁ false;
        ଯଦି (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            ଫେରନ୍ତୁ true;
        } ଅନ୍ୟଥା ଯଦି (ଚାବି && ଚାବି->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ଚାବି_Info_Text(info, keyi));
        } ଅନ୍ୟଥା {
            ଅଚଳ ସଂରଚନା xkb_ଚାବି *prev;
            ଯଦି (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ଯଦି (ଚାବି != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ଚାବି_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? ଚାବି_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ଚାବି
                                 ? ଚାବି_Name_Text(info->ctx, ଚାବି->name)
                                 : "none"));
                }
            } ଅନ୍ୟଥା ଯଦି (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ଯଦି (!overlays_ଭର୍ତ୍ତି(keyi, overlay, ଚାବି))
                    ଫେରନ୍ତୁ false;
                keyi->defined |= ଚାବି_FIELD_OVERLAY;
            } ଅନ୍ୟଥା {
                /* Handle disjoint overlays mode */
                ଅଚଳ xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                ଯଦି (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ଯଦି (ଚାବି) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_ଚାବି = ଚାବି;
                    } ଅନ୍ୟଥା {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ଚାବି_FIELD_OVERLAY;
                } ଅନ୍ୟଥା ଯଦି (keyi->overlays) {
                    ଯଦି (ଚାବି) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ଚାବି_Info_Text(info, keyi), keyi->overlays,
                                ଚାବି_Name_Text(info->ctx, keyi->overlay_ଚାବି->name),
                                overlay + 1, ଚାବି_Name_Text(info->ctx, ଚାବି->name));
                        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_ମୂଲ୍ୟ_MISMATCH);
                    } ଅନ୍ୟଥା {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Enum(info->ctx, ମୂଲ୍ୟ, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }

        keyi->repeat = (ଗଣନା ଚାବି_repeat) val;
        keyi->defined |= ଚାବି_FIELD_REPEAT;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Boolean(info->ctx, ମୂଲ୍ୟ, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= ଚାବି_FIELD_GROUPINFO;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Boolean(info->ctx, ମୂଲ୍ୟ, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ମୂଲ୍ୟ,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= ଚାବି_FIELD_GROUPINFO;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ଯଦି (ପଦସମଷ୍ଟି_Resolve_Group(info->keymap_info, ମୂଲ୍ୟ, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ଚାବି_Info_Text(info, keyi));
            ଫେରନ୍ତୁ false;
        }

        ଯଦି (pending) {
            keyi->out_of_range_pending_group = true;
            ଅଚଳ darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ସଂରଚନା pending_computation) {
                    .ପଦସମଷ୍ଟି = *ମୂଲ୍ୟ_ptr,
                    .computed = false,
                    .ମୂଲ୍ୟ = 0,
                }
            );
            *ମୂଲ୍ୟ_ptr = ଖାଲି;
            static_assert(ଆକାର(keyi->out_of_range_group_ସଂଖ୍ୟା) ==
                          ଆକାର(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_ସଂଖ୍ୟା = pending_index;
        } ଅନ୍ୟଥା {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_ସଂଖ୍ୟା = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= ଚାବି_FIELD_GROUPINFO;
    }
    ଅନ୍ୟଥା {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ଚାବି_FIELDS);
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
SetGroupName(SymbolsInfo *info, ପଦସମଷ୍ଟି_Def *arrayNdx, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
             ଗଣନା merge_mode merge)
{
    ଯଦି (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        ଫେରନ୍ତୁ false;
    }

    xkb_layout_index_t group = 0;
    ଯଦି (ପଦସମଷ୍ଟି_Resolve_Group(info->keymap_info, arrayNdx, false, &group, ଖାଲି) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        ଫେରନ୍ତୁ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_String(info->ctx, ମୂଲ୍ୟ, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        ଫେରନ୍ତୁ false;
    }

    xkb_layout_index_t group_to_use;
    ଯଦି (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    ଅନ୍ୟଥା ଯଦି (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    ଅନ୍ୟଥା {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        ପ୍ରତିଜ୍ଞା(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } ଅନ୍ୟଥା {
        ଅଚଳ xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        ଯଦି (old_name != XKB_ATOM_NONE && old_name != name) {
            ଅଚଳ bool replace = (merge != MERGE_AUGMENT);
            ଅଚଳ xkb_atom_t use    = (replace ? name : old_name);
            ଅଚଳ xkb_atom_t ignore = (replace ? old_name : name);
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

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *arrayNdx;
    bool ret;

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ଫେରନ୍ତୁ false;

    ଯଦି (elem && istreq(elem, "key")) {
        ଚାବି_Info temp = {0};
        Init_ଚାବି_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->ମୂଲ୍ୟ) &&
              Merge_ଚାବିଗୁଡ଼ିକ(info, &info->default_ଚାବି, &temp, true);
        Clear_ଚାବି_Info(&temp);
    }
    ଅନ୍ୟଥା ଯଦି (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->ମୂଲ୍ୟ, stmt->merge);
    }
    ଅନ୍ୟଥା ଯଦି (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ଅନ୍ୟଥା ଯଦି (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ଅନ୍ୟଥା ଯଦି (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ଅନ୍ୟଥା ଯଦି (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ଅନ୍ୟଥା ଯଦି (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->ମୂଲ୍ୟ, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } ଅନ୍ୟଥା {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ଚାବି_Info *keyi)
{
    bool all_valid_entries = true;

    ପାଇଁ (; def; def = (VarDef *) def->common.next) {
        ଅଚଳ ଅକ୍ଷର *field;
        ପଦସମଷ୍ଟି_Def *arrayNdx;
        bool ok = true;

        ଯଦି (!def->name) {
            ଯଦି (unlikely(!def->ମୂଲ୍ୟ) || def->ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            ଅନ୍ୟଥା
                field = "actions";
            arrayNdx = ଖାଲି;
        }
        ଅନ୍ୟଥା {
            ଅଚଳ ଅକ୍ଷର *elem;
            ok = ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            ଯଦି (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        ଯଦି (unlikely(!def->ମୂଲ୍ୟ)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        ଯଦି (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->ମୂଲ୍ୟ))
            all_valid_entries = false;
    }

    ଫେରନ୍ତୁ all_valid_entries;
}

ସ୍ଥିର bool
SetExplicitGroup(SymbolsInfo *info, ଚାବି_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    ଯଦି (info->explicit_group == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ଯଦି (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    ଯଦି (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, ଚାବି_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    ଯଦି (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ଚାବି_Info keyi;

    keyi = info->default_ଚାବି;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ଚାବି.groups);
    ପାଇଁ (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_ଚାବି.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->ଚାବି_Name;

    ଯଦି (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_ଚାବି_Symbols(info, &keyi, true)) {
        ଫେରନ୍ତୁ true;
    }

    Clear_ଚାବି_Info(&keyi);
    info->error_ଗଣନା++;
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    ସଂରଚନା xkb_context *ctx = info->ctx;

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        ଫେରନ୍ତୁ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ପ୍ରତିଜ୍ଞା(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_ପଦସମଷ୍ଟି_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    ପାଇଁ (ପଦସମଷ୍ଟି_Def *ଚାବି = def->ଚାବିଗୁଡ଼ିକ; ଚାବି; ଚାବି = (ପଦସମଷ୍ଟି_Def *) ଚାବି->common.next) {
        ଯଦି (ଚାବି->common.type == STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL) {
            tmp.have_ପ୍ରତୀକ = false;
            tmp.u.ଚାବି_Name = ଚାବି->ଚାବି_name.ଚାବି_name;
        }
        ଅନ୍ୟଥା ଯଦି (ଚାବି->common.type == STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL) {
            ଯଦି (ଚାବି->keysym.keysym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
                /* Invalid keysym: ignore. Error message already printed */
                ଜାରି;
            } ଅନ୍ୟଥା {
                tmp.have_ପ୍ରତୀକ = true;
                tmp.u.ଚାବି_Sym = ଚାବି->keysym.keysym;
            }
        }
        ଅନ୍ୟଥା {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ଜାରି;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    ମୁକ୍ତ(info->name);
    info->name = strdup_safe(file->name);

    ପାଇଁ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ବାଛ (stmt->type) {
        ସ୍ଥିତି STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_UNKNOWN_DECLARATION:
        ସ୍ଥିତି STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ଭାଙ୍ଗ;
        }

        ଯଦି (!ok)
            info->error_ଗଣନା++;

        ଯଦି (info->error_ଗଣନା > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            ଭାଙ୍ଗ;
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
ସ୍ଥିର ସଂରଚନା xkb_ଚାବି *
Find_ଚାବି_For_ପ୍ରତୀକ(ସଂରଚନା xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    କର {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        କର {
            got_one_level = false;
            ସଂରଚନା xkb_ଚାବି *ଚାବି;
            xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
                ଯଦି (group < ଚାବି->ଅଙ୍କ_groups &&
                    level < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, group)) {
                    got_one_group = got_one_level = true;
                    ଅଚଳ xkb_keysym_ଗଣନା_t ଅଙ୍କ_syms =
                        ଚାବି->groups[group].levels[level].ଅଙ୍କ_syms;
                    ଯଦି (ଅଙ୍କ_syms > 1) {
                        ପାଇଁ (xkb_keysym_ଗଣନା_t k = 0; k < ଅଙ୍କ_syms; k++) {
                            ଯଦି (ଚାବି->groups[group].levels[level].s.syms[k] == sym)
                                ଫେରନ୍ତୁ ଚାବି;
                        }
                    } ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_syms &&
                               ଚାବି->groups[group].levels[level].s.sym == sym) {
                        ଫେରନ୍ତୁ ଚାବି;
                    }
                }
            }
            level++;
        } ଯେତେବେଳେ (got_one_level);
        group++;
    } ଯେତେବେଳେ (got_one_group);

    ଫେରନ୍ତୁ ଖାଲି;
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
ସ୍ଥିର xkb_atom_t
FindAutomaticType(ସଂରଚନା xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    ଅଚଳ xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).ଅଙ୍କ_syms == 0 ? \
        XKB_ଚାବି_No_ପ୍ରତୀକ : \
     darray_item(groupi->levels, level).ଅଙ୍କ_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    ଯଦି (width == 1 || width <= 0)
        ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ଯଦି (width == 2) {
        ଯଦି (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ଯଦି (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "KEYPAD");

        ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ଯଦି (width <= 4) {
        ଯଦି (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_ଚାବି_No_ପ୍ରତୀକ);

            ଯଦି (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ଯଦି (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        ଫେରନ୍ତୁ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    ଫେରନ୍ତୁ XKB_ATOM_NONE;

#undef GET_SYM
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି_type *
FindTypeForGroup(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ଯଦି (type_name == XKB_ATOM_NONE) {
        ଯଦି (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ଅନ୍ୟଥା {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            ଯଦି (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ଯଦି (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ଚାବି_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ଚାବି_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        ଯାଅ use_default;
    }

    darray_size_t i;
    ପାଇଁ (i = 0; i < keymap->ଅଙ୍କ_types; i++)
        ଯଦି (keymap->types[i].name == type_name)
            ଭାଙ୍ଗ;

    ଯଦି (i >= keymap->ଅଙ୍କ_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ଚାବି_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 ଚାବି_Name_Text(keymap->ctx, keyi->name), group + 1);
        ଯାଅ use_default;
    }

    keymap->types[i].required = true;
    ଫେରନ୍ତୁ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    ଫେରନ୍ତୁ &keymap->types[0];
}

ସ୍ଥିର bool
CopySymbolsDefToKeymap(ସଂରଚନା xkb_keymap *keymap, SymbolsInfo *info,
                       ଚାବି_Info *keyi)
{
    ସଂରଚନା xkb_ଚାବି *ଚାବି;
    GroupInfo *groupi;
    ଅଚଳ GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ଚାବି = Xkb_ଚାବି_By_Name(keymap, keyi->name, false);
    ଯଦି (!ଚାବି) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ଚାବି_Info_Text(info, keyi));
        ଫେରନ୍ତୁ false;
    }

    /* Find the range of groups we need. */
    ଚାବି->ଅଙ୍କ_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ଅଚଳ bool has_explicit_type = (keyi->defined & ଚାବି_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        ଯଦି (darray_size(groupi->levels) > 0 || has_explicit_type)
            ଚାବି->ଅଙ୍କ_groups = i + 1;
        ଯଦି (has_explicit_type)
            ଚାବି->explicit |= EXPLICIT_TYPES;
    }

    ଯଦି (ଚାବି->ଅଙ୍କ_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ଯଦି (keyi->defined)
            ଯାଅ ଚାବି_fields;
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ false;
    }

    darray_resize(keyi->groups, ଚାବି->ଅଙ୍କ_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ଯଦି (groupi->defined)
            ଜାରି;

        CopyGroupInfo(groupi, group0);
    }

    ଚାବି->groups = calloc(ଚାବି->ଅଙ୍କ_groups, ଆକାର(*ଚାବି->groups));
    ପ୍ରତିଜ୍ଞା(darray_size(keyi->groups) == ଚାବି->ଅଙ୍କ_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ଚାବି->ଅଙ୍କ_groups);
        #endif
        bool explicit_type = false;
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_type * ଅଚଳ type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ଯଦି (type->ଅଙ୍କ_levels < darray_size(groupi->levels)) {
            ସଂରଚନା xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->ଅଙ୍କ_levels,
                    ଚାବି_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->ଅଙ୍କ_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->ଅଙ୍କ_levels);

        ଚାବି->groups[i].explicit_type = explicit_type;
        ଚାବି->groups[i].type = type;
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
        ସଂରଚନା xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            ବାଛ (leveli->ଅଙ୍କ_syms) {
            ସ୍ଥିତି 0:
                leveli->upper = XKB_ଚାବି_No_ପ୍ରତୀକ;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                ପାଇଁ (xkb_keysym_ଗଣନା_t k = 0; k < leveli->ଅଙ୍କ_syms; k++) {
                    ଅଚଳ xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    ଯଦି (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        ଭାଙ୍ଗ;
                    }
                }
                ଯଦି (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->ଅଙ୍କ_syms *
                                             ଆକାର(*leveli->s.syms));
                    ଯଦି (!leveli->s.syms)
                        ଫେରନ୍ତୁ false; /* FIXME: better handling? */
                    ପାଇଁ (xkb_keysym_ଗଣନା_t k = 0; k < leveli->ଅଙ୍କ_syms; k++) {
                        leveli->s.syms[leveli->ଅଙ୍କ_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &ଚାବି->groups[i].levels, ଖାଲି);
        ଯଦି (ଚାବି->groups[i].type->ଅଙ୍କ_levels > 1 ||
            ଚାବି->groups[i].levels[0].ଅଙ୍କ_syms > 0) {
            ଚାବି->groups[i].explicit_symbols = true;
            ଚାବି->explicit |= EXPLICIT_SYMBOLS;
        }
        ଯଦି (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            ଚାବି->groups[i].explicit_actions = true;
            ଚାବି->explicit |= EXPLICIT_INTERP;
        }
        ଯଦି (ଚାବି->groups[i].explicit_type)
            ଚାବି->explicit |= EXPLICIT_TYPES;
    }

    ଚାବି->out_of_range_pending_group = keyi->out_of_range_pending_group;
    ଚାବି->out_of_range_group_ସଂଖ୍ୟା = keyi->out_of_range_group_ସଂଖ୍ୟା;
    ଚାବି->out_of_range_group_policy = keyi->out_of_range_group_policy;

ଚାବି_fields:
    ଯଦି (keyi->defined & ଚାବି_FIELD_VMODMAP) {
        ଚାବି->vmodmap = keyi->vmodmap;
        ଚାବି->explicit |= EXPLICIT_VMODMAP;
    }

    ଯଦି (keyi->repeat != ଚାବି_REPEAT_UNDEFINED) {
        ଚାବି->repeats = (keyi->repeat == ଚାବି_REPEAT_YES);
        ଚାବି->explicit |= EXPLICIT_REPEAT;
    }

    ଯଦି ((keyi->defined & ଚାବି_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ଚାବି->overlays = keyi->overlays;
        ଯଦି (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = ଚାବି->overlays;
            ଅଚଳ ସଂରଚନା xkb_ଚାବି **overlays_ଚାବିଗୁଡ଼ିକ = keyi->overlays_ଚାବିଗୁଡ଼ିକ;
            ଯେତେବେଳେ (remaining) {
                /* isolate lowest set bit */
                ଅଚଳ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                ଯଦି (*overlays_ଚାବିଗୁଡ଼ିକ) {
                    overlays_ଚାବିଗୁଡ଼ିକ++;
                } ଅନ୍ୟଥା {
                    /* drop current null value (overlayN=none) */
                    ଚାବି->overlays &= ~lsb;
                }
            }

            ଯଦି (ଚାବି->overlays) {
                /* Steal */
                ଚାବି->overlays_ଚାବିଗୁଡ଼ିକ = keyi->overlays_ଚାବିଗୁଡ଼ିକ;
                keyi->overlays_ଚାବିଗୁଡ଼ିକ = ଖାଲି;
                keyi->overlays_alloc = 0;

                ଚାବି->overlays_inline = false;
                ଚାବି->explicit |= EXPLICIT_OVERLAY;
            }
        } ଅନ୍ୟଥା {
            ଚାବି->overlay_ଚାବି = keyi->overlay_ଚାବି;
            ଚାବି->overlays_inline = true;
            ଚାବି->explicit |= EXPLICIT_OVERLAY;
        }
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
CopyModMapDefToKeymap(ସଂରଚନା xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    ସଂରଚନା xkb_ଚାବି *ଚାବି;

    ଯଦି (!entry->have_ପ୍ରତୀକ) {
        ଚାବି = Xkb_ଚାବି_By_Name(keymap, entry->u.ଚାବି_Name, true);
        ଯଦି (!ଚାବି) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ଚାବି_Name_Text(info->ctx, entry->u.ଚାବି_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ଫେରନ୍ତୁ false;
        }
    }
    ଅନ୍ୟଥା {
        ଚାବି = Find_ଚାବି_For_ପ୍ରତୀକ(keymap, entry->u.ଚାବି_Sym);
        ଯଦି (!ଚାବି) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_ପ୍ରତୀକ,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.ଚାବି_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            ଫେରନ୍ତୁ false;
        }
    }

    /* Skip modMap None */
    ଯଦି (entry->mods) {
        ଚାବି->modmap |= entry->mods;
        entry->ଚାବି_Code = ଚାବି->keycode;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
CopySymbolsToKeymap(ସଂରଚନା xkb_keymap *keymap, SymbolsInfo *info)
{
    ଚାବି_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->ଅଙ୍କ_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, ଖାଲି);

    darray_foreach(keyi, info->ଚାବିଗୁଡ଼ିକ)
        ଯଦି (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_ଗଣନା++;

    ଯଦି (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ସଂରଚନା xkb_ଚାବି *ଚାବି;

        xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
            ଯଦି (ଚାବି->name == XKB_ATOM_NONE)
                ଜାରି;

            ଯଦି (ଚାବି->ଅଙ୍କ_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         ଚାବି_Name_Text(info->ctx, ଚାବି->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        ଯଦି (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_ଗଣନା++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->ଅଙ୍କ_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    ଫେରନ୍ତୁ true;
}

bool
CompileSymbols(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ଯଦି (file !=ଖାଲି)
        HandleSymbolsFile(&info, file);

    ଯଦି (info.error_ଗଣନା != 0)
        ଯାଅ err_info;

    ଯଦି (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        ଯାଅ err_info;

    ClearSymbolsInfo(&info);
    ଫେରନ୍ତୁ true;

err_info:
    ClearSymbolsInfo(&info);
    ଫେରନ୍ତୁ false;
}
