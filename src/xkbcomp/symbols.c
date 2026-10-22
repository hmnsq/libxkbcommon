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

تعداد ڪنجي_repeat {
    ڪنجي_REPEAT_UNDEFINED = 0,
    ڪنجي_REPEAT_YES,
    ڪنجي_REPEAT_NO,
    ڪنجي_REPEAT_انگ_ENTRIES,
};
تعداد { ڪنجي_REPEAT_MIN_ويڪر = 3 /* 2 bits + sign */ };
static_assert(ڪنجي_REPEAT_انگ_ENTRIES - 1 <=
              (1 << (ڪنجي_REPEAT_MIN_ويڪر - 1)) - 1,
              "Cannot encode key repeat");

تعداد گروهه_ميدان {
    گروهه_ميدان_SYMS = (1 << 0),
    گروهه_ميدان_ACTS = (1 << 1),
    گروهه_ميدان_TYPE = (1 << 2),
};

تعداد ڪنجي_ميدان {
    ڪنجي_ميدان_NONE = 0,
    ڪنجي_ميدان_REPEAT = (1 << 0),
    ڪنجي_ميدان_DEFAULT_TYPE = (1 << 1),
    ڪنجي_ميدان_GROUPINFO = (1 << 2),
    ڪنجي_ميدان_VMODMAP = (1 << 3),
    ڪنجي_ميدان_OVERLAY = (1 << 4),
    ڪنجي_ميدان_ALL = ڪنجي_ميدان_REPEAT | ڪنجي_ميدان_DEFAULT_TYPE
                  | ڪنجي_ميدان_GROUPINFO | ڪنجي_ميدان_VMODMAP | ڪنجي_ميدان_OVERLAY,
};
تعداد { ڪنجي_ميدان_MIN_ويڪر = 6 /* 5 bits + sign */ };
static_assert(ڪنجي_ميدان_ALL <= (1 << (ڪنجي_ميدان_MIN_ويڪر - 1)) - 1,
              "Cannot encode key field");

تعريفنوع بنية {
    darray(بنية xkb_level) سطحون;
    تعداد گروهه_ميدان defined;
    xkb_atom_t type;
} گروهه_Info_2;

تعداد {
    XKB_LAYOUT_ٻاهريون_جو_RANGE_POLICY_MIN_ويڪر = 4 /* 3 bits + sign */,
    KEYINFO_ٻاهريون_جو_RANGE_گروهه_POLICY_ٻٽو_انگ_ميدان = 8,
    KEYINFO_DEFINED_ٻٽو_انگ_ميدان = 16,
    KEYINFO_ملايو_ٻٽو_انگ_ميدان = 8,
    KEYINFO_REPEAT_ٻٽو_انگ_ميدان = 8,
};
static_assert(
    XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP < XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP &&
    XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP < XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_ٻاهريون_جو_RANGE_POLICY_MIN_ويڪر) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_ٻاهريون_جو_RANGE_گروهه_POLICY_ٻٽو_انگ_ميدان >=
              XKB_LAYOUT_ٻاهريون_جو_RANGE_POLICY_MIN_ويڪر, "");
static_assert((بنانشان)KEYINFO_DEFINED_ٻٽو_انگ_ميدان >=
              (بنانشان)ڪنجي_ميدان_MIN_ويڪر, "Cannot encode KeyInfo field");
static_assert((بنانشان)KEYINFO_ملايو_ٻٽو_انگ_ميدان >=
              (بنانشان)ملايو_گهڻائي_MIN_ويڪر, "Cannot encode KeyInfo field");
static_assert((بنانشان)KEYINFO_REPEAT_ٻٽو_انگ_ميدان >=
              (بنانشان)ڪنجي_REPEAT_MIN_ويڪر, "Cannot encode KeyInfo field");

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_نقاب_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_اشاريو_t ٻاهريون_جو_range_گروهه_عدد;
    darray(گروهه_Info_2) groups;
    تعداد xkb_layout_ٻاهريون_جو_range_policy
        ٻاهريون_جو_range_گروهه_policy:KEYINFO_ٻاهريون_جو_RANGE_گروهه_POLICY_ٻٽو_انگ_ميدان;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد ڪنجي_ميدان defined:KEYINFO_DEFINED_ٻٽو_انگ_ميدان;
    تعداد ملايو_گهڻائي ملايو:KEYINFO_ملايو_ٻٽو_انگ_ميدان;
    تعداد ڪنجي_repeat repeat:KEYINFO_REPEAT_ٻٽو_انگ_ميدان;

    bool ٻاهريون_جو_range_pending_گروهه:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_اشاريو_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_نقاب_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ثابت بنية xkb_ڪنجي *overlay_ڪنجي;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_ڪنجي **overlays_ڪنجيون;
    };
} ڪنجي_Info;

ساكن فراغ
StealLevelInfo(بنية xkb_level *into, بنية xkb_level *from)
{
    clear_level(into);
    إذا (from->انگ_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } وإلا {
        into->s.sym = from->s.sym;
    }
    into->انگ_syms = from->انگ_syms;
    from->انگ_syms = 0;

    إذا (from->انگ_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } وإلا {
        into->a.action = from->a.action;
    }
    into->انگ_actions = from->انگ_actions;
    from->انگ_actions = 0;
}

ساكن فراغ
Init_گروهه_Info(گروهه_Info_2 *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساكن فراغ
Clear_گروهه_Info(گروهه_Info_2 *groupi)
{
    بنية xkb_level *leveli;
    darray_foreach(leveli, groupi->سطحون)
        clear_level(leveli);
    darray_free(groupi->سطحون);
}

ساكن فراغ
Copy_گروهه_Info(گروهه_Info_2 *منزل, ثابت گروهه_Info_2 *from)
{
    منزل->defined = from->defined;
    منزل->type = from->type;
    darray_init(منزل->سطحون);
    darray_copy(منزل->سطحون, from->سطحون);
    لكل (xkb_level_اشاريو_t j = 0; j < darray_سائيز(منزل->سطحون); j++) {
        إذا (darray_شي(from->سطحون, j).انگ_syms > 1) {
            darray_شي(منزل->سطحون, j).s.syms =
                memdup(darray_شي(from->سطحون, j).s.syms,
                       darray_شي(from->سطحون, j).انگ_syms,
                       حجم(xkb_keysym_t));
        }
        إذا (darray_شي(from->سطحون, j).انگ_actions > 1) {
            darray_شي(منزل->سطحون, j).a.actions =
                memdup(darray_شي(from->سطحون, j).a.actions,
                       darray_شي(from->سطحون, j).انگ_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساكن فراغ
Init_ڪنجي_Info(بنية xkb_context *ctx, ڪنجي_Info *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_ڪنجي_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ڪنجي_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->ٻاهريون_جو_range_گروهه_policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ڪنجي = عدم;
}

ساكن فراغ
Clear_ڪنجي_Info(ڪنجي_Info *keyi)
{
    گروهه_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_گروهه_Info(groupi);
    darray_free(keyi->groups);
    إذا (keyi->overlays_alloc)
        حرر(keyi->overlays_ڪنجيون);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح error_ڳڻپ;
    بنانشان صحيح include_اونهائي;
    xkb_layout_اشاريو_t explicit_گروهه;
    xkb_layout_اشاريو_t max_groups;
    darray(ڪنجي_Info) ڪنجيون;
    ڪنجي_Info default_ڪنجي;
    ActionsInfo default_actions;
    darray(xkb_atom_t) گروهه_نالا;
    darray(Mod_Map_داخلا) modmaps;
    بنية xkb_mod_سيٽ mods;

    بنية xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت بنية xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساكن فراغ
InitSymbolsInfo(SymbolsInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                بنانشان صحيح include_اونهائي, ثابت بنية xkb_mod_سيٽ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_اونهائي = include_اونهائي;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ڪنجي_Info(keymap_info->keymap.ctx, &info->default_ڪنجي);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_اونهائي > 0);
    info->explicit_گروهه = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ڪنجي_Info *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->ڪنجيون)
        Clear_ڪنجي_Info(keyi);
    darray_free(info->ڪنجيون);
    darray_free(info->گروهه_نالا);
    darray_free(info->modmaps);
    Clear_ڪنجي_Info(&info->default_ڪنجي);
}

ساكن ثابت حرف *
ڪنجي_Info_متن(SymbolsInfo *info, ڪنجي_Info *keyi)
{
    أرجع ڪنجي_Name_متن(info->ctx, keyi->name);
}

ساكن bool
ملايو_Groups(SymbolsInfo *info, گروهه_Info_2 *into, گروهه_Info_2 *from, bool clobber,
            bool report, xkb_layout_اشاريو_t گروهه, xkb_atom_t ڪنجي_name)
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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         گروهه + 1, ڪنجي_Name_متن(info->ctx, ڪنجي_name),
                         xkb_atom_متن(info->ctx, use),
                         xkb_atom_متن(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & گروهه_ميدان_TYPE);

    /* Now look at the levels. */

    إذا (darray_خالي_جايون(from->سطحون)) {
        Init_گروهه_Info(from);
        أرجع true;
    }

    إذا (darray_خالي_جايون(into->سطحون)) {
        from->type = into->type;
        *into = *from;
        Init_گروهه_Info(from);
        أرجع true;
    }

    /* Merge the actions and syms. */
    ثابت darray_سائيز_t سطحون_اندريون_both =
        MIN(darray_سائيز(into->سطحون), darray_سائيز(from->سطحون));
    darray_سائيز_t from_Keysyms_ڳڻپ = 0;
    darray_سائيز_t from_Actions_ڳڻپ = 0;
    لكل (darray_سائيز_t i = 0; i < سطحون_اندريون_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_شي(into->سطحون, i);
        بنية xkb_level* ثابت fromLevel = &darray_شي(from->سطحون, i);

        ثابت bool fromHasNoKeysym = fromLevel->انگ_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->انگ_actions == 0;
        إذا (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            استمر;
        }

        ثابت bool intoHasNoKeysym = intoLevel->انگ_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->انگ_actions == 0;
        إذا (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ڳڻپ++;
            from_Actions_ڳڻپ++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->انگ_syms > 0 || intoLevel->انگ_actions > 0);
        أكد(fromLevel->انگ_syms > 0 || fromLevel->انگ_actions > 0);

        /* Handle keysyms */
        إذا (!Xkb_سطحون_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            إذا (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_نشان,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, گروهه + 1, ڪنجي_Name_متن(info->ctx, ڪنجي_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            إذا (fromHasNoKeysym) {
                /* No keysym to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined keysym from `from` */
                إذا (unlikely(fromLevel->انگ_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->انگ_syms > 1))
                        حرر(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->انگ_syms = fromLevel->انگ_syms;
                    fromLevel->انگ_syms = 0;
                    from_Keysyms_ڳڻپ++;
                } وإلا إذا (fromLevel->s.sym != XKB_ڪنجي_No_نشان) {
                    /* Single defined keysym */
                    إذا (unlikely(intoLevel->انگ_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->انگ_syms = 1;
                    from_Keysyms_ڳڻپ++;
                }
            } وإلا {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->انگ_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->s.sym == XKB_ڪنجي_No_نشان) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->انگ_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->انگ_syms = fromLevel->انگ_syms;
                        fromLevel->انگ_syms = 0;
                    } وإلا {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->انگ_syms = 1;
                    }
                    from_Keysyms_ڳڻپ++;
                }
            }
        }

        /* Handle actions */
        إذا (!Xkb_سطحون_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            إذا (report && !(intoHasNoAction || fromHasNoAction)) {
                إذا (unlikely(intoLevel->انگ_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, گروهه + 1, ڪنجي_Name_متن(info->ctx, ڪنجي_name),
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

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, گروهه + 1,
                                ڪنجي_Name_متن(info->ctx, ڪنجي_name),
                                Action_Type_متن(use->type),
                                Action_Type_متن(ignore->type));
                }
            }
            إذا (fromHasNoAction) {
                /* No action to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined action from `from` */
                إذا (unlikely(fromLevel->انگ_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->انگ_actions > 1))
                        حرر(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->انگ_actions = fromLevel->انگ_actions;
                    fromLevel->انگ_actions = 0;
                    from_Actions_ڳڻپ++;
                } وإلا إذا (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    إذا (unlikely(intoLevel->انگ_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->انگ_actions = 1;
                    from_Actions_ڳڻپ++;
                }
            } وإلا {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->انگ_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->انگ_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->انگ_actions = fromLevel->انگ_actions;
                        fromLevel->انگ_actions = 0;
                    } وإلا {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->انگ_actions = 1;
                    }
                    from_Actions_ڳڻپ++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    بنية xkb_level *level;
    darray_foreach_from(level, from->سطحون, سطحون_اندريون_both) {
        darray_append(into->سطحون, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->انگ_syms = 0;
        level->انگ_actions = 0;
        from_Keysyms_ڳڻپ++;
        from_Actions_ڳڻپ++;
    }
    إذا (from_Keysyms_ڳڻپ) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        إذا (from_Keysyms_ڳڻپ == darray_سائيز(into->سطحون))
            into->defined &= ~گروهه_ميدان_SYMS;
        into->defined |= (from->defined & گروهه_ميدان_SYMS);
    }
    إذا (from_Actions_ڳڻپ) {
        /* Reset defined actions field if we used no action from `into` */
        إذا (from_Actions_ڳڻپ == darray_سائيز(into->سطحون))
            into->defined &= ~گروهه_ميدان_ACTS;
        into->defined |= (from->defined & گروهه_ميدان_ACTS);
    }

    أرجع true;
}

ساكن bool
Use_New_ڪنجي_ميدان(تعداد ڪنجي_ميدان ميدان_2, تعداد ڪنجي_ميدان old, تعداد ڪنجي_ميدان new,
               bool clobber, bool report, تعداد ڪنجي_ميدان *collide)
{
    إذا (!(old & ميدان_2))
        أرجع (new & ميدان_2);

    إذا (new & ميدان_2) {
        إذا (report)
            *collide |= ميدان_2;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
overlays_get(ثابت ڪنجي_Info *info, xkb_overlay_اشاريو_t ٻٽو_انگ,
             ثابت بنية xkb_ڪنجي **ڪنجي_ٻاهريون)
{
    إذا (ٻٽو_انگ >= (xkb_overlay_اشاريو_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        أرجع false;
    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ٻٽو_انگ);
    إذا (!(info->overlays & نقاب)) أرجع false;
    إذا (ڪنجي_ٻاهريون) {
        إذا (!info->overlays_alloc) {
            /* Stored inline */
            *ڪنجي_ٻاهريون = info->overlay_ڪنجي;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
                info->overlays & (نقاب - 1u)
            );
            ثابت xkb_overlay_اشاريو_t اشاريو = (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
            *ڪنجي_ٻاهريون = info->overlays_ڪنجيون[اشاريو];
        }
    }
    أرجع true;
}

ساكن bool
overlays_وجهو(ڪنجي_Info *keyi, xkb_overlay_اشاريو_t ٻٽو_انگ,
                ثابت بنية xkb_ڪنجي *ڪنجي)
{
    إذا (ٻٽو_انگ >= (xkb_overlay_اشاريو_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        أرجع false;

    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ٻٽو_انگ);

    إذا ((keyi->overlays & نقاب) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        إذا (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ڪنجي = ڪنجي;
            إذا (!ڪنجي)
                keyi->overlays_clear = true;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
                keyi->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_اشاريو_t اشاريو =
                (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
            keyi->overlays_ڪنجيون[اشاريو] = ڪنجي;
        }
        أرجع true;
    }

    /* Insert new element */
    إذا (!keyi->overlays || (keyi->overlays_clear && !ڪنجي)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ڪنجي = ڪنجي;
        keyi->overlays |= نقاب;
        keyi->overlays_clear = !ڪنجي;
    } وإلا إذا (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_نقاب_t overlays = keyi->overlays | نقاب;
        ثابت xkb_overlay_اشاريو_t alloc =
            (xkb_overlay_اشاريو_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_ڪنجي ** ثابت tmp = صفر_يادگيري_ڏيو(alloc, حجم(*tmp));
        إذا (!tmp) أرجع false;

        /* Copy previous entries to the buffer */
        إذا (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
                overlays & (xkb_overlay_نقاب_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_اشاريو_t idx = (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
            tmp[idx] = keyi->overlay_ڪنجي;
        } وإلا {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
            overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
        );
        ثابت xkb_overlay_اشاريو_t idx = (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
        tmp[idx] = ڪنجي;

        keyi->overlays_ڪنجيون = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وإلا {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_نقاب_t overlays =
            (xkb_overlay_نقاب_t)(keyi->overlays | نقاب);
        ثابت xkb_overlay_اشاريو_t ڳڻپ =
            (xkb_overlay_اشاريو_t)popcount32(overlays);
        إذا (ڳڻپ > keyi->overlays_alloc) {
            ثابت xkb_overlay_اشاريو_t alloc =
                (xkb_overlay_اشاريو_t)ايندڙ_pow2(ڳڻپ);
            أكد(alloc >= ڳڻپ);
            ثابت بنية xkb_ڪنجي ** ثابت tmp = realloc(
                keyi->overlays_ڪنجيون,
                (سائيز_t)alloc * حجم(*keyi->overlays_ڪنجيون)
            );
            إذا (!tmp) أرجع false;
            keyi->overlays_ڪنجيون = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
            overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
        );
        ثابت xkb_overlay_اشاريو_t اشاريو = (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
        إذا (اشاريو >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        إذا (اشاريو < ڳڻپ - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(اشاريو + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ڪنجيون + اشاريو + 1, keyi->overlays_ڪنجيون + اشاريو,
                    (سائيز_t)(ڳڻپ - 1u - اشاريو) * حجم(ثابت بنية xkb_ڪنجي *));
        }
        keyi->overlays_ڪنجيون[اشاريو] = ڪنجي;
        keyi->overlays = overlays;
    }

    أرجع true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
ملايو_overlays(SymbolsInfo *info, ڪنجي_Info *into, ڪنجي_Info *from,
               bool clobber, bool report, تعداد ڪنجي_ميدان *collide)
{
    إذا (!(from->defined & ڪنجي_ميدان_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وإلا إذا (!(into->defined & ڪنجي_ميدان_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ڪنجيون = from->overlays_ڪنجيون;
        from->overlays_ڪنجيون = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ڪنجي_ميدان_OVERLAY;
    } وإلا إذا (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وإلا {
        إذا (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_نقاب_t نتيجو_نقاب =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_اشاريو_t ڳڻپ =
                (xkb_overlay_اشاريو_t)popcount32(نتيجو_نقاب);

            إذا (ڳڻپ == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ڪنجي_Info *dest = into;
            ڪنجي_Info *src = from;
            إذا (into->overlays_alloc >= ڳڻپ &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وإلا إذا (from->overlays_alloc >= ڳڻپ) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وإلا إذا (ڳڻپ > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                إذا (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                إذا (!dest->overlays_alloc) {
                    ثابت بنية xkb_ڪنجي **tmp = صفر_يادگيري_ڏيو(ڳڻپ, حجم(*tmp));
                    إذا (!tmp)
                        أرجع false;
                    إذا (dest->overlay_ڪنجي)
                        tmp[0] = dest->overlay_ڪنجي;
                    dest->overlays_ڪنجيون = tmp;
                    dest->overlays_alloc = ڳڻپ;
                } وإلا {
                    ثابت بنية xkb_ڪنجي **tmp =
                        realloc(dest->overlays_ڪنجيون,
                                ڳڻپ * حجم(*dest->overlays_ڪنجيون));
                    إذا (!tmp)
                        أرجع false;
                    dest->overlays_ڪنجيون = tmp;
                    dest->overlays_alloc = ڳڻپ;
                }
            }

            xkb_overlay_نقاب_t بچيل = src->overlays;
            ثابت بنية xkb_ڪنجي **src_ڪنجيون = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_ڪنجيون
                    : &src->overlay_ڪنجي;
            طالما (بچيل) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بچيل &
                    (xkb_overlay_نقاب_t)((بنانشان)~بچيل + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_اشاريو_t ٻٽو_انگ =
                    (xkb_overlay_اشاريو_t)popcount32(lsb - 1u);
                بچيل = (بچيل & ~lsb);

                /* pop current value */
                إذا (!src->overlays_clear && !src->overlays_alloc && بچيل)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_ڪنجي *src_ڪنجي = (src_ڪنجيون)
                    ? *(src_ڪنجيون++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_ڪنجي *dest_ڪنجي = عدم;
                ثابت bool conflict = overlays_get(dest, ٻٽو_انگ, &dest_ڪنجي);
                إذا (conflict) {
                    إذا (dest_ڪنجي == src_ڪنجي)
                       استمر;
                    وإلا إذا (report)
                        *collide |= ڪنجي_ميدان_OVERLAY;
                }

                إذا ((!conflict || clobber) && !overlays_وجهو(dest, ٻٽو_انگ, src_ڪنجي))
                    أرجع false;
            }

            إذا (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                إذا (into->overlays_alloc)
                    حرر(into->overlays_ڪنجيون);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                إذا (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ڪنجيون = from->overlays_ڪنجيون;
                    from->overlays_ڪنجيون = عدم;
                    from->overlays_alloc = 0;
                } وإلا {
                    into->overlay_ڪنجي = from->overlay_ڪنجي;
                }
            }
        } وإلا {
            /* Disjoint overlays */

            إذا (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ڪنجي == from->overlay_ڪنجي) {
                /* Indentical: do nothing */
                أرجع true;
            }
            إذا (!(into->overlays & from->overlays)) {
                إذا (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ڪنجي = from->overlay_ڪنجي;
                    أرجع true;
                } وإلا إذا (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    أرجع true;
                }
            }
            /* Conflict */
            إذا (report)
                *collide |= ڪنجي_ميدان_OVERLAY;
            إذا (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ڪنجي = from->overlay_ڪنجي;
            }
        }
    }
    أرجع true;
}

ساكن bool
ملايو_ڪنجيون(SymbolsInfo *info, ڪنجي_Info *into, ڪنجي_Info *from, bool same_file)
{
    xkb_layout_اشاريو_t i;
    xkb_layout_اشاريو_t groups_اندريون_both;
    تعداد ڪنجي_ميدان collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->ملايو != ملايو_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (from->ملايو == ملايو_REPLACE) {
        Clear_ڪنجي_Info(into);
        *into = *from;
        Init_ڪنجي_Info(info->ctx, from);
        أرجع true;
    }

    groups_اندريون_both = MIN(darray_سائيز(into->groups), darray_سائيز(from->groups));
    لكل (i = 0; i < groups_اندريون_both; i++)
        ملايو_Groups(info,
                    &darray_شي(into->groups, i),
                    &darray_شي(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    لكل (i = groups_اندريون_both; i < darray_سائيز(from->groups); i++) {
        darray_append(into->groups, darray_شي(from->groups, i));
        Init_گروهه_Info(&darray_شي(from->groups, i));
    }

    إذا (Use_New_ڪنجي_ميدان(ڪنجي_ميدان_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ڪنجي_ميدان_VMODMAP;
    }
    إذا (Use_New_ڪنجي_ميدان(ڪنجي_ميدان_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ڪنجي_ميدان_REPEAT;
    }
    إذا (Use_New_ڪنجي_ميدان(ڪنجي_ميدان_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ڪنجي_ميدان_DEFAULT_TYPE;
    }
    إذا (Use_New_ڪنجي_ميدان(ڪنجي_ميدان_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->ٻاهريون_جو_range_pending_گروهه = from->ٻاهريون_جو_range_pending_گروهه;
        into->ٻاهريون_جو_range_گروهه_policy = from->ٻاهريون_جو_range_گروهه_policy;
        into->ٻاهريون_جو_range_گروهه_عدد = from->ٻاهريون_جو_range_گروهه_عدد;
        into->defined |= ڪنجي_ميدان_GROUPINFO;
    }

    إذا (!ملايو_overlays(info, into, from, clobber, report, &collide))
        أرجع false;

    إذا (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ڪنجي_Name_متن(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ڪنجي_Info(from);
    Init_ڪنجي_Info(info->ctx, from);
    أرجع true;
}

ساكن xkb_atom_t
Xkb_Resolve_ڪنجي_Alias(ثابت بنية xkb_keymap *keymap, xkb_atom_t name)
{
    إذا (name < keymap->انگ_ڪنجي_نالا) {
        ثابت Keycode_ميلاپ ميلاپ = keymap->ڪنجي_نالا[name];
        إذا (ميلاپ.مليو && ميلاپ.is_alias) {
            أرجع ميلاپ.alias.real;
        }
    }
    أرجع name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
جوڙيو_ڪنجي_Symbols(SymbolsInfo *info, ڪنجي_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ڪنجي_Alias(&info->keymap_info->keymap, keyi->name);

    ڪنجي_Info *iter;
    darray_foreach(iter, info->ڪنجيون)
        إذا (iter->name == keyi->name)
            أرجع ملايو_ڪنجيون(info, iter, keyi, same_file);

    darray_append(info->ڪنجيون, *keyi);
    Init_ڪنجي_Info(info->ctx, keyi);
    أرجع true;
}

ساكن bool
جوڙيو_Mod_Map_داخلا(SymbolsInfo *info, ثابت Mod_Map_داخلا *new)
{
    ثابت bool clobber = (new->ملايو != ملايو_AUGMENT);

    Mod_Map_داخلا *old;
    darray_foreach(old, info->modmaps) {
        إذا ((new->have_نشان != old->have_نشان) ||
            (new->have_نشان && new->u.ڪنجي_Sym != old->u.ڪنجي_Sym) ||
            (!new->have_نشان && new->u.ڪنجي_Name != old->u.ڪنجي_Name))
            استمر;

        إذا (new->mods == old->mods)
            أرجع true;

        ثابت xkb_mod_نقاب_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_نقاب_t ignore = (clobber ? old->mods : new->mods);

        إذا (new->have_نشان) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_متن(info->ctx, new->u.ڪنجي_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        } وإلا {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ڪنجي_Name_متن(info->ctx, new->u.ڪنجي_Name),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        أرجع true;
    }

    darray_append(info->modmaps, *new);

    أرجع true;
}

/***====================================================================***/

ساكن فراغ
ملايو_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد ملايو_گهڻائي ملايو)
{
    xkb_atom_t *گروهه_name;
    xkb_layout_اشاريو_t گروهه_نالا_اندريون_both;

    إذا (from->error_ڳڻپ > 0) {
        into->error_ڳڻپ += from->error_ڳڻپ;
        أرجع;
    }

    ملايو_Mod_Sets(into->ctx, &into->mods, &from->mods, ملايو);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    گروهه_نالا_اندريون_both = MIN(darray_سائيز(into->گروهه_نالا),
                              darray_سائيز(from->گروهه_نالا));
    لكل (xkb_layout_اشاريو_t i = 0; i < گروهه_نالا_اندريون_both; i++) {
        إذا (!darray_شي(from->گروهه_نالا, i))
            استمر;

        إذا (ملايو == ملايو_AUGMENT && darray_شي(into->گروهه_نالا, i))
            استمر;

        darray_شي(into->گروهه_نالا, i) = darray_شي(from->گروهه_نالا, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(گروهه_name, from->گروهه_نالا, گروهه_نالا_اندريون_both)
        darray_append(into->گروهه_نالا, *گروهه_name);

    إذا (darray_خالي_جايون(into->ڪنجيون)) {
        into->ڪنجيون = from->ڪنجيون;
        darray_init(from->ڪنجيون);
    }
    وإلا {
        ڪنجي_Info *keyi;
        darray_foreach(keyi, from->ڪنجيون) {
            keyi->ملايو = ملايو;
            إذا (!جوڙيو_ڪنجي_Symbols(into, keyi, false))
                into->error_ڳڻپ++;
        }
    }

    إذا (darray_خالي_جايون(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وإلا {
        Mod_Map_داخلا *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ملايو = ملايو;
            إذا (!جوڙيو_Mod_Map_داخلا(into, mm))
                into->error_ڳڻپ++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    إذا (Exceeds_Include_Max_اونهائي(info->ctx, info->include_اونهائي)) {
        info->error_ڳڻپ += 10;
        أرجع false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_اونهائي + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->ايندڙ_incl) {
        SymbolsInfo ايندڙ_incl;
        XkbFile *file;

        حرف رستو[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  رستو, حجم(رستو));
        إذا (!file) {
            info->error_ڳڻپ += 10;
            ClearSymbolsInfo(&included);
            أرجع false;
        }

        InitSymbolsInfo(&ايندڙ_incl, info->keymap_info, info->include_اونهائي + 1,
                        &included.mods);
        إذا (stmt->modifier) {
            ثابت صحيح ڳڻپ = parse_dec_منزل_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &ايندڙ_incl.explicit_گروهه);
            إذا (ڳڻپ <= 0 || *(stmt->modifier + ڳڻپ) != '\0' ||
                ايندڙ_incl.explicit_گروهه == 0 ||
                ايندڙ_incl.explicit_گروهه > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                ايندڙ_incl.explicit_گروهه = info->explicit_گروهه;
            } وإلا {
                ايندڙ_incl.explicit_گروهه--;
            }
        }
        وإلا إذا (info->keymap_info->keymap.انگ_groups != 0 &&
                 ايندڙ_incl.include_اونهائي == 1) {
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
            ايندڙ_incl.explicit_گروهه = 0;
        }
        وإلا {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            ايندڙ_incl.explicit_گروهه = info->explicit_گروهه;
        }

        HandleSymbolsFile(&ايندڙ_incl, file);

        ملايو_Included_Symbols(&included, &ايندڙ_incl, stmt->ملايو);

        ClearSymbolsInfo(&ايندڙ_incl);
        FreeXkbFile(file);
    }

    ملايو_Included_Symbols(info, &included, include->ملايو);
    ClearSymbolsInfo(&included);

    أرجع (info->error_ڳڻپ == 0);
}

ساكن bool
Get_گروهه_اشاريو(SymbolsInfo *info, ڪنجي_Info *keyi, اظهار_Def *arrayNdx,
              تعداد گروهه_ميدان ميدان_2, xkb_layout_اشاريو_t *ndx_rtrn)
{
    أكد (ميدان_2 == گروهه_ميدان_SYMS || ميدان_2 == گروهه_ميدان_ACTS);
    ثابت حرف *name = (ميدان_2 == گروهه_ميدان_SYMS ? "symbols" : "actions");

    إذا (arrayNdx == عدم) {
        xkb_layout_اشاريو_t i = 0;
        گروهه_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            إذا (!(groupi->defined & ميدان_2)) {
                *ndx_rtrn = i;
                أرجع true;
            }
        }

        إذا (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ڪنجي_Info_متن(info, keyi), info->max_groups, name);
            أرجع false;
        }

        darray_resize0(keyi->groups, darray_سائيز(keyi->groups) + 1);
        *ndx_rtrn = darray_سائيز(keyi->groups) - 1;
        أرجع true;
    }

    إذا (اظهار_Resolve_گروهه(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ڪنجي_Info_متن(info, keyi));
        أرجع false;
    }

    (*ndx_rtrn)--;
    إذا (*ndx_rtrn >= darray_سائيز(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    أرجع true;
}

ساكن bool
جوڙيو_Symbols_منزل_ڪنجي(SymbolsInfo *info, ڪنجي_Info *keyi, اظهار_Def *arrayNdx,
                اظهار_Def *قدر)
{
    xkb_layout_اشاريو_t ndx = 0;

    إذا (!Get_گروهه_اشاريو(info, keyi, arrayNdx, گروهه_ميدان_SYMS, &ndx))
        أرجع false;

    گروهه_Info_2 *groupi = &darray_شي(keyi->groups, ndx);

    إذا (قدر->common.type == STMT_اظهار_خالي_جايون_LIST) {
        groupi->defined |= گروهه_ميدان_SYMS;
        أرجع true;
    }

    إذا (قدر->common.type != STMT_اظهار_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_منزل_string(قدر->common.type), ndx + 1,
                ڪنجي_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروهه_ميدان_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ڪنجي_SYMBOLS_داخلا,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ڪنجي_Info_متن(info, keyi), ndx + 1);
        أرجع false;
    }

    xkb_level_اشاريو_t n_سطحون = 0;
    xkb_level_اشاريو_t non_خالي_جايون_سطحون = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (اظهار_Keysym_List *keysymList = (اظهار_Keysym_List *) قدر;
         keysymList;
         keysymList = (اظهار_Keysym_List *) keysymList->common.ايندڙ) {
        n_سطحون++;
        /* Drop trailing NoSymbol */
        إذا (darray_سائيز(keysymList->syms) > 0)
            non_خالي_جايون_سطحون = n_سطحون;
    }
    إذا (non_خالي_جايون_سطحون < n_سطحون)
        n_سطحون = non_خالي_جايون_سطحون;
    إذا (darray_سائيز(groupi->سطحون) < n_سطحون)
        darray_resize0(groupi->سطحون, n_سطحون);

    groupi->defined |= گروهه_ميدان_SYMS;

    xkb_level_اشاريو_t level = 0;
    لكل (اظهار_Keysym_List *keysymList = (اظهار_Keysym_List *) قدر;
         keysymList && level < n_سطحون;
         keysymList = (اظهار_Keysym_List *) keysymList->common.ايندڙ, level++) {
        بنية xkb_level *leveli = &darray_شي(groupi->سطحون, level);
        أكد(leveli->انگ_syms == 0);

        إذا (unlikely(darray_سائيز(keysymList->syms) > MAX_KEYSYMS_في_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ڪنجي_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_في_LEVEL, darray_سائيز(keysymList->syms));
            أرجع false;
        }

        leveli->انگ_syms = (xkb_keysym_ڳڻپ_t) darray_سائيز(keysymList->syms);
        بدل (leveli->انگ_syms) {
        حالة 0:
            leveli->s.sym = XKB_ڪنجي_No_نشان;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_شي(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_ڪنجي_No_نشان);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#جيڪڏهن_وصف_نه_ٿيل NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_ڳڻپ_t k = 0; k < leveli->انگ_syms; k++)
                أكد(leveli->s.syms[k] != XKB_ڪنجي_No_نشان);
#جيڪڏهن_پورو
        }
    }

    أرجع true;
}

ساكن bool
جوڙيو_Actions_منزل_ڪنجي(SymbolsInfo *info, ڪنجي_Info *keyi, اظهار_Def *arrayNdx,
                اظهار_Def *قدر)
{
    xkb_layout_اشاريو_t ndx = 0;

    إذا (!Get_گروهه_اشاريو(info, keyi, arrayNdx, گروهه_ميدان_ACTS, &ndx))
        أرجع false;

    گروهه_Info_2 *groupi = &darray_شي(keyi->groups, ndx);

    إذا (قدر->common.type == STMT_اظهار_خالي_جايون_LIST) {
        groupi->defined |= گروهه_ميدان_ACTS;
        أرجع true;
    }

    إذا (قدر->common.type != STMT_اظهار_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_اظهار_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 قدر->common.type, ndx, ڪنجي_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروهه_ميدان_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ڪنجي_Info_متن(info, keyi), ndx);
        أرجع false;
    }

    xkb_level_اشاريو_t n_سطحون = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &قدر->common; p; p = p->ايندڙ)
        n_سطحون++;
    إذا (darray_سائيز(groupi->سطحون) < n_سطحون)
        darray_resize0(groupi->سطحون, n_سطحون);

    groupi->defined |= گروهه_ميدان_ACTS;

    xkb_level_اشاريو_t level = 0;
    xkb_level_اشاريو_t non_خالي_جايون_سطحون = 0;
    لكل (اظهار_Action_List *actionList = (اظهار_Action_List *) قدر;
         actionList;
         actionList = (اظهار_Action_List *) actionList->common.ايندڙ, level++) {
        بنية xkb_level *leveli = &darray_شي(groupi->سطحون, level);
        أكد(leveli->انگ_actions == 0);

        بنانشان صحيح انگ_actions = 0;
        لكل (اظهار_Def *act = actionList->actions;
             act; act = (اظهار_Def *) act->common.ايندڙ)
            انگ_actions++;

        إذا (unlikely(انگ_actions > MAX_ACTIONS_في_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ڪنجي_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_في_LEVEL, انگ_actions);
            أرجع false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (اظهار_Def *act = actionList->actions;
             act; act = (اظهار_Def *) act->common.ايندڙ) {
            اتحاد xkb_action منزل_Act = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &منزل_Act);
            إذا (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ڪنجي_Info_متن(info, keyi), ndx + 1, level + 1);
                إذا (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    أرجع false;
                } وإلا {
                    /* Ensure action type is reset */
                    منزل_Act.type = ACTION_TYPE_NONE;
                }
            }
            إذا (منزل_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } وإلا إذا (likely(انگ_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->انگ_actions = 1;
                leveli->a.action = منزل_Act;
                اذهب ايندڙ_level;
            } وإلا {
                darray_append(actions, منزل_Act);
            }
        }
        إذا (darray_خالي_جايون(actions)) {
            leveli->انگ_actions = 0;
        } وإلا إذا (likely(darray_سائيز(actions) > 1)) {
            leveli->انگ_actions = (xkb_action_ڳڻپ_t) darray_سائيز(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#جيڪڏهن_وصف_نه_ٿيل NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_ڳڻپ_t k = 0; k < leveli->انگ_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#جيڪڏهن_پورو
        } وإلا {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(انگ_actions > darray_سائيز(actions));
            leveli->انگ_actions = 1;
            leveli->a.action = darray_شي(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
ايندڙ_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        إذا (leveli->انگ_actions > 0 || leveli->انگ_syms > 0)
            non_خالي_جايون_سطحون = level + 1;
    }

    إذا (non_خالي_جايون_سطحون < n_سطحون) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_سائيز(groupi->سطحون) = non_خالي_جايون_سطحون;
        إذا (non_خالي_جايون_سطحون > 0)
            darray_shrink(groupi->سطحون);
        وإلا
            darray_free(groupi->سطحون);
    }

    أرجع true;
}

ساكن ثابت Lookup_داخلا repeatEntries[] = {
    { "true", ڪنجي_REPEAT_YES },
    { "yes", ڪنجي_REPEAT_YES },
    { "on", ڪنجي_REPEAT_YES },
    { "false", ڪنجي_REPEAT_NO },
    { "no", ڪنجي_REPEAT_NO },
    { "off", ڪنجي_REPEAT_NO },
    { "default", ڪنجي_REPEAT_UNDEFINED },
    { عدم, 0 }
};

/** Handle overlay<N> = <key> entry */
ساكن bool
اظهار_Resolve_Overlay_داخلا(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *ميدان_2, ثابت اظهار_Def *arrayNdx,
                        ثابت اظهار_Def *اظهار, ڪنجي_Info *keyi,
                        xkb_overlay_اشاريو_t *overlay_rtrn,
                        ثابت بنية xkb_ڪنجي **ڪنجي_rtrn)
{
    /* Overlay index */
    إذا (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                ميدان_2, ڪنجي_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    static_assert(حجم(xkb_overlay_اشاريو_t) == حجم(uint8_t), "");
    ثابت سائيز_t اڳياڙي = حجم("overlay") - 1;
    ثابت سائيز_t len = لڙي_ڊيگهه(ميدان_2 + اڳياڙي);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    إذا (parse_dec_منزل_uint64_t(ميدان_2 + اڳياڙي, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاريو,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                ميدان_2, ڪنجي_Name_متن(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        أرجع false;
    }
    *overlay_rtrn = (xkb_overlay_اشاريو_t)raw_overlay - 1;

    /* Overlay key */
    بدل (اظهار->common.type) {
    حالة STMT_اظهار_KEYNAME_LITERAL:
        *ڪنجي_rtrn = Xkb_ڪنجي_By_Name(&keymap_info->keymap, اظهار->ڪنجي_name.ڪنجي_name,
                                 false);
        إذا (!*ڪنجي_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_متن(keymap_info->keymap.ctx, اظهار->ڪنجي_name.ڪنجي_name),
                    ميدان_2, ڪنجي_Name_متن(keymap_info->keymap.ctx, keyi->name));
            أرجع false;
        }
        أرجع true;
    حالة STMT_اظهار_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_متن(keymap_info->keymap.ctx, اظهار->ident.ident);
        إذا (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ڪنجي_rtrn = عدم;
            أرجع true;
        } وإلا إذا (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ڪنجي_rtrn = عدم;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            أرجع true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, ميدان_2, ڪنجي_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_منزل_string(STMT_اظهار_KEYNAME_LITERAL),
                ميدان_2, ڪنجي_Name_متن(keymap_info->keymap.ctx, keyi->name),
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;
    }
}

ساكن bool
سيٽ_Symbols_ميدان(SymbolsInfo *info, ڪنجي_Info *keyi, ثابت حرف *ميدان_2,
                اظهار_Def *arrayNdx, اظهار_Def **قدر_ptr)
{
    اظهار_Def * محدود ثابت قدر = *قدر_ptr;
    إذا (istreq(ميدان_2, "type")) {
        xkb_layout_اشاريو_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        إذا (!اظهار_Resolve_String(info->ctx, قدر, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            أرجع false;
        }

        إذا (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ڪنجي_ميدان_DEFAULT_TYPE;
        }
        وإلا إذا (اظهار_Resolve_گروهه(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }
        وإلا {
            ndx--;
            إذا (ndx >= darray_سائيز(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_سائيز(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_شي(keyi->groups, ndx).type = val;
            darray_شي(keyi->groups, ndx).defined |= گروهه_ميدان_TYPE;
        }
    }
    وإلا إذا (istreq(ميدان_2, "symbols")) {
        أرجع جوڙيو_Symbols_منزل_ڪنجي(info, keyi, arrayNdx, قدر);
    }
    وإلا إذا (istreq(ميدان_2, "actions")) {
        أرجع جوڙيو_Actions_منزل_ڪنجي(info, keyi, arrayNdx, قدر);
    }
    وإلا إذا (istreq(ميدان_2, "vmods") ||
             istreq(ميدان_2, "virtualmods") ||
             istreq(ميدان_2, "virtualmodifiers")) {
        xkb_mod_نقاب_t نقاب = 0;

        إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, قدر, MOD_VIRT, &info->mods,
                                &نقاب)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_منزل_string(قدر->common.type),
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->vmodmap = نقاب;
        keyi->defined |= ڪنجي_ميدان_VMODMAP;
    }
    وإلا إذا (istreq(ميدان_2, "locking") ||
             istreq(ميدان_2, "lock") ||
             istreq(ميدان_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ڪنجي_Info_متن(info, keyi));
    }
    وإلا إذا (istreq(ميدان_2, "radiogroup") ||
             istreq(ميدان_2, "permanentradiogroup") ||
             istreq(ميدان_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ڪنجي_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_اڳياڙي("permanentoverlay", ميدان_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ڪنجي_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_اڳياڙي("overlay", ميدان_2)) {
        xkb_overlay_اشاريو_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_ڪنجي *ڪنجي = عدم;
        إذا (!اظهار_Resolve_Overlay_داخلا(info->keymap_info, ميدان_2, arrayNdx,
                                     *قدر_ptr, keyi, &overlay, &ڪنجي))
            أرجع false;
        إذا (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            أرجع true;
        } وإلا إذا (ڪنجي && ڪنجي->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_پيغام_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ڪنجي_Info_متن(info, keyi));
        } وإلا {
            ثابت بنية xkb_ڪنجي *prev;
            إذا (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                إذا (ڪنجي != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ڪنجي_Info_متن(info, keyi), overlay + 1,
                             (prev
                                 ? ڪنجي_Name_متن(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ڪنجي
                                 ? ڪنجي_Name_متن(info->ctx, ڪنجي->name)
                                 : "none"));
                }
            } وإلا إذا (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                إذا (!overlays_وجهو(keyi, overlay, ڪنجي))
                    أرجع false;
                keyi->defined |= ڪنجي_ميدان_OVERLAY;
            } وإلا {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << overlay);
                إذا (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    إذا (ڪنجي) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = نقاب;
                        keyi->overlays_clear = false;
                        keyi->overlay_ڪنجي = ڪنجي;
                    } وإلا {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= نقاب;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ڪنجي_ميدان_OVERLAY;
                } وإلا إذا (keyi->overlays) {
                    إذا (ڪنجي) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ڪنجي_Info_متن(info, keyi), keyi->overlays,
                                ڪنجي_Name_متن(info->ctx, keyi->overlay_ڪنجي->name),
                                overlay + 1, ڪنجي_Name_متن(info->ctx, ڪنجي->name));
                        أرجع !(info->keymap_info->strict &
                                 PARSER_NO_ميدان_قدر_MISMATCH);
                    } وإلا {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وإلا إذا (istreq(ميدان_2, "repeating") ||
             istreq(ميدان_2, "repeats") ||
             istreq(ميدان_2, "repeat")) {
        uint32_t val = 0;

        إذا (!اظهار_Resolve_Enum(info->ctx, قدر, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->repeat = (تعداد ڪنجي_repeat) val;
        keyi->defined |= ڪنجي_ميدان_REPEAT;
    }
    وإلا إذا (istreq(ميدان_2, "groupswrap") ||
             istreq(ميدان_2, "wrapgroups")) {
        bool سيٽ = false;

        إذا (!اظهار_Resolve_Boolean(info->ctx, قدر, &سيٽ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->ٻاهريون_جو_range_گروهه_policy = (سيٽ)
            ? XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP
            : XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP;
        keyi->defined |= ڪنجي_ميدان_GROUPINFO;
    }
    وإلا إذا (istreq(ميدان_2, "groupsclamp") ||
             istreq(ميدان_2, "clampgroups")) {
        bool سيٽ = false;

        إذا (!اظهار_Resolve_Boolean(info->ctx, قدر, &سيٽ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->ٻاهريون_جو_range_گروهه_policy = (سيٽ)
            ? XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP
            : XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP;
        keyi->defined |= ڪنجي_ميدان_GROUPINFO;
    }
    وإلا إذا (istreq(ميدان_2, "groupsredirect") ||
             istreq(ميدان_2, "redirectgroups")) {
        xkb_layout_اشاريو_t grp = 0;
        bool pending = false;

        // TODO: recover?
        إذا (اظهار_Resolve_گروهه(info->keymap_info, قدر, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ڪنجي_Info_متن(info, keyi));
            أرجع false;
        }

        إذا (pending) {
            keyi->ٻاهريون_جو_range_pending_گروهه = true;
            ثابت darray_سائيز_t pending_اشاريو =
                darray_سائيز(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .اظهار = *قدر_ptr,
                    .computed = false,
                    .قدر = 0,
                }
            );
            *قدر_ptr = عدم;
            static_assert(حجم(keyi->ٻاهريون_جو_range_گروهه_عدد) ==
                          حجم(pending_اشاريو),
                          "Cannot save pending computation");
            keyi->ٻاهريون_جو_range_گروهه_عدد = pending_اشاريو;
        } وإلا {
            keyi->ٻاهريون_جو_range_pending_گروهه = false;
            keyi->ٻاهريون_جو_range_گروهه_عدد = grp - 1;
        }

        keyi->ٻاهريون_جو_range_گروهه_policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT;
        keyi->defined |= ڪنجي_ميدان_GROUPINFO;
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ميدان,
                "Unknown field \"%s\" in a key; definition ignored\n",
                ميدان_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ڪنجي_FIELDS);
    }

    أرجع true;
}

ساكن bool
سيٽ_گروهه_Name(SymbolsInfo *info, اظهار_Def *arrayNdx, اظهار_Def *قدر,
             تعداد ملايو_گهڻائي ملايو)
{
    إذا (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_گروهه_NAME_اشاريو,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        أرجع false;
    }

    xkb_layout_اشاريو_t گروهه = 0;
    إذا (اظهار_Resolve_گروهه(info->keymap_info, arrayNdx, false, &گروهه, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!اظهار_Resolve_String(info->ctx, قدر, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", گروهه);
        أرجع false;
    }

    xkb_layout_اشاريو_t گروهه_منزل_use;
    إذا (info->explicit_گروهه == XKB_LAYOUT_INVALID) {
        گروهه_منزل_use = گروهه - 1;
    }
    وإلا إذا (گروهه - 1 == 0) {
        گروهه_منزل_use = info->explicit_گروهه;
    }
    وإلا {
        log_warn(info->ctx, XKB_WARNING_NON_بنياد_گروهه_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, گروهه,
                 xkb_atom_متن(info->ctx, name));
        أرجع false;
    }

    إذا (گروهه_منزل_use >= darray_سائيز(info->گروهه_نالا)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_سائيز(info->گروهه_نالا) < گروهه_منزل_use + 1);
        darray_resize0(info->گروهه_نالا, گروهه_منزل_use + 1);
    } وإلا {
        ثابت xkb_atom_t old_name = darray_شي(info->گروهه_نالا, گروهه_منزل_use);
        إذا (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (ملايو != ملايو_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     گروهه_منزل_use, info->name,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
            name = use;
        }
    }
    darray_شي(info->گروهه_نالا, گروهه_منزل_use) = name;

    أرجع true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *arrayNdx;
    bool ret;

    إذا (!اظهار_Resolve_Lhs(info->ctx, stmt->name, &elem, &ميدان_2, &arrayNdx))
        أرجع false;

    إذا (elem && istreq(elem, "key")) {
        ڪنجي_Info عارضي = {0};
        Init_ڪنجي_Info(info->ctx, &عارضي);
        /* Do not replace the whole key, only the current field */
        عارضي.ملايو = (عارضي.ملايو == ملايو_REPLACE)
            ? ملايو_OVERRIDE
            : stmt->ملايو;
        ret = سيٽ_Symbols_ميدان(info, &عارضي, ميدان_2, arrayNdx, &stmt->قدر) &&
              ملايو_ڪنجيون(info, &info->default_ڪنجي, &عارضي, true);
        Clear_ڪنجي_Info(&عارضي);
    }
    وإلا إذا (!elem && (istreq(ميدان_2, "name") ||
                       istreq(ميدان_2, "groupname"))) {
        ret = سيٽ_گروهه_Name(info, arrayNdx, stmt->قدر, stmt->ملايو);
    }
    وإلا إذا (!elem && (istreq(ميدان_2, "groupswrap") ||
                       istreq(ميدان_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(ميدان_2, "groupsclamp") ||
                       istreq(ميدان_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(ميدان_2, "groupsredirect") ||
                       istreq(ميدان_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && istreq(ميدان_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ميدان,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وإلا إذا (elem) {
        ret = (سيٽ_Default_Action_ميدان(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ميدان_2, arrayNdx,
                                     &stmt->قدر, stmt->ملايو) !=
               PARSER_FATAL_ERROR);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ميدان,
                "Default defined for unknown field \"%s\"; Ignored\n", ميدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    أرجع ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ڪنجي_Info *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.ايندڙ) {
        ثابت حرف *ميدان_2;
        اظهار_Def *arrayNdx;
        bool ok = true;

        إذا (!def->name) {
            إذا (unlikely(!def->قدر) || def->قدر->common.type != STMT_اظهار_ACTION_LIST)
                ميدان_2 = "symbols"; /* Default to symbols field */
            وإلا
                ميدان_2 = "actions";
            arrayNdx = عدم;
        }
        وإلا {
            ثابت حرف *elem;
            ok = اظهار_Resolve_Lhs(info->ctx, def->name, &elem, &ميدان_2,
                                &arrayNdx);
            إذا (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ميدان_2);
                ok = false;
            }
        }

        إذا (unlikely(!def->قدر)) {
            log_err(info->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", ميدان_2);
            ok = false;
        }

        إذا (!ok || !سيٽ_Symbols_ميدان(info, keyi, ميدان_2, arrayNdx, &def->قدر))
            all_valid_entries = false;
    }

    أرجع all_valid_entries;
}

ساكن bool
سيٽ_Explicit_گروهه(SymbolsInfo *info, ڪنجي_Info *keyi)
{
    xkb_layout_اشاريو_t i;
    گروهه_Info_2 *groupi;
    bool warn = false;

    إذا (info->explicit_گروهه == XKB_LAYOUT_INVALID)
        أرجع true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        إذا (groupi->defined) {
            warn = true;
            Clear_گروهه_Info(groupi);
            Init_گروهه_Info(groupi);
        }
    }

    إذا (warn) {
        log_warn(info->ctx, XKB_WARNING_گڻو_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_گروهه + 1, ڪنجي_Info_متن(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_گروهه + 1);
    إذا (info->explicit_گروهه > 0) {
        darray_شي(keyi->groups, info->explicit_گروهه) =
            darray_شي(keyi->groups, 0);
        Init_گروهه_Info(&darray_شي(keyi->groups, 0));
    }

    أرجع true;
}

ساكن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ڪنجي_Info keyi;

    keyi = info->default_ڪنجي;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ڪنجي.groups);
    لكل (xkb_layout_اشاريو_t i = 0; i < darray_سائيز(keyi.groups); i++)
        Copy_گروهه_Info(&darray_شي(keyi.groups, i),
                      &darray_شي(info->default_ڪنجي.groups, i));
    keyi.ملايو = stmt->ملايو;
    keyi.name = stmt->ڪنجي_Name;

    إذا (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        سيٽ_Explicit_گروهه(info, &keyi) &&
        جوڙيو_ڪنجي_Symbols(info, &keyi, true)) {
        أرجع true;
    }

    Clear_ڪنجي_Info(&keyi);
    info->error_ڳڻپ++;
    أرجع false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_داخلا tmp;
    بنية xkb_context *ctx = info->ctx;

    إذا (!اظهار_Resolve_Mod_نقاب(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        أرجع false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_متن_V1 ||
           def->modifiers->common.type == STMT_اظهار_IDENT);

    bool ok = true;
    tmp.ملايو = def->ملايو;

    لكل (اظهار_Def *ڪنجي = def->ڪنجيون; ڪنجي; ڪنجي = (اظهار_Def *) ڪنجي->common.ايندڙ) {
        إذا (ڪنجي->common.type == STMT_اظهار_KEYNAME_LITERAL) {
            tmp.have_نشان = false;
            tmp.u.ڪنجي_Name = ڪنجي->ڪنجي_name.ڪنجي_name;
        }
        وإلا إذا (ڪنجي->common.type == STMT_اظهار_KEYSYM_LITERAL) {
            إذا (ڪنجي->keysym.keysym == XKB_ڪنجي_No_نشان) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } وإلا {
                tmp.have_نشان = true;
                tmp.u.ڪنجي_Sym = ڪنجي->keysym.keysym;
            }
        }
        وإلا {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_داخلا,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = جوڙيو_Mod_Map_داخلا(info, &tmp) && ok;
    }
    أرجع ok;
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ايندڙ) {
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
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_ڳڻپ++;

        إذا (info->error_ڳڻپ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    محفوظ_map_name(file->name));
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
ساكن بنية xkb_ڪنجي *
ڳوليو_ڪنجي_For_نشان(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_گروهه;
    xkb_layout_اشاريو_t گروهه = 0;
    افعل {
        xkb_level_اشاريو_t level = 0;
        got_one_گروهه = false;
        bool got_one_level;
        افعل {
            got_one_level = false;
            بنية xkb_ڪنجي *ڪنجي;
            xkb_ڪنجيون_foreach(ڪنجي, keymap) {
                إذا (گروهه < ڪنجي->انگ_groups &&
                    level < Xkb_ڪنجي_انگ_سطحون(ڪنجي, گروهه)) {
                    got_one_گروهه = got_one_level = true;
                    ثابت xkb_keysym_ڳڻپ_t انگ_syms =
                        ڪنجي->groups[گروهه].سطحون[level].انگ_syms;
                    إذا (انگ_syms > 1) {
                        لكل (xkb_keysym_ڳڻپ_t k = 0; k < انگ_syms; k++) {
                            إذا (ڪنجي->groups[گروهه].سطحون[level].s.syms[k] == sym)
                                أرجع ڪنجي;
                        }
                    } وإلا إذا (انگ_syms &&
                               ڪنجي->groups[گروهه].سطحون[level].s.sym == sym) {
                        أرجع ڪنجي;
                    }
                }
            }
            level++;
        } طالما (got_one_level);
        گروهه++;
    } طالما (got_one_گروهه);

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
ڳوليو_Automatic_Type(بنية xkb_context *ctx, گروهه_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_اشاريو_t ويڪر = darray_سائيز(groupi->سطحون);

#وصف_ڪريو GET_SYM(level) \
    (darray_شي(groupi->سطحون, level).انگ_syms == 0 ? \
        XKB_ڪنجي_No_نشان : \
     darray_شي(groupi->سطحون, level).انگ_syms == 1 ? \
        darray_شي(groupi->سطحون, level).s.sym : \
     /* num_syms > 1 */ \
        darray_شي(groupi->سطحون, level).s.syms[0])

    إذا (ويڪر == 1 || ويڪر <= 0)
        أرجع xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    إذا (ويڪر == 2) {
        إذا (xkb_keysym_is_هيٺاهون(sym0) && xkb_keysym_is_مٿانهون_or_title(sym1))
            أرجع xkb_atom_intern_literal(ctx, "ALPHABETIC");

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    إذا (ويڪر <= 4) {
        إذا (xkb_keysym_is_هيٺاهون(sym0) && xkb_keysym_is_مٿانهون_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (ويڪر == 4 ? GET_SYM(3) : XKB_ڪنجي_No_نشان);

            إذا (xkb_keysym_is_هيٺاهون(sym2) && xkb_keysym_is_مٿانهون_or_title(sym3))
                أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    أرجع XKB_ATOM_NONE;

#وصف_رد GET_SYM
}

ساكن ثابت بنية xkb_ڪنجي_type *
ڳوليو_Type_For_گروهه(بنية xkb_keymap *keymap, ڪنجي_Info *keyi,
                 xkb_layout_اشاريو_t گروهه, bool *explicit_type)
{
    گروهه_Info_2 *groupi = &darray_شي(keyi->groups, گروهه);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    إذا (type_name == XKB_ATOM_NONE) {
        إذا (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وإلا {
            type_name = ڳوليو_Automatic_Type(keymap->ctx, groupi);
            إذا (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    إذا (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ڪنجي_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ڪنجي_Name_متن(keymap->ctx, keyi->name), گروهه + 1,
                 darray_سائيز(groupi->سطحون));
        اذهب use_default;
    }

    darray_سائيز_t i;
    لكل (i = 0; i < keymap->انگ_types; i++)
        إذا (keymap->types[i].name == type_name)
            اكسر;

    إذا (i >= keymap->انگ_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ڪنجي_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_متن(keymap->ctx, type_name),
                 ڪنجي_Name_متن(keymap->ctx, keyi->name), گروهه + 1);
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
Copy_Symbols_Def_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                       ڪنجي_Info *keyi)
{
    بنية xkb_ڪنجي *ڪنجي;
    گروهه_Info_2 *groupi;
    ثابت گروهه_Info_2 *group0;
    xkb_layout_اشاريو_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ڪنجي = Xkb_ڪنجي_By_Name(keymap, keyi->name, false);
    إذا (!ڪنجي) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ڪنجي_Info_متن(info, keyi));
        أرجع false;
    }

    /* Find the range of groups we need. */
    ڪنجي->انگ_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & ڪنجي_ميدان_DEFAULT_TYPE)
                                    || (groupi->defined & گروهه_ميدان_TYPE);
        إذا (darray_سائيز(groupi->سطحون) > 0 || has_explicit_type)
            ڪنجي->انگ_groups = i + 1;
        إذا (has_explicit_type)
            ڪنجي->explicit |= EXPLICIT_TYPES;
    }

    إذا (ڪنجي->انگ_groups <= 0) {
        /* A key with no group may still have other fields defined */
        إذا (keyi->defined)
            اذهب ڪنجي_fields;
        وإلا
            أرجع false;
    }

    darray_resize(keyi->groups, ڪنجي->انگ_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_شي(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        إذا (groupi->defined)
            استمر;

        Copy_گروهه_Info(groupi, group0);
    }

    ڪنجي->groups = صفر_يادگيري_ڏيو(ڪنجي->انگ_groups, حجم(*ڪنجي->groups));
    أكد(darray_سائيز(keyi->groups) == ڪنجي->انگ_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #جيڪڏهن_وصف_ٿيل __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ڪنجي->انگ_groups);
        #جيڪڏهن_پورو
        bool explicit_type = false;
        ثابت بنية xkb_ڪنجي_type * ثابت type =
            ڳوليو_Type_For_گروهه(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        إذا (type->انگ_سطحون < darray_سائيز(groupi->سطحون)) {
            بنية xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_متن(keymap->ctx, type->name), type->انگ_سطحون,
                    ڪنجي_Info_متن(info, keyi),
                    darray_سائيز(groupi->سطحون));

            darray_foreach_from(leveli, groupi->سطحون, type->انگ_سطحون)
                clear_level(leveli);
        }
        darray_resize0(groupi->سطحون, type->انگ_سطحون);

        ڪنجي->groups[i].explicit_type = explicit_type;
        ڪنجي->groups[i].type = type;
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
        darray_foreach(leveli, groupi->سطحون) {
            بدل (leveli->انگ_syms) {
            حالة 0:
                leveli->مٿانهون = XKB_ڪنجي_No_نشان;
                اكسر;
            حالة 1:
                leveli->مٿانهون = xkb_keysym_منزل_مٿانهون(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_مٿانهون = false;
                لكل (xkb_keysym_ڳڻپ_t k = 0; k < leveli->انگ_syms; k++) {
                    ثابت xkb_keysym_t مٿانهون =
                        xkb_keysym_منزل_مٿانهون(leveli->s.syms[k]);
                    إذا (مٿانهون != leveli->s.syms[k]) {
                        leveli->has_مٿانهون = true;
                        اكسر;
                    }
                }
                إذا (leveli->has_مٿانهون) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (سائيز_t) 2 * leveli->انگ_syms *
                                             حجم(*leveli->s.syms));
                    إذا (!leveli->s.syms)
                        أرجع false; /* FIXME: better handling? */
                    لكل (xkb_keysym_ڳڻپ_t k = 0; k < leveli->انگ_syms; k++) {
                        leveli->s.syms[leveli->انگ_syms + k] =
                            xkb_keysym_منزل_مٿانهون(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->سطحون, &ڪنجي->groups[i].سطحون, عدم);
        إذا (ڪنجي->groups[i].type->انگ_سطحون > 1 ||
            ڪنجي->groups[i].سطحون[0].انگ_syms > 0) {
            ڪنجي->groups[i].explicit_symbols = true;
            ڪنجي->explicit |= EXPLICIT_SYMBOLS;
        }
        إذا (groupi->defined & گروهه_ميدان_ACTS) { // FIXME
            ڪنجي->groups[i].explicit_actions = true;
            ڪنجي->explicit |= EXPLICIT_INTERP;
        }
        إذا (ڪنجي->groups[i].explicit_type)
            ڪنجي->explicit |= EXPLICIT_TYPES;
    }

    ڪنجي->ٻاهريون_جو_range_pending_گروهه = keyi->ٻاهريون_جو_range_pending_گروهه;
    ڪنجي->ٻاهريون_جو_range_گروهه_عدد = keyi->ٻاهريون_جو_range_گروهه_عدد;
    ڪنجي->ٻاهريون_جو_range_گروهه_policy = keyi->ٻاهريون_جو_range_گروهه_policy;

ڪنجي_fields:
    إذا (keyi->defined & ڪنجي_ميدان_VMODMAP) {
        ڪنجي->vmodmap = keyi->vmodmap;
        ڪنجي->explicit |= EXPLICIT_VMODMAP;
    }

    إذا (keyi->repeat != ڪنجي_REPEAT_UNDEFINED) {
        ڪنجي->repeats = (keyi->repeat == ڪنجي_REPEAT_YES);
        ڪنجي->explicit |= EXPLICIT_REPEAT;
    }

    إذا ((keyi->defined & ڪنجي_ميدان_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ڪنجي->overlays = keyi->overlays;
        إذا (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_نقاب_t بچيل = ڪنجي->overlays;
            ثابت بنية xkb_ڪنجي **overlays_ڪنجيون = keyi->overlays_ڪنجيون;
            طالما (بچيل) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بچيل &
                    (xkb_overlay_نقاب_t)((بنانشان)~بچيل + 1u)
                );
                بچيل = (بچيل & ~lsb);
                إذا (*overlays_ڪنجيون) {
                    overlays_ڪنجيون++;
                } وإلا {
                    /* drop current null value (overlayN=none) */
                    ڪنجي->overlays &= ~lsb;
                }
            }

            إذا (ڪنجي->overlays) {
                /* Steal */
                ڪنجي->overlays_ڪنجيون = keyi->overlays_ڪنجيون;
                keyi->overlays_ڪنجيون = عدم;
                keyi->overlays_alloc = 0;

                ڪنجي->overlays_inline = false;
                ڪنجي->explicit |= EXPLICIT_OVERLAY;
            }
        } وإلا {
            ڪنجي->overlay_ڪنجي = keyi->overlay_ڪنجي;
            ڪنجي->overlays_inline = true;
            ڪنجي->explicit |= EXPLICIT_OVERLAY;
        }
    }

    أرجع true;
}

ساكن bool
Copy_Mod_Map_Def_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_داخلا *داخلا)
{
    بنية xkb_ڪنجي *ڪنجي;

    إذا (!داخلا->have_نشان) {
        ڪنجي = Xkb_ڪنجي_By_Name(keymap, داخلا->u.ڪنجي_Name, true);
        إذا (!ڪنجي) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ڪنجي_Name_متن(info->ctx, داخلا->u.ڪنجي_Name),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, داخلا->mods));
            داخلا->mods = 0; /* disable */
            أرجع false;
        }
    }
    وإلا {
        ڪنجي = ڳوليو_ڪنجي_For_نشان(keymap, داخلا->u.ڪنجي_Sym);
        إذا (!ڪنجي) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_نشان,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_متن(info->ctx, داخلا->u.ڪنجي_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, داخلا->mods));
            داخلا->mods = 0; /* disable */
            أرجع false;
        }
    }

    /* Skip modMap None */
    إذا (داخلا->mods) {
        ڪنجي->modmap |= داخلا->mods;
        داخلا->ڪنجي_ڪوڊ = ڪنجي->keycode;
    }

    أرجع true;
}

ساكن bool
Copy_Symbols_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    ڪنجي_Info *keyi;

    keymap->symbols_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->انگ_گروهه_نالا = (xkb_layout_اشاريو_t)darray_سائيز(info->گروهه_نالا);
    darray_steal(info->گروهه_نالا, &keymap->گروهه_نالا, عدم);

    darray_foreach(keyi, info->ڪنجيون)
        إذا (!Copy_Symbols_Def_منزل_Keymap(keymap, info, keyi))
            info->error_ڳڻپ++;

    إذا (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_ڪنجي *ڪنجي;

        xkb_ڪنجيون_foreach(ڪنجي, keymap) {
            إذا (ڪنجي->name == XKB_ATOM_NONE)
                استمر;

            إذا (ڪنجي->انگ_groups < 1)
                log_info(info->ctx, XKB_LOG_پيغام_NO_ID,
                         "No symbols defined for %s\n",
                         ڪنجي_Name_متن(info->ctx, ڪنجي->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_داخلا *mm;
    darray_foreach(mm, info->modmaps)
        إذا (!Copy_Mod_Map_Def_منزل_Keymap(keymap, info, mm))
            info->error_ڳڻپ++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->انگ_modmaps);

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

    إذا (info.error_ڳڻپ != 0)
        اذهب err_info;

    إذا (!Copy_Symbols_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    أرجع true;

err_info:
    ClearSymbolsInfo(&info);
    أرجع false;
}
