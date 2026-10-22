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
    کلید_REPEAT_رقم_ENTRIES,
};
تعداد { کلید_REPEAT_MIN_عرض = 3 /* 2 bits + sign */ };
static_assert(کلید_REPEAT_رقم_ENTRIES - 1 <=
              (1 << (کلید_REPEAT_MIN_عرض - 1)) - 1,
              "Cannot encode key repeat");

تعداد گروه_فیلد {
    گروه_فیلد_SYMS = (1 << 0),
    گروه_فیلد_ACTS = (1 << 1),
    گروه_فیلد_TYPE = (1 << 2),
};

تعداد کلید_فیلد {
    کلید_فیلد_NONE = 0,
    کلید_فیلد_REPEAT = (1 << 0),
    کلید_فیلد_DEFAULT_TYPE = (1 << 1),
    کلید_فیلد_GROUPINFO = (1 << 2),
    کلید_فیلد_VMODMAP = (1 << 3),
    کلید_فیلد_OVERLAY = (1 << 4),
    کلید_فیلد_ALL = کلید_فیلد_REPEAT | کلید_فیلد_DEFAULT_TYPE
                  | کلید_فیلد_GROUPINFO | کلید_فیلد_VMODMAP | کلید_فیلد_OVERLAY,
};
تعداد { کلید_فیلد_MIN_عرض = 6 /* 5 bits + sign */ };
static_assert(کلید_فیلد_ALL <= (1 << (کلید_فیلد_MIN_عرض - 1)) - 1,
              "Cannot encode key field");

تعريفنوع بنية {
    darray(بنية xkb_level) سطوح;
    تعداد گروه_فیلد defined;
    xkb_atom_t type;
} گروه_Info_2;

تعداد {
    XKB_LAYOUT_بیرون_از_RANGE_POLICY_MIN_عرض = 4 /* 3 bits + sign */,
    KEYINFO_بیرون_از_RANGE_گروه_POLICY_بیت_فیلد = 8,
    KEYINFO_DEFINED_بیت_فیلد = 16,
    KEYINFO_ادغام_بیت_فیلد = 8,
    KEYINFO_REPEAT_بیت_فیلد = 8,
};
static_assert(
    XKB_LAYOUT_بیرون_از_RANGE_WRAP < XKB_LAYOUT_بیرون_از_RANGE_CLAMP &&
    XKB_LAYOUT_بیرون_از_RANGE_CLAMP < XKB_LAYOUT_بیرون_از_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_بیرون_از_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_بیرون_از_RANGE_POLICY_MIN_عرض) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_بیرون_از_RANGE_گروه_POLICY_بیت_فیلد >=
              XKB_LAYOUT_بیرون_از_RANGE_POLICY_MIN_عرض, "");
static_assert((بیعلامت)KEYINFO_DEFINED_بیت_فیلد >=
              (بیعلامت)کلید_فیلد_MIN_عرض, "Cannot encode KeyInfo field");
static_assert((بیعلامت)KEYINFO_ادغام_بیت_فیلد >=
              (بیعلامت)ادغام_نما_MIN_عرض, "Cannot encode KeyInfo field");
static_assert((بیعلامت)KEYINFO_REPEAT_بیت_فیلد >=
              (بیعلامت)کلید_REPEAT_MIN_عرض, "Cannot encode KeyInfo field");

تعريفنوع بنية {
    xkb_atom_t name;

    xkb_mod_ماسک_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_شاخص_t بیرون_از_range_گروه_عدد;
    darray(گروه_Info_2) groups;
    تعداد xkb_layout_بیرون_از_range_policy
        بیرون_از_range_گروه_policy:KEYINFO_بیرون_از_RANGE_گروه_POLICY_بیت_فیلد;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    تعداد کلید_فیلد defined:KEYINFO_DEFINED_بیت_فیلد;
    تعداد ادغام_نما ادغام:KEYINFO_ادغام_بیت_فیلد;
    تعداد کلید_repeat repeat:KEYINFO_REPEAT_بیت_فیلد;

    bool بیرون_از_range_pending_گروه:1;

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
        ثابت بنية xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_کلید **overlays_کلیدها;
    };
} کلید_Info;

ساكن فراغ
StealLevelInfo(بنية xkb_level *into, بنية xkb_level *from)
{
    clear_level(into);
    إذا (from->رقم_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } وإلا {
        into->s.sym = from->s.sym;
    }
    into->رقم_syms = from->رقم_syms;
    from->رقم_syms = 0;

    إذا (from->رقم_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } وإلا {
        into->a.action = from->a.action;
    }
    into->رقم_actions = from->رقم_actions;
    from->رقم_actions = 0;
}

ساكن فراغ
Init_گروه_Info(گروه_Info_2 *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساكن فراغ
Clear_گروه_Info(گروه_Info_2 *groupi)
{
    بنية xkb_level *leveli;
    darray_foreach(leveli, groupi->سطوح)
        clear_level(leveli);
    darray_free(groupi->سطوح);
}

ساكن فراغ
Copy_گروه_Info(گروه_Info_2 *مقصد, ثابت گروه_Info_2 *from)
{
    مقصد->defined = from->defined;
    مقصد->type = from->type;
    darray_init(مقصد->سطوح);
    darray_copy(مقصد->سطوح, from->سطوح);
    لكل (xkb_level_شاخص_t j = 0; j < darray_اندازه(مقصد->سطوح); j++) {
        إذا (darray_قلم(from->سطوح, j).رقم_syms > 1) {
            darray_قلم(مقصد->سطوح, j).s.syms =
                memdup(darray_قلم(from->سطوح, j).s.syms,
                       darray_قلم(from->سطوح, j).رقم_syms,
                       حجم(xkb_keysym_t));
        }
        إذا (darray_قلم(from->سطوح, j).رقم_actions > 1) {
            darray_قلم(مقصد->سطوح, j).a.actions =
                memdup(darray_قلم(from->سطوح, j).a.actions,
                       darray_قلم(from->سطوح, j).رقم_actions,
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
    keyi->بیرون_از_range_گروه_policy = XKB_LAYOUT_بیرون_از_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_کلید = عدم;
}

ساكن فراغ
Clear_کلید_Info(کلید_Info *keyi)
{
    گروه_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_گروه_Info(groupi);
    darray_free(keyi->groups);
    إذا (keyi->overlays_alloc)
        حرر(keyi->overlays_کلیدها);
}

/***====================================================================***/

تعريفنوع بنية {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحيح error_شمار;
    بیعلامت صحيح include_عمق;
    xkb_layout_شاخص_t explicit_گروه;
    xkb_layout_شاخص_t max_groups;
    darray(کلید_Info) کلیدها;
    کلید_Info default_کلید;
    ActionsInfo default_actions;
    darray(xkb_atom_t) گروه_نامها;
    darray(Mod_Map_مدخل) modmaps;
    بنية xkb_mod_مجموعه mods;

    بنية xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت بنية xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساكن فراغ
InitSymbolsInfo(SymbolsInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                بیعلامت صحيح include_عمق, ثابت بنية xkb_mod_مجموعه *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_عمق = include_عمق;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_کلید_Info(keymap_info->keymap.ctx, &info->default_کلید);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_عمق > 0);
    info->explicit_گروه = XKB_LAYOUT_INVALID;
}

ساكن فراغ
ClearSymbolsInfo(SymbolsInfo *info)
{
    کلید_Info *keyi;
    حرر(info->name);
    darray_foreach(keyi, info->کلیدها)
        Clear_کلید_Info(keyi);
    darray_free(info->کلیدها);
    darray_free(info->گروه_نامها);
    darray_free(info->modmaps);
    Clear_کلید_Info(&info->default_کلید);
}

ساكن ثابت حرف *
کلید_Info_متن(SymbolsInfo *info, کلید_Info *keyi)
{
    أرجع کلید_Name_متن(info->ctx, keyi->name);
}

ساكن bool
ادغام_Groups(SymbolsInfo *info, گروه_Info_2 *into, گروه_Info_2 *from, bool clobber,
            bool report, xkb_layout_شاخص_t گروه, xkb_atom_t کلید_name)
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
                         گروه + 1, کلید_Name_متن(info->ctx, کلید_name),
                         xkb_atom_متن(info->ctx, use),
                         xkb_atom_متن(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & گروه_فیلد_TYPE);

    /* Now look at the levels. */

    إذا (darray_خالی(from->سطوح)) {
        Init_گروه_Info(from);
        أرجع true;
    }

    إذا (darray_خالی(into->سطوح)) {
        from->type = into->type;
        *into = *from;
        Init_گروه_Info(from);
        أرجع true;
    }

    /* Merge the actions and syms. */
    ثابت darray_اندازه_t سطوح_درون_both =
        MIN(darray_اندازه(into->سطوح), darray_اندازه(from->سطوح));
    darray_اندازه_t from_Keysyms_شمار = 0;
    darray_اندازه_t from_Actions_شمار = 0;
    لكل (darray_اندازه_t i = 0; i < سطوح_درون_both; i++) {
        بنية xkb_level* ثابت intoLevel = &darray_قلم(into->سطوح, i);
        بنية xkb_level* ثابت fromLevel = &darray_قلم(from->سطوح, i);

        ثابت bool fromHasNoKeysym = fromLevel->رقم_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->رقم_actions == 0;
        إذا (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            استمر;
        }

        ثابت bool intoHasNoKeysym = intoLevel->رقم_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->رقم_actions == 0;
        إذا (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_شمار++;
            from_Actions_شمار++;
            استمر;
        }

        /* Possible level conflict */
        أكد(intoLevel->رقم_syms > 0 || intoLevel->رقم_actions > 0);
        أكد(fromLevel->رقم_syms > 0 || fromLevel->رقم_actions > 0);

        /* Handle keysyms */
        إذا (!Xkb_سطوح_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            إذا (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_نماد,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, گروه + 1, کلید_Name_متن(info->ctx, کلید_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            إذا (fromHasNoKeysym) {
                /* No keysym to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined keysym from `from` */
                إذا (unlikely(fromLevel->رقم_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->رقم_syms > 1))
                        حرر(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->رقم_syms = fromLevel->رقم_syms;
                    fromLevel->رقم_syms = 0;
                    from_Keysyms_شمار++;
                } وإلا إذا (fromLevel->s.sym != XKB_کلید_No_نماد) {
                    /* Single defined keysym */
                    إذا (unlikely(intoLevel->رقم_syms > 1))
                        حرر(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->رقم_syms = 1;
                    from_Keysyms_شمار++;
                }
            } وإلا {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->رقم_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->s.sym == XKB_کلید_No_نماد) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->رقم_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->رقم_syms = fromLevel->رقم_syms;
                        fromLevel->رقم_syms = 0;
                    } وإلا {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->رقم_syms = 1;
                    }
                    from_Keysyms_شمار++;
                }
            }
        }

        /* Handle actions */
        إذا (!Xkb_سطوح_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            إذا (report && !(intoHasNoAction || fromHasNoAction)) {
                إذا (unlikely(intoLevel->رقم_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, گروه + 1, کلید_Name_متن(info->ctx, کلید_name),
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
                                i + 1, گروه + 1,
                                کلید_Name_متن(info->ctx, کلید_name),
                                Action_Type_متن(use->type),
                                Action_Type_متن(ignore->type));
                }
            }
            إذا (fromHasNoAction) {
                /* No action to copy */
            } وإلا إذا (clobber) {
                /* Override: copy any defined action from `from` */
                إذا (unlikely(fromLevel->رقم_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    إذا (unlikely(intoLevel->رقم_actions > 1))
                        حرر(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->رقم_actions = fromLevel->رقم_actions;
                    fromLevel->رقم_actions = 0;
                    from_Actions_شمار++;
                } وإلا إذا (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    إذا (unlikely(intoLevel->رقم_actions > 1))
                        حرر(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->رقم_actions = 1;
                    from_Actions_شمار++;
                }
            } وإلا {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                إذا (unlikely(intoLevel->رقم_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } وإلا إذا (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    إذا (unlikely(fromLevel->رقم_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->رقم_actions = fromLevel->رقم_actions;
                        fromLevel->رقم_actions = 0;
                    } وإلا {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->رقم_actions = 1;
                    }
                    from_Actions_شمار++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    بنية xkb_level *level;
    darray_foreach_from(level, from->سطوح, سطوح_درون_both) {
        darray_append(into->سطوح, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->رقم_syms = 0;
        level->رقم_actions = 0;
        from_Keysyms_شمار++;
        from_Actions_شمار++;
    }
    إذا (from_Keysyms_شمار) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        إذا (from_Keysyms_شمار == darray_اندازه(into->سطوح))
            into->defined &= ~گروه_فیلد_SYMS;
        into->defined |= (from->defined & گروه_فیلد_SYMS);
    }
    إذا (from_Actions_شمار) {
        /* Reset defined actions field if we used no action from `into` */
        إذا (from_Actions_شمار == darray_اندازه(into->سطوح))
            into->defined &= ~گروه_فیلد_ACTS;
        into->defined |= (from->defined & گروه_فیلد_ACTS);
    }

    أرجع true;
}

ساكن bool
Use_New_کلید_فیلد(تعداد کلید_فیلد فیلد_2, تعداد کلید_فیلد old, تعداد کلید_فیلد new,
               bool clobber, bool report, تعداد کلید_فیلد *collide)
{
    إذا (!(old & فیلد_2))
        أرجع (new & فیلد_2);

    إذا (new & فیلد_2) {
        إذا (report)
            *collide |= فیلد_2;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
overlays_get(ثابت کلید_Info *info, xkb_overlay_شاخص_t بیت,
             ثابت بنية xkb_کلید **کلید_بیرون)
{
    إذا (بیت >= (xkb_overlay_شاخص_t)(حجم(xkb_overlay_ماسک_t) * CHAR_BIT))
        أرجع false;
    ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << بیت);
    إذا (!(info->overlays & ماسک)) أرجع false;
    إذا (کلید_بیرون) {
        إذا (!info->overlays_alloc) {
            /* Stored inline */
            *کلید_بیرون = info->overlay_کلید;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
                info->overlays & (ماسک - 1u)
            );
            ثابت xkb_overlay_شاخص_t شاخص = (xkb_overlay_شاخص_t)popcount32(کم);
            *کلید_بیرون = info->overlays_کلیدها[شاخص];
        }
    }
    أرجع true;
}

ساكن bool
overlays_درج(کلید_Info *keyi, xkb_overlay_شاخص_t بیت,
                ثابت بنية xkb_کلید *کلید)
{
    إذا (بیت >= (xkb_overlay_شاخص_t)(حجم(xkb_overlay_ماسک_t) * CHAR_BIT))
        أرجع false;

    ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << بیت);

    إذا ((keyi->overlays & ماسک) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        إذا (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_کلید = کلید;
            إذا (!کلید)
                keyi->overlays_clear = true;
        } وإلا {
            /* Stored in sparse array */
            ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
                keyi->overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_شاخص_t شاخص =
                (xkb_overlay_شاخص_t)popcount32(کم);
            keyi->overlays_کلیدها[شاخص] = کلید;
        }
        أرجع true;
    }

    /* Insert new element */
    إذا (!keyi->overlays || (keyi->overlays_clear && !کلید)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_کلید = کلید;
        keyi->overlays |= ماسک;
        keyi->overlays_clear = !کلید;
    } وإلا إذا (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_ماسک_t overlays = keyi->overlays | ماسک;
        ثابت xkb_overlay_شاخص_t alloc =
            (xkb_overlay_شاخص_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت بنية xkb_کلید ** ثابت tmp = حافظه_صفر_بده(alloc, حجم(*tmp));
        إذا (!tmp) أرجع false;

        /* Copy previous entries to the buffer */
        إذا (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
                overlays & (xkb_overlay_ماسک_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_شاخص_t idx = (xkb_overlay_شاخص_t)popcount32(کم);
            tmp[idx] = keyi->overlay_کلید;
        } وإلا {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
            overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
        );
        ثابت xkb_overlay_شاخص_t idx = (xkb_overlay_شاخص_t)popcount32(کم);
        tmp[idx] = کلید;

        keyi->overlays_کلیدها = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } وإلا {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_ماسک_t overlays =
            (xkb_overlay_ماسک_t)(keyi->overlays | ماسک);
        ثابت xkb_overlay_شاخص_t شمار =
            (xkb_overlay_شاخص_t)popcount32(overlays);
        إذا (شمار > keyi->overlays_alloc) {
            ثابت xkb_overlay_شاخص_t alloc =
                (xkb_overlay_شاخص_t)بعدی_pow2(شمار);
            أكد(alloc >= شمار);
            ثابت بنية xkb_کلید ** ثابت tmp = realloc(
                keyi->overlays_کلیدها,
                (اندازه_t)alloc * حجم(*keyi->overlays_کلیدها)
            );
            إذا (!tmp) أرجع false;
            keyi->overlays_کلیدها = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
            overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
        );
        ثابت xkb_overlay_شاخص_t شاخص = (xkb_overlay_شاخص_t)popcount32(کم);
        إذا (شاخص >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        إذا (شاخص < شمار - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(شاخص + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_کلیدها + شاخص + 1, keyi->overlays_کلیدها + شاخص,
                    (اندازه_t)(شمار - 1u - شاخص) * حجم(ثابت بنية xkb_کلید *));
        }
        keyi->overlays_کلیدها[شاخص] = کلید;
        keyi->overlays = overlays;
    }

    أرجع true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساكن bool
ادغام_overlays(SymbolsInfo *info, کلید_Info *into, کلید_Info *from,
               bool clobber, bool report, تعداد کلید_فیلد *collide)
{
    إذا (!(from->defined & کلید_فیلد_OVERLAY)) {
        /* `from` is empty: do nothing */
    } وإلا إذا (!(into->defined & کلید_فیلد_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_کلیدها = from->overlays_کلیدها;
        from->overlays_کلیدها = عدم;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= کلید_فیلد_OVERLAY;
    } وإلا إذا (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } وإلا {
        إذا (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_ماسک_t نتیجه_ماسک =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_شاخص_t شمار =
                (xkb_overlay_شاخص_t)popcount32(نتیجه_ماسک);

            إذا (شمار == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            کلید_Info *dest = into;
            کلید_Info *src = from;
            إذا (into->overlays_alloc >= شمار &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } وإلا إذا (from->overlays_alloc >= شمار) {
                dest = from;
                src = into;
                clobber = !clobber;
            } وإلا إذا (شمار > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                إذا (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                إذا (!dest->overlays_alloc) {
                    ثابت بنية xkb_کلید **tmp = حافظه_صفر_بده(شمار, حجم(*tmp));
                    إذا (!tmp)
                        أرجع false;
                    إذا (dest->overlay_کلید)
                        tmp[0] = dest->overlay_کلید;
                    dest->overlays_کلیدها = tmp;
                    dest->overlays_alloc = شمار;
                } وإلا {
                    ثابت بنية xkb_کلید **tmp =
                        realloc(dest->overlays_کلیدها,
                                شمار * حجم(*dest->overlays_کلیدها));
                    إذا (!tmp)
                        أرجع false;
                    dest->overlays_کلیدها = tmp;
                    dest->overlays_alloc = شمار;
                }
            }

            xkb_overlay_ماسک_t مانده = src->overlays;
            ثابت بنية xkb_کلید **src_کلیدها = (src->overlays_clear)
                ? عدم
                : (src->overlays_alloc)
                    ? src->overlays_کلیدها
                    : &src->overlay_کلید;
            طالما (مانده) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                    مانده &
                    (xkb_overlay_ماسک_t)((بیعلامت)~مانده + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_شاخص_t بیت =
                    (xkb_overlay_شاخص_t)popcount32(lsb - 1u);
                مانده = (مانده & ~lsb);

                /* pop current value */
                إذا (!src->overlays_clear && !src->overlays_alloc && مانده)
                    PANIC_UNREACHABLE();
                ثابت بنية xkb_کلید *src_کلید = (src_کلیدها)
                    ? *(src_کلیدها++)
                    : عدم;

                /* check dest */
                ثابت بنية xkb_کلید *dest_کلید = عدم;
                ثابت bool conflict = overlays_get(dest, بیت, &dest_کلید);
                إذا (conflict) {
                    إذا (dest_کلید == src_کلید)
                       استمر;
                    وإلا إذا (report)
                        *collide |= کلید_فیلد_OVERLAY;
                }

                إذا ((!conflict || clobber) && !overlays_درج(dest, بیت, src_کلید))
                    أرجع false;
            }

            إذا (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                إذا (into->overlays_alloc)
                    حرر(into->overlays_کلیدها);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                إذا (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_کلیدها = from->overlays_کلیدها;
                    from->overlays_کلیدها = عدم;
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
                *collide |= کلید_فیلد_OVERLAY;
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
ادغام_کلیدها(SymbolsInfo *info, کلید_Info *into, کلید_Info *from, bool same_file)
{
    xkb_layout_شاخص_t i;
    xkb_layout_شاخص_t groups_درون_both;
    تعداد کلید_فیلد collide = 0;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->ادغام != ادغام_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (from->ادغام == ادغام_REPLACE) {
        Clear_کلید_Info(into);
        *into = *from;
        Init_کلید_Info(info->ctx, from);
        أرجع true;
    }

    groups_درون_both = MIN(darray_اندازه(into->groups), darray_اندازه(from->groups));
    لكل (i = 0; i < groups_درون_both; i++)
        ادغام_Groups(info,
                    &darray_قلم(into->groups, i),
                    &darray_قلم(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    لكل (i = groups_درون_both; i < darray_اندازه(from->groups); i++) {
        darray_append(into->groups, darray_قلم(from->groups, i));
        Init_گروه_Info(&darray_قلم(from->groups, i));
    }

    إذا (Use_New_کلید_فیلد(کلید_فیلد_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= کلید_فیلد_VMODMAP;
    }
    إذا (Use_New_کلید_فیلد(کلید_فیلد_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= کلید_فیلد_REPEAT;
    }
    إذا (Use_New_کلید_فیلد(کلید_فیلد_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= کلید_فیلد_DEFAULT_TYPE;
    }
    إذا (Use_New_کلید_فیلد(کلید_فیلد_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->بیرون_از_range_pending_گروه = from->بیرون_از_range_pending_گروه;
        into->بیرون_از_range_گروه_policy = from->بیرون_از_range_گروه_policy;
        into->بیرون_از_range_گروه_عدد = from->بیرون_از_range_گروه_عدد;
        into->defined |= کلید_فیلد_GROUPINFO;
    }

    إذا (!ادغام_overlays(info, into, from, clobber, report, &collide))
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
    إذا (name < keymap->رقم_کلید_نامها) {
        ثابت Keycode_تطابق تطابق = keymap->کلید_نامها[name];
        إذا (تطابق.یافت && تطابق.is_alias) {
            أرجع تطابق.alias.real;
        }
    }
    أرجع name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساكن bool
جمع_کن_کلید_Symbols(SymbolsInfo *info, کلید_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_کلید_Alias(&info->keymap_info->keymap, keyi->name);

    کلید_Info *iter;
    darray_foreach(iter, info->کلیدها)
        إذا (iter->name == keyi->name)
            أرجع ادغام_کلیدها(info, iter, keyi, same_file);

    darray_append(info->کلیدها, *keyi);
    Init_کلید_Info(info->ctx, keyi);
    أرجع true;
}

ساكن bool
جمع_کن_Mod_Map_مدخل(SymbolsInfo *info, ثابت Mod_Map_مدخل *new)
{
    ثابت bool clobber = (new->ادغام != ادغام_AUGMENT);

    Mod_Map_مدخل *old;
    darray_foreach(old, info->modmaps) {
        إذا ((new->have_نماد != old->have_نماد) ||
            (new->have_نماد && new->u.کلید_Sym != old->u.کلید_Sym) ||
            (!new->have_نماد && new->u.کلید_Name != old->u.کلید_Name))
            استمر;

        إذا (new->mods == old->mods)
            أرجع true;

        ثابت xkb_mod_ماسک_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_ماسک_t ignore = (clobber ? old->mods : new->mods);

        إذا (new->have_نماد) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_متن(info->ctx, new->u.کلید_Sym),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        } وإلا {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    کلید_Name_متن(info->ctx, new->u.کلید_Name),
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
ادغام_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     تعداد ادغام_نما ادغام)
{
    xkb_atom_t *گروه_name;
    xkb_layout_شاخص_t گروه_نامها_درون_both;

    إذا (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        أرجع;
    }

    ادغام_Mod_Sets(into->ctx, &into->mods, &from->mods, ادغام);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    گروه_نامها_درون_both = MIN(darray_اندازه(into->گروه_نامها),
                              darray_اندازه(from->گروه_نامها));
    لكل (xkb_layout_شاخص_t i = 0; i < گروه_نامها_درون_both; i++) {
        إذا (!darray_قلم(from->گروه_نامها, i))
            استمر;

        إذا (ادغام == ادغام_AUGMENT && darray_قلم(into->گروه_نامها, i))
            استمر;

        darray_قلم(into->گروه_نامها, i) = darray_قلم(from->گروه_نامها, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(گروه_name, from->گروه_نامها, گروه_نامها_درون_both)
        darray_append(into->گروه_نامها, *گروه_name);

    إذا (darray_خالی(into->کلیدها)) {
        into->کلیدها = from->کلیدها;
        darray_init(from->کلیدها);
    }
    وإلا {
        کلید_Info *keyi;
        darray_foreach(keyi, from->کلیدها) {
            keyi->ادغام = ادغام;
            إذا (!جمع_کن_کلید_Symbols(into, keyi, false))
                into->error_شمار++;
        }
    }

    إذا (darray_خالی(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    وإلا {
        Mod_Map_مدخل *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ادغام = ادغام;
            إذا (!جمع_کن_Mod_Map_مدخل(into, mm))
                into->error_شمار++;
        }
    }
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساكن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    إذا (Exceeds_Include_Max_عمق(info->ctx, info->include_عمق)) {
        info->error_شمار += 10;
        أرجع false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_عمق + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بعدی_incl) {
        SymbolsInfo بعدی_incl;
        XkbFile *file;

        حرف مسیر[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  مسیر, حجم(مسیر));
        إذا (!file) {
            info->error_شمار += 10;
            ClearSymbolsInfo(&included);
            أرجع false;
        }

        InitSymbolsInfo(&بعدی_incl, info->keymap_info, info->include_عمق + 1,
                        &included.mods);
        إذا (stmt->modifier) {
            ثابت صحيح شمار = parse_dec_مقصد_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &بعدی_incl.explicit_گروه);
            إذا (شمار <= 0 || *(stmt->modifier + شمار) != '\0' ||
                بعدی_incl.explicit_گروه == 0 ||
                بعدی_incl.explicit_گروه > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                بعدی_incl.explicit_گروه = info->explicit_گروه;
            } وإلا {
                بعدی_incl.explicit_گروه--;
            }
        }
        وإلا إذا (info->keymap_info->keymap.رقم_groups != 0 &&
                 بعدی_incl.include_عمق == 1) {
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
            بعدی_incl.explicit_گروه = 0;
        }
        وإلا {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            بعدی_incl.explicit_گروه = info->explicit_گروه;
        }

        HandleSymbolsFile(&بعدی_incl, file);

        ادغام_Included_Symbols(&included, &بعدی_incl, stmt->ادغام);

        ClearSymbolsInfo(&بعدی_incl);
        FreeXkbFile(file);
    }

    ادغام_Included_Symbols(info, &included, include->ادغام);
    ClearSymbolsInfo(&included);

    أرجع (info->error_شمار == 0);
}

ساكن bool
Get_گروه_شاخص(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
              تعداد گروه_فیلد فیلد_2, xkb_layout_شاخص_t *ndx_rtrn)
{
    أكد (فیلد_2 == گروه_فیلد_SYMS || فیلد_2 == گروه_فیلد_ACTS);
    ثابت حرف *name = (فیلد_2 == گروه_فیلد_SYMS ? "symbols" : "actions");

    إذا (arrayNdx == عدم) {
        xkb_layout_شاخص_t i = 0;
        گروه_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            إذا (!(groupi->defined & فیلد_2)) {
                *ndx_rtrn = i;
                أرجع true;
            }
        }

        إذا (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, کلید_Info_متن(info, keyi), info->max_groups, name);
            أرجع false;
        }

        darray_resize0(keyi->groups, darray_اندازه(keyi->groups) + 1);
        *ndx_rtrn = darray_اندازه(keyi->groups) - 1;
        أرجع true;
    }

    إذا (عبارت_Resolve_گروه(info->keymap_info, arrayNdx, false, ndx_rtrn, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, کلید_Info_متن(info, keyi));
        أرجع false;
    }

    (*ndx_rtrn)--;
    إذا (*ndx_rtrn >= darray_اندازه(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    أرجع true;
}

ساكن bool
جمع_کن_Symbols_مقصد_کلید(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
                عبارت_Def *مقدار)
{
    xkb_layout_شاخص_t ndx = 0;

    إذا (!Get_گروه_شاخص(info, keyi, arrayNdx, گروه_فیلد_SYMS, &ndx))
        أرجع false;

    گروه_Info_2 *groupi = &darray_قلم(keyi->groups, ndx);

    إذا (مقدار->common.type == STMT_عبارت_خالی_LIST) {
        groupi->defined |= گروه_فیلد_SYMS;
        أرجع true;
    }

    إذا (مقدار->common.type != STMT_عبارت_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_مقصد_string(مقدار->common.type), ndx + 1,
                کلید_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروه_فیلد_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_کلید_SYMBOLS_مدخل,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                کلید_Info_متن(info, keyi), ndx + 1);
        أرجع false;
    }

    xkb_level_شاخص_t n_سطوح = 0;
    xkb_level_شاخص_t non_خالی_سطوح = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    لكل (عبارت_Keysym_List *keysymList = (عبارت_Keysym_List *) مقدار;
         keysymList;
         keysymList = (عبارت_Keysym_List *) keysymList->common.بعدی) {
        n_سطوح++;
        /* Drop trailing NoSymbol */
        إذا (darray_اندازه(keysymList->syms) > 0)
            non_خالی_سطوح = n_سطوح;
    }
    إذا (non_خالی_سطوح < n_سطوح)
        n_سطوح = non_خالی_سطوح;
    إذا (darray_اندازه(groupi->سطوح) < n_سطوح)
        darray_resize0(groupi->سطوح, n_سطوح);

    groupi->defined |= گروه_فیلد_SYMS;

    xkb_level_شاخص_t level = 0;
    لكل (عبارت_Keysym_List *keysymList = (عبارت_Keysym_List *) مقدار;
         keysymList && level < n_سطوح;
         keysymList = (عبارت_Keysym_List *) keysymList->common.بعدی, level++) {
        بنية xkb_level *leveli = &darray_قلم(groupi->سطوح, level);
        أكد(leveli->رقم_syms == 0);

        إذا (unlikely(darray_اندازه(keysymList->syms) > MAX_KEYSYMS_در_هر_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_در_هر_LEVEL, darray_اندازه(keysymList->syms));
            أرجع false;
        }

        leveli->رقم_syms = (xkb_keysym_شمار_t) darray_اندازه(keysymList->syms);
        بدل (leveli->رقم_syms) {
        حالة 0:
            leveli->s.sym = XKB_کلید_No_نماد;
            اكسر;
        حالة 1:
            leveli->s.sym = darray_قلم(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_کلید_No_نماد);
            اكسر;
        افتراضي:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, عدم);
#اگر_تعریف_نشده NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            لكل (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++)
                أكد(leveli->s.syms[k] != XKB_کلید_No_نماد);
#پایان_اگر
        }
    }

    أرجع true;
}

ساكن bool
جمع_کن_Actions_مقصد_کلید(SymbolsInfo *info, کلید_Info *keyi, عبارت_Def *arrayNdx,
                عبارت_Def *مقدار)
{
    xkb_layout_شاخص_t ndx = 0;

    إذا (!Get_گروه_شاخص(info, keyi, arrayNdx, گروه_فیلد_ACTS, &ndx))
        أرجع false;

    گروه_Info_2 *groupi = &darray_قلم(keyi->groups, ndx);

    إذا (مقدار->common.type == STMT_عبارت_خالی_LIST) {
        groupi->defined |= گروه_فیلد_ACTS;
        أرجع true;
    }

    إذا (مقدار->common.type != STMT_عبارت_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 مقدار->common.type, ndx, کلید_Info_متن(info, keyi));
        أرجع false;
    }

    إذا (groupi->defined & گروه_فیلد_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 کلید_Info_متن(info, keyi), ndx);
        أرجع false;
    }

    xkb_level_شاخص_t n_سطوح = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    لكل (ParseCommon *p = &مقدار->common; p; p = p->بعدی)
        n_سطوح++;
    إذا (darray_اندازه(groupi->سطوح) < n_سطوح)
        darray_resize0(groupi->سطوح, n_سطوح);

    groupi->defined |= گروه_فیلد_ACTS;

    xkb_level_شاخص_t level = 0;
    xkb_level_شاخص_t non_خالی_سطوح = 0;
    لكل (عبارت_Action_List *actionList = (عبارت_Action_List *) مقدار;
         actionList;
         actionList = (عبارت_Action_List *) actionList->common.بعدی, level++) {
        بنية xkb_level *leveli = &darray_قلم(groupi->سطوح, level);
        أكد(leveli->رقم_actions == 0);

        بیعلامت صحيح رقم_actions = 0;
        لكل (عبارت_Def *act = actionList->actions;
             act; act = (عبارت_Def *) act->common.بعدی)
            رقم_actions++;

        إذا (unlikely(رقم_actions > MAX_ACTIONS_در_هر_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_در_هر_LEVEL, رقم_actions);
            أرجع false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        لكل (عبارت_Def *act = actionList->actions;
             act; act = (عبارت_Def *) act->common.بعدی) {
            اتحاد xkb_action مقصد_Act = { 0 };
            ثابت تعداد xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &مقصد_Act);
            إذا (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        کلید_Info_متن(info, keyi), ndx + 1, level + 1);
                إذا (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    أرجع false;
                } وإلا {
                    /* Ensure action type is reset */
                    مقصد_Act.type = ACTION_TYPE_NONE;
                }
            }
            إذا (مقصد_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } وإلا إذا (likely(رقم_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->رقم_actions = 1;
                leveli->a.action = مقصد_Act;
                اذهب بعدی_level;
            } وإلا {
                darray_append(actions, مقصد_Act);
            }
        }
        إذا (darray_خالی(actions)) {
            leveli->رقم_actions = 0;
        } وإلا إذا (likely(darray_اندازه(actions) > 1)) {
            leveli->رقم_actions = (xkb_action_شمار_t) darray_اندازه(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, عدم);
#اگر_تعریف_نشده NDEBUG
            /* Canonical list: all NoAction() were dropped */
            لكل (xkb_action_شمار_t k = 0; k < leveli->رقم_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#پایان_اگر
        } وإلا {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(رقم_actions > darray_اندازه(actions));
            leveli->رقم_actions = 1;
            leveli->a.action = darray_قلم(actions, 0);
            أكد(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
بعدی_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        إذا (leveli->رقم_actions > 0 || leveli->رقم_syms > 0)
            non_خالی_سطوح = level + 1;
    }

    إذا (non_خالی_سطوح < n_سطوح) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_اندازه(groupi->سطوح) = non_خالی_سطوح;
        إذا (non_خالی_سطوح > 0)
            darray_shrink(groupi->سطوح);
        وإلا
            darray_free(groupi->سطوح);
    }

    أرجع true;
}

ساكن ثابت Lookup_مدخل repeatEntries[] = {
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
عبارت_Resolve_Overlay_مدخل(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت حرف *فیلد_2, ثابت عبارت_Def *arrayNdx,
                        ثابت عبارت_Def *عبارت, کلید_Info *keyi,
                        xkb_overlay_شاخص_t *overlay_rtrn,
                        ثابت بنية xkb_کلید **کلید_rtrn)
{
    /* Overlay index */
    إذا (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                فیلد_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    static_assert(حجم(xkb_overlay_شاخص_t) == حجم(uint8_t), "");
    ثابت اندازه_t پیشوند = حجم("overlay") - 1;
    ثابت اندازه_t len = طول_رشته(فیلد_2 + پیشوند);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    إذا (parse_dec_مقصد_uint64_t(فیلد_2 + پیشوند, len, (uint64_t *)&raw_overlay)
        != (صحيح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_شاخص,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                فیلد_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        أرجع false;
    }
    *overlay_rtrn = (xkb_overlay_شاخص_t)raw_overlay - 1;

    /* Overlay key */
    بدل (عبارت->common.type) {
    حالة STMT_عبارت_KEYNAME_LITERAL:
        *کلید_rtrn = Xkb_کلید_By_Name(&keymap_info->keymap, عبارت->کلید_name.کلید_name,
                                 false);
        إذا (!*کلید_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_متن(keymap_info->keymap.ctx, عبارت->کلید_name.کلید_name),
                    فیلد_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
            أرجع false;
        }
        أرجع true;
    حالة STMT_عبارت_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_متن(keymap_info->keymap.ctx, عبارت->ident.ident);
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
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_مقدار,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, فیلد_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        أرجع false;
    }
    افتراضي:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_مقدار,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_مقصد_string(STMT_عبارت_KEYNAME_LITERAL),
                فیلد_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;
    }
}

ساكن bool
مجموعه_Symbols_فیلد(SymbolsInfo *info, کلید_Info *keyi, ثابت حرف *فیلد_2,
                عبارت_Def *arrayNdx, عبارت_Def **مقدار_ptr)
{
    عبارت_Def * محدود ثابت مقدار = *مقدار_ptr;
    إذا (istreq(فیلد_2, "type")) {
        xkb_layout_شاخص_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        إذا (!عبارت_Resolve_String(info->ctx, مقدار, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            أرجع false;
        }

        إذا (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= کلید_فیلد_DEFAULT_TYPE;
        }
        وإلا إذا (عبارت_Resolve_گروه(info->keymap_info, arrayNdx, false,
                                  &ndx, عدم) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }
        وإلا {
            ndx--;
            إذا (ndx >= darray_اندازه(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_اندازه(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_قلم(keyi->groups, ndx).type = val;
            darray_قلم(keyi->groups, ndx).defined |= گروه_فیلد_TYPE;
        }
    }
    وإلا إذا (istreq(فیلد_2, "symbols")) {
        أرجع جمع_کن_Symbols_مقصد_کلید(info, keyi, arrayNdx, مقدار);
    }
    وإلا إذا (istreq(فیلد_2, "actions")) {
        أرجع جمع_کن_Actions_مقصد_کلید(info, keyi, arrayNdx, مقدار);
    }
    وإلا إذا (istreq(فیلد_2, "vmods") ||
             istreq(فیلد_2, "virtualmods") ||
             istreq(فیلد_2, "virtualmodifiers")) {
        xkb_mod_ماسک_t ماسک = 0;

        إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, مقدار, MOD_VIRT, &info->mods,
                                &ماسک)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_مقصد_string(مقدار->common.type),
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->vmodmap = ماسک;
        keyi->defined |= کلید_فیلد_VMODMAP;
    }
    وإلا إذا (istreq(فیلد_2, "locking") ||
             istreq(فیلد_2, "lock") ||
             istreq(فیلد_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq(فیلد_2, "radiogroup") ||
             istreq(فیلد_2, "permanentradiogroup") ||
             istreq(فیلد_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_پیشوند("permanentoverlay", فیلد_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    وإلا إذا (istreq_پیشوند("overlay", فیلد_2)) {
        xkb_overlay_شاخص_t overlay = XKB_OVERLAY_INVALID;
        ثابت بنية xkb_کلید *کلید = عدم;
        إذا (!عبارت_Resolve_Overlay_مدخل(info->keymap_info, فیلد_2, arrayNdx,
                                     *مقدار_ptr, keyi, &overlay, &کلید))
            أرجع false;
        إذا (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            أرجع true;
        } وإلا إذا (کلید && کلید->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_پیام_NO_ID,
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
                إذا (!overlays_درج(keyi, overlay, کلید))
                    أرجع false;
                keyi->defined |= کلید_فیلد_OVERLAY;
            } وإلا {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_ماسک_t ماسک = (xkb_overlay_ماسک_t)(1u << overlay);
                إذا (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    إذا (کلید) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = ماسک;
                        keyi->overlays_clear = false;
                        keyi->overlay_کلید = کلید;
                    } وإلا {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= ماسک;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= کلید_فیلد_OVERLAY;
                } وإلا إذا (keyi->overlays) {
                    إذا (کلید) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                کلید_Info_متن(info, keyi), keyi->overlays,
                                کلید_Name_متن(info->ctx, keyi->overlay_کلید->name),
                                overlay + 1, کلید_Name_متن(info->ctx, کلید->name));
                        أرجع !(info->keymap_info->strict &
                                 PARSER_NO_فیلد_مقدار_MISMATCH);
                    } وإلا {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    وإلا إذا (istreq(فیلد_2, "repeating") ||
             istreq(فیلد_2, "repeats") ||
             istreq(فیلد_2, "repeat")) {
        uint32_t val = 0;

        إذا (!عبارت_Resolve_Enum(info->ctx, مقدار, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->repeat = (تعداد کلید_repeat) val;
        keyi->defined |= کلید_فیلد_REPEAT;
    }
    وإلا إذا (istreq(فیلد_2, "groupswrap") ||
             istreq(فیلد_2, "wrapgroups")) {
        bool مجموعه = false;

        إذا (!عبارت_Resolve_Boolean(info->ctx, مقدار, &مجموعه)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->بیرون_از_range_گروه_policy = (مجموعه)
            ? XKB_LAYOUT_بیرون_از_RANGE_WRAP
            : XKB_LAYOUT_بیرون_از_RANGE_CLAMP;
        keyi->defined |= کلید_فیلد_GROUPINFO;
    }
    وإلا إذا (istreq(فیلد_2, "groupsclamp") ||
             istreq(فیلد_2, "clampgroups")) {
        bool مجموعه = false;

        إذا (!عبارت_Resolve_Boolean(info->ctx, مقدار, &مجموعه)) {
            log_err(info->ctx, XKB_ERROR_INVALID_مقدار,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        keyi->بیرون_از_range_گروه_policy = (مجموعه)
            ? XKB_LAYOUT_بیرون_از_RANGE_CLAMP
            : XKB_LAYOUT_بیرون_از_RANGE_WRAP;
        keyi->defined |= کلید_فیلد_GROUPINFO;
    }
    وإلا إذا (istreq(فیلد_2, "groupsredirect") ||
             istreq(فیلد_2, "redirectgroups")) {
        xkb_layout_شاخص_t grp = 0;
        bool pending = false;

        // TODO: recover?
        إذا (عبارت_Resolve_گروه(info->keymap_info, مقدار, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    کلید_Info_متن(info, keyi));
            أرجع false;
        }

        إذا (pending) {
            keyi->بیرون_از_range_pending_گروه = true;
            ثابت darray_اندازه_t pending_شاخص =
                darray_اندازه(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (بنية pending_computation) {
                    .عبارت = *مقدار_ptr,
                    .computed = false,
                    .مقدار = 0,
                }
            );
            *مقدار_ptr = عدم;
            static_assert(حجم(keyi->بیرون_از_range_گروه_عدد) ==
                          حجم(pending_شاخص),
                          "Cannot save pending computation");
            keyi->بیرون_از_range_گروه_عدد = pending_شاخص;
        } وإلا {
            keyi->بیرون_از_range_pending_گروه = false;
            keyi->بیرون_از_range_گروه_عدد = grp - 1;
        }

        keyi->بیرون_از_range_گروه_policy = XKB_LAYOUT_بیرون_از_RANGE_REDIRECT;
        keyi->defined |= کلید_فیلد_GROUPINFO;
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_فیلد,
                "Unknown field \"%s\" in a key; definition ignored\n",
                فیلد_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_کلید_FIELDS);
    }

    أرجع true;
}

ساكن bool
مجموعه_گروه_Name(SymbolsInfo *info, عبارت_Def *arrayNdx, عبارت_Def *مقدار,
             تعداد ادغام_نما ادغام)
{
    إذا (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_گروه_NAME_شاخص,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        أرجع false;
    }

    xkb_layout_شاخص_t گروه = 0;
    إذا (عبارت_Resolve_گروه(info->keymap_info, arrayNdx, false, &گروه, عدم) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!عبارت_Resolve_String(info->ctx, مقدار, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", گروه);
        أرجع false;
    }

    xkb_layout_شاخص_t گروه_مقصد_use;
    إذا (info->explicit_گروه == XKB_LAYOUT_INVALID) {
        گروه_مقصد_use = گروه - 1;
    }
    وإلا إذا (گروه - 1 == 0) {
        گروه_مقصد_use = info->explicit_گروه;
    }
    وإلا {
        log_warn(info->ctx, XKB_WARNING_NON_پایه_گروه_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, گروه,
                 xkb_atom_متن(info->ctx, name));
        أرجع false;
    }

    إذا (گروه_مقصد_use >= darray_اندازه(info->گروه_نامها)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_اندازه(info->گروه_نامها) < گروه_مقصد_use + 1);
        darray_resize0(info->گروه_نامها, گروه_مقصد_use + 1);
    } وإلا {
        ثابت xkb_atom_t old_name = darray_قلم(info->گروه_نامها, گروه_مقصد_use);
        إذا (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (ادغام != ادغام_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     گروه_مقصد_use, info->name,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
            name = use;
        }
    }
    darray_قلم(info->گروه_نامها, گروه_مقصد_use) = name;

    أرجع true;
}

ساكن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *arrayNdx;
    bool ret;

    إذا (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &فیلد_2, &arrayNdx))
        أرجع false;

    إذا (elem && istreq(elem, "key")) {
        کلید_Info موقت = {0};
        Init_کلید_Info(info->ctx, &موقت);
        /* Do not replace the whole key, only the current field */
        موقت.ادغام = (موقت.ادغام == ادغام_REPLACE)
            ? ادغام_OVERRIDE
            : stmt->ادغام;
        ret = مجموعه_Symbols_فیلد(info, &موقت, فیلد_2, arrayNdx, &stmt->مقدار) &&
              ادغام_کلیدها(info, &info->default_کلید, &موقت, true);
        Clear_کلید_Info(&موقت);
    }
    وإلا إذا (!elem && (istreq(فیلد_2, "name") ||
                       istreq(فیلد_2, "groupname"))) {
        ret = مجموعه_گروه_Name(info, arrayNdx, stmt->مقدار, stmt->ادغام);
    }
    وإلا إذا (!elem && (istreq(فیلد_2, "groupswrap") ||
                       istreq(فیلد_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(فیلد_2, "groupsclamp") ||
                       istreq(فیلد_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && (istreq(فیلد_2, "groupsredirect") ||
                       istreq(فیلد_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    وإلا إذا (!elem && istreq(فیلد_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    وإلا إذا (elem) {
        ret = (مجموعه_Default_Action_فیلد(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, فیلد_2, arrayNdx,
                                     &stmt->مقدار, stmt->ادغام) !=
               PARSER_FATAL_ERROR);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_فیلد,
                "Default defined for unknown field \"%s\"; Ignored\n", فیلد_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    أرجع ret;
}

ساكن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, کلید_Info *keyi)
{
    bool all_valid_entries = true;

    لكل (; def; def = (VarDef *) def->common.بعدی) {
        ثابت حرف *فیلد_2;
        عبارت_Def *arrayNdx;
        bool ok = true;

        إذا (!def->name) {
            إذا (unlikely(!def->مقدار) || def->مقدار->common.type != STMT_عبارت_ACTION_LIST)
                فیلد_2 = "symbols"; /* Default to symbols field */
            وإلا
                فیلد_2 = "actions";
            arrayNdx = عدم;
        }
        وإلا {
            ثابت حرف *elem;
            ok = عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &فیلد_2,
                                &arrayNdx);
            إذا (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, فیلد_2);
                ok = false;
            }
        }

        إذا (unlikely(!def->مقدار)) {
            log_err(info->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", فیلد_2);
            ok = false;
        }

        إذا (!ok || !مجموعه_Symbols_فیلد(info, keyi, فیلد_2, arrayNdx, &def->مقدار))
            all_valid_entries = false;
    }

    أرجع all_valid_entries;
}

ساكن bool
مجموعه_Explicit_گروه(SymbolsInfo *info, کلید_Info *keyi)
{
    xkb_layout_شاخص_t i;
    گروه_Info_2 *groupi;
    bool warn = false;

    إذا (info->explicit_گروه == XKB_LAYOUT_INVALID)
        أرجع true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        إذا (groupi->defined) {
            warn = true;
            Clear_گروه_Info(groupi);
            Init_گروه_Info(groupi);
        }
    }

    إذا (warn) {
        log_warn(info->ctx, XKB_WARNING_مضرب_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_گروه + 1, کلید_Info_متن(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_گروه + 1);
    إذا (info->explicit_گروه > 0) {
        darray_قلم(keyi->groups, info->explicit_گروه) =
            darray_قلم(keyi->groups, 0);
        Init_گروه_Info(&darray_قلم(keyi->groups, 0));
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
    لكل (xkb_layout_شاخص_t i = 0; i < darray_اندازه(keyi.groups); i++)
        Copy_گروه_Info(&darray_قلم(keyi.groups, i),
                      &darray_قلم(info->default_کلید.groups, i));
    keyi.ادغام = stmt->ادغام;
    keyi.name = stmt->کلید_Name;

    إذا (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        مجموعه_Explicit_گروه(info, &keyi) &&
        جمع_کن_کلید_Symbols(info, &keyi, true)) {
        أرجع true;
    }

    Clear_کلید_Info(&keyi);
    info->error_شمار++;
    أرجع false;
}

ساكن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_مدخل tmp;
    بنية xkb_context *ctx = info->ctx;

    إذا (!عبارت_Resolve_Mod_ماسک(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        أرجع false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_متن_V1 ||
           def->modifiers->common.type == STMT_عبارت_IDENT);

    bool ok = true;
    tmp.ادغام = def->ادغام;

    لكل (عبارت_Def *کلید = def->کلیدها; کلید; کلید = (عبارت_Def *) کلید->common.بعدی) {
        إذا (کلید->common.type == STMT_عبارت_KEYNAME_LITERAL) {
            tmp.have_نماد = false;
            tmp.u.کلید_Name = کلید->کلید_name.کلید_name;
        }
        وإلا إذا (کلید->common.type == STMT_عبارت_KEYSYM_LITERAL) {
            إذا (کلید->keysym.keysym == XKB_کلید_No_نماد) {
                /* Invalid keysym: ignore. Error message already printed */
                استمر;
            } وإلا {
                tmp.have_نماد = true;
                tmp.u.کلید_Sym = کلید->keysym.keysym;
            }
        }
        وإلا {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_مدخل,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            استمر;
        }

        ok = جمع_کن_Mod_Map_مدخل(info, &tmp) && ok;
    }
    أرجع ok;
}

ساكن فراغ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_امن(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بعدی) {
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
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_شمار++;

        إذا (info->error_شمار > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    امن_map_name(file->name));
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
پیدا_کن_کلید_For_نماد(بنية xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_گروه;
    xkb_layout_شاخص_t گروه = 0;
    افعل {
        xkb_level_شاخص_t level = 0;
        got_one_گروه = false;
        bool got_one_level;
        افعل {
            got_one_level = false;
            بنية xkb_کلید *کلید;
            xkb_کلیدها_foreach(کلید, keymap) {
                إذا (گروه < کلید->رقم_groups &&
                    level < Xkb_کلید_رقم_سطوح(کلید, گروه)) {
                    got_one_گروه = got_one_level = true;
                    ثابت xkb_keysym_شمار_t رقم_syms =
                        کلید->groups[گروه].سطوح[level].رقم_syms;
                    إذا (رقم_syms > 1) {
                        لكل (xkb_keysym_شمار_t k = 0; k < رقم_syms; k++) {
                            إذا (کلید->groups[گروه].سطوح[level].s.syms[k] == sym)
                                أرجع کلید;
                        }
                    } وإلا إذا (رقم_syms &&
                               کلید->groups[گروه].سطوح[level].s.sym == sym) {
                        أرجع کلید;
                    }
                }
            }
            level++;
        } طالما (got_one_level);
        گروه++;
    } طالما (got_one_گروه);

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
پیدا_کن_Automatic_Type(بنية xkb_context *ctx, گروه_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_شاخص_t عرض = darray_اندازه(groupi->سطوح);

#تعریف_کن GET_SYM(level) \
    (darray_قلم(groupi->سطوح, level).رقم_syms == 0 ? \
        XKB_کلید_No_نماد : \
     darray_قلم(groupi->سطوح, level).رقم_syms == 1 ? \
        darray_قلم(groupi->سطوح, level).s.sym : \
     /* num_syms > 1 */ \
        darray_قلم(groupi->سطوح, level).s.syms[0])

    إذا (عرض == 1 || عرض <= 0)
        أرجع xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    إذا (عرض == 2) {
        إذا (xkb_keysym_is_پایینی(sym0) && xkb_keysym_is_فوقانی_or_title(sym1))
            أرجع xkb_atom_intern_literal(ctx, "ALPHABETIC");

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    إذا (عرض <= 4) {
        إذا (xkb_keysym_is_پایینی(sym0) && xkb_keysym_is_فوقانی_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (عرض == 4 ? GET_SYM(3) : XKB_کلید_No_نماد);

            إذا (xkb_keysym_is_پایینی(sym2) && xkb_keysym_is_فوقانی_or_title(sym3))
                أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        إذا (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        أرجع xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    أرجع XKB_ATOM_NONE;

#لغو_تعریف GET_SYM
}

ساكن ثابت بنية xkb_کلید_type *
پیدا_کن_Type_For_گروه(بنية xkb_keymap *keymap, کلید_Info *keyi,
                 xkb_layout_شاخص_t گروه, bool *explicit_type)
{
    گروه_Info_2 *groupi = &darray_قلم(keyi->groups, گروه);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    إذا (type_name == XKB_ATOM_NONE) {
        إذا (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        وإلا {
            type_name = پیدا_کن_Automatic_Type(keymap->ctx, groupi);
            إذا (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    إذا (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_کلید_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 کلید_Name_متن(keymap->ctx, keyi->name), گروه + 1,
                 darray_اندازه(groupi->سطوح));
        اذهب use_default;
    }

    darray_اندازه_t i;
    لكل (i = 0; i < keymap->رقم_types; i++)
        إذا (keymap->types[i].name == type_name)
            اكسر;

    إذا (i >= keymap->رقم_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_کلید_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_متن(keymap->ctx, type_name),
                 کلید_Name_متن(keymap->ctx, keyi->name), گروه + 1);
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
Copy_Symbols_Def_مقصد_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                       کلید_Info *keyi)
{
    بنية xkb_کلید *کلید;
    گروه_Info_2 *groupi;
    ثابت گروه_Info_2 *group0;
    xkb_layout_شاخص_t i;

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
    کلید->رقم_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & کلید_فیلد_DEFAULT_TYPE)
                                    || (groupi->defined & گروه_فیلد_TYPE);
        إذا (darray_اندازه(groupi->سطوح) > 0 || has_explicit_type)
            کلید->رقم_groups = i + 1;
        إذا (has_explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    إذا (کلید->رقم_groups <= 0) {
        /* A key with no group may still have other fields defined */
        إذا (keyi->defined)
            اذهب کلید_fields;
        وإلا
            أرجع false;
    }

    darray_resize(keyi->groups, کلید->رقم_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_قلم(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        إذا (groupi->defined)
            استمر;

        Copy_گروه_Info(groupi, group0);
    }

    کلید->groups = حافظه_صفر_بده(کلید->رقم_groups, حجم(*کلید->groups));
    أكد(darray_اندازه(keyi->groups) == کلید->رقم_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #اگر_تعریف_شده __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < کلید->رقم_groups);
        #پایان_اگر
        bool explicit_type = false;
        ثابت بنية xkb_کلید_type * ثابت type =
            پیدا_کن_Type_For_گروه(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        إذا (type->رقم_سطوح < darray_اندازه(groupi->سطوح)) {
            بنية xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_متن(keymap->ctx, type->name), type->رقم_سطوح,
                    کلید_Info_متن(info, keyi),
                    darray_اندازه(groupi->سطوح));

            darray_foreach_from(leveli, groupi->سطوح, type->رقم_سطوح)
                clear_level(leveli);
        }
        darray_resize0(groupi->سطوح, type->رقم_سطوح);

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
        darray_foreach(leveli, groupi->سطوح) {
            بدل (leveli->رقم_syms) {
            حالة 0:
                leveli->فوقانی = XKB_کلید_No_نماد;
                اكسر;
            حالة 1:
                leveli->فوقانی = xkb_keysym_مقصد_فوقانی(leveli->s.sym);
                اكسر;
            افتراضي:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_فوقانی = false;
                لكل (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++) {
                    ثابت xkb_keysym_t فوقانی =
                        xkb_keysym_مقصد_فوقانی(leveli->s.syms[k]);
                    إذا (فوقانی != leveli->s.syms[k]) {
                        leveli->has_فوقانی = true;
                        اكسر;
                    }
                }
                إذا (leveli->has_فوقانی) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (اندازه_t) 2 * leveli->رقم_syms *
                                             حجم(*leveli->s.syms));
                    إذا (!leveli->s.syms)
                        أرجع false; /* FIXME: better handling? */
                    لكل (xkb_keysym_شمار_t k = 0; k < leveli->رقم_syms; k++) {
                        leveli->s.syms[leveli->رقم_syms + k] =
                            xkb_keysym_مقصد_فوقانی(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->سطوح, &کلید->groups[i].سطوح, عدم);
        إذا (کلید->groups[i].type->رقم_سطوح > 1 ||
            کلید->groups[i].سطوح[0].رقم_syms > 0) {
            کلید->groups[i].explicit_symbols = true;
            کلید->explicit |= EXPLICIT_SYMBOLS;
        }
        إذا (groupi->defined & گروه_فیلد_ACTS) { // FIXME
            کلید->groups[i].explicit_actions = true;
            کلید->explicit |= EXPLICIT_INTERP;
        }
        إذا (کلید->groups[i].explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    کلید->بیرون_از_range_pending_گروه = keyi->بیرون_از_range_pending_گروه;
    کلید->بیرون_از_range_گروه_عدد = keyi->بیرون_از_range_گروه_عدد;
    کلید->بیرون_از_range_گروه_policy = keyi->بیرون_از_range_گروه_policy;

کلید_fields:
    إذا (keyi->defined & کلید_فیلد_VMODMAP) {
        کلید->vmodmap = keyi->vmodmap;
        کلید->explicit |= EXPLICIT_VMODMAP;
    }

    إذا (keyi->repeat != کلید_REPEAT_UNDEFINED) {
        کلید->repeats = (keyi->repeat == کلید_REPEAT_YES);
        کلید->explicit |= EXPLICIT_REPEAT;
    }

    إذا ((keyi->defined & کلید_فیلد_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        کلید->overlays = keyi->overlays;
        إذا (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_ماسک_t مانده = کلید->overlays;
            ثابت بنية xkb_کلید **overlays_کلیدها = keyi->overlays_کلیدها;
            طالما (مانده) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                    مانده &
                    (xkb_overlay_ماسک_t)((بیعلامت)~مانده + 1u)
                );
                مانده = (مانده & ~lsb);
                إذا (*overlays_کلیدها) {
                    overlays_کلیدها++;
                } وإلا {
                    /* drop current null value (overlayN=none) */
                    کلید->overlays &= ~lsb;
                }
            }

            إذا (کلید->overlays) {
                /* Steal */
                کلید->overlays_کلیدها = keyi->overlays_کلیدها;
                keyi->overlays_کلیدها = عدم;
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
Copy_Mod_Map_Def_مقصد_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_مدخل *مدخل)
{
    بنية xkb_کلید *کلید;

    إذا (!مدخل->have_نماد) {
        کلید = Xkb_کلید_By_Name(keymap, مدخل->u.کلید_Name, true);
        إذا (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    کلید_Name_متن(info->ctx, مدخل->u.کلید_Name),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, مدخل->mods));
            مدخل->mods = 0; /* disable */
            أرجع false;
        }
    }
    وإلا {
        کلید = پیدا_کن_کلید_For_نماد(keymap, مدخل->u.کلید_Sym);
        إذا (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_نماد,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_متن(info->ctx, مدخل->u.کلید_Sym),
                    Mod_ماسک_متن(info->ctx, MOD_REAL, &info->mods, مدخل->mods));
            مدخل->mods = 0; /* disable */
            أرجع false;
        }
    }

    /* Skip modMap None */
    إذا (مدخل->mods) {
        کلید->modmap |= مدخل->mods;
        مدخل->کلید_کد = کلید->keycode;
    }

    أرجع true;
}

ساكن bool
Copy_Symbols_مقصد_Keymap(بنية xkb_keymap *keymap, SymbolsInfo *info)
{
    کلید_Info *keyi;

    keymap->symbols_section_name = strdup_امن(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->رقم_گروه_نامها = (xkb_layout_شاخص_t)darray_اندازه(info->گروه_نامها);
    darray_steal(info->گروه_نامها, &keymap->گروه_نامها, عدم);

    darray_foreach(keyi, info->کلیدها)
        إذا (!Copy_Symbols_Def_مقصد_Keymap(keymap, info, keyi))
            info->error_شمار++;

    إذا (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        بنية xkb_کلید *کلید;

        xkb_کلیدها_foreach(کلید, keymap) {
            إذا (کلید->name == XKB_ATOM_NONE)
                استمر;

            إذا (کلید->رقم_groups < 1)
                log_info(info->ctx, XKB_LOG_پیام_NO_ID,
                         "No symbols defined for %s\n",
                         کلید_Name_متن(info->ctx, کلید->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_مدخل *mm;
    darray_foreach(mm, info->modmaps)
        إذا (!Copy_Mod_Map_Def_مقصد_Keymap(keymap, info, mm))
            info->error_شمار++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->رقم_modmaps);

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

    إذا (info.error_شمار != 0)
        اذهب err_info;

    إذا (!Copy_Symbols_مقصد_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearSymbolsInfo(&info);
    أرجع true;

err_info:
    ClearSymbolsInfo(&info);
    أرجع false;
}
