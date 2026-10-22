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

تعداد کلید_repeat {
    کلید_REPEAT_UNDEFINED = 0,
    کلید_REPEAT_YES,
    کلید_REPEAT_NO,
    کلید_REPEAT_عدد_ENTRIES,
};
تعداد { کلید_REPEAT_MIN_چوڑائی = 3 /* 2 bits + sign */ };
static_assert(کلید_REPEAT_عدد_ENTRIES - 1 <=
              (1 << (کلید_REPEAT_MIN_چوڑائی - 1)) - 1,
              "Cannot encode key repeat");

تعداد گروہ_میدان {
    گروہ_میدان_SYMS = (1 << 0),
    گروہ_میدان_ACTS = (1 << 1),
    گروہ_میدان_TYPE = (1 << 2),
};

تعداد کلید_میدان {
    کلید_میدان_NONE = 0,
    کلید_میدان_REPEAT = (1 << 0),
    کلید_میدان_DEFAULT_TYPE = (1 << 1),
    کلید_میدان_GROUPINFO = (1 << 2),
    کلید_میدان_VMODMAP = (1 << 3),
    کلید_میدان_OVERLAY = (1 << 4),
    کلید_میدان_ALL = کلید_میدان_REPEAT | کلید_میدان_DEFAULT_TYPE
                  | کلید_میدان_GROUPINFO | کلید_میدان_VMODMAP | کلید_میدان_OVERLAY,
};
تعداد { کلید_میدان_MIN_چوڑائی = 6 /* 5 bits + sign */ };
static_assert(کلید_میدان_ALL <= (1 << (کلید_میدان_MIN_چوڑائی - 1)) - 1,
              "Cannot encode key field");

تعريفنوع بنية {
    darray(بنية xkb_level) پدھراں;
    تعداد گروہ_میدان defined;
    xkb_atom_t type;
} گروہ_Info_2;

تعداد {
    XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_MIN_چوڑائی = 4 /* 3 bits + sign */,
    KEYINFO_باہرلا_دا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان = 8,
    KEYINFO_DEFINED_ثنائی_ہندسہ_میدان = 16,
    KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان = 8,
    KEYINFO_REPEAT_ثنائی_ہندسہ_میدان = 8,
};
static_assert(
    XKB_LAYOUT_باہرلا_دا_RANGE_WRAP < XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP &&
    XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP < XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_MIN_چوڑائی) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_باہرلا_دا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان >=
              XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_MIN_چوڑائی, "");
static_assert((بنانشانی)KEYINFO_DEFINED_ثنائی_ہندسہ_میدان >=
              (بنانشانی)کلید_میدان_MIN_چوڑائی, "Cannot encode KeyInfo field");
static_assert((بنانشانی)KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان >=
              (بنانشانی)ملاؤ_منوال_MIN_چوڑائی, "Cannot encode KeyInfo field");
static_assert((بنانشانی)KEYINFO_REPEAT_ثنائی_ہندسہ_میدان >=
              (بنانشانی)کلید_REPEAT_MIN_چوڑائی, "Cannot encode KeyInfo field");

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_نقاب_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_اشاریہ_t باہرلا_دا_range_گروہ_نمبر;
    darray(گروہ_Info_2) groups;
    تعداد xkb_layout_باہرلا_دا_range_policy
        باہرلا_دا_range_گروہ_policy:KEYINFO_باہرلا_دا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد کلید_میدان defined:KEYINFO_DEFINED_ثنائی_ہندسہ_میدان;
    تعداد ملاؤ_منوال ملاؤ:KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان;
    تعداد کلید_repeat repeat:KEYINFO_REPEAT_ثنائی_ہندسہ_میدان;

    bool باہرلا_دا_range_pending_گروہ:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_اشاریہ_t overlays_alloc;
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
        ثابت بنية xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_کلید **overlays_کلیدیں;
    };
} کلید_Info;

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
Init_گروہ_Info(گروہ_Info_2 *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساكن فراغ
Clear_گروہ_Info(گروہ_Info_2 *groupi)
{
    بنية xkb_level *leveli;
    darray_foreach(leveli, groupi->پدھراں)
        clear_level(leveli);
    darray_free(groupi->پدھراں);
}

ساكن فراغ
Copy_گروہ_Info(گروہ_Info_2 *منزل, ثابت گروہ_Info_2 *from)
{
    منزل->defined = from->defined;
    منزل->type = from->type;
    darray_init(منزل->پدھراں);
    darray_copy(منزل->پدھراں, from->پدھراں);
    لكل (xkb_level_اشاریہ_t j = 0; j < darray_ناپ(منزل->پدھراں); j++) {
        إذا (darray_شے(from->پدھراں, j).عدد_syms > 1) {
            darray_شے(منزل->پدھراں, j).s.syms =
                memdup(darray_شے(from->پدھراں, j).s.syms,
                       darray_شے(from->پدھراں, j).عدد_syms,
                       حجم(xkb_keysym_t));
        }
        إذا (darray_شے(from->پدھراں, j).عدد_actions > 1) {
            darray_شے(منزل->پدھراں, j).a.actions =
                memdup(darray_شے(from->پدھراں, j).a.actions,
                       darray_شے(from->پدھراں, j).عدد_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساكن فراغ
Init_کلید_Info(بنية xkb_context *ctx, کلید_Info *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_کلید_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_کلید_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->باہرلا_دا_range_گروہ_policy = XKB_LAYOUT_باہرلا_دا_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_کلید = عدم;
}

ساكن فراغ
Clear_کلید_Info(کلید_Info *keyi)
{
    گروہ_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_گروہ_Info(groupi);
    darray_free(keyi->groups);
    إذا (keyi->overlays_alloc)
        حرر(keyi->overlays_کلیدیں);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح error_گنتی;
    بنانشانی صحيح include_ڈونگھائی;
    xkb_layout_اشاریہ_t explicit_گروہ;
    xkb_layout_اشاریہ_t max_groups;
    darray(کلید_Info) کلیدیں;
    کلید_Info default_کلید;
    ActionsInfo default_actions;
    darray(xkb_atom_t) گروہ_ناں;
    darray(Mod_Map_اندراج) modmaps;
    بنية xkb_mod_سیٹ mods;

    بنية xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت بنية xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساكن فراغ
InitSymbolsInfo(SymbolsInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                بنانشانی صحيح include_ڈونگھائی, ثابت بنية xkb_mod_سیٹ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_ڈونگھائی = include_ڈونگھائی;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_کلید_Info(keymap_info->keymap.ctx, &info->default_کلید);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ڈونگھائی > 0);
    info->explicit_گروہ = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    کلید_Info *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->کلیدیں)
        Clear_کلید_Info(keyi);
    darray_free(info->کلیدیں);
    darray_free(info->گروہ_ناں);
    darray_free(info->modmaps);
    Clear_کلید_Info(&info->default_کلید);
}

ساكن ثابت حرف *
کلید_Info_متن(SymbolsInfo *info, کلید_Info *keyi)
{
    أرجع کلید_Name_متن(info->ctx, keyi->name);
}

ساكن bool
ملاؤ_Groups(SymbolsInfo *info, گروہ_Info_2 *into, گروہ_Info_2 *from, bool clobber,
            bool report, xkb_layout_اشاریہ_t گروہ, xkb_atom_t کلید_name)
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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         گروہ + 1, کلید_Name_متن(info->ctx, کلید_name),
                         xkb_atom_متن(info->ctx, use),
                         xkb_atom_متن(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & گروہ_میدان_TYPE);

    /* Now look at the levels. */

    إذا (darray_خالی_تھاواں(from->پدھراں)) {
        Init_گروہ_Info(from);
        أرجع true;
    }

    إذا (darray_خالی_تھاواں(into->پدھراں)) {
        from->type = into->type;
        *into = *from;
        Init_گروہ_Info(from);
        أرجع true;
    }

    /* Merge the actions and syms. */
    ثابت darray_ناپ_t پدھراں_وچ_both =
        MIN(darray_ناپ(into->پدھراں), darray_ناپ(from->پدھراں));
    darray_ناپ_t from_Keysyms_گنتی = 0;
    darray_ناپ_t from_Actions_گنتی = 0;
    لكل (darray_ناپ_t i = 0; i < پدھراں_وچ_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_شے(into->پدھراں, i);
        بنية xkb_level* ثابت fromLevel = &darray_شے(from->پدھراں, i);

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
            from_Keysyms_گنتی++;
            from_Actions_گنتی++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->عدد_syms > 0 || intoLevel->عدد_actions > 0);
        أكد(fromLevel->عدد_syms > 0 || fromLevel->عدد_actions > 0);

        /* Handle keysyms */
        إذا (!Xkb_پدھراں_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            إذا (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_علامت,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, گروہ + 1, کلید_Name_متن(info->ctx, کلید_name),
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
                    from_Keysyms_گنتی++;
                } وإلا إذا (fromLevel->s.sym != XKB_کلید_No_علامت) {
                    /* Single defined keysym */
                    إذا (unlikely(intoLevel->عدد_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->عدد_syms = 1;
                    from_Keysyms_گنتی++;
                }
            } وإلا {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->عدد_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->s.sym == XKB_کلید_No_علامت) {
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
                    from_Keysyms_گنتی++;
                }
            }
        }

        /* Handle actions */
        إذا (!Xkb_پدھراں_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            إذا (report && !(intoHasNoAction || fromHasNoAction)) {
                إذا (unlikely(intoLevel->عدد_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, گروہ + 1, کلید_Name_متن(info->ctx, کلید_name),
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

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, گروہ + 1,
                                کلید_Name_متن(info->ctx, کلید_name),
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
                    from_Actions_گنتی++;
                } وإلا إذا (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    إذا (unlikely(intoLevel->عدد_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->عدد_actions = 1;
                    from_Actions_گنتی++;
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
                    from_Actions_گنتی++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    بنية xkb_level *level;
    darray_foreach_from(level, from->پدھراں, پدھراں_وچ_both) {
        darray_append(into->پدھراں, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->عدد_syms = 0;
        level->عدد_actions = 0;
        from_Keysyms_گنتی++;
        from_Actions_گنتی++;
    }
    إذا (from_Keysyms_گنتی) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        إذا (from_Keysyms_گنتی == darray_ناپ(into->پدھراں))
            into->defined &= ~گروہ_میدان_SYMS;
        into->defined |= (from->defined & گروہ_میدان_SYMS);
    }
    إذا (from_Actions_گنتی) {
        /* Reset defined actions field if we used no action from `into` */
        إذا (from_Actions_گنتی == darray_ناپ(into->پدھراں))
            into->defined &= ~گروہ_میدان_ACTS;
        into->defined |= (from->defined & گروہ_میدان_ACTS);
    }

    أرجع true;
}

ساكن bool
Use_New_کلید_میدان(تعداد کلید_میدان میدان_2, تعداد کلید_میدان old, تعداد کلید_میدان new,
               bool clobber, bool report, تعداد کلید_میدان *collide)
{
    إذا (!(old & میدان_2))
        أرجع (new & میدان_2);

    إذا (new & میدان_2) {
        إذا (report)
            *collide |= میدان_2;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
overlays_get(ثابت کلید_Info *info, xkb_overlay_اشاریہ_t ثنائی_ہندسہ,
             ثابت بنية xkb_کلید **کلید_باہرلا)
{
    إذا (ثنائی_ہندسہ >= (xkb_overlay_اشاریہ_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        أرجع false;
    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ثنائی_ہندسہ);
    إذا (!(info->overlays & نقاب)) أرجع false;
    إذا (کلید_باہرلا) {
        إذا (!info->overlays_alloc) {
            /* Stored inline */
            *کلید_باہرلا = info->overlay_کلید;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                info->overlays & (نقاب - 1u)
            );
            ثابت xkb_overlay_اشاریہ_t اشاریہ = (xkb_overlay_اشاریہ_t)popcount32(کم);
            *کلید_باہرلا = info->overlays_کلیدیں[اشاریہ];
        }
    }
    أرجع true;
}

ساكن bool
overlays_داخل_کرو(کلید_Info *keyi, xkb_overlay_اشاریہ_t ثنائی_ہندسہ,
                ثابت بنية xkb_کلید *کلید)
{
    إذا (ثنائی_ہندسہ >= (xkb_overlay_اشاریہ_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        أرجع false;

    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ثنائی_ہندسہ);

    إذا ((keyi->overlays & نقاب) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        إذا (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_کلید = کلید;
            إذا (!کلید)
                keyi->overlays_clear = true;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                keyi->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_اشاریہ_t اشاریہ =
                (xkb_overlay_اشاریہ_t)popcount32(کم);
            keyi->overlays_کلیدیں[اشاریہ] = کلید;
        }
        أرجع true;
    }

    /* Insert new element */
    إذا (!keyi->overlays || (keyi->overlays_clear && !کلید)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_کلید = کلید;
        keyi->overlays |= نقاب;
        keyi->overlays_clear = !کلید;
    } وإلا إذا (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_نقاب_t overlays = keyi->overlays | نقاب;
        ثابت xkb_overlay_اشاریہ_t alloc =
            (xkb_overlay_اشاریہ_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_کلید ** ثابت tmp = صفر_حافظہ_دیو(alloc, حجم(*tmp));
        إذا (!tmp) أرجع false;

        /* Copy previous entries to the buffer */
        إذا (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                overlays & (xkb_overlay_نقاب_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_اشاریہ_t idx = (xkb_overlay_اشاریہ_t)popcount32(کم);
            tmp[idx] = keyi->overlay_کلید;
        } وإلا {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
            overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
        );
        ثابت xkb_overlay_اشاریہ_t idx = (xkb_overlay_اشاریہ_t)popcount32(کم);
        tmp[idx] = کلید;

        keyi->overlays_کلیدیں = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وإلا {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_نقاب_t overlays =
            (xkb_overlay_نقاب_t)(keyi->overlays | نقاب);
        ثابت xkb_overlay_اشاریہ_t گنتی =
            (xkb_overlay_اشاریہ_t)popcount32(overlays);
        إذا (گنتی > keyi->overlays_alloc) {
            ثابت xkb_overlay_اشاریہ_t alloc =
                (xkb_overlay_اشاریہ_t)اگلا_pow2(گنتی);
            أكد(alloc >= گنتی);
            ثابت بنية xkb_کلید ** ثابت tmp = realloc(
                keyi->overlays_کلیدیں,
                (ناپ_t)alloc * حجم(*keyi->overlays_کلیدیں)
            );
            إذا (!tmp) أرجع false;
            keyi->overlays_کلیدیں = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
            overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
        );
        ثابت xkb_overlay_اشاریہ_t اشاریہ = (xkb_overlay_اشاریہ_t)popcount32(کم);
        إذا (اشاریہ >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        إذا (اشاریہ < گنتی - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(اشاریہ + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_کلیدیں + اشاریہ + 1, keyi->overlays_کلیدیں + اشاریہ,
                    (ناپ_t)(گنتی - 1u - اشاریہ) * حجم(ثابت بنية xkb_کلید *));
        }
        keyi->overlays_کلیدیں[اشاریہ] = کلید;
        keyi->overlays = overlays;
    }

    أرجع true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
ملاؤ_overlays(SymbolsInfo *info, کلید_Info *into, کلید_Info *from,
               bool clobber, bool report, تعداد کلید_میدان *collide)
{
    إذا (!(from->defined & کلید_میدان_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وإلا إذا (!(into->defined & کلید_میدان_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_کلیدیں = from->overlays_کلیدیں;
        from->overlays_کلیدیں = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= کلید_میدان_OVERLAY;
    } وإلا إذا (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وإلا {
        إذا (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_نقاب_t نتیجہ_نقاب =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_اشاریہ_t گنتی =
                (xkb_overlay_اشاریہ_t)popcount32(نتیجہ_نقاب);

            إذا (گنتی == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            کلید_Info *dest = into;
            کلید_Info *src = from;
            إذا (into->overlays_alloc >= گنتی &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وإلا إذا (from->overlays_alloc >= گنتی) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وإلا إذا (گنتی > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                إذا (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                إذا (!dest->overlays_alloc) {
                    ثابت بنية xkb_کلید **tmp = صفر_حافظہ_دیو(گنتی, حجم(*tmp));
                    إذا (!tmp)
                        أرجع false;
                    إذا (dest->overlay_کلید)
                        tmp[0] = dest->overlay_کلید;
                    dest->overlays_کلیدیں = tmp;
                    dest->overlays_alloc = گنتی;
                } وإلا {
                    ثابت بنية xkb_کلید **tmp =
                        realloc(dest->overlays_کلیدیں,
                                گنتی * حجم(*dest->overlays_کلیدیں));
                    إذا (!tmp)
                        أرجع false;
                    dest->overlays_کلیدیں = tmp;
                    dest->overlays_alloc = گنتی;
                }
            }

            xkb_overlay_نقاب_t بچیا = src->overlays;
            ثابت بنية xkb_کلید **src_کلیدیں = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_کلیدیں
                    : &src->overlay_کلید;
            طالما (بچیا) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بچیا &
                    (xkb_overlay_نقاب_t)((بنانشانی)~بچیا + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_اشاریہ_t ثنائی_ہندسہ =
                    (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
                بچیا = (بچیا & ~lsb);

                /* pop current value */
                إذا (!src->overlays_clear && !src->overlays_alloc && بچیا)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_کلید *src_کلید = (src_کلیدیں)
                    ? *(src_کلیدیں++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_کلید *dest_کلید = عدم;
                ثابت bool conflict = overlays_get(dest, ثنائی_ہندسہ, &dest_کلید);
                إذا (conflict) {
                    إذا (dest_کلید == src_کلید)
                       استمر;
                    وإلا إذا (report)
                        *collide |= کلید_میدان_OVERLAY;
                }

                إذا ((!conflict || clobber) && !overlays_داخل_کرو(dest, ثنائی_ہندسہ, src_کلید))
                    أرجع false;
            }

            إذا (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                إذا (into->overlays_alloc)
                    حرر(into->overlays_کلیدیں);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                إذا (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_کلیدیں = from->overlays_کلیدیں;
                    from->overlays_کلیدیں = عدم;
                    from->overlays_alloc = 0;
                } وإلا {
                    into->overlay_کلید = from->overlay_کلید;
                }
            }
        } وإلا {
            /* Disjoint overlays */

            إذا (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_کلید == from->overlay_کلید) {
                /* Indentical: do nothing */
                أرجع true;
            }
            إذا (!(into->overlays & from->overlays)) {
                إذا (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_کلید = from->overlay_کلید;
                    أرجع true;
                } وإلا إذا (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    أرجع true;
                }
            }
            /* Conflict */
            إذا (report)
                *collide |= کلید_میدان_OVERLAY;
            إذا (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_کلید = from->overlay_کلید;
            }
        }
    }
    أرجع true;
}

ساكن bool
ملاؤ_کلیدیں(SymbolsInfo *info, کلید_Info *into, کلید_Info *from, bool same_file)
{
    xkb_layout_اشاریہ_t i;
    xkb_layout_اشاریہ_t groups_وچ_both;
    تعداد کلید_میدان collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->ملاؤ != ملاؤ_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (from->ملاؤ == ملاؤ_REPLACE) {
        Clear_کلید_Info(into);
        *into = *from;
        Init_کلید_Info(info->ctx, from);
        أرجع true;
    }

    groups_وچ_both = MIN(darray_ناپ(into->groups), darray_ناپ(from->groups));
    لكل (i = 0; i < groups_وچ_both; i++)
        ملاؤ_Groups(info,
                    &darray_شے(into->groups, i),
                    &darray_شے(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    لكل (i = groups_وچ_both; i < darray_ناپ(from->groups); i++) {
        darray_append(into->groups, darray_شے(from->groups, i));
        Init_گروہ_Info(&darray_شے(from->groups, i));
    }

    إذا (Use_New_کلید_میدان(کلید_میدان_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= کلید_میدان_VMODMAP;
    }
    إذا (Use_New_کلید_میدان(کلید_میدان_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= کلید_میدان_REPEAT;
    }
    إذا (Use_New_کلید_میدان(کلید_میدان_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= کلید_میدان_DEFAULT_TYPE;
    }
    إذا (Use_New_کلید_میدان(کلید_میدان_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->باہرلا_دا_range_pending_گروہ = from->باہرلا_دا_range_pending_گروہ;
        into->باہرلا_دا_range_گروہ_policy = from->باہرلا_دا_range_گروہ_policy;
        into->باہرلا_دا_range_گروہ_نمبر = from->باہرلا_دا_range_گروہ_نمبر;
        into->defined |= کلید_میدان_GROUPINFO;
    }

    إذا (!ملاؤ_overlays(info, into, from, clobber, report, &collide))
        أرجع false;

    إذا (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 کلید_Name_متن(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_کلید_Info(from);
    Init_کلید_Info(info->ctx, from);
    أرجع true;
}

ساكن xkb_atom_t
Xkb_Resolve_کلید_Alias(ثابت بنية xkb_keymap *keymap, xkb_atom_t name)
{
    إذا (name < keymap->عدد_کلید_ناں) {
        ثابت Keycode_میل میل = keymap->کلید_ناں[name];
        إذا (میل.لبھیا && میل.is_alias) {
            أرجع میل.alias.real;
        }
    }
    أرجع name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
جوڑو_کلید_Symbols(SymbolsInfo *info, کلید_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_کلید_Alias(&info->keymap_info->keymap, keyi->name);

    کلید_Info *iter;
    darray_foreach(iter, info->کلیدیں)
        إذا (iter->name == keyi->name)
            أرجع ملاؤ_کلیدیں(info, iter, keyi, same_file);

    darray_append(info->کلیدیں, *keyi);
    Init_کلید_Info(info->ctx, keyi);
    أرجع true;
}

ساكن bool
جوڑو_Mod_Map_اندراج(SymbolsInfo *info, ثابت Mod_Map_اندراج *new)
{
    ثابت bool clobber = (new->ملاؤ != ملاؤ_AUGMENT);

    Mod_Map_اندراج *old;
    darray_foreach(old, info->modmaps) {
        إذا ((new->have_علامت != old->have_علامت) ||
            (new->have_علامت && new->u.کلید_Sym != old->u.کلید_Sym) ||
            (!new->have_علامت && new->u.کلید_Name != old->u.کلید_Name))
            استمر;

        إذا (new->mods == old->mods)
            أرجع true;

        ثابت xkb_mod_نقاب_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_نقاب_t ignore = (clobber ? old->mods : new->mods);

        إذا (new->have_علامت) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_متن(info->ctx, new->u.کلید_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        } وإلا {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    کلید_Name_متن(info->ctx, new->u.کلید_Name),
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
ملاؤ_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد ملاؤ_منوال ملاؤ)
{
    xkb_atom_t *گروہ_name;
    xkb_layout_اشاریہ_t گروہ_ناں_وچ_both;

    إذا (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        أرجع;
    }

    ملاؤ_Mod_Sets(into->ctx, &into->mods, &from->mods, ملاؤ);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    گروہ_ناں_وچ_both = MIN(darray_ناپ(into->گروہ_ناں),
                              darray_ناپ(from->گروہ_ناں));
    لكل (xkb_layout_اشاریہ_t i = 0; i < گروہ_ناں_وچ_both; i++) {
        إذا (!darray_شے(from->گروہ_ناں, i))
            استمر;

        إذا (ملاؤ == ملاؤ_AUGMENT && darray_شے(into->گروہ_ناں, i))
            استمر;

        darray_شے(into->گروہ_ناں, i) = darray_شے(from->گروہ_ناں, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(گروہ_name, from->گروہ_ناں, گروہ_ناں_وچ_both)
        darray_append(into->گروہ_ناں, *گروہ_name);

    إذا (darray_خالی_تھاواں(into->کلیدیں)) {
        into->کلیدیں = from->کلیدیں;
        darray_init(from->کلیدیں);
    }
    وإلا {
        کلید_Info *keyi;
        darray_foreach(keyi, from->کلیدیں) {
            keyi->ملاؤ = ملاؤ;
            إذا (!جوڑو_کلید_Symbols(into, keyi, false))
                into->error_گنتی++;
        }
    }

    إذا (darray_خالی_تھاواں(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وإلا {
        Mod_Map_اندراج *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ملاؤ = ملاؤ;
            إذا (!جوڑو_Mod_Map_اندراج(into, mm))
                into->error_گنتی++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    إذا (Exceeds_Include_Max_ڈونگھائی(info->ctx, info->include_ڈونگھائی)) {
        info->error_گنتی += 10;
        أرجع false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_ڈونگھائی + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        SymbolsInfo اگلا_incl;
        XkbFile *file;

        حرف رستہ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  رستہ, حجم(رستہ));
        إذا (!file) {
            info->error_گنتی += 10;
            ClearSymbolsInfo(&included);
            أرجع false;
        }

        InitSymbolsInfo(&اگلا_incl, info->keymap_info, info->include_ڈونگھائی + 1,
                        &included.mods);
        إذا (stmt->modifier) {
            ثابت صحيح گنتی = parse_dec_منزل_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &اگلا_incl.explicit_گروہ);
            إذا (گنتی <= 0 || *(stmt->modifier + گنتی) != '\0' ||
                اگلا_incl.explicit_گروہ == 0 ||
                اگلا_incl.explicit_گروہ > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                اگلا_incl.explicit_گروہ = info->explicit_گروہ;
            } وإلا {
                اگلا_incl.explicit_گروہ--;
            }
        }
        وإلا إذا (info->keymap_info->keymap.عدد_groups != 0 &&
                 اگلا_incl.include_ڈونگھائی == 1) {
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
            اگلا_incl.explicit_گروہ = 0;
        }
        وإلا {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            اگلا_incl.explicit_گروہ = info->explicit_گروہ;
        }

        HandleSymbolsFile(&اگلا_incl, file);

        ملاؤ_Included_Symbols(&included, &اگلا_incl, stmt->ملاؤ);

        ClearSymbolsInfo(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_Symbols(info, &included, include->ملاؤ);
    ClearSymbolsInfo(&included);

    أرجع (info->error_گنتی == 0);
}

ساكن bool
Get_گروہ_اشاریہ(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
              تعداد گروہ_میدان میدان_2, xkb_layout_اشاریہ_t *ndx_rtrn)
{
    أكد (میدان_2 == گروہ_میدان_SYMS || میدان_2 == گروہ_میدان_ACTS);
    ثابت حرف *name = (میدان_2 == گروہ_میدان_SYMS ? "symbols" : "actions");

    إذا (arrayNdx == عدم) {
        xkb_layout_اشاریہ_t i = 0;
        گروہ_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            إذا (!(groupi->defined & میدان_2)) {
                *ndx_rtrn = i;
                أرجع true;
            }
        }

        إذا (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, کلید_Info_متن(info, keyi), info->max_groups, name);
            أرجع false;
        }

        darray_resize0(keyi->groups, darray_ناپ(keyi->groups) + 1);
        *ndx_rtrn = darray_ناپ(keyi->groups) - 1;
        أرجع true;
    }

    إذا (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, کلید_Info_متن(info, keyi));
        أرجع false;
    }

    (*ndx_rtrn)--;
    إذا (*ndx_rtrn >= darray_ناپ(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    أرجع true;
}

ساكن bool
جوڑو_Symbols_منزل_کلید(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
                اظہاریہ_Def *قدر)
{
    xkb_layout_اشاریہ_t ndx = 0;

    إذا (!Get_گروہ_اشاریہ(info, keyi, arrayNdx, گروہ_میدان_SYMS, &ndx))
        أرجع false;

    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, ndx);

    إذا (قدر->common.type == STMT_اظہاریہ_خالی_تھاواں_LIST) {
        groupi->defined |= گروہ_میدان_SYMS;
        أرجع true;
    }

    إذا (قدر->common.type != STMT_اظہاریہ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_منزل_string(قدر->common.type), ndx + 1,
                کلید_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروہ_میدان_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_کلید_SYMBOLS_اندراج,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                کلید_Info_متن(info, keyi), ndx + 1);
        أرجع false;
    }

    xkb_level_اشاریہ_t n_پدھراں = 0;
    xkb_level_اشاریہ_t non_خالی_تھاواں_پدھراں = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (اظہاریہ_Keysym_List *keysymList = (اظہاریہ_Keysym_List *) قدر;
         keysymList;
         keysymList = (اظہاریہ_Keysym_List *) keysymList->common.اگلا) {
        n_پدھراں++;
        /* Drop trailing NoSymbol */
        إذا (darray_ناپ(keysymList->syms) > 0)
            non_خالی_تھاواں_پدھراں = n_پدھراں;
    }
    إذا (non_خالی_تھاواں_پدھراں < n_پدھراں)
        n_پدھراں = non_خالی_تھاواں_پدھراں;
    إذا (darray_ناپ(groupi->پدھراں) < n_پدھراں)
        darray_resize0(groupi->پدھراں, n_پدھراں);

    groupi->defined |= گروہ_میدان_SYMS;

    xkb_level_اشاریہ_t level = 0;
    لكل (اظہاریہ_Keysym_List *keysymList = (اظہاریہ_Keysym_List *) قدر;
         keysymList && level < n_پدھراں;
         keysymList = (اظہاریہ_Keysym_List *) keysymList->common.اگلا, level++) {
        بنية xkb_level *leveli = &darray_شے(groupi->پدھراں, level);
        أكد(leveli->عدد_syms == 0);

        إذا (unlikely(darray_ناپ(keysymList->syms) > MAX_KEYSYMS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_فی_LEVEL, darray_ناپ(keysymList->syms));
            أرجع false;
        }

        leveli->عدد_syms = (xkb_keysym_گنتی_t) darray_ناپ(keysymList->syms);
        بدل (leveli->عدد_syms) {
        حالة 0:
            leveli->s.sym = XKB_کلید_No_علامت;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_شے(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_کلید_No_علامت);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#جے_بنا_تعریف NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++)
                أكد(leveli->s.syms[k] != XKB_کلید_No_علامت);
#جے_انت
        }
    }

    أرجع true;
}

ساكن bool
جوڑو_Actions_منزل_کلید(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
                اظہاریہ_Def *قدر)
{
    xkb_layout_اشاریہ_t ndx = 0;

    إذا (!Get_گروہ_اشاریہ(info, keyi, arrayNdx, گروہ_میدان_ACTS, &ndx))
        أرجع false;

    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, ndx);

    إذا (قدر->common.type == STMT_اظہاریہ_خالی_تھاواں_LIST) {
        groupi->defined |= گروہ_میدان_ACTS;
        أرجع true;
    }

    إذا (قدر->common.type != STMT_اظہاریہ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 قدر->common.type, ndx, کلید_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروہ_میدان_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 کلید_Info_متن(info, keyi), ndx);
        أرجع false;
    }

    xkb_level_اشاریہ_t n_پدھراں = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &قدر->common; p; p = p->اگلا)
        n_پدھراں++;
    إذا (darray_ناپ(groupi->پدھراں) < n_پدھراں)
        darray_resize0(groupi->پدھراں, n_پدھراں);

    groupi->defined |= گروہ_میدان_ACTS;

    xkb_level_اشاریہ_t level = 0;
    xkb_level_اشاریہ_t non_خالی_تھاواں_پدھراں = 0;
    لكل (اظہاریہ_Action_List *actionList = (اظہاریہ_Action_List *) قدر;
         actionList;
         actionList = (اظہاریہ_Action_List *) actionList->common.اگلا, level++) {
        بنية xkb_level *leveli = &darray_شے(groupi->پدھراں, level);
        أكد(leveli->عدد_actions == 0);

        بنانشانی صحيح عدد_actions = 0;
        لكل (اظہاریہ_Def *act = actionList->actions;
             act; act = (اظہاریہ_Def *) act->common.اگلا)
            عدد_actions++;

        إذا (unlikely(عدد_actions > MAX_ACTIONS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_فی_LEVEL, عدد_actions);
            أرجع false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (اظہاریہ_Def *act = actionList->actions;
             act; act = (اظہاریہ_Def *) act->common.اگلا) {
            اتحاد xkb_action منزل_Act = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &منزل_Act);
            إذا (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        کلید_Info_متن(info, keyi), ndx + 1, level + 1);
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
                اذهب اگلا_level;
            } وإلا {
                darray_append(actions, منزل_Act);
            }
        }
        إذا (darray_خالی_تھاواں(actions)) {
            leveli->عدد_actions = 0;
        } وإلا إذا (likely(darray_ناپ(actions) > 1)) {
            leveli->عدد_actions = (xkb_action_گنتی_t) darray_ناپ(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#جے_بنا_تعریف NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_گنتی_t k = 0; k < leveli->عدد_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#جے_انت
        } وإلا {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(عدد_actions > darray_ناپ(actions));
            leveli->عدد_actions = 1;
            leveli->a.action = darray_شے(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
اگلا_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        إذا (leveli->عدد_actions > 0 || leveli->عدد_syms > 0)
            non_خالی_تھاواں_پدھراں = level + 1;
    }

    إذا (non_خالی_تھاواں_پدھراں < n_پدھراں) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_ناپ(groupi->پدھراں) = non_خالی_تھاواں_پدھراں;
        إذا (non_خالی_تھاواں_پدھراں > 0)
            darray_shrink(groupi->پدھراں);
        وإلا
            darray_free(groupi->پدھراں);
    }

    أرجع true;
}

ساكن ثابت Lookup_اندراج repeatEntries[] = {
    { "true", کلید_REPEAT_YES },
    { "yes", کلید_REPEAT_YES },
    { "on", کلید_REPEAT_YES },
    { "false", کلید_REPEAT_NO },
    { "no", کلید_REPEAT_NO },
    { "off", کلید_REPEAT_NO },
    { "default", کلید_REPEAT_UNDEFINED },
    { عدم, 0 }
};

/** Handle overlay<N> = <key> entry */
ساكن bool
اظہاریہ_Resolve_Overlay_اندراج(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *میدان_2, ثابت اظہاریہ_Def *arrayNdx,
                        ثابت اظہاریہ_Def *اظہاریہ, کلید_Info *keyi,
                        xkb_overlay_اشاریہ_t *overlay_rtrn,
                        ثابت بنية xkb_کلید **کلید_rtrn)
{
    /* Overlay index */
    إذا (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    static_assert(حجم(xkb_overlay_اشاریہ_t) == حجم(uint8_t), "");
    ثابت ناپ_t اگیتر = حجم("overlay") - 1;
    ثابت ناپ_t len = لڑی_لمبائی(میدان_2 + اگیتر);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    إذا (parse_dec_منزل_uint64_t(میدان_2 + اگیتر, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاریہ,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        أرجع false;
    }
    *overlay_rtrn = (xkb_overlay_اشاریہ_t)raw_overlay - 1;

    /* Overlay key */
    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_KEYNAME_LITERAL:
        *کلید_rtrn = Xkb_کلید_By_Name(&keymap_info->keymap, اظہاریہ->کلید_name.کلید_name,
                                 false);
        إذا (!*کلید_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_متن(keymap_info->keymap.ctx, اظہاریہ->کلید_name.کلید_name),
                    میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
            أرجع false;
        }
        أرجع true;
    حالة STMT_اظہاریہ_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_متن(keymap_info->keymap.ctx, اظہاریہ->ident.ident);
        إذا (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *کلید_rtrn = عدم;
            أرجع true;
        } وإلا إذا (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *کلید_rtrn = عدم;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            أرجع true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_منزل_string(STMT_اظہاریہ_KEYNAME_LITERAL),
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;
    }
}

ساكن bool
سیٹ_Symbols_میدان(SymbolsInfo *info, کلید_Info *keyi, ثابت حرف *میدان_2,
                اظہاریہ_Def *arrayNdx, اظہاریہ_Def **قدر_ptr)
{
    اظہاریہ_Def * محدود ثابت قدر = *قدر_ptr;
    إذا (istreq(میدان_2, "type")) {
        xkb_layout_اشاریہ_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        إذا (!اظہاریہ_Resolve_String(info->ctx, قدر, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            أرجع false;
        }

        إذا (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= کلید_میدان_DEFAULT_TYPE;
        }
        وإلا إذا (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }
        وإلا {
            ndx--;
            إذا (ndx >= darray_ناپ(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_ناپ(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_شے(keyi->groups, ndx).type = val;
            darray_شے(keyi->groups, ndx).defined |= گروہ_میدان_TYPE;
        }
    }
    وإلا إذا (istreq(میدان_2, "symbols")) {
        أرجع جوڑو_Symbols_منزل_کلید(info, keyi, arrayNdx, قدر);
    }
    وإلا إذا (istreq(میدان_2, "actions")) {
        أرجع جوڑو_Actions_منزل_کلید(info, keyi, arrayNdx, قدر);
    }
    وإلا إذا (istreq(میدان_2, "vmods") ||
             istreq(میدان_2, "virtualmods") ||
             istreq(میدان_2, "virtualmodifiers")) {
        xkb_mod_نقاب_t نقاب = 0;

        إذا (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_VIRT, &info->mods,
                                &نقاب)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_منزل_string(قدر->common.type),
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->vmodmap = نقاب;
        keyi->defined |= کلید_میدان_VMODMAP;
    }
    وإلا إذا (istreq(میدان_2, "locking") ||
             istreq(میدان_2, "lock") ||
             istreq(میدان_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq(میدان_2, "radiogroup") ||
             istreq(میدان_2, "permanentradiogroup") ||
             istreq(میدان_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_اگیتر("permanentoverlay", میدان_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_اگیتر("overlay", میدان_2)) {
        xkb_overlay_اشاریہ_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_کلید *کلید = عدم;
        إذا (!اظہاریہ_Resolve_Overlay_اندراج(info->keymap_info, میدان_2, arrayNdx,
                                     *قدر_ptr, keyi, &overlay, &کلید))
            أرجع false;
        إذا (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            أرجع true;
        } وإلا إذا (کلید && کلید->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_سنیہا_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, کلید_Info_متن(info, keyi));
        } وإلا {
            ثابت بنية xkb_کلید *prev;
            إذا (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                إذا (کلید != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             کلید_Info_متن(info, keyi), overlay + 1,
                             (prev
                                 ? کلید_Name_متن(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (کلید
                                 ? کلید_Name_متن(info->ctx, کلید->name)
                                 : "none"));
                }
            } وإلا إذا (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                إذا (!overlays_داخل_کرو(keyi, overlay, کلید))
                    أرجع false;
                keyi->defined |= کلید_میدان_OVERLAY;
            } وإلا {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << overlay);
                إذا (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    إذا (کلید) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = نقاب;
                        keyi->overlays_clear = false;
                        keyi->overlay_کلید = کلید;
                    } وإلا {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= نقاب;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= کلید_میدان_OVERLAY;
                } وإلا إذا (keyi->overlays) {
                    إذا (کلید) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                کلید_Info_متن(info, keyi), keyi->overlays,
                                کلید_Name_متن(info->ctx, keyi->overlay_کلید->name),
                                overlay + 1, کلید_Name_متن(info->ctx, کلید->name));
                        أرجع !(info->keymap_info->strict &
                                 PARSER_NO_میدان_قدر_MISMATCH);
                    } وإلا {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وإلا إذا (istreq(میدان_2, "repeating") ||
             istreq(میدان_2, "repeats") ||
             istreq(میدان_2, "repeat")) {
        uint32_t val = 0;

        إذا (!اظہاریہ_Resolve_Enum(info->ctx, قدر, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->repeat = (تعداد کلید_repeat) val;
        keyi->defined |= کلید_میدان_REPEAT;
    }
    وإلا إذا (istreq(میدان_2, "groupswrap") ||
             istreq(میدان_2, "wrapgroups")) {
        bool سیٹ = false;

        إذا (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &سیٹ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->باہرلا_دا_range_گروہ_policy = (سیٹ)
            ? XKB_LAYOUT_باہرلا_دا_RANGE_WRAP
            : XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    وإلا إذا (istreq(میدان_2, "groupsclamp") ||
             istreq(میدان_2, "clampgroups")) {
        bool سیٹ = false;

        إذا (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &سیٹ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->باہرلا_دا_range_گروہ_policy = (سیٹ)
            ? XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP
            : XKB_LAYOUT_باہرلا_دا_RANGE_WRAP;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    وإلا إذا (istreq(میدان_2, "groupsredirect") ||
             istreq(میدان_2, "redirectgroups")) {
        xkb_layout_اشاریہ_t grp = 0;
        bool pending = false;

        // TODO: recover?
        إذا (اظہاریہ_Resolve_گروہ(info->keymap_info, قدر, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        إذا (pending) {
            keyi->باہرلا_دا_range_pending_گروہ = true;
            ثابت darray_ناپ_t pending_اشاریہ =
                darray_ناپ(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .اظہاریہ = *قدر_ptr,
                    .computed = false,
                    .قدر = 0,
                }
            );
            *قدر_ptr = عدم;
            static_assert(حجم(keyi->باہرلا_دا_range_گروہ_نمبر) ==
                          حجم(pending_اشاریہ),
                          "Cannot save pending computation");
            keyi->باہرلا_دا_range_گروہ_نمبر = pending_اشاریہ;
        } وإلا {
            keyi->باہرلا_دا_range_pending_گروہ = false;
            keyi->باہرلا_دا_range_گروہ_نمبر = grp - 1;
        }

        keyi->باہرلا_دا_range_گروہ_policy = XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_میدان,
                "Unknown field \"%s\" in a key; definition ignored\n",
                میدان_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_کلید_FIELDS);
    }

    أرجع true;
}

ساكن bool
سیٹ_گروہ_Name(SymbolsInfo *info, اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر,
             تعداد ملاؤ_منوال ملاؤ)
{
    إذا (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_گروہ_NAME_اشاریہ,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        أرجع false;
    }

    xkb_layout_اشاریہ_t گروہ = 0;
    إذا (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false, &گروہ, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!اظہاریہ_Resolve_String(info->ctx, قدر, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", گروہ);
        أرجع false;
    }

    xkb_layout_اشاریہ_t گروہ_منزل_use;
    إذا (info->explicit_گروہ == XKB_LAYOUT_INVALID) {
        گروہ_منزل_use = گروہ - 1;
    }
    وإلا إذا (گروہ - 1 == 0) {
        گروہ_منزل_use = info->explicit_گروہ;
    }
    وإلا {
        log_warn(info->ctx, XKB_WARNING_NON_ادھار_گروہ_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, گروہ,
                 xkb_atom_متن(info->ctx, name));
        أرجع false;
    }

    إذا (گروہ_منزل_use >= darray_ناپ(info->گروہ_ناں)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_ناپ(info->گروہ_ناں) < گروہ_منزل_use + 1);
        darray_resize0(info->گروہ_ناں, گروہ_منزل_use + 1);
    } وإلا {
        ثابت xkb_atom_t old_name = darray_شے(info->گروہ_ناں, گروہ_منزل_use);
        إذا (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (ملاؤ != ملاؤ_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     گروہ_منزل_use, info->name,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
            name = use;
        }
    }
    darray_شے(info->گروہ_ناں, گروہ_منزل_use) = name;

    أرجع true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;
    bool ret;

    إذا (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2, &arrayNdx))
        أرجع false;

    إذا (elem && istreq(elem, "key")) {
        کلید_Info عارضی = {0};
        Init_کلید_Info(info->ctx, &عارضی);
        /* Do not replace the whole key, only the current field */
        عارضی.ملاؤ = (عارضی.ملاؤ == ملاؤ_REPLACE)
            ? ملاؤ_OVERRIDE
            : stmt->ملاؤ;
        ret = سیٹ_Symbols_میدان(info, &عارضی, میدان_2, arrayNdx, &stmt->قدر) &&
              ملاؤ_کلیدیں(info, &info->default_کلید, &عارضی, true);
        Clear_کلید_Info(&عارضی);
    }
    وإلا إذا (!elem && (istreq(میدان_2, "name") ||
                       istreq(میدان_2, "groupname"))) {
        ret = سیٹ_گروہ_Name(info, arrayNdx, stmt->قدر, stmt->ملاؤ);
    }
    وإلا إذا (!elem && (istreq(میدان_2, "groupswrap") ||
                       istreq(میدان_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(میدان_2, "groupsclamp") ||
                       istreq(میدان_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(میدان_2, "groupsredirect") ||
                       istreq(میدان_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && istreq(میدان_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وإلا إذا (elem) {
        ret = (سیٹ_Default_Action_میدان(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, میدان_2, arrayNdx,
                                     &stmt->قدر, stmt->ملاؤ) !=
               PARSER_FATAL_ERROR);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    أرجع ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, کلید_Info *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.اگلا) {
        ثابت حرف *میدان_2;
        اظہاریہ_Def *arrayNdx;
        bool ok = true;

        إذا (!def->name) {
            إذا (unlikely(!def->قدر) || def->قدر->common.type != STMT_اظہاریہ_ACTION_LIST)
                میدان_2 = "symbols"; /* Default to symbols field */
            وإلا
                میدان_2 = "actions";
            arrayNdx = عدم;
        }
        وإلا {
            ثابت حرف *elem;
            ok = اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &میدان_2,
                                &arrayNdx);
            إذا (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, میدان_2);
                ok = false;
            }
        }

        إذا (unlikely(!def->قدر)) {
            log_err(info->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", میدان_2);
            ok = false;
        }

        إذا (!ok || !سیٹ_Symbols_میدان(info, keyi, میدان_2, arrayNdx, &def->قدر))
            all_valid_entries = false;
    }

    أرجع all_valid_entries;
}

ساكن bool
سیٹ_Explicit_گروہ(SymbolsInfo *info, کلید_Info *keyi)
{
    xkb_layout_اشاریہ_t i;
    گروہ_Info_2 *groupi;
    bool warn = false;

    إذا (info->explicit_گروہ == XKB_LAYOUT_INVALID)
        أرجع true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        إذا (groupi->defined) {
            warn = true;
            Clear_گروہ_Info(groupi);
            Init_گروہ_Info(groupi);
        }
    }

    إذا (warn) {
        log_warn(info->ctx, XKB_WARNING_مضرب_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_گروہ + 1, کلید_Info_متن(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_گروہ + 1);
    إذا (info->explicit_گروہ > 0) {
        darray_شے(keyi->groups, info->explicit_گروہ) =
            darray_شے(keyi->groups, 0);
        Init_گروہ_Info(&darray_شے(keyi->groups, 0));
    }

    أرجع true;
}

ساكن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    کلید_Info keyi;

    keyi = info->default_کلید;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_کلید.groups);
    لكل (xkb_layout_اشاریہ_t i = 0; i < darray_ناپ(keyi.groups); i++)
        Copy_گروہ_Info(&darray_شے(keyi.groups, i),
                      &darray_شے(info->default_کلید.groups, i));
    keyi.ملاؤ = stmt->ملاؤ;
    keyi.name = stmt->کلید_Name;

    إذا (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        سیٹ_Explicit_گروہ(info, &keyi) &&
        جوڑو_کلید_Symbols(info, &keyi, true)) {
        أرجع true;
    }

    Clear_کلید_Info(&keyi);
    info->error_گنتی++;
    أرجع false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_اندراج tmp;
    بنية xkb_context *ctx = info->ctx;

    إذا (!اظہاریہ_Resolve_Mod_نقاب(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        أرجع false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_متن_V1 ||
           def->modifiers->common.type == STMT_اظہاریہ_IDENT);

    bool ok = true;
    tmp.ملاؤ = def->ملاؤ;

    لكل (اظہاریہ_Def *کلید = def->کلیدیں; کلید; کلید = (اظہاریہ_Def *) کلید->common.اگلا) {
        إذا (کلید->common.type == STMT_اظہاریہ_KEYNAME_LITERAL) {
            tmp.have_علامت = false;
            tmp.u.کلید_Name = کلید->کلید_name.کلید_name;
        }
        وإلا إذا (کلید->common.type == STMT_اظہاریہ_KEYSYM_LITERAL) {
            إذا (کلید->keysym.keysym == XKB_کلید_No_علامت) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } وإلا {
                tmp.have_علامت = true;
                tmp.u.کلید_Sym = کلید->keysym.keysym;
            }
        }
        وإلا {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_اندراج,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = جوڑو_Mod_Map_اندراج(info, &tmp) && ok;
    }
    أرجع ok;
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
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
            info->error_گنتی++;

        إذا (info->error_گنتی > 10) {
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
ساكن بنية xkb_کلید *
لبھو_کلید_For_علامت(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_گروہ;
    xkb_layout_اشاریہ_t گروہ = 0;
    افعل {
        xkb_level_اشاریہ_t level = 0;
        got_one_گروہ = false;
        bool got_one_level;
        افعل {
            got_one_level = false;
            بنية xkb_کلید *کلید;
            xkb_کلیدیں_foreach(کلید, keymap) {
                إذا (گروہ < کلید->عدد_groups &&
                    level < Xkb_کلید_عدد_پدھراں(کلید, گروہ)) {
                    got_one_گروہ = got_one_level = true;
                    ثابت xkb_keysym_گنتی_t عدد_syms =
                        کلید->groups[گروہ].پدھراں[level].عدد_syms;
                    إذا (عدد_syms > 1) {
                        لكل (xkb_keysym_گنتی_t k = 0; k < عدد_syms; k++) {
                            إذا (کلید->groups[گروہ].پدھراں[level].s.syms[k] == sym)
                                أرجع کلید;
                        }
                    } وإلا إذا (عدد_syms &&
                               کلید->groups[گروہ].پدھراں[level].s.sym == sym) {
                        أرجع کلید;
                    }
                }
            }
            level++;
        } طالما (got_one_level);
        گروہ++;
    } طالما (got_one_گروہ);

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
لبھو_Automatic_Type(بنية xkb_context *ctx, گروہ_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_اشاریہ_t چوڑائی = darray_ناپ(groupi->پدھراں);

#تعریف_کرو GET_SYM(level) \
    (darray_شے(groupi->پدھراں, level).عدد_syms == 0 ? \
        XKB_کلید_No_علامت : \
     darray_شے(groupi->پدھراں, level).عدد_syms == 1 ? \
        darray_شے(groupi->پدھراں, level).s.sym : \
     /* num_syms > 1 */ \
        darray_شے(groupi->پدھراں, level).s.syms[0])

    إذا (چوڑائی == 1 || چوڑائی <= 0)
        أرجع xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    إذا (چوڑائی == 2) {
        إذا (xkb_keysym_is_ہیٹھلا(sym0) && xkb_keysym_is_اتلا_or_title(sym1))
            أرجع xkb_atom_intern_literal(ctx, "ALPHABETIC");

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    إذا (چوڑائی <= 4) {
        إذا (xkb_keysym_is_ہیٹھلا(sym0) && xkb_keysym_is_اتلا_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (چوڑائی == 4 ? GET_SYM(3) : XKB_کلید_No_علامت);

            إذا (xkb_keysym_is_ہیٹھلا(sym2) && xkb_keysym_is_اتلا_or_title(sym3))
                أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    أرجع XKB_ATOM_NONE;

#تعریف_رد GET_SYM
}

ساكن ثابت بنية xkb_کلید_type *
لبھو_Type_For_گروہ(بنية xkb_keymap *keymap, کلید_Info *keyi,
                 xkb_layout_اشاریہ_t گروہ, bool *explicit_type)
{
    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, گروہ);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    إذا (type_name == XKB_ATOM_NONE) {
        إذا (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وإلا {
            type_name = لبھو_Automatic_Type(keymap->ctx, groupi);
            إذا (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    إذا (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_کلید_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 کلید_Name_متن(keymap->ctx, keyi->name), گروہ + 1,
                 darray_ناپ(groupi->پدھراں));
        اذهب use_default;
    }

    darray_ناپ_t i;
    لكل (i = 0; i < keymap->عدد_types; i++)
        إذا (keymap->types[i].name == type_name)
            اكسر;

    إذا (i >= keymap->عدد_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_کلید_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_متن(keymap->ctx, type_name),
                 کلید_Name_متن(keymap->ctx, keyi->name), گروہ + 1);
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
                       کلید_Info *keyi)
{
    بنية xkb_کلید *کلید;
    گروہ_Info_2 *groupi;
    ثابت گروہ_Info_2 *group0;
    xkb_layout_اشاریہ_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    کلید = Xkb_کلید_By_Name(keymap, keyi->name, false);
    إذا (!کلید) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                کلید_Info_متن(info, keyi));
        أرجع false;
    }

    /* Find the range of groups we need. */
    کلید->عدد_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & کلید_میدان_DEFAULT_TYPE)
                                    || (groupi->defined & گروہ_میدان_TYPE);
        إذا (darray_ناپ(groupi->پدھراں) > 0 || has_explicit_type)
            کلید->عدد_groups = i + 1;
        إذا (has_explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    إذا (کلید->عدد_groups <= 0) {
        /* A key with no group may still have other fields defined */
        إذا (keyi->defined)
            اذهب کلید_fields;
        وإلا
            أرجع false;
    }

    darray_resize(keyi->groups, کلید->عدد_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_شے(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        إذا (groupi->defined)
            استمر;

        Copy_گروہ_Info(groupi, group0);
    }

    کلید->groups = صفر_حافظہ_دیو(کلید->عدد_groups, حجم(*کلید->groups));
    أكد(darray_ناپ(keyi->groups) == کلید->عدد_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #جے_تعریف_شدہ __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < کلید->عدد_groups);
        #جے_انت
        bool explicit_type = false;
        ثابت بنية xkb_کلید_type * ثابت type =
            لبھو_Type_For_گروہ(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        إذا (type->عدد_پدھراں < darray_ناپ(groupi->پدھراں)) {
            بنية xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_متن(keymap->ctx, type->name), type->عدد_پدھراں,
                    کلید_Info_متن(info, keyi),
                    darray_ناپ(groupi->پدھراں));

            darray_foreach_from(leveli, groupi->پدھراں, type->عدد_پدھراں)
                clear_level(leveli);
        }
        darray_resize0(groupi->پدھراں, type->عدد_پدھراں);

        کلید->groups[i].explicit_type = explicit_type;
        کلید->groups[i].type = type;
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
        darray_foreach(leveli, groupi->پدھراں) {
            بدل (leveli->عدد_syms) {
            حالة 0:
                leveli->اتلا = XKB_کلید_No_علامت;
                اكسر;
            حالة 1:
                leveli->اتلا = xkb_keysym_منزل_اتلا(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_اتلا = false;
                لكل (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++) {
                    ثابت xkb_keysym_t اتلا =
                        xkb_keysym_منزل_اتلا(leveli->s.syms[k]);
                    إذا (اتلا != leveli->s.syms[k]) {
                        leveli->has_اتلا = true;
                        اكسر;
                    }
                }
                إذا (leveli->has_اتلا) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (ناپ_t) 2 * leveli->عدد_syms *
                                             حجم(*leveli->s.syms));
                    إذا (!leveli->s.syms)
                        أرجع false; /* FIXME: better handling? */
                    لكل (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++) {
                        leveli->s.syms[leveli->عدد_syms + k] =
                            xkb_keysym_منزل_اتلا(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->پدھراں, &کلید->groups[i].پدھراں, عدم);
        إذا (کلید->groups[i].type->عدد_پدھراں > 1 ||
            کلید->groups[i].پدھراں[0].عدد_syms > 0) {
            کلید->groups[i].explicit_symbols = true;
            کلید->explicit |= EXPLICIT_SYMBOLS;
        }
        إذا (groupi->defined & گروہ_میدان_ACTS) { // FIXME
            کلید->groups[i].explicit_actions = true;
            کلید->explicit |= EXPLICIT_INTERP;
        }
        إذا (کلید->groups[i].explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    کلید->باہرلا_دا_range_pending_گروہ = keyi->باہرلا_دا_range_pending_گروہ;
    کلید->باہرلا_دا_range_گروہ_نمبر = keyi->باہرلا_دا_range_گروہ_نمبر;
    کلید->باہرلا_دا_range_گروہ_policy = keyi->باہرلا_دا_range_گروہ_policy;

کلید_fields:
    إذا (keyi->defined & کلید_میدان_VMODMAP) {
        کلید->vmodmap = keyi->vmodmap;
        کلید->explicit |= EXPLICIT_VMODMAP;
    }

    إذا (keyi->repeat != کلید_REPEAT_UNDEFINED) {
        کلید->repeats = (keyi->repeat == کلید_REPEAT_YES);
        کلید->explicit |= EXPLICIT_REPEAT;
    }

    إذا ((keyi->defined & کلید_میدان_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        کلید->overlays = keyi->overlays;
        إذا (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_نقاب_t بچیا = کلید->overlays;
            ثابت بنية xkb_کلید **overlays_کلیدیں = keyi->overlays_کلیدیں;
            طالما (بچیا) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بچیا &
                    (xkb_overlay_نقاب_t)((بنانشانی)~بچیا + 1u)
                );
                بچیا = (بچیا & ~lsb);
                إذا (*overlays_کلیدیں) {
                    overlays_کلیدیں++;
                } وإلا {
                    /* drop current null value (overlayN=none) */
                    کلید->overlays &= ~lsb;
                }
            }

            إذا (کلید->overlays) {
                /* Steal */
                کلید->overlays_کلیدیں = keyi->overlays_کلیدیں;
                keyi->overlays_کلیدیں = عدم;
                keyi->overlays_alloc = 0;

                کلید->overlays_inline = false;
                کلید->explicit |= EXPLICIT_OVERLAY;
            }
        } وإلا {
            کلید->overlay_کلید = keyi->overlay_کلید;
            کلید->overlays_inline = true;
            کلید->explicit |= EXPLICIT_OVERLAY;
        }
    }

    أرجع true;
}

ساكن bool
Copy_Mod_Map_Def_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_اندراج *اندراج)
{
    بنية xkb_کلید *کلید;

    إذا (!اندراج->have_علامت) {
        کلید = Xkb_کلید_By_Name(keymap, اندراج->u.کلید_Name, true);
        إذا (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    کلید_Name_متن(info->ctx, اندراج->u.کلید_Name),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, اندراج->mods));
            اندراج->mods = 0; /* disable */
            أرجع false;
        }
    }
    وإلا {
        کلید = لبھو_کلید_For_علامت(keymap, اندراج->u.کلید_Sym);
        إذا (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_علامت,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_متن(info->ctx, اندراج->u.کلید_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, اندراج->mods));
            اندراج->mods = 0; /* disable */
            أرجع false;
        }
    }

    /* Skip modMap None */
    إذا (اندراج->mods) {
        کلید->modmap |= اندراج->mods;
        اندراج->کلید_کوڈ = کلید->keycode;
    }

    أرجع true;
}

ساكن bool
Copy_Symbols_منزل_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    کلید_Info *keyi;

    keymap->symbols_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->عدد_گروہ_ناں = (xkb_layout_اشاریہ_t)darray_ناپ(info->گروہ_ناں);
    darray_steal(info->گروہ_ناں, &keymap->گروہ_ناں, عدم);

    darray_foreach(keyi, info->کلیدیں)
        إذا (!Copy_Symbols_Def_منزل_Keymap(keymap, info, keyi))
            info->error_گنتی++;

    إذا (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_کلید *کلید;

        xkb_کلیدیں_foreach(کلید, keymap) {
            إذا (کلید->name == XKB_ATOM_NONE)
                استمر;

            إذا (کلید->عدد_groups < 1)
                log_info(info->ctx, XKB_LOG_سنیہا_NO_ID,
                         "No symbols defined for %s\n",
                         کلید_Name_متن(info->ctx, کلید->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_اندراج *mm;
    darray_foreach(mm, info->modmaps)
        إذا (!Copy_Mod_Map_Def_منزل_Keymap(keymap, info, mm))
            info->error_گنتی++;

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

    إذا (info.error_گنتی != 0)
        اذهب err_info;

    إذا (!Copy_Symbols_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    أرجع true;

err_info:
    ClearSymbolsInfo(&info);
    أرجع false;
}
