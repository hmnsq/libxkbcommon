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

تعداد کلی_repeat {
    کلی_REPEAT_UNDEFINED = 0,
    کلی_REPEAT_YES,
    کلی_REPEAT_NO,
    کلی_REPEAT_عدد_ENTRIES,
};
تعداد { کلی_REPEAT_MIN_پلنوالی = 3 /* 2 bits + sign */ };
static_assert(کلی_REPEAT_عدد_ENTRIES - 1 <=
              (1 << (کلی_REPEAT_MIN_پلنوالی - 1)) - 1,
              "Cannot encode key repeat");

تعداد ډله_ډګر {
    ډله_ډګر_SYMS = (1 << 0),
    ډله_ډګر_ACTS = (1 << 1),
    ډله_ډګر_TYPE = (1 << 2),
};

تعداد کلی_ډګر {
    کلی_ډګر_NONE = 0,
    کلی_ډګر_REPEAT = (1 << 0),
    کلی_ډګر_DEFAULT_TYPE = (1 << 1),
    کلی_ډګر_GROUPINFO = (1 << 2),
    کلی_ډګر_VMODMAP = (1 << 3),
    کلی_ډګر_OVERLAY = (1 << 4),
    کلی_ډګر_ALL = کلی_ډګر_REPEAT | کلی_ډګر_DEFAULT_TYPE
                  | کلی_ډګر_GROUPINFO | کلی_ډګر_VMODMAP | کلی_ډګر_OVERLAY,
};
تعداد { کلی_ډګر_MIN_پلنوالی = 6 /* 5 bits + sign */ };
static_assert(کلی_ډګر_ALL <= (1 << (کلی_ډګر_MIN_پلنوالی - 1)) - 1,
              "Cannot encode key field");

تعريفنوع بنية {
    darray(بنية xkb_level) پوړونه;
    تعداد ډله_ډګر defined;
    xkb_atom_t type;
} ډله_Info_2;

تعداد {
    XKB_LAYOUT_بهر_د_RANGE_POLICY_MIN_پلنوالی = 4 /* 3 bits + sign */,
    KEYINFO_بهر_د_RANGE_ډله_POLICY_بټ_ډګر = 8,
    KEYINFO_DEFINED_بټ_ډګر = 16,
    KEYINFO_یوځای_بټ_ډګر = 8,
    KEYINFO_REPEAT_بټ_ډګر = 8,
};
static_assert(
    XKB_LAYOUT_بهر_د_RANGE_WRAP < XKB_LAYOUT_بهر_د_RANGE_CLAMP &&
    XKB_LAYOUT_بهر_د_RANGE_CLAMP < XKB_LAYOUT_بهر_د_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_بهر_د_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_بهر_د_RANGE_POLICY_MIN_پلنوالی) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_بهر_د_RANGE_ډله_POLICY_بټ_ډګر >=
              XKB_LAYOUT_بهر_د_RANGE_POLICY_MIN_پلنوالی, "");
static_assert((بینښه)KEYINFO_DEFINED_بټ_ډګر >=
              (بینښه)کلی_ډګر_MIN_پلنوالی, "Cannot encode KeyInfo field");
static_assert((بینښه)KEYINFO_یوځای_بټ_ډګر >=
              (بینښه)یوځای_ډېرپېښ_MIN_پلنوالی, "Cannot encode KeyInfo field");
static_assert((بینښه)KEYINFO_REPEAT_بټ_ډګر >=
              (بینښه)کلی_REPEAT_MIN_پلنوالی, "Cannot encode KeyInfo field");

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_ماسک_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_شاخص_t بهر_د_range_ډله_شمېره;
    darray(ډله_Info_2) groups;
    تعداد xkb_layout_بهر_د_range_policy
        بهر_د_range_ډله_policy:KEYINFO_بهر_د_RANGE_ډله_POLICY_بټ_ډګر;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد کلی_ډګر defined:KEYINFO_DEFINED_بټ_ډګر;
    تعداد یوځای_ډېرپېښ یوځای:KEYINFO_یوځای_بټ_ډګر;
    تعداد کلی_repeat repeat:KEYINFO_REPEAT_بټ_ډګر;

    bool بهر_د_range_pending_ډله:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_شاخص_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_ماسک_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ثابت بنية xkb_کلی *overlay_کلی;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_کلی **overlays_کلیګانې;
    };
} کلی_Info;

ساكن فراغ
StealLevelInfo(بنية xkb_level *into, بنية xkb_level *from)
{
    clear_level(into);
    إذا (from->عدد_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } وإلا {
        into->s.sym = from->s.sym;
    }
    into->عدد_syms = from->عدد_syms;
    from->عدد_syms = 0;

    إذا (from->عدد_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } وإلا {
        into->a.action = from->a.action;
    }
    into->عدد_actions = from->عدد_actions;
    from->عدد_actions = 0;
}

ساكن فراغ
Init_ډله_Info(ډله_Info_2 *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساكن فراغ
Clear_ډله_Info(ډله_Info_2 *groupi)
{
    بنية xkb_level *leveli;
    darray_foreach(leveli, groupi->پوړونه)
        clear_level(leveli);
    darray_free(groupi->پوړونه);
}

ساكن فراغ
Copy_ډله_Info(ډله_Info_2 *منزل, ثابت ډله_Info_2 *from)
{
    منزل->defined = from->defined;
    منزل->type = from->type;
    darray_init(منزل->پوړونه);
    darray_copy(منزل->پوړونه, from->پوړونه);
    لكل (xkb_level_شاخص_t j = 0; j < darray_اندازه(منزل->پوړونه); j++) {
        إذا (darray_توکی(from->پوړونه, j).عدد_syms > 1) {
            darray_توکی(منزل->پوړونه, j).s.syms =
                memdup(darray_توکی(from->پوړونه, j).s.syms,
                       darray_توکی(from->پوړونه, j).عدد_syms,
                       حجم(xkb_keysym_t));
        }
        إذا (darray_توکی(from->پوړونه, j).عدد_actions > 1) {
            darray_توکی(منزل->پوړونه, j).a.actions =
                memdup(darray_توکی(from->پوړونه, j).a.actions,
                       darray_توکی(from->پوړونه, j).عدد_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساكن فراغ
Init_کلی_Info(بنية xkb_context *ctx, کلی_Info *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_کلی_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_کلی_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->بهر_د_range_ډله_policy = XKB_LAYOUT_بهر_د_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_کلی = عدم;
}

ساكن فراغ
Clear_کلی_Info(کلی_Info *keyi)
{
    ډله_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_ډله_Info(groupi);
    darray_free(keyi->groups);
    إذا (keyi->overlays_alloc)
        حرر(keyi->overlays_کلیګانې);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح error_شمېر;
    بینښه صحيح include_ژوروالی;
    xkb_layout_شاخص_t explicit_ډله;
    xkb_layout_شاخص_t max_groups;
    darray(کلی_Info) کلیګانې;
    کلی_Info default_کلی;
    ActionsInfo default_actions;
    darray(xkb_atom_t) ډله_نومونه;
    darray(Mod_Map_ثبت) modmaps;
    بنية xkb_mod_سېټ mods;

    بنية xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت بنية xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساكن فراغ
InitSymbolsInfo(SymbolsInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                بینښه صحيح include_ژوروالی, ثابت بنية xkb_mod_سېټ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_ژوروالی = include_ژوروالی;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_کلی_Info(keymap_info->keymap.ctx, &info->default_کلی);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ژوروالی > 0);
    info->explicit_ډله = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    کلی_Info *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->کلیګانې)
        Clear_کلی_Info(keyi);
    darray_free(info->کلیګانې);
    darray_free(info->ډله_نومونه);
    darray_free(info->modmaps);
    Clear_کلی_Info(&info->default_کلی);
}

ساكن ثابت حرف *
کلی_Info_متن(SymbolsInfo *info, کلی_Info *keyi)
{
    أرجع کلی_Name_متن(info->ctx, keyi->name);
}

ساكن bool
یوځای_Groups(SymbolsInfo *info, ډله_Info_2 *into, ډله_Info_2 *from, bool clobber,
            bool report, xkb_layout_شاخص_t ډله, xkb_atom_t کلی_name)
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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         ډله + 1, کلی_Name_متن(info->ctx, کلی_name),
                         xkb_atom_متن(info->ctx, use),
                         xkb_atom_متن(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & ډله_ډګر_TYPE);

    /* Now look at the levels. */

    إذا (darray_تشه(from->پوړونه)) {
        Init_ډله_Info(from);
        أرجع true;
    }

    إذا (darray_تشه(into->پوړونه)) {
        from->type = into->type;
        *into = *from;
        Init_ډله_Info(from);
        أرجع true;
    }

    /* Merge the actions and syms. */
    ثابت darray_اندازه_t پوړونه_دننه_both =
        MIN(darray_اندازه(into->پوړونه), darray_اندازه(from->پوړونه));
    darray_اندازه_t from_Keysyms_شمېر = 0;
    darray_اندازه_t from_Actions_شمېر = 0;
    لكل (darray_اندازه_t i = 0; i < پوړونه_دننه_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_توکی(into->پوړونه, i);
        بنية xkb_level* ثابت fromLevel = &darray_توکی(from->پوړونه, i);

        ثابت bool fromHasNoKeysym = fromLevel->عدد_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->عدد_actions == 0;
        إذا (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            استمر;
        }

        ثابت bool intoHasNoKeysym = intoLevel->عدد_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->عدد_actions == 0;
        إذا (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_شمېر++;
            from_Actions_شمېر++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->عدد_syms > 0 || intoLevel->عدد_actions > 0);
        أكد(fromLevel->عدد_syms > 0 || fromLevel->عدد_actions > 0);

        /* Handle keysyms */
        إذا (!Xkb_پوړونه_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            إذا (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_نښه,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, ډله + 1, کلی_Name_متن(info->ctx, کلی_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            إذا (fromHasNoKeysym) {
                /* No keysym to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined keysym from `from` */
                إذا (unlikely(fromLevel->عدد_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->عدد_syms > 1))
                        حرر(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->عدد_syms = fromLevel->عدد_syms;
                    fromLevel->عدد_syms = 0;
                    from_Keysyms_شمېر++;
                } وإلا إذا (fromLevel->s.sym != XKB_کلی_No_نښه) {
                    /* Single defined keysym */
                    إذا (unlikely(intoLevel->عدد_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->عدد_syms = 1;
                    from_Keysyms_شمېر++;
                }
            } وإلا {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->عدد_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->s.sym == XKB_کلی_No_نښه) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->عدد_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->عدد_syms = fromLevel->عدد_syms;
                        fromLevel->عدد_syms = 0;
                    } وإلا {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->عدد_syms = 1;
                    }
                    from_Keysyms_شمېر++;
                }
            }
        }

        /* Handle actions */
        إذا (!Xkb_پوړونه_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            إذا (report && !(intoHasNoAction || fromHasNoAction)) {
                إذا (unlikely(intoLevel->عدد_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_کلی_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, ډله + 1, کلی_Name_متن(info->ctx, کلی_name),
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

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, ډله + 1,
                                کلی_Name_متن(info->ctx, کلی_name),
                                Action_Type_متن(use->type),
                                Action_Type_متن(ignore->type));
                }
            }
            إذا (fromHasNoAction) {
                /* No action to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined action from `from` */
                إذا (unlikely(fromLevel->عدد_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->عدد_actions > 1))
                        حرر(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->عدد_actions = fromLevel->عدد_actions;
                    fromLevel->عدد_actions = 0;
                    from_Actions_شمېر++;
                } وإلا إذا (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    إذا (unlikely(intoLevel->عدد_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->عدد_actions = 1;
                    from_Actions_شمېر++;
                }
            } وإلا {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->عدد_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->عدد_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->عدد_actions = fromLevel->عدد_actions;
                        fromLevel->عدد_actions = 0;
                    } وإلا {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->عدد_actions = 1;
                    }
                    from_Actions_شمېر++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    بنية xkb_level *level;
    darray_foreach_from(level, from->پوړونه, پوړونه_دننه_both) {
        darray_append(into->پوړونه, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->عدد_syms = 0;
        level->عدد_actions = 0;
        from_Keysyms_شمېر++;
        from_Actions_شمېر++;
    }
    إذا (from_Keysyms_شمېر) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        إذا (from_Keysyms_شمېر == darray_اندازه(into->پوړونه))
            into->defined &= ~ډله_ډګر_SYMS;
        into->defined |= (from->defined & ډله_ډګر_SYMS);
    }
    إذا (from_Actions_شمېر) {
        /* Reset defined actions field if we used no action from `into` */
        إذا (from_Actions_شمېر == darray_اندازه(into->پوړونه))
            into->defined &= ~ډله_ډګر_ACTS;
        into->defined |= (from->defined & ډله_ډګر_ACTS);
    }

    أرجع true;
}

ساكن bool
Use_New_کلی_ډګر(تعداد کلی_ډګر ډګر_2, تعداد کلی_ډګر old, تعداد کلی_ډګر new,
               bool clobber, bool report, تعداد کلی_ډګر *collide)
{
    إذا (!(old & ډګر_2))
        أرجع (new & ډګر_2);

    إذا (new & ډګر_2) {
        إذا (report)
            *collide |= ډګر_2;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
overlays_get(ثابت کلی_Info *info, xkb_overlay_شاخص_t بټ,
             ثابت بنية xkb_کلی **کلی_بهر)
{
    إذا (بټ >= (xkb_overlay_شاخص_t)(حجم(xkb_overlay_ماسک_t) * CHAR_BIT))
        أرجع false;
    ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << بټ);
    إذا (!(info->overlays & ماسک)) أرجع false;
    إذا (کلی_بهر) {
        إذا (!info->overlays_alloc) {
            /* Stored inline */
            *کلی_بهر = info->overlay_کلی;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
                info->overlays & (ماسک - 1u)
            );
            ثابت xkb_overlay_شاخص_t شاخص = (xkb_overlay_شاخص_t)popcount32(ټیټ);
            *کلی_بهر = info->overlays_کلیګانې[شاخص];
        }
    }
    أرجع true;
}

ساكن bool
overlays_ورننباسه(کلی_Info *keyi, xkb_overlay_شاخص_t بټ,
                ثابت بنية xkb_کلی *کلی)
{
    إذا (بټ >= (xkb_overlay_شاخص_t)(حجم(xkb_overlay_ماسک_t) * CHAR_BIT))
        أرجع false;

    ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << بټ);

    إذا ((keyi->overlays & ماسک) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        إذا (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_کلی = کلی;
            إذا (!کلی)
                keyi->overlays_clear = true;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
                keyi->overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_شاخص_t شاخص =
                (xkb_overlay_شاخص_t)popcount32(ټیټ);
            keyi->overlays_کلیګانې[شاخص] = کلی;
        }
        أرجع true;
    }

    /* Insert new element */
    إذا (!keyi->overlays || (keyi->overlays_clear && !کلی)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_کلی = کلی;
        keyi->overlays |= ماسک;
        keyi->overlays_clear = !کلی;
    } وإلا إذا (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_ماسک_t overlays = keyi->overlays | ماسک;
        ثابت xkb_overlay_شاخص_t alloc =
            (xkb_overlay_شاخص_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_کلی ** ثابت tmp = حافظه_صفر_ورکړه(alloc, حجم(*tmp));
        إذا (!tmp) أرجع false;

        /* Copy previous entries to the buffer */
        إذا (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
                overlays & (xkb_overlay_ماسک_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_شاخص_t idx = (xkb_overlay_شاخص_t)popcount32(ټیټ);
            tmp[idx] = keyi->overlay_کلی;
        } وإلا {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
            overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
        );
        ثابت xkb_overlay_شاخص_t idx = (xkb_overlay_شاخص_t)popcount32(ټیټ);
        tmp[idx] = کلی;

        keyi->overlays_کلیګانې = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وإلا {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_ماسک_t overlays =
            (xkb_overlay_ماسک_t)(keyi->overlays | ماسک);
        ثابت xkb_overlay_شاخص_t شمېر =
            (xkb_overlay_شاخص_t)popcount32(overlays);
        إذا (شمېر > keyi->overlays_alloc) {
            ثابت xkb_overlay_شاخص_t alloc =
                (xkb_overlay_شاخص_t)بل_pow2(شمېر);
            أكد(alloc >= شمېر);
            ثابت بنية xkb_کلی ** ثابت tmp = realloc(
                keyi->overlays_کلیګانې,
                (اندازه_t)alloc * حجم(*keyi->overlays_کلیګانې)
            );
            إذا (!tmp) أرجع false;
            keyi->overlays_کلیګانې = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
            overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
        );
        ثابت xkb_overlay_شاخص_t شاخص = (xkb_overlay_شاخص_t)popcount32(ټیټ);
        إذا (شاخص >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        إذا (شاخص < شمېر - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(شاخص + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_کلیګانې + شاخص + 1, keyi->overlays_کلیګانې + شاخص,
                    (اندازه_t)(شمېر - 1u - شاخص) * حجم(ثابت بنية xkb_کلی *));
        }
        keyi->overlays_کلیګانې[شاخص] = کلی;
        keyi->overlays = overlays;
    }

    أرجع true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
یوځای_overlays(SymbolsInfo *info, کلی_Info *into, کلی_Info *from,
               bool clobber, bool report, تعداد کلی_ډګر *collide)
{
    إذا (!(from->defined & کلی_ډګر_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وإلا إذا (!(into->defined & کلی_ډګر_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_کلیګانې = from->overlays_کلیګانې;
        from->overlays_کلیګانې = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= کلی_ډګر_OVERLAY;
    } وإلا إذا (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وإلا {
        إذا (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_ماسک_t پایله_ماسک =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_شاخص_t شمېر =
                (xkb_overlay_شاخص_t)popcount32(پایله_ماسک);

            إذا (شمېر == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            کلی_Info *dest = into;
            کلی_Info *src = from;
            إذا (into->overlays_alloc >= شمېر &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وإلا إذا (from->overlays_alloc >= شمېر) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وإلا إذا (شمېر > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                إذا (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                إذا (!dest->overlays_alloc) {
                    ثابت بنية xkb_کلی **tmp = حافظه_صفر_ورکړه(شمېر, حجم(*tmp));
                    إذا (!tmp)
                        أرجع false;
                    إذا (dest->overlay_کلی)
                        tmp[0] = dest->overlay_کلی;
                    dest->overlays_کلیګانې = tmp;
                    dest->overlays_alloc = شمېر;
                } وإلا {
                    ثابت بنية xkb_کلی **tmp =
                        realloc(dest->overlays_کلیګانې,
                                شمېر * حجم(*dest->overlays_کلیګانې));
                    إذا (!tmp)
                        أرجع false;
                    dest->overlays_کلیګانې = tmp;
                    dest->overlays_alloc = شمېر;
                }
            }

            xkb_overlay_ماسک_t پاتې_شوی = src->overlays;
            ثابت بنية xkb_کلی **src_کلیګانې = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_کلیګانې
                    : &src->overlay_کلی;
            طالما (پاتې_شوی) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                    پاتې_شوی &
                    (xkb_overlay_ماسک_t)((بینښه)~پاتې_شوی + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_شاخص_t بټ =
                    (xkb_overlay_شاخص_t)popcount32(lsb - 1u);
                پاتې_شوی = (پاتې_شوی & ~lsb);

                /* pop current value */
                إذا (!src->overlays_clear && !src->overlays_alloc && پاتې_شوی)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_کلی *src_کلی = (src_کلیګانې)
                    ? *(src_کلیګانې++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_کلی *dest_کلی = عدم;
                ثابت bool conflict = overlays_get(dest, بټ, &dest_کلی);
                إذا (conflict) {
                    إذا (dest_کلی == src_کلی)
                       استمر;
                    وإلا إذا (report)
                        *collide |= کلی_ډګر_OVERLAY;
                }

                إذا ((!conflict || clobber) && !overlays_ورننباسه(dest, بټ, src_کلی))
                    أرجع false;
            }

            إذا (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                إذا (into->overlays_alloc)
                    حرر(into->overlays_کلیګانې);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                إذا (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_کلیګانې = from->overlays_کلیګانې;
                    from->overlays_کلیګانې = عدم;
                    from->overlays_alloc = 0;
                } وإلا {
                    into->overlay_کلی = from->overlay_کلی;
                }
            }
        } وإلا {
            /* Disjoint overlays */

            إذا (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_کلی == from->overlay_کلی) {
                /* Indentical: do nothing */
                أرجع true;
            }
            إذا (!(into->overlays & from->overlays)) {
                إذا (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_کلی = from->overlay_کلی;
                    أرجع true;
                } وإلا إذا (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    أرجع true;
                }
            }
            /* Conflict */
            إذا (report)
                *collide |= کلی_ډګر_OVERLAY;
            إذا (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_کلی = from->overlay_کلی;
            }
        }
    }
    أرجع true;
}

ساكن bool
یوځای_کلیګانې(SymbolsInfo *info, کلی_Info *into, کلی_Info *from, bool same_file)
{
    xkb_layout_شاخص_t i;
    xkb_layout_شاخص_t groups_دننه_both;
    تعداد کلی_ډګر collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->یوځای != یوځای_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (from->یوځای == یوځای_REPLACE) {
        Clear_کلی_Info(into);
        *into = *from;
        Init_کلی_Info(info->ctx, from);
        أرجع true;
    }

    groups_دننه_both = MIN(darray_اندازه(into->groups), darray_اندازه(from->groups));
    لكل (i = 0; i < groups_دننه_both; i++)
        یوځای_Groups(info,
                    &darray_توکی(into->groups, i),
                    &darray_توکی(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    لكل (i = groups_دننه_both; i < darray_اندازه(from->groups); i++) {
        darray_append(into->groups, darray_توکی(from->groups, i));
        Init_ډله_Info(&darray_توکی(from->groups, i));
    }

    إذا (Use_New_کلی_ډګر(کلی_ډګر_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= کلی_ډګر_VMODMAP;
    }
    إذا (Use_New_کلی_ډګر(کلی_ډګر_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= کلی_ډګر_REPEAT;
    }
    إذا (Use_New_کلی_ډګر(کلی_ډګر_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= کلی_ډګر_DEFAULT_TYPE;
    }
    إذا (Use_New_کلی_ډګر(کلی_ډګر_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->بهر_د_range_pending_ډله = from->بهر_د_range_pending_ډله;
        into->بهر_د_range_ډله_policy = from->بهر_د_range_ډله_policy;
        into->بهر_د_range_ډله_شمېره = from->بهر_د_range_ډله_شمېره;
        into->defined |= کلی_ډګر_GROUPINFO;
    }

    إذا (!یوځای_overlays(info, into, from, clobber, report, &collide))
        أرجع false;

    إذا (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 کلی_Name_متن(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_کلی_Info(from);
    Init_کلی_Info(info->ctx, from);
    أرجع true;
}

ساكن xkb_atom_t
Xkb_Resolve_کلی_Alias(ثابت بنية xkb_keymap *keymap, xkb_atom_t name)
{
    إذا (name < keymap->عدد_کلی_نومونه) {
        ثابت Keycode_سمون سمون = keymap->کلی_نومونه[name];
        إذا (سمون.وموندل && سمون.is_alias) {
            أرجع سمون.alias.real;
        }
    }
    أرجع name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
جمع_کړه_کلی_Symbols(SymbolsInfo *info, کلی_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_کلی_Alias(&info->keymap_info->keymap, keyi->name);

    کلی_Info *iter;
    darray_foreach(iter, info->کلیګانې)
        إذا (iter->name == keyi->name)
            أرجع یوځای_کلیګانې(info, iter, keyi, same_file);

    darray_append(info->کلیګانې, *keyi);
    Init_کلی_Info(info->ctx, keyi);
    أرجع true;
}

ساكن bool
جمع_کړه_Mod_Map_ثبت(SymbolsInfo *info, ثابت Mod_Map_ثبت *new)
{
    ثابت bool clobber = (new->یوځای != یوځای_AUGMENT);

    Mod_Map_ثبت *old;
    darray_foreach(old, info->modmaps) {
        إذا ((new->have_نښه != old->have_نښه) ||
            (new->have_نښه && new->u.کلی_Sym != old->u.کلی_Sym) ||
            (!new->have_نښه && new->u.کلی_Name != old->u.کلی_Name))
            استمر;

        إذا (new->mods == old->mods)
            أرجع true;

        ثابت xkb_mod_ماسک_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_ماسک_t ignore = (clobber ? old->mods : new->mods);

        إذا (new->have_نښه) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_متن(info->ctx, new->u.کلی_Sym),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        } وإلا {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    کلی_Name_متن(info->ctx, new->u.کلی_Name),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        أرجع true;
    }

    darray_append(info->modmaps, *new);

    أرجع true;
}

/***====================================================================***/

ساكن فراغ
یوځای_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد یوځای_ډېرپېښ یوځای)
{
    xkb_atom_t *ډله_name;
    xkb_layout_شاخص_t ډله_نومونه_دننه_both;

    إذا (from->error_شمېر > 0) {
        into->error_شمېر += from->error_شمېر;
        أرجع;
    }

    یوځای_Mod_Sets(into->ctx, &into->mods, &from->mods, یوځای);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    ډله_نومونه_دننه_both = MIN(darray_اندازه(into->ډله_نومونه),
                              darray_اندازه(from->ډله_نومونه));
    لكل (xkb_layout_شاخص_t i = 0; i < ډله_نومونه_دننه_both; i++) {
        إذا (!darray_توکی(from->ډله_نومونه, i))
            استمر;

        إذا (یوځای == یوځای_AUGMENT && darray_توکی(into->ډله_نومونه, i))
            استمر;

        darray_توکی(into->ډله_نومونه, i) = darray_توکی(from->ډله_نومونه, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(ډله_name, from->ډله_نومونه, ډله_نومونه_دننه_both)
        darray_append(into->ډله_نومونه, *ډله_name);

    إذا (darray_تشه(into->کلیګانې)) {
        into->کلیګانې = from->کلیګانې;
        darray_init(from->کلیګانې);
    }
    وإلا {
        کلی_Info *keyi;
        darray_foreach(keyi, from->کلیګانې) {
            keyi->یوځای = یوځای;
            إذا (!جمع_کړه_کلی_Symbols(into, keyi, false))
                into->error_شمېر++;
        }
    }

    إذا (darray_تشه(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وإلا {
        Mod_Map_ثبت *mm;
        darray_foreach(mm, from->modmaps) {
            mm->یوځای = یوځای;
            إذا (!جمع_کړه_Mod_Map_ثبت(into, mm))
                into->error_شمېر++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    إذا (Exceeds_Include_Max_ژوروالی(info->ctx, info->include_ژوروالی)) {
        info->error_شمېر += 10;
        أرجع false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_ژوروالی + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بل_incl) {
        SymbolsInfo بل_incl;
        XkbFile *file;

        حرف لار[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  لار, حجم(لار));
        إذا (!file) {
            info->error_شمېر += 10;
            ClearSymbolsInfo(&included);
            أرجع false;
        }

        InitSymbolsInfo(&بل_incl, info->keymap_info, info->include_ژوروالی + 1,
                        &included.mods);
        إذا (stmt->modifier) {
            ثابت صحيح شمېر = parse_dec_منزل_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &بل_incl.explicit_ډله);
            إذا (شمېر <= 0 || *(stmt->modifier + شمېر) != '\0' ||
                بل_incl.explicit_ډله == 0 ||
                بل_incl.explicit_ډله > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                بل_incl.explicit_ډله = info->explicit_ډله;
            } وإلا {
                بل_incl.explicit_ډله--;
            }
        }
        وإلا إذا (info->keymap_info->keymap.عدد_groups != 0 &&
                 بل_incl.include_ژوروالی == 1) {
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
            بل_incl.explicit_ډله = 0;
        }
        وإلا {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            بل_incl.explicit_ډله = info->explicit_ډله;
        }

        HandleSymbolsFile(&بل_incl, file);

        یوځای_Included_Symbols(&included, &بل_incl, stmt->یوځای);

        ClearSymbolsInfo(&بل_incl);
        FreeXkbFile(file);
    }

    یوځای_Included_Symbols(info, &included, include->یوځای);
    ClearSymbolsInfo(&included);

    أرجع (info->error_شمېر == 0);
}

ساكن bool
Get_ډله_شاخص(SymbolsInfo *info, کلی_Info *keyi, بیان_Def *arrayNdx,
              تعداد ډله_ډګر ډګر_2, xkb_layout_شاخص_t *ndx_rtrn)
{
    أكد (ډګر_2 == ډله_ډګر_SYMS || ډګر_2 == ډله_ډګر_ACTS);
    ثابت حرف *name = (ډګر_2 == ډله_ډګر_SYMS ? "symbols" : "actions");

    إذا (arrayNdx == عدم) {
        xkb_layout_شاخص_t i = 0;
        ډله_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            إذا (!(groupi->defined & ډګر_2)) {
                *ndx_rtrn = i;
                أرجع true;
            }
        }

        إذا (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, کلی_Info_متن(info, keyi), info->max_groups, name);
            أرجع false;
        }

        darray_resize0(keyi->groups, darray_اندازه(keyi->groups) + 1);
        *ndx_rtrn = darray_اندازه(keyi->groups) - 1;
        أرجع true;
    }

    إذا (بیان_Resolve_ډله(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, کلی_Info_متن(info, keyi));
        أرجع false;
    }

    (*ndx_rtrn)--;
    إذا (*ndx_rtrn >= darray_اندازه(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    أرجع true;
}

ساكن bool
جمع_کړه_Symbols_منزل_کلی(SymbolsInfo *info, کلی_Info *keyi, بیان_Def *arrayNdx,
                بیان_Def *ارزښت)
{
    xkb_layout_شاخص_t ndx = 0;

    إذا (!Get_ډله_شاخص(info, keyi, arrayNdx, ډله_ډګر_SYMS, &ndx))
        أرجع false;

    ډله_Info_2 *groupi = &darray_توکی(keyi->groups, ndx);

    إذا (ارزښت->common.type == STMT_بیان_تشه_LIST) {
        groupi->defined |= ډله_ډګر_SYMS;
        أرجع true;
    }

    إذا (ارزښت->common.type != STMT_بیان_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_منزل_string(ارزښت->common.type), ndx + 1,
                کلی_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & ډله_ډګر_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_کلی_SYMBOLS_ثبت,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                کلی_Info_متن(info, keyi), ndx + 1);
        أرجع false;
    }

    xkb_level_شاخص_t n_پوړونه = 0;
    xkb_level_شاخص_t non_تشه_پوړونه = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (بیان_Keysym_List *keysymList = (بیان_Keysym_List *) ارزښت;
         keysymList;
         keysymList = (بیان_Keysym_List *) keysymList->common.بل) {
        n_پوړونه++;
        /* Drop trailing NoSymbol */
        إذا (darray_اندازه(keysymList->syms) > 0)
            non_تشه_پوړونه = n_پوړونه;
    }
    إذا (non_تشه_پوړونه < n_پوړونه)
        n_پوړونه = non_تشه_پوړونه;
    إذا (darray_اندازه(groupi->پوړونه) < n_پوړونه)
        darray_resize0(groupi->پوړونه, n_پوړونه);

    groupi->defined |= ډله_ډګر_SYMS;

    xkb_level_شاخص_t level = 0;
    لكل (بیان_Keysym_List *keysymList = (بیان_Keysym_List *) ارزښت;
         keysymList && level < n_پوړونه;
         keysymList = (بیان_Keysym_List *) keysymList->common.بل, level++) {
        بنية xkb_level *leveli = &darray_توکی(groupi->پوړونه, level);
        أكد(leveli->عدد_syms == 0);

        إذا (unlikely(darray_اندازه(keysymList->syms) > MAX_KEYSYMS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلی_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_فی_LEVEL, darray_اندازه(keysymList->syms));
            أرجع false;
        }

        leveli->عدد_syms = (xkb_keysym_شمېر_t) darray_اندازه(keysymList->syms);
        بدل (leveli->عدد_syms) {
        حالة 0:
            leveli->s.sym = XKB_کلی_No_نښه;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_توکی(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_کلی_No_نښه);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#که_نه_تعریف_شوی NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_شمېر_t k = 0; k < leveli->عدد_syms; k++)
                أكد(leveli->s.syms[k] != XKB_کلی_No_نښه);
#د_که_پای
        }
    }

    أرجع true;
}

ساكن bool
جمع_کړه_Actions_منزل_کلی(SymbolsInfo *info, کلی_Info *keyi, بیان_Def *arrayNdx,
                بیان_Def *ارزښت)
{
    xkb_layout_شاخص_t ndx = 0;

    إذا (!Get_ډله_شاخص(info, keyi, arrayNdx, ډله_ډګر_ACTS, &ndx))
        أرجع false;

    ډله_Info_2 *groupi = &darray_توکی(keyi->groups, ndx);

    إذا (ارزښت->common.type == STMT_بیان_تشه_LIST) {
        groupi->defined |= ډله_ډګر_ACTS;
        أرجع true;
    }

    إذا (ارزښت->common.type != STMT_بیان_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_افاده_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 ارزښت->common.type, ndx, کلی_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & ډله_ډګر_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_کلی_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 کلی_Info_متن(info, keyi), ndx);
        أرجع false;
    }

    xkb_level_شاخص_t n_پوړونه = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &ارزښت->common; p; p = p->بل)
        n_پوړونه++;
    إذا (darray_اندازه(groupi->پوړونه) < n_پوړونه)
        darray_resize0(groupi->پوړونه, n_پوړونه);

    groupi->defined |= ډله_ډګر_ACTS;

    xkb_level_شاخص_t level = 0;
    xkb_level_شاخص_t non_تشه_پوړونه = 0;
    لكل (بیان_Action_List *actionList = (بیان_Action_List *) ارزښت;
         actionList;
         actionList = (بیان_Action_List *) actionList->common.بل, level++) {
        بنية xkb_level *leveli = &darray_توکی(groupi->پوړونه, level);
        أكد(leveli->عدد_actions == 0);

        بینښه صحيح عدد_actions = 0;
        لكل (بیان_Def *act = actionList->actions;
             act; act = (بیان_Def *) act->common.بل)
            عدد_actions++;

        إذا (unlikely(عدد_actions > MAX_ACTIONS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلی_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_فی_LEVEL, عدد_actions);
            أرجع false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (بیان_Def *act = actionList->actions;
             act; act = (بیان_Def *) act->common.بل) {
            اتحاد xkb_action منزل_Act = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &منزل_Act);
            إذا (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_ارزښت,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        کلی_Info_متن(info, keyi), ndx + 1, level + 1);
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
            } وإلا إذا (likely(عدد_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->عدد_actions = 1;
                leveli->a.action = منزل_Act;
                اذهب بل_level;
            } وإلا {
                darray_append(actions, منزل_Act);
            }
        }
        إذا (darray_تشه(actions)) {
            leveli->عدد_actions = 0;
        } وإلا إذا (likely(darray_اندازه(actions) > 1)) {
            leveli->عدد_actions = (xkb_action_شمېر_t) darray_اندازه(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#که_نه_تعریف_شوی NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_شمېر_t k = 0; k < leveli->عدد_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#د_که_پای
        } وإلا {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(عدد_actions > darray_اندازه(actions));
            leveli->عدد_actions = 1;
            leveli->a.action = darray_توکی(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
بل_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        إذا (leveli->عدد_actions > 0 || leveli->عدد_syms > 0)
            non_تشه_پوړونه = level + 1;
    }

    إذا (non_تشه_پوړونه < n_پوړونه) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_اندازه(groupi->پوړونه) = non_تشه_پوړونه;
        إذا (non_تشه_پوړونه > 0)
            darray_shrink(groupi->پوړونه);
        وإلا
            darray_free(groupi->پوړونه);
    }

    أرجع true;
}

ساكن ثابت Lookup_ثبت repeatEntries[] = {
    { "true", کلی_REPEAT_YES },
    { "yes", کلی_REPEAT_YES },
    { "on", کلی_REPEAT_YES },
    { "false", کلی_REPEAT_NO },
    { "no", کلی_REPEAT_NO },
    { "off", کلی_REPEAT_NO },
    { "default", کلی_REPEAT_UNDEFINED },
    { عدم, 0 }
};

/** Handle overlay<N> = <key> entry */
ساكن bool
بیان_Resolve_Overlay_ثبت(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *ډګر_2, ثابت بیان_Def *arrayNdx,
                        ثابت بیان_Def *بیان, کلی_Info *keyi,
                        xkb_overlay_شاخص_t *overlay_rtrn,
                        ثابت بنية xkb_کلی **کلی_rtrn)
{
    /* Overlay index */
    إذا (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                ډګر_2, کلی_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    static_assert(حجم(xkb_overlay_شاخص_t) == حجم(uint8_t), "");
    ثابت اندازه_t مختاړی = حجم("overlay") - 1;
    ثابت اندازه_t len = مزي_اوږدوالی(ډګر_2 + مختاړی);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    إذا (parse_dec_منزل_uint64_t(ډګر_2 + مختاړی, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_شاخص,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                ډګر_2, کلی_Name_متن(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        أرجع false;
    }
    *overlay_rtrn = (xkb_overlay_شاخص_t)raw_overlay - 1;

    /* Overlay key */
    بدل (بیان->common.type) {
    حالة STMT_بیان_KEYNAME_LITERAL:
        *کلی_rtrn = Xkb_کلی_By_Name(&keymap_info->keymap, بیان->کلی_name.کلی_name,
                                 false);
        إذا (!*کلی_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_متن(keymap_info->keymap.ctx, بیان->کلی_name.کلی_name),
                    ډګر_2, کلی_Name_متن(keymap_info->keymap.ctx, keyi->name));
            أرجع false;
        }
        أرجع true;
    حالة STMT_بیان_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_متن(keymap_info->keymap.ctx, بیان->ident.ident);
        إذا (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *کلی_rtrn = عدم;
            أرجع true;
        } وإلا إذا (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *کلی_rtrn = عدم;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            أرجع true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ارزښت,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, ډګر_2, کلی_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ارزښت,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_منزل_string(STMT_بیان_KEYNAME_LITERAL),
                ډګر_2, کلی_Name_متن(keymap_info->keymap.ctx, keyi->name),
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;
    }
}

ساكن bool
سېټ_Symbols_ډګر(SymbolsInfo *info, کلی_Info *keyi, ثابت حرف *ډګر_2,
                بیان_Def *arrayNdx, بیان_Def **ارزښت_ptr)
{
    بیان_Def * محدود ثابت ارزښت = *ارزښت_ptr;
    إذا (istreq(ډګر_2, "type")) {
        xkb_layout_شاخص_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        إذا (!بیان_Resolve_String(info->ctx, ارزښت, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            أرجع false;
        }

        إذا (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= کلی_ډګر_DEFAULT_TYPE;
        }
        وإلا إذا (بیان_Resolve_ډله(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }
        وإلا {
            ndx--;
            إذا (ndx >= darray_اندازه(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_اندازه(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_توکی(keyi->groups, ndx).type = val;
            darray_توکی(keyi->groups, ndx).defined |= ډله_ډګر_TYPE;
        }
    }
    وإلا إذا (istreq(ډګر_2, "symbols")) {
        أرجع جمع_کړه_Symbols_منزل_کلی(info, keyi, arrayNdx, ارزښت);
    }
    وإلا إذا (istreq(ډګر_2, "actions")) {
        أرجع جمع_کړه_Actions_منزل_کلی(info, keyi, arrayNdx, ارزښت);
    }
    وإلا إذا (istreq(ډګر_2, "vmods") ||
             istreq(ډګر_2, "virtualmods") ||
             istreq(ډګر_2, "virtualmodifiers")) {
        xkb_mod_ماسک_t ماسک = 0;

        إذا (!بیان_Resolve_Mod_ماسک(info->ctx, ارزښت, MOD_VIRT, &info->mods,
                                &ماسک)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_منزل_string(ارزښت->common.type),
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->vmodmap = ماسک;
        keyi->defined |= کلی_ډګر_VMODMAP;
    }
    وإلا إذا (istreq(ډګر_2, "locking") ||
             istreq(ډګر_2, "lock") ||
             istreq(ډګر_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                کلی_Info_متن(info, keyi));
    }
    وإلا إذا (istreq(ډګر_2, "radiogroup") ||
             istreq(ډګر_2, "permanentradiogroup") ||
             istreq(ډګر_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                کلی_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_مختاړی("permanentoverlay", ډګر_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                کلی_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_مختاړی("overlay", ډګر_2)) {
        xkb_overlay_شاخص_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_کلی *کلی = عدم;
        إذا (!بیان_Resolve_Overlay_ثبت(info->keymap_info, ډګر_2, arrayNdx,
                                     *ارزښت_ptr, keyi, &overlay, &کلی))
            أرجع false;
        إذا (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            أرجع true;
        } وإلا إذا (کلی && کلی->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_پیغام_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, کلی_Info_متن(info, keyi));
        } وإلا {
            ثابت بنية xkb_کلی *prev;
            إذا (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                إذا (کلی != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             کلی_Info_متن(info, keyi), overlay + 1,
                             (prev
                                 ? کلی_Name_متن(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (کلی
                                 ? کلی_Name_متن(info->ctx, کلی->name)
                                 : "none"));
                }
            } وإلا إذا (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                إذا (!overlays_ورننباسه(keyi, overlay, کلی))
                    أرجع false;
                keyi->defined |= کلی_ډګر_OVERLAY;
            } وإلا {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << overlay);
                إذا (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    إذا (کلی) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = ماسک;
                        keyi->overlays_clear = false;
                        keyi->overlay_کلی = کلی;
                    } وإلا {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= ماسک;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= کلی_ډګر_OVERLAY;
                } وإلا إذا (keyi->overlays) {
                    إذا (کلی) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                کلی_Info_متن(info, keyi), keyi->overlays,
                                کلی_Name_متن(info->ctx, keyi->overlay_کلی->name),
                                overlay + 1, کلی_Name_متن(info->ctx, کلی->name));
                        أرجع !(info->keymap_info->strict &
                                 PARSER_NO_ډګر_ارزښت_MISMATCH);
                    } وإلا {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وإلا إذا (istreq(ډګر_2, "repeating") ||
             istreq(ډګر_2, "repeats") ||
             istreq(ډګر_2, "repeat")) {
        uint32_t val = 0;

        إذا (!بیان_Resolve_Enum(info->ctx, ارزښت, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ارزښت,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->repeat = (تعداد کلی_repeat) val;
        keyi->defined |= کلی_ډګر_REPEAT;
    }
    وإلا إذا (istreq(ډګر_2, "groupswrap") ||
             istreq(ډګر_2, "wrapgroups")) {
        bool سېټ = false;

        إذا (!بیان_Resolve_Boolean(info->ctx, ارزښت, &سېټ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ارزښت,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->بهر_د_range_ډله_policy = (سېټ)
            ? XKB_LAYOUT_بهر_د_RANGE_WRAP
            : XKB_LAYOUT_بهر_د_RANGE_CLAMP;
        keyi->defined |= کلی_ډګر_GROUPINFO;
    }
    وإلا إذا (istreq(ډګر_2, "groupsclamp") ||
             istreq(ډګر_2, "clampgroups")) {
        bool سېټ = false;

        إذا (!بیان_Resolve_Boolean(info->ctx, ارزښت, &سېټ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ارزښت,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->بهر_د_range_ډله_policy = (سېټ)
            ? XKB_LAYOUT_بهر_د_RANGE_CLAMP
            : XKB_LAYOUT_بهر_د_RANGE_WRAP;
        keyi->defined |= کلی_ډګر_GROUPINFO;
    }
    وإلا إذا (istreq(ډګر_2, "groupsredirect") ||
             istreq(ډګر_2, "redirectgroups")) {
        xkb_layout_شاخص_t grp = 0;
        bool pending = false;

        // TODO: recover?
        إذا (بیان_Resolve_ډله(info->keymap_info, ارزښت, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    کلی_Info_متن(info, keyi));
            أرجع false;
        }

        إذا (pending) {
            keyi->بهر_د_range_pending_ډله = true;
            ثابت darray_اندازه_t pending_شاخص =
                darray_اندازه(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .بیان = *ارزښت_ptr,
                    .computed = false,
                    .ارزښت = 0,
                }
            );
            *ارزښت_ptr = عدم;
            static_assert(حجم(keyi->بهر_د_range_ډله_شمېره) ==
                          حجم(pending_شاخص),
                          "Cannot save pending computation");
            keyi->بهر_د_range_ډله_شمېره = pending_شاخص;
        } وإلا {
            keyi->بهر_د_range_pending_ډله = false;
            keyi->بهر_د_range_ډله_شمېره = grp - 1;
        }

        keyi->بهر_د_range_ډله_policy = XKB_LAYOUT_بهر_د_RANGE_REDIRECT;
        keyi->defined |= کلی_ډګر_GROUPINFO;
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ډګر,
                "Unknown field \"%s\" in a key; definition ignored\n",
                ډګر_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_کلی_FIELDS);
    }

    أرجع true;
}

ساكن bool
سېټ_ډله_Name(SymbolsInfo *info, بیان_Def *arrayNdx, بیان_Def *ارزښت,
             تعداد یوځای_ډېرپېښ یوځای)
{
    إذا (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_ډله_NAME_شاخص,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        أرجع false;
    }

    xkb_layout_شاخص_t ډله = 0;
    إذا (بیان_Resolve_ډله(info->keymap_info, arrayNdx, false, &ډله, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!بیان_Resolve_String(info->ctx, ارزښت, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", ډله);
        أرجع false;
    }

    xkb_layout_شاخص_t ډله_منزل_use;
    إذا (info->explicit_ډله == XKB_LAYOUT_INVALID) {
        ډله_منزل_use = ډله - 1;
    }
    وإلا إذا (ډله - 1 == 0) {
        ډله_منزل_use = info->explicit_ډله;
    }
    وإلا {
        log_warn(info->ctx, XKB_WARNING_NON_بنسټ_ډله_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, ډله,
                 xkb_atom_متن(info->ctx, name));
        أرجع false;
    }

    إذا (ډله_منزل_use >= darray_اندازه(info->ډله_نومونه)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_اندازه(info->ډله_نومونه) < ډله_منزل_use + 1);
        darray_resize0(info->ډله_نومونه, ډله_منزل_use + 1);
    } وإلا {
        ثابت xkb_atom_t old_name = darray_توکی(info->ډله_نومونه, ډله_منزل_use);
        إذا (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (یوځای != یوځای_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     ډله_منزل_use, info->name,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
            name = use;
        }
    }
    darray_توکی(info->ډله_نومونه, ډله_منزل_use) = name;

    أرجع true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *ډګر_2;
    بیان_Def *arrayNdx;
    bool ret;

    إذا (!بیان_Resolve_Lhs(info->ctx, stmt->name, &elem, &ډګر_2, &arrayNdx))
        أرجع false;

    إذا (elem && istreq(elem, "key")) {
        کلی_Info لنډمهاله = {0};
        Init_کلی_Info(info->ctx, &لنډمهاله);
        /* Do not replace the whole key, only the current field */
        لنډمهاله.یوځای = (لنډمهاله.یوځای == یوځای_REPLACE)
            ? یوځای_OVERRIDE
            : stmt->یوځای;
        ret = سېټ_Symbols_ډګر(info, &لنډمهاله, ډګر_2, arrayNdx, &stmt->ارزښت) &&
              یوځای_کلیګانې(info, &info->default_کلی, &لنډمهاله, true);
        Clear_کلی_Info(&لنډمهاله);
    }
    وإلا إذا (!elem && (istreq(ډګر_2, "name") ||
                       istreq(ډګر_2, "groupname"))) {
        ret = سېټ_ډله_Name(info, arrayNdx, stmt->ارزښت, stmt->یوځای);
    }
    وإلا إذا (!elem && (istreq(ډګر_2, "groupswrap") ||
                       istreq(ډګر_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(ډګر_2, "groupsclamp") ||
                       istreq(ډګر_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(ډګر_2, "groupsredirect") ||
                       istreq(ډګر_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && istreq(ډګر_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ډګر,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وإلا إذا (elem) {
        ret = (سېټ_Default_Action_ډګر(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ډګر_2, arrayNdx,
                                     &stmt->ارزښت, stmt->یوځای) !=
               PARSER_FATAL_ERROR);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ډګر,
                "Default defined for unknown field \"%s\"; Ignored\n", ډګر_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    أرجع ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, کلی_Info *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.بل) {
        ثابت حرف *ډګر_2;
        بیان_Def *arrayNdx;
        bool ok = true;

        إذا (!def->name) {
            إذا (unlikely(!def->ارزښت) || def->ارزښت->common.type != STMT_بیان_ACTION_LIST)
                ډګر_2 = "symbols"; /* Default to symbols field */
            وإلا
                ډګر_2 = "actions";
            arrayNdx = عدم;
        }
        وإلا {
            ثابت حرف *elem;
            ok = بیان_Resolve_Lhs(info->ctx, def->name, &elem, &ډګر_2,
                                &arrayNdx);
            إذا (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ډګر_2);
                ok = false;
            }
        }

        إذا (unlikely(!def->ارزښت)) {
            log_err(info->ctx, XKB_ERROR_ویش_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", ډګر_2);
            ok = false;
        }

        إذا (!ok || !سېټ_Symbols_ډګر(info, keyi, ډګر_2, arrayNdx, &def->ارزښت))
            all_valid_entries = false;
    }

    أرجع all_valid_entries;
}

ساكن bool
سېټ_Explicit_ډله(SymbolsInfo *info, کلی_Info *keyi)
{
    xkb_layout_شاخص_t i;
    ډله_Info_2 *groupi;
    bool warn = false;

    إذا (info->explicit_ډله == XKB_LAYOUT_INVALID)
        أرجع true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        إذا (groupi->defined) {
            warn = true;
            Clear_ډله_Info(groupi);
            Init_ډله_Info(groupi);
        }
    }

    إذا (warn) {
        log_warn(info->ctx, XKB_WARNING_مضرب_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_ډله + 1, کلی_Info_متن(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_ډله + 1);
    إذا (info->explicit_ډله > 0) {
        darray_توکی(keyi->groups, info->explicit_ډله) =
            darray_توکی(keyi->groups, 0);
        Init_ډله_Info(&darray_توکی(keyi->groups, 0));
    }

    أرجع true;
}

ساكن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    کلی_Info keyi;

    keyi = info->default_کلی;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_کلی.groups);
    لكل (xkb_layout_شاخص_t i = 0; i < darray_اندازه(keyi.groups); i++)
        Copy_ډله_Info(&darray_توکی(keyi.groups, i),
                      &darray_توکی(info->default_کلی.groups, i));
    keyi.یوځای = stmt->یوځای;
    keyi.name = stmt->کلی_Name;

    إذا (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        سېټ_Explicit_ډله(info, &keyi) &&
        جمع_کړه_کلی_Symbols(info, &keyi, true)) {
        أرجع true;
    }

    Clear_کلی_Info(&keyi);
    info->error_شمېر++;
    أرجع false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_ثبت tmp;
    بنية xkb_context *ctx = info->ctx;

    إذا (!بیان_Resolve_Mod_ماسک(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        أرجع false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_متن_V1 ||
           def->modifiers->common.type == STMT_بیان_IDENT);

    bool ok = true;
    tmp.یوځای = def->یوځای;

    لكل (بیان_Def *کلی = def->کلیګانې; کلی; کلی = (بیان_Def *) کلی->common.بل) {
        إذا (کلی->common.type == STMT_بیان_KEYNAME_LITERAL) {
            tmp.have_نښه = false;
            tmp.u.کلی_Name = کلی->کلی_name.کلی_name;
        }
        وإلا إذا (کلی->common.type == STMT_بیان_KEYSYM_LITERAL) {
            إذا (کلی->keysym.keysym == XKB_کلی_No_نښه) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } وإلا {
                tmp.have_نښه = true;
                tmp.u.کلی_Sym = کلی->keysym.keysym;
            }
        }
        وإلا {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ثبت,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = جمع_کړه_Mod_Map_ثبت(info, &tmp) && ok;
    }
    أرجع ok;
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_خوندي(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بل) {
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
            info->error_شمېر++;

        إذا (info->error_شمېر > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    خوندي_map_name(file->name));
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
ساكن بنية xkb_کلی *
پیدا_کړه_کلی_For_نښه(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_ډله;
    xkb_layout_شاخص_t ډله = 0;
    افعل {
        xkb_level_شاخص_t level = 0;
        got_one_ډله = false;
        bool got_one_level;
        افعل {
            got_one_level = false;
            بنية xkb_کلی *کلی;
            xkb_کلیګانې_foreach(کلی, keymap) {
                إذا (ډله < کلی->عدد_groups &&
                    level < Xkb_کلی_عدد_پوړونه(کلی, ډله)) {
                    got_one_ډله = got_one_level = true;
                    ثابت xkb_keysym_شمېر_t عدد_syms =
                        کلی->groups[ډله].پوړونه[level].عدد_syms;
                    إذا (عدد_syms > 1) {
                        لكل (xkb_keysym_شمېر_t k = 0; k < عدد_syms; k++) {
                            إذا (کلی->groups[ډله].پوړونه[level].s.syms[k] == sym)
                                أرجع کلی;
                        }
                    } وإلا إذا (عدد_syms &&
                               کلی->groups[ډله].پوړونه[level].s.sym == sym) {
                        أرجع کلی;
                    }
                }
            }
            level++;
        } طالما (got_one_level);
        ډله++;
    } طالما (got_one_ډله);

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
پیدا_کړه_Automatic_Type(بنية xkb_context *ctx, ډله_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_شاخص_t پلنوالی = darray_اندازه(groupi->پوړونه);

#تعریف_کړه GET_SYM(level) \
    (darray_توکی(groupi->پوړونه, level).عدد_syms == 0 ? \
        XKB_کلی_No_نښه : \
     darray_توکی(groupi->پوړونه, level).عدد_syms == 1 ? \
        darray_توکی(groupi->پوړونه, level).s.sym : \
     /* num_syms > 1 */ \
        darray_توکی(groupi->پوړونه, level).s.syms[0])

    إذا (پلنوالی == 1 || پلنوالی <= 0)
        أرجع xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    إذا (پلنوالی == 2) {
        إذا (xkb_keysym_is_ښکتنی(sym0) && xkb_keysym_is_پاسنی_or_title(sym1))
            أرجع xkb_atom_intern_literal(ctx, "ALPHABETIC");

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    إذا (پلنوالی <= 4) {
        إذا (xkb_keysym_is_ښکتنی(sym0) && xkb_keysym_is_پاسنی_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (پلنوالی == 4 ? GET_SYM(3) : XKB_کلی_No_نښه);

            إذا (xkb_keysym_is_ښکتنی(sym2) && xkb_keysym_is_پاسنی_or_title(sym3))
                أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    أرجع XKB_ATOM_NONE;

#تعریف_لغوه GET_SYM
}

ساكن ثابت بنية xkb_کلی_type *
پیدا_کړه_Type_For_ډله(بنية xkb_keymap *keymap, کلی_Info *keyi,
                 xkb_layout_شاخص_t ډله, bool *explicit_type)
{
    ډله_Info_2 *groupi = &darray_توکی(keyi->groups, ډله);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    إذا (type_name == XKB_ATOM_NONE) {
        إذا (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وإلا {
            type_name = پیدا_کړه_Automatic_Type(keymap->ctx, groupi);
            إذا (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    إذا (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_کلی_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 کلی_Name_متن(keymap->ctx, keyi->name), ډله + 1,
                 darray_اندازه(groupi->پوړونه));
        اذهب use_default;
    }

    darray_اندازه_t i;
    لكل (i = 0; i < keymap->عدد_types; i++)
        إذا (keymap->types[i].name == type_name)
            اكسر;

    إذا (i >= keymap->عدد_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_کلی_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_متن(keymap->ctx, type_name),
                 کلی_Name_متن(keymap->ctx, keyi->name), ډله + 1);
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
                       کلی_Info *keyi)
{
    بنية xkb_کلی *کلی;
    ډله_Info_2 *groupi;
    ثابت ډله_Info_2 *group0;
    xkb_layout_شاخص_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    کلی = Xkb_کلی_By_Name(keymap, keyi->name, false);
    إذا (!کلی) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                کلی_Info_متن(info, keyi));
        أرجع false;
    }

    /* Find the range of groups we need. */
    کلی->عدد_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & کلی_ډګر_DEFAULT_TYPE)
                                    || (groupi->defined & ډله_ډګر_TYPE);
        إذا (darray_اندازه(groupi->پوړونه) > 0 || has_explicit_type)
            کلی->عدد_groups = i + 1;
        إذا (has_explicit_type)
            کلی->explicit |= EXPLICIT_TYPES;
    }

    إذا (کلی->عدد_groups <= 0) {
        /* A key with no group may still have other fields defined */
        إذا (keyi->defined)
            اذهب کلی_fields;
        وإلا
            أرجع false;
    }

    darray_resize(keyi->groups, کلی->عدد_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_توکی(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        إذا (groupi->defined)
            استمر;

        Copy_ډله_Info(groupi, group0);
    }

    کلی->groups = حافظه_صفر_ورکړه(کلی->عدد_groups, حجم(*کلی->groups));
    أكد(darray_اندازه(keyi->groups) == کلی->عدد_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #که_تعریف_شوی __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < کلی->عدد_groups);
        #د_که_پای
        bool explicit_type = false;
        ثابت بنية xkb_کلی_type * ثابت type =
            پیدا_کړه_Type_For_ډله(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        إذا (type->عدد_پوړونه < darray_اندازه(groupi->پوړونه)) {
            بنية xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_متن(keymap->ctx, type->name), type->عدد_پوړونه,
                    کلی_Info_متن(info, keyi),
                    darray_اندازه(groupi->پوړونه));

            darray_foreach_from(leveli, groupi->پوړونه, type->عدد_پوړونه)
                clear_level(leveli);
        }
        darray_resize0(groupi->پوړونه, type->عدد_پوړونه);

        کلی->groups[i].explicit_type = explicit_type;
        کلی->groups[i].type = type;
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
        darray_foreach(leveli, groupi->پوړونه) {
            بدل (leveli->عدد_syms) {
            حالة 0:
                leveli->پاسنی = XKB_کلی_No_نښه;
                اكسر;
            حالة 1:
                leveli->پاسنی = xkb_keysym_منزل_پاسنی(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_پاسنی = false;
                لكل (xkb_keysym_شمېر_t k = 0; k < leveli->عدد_syms; k++) {
                    ثابت xkb_keysym_t پاسنی =
                        xkb_keysym_منزل_پاسنی(leveli->s.syms[k]);
                    إذا (پاسنی != leveli->s.syms[k]) {
                        leveli->has_پاسنی = true;
                        اكسر;
                    }
                }
                إذا (leveli->has_پاسنی) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (اندازه_t) 2 * leveli->عدد_syms *
                                             حجم(*leveli->s.syms));
                    إذا (!leveli->s.syms)
                        أرجع false; /* FIXME: better handling? */
                    لكل (xkb_keysym_شمېر_t k = 0; k < leveli->عدد_syms; k++) {
                        leveli->s.syms[leveli->عدد_syms + k] =
                            xkb_keysym_منزل_پاسنی(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->پوړونه, &کلی->groups[i].پوړونه, عدم);
        إذا (کلی->groups[i].type->عدد_پوړونه > 1 ||
            کلی->groups[i].پوړونه[0].عدد_syms > 0) {
            کلی->groups[i].explicit_symbols = true;
            کلی->explicit |= EXPLICIT_SYMBOLS;
        }
        إذا (groupi->defined & ډله_ډګر_ACTS) { // FIXME
            کلی->groups[i].explicit_actions = true;
            کلی->explicit |= EXPLICIT_INTERP;
        }
        إذا (کلی->groups[i].explicit_type)
            کلی->explicit |= EXPLICIT_TYPES;
    }

    کلی->بهر_د_range_pending_ډله = keyi->بهر_د_range_pending_ډله;
    کلی->بهر_د_range_ډله_شمېره = keyi->بهر_د_range_ډله_شمېره;
    کلی->بهر_د_range_ډله_policy = keyi->بهر_د_range_ډله_policy;

کلی_fields:
    إذا (keyi->defined & کلی_ډګر_VMODMAP) {
        کلی->vmodmap = keyi->vmodmap;
        کلی->explicit |= EXPLICIT_VMODMAP;
    }

    إذا (keyi->repeat != کلی_REPEAT_UNDEFINED) {
        کلی->repeats = (keyi->repeat == کلی_REPEAT_YES);
        کلی->explicit |= EXPLICIT_REPEAT;
    }

    إذا ((keyi->defined & کلی_ډګر_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        کلی->overlays = keyi->overlays;
        إذا (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_ماسک_t پاتې_شوی = کلی->overlays;
            ثابت بنية xkb_کلی **overlays_کلیګانې = keyi->overlays_کلیګانې;
            طالما (پاتې_شوی) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                    پاتې_شوی &
                    (xkb_overlay_ماسک_t)((بینښه)~پاتې_شوی + 1u)
                );
                پاتې_شوی = (پاتې_شوی & ~lsb);
                إذا (*overlays_کلیګانې) {
                    overlays_کلیګانې++;
                } وإلا {
                    /* drop current null value (overlayN=none) */
                    کلی->overlays &= ~lsb;
                }
            }

            إذا (کلی->overlays) {
                /* Steal */
                کلی->overlays_کلیګانې = keyi->overlays_کلیګانې;
                keyi->overlays_کلیګانې = عدم;
                keyi->overlays_alloc = 0;

                کلی->overlays_inline = false;
                کلی->explicit |= EXPLICIT_OVERLAY;
            }
        } وإلا {
            کلی->overlay_کلی = keyi->overlay_کلی;
            کلی->overlays_inline = true;
            کلی->explicit |= EXPLICIT_OVERLAY;
        }
    }

    أرجع true;
}

ساكن bool
Copy_Mod_Map_Def_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_ثبت *ثبت)
{
    بنية xkb_کلی *کلی;

    إذا (!ثبت->have_نښه) {
        کلی = Xkb_کلی_By_Name(keymap, ثبت->u.کلی_Name, true);
        إذا (!کلی) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    کلی_Name_متن(info->ctx, ثبت->u.کلی_Name),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, ثبت->mods));
            ثبت->mods = 0; /* disable */
            أرجع false;
        }
    }
    وإلا {
        کلی = پیدا_کړه_کلی_For_نښه(keymap, ثبت->u.کلی_Sym);
        إذا (!کلی) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_نښه,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_متن(info->ctx, ثبت->u.کلی_Sym),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, ثبت->mods));
            ثبت->mods = 0; /* disable */
            أرجع false;
        }
    }

    /* Skip modMap None */
    إذا (ثبت->mods) {
        کلی->modmap |= ثبت->mods;
        ثبت->کلی_کوډ = کلی->keycode;
    }

    أرجع true;
}

ساكن bool
Copy_Symbols_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    کلی_Info *keyi;

    keymap->symbols_section_name = strdup_خوندي(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->عدد_ډله_نومونه = (xkb_layout_شاخص_t)darray_اندازه(info->ډله_نومونه);
    darray_steal(info->ډله_نومونه, &keymap->ډله_نومونه, عدم);

    darray_foreach(keyi, info->کلیګانې)
        إذا (!Copy_Symbols_Def_منزل_Keymap(keymap, info, keyi))
            info->error_شمېر++;

    إذا (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_کلی *کلی;

        xkb_کلیګانې_foreach(کلی, keymap) {
            إذا (کلی->name == XKB_ATOM_NONE)
                استمر;

            إذا (کلی->عدد_groups < 1)
                log_info(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "No symbols defined for %s\n",
                         کلی_Name_متن(info->ctx, کلی->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_ثبت *mm;
    darray_foreach(mm, info->modmaps)
        إذا (!Copy_Mod_Map_Def_منزل_Keymap(keymap, info, mm))
            info->error_شمېر++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->عدد_modmaps);

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

    إذا (info.error_شمېر != 0)
        اذهب err_info;

    إذا (!Copy_Symbols_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    أرجع true;

err_info:
    ClearSymbolsInfo(&info);
    أرجع false;
}
