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

#అన్తర్భావ "config.h"

#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcommon/xkbcommon-keysyms.h"

#అన్తర్భావ "action.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-numbers.h"
#అన్తర్భావ "util-mem.h"
#అన్తర్భావ "vmod.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "xkbcomp/ast.h"

గణన కీలకం_repeat {
    కీలకం_REPEAT_UNDEFINED = 0,
    కీలకం_REPEAT_YES,
    కీలకం_REPEAT_NO,
    కీలకం_REPEAT_అంకె_ENTRIES,
};
గణన { కీలకం_REPEAT_MIN_వెడల్పు = 3 /* 2 bits + sign */ };
static_assert(కీలకం_REPEAT_అంకె_ENTRIES - 1 <=
              (1 << (కీలకం_REPEAT_MIN_వెడల్పు - 1)) - 1,
              "Cannot encode key repeat");

గణన సమూహం_క్షేత్రం {
    సమూహం_క్షేత్రం_SYMS = (1 << 0),
    సమూహం_క్షేత్రం_ACTS = (1 << 1),
    సమూహం_క్షేత్రం_TYPE = (1 << 2),
};

గణన కీలకం_క్షేత్రం {
    కీలకం_క్షేత్రం_NONE = 0,
    కీలకం_క్షేత్రం_REPEAT = (1 << 0),
    కీలకం_క్షేత్రం_DEFAULT_TYPE = (1 << 1),
    కీలకం_క్షేత్రం_GROUPINFO = (1 << 2),
    కీలకం_క్షేత్రం_VMODMAP = (1 << 3),
    కీలకం_క్షేత్రం_OVERLAY = (1 << 4),
    కీలకం_క్షేత్రం_ALL = కీలకం_క్షేత్రం_REPEAT | కీలకం_క్షేత్రం_DEFAULT_TYPE
                  | కీలకం_క్షేత్రం_GROUPINFO | కీలకం_క్షేత్రం_VMODMAP | కీలకం_క్షేత్రం_OVERLAY,
};
గణన { కీలకం_క్షేత్రం_MIN_వెడల్పు = 6 /* 5 bits + sign */ };
static_assert(కీలకం_క్షేత్రం_ALL <= (1 << (కీలకం_క్షేత్రం_MIN_వెడల్పు - 1)) - 1,
              "Cannot encode key field");

రకనిర్వచనం నిర్మాణం {
    darray(నిర్మాణం xkb_level) స్థాయిలు;
    గణన సమూహం_క్షేత్రం defined;
    xkb_atom_t type;
} సమూహం_Info_2;

గణన {
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_MIN_వెడల్పు = 4 /* 3 bits + sign */,
    KEYINFO_బహిర్గతం_యొక్క_RANGE_సమూహం_POLICY_బిట్టు_క్షేత్రం = 8,
    KEYINFO_DEFINED_బిట్టు_క్షేత్రం = 16,
    KEYINFO_విలీనం_బిట్టు_క్షేత్రం = 8,
    KEYINFO_REPEAT_బిట్టు_క్షేత్రం = 8,
};
static_assert(
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP < XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP &&
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP < XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_MIN_వెడల్పు) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_బహిర్గతం_యొక్క_RANGE_సమూహం_POLICY_బిట్టు_క్షేత్రం >=
              XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_MIN_వెడల్పు, "");
static_assert((గుర్తులేని)KEYINFO_DEFINED_బిట్టు_క్షేత్రం >=
              (గుర్తులేని)కీలకం_క్షేత్రం_MIN_వెడల్పు, "Cannot encode KeyInfo field");
static_assert((గుర్తులేని)KEYINFO_విలీనం_బిట్టు_క్షేత్రం >=
              (గుర్తులేని)విలీనం_బహుళకం_MIN_వెడల్పు, "Cannot encode KeyInfo field");
static_assert((గుర్తులేని)KEYINFO_REPEAT_బిట్టు_క్షేత్రం >=
              (గుర్తులేని)కీలకం_REPEAT_MIN_వెడల్పు, "Cannot encode KeyInfo field");

రకనిర్వచనం నిర్మాణం {
    xkb_atom_t name;

    xkb_mod_ముసుగు_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_సూచీ_t బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య;
    darray(సమూహం_Info_2) groups;
    గణన xkb_layout_బహిర్గతం_యొక్క_range_policy
        బహిర్గతం_యొక్క_range_సమూహం_policy:KEYINFO_బహిర్గతం_యొక్క_RANGE_సమూహం_POLICY_బిట్టు_క్షేత్రం;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    గణన కీలకం_క్షేత్రం defined:KEYINFO_DEFINED_బిట్టు_క్షేత్రం;
    గణన విలీనం_బహుళకం విలీనం:KEYINFO_విలీనం_బిట్టు_క్షేత్రం;
    గణన కీలకం_repeat repeat:KEYINFO_REPEAT_బిట్టు_క్షేత్రం;

    bool బహిర్గతం_యొక్క_range_pending_సమూహం:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_సూచీ_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_ముసుగు_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    సంఘం {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        స్థిరరాశి నిర్మాణం xkb_కీలకం *overlay_కీలకం;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        స్థిరరాశి నిర్మాణం xkb_కీలకం **overlays_కీలకాలు;
    };
} కీలకం_Info;

స్థిర శూన్య
StealLevelInfo(నిర్మాణం xkb_level *into, నిర్మాణం xkb_level *from)
{
    clear_level(into);
    ఒకవేళ (from->అంకె_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } లేకపోతే {
        into->s.sym = from->s.sym;
    }
    into->అంకె_syms = from->అంకె_syms;
    from->అంకె_syms = 0;

    ఒకవేళ (from->అంకె_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } లేకపోతే {
        into->a.action = from->a.action;
    }
    into->అంకె_actions = from->అంకె_actions;
    from->అంకె_actions = 0;
}

స్థిర శూన్య
Init_సమూహం_Info(సమూహం_Info_2 *groupi)
{
    memset(groupi, 0, పరిమాణం(*groupi));
}

స్థిర శూన్య
Clear_సమూహం_Info(సమూహం_Info_2 *groupi)
{
    నిర్మాణం xkb_level *leveli;
    darray_foreach(leveli, groupi->స్థాయిలు)
        clear_level(leveli);
    darray_free(groupi->స్థాయిలు);
}

స్థిర శూన్య
Copy_సమూహం_Info(సమూహం_Info_2 *గమ్యం, స్థిరరాశి సమూహం_Info_2 *from)
{
    గమ్యం->defined = from->defined;
    గమ్యం->type = from->type;
    darray_init(గమ్యం->స్థాయిలు);
    darray_copy(గమ్యం->స్థాయిలు, from->స్థాయిలు);
    కొరకు (xkb_level_సూచీ_t j = 0; j < darray_కొలత(గమ్యం->స్థాయిలు); j++) {
        ఒకవేళ (darray_అంశం(from->స్థాయిలు, j).అంకె_syms > 1) {
            darray_అంశం(గమ్యం->స్థాయిలు, j).s.syms =
                memdup(darray_అంశం(from->స్థాయిలు, j).s.syms,
                       darray_అంశం(from->స్థాయిలు, j).అంకె_syms,
                       పరిమాణం(xkb_keysym_t));
        }
        ఒకవేళ (darray_అంశం(from->స్థాయిలు, j).అంకె_actions > 1) {
            darray_అంశం(గమ్యం->స్థాయిలు, j).a.actions =
                memdup(darray_అంశం(from->స్థాయిలు, j).a.actions,
                       darray_అంశం(from->స్థాయిలు, j).అంకె_actions,
                       పరిమాణం(సంఘం xkb_action));
        }
    }
}

స్థిర శూన్య
Init_కీలకం_Info(నిర్మాణం xkb_context *ctx, కీలకం_Info *keyi)
{
    memset(keyi, 0, పరిమాణం(*keyi));
    static_assert(!DEFAULT_కీలకం_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_కీలకం_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->బహిర్గతం_యొక్క_range_సమూహం_policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_కీలకం = ఖాళీ;
}

స్థిర శూన్య
Clear_కీలకం_Info(కీలకం_Info *keyi)
{
    సమూహం_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_సమూహం_Info(groupi);
    darray_free(keyi->groups);
    ఒకవేళ (keyi->overlays_alloc)
        విడుదల(keyi->overlays_కీలకాలు);
}

/***====================================================================***/

రకనిర్వచనం నిర్మాణం {
    అక్షరం *name;         /* e.g. pc+us+inet(evdev) */
    పూర్ణాంకం error_లెక్క;
    గుర్తులేని పూర్ణాంకం include_లోతు;
    xkb_layout_సూచీ_t explicit_సమూహం;
    xkb_layout_సూచీ_t max_groups;
    darray(కీలకం_Info) కీలకాలు;
    కీలకం_Info default_కీలకం;
    ActionsInfo default_actions;
    darray(xkb_atom_t) సమూహం_పేర్లు;
    darray(Mod_Map_నమోదు) modmaps;
    నిర్మాణం xkb_mod_సమితి mods;

    నిర్మాణం xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info;
} SymbolsInfo;

స్థిర శూన్య
InitSymbolsInfo(SymbolsInfo *info, స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                గుర్తులేని పూర్ణాంకం include_లోతు, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods)
{
    memset(info, 0, పరిమాణం(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_లోతు = include_లోతు;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_కీలకం_Info(keymap_info->keymap.ctx, &info->default_కీలకం);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_లోతు > 0);
    info->explicit_సమూహం = XKB_LAYOUT_INVALID;
}

స్థిర శూన్య
ClearSymbolsInfo(SymbolsInfo *info)
{
    కీలకం_Info *keyi;
    విడుదల(info->name);
    darray_foreach(keyi, info->కీలకాలు)
        Clear_కీలకం_Info(keyi);
    darray_free(info->కీలకాలు);
    darray_free(info->సమూహం_పేర్లు);
    darray_free(info->modmaps);
    Clear_కీలకం_Info(&info->default_కీలకం);
}

స్థిర స్థిరరాశి అక్షరం *
కీలకం_Info_పాఠ్యం(SymbolsInfo *info, కీలకం_Info *keyi)
{
    తిరిగి కీలకం_Name_పాఠ్యం(info->ctx, keyi->name);
}

స్థిర bool
విలీనం_Groups(SymbolsInfo *info, సమూహం_Info_2 *into, సమూహం_Info_2 *from, bool clobber,
            bool report, xkb_layout_సూచీ_t సమూహం, xkb_atom_t కీలకం_name)
{
    /* First find the type of the merged group. */
    ఒకవేళ (into->type != from->type) {
        ఒకవేళ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        లేకపోతే ఒకవేళ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        లేకపోతే {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ఒకవేళ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         సమూహం + 1, కీలకం_Name_పాఠ్యం(info->ctx, కీలకం_name),
                         xkb_atom_పాఠ్యం(info->ctx, use),
                         xkb_atom_పాఠ్యం(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & సమూహం_క్షేత్రం_TYPE);

    /* Now look at the levels. */

    ఒకవేళ (darray_ఖాళీలు(from->స్థాయిలు)) {
        Init_సమూహం_Info(from);
        తిరిగి true;
    }

    ఒకవేళ (darray_ఖాళీలు(into->స్థాయిలు)) {
        from->type = into->type;
        *into = *from;
        Init_సమూహం_Info(from);
        తిరిగి true;
    }

    /* Merge the actions and syms. */
    స్థిరరాశి darray_కొలత_t స్థాయిలు_లోపల_both =
        MIN(darray_కొలత(into->స్థాయిలు), darray_కొలత(from->స్థాయిలు));
    darray_కొలత_t from_Keysyms_లెక్క = 0;
    darray_కొలత_t from_Actions_లెక్క = 0;
    కొరకు (darray_కొలత_t i = 0; i < స్థాయిలు_లోపల_both; i++) {
        నిర్మాణం xkb_level* స్థిరరాశి intoLevel = &darray_అంశం(into->స్థాయిలు, i);
        నిర్మాణం xkb_level* స్థిరరాశి fromLevel = &darray_అంశం(from->స్థాయిలు, i);

        స్థిరరాశి bool fromHasNoKeysym = fromLevel->అంకె_syms == 0;
        స్థిరరాశి bool fromHasNoAction = fromLevel->అంకె_actions == 0;
        ఒకవేళ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            కొనసాగు;
        }

        స్థిరరాశి bool intoHasNoKeysym = intoLevel->అంకె_syms == 0;
        స్థిరరాశి bool intoHasNoAction = intoLevel->అంకె_actions == 0;
        ఒకవేళ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_లెక్క++;
            from_Actions_లెక్క++;
            కొనసాగు;
        }

        /* Possible level conflict */
        ప్రతిజ్ఞా(intoLevel->అంకె_syms > 0 || intoLevel->అంకె_actions > 0);
        ప్రతిజ్ఞా(fromLevel->అంకె_syms > 0 || fromLevel->అంకె_actions > 0);

        /* Handle keysyms */
        ఒకవేళ (!Xkb_స్థాయిలు_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ఒకవేళ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_చిహ్నం,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, సమూహం + 1, కీలకం_Name_పాఠ్యం(info->ctx, కీలకం_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ఒకవేళ (fromHasNoKeysym) {
                /* No keysym to copy */
            } లేకపోతే ఒకవేళ (clobber) {
                /* Override: copy any defined keysym from `from` */
                ఒకవేళ (unlikely(fromLevel->అంకె_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ఒకవేళ (unlikely(intoLevel->అంకె_syms > 1))
                        విడుదల(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->అంకె_syms = fromLevel->అంకె_syms;
                    fromLevel->అంకె_syms = 0;
                    from_Keysyms_లెక్క++;
                } లేకపోతే ఒకవేళ (fromLevel->s.sym != XKB_కీలకం_No_చిహ్నం) {
                    /* Single defined keysym */
                    ఒకవేళ (unlikely(intoLevel->అంకె_syms > 1))
                        విడుదల(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->అంకె_syms = 1;
                    from_Keysyms_లెక్క++;
                }
            } లేకపోతే {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ఒకవేళ (unlikely(intoLevel->అంకె_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } లేకపోతే ఒకవేళ (intoLevel->s.sym == XKB_కీలకం_No_చిహ్నం) {
                    /* Single undefined keysym */
                    ఒకవేళ (unlikely(fromLevel->అంకె_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->అంకె_syms = fromLevel->అంకె_syms;
                        fromLevel->అంకె_syms = 0;
                    } లేకపోతే {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->అంకె_syms = 1;
                    }
                    from_Keysyms_లెక్క++;
                }
            }
        }

        /* Handle actions */
        ఒకవేళ (!Xkb_స్థాయిలు_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ఒకవేళ (report && !(intoHasNoAction || fromHasNoAction)) {
                ఒకవేళ (unlikely(intoLevel->అంకె_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_కీలకం_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, సమూహం + 1, కీలకం_Name_పాఠ్యం(info->ctx, కీలకం_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } లేకపోతే {
                    సంఘం xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, సమూహం + 1,
                                కీలకం_Name_పాఠ్యం(info->ctx, కీలకం_name),
                                Action_Type_పాఠ్యం(use->type),
                                Action_Type_పాఠ్యం(ignore->type));
                }
            }
            ఒకవేళ (fromHasNoAction) {
                /* No action to copy */
            } లేకపోతే ఒకవేళ (clobber) {
                /* Override: copy any defined action from `from` */
                ఒకవేళ (unlikely(fromLevel->అంకె_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ఒకవేళ (unlikely(intoLevel->అంకె_actions > 1))
                        విడుదల(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->అంకె_actions = fromLevel->అంకె_actions;
                    fromLevel->అంకె_actions = 0;
                    from_Actions_లెక్క++;
                } లేకపోతే ఒకవేళ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ఒకవేళ (unlikely(intoLevel->అంకె_actions > 1))
                        విడుదల(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->అంకె_actions = 1;
                    from_Actions_లెక్క++;
                }
            } లేకపోతే {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ఒకవేళ (unlikely(intoLevel->అంకె_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } లేకపోతే ఒకవేళ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ఒకవేళ (unlikely(fromLevel->అంకె_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->అంకె_actions = fromLevel->అంకె_actions;
                        fromLevel->అంకె_actions = 0;
                    } లేకపోతే {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->అంకె_actions = 1;
                    }
                    from_Actions_లెక్క++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    నిర్మాణం xkb_level *level;
    darray_foreach_from(level, from->స్థాయిలు, స్థాయిలు_లోపల_both) {
        darray_append(into->స్థాయిలు, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->అంకె_syms = 0;
        level->అంకె_actions = 0;
        from_Keysyms_లెక్క++;
        from_Actions_లెక్క++;
    }
    ఒకవేళ (from_Keysyms_లెక్క) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ఒకవేళ (from_Keysyms_లెక్క == darray_కొలత(into->స్థాయిలు))
            into->defined &= ~సమూహం_క్షేత్రం_SYMS;
        into->defined |= (from->defined & సమూహం_క్షేత్రం_SYMS);
    }
    ఒకవేళ (from_Actions_లెక్క) {
        /* Reset defined actions field if we used no action from `into` */
        ఒకవేళ (from_Actions_లెక్క == darray_కొలత(into->స్థాయిలు))
            into->defined &= ~సమూహం_క్షేత్రం_ACTS;
        into->defined |= (from->defined & సమూహం_క్షేత్రం_ACTS);
    }

    తిరిగి true;
}

స్థిర bool
Use_New_కీలకం_క్షేత్రం(గణన కీలకం_క్షేత్రం క్షేత్రం_2, గణన కీలకం_క్షేత్రం old, గణన కీలకం_క్షేత్రం new,
               bool clobber, bool report, గణన కీలకం_క్షేత్రం *collide)
{
    ఒకవేళ (!(old & క్షేత్రం_2))
        తిరిగి (new & క్షేత్రం_2);

    ఒకవేళ (new & క్షేత్రం_2) {
        ఒకవేళ (report)
            *collide |= క్షేత్రం_2;

        తిరిగి clobber;
    }

    తిరిగి false;
}

స్థిర bool
overlays_get(స్థిరరాశి కీలకం_Info *info, xkb_overlay_సూచీ_t బిట్టు,
             స్థిరరాశి నిర్మాణం xkb_కీలకం **కీలకం_బహిర్గతం)
{
    ఒకవేళ (బిట్టు >= (xkb_overlay_సూచీ_t)(పరిమాణం(xkb_overlay_ముసుగు_t) * CHAR_BIT))
        తిరిగి false;
    స్థిరరాశి xkb_overlay_ముసుగు_t ముసుగు = (xkb_overlay_ముసుగు_t)(1u << బిట్టు);
    ఒకవేళ (!(info->overlays & ముసుగు)) తిరిగి false;
    ఒకవేళ (కీలకం_బహిర్గతం) {
        ఒకవేళ (!info->overlays_alloc) {
            /* Stored inline */
            *కీలకం_బహిర్గతం = info->overlay_కీలకం;
        } లేకపోతే {
            /* Stored in sparse array */
            స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
                info->overlays & (ముసుగు - 1u)
            );
            స్థిరరాశి xkb_overlay_సూచీ_t సూచీ = (xkb_overlay_సూచీ_t)popcount32(దిగువ);
            *కీలకం_బహిర్గతం = info->overlays_కీలకాలు[సూచీ];
        }
    }
    తిరిగి true;
}

స్థిర bool
overlays_చొప్పించు(కీలకం_Info *keyi, xkb_overlay_సూచీ_t బిట్టు,
                స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం)
{
    ఒకవేళ (బిట్టు >= (xkb_overlay_సూచీ_t)(పరిమాణం(xkb_overlay_ముసుగు_t) * CHAR_BIT))
        తిరిగి false;

    స్థిరరాశి xkb_overlay_ముసుగు_t ముసుగు = (xkb_overlay_ముసుగు_t)(1u << బిట్టు);

    ఒకవేళ ((keyi->overlays & ముసుగు) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ఒకవేళ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_కీలకం = కీలకం;
            ఒకవేళ (!కీలకం)
                keyi->overlays_clear = true;
        } లేకపోతే {
            /* Stored in sparse array */
            స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
                keyi->overlays & (xkb_overlay_ముసుగు_t)(ముసుగు - 1u)
            );
            /* index = number of set bits strictly below bit */
            స్థిరరాశి xkb_overlay_సూచీ_t సూచీ =
                (xkb_overlay_సూచీ_t)popcount32(దిగువ);
            keyi->overlays_కీలకాలు[సూచీ] = కీలకం;
        }
        తిరిగి true;
    }

    /* Insert new element */
    ఒకవేళ (!keyi->overlays || (keyi->overlays_clear && !కీలకం)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_కీలకం = కీలకం;
        keyi->overlays |= ముసుగు;
        keyi->overlays_clear = !కీలకం;
    } లేకపోతే ఒకవేళ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        స్థిరరాశి xkb_overlay_ముసుగు_t overlays = keyi->overlays | ముసుగు;
        స్థిరరాశి xkb_overlay_సూచీ_t alloc =
            (xkb_overlay_సూచీ_t)popcount32(overlays);

        /* Alloc sparse array */
        స్థిరరాశి నిర్మాణం xkb_కీలకం ** స్థిరరాశి tmp = సున్నా_జ్ఞాపకం_ఇవ్వు(alloc, పరిమాణం(*tmp));
        ఒకవేళ (!tmp) తిరిగి false;

        /* Copy previous entries to the buffer */
        ఒకవేళ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
                overlays & (xkb_overlay_ముసుగు_t)(keyi->overlays - 1u)
            );
            స్థిరరాశి xkb_overlay_సూచీ_t idx = (xkb_overlay_సూచీ_t)popcount32(దిగువ);
            tmp[idx] = keyi->overlay_కీలకం;
        } లేకపోతే {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
            overlays & (xkb_overlay_ముసుగు_t)(ముసుగు - 1u)
        );
        స్థిరరాశి xkb_overlay_సూచీ_t idx = (xkb_overlay_సూచీ_t)popcount32(దిగువ);
        tmp[idx] = కీలకం;

        keyi->overlays_కీలకాలు = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } లేకపోతే {
        /* Allocated items: grow heap and shift if relevant */
        స్థిరరాశి xkb_overlay_ముసుగు_t overlays =
            (xkb_overlay_ముసుగు_t)(keyi->overlays | ముసుగు);
        స్థిరరాశి xkb_overlay_సూచీ_t లెక్క =
            (xkb_overlay_సూచీ_t)popcount32(overlays);
        ఒకవేళ (లెక్క > keyi->overlays_alloc) {
            స్థిరరాశి xkb_overlay_సూచీ_t alloc =
                (xkb_overlay_సూచీ_t)తదుపరి_pow2(లెక్క);
            ప్రతిజ్ఞా(alloc >= లెక్క);
            స్థిరరాశి నిర్మాణం xkb_కీలకం ** స్థిరరాశి tmp = realloc(
                keyi->overlays_కీలకాలు,
                (కొలత_t)alloc * పరిమాణం(*keyi->overlays_కీలకాలు)
            );
            ఒకవేళ (!tmp) తిరిగి false;
            keyi->overlays_కీలకాలు = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
            overlays & (xkb_overlay_ముసుగు_t)(ముసుగు - 1u)
        );
        స్థిరరాశి xkb_overlay_సూచీ_t సూచీ = (xkb_overlay_సూచీ_t)popcount32(దిగువ);
        ఒకవేళ (సూచీ >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ఒకవేళ (సూచీ < లెక్క - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ప్రతిజ్ఞా(సూచీ + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_కీలకాలు + సూచీ + 1, keyi->overlays_కీలకాలు + సూచీ,
                    (కొలత_t)(లెక్క - 1u - సూచీ) * పరిమాణం(స్థిరరాశి నిర్మాణం xkb_కీలకం *));
        }
        keyi->overlays_కీలకాలు[సూచీ] = కీలకం;
        keyi->overlays = overlays;
    }

    తిరిగి true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
స్థిర bool
విలీనం_overlays(SymbolsInfo *info, కీలకం_Info *into, కీలకం_Info *from,
               bool clobber, bool report, గణన కీలకం_క్షేత్రం *collide)
{
    ఒకవేళ (!(from->defined & కీలకం_క్షేత్రం_OVERLAY)) {
        /* `from` is empty: do nothing */
    } లేకపోతే ఒకవేళ (!(into->defined & కీలకం_క్షేత్రం_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_కీలకాలు = from->overlays_కీలకాలు;
        from->overlays_కీలకాలు = ఖాళీ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= కీలకం_క్షేత్రం_OVERLAY;
    } లేకపోతే ఒకవేళ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } లేకపోతే {
        ఒకవేళ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            స్థిరరాశి xkb_overlay_ముసుగు_t ఫలితం_ముసుగు =
                (into->overlays | from->overlays);
            స్థిరరాశి xkb_overlay_సూచీ_t లెక్క =
                (xkb_overlay_సూచీ_t)popcount32(ఫలితం_ముసుగు);

            ఒకవేళ (లెక్క == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            కీలకం_Info *dest = into;
            కీలకం_Info *src = from;
            ఒకవేళ (into->overlays_alloc >= లెక్క &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } లేకపోతే ఒకవేళ (from->overlays_alloc >= లెక్క) {
                dest = from;
                src = into;
                clobber = !clobber;
            } లేకపోతే ఒకవేళ (లెక్క > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ఒకవేళ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ఒకవేళ (!dest->overlays_alloc) {
                    స్థిరరాశి నిర్మాణం xkb_కీలకం **tmp = సున్నా_జ్ఞాపకం_ఇవ్వు(లెక్క, పరిమాణం(*tmp));
                    ఒకవేళ (!tmp)
                        తిరిగి false;
                    ఒకవేళ (dest->overlay_కీలకం)
                        tmp[0] = dest->overlay_కీలకం;
                    dest->overlays_కీలకాలు = tmp;
                    dest->overlays_alloc = లెక్క;
                } లేకపోతే {
                    స్థిరరాశి నిర్మాణం xkb_కీలకం **tmp =
                        realloc(dest->overlays_కీలకాలు,
                                లెక్క * పరిమాణం(*dest->overlays_కీలకాలు));
                    ఒకవేళ (!tmp)
                        తిరిగి false;
                    dest->overlays_కీలకాలు = tmp;
                    dest->overlays_alloc = లెక్క;
                }
            }

            xkb_overlay_ముసుగు_t మిగిలిన = src->overlays;
            స్థిరరాశి నిర్మాణం xkb_కీలకం **src_కీలకాలు = (src->overlays_clear)
                ? ఖాళీ
                : (src->overlays_alloc)
                    ? src->overlays_కీలకాలు
                    : &src->overlay_కీలకం;
            అయితే (మిగిలిన) {
                /* isolate lowest set bit */
                స్థిరరాశి xkb_overlay_ముసుగు_t lsb = (xkb_overlay_ముసుగు_t)(
                    మిగిలిన &
                    (xkb_overlay_ముసుగు_t)((గుర్తులేని)~మిగిలిన + 1u)
                );
                /* get its index */
                స్థిరరాశి xkb_overlay_సూచీ_t బిట్టు =
                    (xkb_overlay_సూచీ_t)popcount32(lsb - 1u);
                మిగిలిన = (మిగిలిన & ~lsb);

                /* pop current value */
                ఒకవేళ (!src->overlays_clear && !src->overlays_alloc && మిగిలిన)
                    PANIC_UNREACHABLE();
                స్థిరరాశి నిర్మాణం xkb_కీలకం *src_కీలకం = (src_కీలకాలు)
                    ? *(src_కీలకాలు++)
                    : ఖాళీ;

                /* check dest */
                స్థిరరాశి నిర్మాణం xkb_కీలకం *dest_కీలకం = ఖాళీ;
                స్థిరరాశి bool conflict = overlays_get(dest, బిట్టు, &dest_కీలకం);
                ఒకవేళ (conflict) {
                    ఒకవేళ (dest_కీలకం == src_కీలకం)
                       కొనసాగు;
                    లేకపోతే ఒకవేళ (report)
                        *collide |= కీలకం_క్షేత్రం_OVERLAY;
                }

                ఒకవేళ ((!conflict || clobber) && !overlays_చొప్పించు(dest, బిట్టు, src_కీలకం))
                    తిరిగి false;
            }

            ఒకవేళ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ఒకవేళ (into->overlays_alloc)
                    విడుదల(into->overlays_కీలకాలు);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ఒకవేళ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_కీలకాలు = from->overlays_కీలకాలు;
                    from->overlays_కీలకాలు = ఖాళీ;
                    from->overlays_alloc = 0;
                } లేకపోతే {
                    into->overlay_కీలకం = from->overlay_కీలకం;
                }
            }
        } లేకపోతే {
            /* Disjoint overlays */

            ఒకవేళ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_కీలకం == from->overlay_కీలకం) {
                /* Indentical: do nothing */
                తిరిగి true;
            }
            ఒకవేళ (!(into->overlays & from->overlays)) {
                ఒకవేళ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_కీలకం = from->overlay_కీలకం;
                    తిరిగి true;
                } లేకపోతే ఒకవేళ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    తిరిగి true;
                }
            }
            /* Conflict */
            ఒకవేళ (report)
                *collide |= కీలకం_క్షేత్రం_OVERLAY;
            ఒకవేళ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_కీలకం = from->overlay_కీలకం;
            }
        }
    }
    తిరిగి true;
}

స్థిర bool
విలీనం_కీలకాలు(SymbolsInfo *info, కీలకం_Info *into, కీలకం_Info *from, bool same_file)
{
    xkb_layout_సూచీ_t i;
    xkb_layout_సూచీ_t groups_లోపల_both;
    గణన కీలకం_క్షేత్రం collide = 0;
    స్థిరరాశి పూర్ణాంకం verbosity = xkb_context_get_log_verbosity(info->ctx);
    స్థిరరాశి bool clobber = (from->విలీనం != విలీనం_AUGMENT);
    స్థిరరాశి bool report = (same_file && verbosity > 0) || verbosity > 9;

    ఒకవేళ (from->విలీనం == విలీనం_REPLACE) {
        Clear_కీలకం_Info(into);
        *into = *from;
        Init_కీలకం_Info(info->ctx, from);
        తిరిగి true;
    }

    groups_లోపల_both = MIN(darray_కొలత(into->groups), darray_కొలత(from->groups));
    కొరకు (i = 0; i < groups_లోపల_both; i++)
        విలీనం_Groups(info,
                    &darray_అంశం(into->groups, i),
                    &darray_అంశం(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    కొరకు (i = groups_లోపల_both; i < darray_కొలత(from->groups); i++) {
        darray_append(into->groups, darray_అంశం(from->groups, i));
        Init_సమూహం_Info(&darray_అంశం(from->groups, i));
    }

    ఒకవేళ (Use_New_కీలకం_క్షేత్రం(కీలకం_క్షేత్రం_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= కీలకం_క్షేత్రం_VMODMAP;
    }
    ఒకవేళ (Use_New_కీలకం_క్షేత్రం(కీలకం_క్షేత్రం_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= కీలకం_క్షేత్రం_REPEAT;
    }
    ఒకవేళ (Use_New_కీలకం_క్షేత్రం(కీలకం_క్షేత్రం_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= కీలకం_క్షేత్రం_DEFAULT_TYPE;
    }
    ఒకవేళ (Use_New_కీలకం_క్షేత్రం(కీలకం_క్షేత్రం_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->బహిర్గతం_యొక్క_range_pending_సమూహం = from->బహిర్గతం_యొక్క_range_pending_సమూహం;
        into->బహిర్గతం_యొక్క_range_సమూహం_policy = from->బహిర్గతం_యొక్క_range_సమూహం_policy;
        into->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = from->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య;
        into->defined |= కీలకం_క్షేత్రం_GROUPINFO;
    }

    ఒకవేళ (!విలీనం_overlays(info, into, from, clobber, report, &collide))
        తిరిగి false;

    ఒకవేళ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 కీలకం_Name_పాఠ్యం(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_కీలకం_Info(from);
    Init_కీలకం_Info(info->ctx, from);
    తిరిగి true;
}

స్థిర xkb_atom_t
Xkb_Resolve_కీలకం_Alias(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, xkb_atom_t name)
{
    ఒకవేళ (name < keymap->అంకె_కీలకం_పేర్లు) {
        స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక = keymap->కీలకం_పేర్లు[name];
        ఒకవేళ (సరిపోలిక.దొరికింది && సరిపోలిక.is_alias) {
            తిరిగి సరిపోలిక.alias.real;
        }
    }
    తిరిగి name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
స్థిర bool
కూడు_కీలకం_Symbols(SymbolsInfo *info, కీలకం_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_కీలకం_Alias(&info->keymap_info->keymap, keyi->name);

    కీలకం_Info *iter;
    darray_foreach(iter, info->కీలకాలు)
        ఒకవేళ (iter->name == keyi->name)
            తిరిగి విలీనం_కీలకాలు(info, iter, keyi, same_file);

    darray_append(info->కీలకాలు, *keyi);
    Init_కీలకం_Info(info->ctx, keyi);
    తిరిగి true;
}

స్థిర bool
కూడు_Mod_Map_నమోదు(SymbolsInfo *info, స్థిరరాశి Mod_Map_నమోదు *new)
{
    స్థిరరాశి bool clobber = (new->విలీనం != విలీనం_AUGMENT);

    Mod_Map_నమోదు *old;
    darray_foreach(old, info->modmaps) {
        ఒకవేళ ((new->have_చిహ్నం != old->have_చిహ్నం) ||
            (new->have_చిహ్నం && new->u.కీలకం_Sym != old->u.కీలకం_Sym) ||
            (!new->have_చిహ్నం && new->u.కీలకం_Name != old->u.కీలకం_Name))
            కొనసాగు;

        ఒకవేళ (new->mods == old->mods)
            తిరిగి true;

        స్థిరరాశి xkb_mod_ముసుగు_t use = (clobber ? new->mods : old->mods);
        స్థిరరాశి xkb_mod_ముసుగు_t ignore = (clobber ? old->mods : new->mods);

        ఒకవేళ (new->have_చిహ్నం) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_పాఠ్యం(info->ctx, new->u.కీలకం_Sym),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, ignore));
        } లేకపోతే {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    కీలకం_Name_పాఠ్యం(info->ctx, new->u.కీలకం_Name),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        తిరిగి true;
    }

    darray_append(info->modmaps, *new);

    తిరిగి true;
}

/***====================================================================***/

స్థిర శూన్య
విలీనం_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     గణన విలీనం_బహుళకం విలీనం)
{
    xkb_atom_t *సమూహం_name;
    xkb_layout_సూచీ_t సమూహం_పేర్లు_లోపల_both;

    ఒకవేళ (from->error_లెక్క > 0) {
        into->error_లెక్క += from->error_లెక్క;
        తిరిగి;
    }

    విలీనం_Mod_Sets(into->ctx, &into->mods, &from->mods, విలీనం);

    ఒకవేళ (into->name == ఖాళీ) {
        into->name = steal(&from->name);
    }

    సమూహం_పేర్లు_లోపల_both = MIN(darray_కొలత(into->సమూహం_పేర్లు),
                              darray_కొలత(from->సమూహం_పేర్లు));
    కొరకు (xkb_layout_సూచీ_t i = 0; i < సమూహం_పేర్లు_లోపల_both; i++) {
        ఒకవేళ (!darray_అంశం(from->సమూహం_పేర్లు, i))
            కొనసాగు;

        ఒకవేళ (విలీనం == విలీనం_AUGMENT && darray_అంశం(into->సమూహం_పేర్లు, i))
            కొనసాగు;

        darray_అంశం(into->సమూహం_పేర్లు, i) = darray_అంశం(from->సమూహం_పేర్లు, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(సమూహం_name, from->సమూహం_పేర్లు, సమూహం_పేర్లు_లోపల_both)
        darray_append(into->సమూహం_పేర్లు, *సమూహం_name);

    ఒకవేళ (darray_ఖాళీలు(into->కీలకాలు)) {
        into->కీలకాలు = from->కీలకాలు;
        darray_init(from->కీలకాలు);
    }
    లేకపోతే {
        కీలకం_Info *keyi;
        darray_foreach(keyi, from->కీలకాలు) {
            keyi->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_కీలకం_Symbols(into, keyi, false))
                into->error_లెక్క++;
        }
    }

    ఒకవేళ (darray_ఖాళీలు(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    లేకపోతే {
        Mod_Map_నమోదు *mm;
        darray_foreach(mm, from->modmaps) {
            mm->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_Mod_Map_నమోదు(into, mm))
                into->error_లెక్క++;
        }
    }
}

స్థిర శూన్య
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

స్థిర bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ఒకవేళ (Exceeds_Include_Max_లోతు(info->ctx, info->include_లోతు)) {
        info->error_లెక్క += 10;
        తిరిగి false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_లోతు + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    కొరకు (IncludeStmt *stmt = include; stmt; stmt = stmt->తదుపరి_incl) {
        SymbolsInfo తదుపరి_incl;
        XkbFile *file;

        అక్షరం మార్గం[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  మార్గం, పరిమాణం(మార్గం));
        ఒకవేళ (!file) {
            info->error_లెక్క += 10;
            ClearSymbolsInfo(&included);
            తిరిగి false;
        }

        InitSymbolsInfo(&తదుపరి_incl, info->keymap_info, info->include_లోతు + 1,
                        &included.mods);
        ఒకవేళ (stmt->modifier) {
            స్థిరరాశి పూర్ణాంకం లెక్క = parse_dec_గమ్యం_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &తదుపరి_incl.explicit_సమూహం);
            ఒకవేళ (లెక్క <= 0 || *(stmt->modifier + లెక్క) != '\0' ||
                తదుపరి_incl.explicit_సమూహం == 0 ||
                తదుపరి_incl.explicit_సమూహం > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                తదుపరి_incl.explicit_సమూహం = info->explicit_సమూహం;
            } లేకపోతే {
                తదుపరి_incl.explicit_సమూహం--;
            }
        }
        లేకపోతే ఒకవేళ (info->keymap_info->keymap.అంకె_groups != 0 &&
                 తదుపరి_incl.include_లోతు == 1) {
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
            తదుపరి_incl.explicit_సమూహం = 0;
        }
        లేకపోతే {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            తదుపరి_incl.explicit_సమూహం = info->explicit_సమూహం;
        }

        HandleSymbolsFile(&తదుపరి_incl, file);

        విలీనం_Included_Symbols(&included, &తదుపరి_incl, stmt->విలీనం);

        ClearSymbolsInfo(&తదుపరి_incl);
        FreeXkbFile(file);
    }

    విలీనం_Included_Symbols(info, &included, include->విలీనం);
    ClearSymbolsInfo(&included);

    తిరిగి (info->error_లెక్క == 0);
}

స్థిర bool
Get_సమూహం_సూచీ(SymbolsInfo *info, కీలకం_Info *keyi, సమాసం_Def *arrayNdx,
              గణన సమూహం_క్షేత్రం క్షేత్రం_2, xkb_layout_సూచీ_t *ndx_rtrn)
{
    ప్రతిజ్ఞా (క్షేత్రం_2 == సమూహం_క్షేత్రం_SYMS || క్షేత్రం_2 == సమూహం_క్షేత్రం_ACTS);
    స్థిరరాశి అక్షరం *name = (క్షేత్రం_2 == సమూహం_క్షేత్రం_SYMS ? "symbols" : "actions");

    ఒకవేళ (arrayNdx == ఖాళీ) {
        xkb_layout_సూచీ_t i = 0;
        సమూహం_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ఒకవేళ (!(groupi->defined & క్షేత్రం_2)) {
                *ndx_rtrn = i;
                తిరిగి true;
            }
        }

        ఒకవేళ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, కీలకం_Info_పాఠ్యం(info, keyi), info->max_groups, name);
            తిరిగి false;
        }

        darray_resize0(keyi->groups, darray_కొలత(keyi->groups) + 1);
        *ndx_rtrn = darray_కొలత(keyi->groups) - 1;
        తిరిగి true;
    }

    ఒకవేళ (సమాసం_Resolve_సమూహం(info->keymap_info, arrayNdx, false, ndx_rtrn, ఖాళీ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, కీలకం_Info_పాఠ్యం(info, keyi));
        తిరిగి false;
    }

    (*ndx_rtrn)--;
    ఒకవేళ (*ndx_rtrn >= darray_కొలత(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    తిరిగి true;
}

స్థిర bool
కూడు_Symbols_గమ్యం_కీలకం(SymbolsInfo *info, కీలకం_Info *keyi, సమాసం_Def *arrayNdx,
                సమాసం_Def *విలువ)
{
    xkb_layout_సూచీ_t ndx = 0;

    ఒకవేళ (!Get_సమూహం_సూచీ(info, keyi, arrayNdx, సమూహం_క్షేత్రం_SYMS, &ndx))
        తిరిగి false;

    సమూహం_Info_2 *groupi = &darray_అంశం(keyi->groups, ndx);

    ఒకవేళ (విలువ->common.type == STMT_సమాసం_ఖాళీలు_LIST) {
        groupi->defined |= సమూహం_క్షేత్రం_SYMS;
        తిరిగి true;
    }

    ఒకవేళ (విలువ->common.type != STMT_సమాసం_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_గమ్యం_string(విలువ->common.type), ndx + 1,
                కీలకం_Info_పాఠ్యం(info, keyi));
        తిరిగి false;
    }

    ఒకవేళ (groupi->defined & సమూహం_క్షేత్రం_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_కీలకం_SYMBOLS_నమోదు,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                కీలకం_Info_పాఠ్యం(info, keyi), ndx + 1);
        తిరిగి false;
    }

    xkb_level_సూచీ_t n_స్థాయిలు = 0;
    xkb_level_సూచీ_t non_ఖాళీలు_స్థాయిలు = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    కొరకు (సమాసం_Keysym_List *keysymList = (సమాసం_Keysym_List *) విలువ;
         keysymList;
         keysymList = (సమాసం_Keysym_List *) keysymList->common.తదుపరి) {
        n_స్థాయిలు++;
        /* Drop trailing NoSymbol */
        ఒకవేళ (darray_కొలత(keysymList->syms) > 0)
            non_ఖాళీలు_స్థాయిలు = n_స్థాయిలు;
    }
    ఒకవేళ (non_ఖాళీలు_స్థాయిలు < n_స్థాయిలు)
        n_స్థాయిలు = non_ఖాళీలు_స్థాయిలు;
    ఒకవేళ (darray_కొలత(groupi->స్థాయిలు) < n_స్థాయిలు)
        darray_resize0(groupi->స్థాయిలు, n_స్థాయిలు);

    groupi->defined |= సమూహం_క్షేత్రం_SYMS;

    xkb_level_సూచీ_t level = 0;
    కొరకు (సమాసం_Keysym_List *keysymList = (సమాసం_Keysym_List *) విలువ;
         keysymList && level < n_స్థాయిలు;
         keysymList = (సమాసం_Keysym_List *) keysymList->common.తదుపరి, level++) {
        నిర్మాణం xkb_level *leveli = &darray_అంశం(groupi->స్థాయిలు, level);
        ప్రతిజ్ఞా(leveli->అంకె_syms == 0);

        ఒకవేళ (unlikely(darray_కొలత(keysymList->syms) > MAX_KEYSYMS_చొప్పున_LEVEL)) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    కీలకం_Info_పాఠ్యం(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_చొప్పున_LEVEL, darray_కొలత(keysymList->syms));
            తిరిగి false;
        }

        leveli->అంకె_syms = (xkb_keysym_లెక్క_t) darray_కొలత(keysymList->syms);
        ఎంపిక (leveli->అంకె_syms) {
        స్థితి 0:
            leveli->s.sym = XKB_కీలకం_No_చిహ్నం;
            ఆపు;
        స్థితి 1:
            leveli->s.sym = darray_అంశం(keysymList->syms, 0);
            ప్రతిజ్ఞా(leveli->s.sym != XKB_కీలకం_No_చిహ్నం);
            ఆపు;
        అప్రమేయ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ఖాళీ);
#నిర్వచితం_కాకపోతే NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            కొరకు (xkb_keysym_లెక్క_t k = 0; k < leveli->అంకె_syms; k++)
                ప్రతిజ్ఞా(leveli->s.syms[k] != XKB_కీలకం_No_చిహ్నం);
#అయితే_ముగింపు
        }
    }

    తిరిగి true;
}

స్థిర bool
కూడు_Actions_గమ్యం_కీలకం(SymbolsInfo *info, కీలకం_Info *keyi, సమాసం_Def *arrayNdx,
                సమాసం_Def *విలువ)
{
    xkb_layout_సూచీ_t ndx = 0;

    ఒకవేళ (!Get_సమూహం_సూచీ(info, keyi, arrayNdx, సమూహం_క్షేత్రం_ACTS, &ndx))
        తిరిగి false;

    సమూహం_Info_2 *groupi = &darray_అంశం(keyi->groups, ndx);

    ఒకవేళ (విలువ->common.type == STMT_సమాసం_ఖాళీలు_LIST) {
        groupi->defined |= సమూహం_క్షేత్రం_ACTS;
        తిరిగి true;
    }

    ఒకవేళ (విలువ->common.type != STMT_సమాసం_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_వ్యక్తీకరణ_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 విలువ->common.type, ndx, కీలకం_Info_పాఠ్యం(info, keyi));
        తిరిగి false;
    }

    ఒకవేళ (groupi->defined & సమూహం_క్షేత్రం_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 కీలకం_Info_పాఠ్యం(info, keyi), ndx);
        తిరిగి false;
    }

    xkb_level_సూచీ_t n_స్థాయిలు = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    కొరకు (ParseCommon *p = &విలువ->common; p; p = p->తదుపరి)
        n_స్థాయిలు++;
    ఒకవేళ (darray_కొలత(groupi->స్థాయిలు) < n_స్థాయిలు)
        darray_resize0(groupi->స్థాయిలు, n_స్థాయిలు);

    groupi->defined |= సమూహం_క్షేత్రం_ACTS;

    xkb_level_సూచీ_t level = 0;
    xkb_level_సూచీ_t non_ఖాళీలు_స్థాయిలు = 0;
    కొరకు (సమాసం_Action_List *actionList = (సమాసం_Action_List *) విలువ;
         actionList;
         actionList = (సమాసం_Action_List *) actionList->common.తదుపరి, level++) {
        నిర్మాణం xkb_level *leveli = &darray_అంశం(groupi->స్థాయిలు, level);
        ప్రతిజ్ఞా(leveli->అంకె_actions == 0);

        గుర్తులేని పూర్ణాంకం అంకె_actions = 0;
        కొరకు (సమాసం_Def *act = actionList->actions;
             act; act = (సమాసం_Def *) act->common.తదుపరి)
            అంకె_actions++;

        ఒకవేళ (unlikely(అంకె_actions > MAX_ACTIONS_చొప్పున_LEVEL)) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    కీలకం_Info_పాఠ్యం(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_చొప్పున_LEVEL, అంకె_actions);
            తిరిగి false;
        }

        /* Parse actions and add only defined actions */
        darray(సంఘం xkb_action) actions = darray_new();
        కొరకు (సమాసం_Def *act = actionList->actions;
             act; act = (సమాసం_Def *) act->common.తదుపరి) {
            సంఘం xkb_action గమ్యం_Act = { 0 };
            స్థిరరాశి గణన xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &గమ్యం_Act);
            ఒకవేళ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_విలువ,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        కీలకం_Info_పాఠ్యం(info, keyi), ndx + 1, level + 1);
                ఒకవేళ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    తిరిగి false;
                } లేకపోతే {
                    /* Ensure action type is reset */
                    గమ్యం_Act.type = ACTION_TYPE_NONE;
                }
            }
            ఒకవేళ (గమ్యం_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } లేకపోతే ఒకవేళ (likely(అంకె_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->అంకె_actions = 1;
                leveli->a.action = గమ్యం_Act;
                వెళ్ళు తదుపరి_level;
            } లేకపోతే {
                darray_append(actions, గమ్యం_Act);
            }
        }
        ఒకవేళ (darray_ఖాళీలు(actions)) {
            leveli->అంకె_actions = 0;
        } లేకపోతే ఒకవేళ (likely(darray_కొలత(actions) > 1)) {
            leveli->అంకె_actions = (xkb_action_లెక్క_t) darray_కొలత(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ఖాళీ);
#నిర్వచితం_కాకపోతే NDEBUG
            /* Canonical list: all NoAction() were dropped */
            కొరకు (xkb_action_లెక్క_t k = 0; k < leveli->అంకె_actions; k++)
                ప్రతిజ్ఞా(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#అయితే_ముగింపు
        } లేకపోతే {
            /* Unlikely: some actions were dropped and only one remains */
            ప్రతిజ్ఞా(అంకె_actions > darray_కొలత(actions));
            leveli->అంకె_actions = 1;
            leveli->a.action = darray_అంశం(actions, 0);
            ప్రతిజ్ఞా(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
తదుపరి_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ఒకవేళ (leveli->అంకె_actions > 0 || leveli->అంకె_syms > 0)
            non_ఖాళీలు_స్థాయిలు = level + 1;
    }

    ఒకవేళ (non_ఖాళీలు_స్థాయిలు < n_స్థాయిలు) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_కొలత(groupi->స్థాయిలు) = non_ఖాళీలు_స్థాయిలు;
        ఒకవేళ (non_ఖాళీలు_స్థాయిలు > 0)
            darray_shrink(groupi->స్థాయిలు);
        లేకపోతే
            darray_free(groupi->స్థాయిలు);
    }

    తిరిగి true;
}

స్థిర స్థిరరాశి Lookup_నమోదు repeatEntries[] = {
    { "true", కీలకం_REPEAT_YES },
    { "yes", కీలకం_REPEAT_YES },
    { "on", కీలకం_REPEAT_YES },
    { "false", కీలకం_REPEAT_NO },
    { "no", కీలకం_REPEAT_NO },
    { "off", కీలకం_REPEAT_NO },
    { "default", కీలకం_REPEAT_UNDEFINED },
    { ఖాళీ, 0 }
};

/** Handle overlay<N> = <key> entry */
స్థిర bool
సమాసం_Resolve_Overlay_నమోదు(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                        స్థిరరాశి అక్షరం *క్షేత్రం_2, స్థిరరాశి సమాసం_Def *arrayNdx,
                        స్థిరరాశి సమాసం_Def *సమాసం, కీలకం_Info *keyi,
                        xkb_overlay_సూచీ_t *overlay_rtrn,
                        స్థిరరాశి నిర్మాణం xkb_కీలకం **కీలకం_rtrn)
{
    /* Overlay index */
    ఒకవేళ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                క్షేత్రం_2, కీలకం_Name_పాఠ్యం(keymap_info->keymap.ctx, keyi->name));
        తిరిగి false;
    }
    static_assert(పరిమాణం(xkb_overlay_సూచీ_t) == పరిమాణం(uint8_t), "");
    స్థిరరాశి కొలత_t ఉపసర్గ = పరిమాణం("overlay") - 1;
    స్థిరరాశి కొలత_t len = అక్షరశ్రేణి_పొడవు(క్షేత్రం_2 + ఉపసర్గ);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ఒకవేళ (parse_dec_గమ్యం_uint64_t(క్షేత్రం_2 + ఉపసర్గ, len, (uint64_t *)&raw_overlay)
        != (పూర్ణాంకం)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_సూచీ,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                క్షేత్రం_2, కీలకం_Name_పాఠ్యం(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        తిరిగి false;
    }
    *overlay_rtrn = (xkb_overlay_సూచీ_t)raw_overlay - 1;

    /* Overlay key */
    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_KEYNAME_LITERAL:
        *కీలకం_rtrn = Xkb_కీలకం_By_Name(&keymap_info->keymap, సమాసం->కీలకం_name.కీలకం_name,
                                 false);
        ఒకవేళ (!*కీలకం_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_పాఠ్యం(keymap_info->keymap.ctx, సమాసం->కీలకం_name.కీలకం_name),
                    క్షేత్రం_2, కీలకం_Name_పాఠ్యం(keymap_info->keymap.ctx, keyi->name));
            తిరిగి false;
        }
        తిరిగి true;
    స్థితి STMT_సమాసం_IDENT: {
        స్థిరరాశి అక్షరం * స్థిరరాశి id =
            xkb_atom_పాఠ్యం(keymap_info->keymap.ctx, సమాసం->ident.ident);
        ఒకవేళ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *కీలకం_rtrn = ఖాళీ;
            తిరిగి true;
        } లేకపోతే ఒకవేళ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *కీలకం_rtrn = ఖాళీ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            తిరిగి true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_విలువ,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, క్షేత్రం_2, కీలకం_Name_పాఠ్యం(keymap_info->keymap.ctx, keyi->name));
        తిరిగి false;
    }
    అప్రమేయ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_విలువ,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_గమ్యం_string(STMT_సమాసం_KEYNAME_LITERAL),
                క్షేత్రం_2, కీలకం_Name_పాఠ్యం(keymap_info->keymap.ctx, keyi->name),
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;
    }
}

స్థిర bool
సమితి_Symbols_క్షేత్రం(SymbolsInfo *info, కీలకం_Info *keyi, స్థిరరాశి అక్షరం *క్షేత్రం_2,
                సమాసం_Def *arrayNdx, సమాసం_Def **విలువ_ptr)
{
    సమాసం_Def * పరిమిత స్థిరరాశి విలువ = *విలువ_ptr;
    ఒకవేళ (istreq(క్షేత్రం_2, "type")) {
        xkb_layout_సూచీ_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ఒకవేళ (!సమాసం_Resolve_String(info->ctx, విలువ, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            తిరిగి false;
        }

        ఒకవేళ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= కీలకం_క్షేత్రం_DEFAULT_TYPE;
        }
        లేకపోతే ఒకవేళ (సమాసం_Resolve_సమూహం(info->keymap_info, arrayNdx, false,
                                  &ndx, ఖాళీ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }
        లేకపోతే {
            ndx--;
            ఒకవేళ (ndx >= darray_కొలత(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ప్రతిజ్ఞా(darray_కొలత(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_అంశం(keyi->groups, ndx).type = val;
            darray_అంశం(keyi->groups, ndx).defined |= సమూహం_క్షేత్రం_TYPE;
        }
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "symbols")) {
        తిరిగి కూడు_Symbols_గమ్యం_కీలకం(info, keyi, arrayNdx, విలువ);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "actions")) {
        తిరిగి కూడు_Actions_గమ్యం_కీలకం(info, keyi, arrayNdx, విలువ);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "vmods") ||
             istreq(క్షేత్రం_2, "virtualmods") ||
             istreq(క్షేత్రం_2, "virtualmodifiers")) {
        xkb_mod_ముసుగు_t ముసుగు = 0;

        ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, విలువ, MOD_VIRT, &info->mods,
                                &ముసుగు)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_గమ్యం_string(విలువ->common.type),
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }

        keyi->vmodmap = ముసుగు;
        keyi->defined |= కీలకం_క్షేత్రం_VMODMAP;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "locking") ||
             istreq(క్షేత్రం_2, "lock") ||
             istreq(క్షేత్రం_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                కీలకం_Info_పాఠ్యం(info, keyi));
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "radiogroup") ||
             istreq(క్షేత్రం_2, "permanentradiogroup") ||
             istreq(క్షేత్రం_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                కీలకం_Info_పాఠ్యం(info, keyi));
    }
    లేకపోతే ఒకవేళ (istreq_ఉపసర్గ("permanentoverlay", క్షేత్రం_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                కీలకం_Info_పాఠ్యం(info, keyi));
    }
    లేకపోతే ఒకవేళ (istreq_ఉపసర్గ("overlay", క్షేత్రం_2)) {
        xkb_overlay_సూచీ_t overlay = XKB_OVERLAY_INVALID;
        స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = ఖాళీ;
        ఒకవేళ (!సమాసం_Resolve_Overlay_నమోదు(info->keymap_info, క్షేత్రం_2, arrayNdx,
                                     *విలువ_ptr, keyi, &overlay, &కీలకం))
            తిరిగి false;
        ఒకవేళ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            తిరిగి true;
        } లేకపోతే ఒకవేళ (కీలకం && కీలకం->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_సందేశం_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, కీలకం_Info_పాఠ్యం(info, keyi));
        } లేకపోతే {
            స్థిరరాశి నిర్మాణం xkb_కీలకం *prev;
            ఒకవేళ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ఒకవేళ (కీలకం != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             కీలకం_Info_పాఠ్యం(info, keyi), overlay + 1,
                             (prev
                                 ? కీలకం_Name_పాఠ్యం(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (కీలకం
                                 ? కీలకం_Name_పాఠ్యం(info->ctx, కీలకం->name)
                                 : "none"));
                }
            } లేకపోతే ఒకవేళ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ఒకవేళ (!overlays_చొప్పించు(keyi, overlay, కీలకం))
                    తిరిగి false;
                keyi->defined |= కీలకం_క్షేత్రం_OVERLAY;
            } లేకపోతే {
                /* Handle disjoint overlays mode */
                స్థిరరాశి xkb_overlay_ముసుగు_t ముసుగు = (xkb_overlay_ముసుగు_t)(1u << overlay);
                ఒకవేళ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ఒకవేళ (కీలకం) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = ముసుగు;
                        keyi->overlays_clear = false;
                        keyi->overlay_కీలకం = కీలకం;
                    } లేకపోతే {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= ముసుగు;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= కీలకం_క్షేత్రం_OVERLAY;
                } లేకపోతే ఒకవేళ (keyi->overlays) {
                    ఒకవేళ (కీలకం) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                కీలకం_Info_పాఠ్యం(info, keyi), keyi->overlays,
                                కీలకం_Name_పాఠ్యం(info->ctx, keyi->overlay_కీలకం->name),
                                overlay + 1, కీలకం_Name_పాఠ్యం(info->ctx, కీలకం->name));
                        తిరిగి !(info->keymap_info->strict &
                                 PARSER_NO_క్షేత్రం_విలువ_MISMATCH);
                    } లేకపోతే {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "repeating") ||
             istreq(క్షేత్రం_2, "repeats") ||
             istreq(క్షేత్రం_2, "repeat")) {
        uint32_t val = 0;

        ఒకవేళ (!సమాసం_Resolve_Enum(info->ctx, విలువ, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_విలువ,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }

        keyi->repeat = (గణన కీలకం_repeat) val;
        keyi->defined |= కీలకం_క్షేత్రం_REPEAT;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "groupswrap") ||
             istreq(క్షేత్రం_2, "wrapgroups")) {
        bool సమితి = false;

        ఒకవేళ (!సమాసం_Resolve_Boolean(info->ctx, విలువ, &సమితి)) {
            log_err(info->ctx, XKB_ERROR_INVALID_విలువ,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }

        keyi->బహిర్గతం_యొక్క_range_సమూహం_policy = (సమితి)
            ? XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP
            : XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP;
        keyi->defined |= కీలకం_క్షేత్రం_GROUPINFO;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "groupsclamp") ||
             istreq(క్షేత్రం_2, "clampgroups")) {
        bool సమితి = false;

        ఒకవేళ (!సమాసం_Resolve_Boolean(info->ctx, విలువ, &సమితి)) {
            log_err(info->ctx, XKB_ERROR_INVALID_విలువ,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }

        keyi->బహిర్గతం_యొక్క_range_సమూహం_policy = (సమితి)
            ? XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP
            : XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP;
        keyi->defined |= కీలకం_క్షేత్రం_GROUPINFO;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "groupsredirect") ||
             istreq(క్షేత్రం_2, "redirectgroups")) {
        xkb_layout_సూచీ_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ఒకవేళ (సమాసం_Resolve_సమూహం(info->keymap_info, విలువ, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    కీలకం_Info_పాఠ్యం(info, keyi));
            తిరిగి false;
        }

        ఒకవేళ (pending) {
            keyi->బహిర్గతం_యొక్క_range_pending_సమూహం = true;
            స్థిరరాశి darray_కొలత_t pending_సూచీ =
                darray_కొలత(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (నిర్మాణం pending_computation) {
                    .సమాసం = *విలువ_ptr,
                    .computed = false,
                    .విలువ = 0,
                }
            );
            *విలువ_ptr = ఖాళీ;
            static_assert(పరిమాణం(keyi->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య) ==
                          పరిమాణం(pending_సూచీ),
                          "Cannot save pending computation");
            keyi->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = pending_సూచీ;
        } లేకపోతే {
            keyi->బహిర్గతం_యొక్క_range_pending_సమూహం = false;
            keyi->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = grp - 1;
        }

        keyi->బహిర్గతం_యొక్క_range_సమూహం_policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT;
        keyi->defined |= కీలకం_క్షేత్రం_GROUPINFO;
    }
    లేకపోతే {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_క్షేత్రం,
                "Unknown field \"%s\" in a key; definition ignored\n",
                క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict & PARSER_NO_UNKNOWN_కీలకం_FIELDS);
    }

    తిరిగి true;
}

స్థిర bool
సమితి_సమూహం_Name(SymbolsInfo *info, సమాసం_Def *arrayNdx, సమాసం_Def *విలువ,
             గణన విలీనం_బహుళకం విలీనం)
{
    ఒకవేళ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_సమూహం_NAME_సూచీ,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        తిరిగి false;
    }

    xkb_layout_సూచీ_t సమూహం = 0;
    ఒకవేళ (సమాసం_Resolve_సమూహం(info->keymap_info, arrayNdx, false, &సమూహం, ఖాళీ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        తిరిగి false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ఒకవేళ (!సమాసం_Resolve_String(info->ctx, విలువ, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", సమూహం);
        తిరిగి false;
    }

    xkb_layout_సూచీ_t సమూహం_గమ్యం_use;
    ఒకవేళ (info->explicit_సమూహం == XKB_LAYOUT_INVALID) {
        సమూహం_గమ్యం_use = సమూహం - 1;
    }
    లేకపోతే ఒకవేళ (సమూహం - 1 == 0) {
        సమూహం_గమ్యం_use = info->explicit_సమూహం;
    }
    లేకపోతే {
        log_warn(info->ctx, XKB_WARNING_NON_పునాది_సమూహం_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, సమూహం,
                 xkb_atom_పాఠ్యం(info->ctx, name));
        తిరిగి false;
    }

    ఒకవేళ (సమూహం_గమ్యం_use >= darray_కొలత(info->సమూహం_పేర్లు)) {
        /* Avoid clang-tidy false positive */
        ప్రతిజ్ఞా(darray_కొలత(info->సమూహం_పేర్లు) < సమూహం_గమ్యం_use + 1);
        darray_resize0(info->సమూహం_పేర్లు, సమూహం_గమ్యం_use + 1);
    } లేకపోతే {
        స్థిరరాశి xkb_atom_t old_name = darray_అంశం(info->సమూహం_పేర్లు, సమూహం_గమ్యం_use);
        ఒకవేళ (old_name != XKB_ATOM_NONE && old_name != name) {
            స్థిరరాశి bool replace = (విలీనం != విలీనం_AUGMENT);
            స్థిరరాశి xkb_atom_t use    = (replace ? name : old_name);
            స్థిరరాశి xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     సమూహం_గమ్యం_use, info->name,
                     xkb_atom_పాఠ్యం(info->ctx, use),
                     xkb_atom_పాఠ్యం(info->ctx, ignore));
            name = use;
        }
    }
    darray_అంశం(info->సమూహం_పేర్లు, సమూహం_గమ్యం_use) = name;

    తిరిగి true;
}

స్థిర bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *arrayNdx;
    bool ret;

    ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, stmt->name, &elem, &క్షేత్రం_2, &arrayNdx))
        తిరిగి false;

    ఒకవేళ (elem && istreq(elem, "key")) {
        కీలకం_Info తాత్కాలిక = {0};
        Init_కీలకం_Info(info->ctx, &తాత్కాలిక);
        /* Do not replace the whole key, only the current field */
        తాత్కాలిక.విలీనం = (తాత్కాలిక.విలీనం == విలీనం_REPLACE)
            ? విలీనం_OVERRIDE
            : stmt->విలీనం;
        ret = సమితి_Symbols_క్షేత్రం(info, &తాత్కాలిక, క్షేత్రం_2, arrayNdx, &stmt->విలువ) &&
              విలీనం_కీలకాలు(info, &info->default_కీలకం, &తాత్కాలిక, true);
        Clear_కీలకం_Info(&తాత్కాలిక);
    }
    లేకపోతే ఒకవేళ (!elem && (istreq(క్షేత్రం_2, "name") ||
                       istreq(క్షేత్రం_2, "groupname"))) {
        ret = సమితి_సమూహం_Name(info, arrayNdx, stmt->విలువ, stmt->విలీనం);
    }
    లేకపోతే ఒకవేళ (!elem && (istreq(క్షేత్రం_2, "groupswrap") ||
                       istreq(క్షేత్రం_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    లేకపోతే ఒకవేళ (!elem && (istreq(క్షేత్రం_2, "groupsclamp") ||
                       istreq(క్షేత్రం_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    లేకపోతే ఒకవేళ (!elem && (istreq(క్షేత్రం_2, "groupsredirect") ||
                       istreq(క్షేత్రం_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    లేకపోతే ఒకవేళ (!elem && istreq(క్షేత్రం_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_క్షేత్రం,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    లేకపోతే ఒకవేళ (elem) {
        ret = (సమితి_Default_Action_క్షేత్రం(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, క్షేత్రం_2, arrayNdx,
                                     &stmt->విలువ, stmt->విలీనం) !=
               PARSER_FATAL_ERROR);
    } లేకపోతే {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_క్షేత్రం,
                "Default defined for unknown field \"%s\"; Ignored\n", క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    తిరిగి ret;
}

స్థిర bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, కీలకం_Info *keyi)
{
    bool all_valid_entries = true;

    కొరకు (; def; def = (VarDef *) def->common.తదుపరి) {
        స్థిరరాశి అక్షరం *క్షేత్రం_2;
        సమాసం_Def *arrayNdx;
        bool ok = true;

        ఒకవేళ (!def->name) {
            ఒకవేళ (unlikely(!def->విలువ) || def->విలువ->common.type != STMT_సమాసం_ACTION_LIST)
                క్షేత్రం_2 = "symbols"; /* Default to symbols field */
            లేకపోతే
                క్షేత్రం_2 = "actions";
            arrayNdx = ఖాళీ;
        }
        లేకపోతే {
            స్థిరరాశి అక్షరం *elem;
            ok = సమాసం_Resolve_Lhs(info->ctx, def->name, &elem, &క్షేత్రం_2,
                                &arrayNdx);
            ఒకవేళ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, క్షేత్రం_2);
                ok = false;
            }
        }

        ఒకవేళ (unlikely(!def->విలువ)) {
            log_err(info->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", క్షేత్రం_2);
            ok = false;
        }

        ఒకవేళ (!ok || !సమితి_Symbols_క్షేత్రం(info, keyi, క్షేత్రం_2, arrayNdx, &def->విలువ))
            all_valid_entries = false;
    }

    తిరిగి all_valid_entries;
}

స్థిర bool
సమితి_Explicit_సమూహం(SymbolsInfo *info, కీలకం_Info *keyi)
{
    xkb_layout_సూచీ_t i;
    సమూహం_Info_2 *groupi;
    bool warn = false;

    ఒకవేళ (info->explicit_సమూహం == XKB_LAYOUT_INVALID)
        తిరిగి true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ఒకవేళ (groupi->defined) {
            warn = true;
            Clear_సమూహం_Info(groupi);
            Init_సమూహం_Info(groupi);
        }
    }

    ఒకవేళ (warn) {
        log_warn(info->ctx, XKB_WARNING_గుణిజం_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_సమూహం + 1, కీలకం_Info_పాఠ్యం(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_సమూహం + 1);
    ఒకవేళ (info->explicit_సమూహం > 0) {
        darray_అంశం(keyi->groups, info->explicit_సమూహం) =
            darray_అంశం(keyi->groups, 0);
        Init_సమూహం_Info(&darray_అంశం(keyi->groups, 0));
    }

    తిరిగి true;
}

స్థిర bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    కీలకం_Info keyi;

    keyi = info->default_కీలకం;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_కీలకం.groups);
    కొరకు (xkb_layout_సూచీ_t i = 0; i < darray_కొలత(keyi.groups); i++)
        Copy_సమూహం_Info(&darray_అంశం(keyi.groups, i),
                      &darray_అంశం(info->default_కీలకం.groups, i));
    keyi.విలీనం = stmt->విలీనం;
    keyi.name = stmt->కీలకం_Name;

    ఒకవేళ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        సమితి_Explicit_సమూహం(info, &keyi) &&
        కూడు_కీలకం_Symbols(info, &keyi, true)) {
        తిరిగి true;
    }

    Clear_కీలకం_Info(&keyi);
    info->error_లెక్క++;
    తిరిగి false;
}

స్థిర bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_నమోదు tmp;
    నిర్మాణం xkb_context *ctx = info->ctx;

    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        తిరిగి false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ప్రతిజ్ఞా(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_పాఠ్యం_V1 ||
           def->modifiers->common.type == STMT_సమాసం_IDENT);

    bool ok = true;
    tmp.విలీనం = def->విలీనం;

    కొరకు (సమాసం_Def *కీలకం = def->కీలకాలు; కీలకం; కీలకం = (సమాసం_Def *) కీలకం->common.తదుపరి) {
        ఒకవేళ (కీలకం->common.type == STMT_సమాసం_KEYNAME_LITERAL) {
            tmp.have_చిహ్నం = false;
            tmp.u.కీలకం_Name = కీలకం->కీలకం_name.కీలకం_name;
        }
        లేకపోతే ఒకవేళ (కీలకం->common.type == STMT_సమాసం_KEYSYM_LITERAL) {
            ఒకవేళ (కీలకం->keysym.keysym == XKB_కీలకం_No_చిహ్నం) {
                /* Invalid keysym: ignore. Error message already printed */
                కొనసాగు;
            } లేకపోతే {
                tmp.have_చిహ్నం = true;
                tmp.u.కీలకం_Sym = కీలకం->keysym.keysym;
            }
        }
        లేకపోతే {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_నమోదు,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            కొనసాగు;
        }

        ok = కూడు_Mod_Map_నమోదు(info, &tmp) && ok;
    }
    తిరిగి ok;
}

స్థిర శూన్య
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    విడుదల(info->name);
    info->name = strdup_సురక్షిత(file->name);

    కొరకు (ParseCommon *stmt = file->defs; stmt; stmt = stmt->తదుపరి) {
        ఎంపిక (stmt->type) {
        స్థితి STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ఆపు;
        స్థితి STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ఆపు;
        స్థితి STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ఆపు;
        స్థితి STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ఆపు;
        స్థితి STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ఆపు;
        స్థితి STMT_UNKNOWN_DECLARATION:
        స్థితి STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ఆపు;
        అప్రమేయ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_గమ్యం_string(stmt->type));
            ok = false;
            ఆపు;
        }

        ఒకవేళ (!ok)
            info->error_లెక్క++;

        ఒకవేళ (info->error_లెక్క > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    సురక్షిత_map_name(file->name));
            ఆపు;
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
స్థిర నిర్మాణం xkb_కీలకం *
కనుగొను_కీలకం_For_చిహ్నం(నిర్మాణం xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_సమూహం;
    xkb_layout_సూచీ_t సమూహం = 0;
    చేయి {
        xkb_level_సూచీ_t level = 0;
        got_one_సమూహం = false;
        bool got_one_level;
        చేయి {
            got_one_level = false;
            నిర్మాణం xkb_కీలకం *కీలకం;
            xkb_కీలకాలు_foreach(కీలకం, keymap) {
                ఒకవేళ (సమూహం < కీలకం->అంకె_groups &&
                    level < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, సమూహం)) {
                    got_one_సమూహం = got_one_level = true;
                    స్థిరరాశి xkb_keysym_లెక్క_t అంకె_syms =
                        కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_syms;
                    ఒకవేళ (అంకె_syms > 1) {
                        కొరకు (xkb_keysym_లెక్క_t k = 0; k < అంకె_syms; k++) {
                            ఒకవేళ (కీలకం->groups[సమూహం].స్థాయిలు[level].s.syms[k] == sym)
                                తిరిగి కీలకం;
                        }
                    } లేకపోతే ఒకవేళ (అంకె_syms &&
                               కీలకం->groups[సమూహం].స్థాయిలు[level].s.sym == sym) {
                        తిరిగి కీలకం;
                    }
                }
            }
            level++;
        } అయితే (got_one_level);
        సమూహం++;
    } అయితే (got_one_సమూహం);

    తిరిగి ఖాళీ;
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
స్థిర xkb_atom_t
కనుగొను_Automatic_Type(నిర్మాణం xkb_context *ctx, సమూహం_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    స్థిరరాశి xkb_level_సూచీ_t వెడల్పు = darray_కొలత(groupi->స్థాయిలు);

#నిర్వచించు GET_SYM(level) \
    (darray_అంశం(groupi->స్థాయిలు, level).అంకె_syms == 0 ? \
        XKB_కీలకం_No_చిహ్నం : \
     darray_అంశం(groupi->స్థాయిలు, level).అంకె_syms == 1 ? \
        darray_అంశం(groupi->స్థాయిలు, level).s.sym : \
     /* num_syms > 1 */ \
        darray_అంశం(groupi->స్థాయిలు, level).s.syms[0])

    ఒకవేళ (వెడల్పు == 1 || వెడల్పు <= 0)
        తిరిగి xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ఒకవేళ (వెడల్పు == 2) {
        ఒకవేళ (xkb_keysym_is_క్రింది(sym0) && xkb_keysym_is_పై_or_title(sym1))
            తిరిగి xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ఒకవేళ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            తిరిగి xkb_atom_intern_literal(ctx, "KEYPAD");

        తిరిగి xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ఒకవేళ (వెడల్పు <= 4) {
        ఒకవేళ (xkb_keysym_is_క్రింది(sym0) && xkb_keysym_is_పై_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (వెడల్పు == 4 ? GET_SYM(3) : XKB_కీలకం_No_చిహ్నం);

            ఒకవేళ (xkb_keysym_is_క్రింది(sym2) && xkb_keysym_is_పై_or_title(sym3))
                తిరిగి xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            తిరిగి xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ఒకవేళ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            తిరిగి xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        తిరిగి xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    తిరిగి XKB_ATOM_NONE;

#నిర్వచనం_రద్దు GET_SYM
}

స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం_type *
కనుగొను_Type_For_సమూహం(నిర్మాణం xkb_keymap *keymap, కీలకం_Info *keyi,
                 xkb_layout_సూచీ_t సమూహం, bool *explicit_type)
{
    సమూహం_Info_2 *groupi = &darray_అంశం(keyi->groups, సమూహం);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ఒకవేళ (type_name == XKB_ATOM_NONE) {
        ఒకవేళ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        లేకపోతే {
            type_name = కనుగొను_Automatic_Type(keymap->ctx, groupi);
            ఒకవేళ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ఒకవేళ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_కీలకం_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 కీలకం_Name_పాఠ్యం(keymap->ctx, keyi->name), సమూహం + 1,
                 darray_కొలత(groupi->స్థాయిలు));
        వెళ్ళు use_default;
    }

    darray_కొలత_t i;
    కొరకు (i = 0; i < keymap->అంకె_types; i++)
        ఒకవేళ (keymap->types[i].name == type_name)
            ఆపు;

    ఒకవేళ (i >= keymap->అంకె_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_కీలకం_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_పాఠ్యం(keymap->ctx, type_name),
                 కీలకం_Name_పాఠ్యం(keymap->ctx, keyi->name), సమూహం + 1);
        వెళ్ళు use_default;
    }

    keymap->types[i].required = true;
    తిరిగి &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    తిరిగి &keymap->types[0];
}

స్థిర bool
Copy_Symbols_Def_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, SymbolsInfo *info,
                       కీలకం_Info *keyi)
{
    నిర్మాణం xkb_కీలకం *కీలకం;
    సమూహం_Info_2 *groupi;
    స్థిరరాశి సమూహం_Info_2 *group0;
    xkb_layout_సూచీ_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    కీలకం = Xkb_కీలకం_By_Name(keymap, keyi->name, false);
    ఒకవేళ (!కీలకం) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                కీలకం_Info_పాఠ్యం(info, keyi));
        తిరిగి false;
    }

    /* Find the range of groups we need. */
    కీలకం->అంకె_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        స్థిరరాశి bool has_explicit_type = (keyi->defined & కీలకం_క్షేత్రం_DEFAULT_TYPE)
                                    || (groupi->defined & సమూహం_క్షేత్రం_TYPE);
        ఒకవేళ (darray_కొలత(groupi->స్థాయిలు) > 0 || has_explicit_type)
            కీలకం->అంకె_groups = i + 1;
        ఒకవేళ (has_explicit_type)
            కీలకం->explicit |= EXPLICIT_TYPES;
    }

    ఒకవేళ (కీలకం->అంకె_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ఒకవేళ (keyi->defined)
            వెళ్ళు కీలకం_fields;
        లేకపోతే
            తిరిగి false;
    }

    darray_resize(keyi->groups, కీలకం->అంకె_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_అంశం(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ఒకవేళ (groupi->defined)
            కొనసాగు;

        Copy_సమూహం_Info(groupi, group0);
    }

    కీలకం->groups = సున్నా_జ్ఞాపకం_ఇవ్వు(కీలకం->అంకె_groups, పరిమాణం(*కీలకం->groups));
    ప్రతిజ్ఞా(darray_కొలత(keyi->groups) == కీలకం->అంకె_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #నిర్వచితమైతే __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < కీలకం->అంకె_groups);
        #అయితే_ముగింపు
        bool explicit_type = false;
        స్థిరరాశి నిర్మాణం xkb_కీలకం_type * స్థిరరాశి type =
            కనుగొను_Type_For_సమూహం(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ఒకవేళ (type->అంకె_స్థాయిలు < darray_కొలత(groupi->స్థాయిలు)) {
            నిర్మాణం xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_పాఠ్యం(keymap->ctx, type->name), type->అంకె_స్థాయిలు,
                    కీలకం_Info_పాఠ్యం(info, keyi),
                    darray_కొలత(groupi->స్థాయిలు));

            darray_foreach_from(leveli, groupi->స్థాయిలు, type->అంకె_స్థాయిలు)
                clear_level(leveli);
        }
        darray_resize0(groupi->స్థాయిలు, type->అంకె_స్థాయిలు);

        కీలకం->groups[i].explicit_type = explicit_type;
        కీలకం->groups[i].type = type;
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
        నిర్మాణం xkb_level *leveli;
        darray_foreach(leveli, groupi->స్థాయిలు) {
            ఎంపిక (leveli->అంకె_syms) {
            స్థితి 0:
                leveli->పై = XKB_కీలకం_No_చిహ్నం;
                ఆపు;
            స్థితి 1:
                leveli->పై = xkb_keysym_గమ్యం_పై(leveli->s.sym);
                ఆపు;
            అప్రమేయ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_పై = false;
                కొరకు (xkb_keysym_లెక్క_t k = 0; k < leveli->అంకె_syms; k++) {
                    స్థిరరాశి xkb_keysym_t పై =
                        xkb_keysym_గమ్యం_పై(leveli->s.syms[k]);
                    ఒకవేళ (పై != leveli->s.syms[k]) {
                        leveli->has_పై = true;
                        ఆపు;
                    }
                }
                ఒకవేళ (leveli->has_పై) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (కొలత_t) 2 * leveli->అంకె_syms *
                                             పరిమాణం(*leveli->s.syms));
                    ఒకవేళ (!leveli->s.syms)
                        తిరిగి false; /* FIXME: better handling? */
                    కొరకు (xkb_keysym_లెక్క_t k = 0; k < leveli->అంకె_syms; k++) {
                        leveli->s.syms[leveli->అంకె_syms + k] =
                            xkb_keysym_గమ్యం_పై(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->స్థాయిలు, &కీలకం->groups[i].స్థాయిలు, ఖాళీ);
        ఒకవేళ (కీలకం->groups[i].type->అంకె_స్థాయిలు > 1 ||
            కీలకం->groups[i].స్థాయిలు[0].అంకె_syms > 0) {
            కీలకం->groups[i].explicit_symbols = true;
            కీలకం->explicit |= EXPLICIT_SYMBOLS;
        }
        ఒకవేళ (groupi->defined & సమూహం_క్షేత్రం_ACTS) { // FIXME
            కీలకం->groups[i].explicit_actions = true;
            కీలకం->explicit |= EXPLICIT_INTERP;
        }
        ఒకవేళ (కీలకం->groups[i].explicit_type)
            కీలకం->explicit |= EXPLICIT_TYPES;
    }

    కీలకం->బహిర్గతం_యొక్క_range_pending_సమూహం = keyi->బహిర్గతం_యొక్క_range_pending_సమూహం;
    కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = keyi->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య;
    కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy = keyi->బహిర్గతం_యొక్క_range_సమూహం_policy;

కీలకం_fields:
    ఒకవేళ (keyi->defined & కీలకం_క్షేత్రం_VMODMAP) {
        కీలకం->vmodmap = keyi->vmodmap;
        కీలకం->explicit |= EXPLICIT_VMODMAP;
    }

    ఒకవేళ (keyi->repeat != కీలకం_REPEAT_UNDEFINED) {
        కీలకం->repeats = (keyi->repeat == కీలకం_REPEAT_YES);
        కీలకం->explicit |= EXPLICIT_REPEAT;
    }

    ఒకవేళ ((keyi->defined & కీలకం_క్షేత్రం_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        కీలకం->overlays = keyi->overlays;
        ఒకవేళ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_ముసుగు_t మిగిలిన = కీలకం->overlays;
            స్థిరరాశి నిర్మాణం xkb_కీలకం **overlays_కీలకాలు = keyi->overlays_కీలకాలు;
            అయితే (మిగిలిన) {
                /* isolate lowest set bit */
                స్థిరరాశి xkb_overlay_ముసుగు_t lsb = (xkb_overlay_ముసుగు_t)(
                    మిగిలిన &
                    (xkb_overlay_ముసుగు_t)((గుర్తులేని)~మిగిలిన + 1u)
                );
                మిగిలిన = (మిగిలిన & ~lsb);
                ఒకవేళ (*overlays_కీలకాలు) {
                    overlays_కీలకాలు++;
                } లేకపోతే {
                    /* drop current null value (overlayN=none) */
                    కీలకం->overlays &= ~lsb;
                }
            }

            ఒకవేళ (కీలకం->overlays) {
                /* Steal */
                కీలకం->overlays_కీలకాలు = keyi->overlays_కీలకాలు;
                keyi->overlays_కీలకాలు = ఖాళీ;
                keyi->overlays_alloc = 0;

                కీలకం->overlays_inline = false;
                కీలకం->explicit |= EXPLICIT_OVERLAY;
            }
        } లేకపోతే {
            కీలకం->overlay_కీలకం = keyi->overlay_కీలకం;
            కీలకం->overlays_inline = true;
            కీలకం->explicit |= EXPLICIT_OVERLAY;
        }
    }

    తిరిగి true;
}

స్థిర bool
Copy_Mod_Map_Def_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_నమోదు *నమోదు_2)
{
    నిర్మాణం xkb_కీలకం *కీలకం;

    ఒకవేళ (!నమోదు_2->have_చిహ్నం) {
        కీలకం = Xkb_కీలకం_By_Name(keymap, నమోదు_2->u.కీలకం_Name, true);
        ఒకవేళ (!కీలకం) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    కీలకం_Name_పాఠ్యం(info->ctx, నమోదు_2->u.కీలకం_Name),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, నమోదు_2->mods));
            నమోదు_2->mods = 0; /* disable */
            తిరిగి false;
        }
    }
    లేకపోతే {
        కీలకం = కనుగొను_కీలకం_For_చిహ్నం(keymap, నమోదు_2->u.కీలకం_Sym);
        ఒకవేళ (!కీలకం) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_చిహ్నం,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_పాఠ్యం(info->ctx, నమోదు_2->u.కీలకం_Sym),
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_REAL, &info->mods, నమోదు_2->mods));
            నమోదు_2->mods = 0; /* disable */
            తిరిగి false;
        }
    }

    /* Skip modMap None */
    ఒకవేళ (నమోదు_2->mods) {
        కీలకం->modmap |= నమోదు_2->mods;
        నమోదు_2->కీలకం_సంకేతం = కీలకం->keycode;
    }

    తిరిగి true;
}

స్థిర bool
Copy_Symbols_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, SymbolsInfo *info)
{
    కీలకం_Info *keyi;

    keymap->symbols_section_name = strdup_సురక్షిత(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->అంకె_సమూహం_పేర్లు = (xkb_layout_సూచీ_t)darray_కొలత(info->సమూహం_పేర్లు);
    darray_steal(info->సమూహం_పేర్లు, &keymap->సమూహం_పేర్లు, ఖాళీ);

    darray_foreach(keyi, info->కీలకాలు)
        ఒకవేళ (!Copy_Symbols_Def_గమ్యం_Keymap(keymap, info, keyi))
            info->error_లెక్క++;

    ఒకవేళ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        నిర్మాణం xkb_కీలకం *కీలకం;

        xkb_కీలకాలు_foreach(కీలకం, keymap) {
            ఒకవేళ (కీలకం->name == XKB_ATOM_NONE)
                కొనసాగు;

            ఒకవేళ (కీలకం->అంకె_groups < 1)
                log_info(info->ctx, XKB_LOG_సందేశం_NO_ID,
                         "No symbols defined for %s\n",
                         కీలకం_Name_పాఠ్యం(info->ctx, కీలకం->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_నమోదు *mm;
    darray_foreach(mm, info->modmaps)
        ఒకవేళ (!Copy_Mod_Map_Def_గమ్యం_Keymap(keymap, info, mm))
            info->error_లెక్క++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->అంకె_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    తిరిగి true;
}

bool
CompileSymbols(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ఒకవేళ (file !=ఖాళీ)
        HandleSymbolsFile(&info, file);

    ఒకవేళ (info.error_లెక్క != 0)
        వెళ్ళు err_info;

    ఒకవేళ (!Copy_Symbols_గమ్యం_Keymap(&keymap_info->keymap, &info))
        వెళ్ళు err_info;

    ClearSymbolsInfo(&info);
    తిరిగి true;

err_info:
    ClearSymbolsInfo(&info);
    తిరిగి false;
}
