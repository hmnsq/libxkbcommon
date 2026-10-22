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

#شامل "config.h"

#شامل <limits.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon-keysyms.h"

#شامل "action.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "keymap.h"
#شامل "keysym.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "utils.h"
#شامل "util-numbers.h"
#شامل "util-mem.h"
#شامل "vmod.h"
#شامل "xkbcomp-priv.h"
#شامل "xkbcomp/ast.h"

شمار کلید_repeat {
    کلید_REPEAT_UNDEFINED = 0,
    کلید_REPEAT_YES,
    کلید_REPEAT_NO,
    کلید_REPEAT_عدد_ENTRIES,
};
شمار { کلید_REPEAT_MIN_چوڑائی = 3 /* 2 bits + sign */ };
static_assert(کلید_REPEAT_عدد_ENTRIES - 1 <=
              (1 << (کلید_REPEAT_MIN_چوڑائی - 1)) - 1,
              "Cannot encode key repeat");

شمار گروہ_میدان {
    گروہ_میدان_SYMS = (1 << 0),
    گروہ_میدان_ACTS = (1 << 1),
    گروہ_میدان_TYPE = (1 << 2),
};

شمار کلید_میدان {
    کلید_میدان_NONE = 0,
    کلید_میدان_REPEAT = (1 << 0),
    کلید_میدان_DEFAULT_TYPE = (1 << 1),
    کلید_میدان_GROUPINFO = (1 << 2),
    کلید_میدان_VMODMAP = (1 << 3),
    کلید_میدان_OVERLAY = (1 << 4),
    کلید_میدان_ALL = کلید_میدان_REPEAT | کلید_میدان_DEFAULT_TYPE
                  | کلید_میدان_GROUPINFO | کلید_میدان_VMODMAP | کلید_میدان_OVERLAY,
};
شمار { کلید_میدان_MIN_چوڑائی = 6 /* 5 bits + sign */ };
static_assert(کلید_میدان_ALL <= (1 << (کلید_میدان_MIN_چوڑائی - 1)) - 1,
              "Cannot encode key field");

قسمتعریف ساخت {
    darray(ساخت xkb_level) سطحیں;
    شمار گروہ_میدان defined;
    xkb_atom_t type;
} گروہ_Info_2;

شمار {
    XKB_LAYOUT_برآمد_کا_RANGE_POLICY_MIN_چوڑائی = 4 /* 3 bits + sign */,
    KEYINFO_برآمد_کا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان = 8,
    KEYINFO_DEFINED_ثنائی_ہندسہ_میدان = 16,
    KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان = 8,
    KEYINFO_REPEAT_ثنائی_ہندسہ_میدان = 8,
};
static_assert(
    XKB_LAYOUT_برآمد_کا_RANGE_WRAP < XKB_LAYOUT_برآمد_کا_RANGE_CLAMP &&
    XKB_LAYOUT_برآمد_کا_RANGE_CLAMP < XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_برآمد_کا_RANGE_POLICY_MIN_چوڑائی) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_برآمد_کا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان >=
              XKB_LAYOUT_برآمد_کا_RANGE_POLICY_MIN_چوڑائی, "");
static_assert((غیرعلامتی)KEYINFO_DEFINED_ثنائی_ہندسہ_میدان >=
              (غیرعلامتی)کلید_میدان_MIN_چوڑائی, "Cannot encode KeyInfo field");
static_assert((غیرعلامتی)KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان >=
              (غیرعلامتی)ملاؤ_منوال_MIN_چوڑائی, "Cannot encode KeyInfo field");
static_assert((غیرعلامتی)KEYINFO_REPEAT_ثنائی_ہندسہ_میدان >=
              (غیرعلامتی)کلید_REPEAT_MIN_چوڑائی, "Cannot encode KeyInfo field");

قسمتعریف ساخت {
    xkb_atom_t name;

    xkb_mod_نقاب_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_اشاریہ_t برآمد_کا_range_گروہ_نمبر;
    darray(گروہ_Info_2) groups;
    شمار xkb_layout_برآمد_کا_range_policy
        برآمد_کا_range_گروہ_policy:KEYINFO_برآمد_کا_RANGE_گروہ_POLICY_ثنائی_ہندسہ_میدان;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    شمار کلید_میدان defined:KEYINFO_DEFINED_ثنائی_ہندسہ_میدان;
    شمار ملاؤ_منوال ملاؤ:KEYINFO_ملاؤ_ثنائی_ہندسہ_میدان;
    شمار کلید_repeat repeat:KEYINFO_REPEAT_ثنائی_ہندسہ_میدان;

    bool برآمد_کا_range_pending_گروہ:1;

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
        ثابت ساخت xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت ساخت xkb_کلید **overlays_کلیدیں;
    };
} کلید_Info;

ساکن خالی
StealLevelInfo(ساخت xkb_level *into, ساخت xkb_level *from)
{
    clear_level(into);
    اگر (from->عدد_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ورنہ {
        into->s.sym = from->s.sym;
    }
    into->عدد_syms = from->عدد_syms;
    from->عدد_syms = 0;

    اگر (from->عدد_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ورنہ {
        into->a.action = from->a.action;
    }
    into->عدد_actions = from->عدد_actions;
    from->عدد_actions = 0;
}

ساکن خالی
Init_گروہ_Info(گروہ_Info_2 *groupi)
{
    memset(groupi, 0, حجم(*groupi));
}

ساکن خالی
Clear_گروہ_Info(گروہ_Info_2 *groupi)
{
    ساخت xkb_level *leveli;
    darray_foreach(leveli, groupi->سطحیں)
        clear_level(leveli);
    darray_free(groupi->سطحیں);
}

ساکن خالی
Copy_گروہ_Info(گروہ_Info_2 *مقصد, ثابت گروہ_Info_2 *from)
{
    مقصد->defined = from->defined;
    مقصد->type = from->type;
    darray_init(مقصد->سطحیں);
    darray_copy(مقصد->سطحیں, from->سطحیں);
    برائے (xkb_level_اشاریہ_t j = 0; j < darray_سائز(مقصد->سطحیں); j++) {
        اگر (darray_شے(from->سطحیں, j).عدد_syms > 1) {
            darray_شے(مقصد->سطحیں, j).s.syms =
                memdup(darray_شے(from->سطحیں, j).s.syms,
                       darray_شے(from->سطحیں, j).عدد_syms,
                       حجم(xkb_keysym_t));
        }
        اگر (darray_شے(from->سطحیں, j).عدد_actions > 1) {
            darray_شے(مقصد->سطحیں, j).a.actions =
                memdup(darray_شے(from->سطحیں, j).a.actions,
                       darray_شے(from->سطحیں, j).عدد_actions,
                       حجم(اتحاد xkb_action));
        }
    }
}

ساکن خالی
Init_کلید_Info(ساخت xkb_context *ctx, کلید_Info *keyi)
{
    memset(keyi, 0, حجم(*keyi));
    static_assert(!DEFAULT_کلید_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_کلید_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->برآمد_کا_range_گروہ_policy = XKB_LAYOUT_برآمد_کا_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_کلید = تہی;
}

ساکن خالی
Clear_کلید_Info(کلید_Info *keyi)
{
    گروہ_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_گروہ_Info(groupi);
    darray_free(keyi->groups);
    اگر (keyi->overlays_alloc)
        آزاد(keyi->overlays_کلیدیں);
}

/***====================================================================***/

قسمتعریف ساخت {
    حرف *name;         /* e.g. pc+us+inet(evdev) */
    صحیح error_گنتی;
    غیرعلامتی صحیح include_گہرائی;
    xkb_layout_اشاریہ_t explicit_گروہ;
    xkb_layout_اشاریہ_t max_groups;
    darray(کلید_Info) کلیدیں;
    کلید_Info default_کلید;
    ActionsInfo default_actions;
    darray(xkb_atom_t) گروہ_نام;
    darray(Mod_Map_اندراج) modmaps;
    ساخت xkb_mod_سیٹ mods;

    ساخت xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ثابت ساخت xkb_keymap_info *keymap_info;
} SymbolsInfo;

ساکن خالی
InitSymbolsInfo(SymbolsInfo *info, ثابت ساخت xkb_keymap_info *keymap_info,
                غیرعلامتی صحیح include_گہرائی, ثابت ساخت xkb_mod_سیٹ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_گہرائی = include_گہرائی;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_کلید_Info(keymap_info->keymap.ctx, &info->default_کلید);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_گہرائی > 0);
    info->explicit_گروہ = XKB_LAYOUT_INVALID;
}

ساکن خالی
ClearSymbolsInfo(SymbolsInfo *info)
{
    کلید_Info *keyi;
    آزاد(info->name);
    darray_foreach(keyi, info->کلیدیں)
        Clear_کلید_Info(keyi);
    darray_free(info->کلیدیں);
    darray_free(info->گروہ_نام);
    darray_free(info->modmaps);
    Clear_کلید_Info(&info->default_کلید);
}

ساکن ثابت حرف *
کلید_Info_متن(SymbolsInfo *info, کلید_Info *keyi)
{
    واپس کلید_Name_متن(info->ctx, keyi->name);
}

ساکن bool
ملاؤ_Groups(SymbolsInfo *info, گروہ_Info_2 *into, گروہ_Info_2 *from, bool clobber,
            bool report, xkb_layout_اشاریہ_t گروہ, xkb_atom_t کلید_name)
{
    /* First find the type of the merged group. */
    اگر (into->type != from->type) {
        اگر (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ورنہ اگر (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ورنہ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            اگر (report) {
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

    اگر (darray_خالی_خانے(from->سطحیں)) {
        Init_گروہ_Info(from);
        واپس true;
    }

    اگر (darray_خالی_خانے(into->سطحیں)) {
        from->type = into->type;
        *into = *from;
        Init_گروہ_Info(from);
        واپس true;
    }

    /* Merge the actions and syms. */
    ثابت darray_سائز_t سطحیں_اندر_both =
        MIN(darray_سائز(into->سطحیں), darray_سائز(from->سطحیں));
    darray_سائز_t from_Keysyms_گنتی = 0;
    darray_سائز_t from_Actions_گنتی = 0;
    برائے (darray_سائز_t i = 0; i < سطحیں_اندر_both; i++) {
        ساخت xkb_level* ثابت intoLevel = &darray_شے(into->سطحیں, i);
        ساخت xkb_level* ثابت fromLevel = &darray_شے(from->سطحیں, i);

        ثابت bool fromHasNoKeysym = fromLevel->عدد_syms == 0;
        ثابت bool fromHasNoAction = fromLevel->عدد_actions == 0;
        اگر (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            جاری;
        }

        ثابت bool intoHasNoKeysym = intoLevel->عدد_syms == 0;
        ثابت bool intoHasNoAction = intoLevel->عدد_actions == 0;
        اگر (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_گنتی++;
            from_Actions_گنتی++;
            جاری;
        }

        /* Possible level conflict */
        أكد(intoLevel->عدد_syms > 0 || intoLevel->عدد_actions > 0);
        أكد(fromLevel->عدد_syms > 0 || fromLevel->عدد_actions > 0);

        /* Handle keysyms */
        اگر (!Xkb_سطحیں_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            اگر (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_علامت,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, گروہ + 1, کلید_Name_متن(info->ctx, کلید_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            اگر (fromHasNoKeysym) {
                /* No keysym to copy */
            } ورنہ اگر (clobber) {
                /* Override: copy any defined keysym from `from` */
                اگر (unlikely(fromLevel->عدد_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    اگر (unlikely(intoLevel->عدد_syms > 1))
                        آزاد(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->عدد_syms = fromLevel->عدد_syms;
                    fromLevel->عدد_syms = 0;
                    from_Keysyms_گنتی++;
                } ورنہ اگر (fromLevel->s.sym != XKB_کلید_No_علامت) {
                    /* Single defined keysym */
                    اگر (unlikely(intoLevel->عدد_syms > 1))
                        آزاد(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->عدد_syms = 1;
                    from_Keysyms_گنتی++;
                }
            } ورنہ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                اگر (unlikely(intoLevel->عدد_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ورنہ اگر (intoLevel->s.sym == XKB_کلید_No_علامت) {
                    /* Single undefined keysym */
                    اگر (unlikely(fromLevel->عدد_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->عدد_syms = fromLevel->عدد_syms;
                        fromLevel->عدد_syms = 0;
                    } ورنہ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->عدد_syms = 1;
                    }
                    from_Keysyms_گنتی++;
                }
            }
        }

        /* Handle actions */
        اگر (!Xkb_سطحیں_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            اگر (report && !(intoHasNoAction || fromHasNoAction)) {
                اگر (unlikely(intoLevel->عدد_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, گروہ + 1, کلید_Name_متن(info->ctx, کلید_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ورنہ {
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
            اگر (fromHasNoAction) {
                /* No action to copy */
            } ورنہ اگر (clobber) {
                /* Override: copy any defined action from `from` */
                اگر (unlikely(fromLevel->عدد_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    اگر (unlikely(intoLevel->عدد_actions > 1))
                        آزاد(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->عدد_actions = fromLevel->عدد_actions;
                    fromLevel->عدد_actions = 0;
                    from_Actions_گنتی++;
                } ورنہ اگر (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    اگر (unlikely(intoLevel->عدد_actions > 1))
                        آزاد(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->عدد_actions = 1;
                    from_Actions_گنتی++;
                }
            } ورنہ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                اگر (unlikely(intoLevel->عدد_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ورنہ اگر (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    اگر (unlikely(fromLevel->عدد_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->عدد_actions = fromLevel->عدد_actions;
                        fromLevel->عدد_actions = 0;
                    } ورنہ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->عدد_actions = 1;
                    }
                    from_Actions_گنتی++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ساخت xkb_level *level;
    darray_foreach_from(level, from->سطحیں, سطحیں_اندر_both) {
        darray_append(into->سطحیں, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->عدد_syms = 0;
        level->عدد_actions = 0;
        from_Keysyms_گنتی++;
        from_Actions_گنتی++;
    }
    اگر (from_Keysyms_گنتی) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        اگر (from_Keysyms_گنتی == darray_سائز(into->سطحیں))
            into->defined &= ~گروہ_میدان_SYMS;
        into->defined |= (from->defined & گروہ_میدان_SYMS);
    }
    اگر (from_Actions_گنتی) {
        /* Reset defined actions field if we used no action from `into` */
        اگر (from_Actions_گنتی == darray_سائز(into->سطحیں))
            into->defined &= ~گروہ_میدان_ACTS;
        into->defined |= (from->defined & گروہ_میدان_ACTS);
    }

    واپس true;
}

ساکن bool
Use_New_کلید_میدان(شمار کلید_میدان میدان_2, شمار کلید_میدان old, شمار کلید_میدان new,
               bool clobber, bool report, شمار کلید_میدان *collide)
{
    اگر (!(old & میدان_2))
        واپس (new & میدان_2);

    اگر (new & میدان_2) {
        اگر (report)
            *collide |= میدان_2;

        واپس clobber;
    }

    واپس false;
}

ساکن bool
overlays_get(ثابت کلید_Info *info, xkb_overlay_اشاریہ_t ثنائی_ہندسہ,
             ثابت ساخت xkb_کلید **کلید_برآمد)
{
    اگر (ثنائی_ہندسہ >= (xkb_overlay_اشاریہ_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        واپس false;
    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ثنائی_ہندسہ);
    اگر (!(info->overlays & نقاب)) واپس false;
    اگر (کلید_برآمد) {
        اگر (!info->overlays_alloc) {
            /* Stored inline */
            *کلید_برآمد = info->overlay_کلید;
        } ورنہ {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                info->overlays & (نقاب - 1u)
            );
            ثابت xkb_overlay_اشاریہ_t اشاریہ = (xkb_overlay_اشاریہ_t)popcount32(کم);
            *کلید_برآمد = info->overlays_کلیدیں[اشاریہ];
        }
    }
    واپس true;
}

ساکن bool
overlays_داخل_کرو(کلید_Info *keyi, xkb_overlay_اشاریہ_t ثنائی_ہندسہ,
                ثابت ساخت xkb_کلید *کلید)
{
    اگر (ثنائی_ہندسہ >= (xkb_overlay_اشاریہ_t)(حجم(xkb_overlay_نقاب_t) * CHAR_BIT))
        واپس false;

    ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << ثنائی_ہندسہ);

    اگر ((keyi->overlays & نقاب) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        اگر (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_کلید = کلید;
            اگر (!کلید)
                keyi->overlays_clear = true;
        } ورنہ {
            /* Stored in sparse array */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                keyi->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
            );
            /* index = number of set bits strictly below bit */
            ثابت xkb_overlay_اشاریہ_t اشاریہ =
                (xkb_overlay_اشاریہ_t)popcount32(کم);
            keyi->overlays_کلیدیں[اشاریہ] = کلید;
        }
        واپس true;
    }

    /* Insert new element */
    اگر (!keyi->overlays || (keyi->overlays_clear && !کلید)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_کلید = کلید;
        keyi->overlays |= نقاب;
        keyi->overlays_clear = !کلید;
    } ورنہ اگر (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ثابت xkb_overlay_نقاب_t overlays = keyi->overlays | نقاب;
        ثابت xkb_overlay_اشاریہ_t alloc =
            (xkb_overlay_اشاریہ_t)popcount32(overlays);

        /* Alloc sparse array */
        ثابت ساخت xkb_کلید ** ثابت tmp = صفر_حافظہ_دو(alloc, حجم(*tmp));
        اگر (!tmp) واپس false;

        /* Copy previous entries to the buffer */
        اگر (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                overlays & (xkb_overlay_نقاب_t)(keyi->overlays - 1u)
            );
            ثابت xkb_overlay_اشاریہ_t idx = (xkb_overlay_اشاریہ_t)popcount32(کم);
            tmp[idx] = keyi->overlay_کلید;
        } ورنہ {
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
    } ورنہ {
        /* Allocated items: grow heap and shift if relevant */
        ثابت xkb_overlay_نقاب_t overlays =
            (xkb_overlay_نقاب_t)(keyi->overlays | نقاب);
        ثابت xkb_overlay_اشاریہ_t گنتی =
            (xkb_overlay_اشاریہ_t)popcount32(overlays);
        اگر (گنتی > keyi->overlays_alloc) {
            ثابت xkb_overlay_اشاریہ_t alloc =
                (xkb_overlay_اشاریہ_t)اگلا_pow2(گنتی);
            أكد(alloc >= گنتی);
            ثابت ساخت xkb_کلید ** ثابت tmp = realloc(
                keyi->overlays_کلیدیں,
                (سائز_t)alloc * حجم(*keyi->overlays_کلیدیں)
            );
            اگر (!tmp) واپس false;
            keyi->overlays_کلیدیں = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
            overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
        );
        ثابت xkb_overlay_اشاریہ_t اشاریہ = (xkb_overlay_اشاریہ_t)popcount32(کم);
        اگر (اشاریہ >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        اگر (اشاریہ < گنتی - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            أكد(اشاریہ + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_کلیدیں + اشاریہ + 1, keyi->overlays_کلیدیں + اشاریہ,
                    (سائز_t)(گنتی - 1u - اشاریہ) * حجم(ثابت ساخت xkb_کلید *));
        }
        keyi->overlays_کلیدیں[اشاریہ] = کلید;
        keyi->overlays = overlays;
    }

    واپس true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ساکن bool
ملاؤ_overlays(SymbolsInfo *info, کلید_Info *into, کلید_Info *from,
               bool clobber, bool report, شمار کلید_میدان *collide)
{
    اگر (!(from->defined & کلید_میدان_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ورنہ اگر (!(into->defined & کلید_میدان_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_کلیدیں = from->overlays_کلیدیں;
        from->overlays_کلیدیں = تہی;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= کلید_میدان_OVERLAY;
    } ورنہ اگر (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ورنہ {
        اگر (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ثابت xkb_overlay_نقاب_t نتیجہ_نقاب =
                (into->overlays | from->overlays);
            ثابت xkb_overlay_اشاریہ_t گنتی =
                (xkb_overlay_اشاریہ_t)popcount32(نتیجہ_نقاب);

            اگر (گنتی == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            کلید_Info *dest = into;
            کلید_Info *src = from;
            اگر (into->overlays_alloc >= گنتی &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ورنہ اگر (from->overlays_alloc >= گنتی) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ورنہ اگر (گنتی > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                اگر (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                اگر (!dest->overlays_alloc) {
                    ثابت ساخت xkb_کلید **tmp = صفر_حافظہ_دو(گنتی, حجم(*tmp));
                    اگر (!tmp)
                        واپس false;
                    اگر (dest->overlay_کلید)
                        tmp[0] = dest->overlay_کلید;
                    dest->overlays_کلیدیں = tmp;
                    dest->overlays_alloc = گنتی;
                } ورنہ {
                    ثابت ساخت xkb_کلید **tmp =
                        realloc(dest->overlays_کلیدیں,
                                گنتی * حجم(*dest->overlays_کلیدیں));
                    اگر (!tmp)
                        واپس false;
                    dest->overlays_کلیدیں = tmp;
                    dest->overlays_alloc = گنتی;
                }
            }

            xkb_overlay_نقاب_t بقایا = src->overlays;
            ثابت ساخت xkb_کلید **src_کلیدیں = (src->overlays_clear)
                ? تہی
                : (src->overlays_alloc)
                    ? src->overlays_کلیدیں
                    : &src->overlay_کلید;
            جب (بقایا) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بقایا &
                    (xkb_overlay_نقاب_t)((غیرعلامتی)~بقایا + 1u)
                );
                /* get its index */
                ثابت xkb_overlay_اشاریہ_t ثنائی_ہندسہ =
                    (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
                بقایا = (بقایا & ~lsb);

                /* pop current value */
                اگر (!src->overlays_clear && !src->overlays_alloc && بقایا)
                    PANIC_UNREACHABLE();
                ثابت ساخت xkb_کلید *src_کلید = (src_کلیدیں)
                    ? *(src_کلیدیں++)
                    : تہی;

                /* check dest */
                ثابت ساخت xkb_کلید *dest_کلید = تہی;
                ثابت bool conflict = overlays_get(dest, ثنائی_ہندسہ, &dest_کلید);
                اگر (conflict) {
                    اگر (dest_کلید == src_کلید)
                       جاری;
                    ورنہ اگر (report)
                        *collide |= کلید_میدان_OVERLAY;
                }

                اگر ((!conflict || clobber) && !overlays_داخل_کرو(dest, ثنائی_ہندسہ, src_کلید))
                    واپس false;
            }

            اگر (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                اگر (into->overlays_alloc)
                    آزاد(into->overlays_کلیدیں);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                اگر (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_کلیدیں = from->overlays_کلیدیں;
                    from->overlays_کلیدیں = تہی;
                    from->overlays_alloc = 0;
                } ورنہ {
                    into->overlay_کلید = from->overlay_کلید;
                }
            }
        } ورنہ {
            /* Disjoint overlays */

            اگر (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_کلید == from->overlay_کلید) {
                /* Indentical: do nothing */
                واپس true;
            }
            اگر (!(into->overlays & from->overlays)) {
                اگر (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_کلید = from->overlay_کلید;
                    واپس true;
                } ورنہ اگر (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    واپس true;
                }
            }
            /* Conflict */
            اگر (report)
                *collide |= کلید_میدان_OVERLAY;
            اگر (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_کلید = from->overlay_کلید;
            }
        }
    }
    واپس true;
}

ساکن bool
ملاؤ_کلیدیں(SymbolsInfo *info, کلید_Info *into, کلید_Info *from, bool same_file)
{
    xkb_layout_اشاریہ_t i;
    xkb_layout_اشاریہ_t groups_اندر_both;
    شمار کلید_میدان collide = 0;
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool clobber = (from->ملاؤ != ملاؤ_AUGMENT);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    اگر (from->ملاؤ == ملاؤ_REPLACE) {
        Clear_کلید_Info(into);
        *into = *from;
        Init_کلید_Info(info->ctx, from);
        واپس true;
    }

    groups_اندر_both = MIN(darray_سائز(into->groups), darray_سائز(from->groups));
    برائے (i = 0; i < groups_اندر_both; i++)
        ملاؤ_Groups(info,
                    &darray_شے(into->groups, i),
                    &darray_شے(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    برائے (i = groups_اندر_both; i < darray_سائز(from->groups); i++) {
        darray_append(into->groups, darray_شے(from->groups, i));
        Init_گروہ_Info(&darray_شے(from->groups, i));
    }

    اگر (Use_New_کلید_میدان(کلید_میدان_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= کلید_میدان_VMODMAP;
    }
    اگر (Use_New_کلید_میدان(کلید_میدان_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= کلید_میدان_REPEAT;
    }
    اگر (Use_New_کلید_میدان(کلید_میدان_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= کلید_میدان_DEFAULT_TYPE;
    }
    اگر (Use_New_کلید_میدان(کلید_میدان_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->برآمد_کا_range_pending_گروہ = from->برآمد_کا_range_pending_گروہ;
        into->برآمد_کا_range_گروہ_policy = from->برآمد_کا_range_گروہ_policy;
        into->برآمد_کا_range_گروہ_نمبر = from->برآمد_کا_range_گروہ_نمبر;
        into->defined |= کلید_میدان_GROUPINFO;
    }

    اگر (!ملاؤ_overlays(info, into, from, clobber, report, &collide))
        واپس false;

    اگر (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 کلید_Name_متن(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_کلید_Info(from);
    Init_کلید_Info(info->ctx, from);
    واپس true;
}

ساکن xkb_atom_t
Xkb_Resolve_کلید_Alias(ثابت ساخت xkb_keymap *keymap, xkb_atom_t name)
{
    اگر (name < keymap->عدد_کلید_نام) {
        ثابت Keycode_مطابقت مطابقت = keymap->کلید_نام[name];
        اگر (مطابقت.ملا && مطابقت.is_alias) {
            واپس مطابقت.alias.real;
        }
    }
    واپس name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ساکن bool
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
        اگر (iter->name == keyi->name)
            واپس ملاؤ_کلیدیں(info, iter, keyi, same_file);

    darray_append(info->کلیدیں, *keyi);
    Init_کلید_Info(info->ctx, keyi);
    واپس true;
}

ساکن bool
جوڑو_Mod_Map_اندراج(SymbolsInfo *info, ثابت Mod_Map_اندراج *new)
{
    ثابت bool clobber = (new->ملاؤ != ملاؤ_AUGMENT);

    Mod_Map_اندراج *old;
    darray_foreach(old, info->modmaps) {
        اگر ((new->have_علامت != old->have_علامت) ||
            (new->have_علامت && new->u.کلید_Sym != old->u.کلید_Sym) ||
            (!new->have_علامت && new->u.کلید_Name != old->u.کلید_Name))
            جاری;

        اگر (new->mods == old->mods)
            واپس true;

        ثابت xkb_mod_نقاب_t use = (clobber ? new->mods : old->mods);
        ثابت xkb_mod_نقاب_t ignore = (clobber ? old->mods : new->mods);

        اگر (new->have_علامت) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_متن(info->ctx, new->u.کلید_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        } ورنہ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    کلید_Name_متن(info->ctx, new->u.کلید_Name),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        واپس true;
    }

    darray_append(info->modmaps, *new);

    واپس true;
}

/***====================================================================***/

ساکن خالی
ملاؤ_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     شمار ملاؤ_منوال ملاؤ)
{
    xkb_atom_t *گروہ_name;
    xkb_layout_اشاریہ_t گروہ_نام_اندر_both;

    اگر (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        واپس;
    }

    ملاؤ_Mod_Sets(into->ctx, &into->mods, &from->mods, ملاؤ);

    اگر (into->name == تہی) {
        into->name = steal(&from->name);
    }

    گروہ_نام_اندر_both = MIN(darray_سائز(into->گروہ_نام),
                              darray_سائز(from->گروہ_نام));
    برائے (xkb_layout_اشاریہ_t i = 0; i < گروہ_نام_اندر_both; i++) {
        اگر (!darray_شے(from->گروہ_نام, i))
            جاری;

        اگر (ملاؤ == ملاؤ_AUGMENT && darray_شے(into->گروہ_نام, i))
            جاری;

        darray_شے(into->گروہ_نام, i) = darray_شے(from->گروہ_نام, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(گروہ_name, from->گروہ_نام, گروہ_نام_اندر_both)
        darray_append(into->گروہ_نام, *گروہ_name);

    اگر (darray_خالی_خانے(into->کلیدیں)) {
        into->کلیدیں = from->کلیدیں;
        darray_init(from->کلیدیں);
    }
    ورنہ {
        کلید_Info *keyi;
        darray_foreach(keyi, from->کلیدیں) {
            keyi->ملاؤ = ملاؤ;
            اگر (!جوڑو_کلید_Symbols(into, keyi, false))
                into->error_گنتی++;
        }
    }

    اگر (darray_خالی_خانے(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ورنہ {
        Mod_Map_اندراج *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ملاؤ = ملاؤ;
            اگر (!جوڑو_Mod_Map_اندراج(into, mm))
                into->error_گنتی++;
        }
    }
}

ساکن خالی
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ساکن bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    اگر (Exceeds_Include_Max_گہرائی(info->ctx, info->include_گہرائی)) {
        info->error_گنتی += 10;
        واپس false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_گہرائی + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساخت parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برائے (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        SymbolsInfo اگلا_incl;
        XkbFile *file;

        حرف راستہ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  راستہ, حجم(راستہ));
        اگر (!file) {
            info->error_گنتی += 10;
            ClearSymbolsInfo(&included);
            واپس false;
        }

        InitSymbolsInfo(&اگلا_incl, info->keymap_info, info->include_گہرائی + 1,
                        &included.mods);
        اگر (stmt->modifier) {
            ثابت صحیح گنتی = parse_dec_مقصد_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &اگلا_incl.explicit_گروہ);
            اگر (گنتی <= 0 || *(stmt->modifier + گنتی) != '\0' ||
                اگلا_incl.explicit_گروہ == 0 ||
                اگلا_incl.explicit_گروہ > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                اگلا_incl.explicit_گروہ = info->explicit_گروہ;
            } ورنہ {
                اگلا_incl.explicit_گروہ--;
            }
        }
        ورنہ اگر (info->keymap_info->keymap.عدد_groups != 0 &&
                 اگلا_incl.include_گہرائی == 1) {
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
        ورنہ {
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

    واپس (info->error_گنتی == 0);
}

ساکن bool
Get_گروہ_اشاریہ(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
              شمار گروہ_میدان میدان_2, xkb_layout_اشاریہ_t *ndx_rtrn)
{
    أكد (میدان_2 == گروہ_میدان_SYMS || میدان_2 == گروہ_میدان_ACTS);
    ثابت حرف *name = (میدان_2 == گروہ_میدان_SYMS ? "symbols" : "actions");

    اگر (arrayNdx == تہی) {
        xkb_layout_اشاریہ_t i = 0;
        گروہ_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            اگر (!(groupi->defined & میدان_2)) {
                *ndx_rtrn = i;
                واپس true;
            }
        }

        اگر (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, کلید_Info_متن(info, keyi), info->max_groups, name);
            واپس false;
        }

        darray_resize0(keyi->groups, darray_سائز(keyi->groups) + 1);
        *ndx_rtrn = darray_سائز(keyi->groups) - 1;
        واپس true;
    }

    اگر (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false, ndx_rtrn, تہی) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, کلید_Info_متن(info, keyi));
        واپس false;
    }

    (*ndx_rtrn)--;
    اگر (*ndx_rtrn >= darray_سائز(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    واپس true;
}

ساکن bool
جوڑو_Symbols_مقصد_کلید(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
                اظہاریہ_Def *قدر)
{
    xkb_layout_اشاریہ_t ndx = 0;

    اگر (!Get_گروہ_اشاریہ(info, keyi, arrayNdx, گروہ_میدان_SYMS, &ndx))
        واپس false;

    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, ndx);

    اگر (قدر->common.type == STMT_اظہاریہ_خالی_خانے_LIST) {
        groupi->defined |= گروہ_میدان_SYMS;
        واپس true;
    }

    اگر (قدر->common.type != STMT_اظہاریہ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_مقصد_string(قدر->common.type), ndx + 1,
                کلید_Info_متن(info, keyi));
        واپس false;
    }

    اگر (groupi->defined & گروہ_میدان_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_کلید_SYMBOLS_اندراج,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                کلید_Info_متن(info, keyi), ndx + 1);
        واپس false;
    }

    xkb_level_اشاریہ_t n_سطحیں = 0;
    xkb_level_اشاریہ_t non_خالی_خانے_سطحیں = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    برائے (اظہاریہ_Keysym_List *keysymList = (اظہاریہ_Keysym_List *) قدر;
         keysymList;
         keysymList = (اظہاریہ_Keysym_List *) keysymList->common.اگلا) {
        n_سطحیں++;
        /* Drop trailing NoSymbol */
        اگر (darray_سائز(keysymList->syms) > 0)
            non_خالی_خانے_سطحیں = n_سطحیں;
    }
    اگر (non_خالی_خانے_سطحیں < n_سطحیں)
        n_سطحیں = non_خالی_خانے_سطحیں;
    اگر (darray_سائز(groupi->سطحیں) < n_سطحیں)
        darray_resize0(groupi->سطحیں, n_سطحیں);

    groupi->defined |= گروہ_میدان_SYMS;

    xkb_level_اشاریہ_t level = 0;
    برائے (اظہاریہ_Keysym_List *keysymList = (اظہاریہ_Keysym_List *) قدر;
         keysymList && level < n_سطحیں;
         keysymList = (اظہاریہ_Keysym_List *) keysymList->common.اگلا, level++) {
        ساخت xkb_level *leveli = &darray_شے(groupi->سطحیں, level);
        أكد(leveli->عدد_syms == 0);

        اگر (unlikely(darray_سائز(keysymList->syms) > MAX_KEYSYMS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_فی_LEVEL, darray_سائز(keysymList->syms));
            واپس false;
        }

        leveli->عدد_syms = (xkb_keysym_گنتی_t) darray_سائز(keysymList->syms);
        انتخاب (leveli->عدد_syms) {
        حالت 0:
            leveli->s.sym = XKB_کلید_No_علامت;
            توڑو;
        حالت 1:
            leveli->s.sym = darray_شے(keysymList->syms, 0);
            أكد(leveli->s.sym != XKB_کلید_No_علامت);
            توڑو;
        طےشدہ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, تہی);
#اگر_غیر_تعریف_شدہ NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            برائے (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++)
                أكد(leveli->s.syms[k] != XKB_کلید_No_علامت);
#اگر_ختم
        }
    }

    واپس true;
}

ساکن bool
جوڑو_Actions_مقصد_کلید(SymbolsInfo *info, کلید_Info *keyi, اظہاریہ_Def *arrayNdx,
                اظہاریہ_Def *قدر)
{
    xkb_layout_اشاریہ_t ndx = 0;

    اگر (!Get_گروہ_اشاریہ(info, keyi, arrayNdx, گروہ_میدان_ACTS, &ndx))
        واپس false;

    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, ndx);

    اگر (قدر->common.type == STMT_اظہاریہ_خالی_خانے_LIST) {
        groupi->defined |= گروہ_میدان_ACTS;
        واپس true;
    }

    اگر (قدر->common.type != STMT_اظہاریہ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 قدر->common.type, ndx, کلید_Info_متن(info, keyi));
        واپس false;
    }

    اگر (groupi->defined & گروہ_میدان_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_کلید_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 کلید_Info_متن(info, keyi), ndx);
        واپس false;
    }

    xkb_level_اشاریہ_t n_سطحیں = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    برائے (ParseCommon *p = &قدر->common; p; p = p->اگلا)
        n_سطحیں++;
    اگر (darray_سائز(groupi->سطحیں) < n_سطحیں)
        darray_resize0(groupi->سطحیں, n_سطحیں);

    groupi->defined |= گروہ_میدان_ACTS;

    xkb_level_اشاریہ_t level = 0;
    xkb_level_اشاریہ_t non_خالی_خانے_سطحیں = 0;
    برائے (اظہاریہ_Action_List *actionList = (اظہاریہ_Action_List *) قدر;
         actionList;
         actionList = (اظہاریہ_Action_List *) actionList->common.اگلا, level++) {
        ساخت xkb_level *leveli = &darray_شے(groupi->سطحیں, level);
        أكد(leveli->عدد_actions == 0);

        غیرعلامتی صحیح عدد_actions = 0;
        برائے (اظہاریہ_Def *act = actionList->actions;
             act; act = (اظہاریہ_Def *) act->common.اگلا)
            عدد_actions++;

        اگر (unlikely(عدد_actions > MAX_ACTIONS_فی_LEVEL)) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    کلید_Info_متن(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_فی_LEVEL, عدد_actions);
            واپس false;
        }

        /* Parse actions and add only defined actions */
        darray(اتحاد xkb_action) actions = darray_new();
        برائے (اظہاریہ_Def *act = actionList->actions;
             act; act = (اظہاریہ_Def *) act->common.اگلا) {
            اتحاد xkb_action مقصد_Act = { 0 };
            ثابت شمار xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &مقصد_Act);
            اگر (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        کلید_Info_متن(info, keyi), ndx + 1, level + 1);
                اگر (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    واپس false;
                } ورنہ {
                    /* Ensure action type is reset */
                    مقصد_Act.type = ACTION_TYPE_NONE;
                }
            }
            اگر (مقصد_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ورنہ اگر (likely(عدد_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->عدد_actions = 1;
                leveli->a.action = مقصد_Act;
                جاؤ اگلا_level;
            } ورنہ {
                darray_append(actions, مقصد_Act);
            }
        }
        اگر (darray_خالی_خانے(actions)) {
            leveli->عدد_actions = 0;
        } ورنہ اگر (likely(darray_سائز(actions) > 1)) {
            leveli->عدد_actions = (xkb_action_گنتی_t) darray_سائز(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, تہی);
#اگر_غیر_تعریف_شدہ NDEBUG
            /* Canonical list: all NoAction() were dropped */
            برائے (xkb_action_گنتی_t k = 0; k < leveli->عدد_actions; k++)
                أكد(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#اگر_ختم
        } ورنہ {
            /* Unlikely: some actions were dropped and only one remains */
            أكد(عدد_actions > darray_سائز(actions));
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
        اگر (leveli->عدد_actions > 0 || leveli->عدد_syms > 0)
            non_خالی_خانے_سطحیں = level + 1;
    }

    اگر (non_خالی_خانے_سطحیں < n_سطحیں) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_سائز(groupi->سطحیں) = non_خالی_خانے_سطحیں;
        اگر (non_خالی_خانے_سطحیں > 0)
            darray_shrink(groupi->سطحیں);
        ورنہ
            darray_free(groupi->سطحیں);
    }

    واپس true;
}

ساکن ثابت Lookup_اندراج repeatEntries[] = {
    { "true", کلید_REPEAT_YES },
    { "yes", کلید_REPEAT_YES },
    { "on", کلید_REPEAT_YES },
    { "false", کلید_REPEAT_NO },
    { "no", کلید_REPEAT_NO },
    { "off", کلید_REPEAT_NO },
    { "default", کلید_REPEAT_UNDEFINED },
    { تہی, 0 }
};

/** Handle overlay<N> = <key> entry */
ساکن bool
اظہاریہ_Resolve_Overlay_اندراج(ثابت ساخت xkb_keymap_info *keymap_info,
                        ثابت حرف *میدان_2, ثابت اظہاریہ_Def *arrayNdx,
                        ثابت اظہاریہ_Def *اظہاریہ, کلید_Info *keyi,
                        xkb_overlay_اشاریہ_t *overlay_rtrn,
                        ثابت ساخت xkb_کلید **کلید_rtrn)
{
    /* Overlay index */
    اگر (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        واپس false;
    }
    static_assert(حجم(xkb_overlay_اشاریہ_t) == حجم(uint8_t), "");
    ثابت سائز_t سابقہ = حجم("overlay") - 1;
    ثابت سائز_t len = لڑی_لمبائی(میدان_2 + سابقہ);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    اگر (parse_dec_مقصد_uint64_t(میدان_2 + سابقہ, len, (uint64_t *)&raw_overlay)
        != (صحیح)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاریہ,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        واپس false;
    }
    *overlay_rtrn = (xkb_overlay_اشاریہ_t)raw_overlay - 1;

    /* Overlay key */
    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_KEYNAME_LITERAL:
        *کلید_rtrn = Xkb_کلید_By_Name(&keymap_info->keymap, اظہاریہ->کلید_name.کلید_name,
                                 false);
        اگر (!*کلید_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_متن(keymap_info->keymap.ctx, اظہاریہ->کلید_name.کلید_name),
                    میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
            واپس false;
        }
        واپس true;
    حالت STMT_اظہاریہ_IDENT: {
        ثابت حرف * ثابت id =
            xkb_atom_متن(keymap_info->keymap.ctx, اظہاریہ->ident.ident);
        اگر (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *کلید_rtrn = تہی;
            واپس true;
        } ورنہ اگر (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *کلید_rtrn = تہی;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            واپس true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name));
        واپس false;
    }
    طےشدہ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_قدر,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_مقصد_string(STMT_اظہاریہ_KEYNAME_LITERAL),
                میدان_2, کلید_Name_متن(keymap_info->keymap.ctx, keyi->name),
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;
    }
}

ساکن bool
سیٹ_Symbols_میدان(SymbolsInfo *info, کلید_Info *keyi, ثابت حرف *میدان_2,
                اظہاریہ_Def *arrayNdx, اظہاریہ_Def **قدر_ptr)
{
    اظہاریہ_Def * محدود ثابت قدر = *قدر_ptr;
    اگر (istreq(میدان_2, "type")) {
        xkb_layout_اشاریہ_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        اگر (!اظہاریہ_Resolve_String(info->ctx, قدر, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            واپس false;
        }

        اگر (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= کلید_میدان_DEFAULT_TYPE;
        }
        ورنہ اگر (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false,
                                  &ndx, تہی) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    کلید_Info_متن(info, keyi));
            واپس false;
        }
        ورنہ {
            ndx--;
            اگر (ndx >= darray_سائز(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                أكد(darray_سائز(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_شے(keyi->groups, ndx).type = val;
            darray_شے(keyi->groups, ndx).defined |= گروہ_میدان_TYPE;
        }
    }
    ورنہ اگر (istreq(میدان_2, "symbols")) {
        واپس جوڑو_Symbols_مقصد_کلید(info, keyi, arrayNdx, قدر);
    }
    ورنہ اگر (istreq(میدان_2, "actions")) {
        واپس جوڑو_Actions_مقصد_کلید(info, keyi, arrayNdx, قدر);
    }
    ورنہ اگر (istreq(میدان_2, "vmods") ||
             istreq(میدان_2, "virtualmods") ||
             istreq(میدان_2, "virtualmodifiers")) {
        xkb_mod_نقاب_t نقاب = 0;

        اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_VIRT, &info->mods,
                                &نقاب)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_مقصد_string(قدر->common.type),
                    کلید_Info_متن(info, keyi));
            واپس false;
        }

        keyi->vmodmap = نقاب;
        keyi->defined |= کلید_میدان_VMODMAP;
    }
    ورنہ اگر (istreq(میدان_2, "locking") ||
             istreq(میدان_2, "lock") ||
             istreq(میدان_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    ورنہ اگر (istreq(میدان_2, "radiogroup") ||
             istreq(میدان_2, "permanentradiogroup") ||
             istreq(میدان_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                کلید_Info_متن(info, keyi));
    }
    ورنہ اگر (istreq_سابقہ("permanentoverlay", میدان_2)) {
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
    ورنہ اگر (istreq_سابقہ("overlay", میدان_2)) {
        xkb_overlay_اشاریہ_t overlay = XKB_OVERLAY_INVALID;
        ثابت ساخت xkb_کلید *کلید = تہی;
        اگر (!اظہاریہ_Resolve_Overlay_اندراج(info->keymap_info, میدان_2, arrayNdx,
                                     *قدر_ptr, keyi, &overlay, &کلید))
            واپس false;
        اگر (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            واپس true;
        } ورنہ اگر (کلید && کلید->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_پیغام_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, کلید_Info_متن(info, keyi));
        } ورنہ {
            ثابت ساخت xkb_کلید *پچھلا;
            اگر (overlays_get(keyi, overlay, &پچھلا)) {
                /* Duplicate entry */
                اگر (کلید != پچھلا) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             کلید_Info_متن(info, keyi), overlay + 1,
                             (پچھلا
                                 ? کلید_Name_متن(info->ctx, پچھلا->name)
                                 : "none"),
                             overlay + 1,
                             (کلید
                                 ? کلید_Name_متن(info->ctx, کلید->name)
                                 : "none"));
                }
            } ورنہ اگر (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                اگر (!overlays_داخل_کرو(keyi, overlay, کلید))
                    واپس false;
                keyi->defined |= کلید_میدان_OVERLAY;
            } ورنہ {
                /* Handle disjoint overlays mode */
                ثابت xkb_overlay_نقاب_t نقاب = (xkb_overlay_نقاب_t)(1u << overlay);
                اگر (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    اگر (کلید) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = نقاب;
                        keyi->overlays_clear = false;
                        keyi->overlay_کلید = کلید;
                    } ورنہ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= نقاب;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= کلید_میدان_OVERLAY;
                } ورنہ اگر (keyi->overlays) {
                    اگر (کلید) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                کلید_Info_متن(info, keyi), keyi->overlays,
                                کلید_Name_متن(info->ctx, keyi->overlay_کلید->name),
                                overlay + 1, کلید_Name_متن(info->ctx, کلید->name));
                        واپس !(info->keymap_info->strict &
                                 PARSER_NO_میدان_قدر_MISMATCH);
                    } ورنہ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ورنہ اگر (istreq(میدان_2, "repeating") ||
             istreq(میدان_2, "repeats") ||
             istreq(میدان_2, "repeat")) {
        uint32_t val = 0;

        اگر (!اظہاریہ_Resolve_Enum(info->ctx, قدر, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    کلید_Info_متن(info, keyi));
            واپس false;
        }

        keyi->repeat = (شمار کلید_repeat) val;
        keyi->defined |= کلید_میدان_REPEAT;
    }
    ورنہ اگر (istreq(میدان_2, "groupswrap") ||
             istreq(میدان_2, "wrapgroups")) {
        bool سیٹ = false;

        اگر (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &سیٹ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            واپس false;
        }

        keyi->برآمد_کا_range_گروہ_policy = (سیٹ)
            ? XKB_LAYOUT_برآمد_کا_RANGE_WRAP
            : XKB_LAYOUT_برآمد_کا_RANGE_CLAMP;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    ورنہ اگر (istreq(میدان_2, "groupsclamp") ||
             istreq(میدان_2, "clampgroups")) {
        bool سیٹ = false;

        اگر (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &سیٹ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_قدر,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    کلید_Info_متن(info, keyi));
            واپس false;
        }

        keyi->برآمد_کا_range_گروہ_policy = (سیٹ)
            ? XKB_LAYOUT_برآمد_کا_RANGE_CLAMP
            : XKB_LAYOUT_برآمد_کا_RANGE_WRAP;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    ورنہ اگر (istreq(میدان_2, "groupsredirect") ||
             istreq(میدان_2, "redirectgroups")) {
        xkb_layout_اشاریہ_t grp = 0;
        bool pending = false;

        // TODO: recover?
        اگر (اظہاریہ_Resolve_گروہ(info->keymap_info, قدر, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    کلید_Info_متن(info, keyi));
            واپس false;
        }

        اگر (pending) {
            keyi->برآمد_کا_range_pending_گروہ = true;
            ثابت darray_سائز_t pending_اشاریہ =
                darray_سائز(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ساخت pending_computation) {
                    .اظہاریہ = *قدر_ptr,
                    .computed = false,
                    .قدر = 0,
                }
            );
            *قدر_ptr = تہی;
            static_assert(حجم(keyi->برآمد_کا_range_گروہ_نمبر) ==
                          حجم(pending_اشاریہ),
                          "Cannot save pending computation");
            keyi->برآمد_کا_range_گروہ_نمبر = pending_اشاریہ;
        } ورنہ {
            keyi->برآمد_کا_range_pending_گروہ = false;
            keyi->برآمد_کا_range_گروہ_نمبر = grp - 1;
        }

        keyi->برآمد_کا_range_گروہ_policy = XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT;
        keyi->defined |= کلید_میدان_GROUPINFO;
    }
    ورنہ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_میدان,
                "Unknown field \"%s\" in a key; definition ignored\n",
                میدان_2);
        واپس !(info->keymap_info->strict & PARSER_NO_UNKNOWN_کلید_FIELDS);
    }

    واپس true;
}

ساکن bool
سیٹ_گروہ_Name(SymbolsInfo *info, اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر,
             شمار ملاؤ_منوال ملاؤ)
{
    اگر (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_گروہ_NAME_اشاریہ,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        واپس false;
    }

    xkb_layout_اشاریہ_t گروہ = 0;
    اگر (اظہاریہ_Resolve_گروہ(info->keymap_info, arrayNdx, false, &گروہ, تہی) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        واپس false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    اگر (!اظہاریہ_Resolve_String(info->ctx, قدر, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", گروہ);
        واپس false;
    }

    xkb_layout_اشاریہ_t گروہ_مقصد_use;
    اگر (info->explicit_گروہ == XKB_LAYOUT_INVALID) {
        گروہ_مقصد_use = گروہ - 1;
    }
    ورنہ اگر (گروہ - 1 == 0) {
        گروہ_مقصد_use = info->explicit_گروہ;
    }
    ورنہ {
        log_warn(info->ctx, XKB_WARNING_NON_بنیاد_گروہ_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, گروہ,
                 xkb_atom_متن(info->ctx, name));
        واپس false;
    }

    اگر (گروہ_مقصد_use >= darray_سائز(info->گروہ_نام)) {
        /* Avoid clang-tidy false positive */
        أكد(darray_سائز(info->گروہ_نام) < گروہ_مقصد_use + 1);
        darray_resize0(info->گروہ_نام, گروہ_مقصد_use + 1);
    } ورنہ {
        ثابت xkb_atom_t old_name = darray_شے(info->گروہ_نام, گروہ_مقصد_use);
        اگر (old_name != XKB_ATOM_NONE && old_name != name) {
            ثابت bool replace = (ملاؤ != ملاؤ_AUGMENT);
            ثابت xkb_atom_t use    = (replace ? name : old_name);
            ثابت xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     گروہ_مقصد_use, info->name,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
            name = use;
        }
    }
    darray_شے(info->گروہ_نام, گروہ_مقصد_use) = name;

    واپس true;
}

ساکن bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;
    bool ret;

    اگر (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2, &arrayNdx))
        واپس false;

    اگر (elem && istreq(elem, "key")) {
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
    ورنہ اگر (!elem && (istreq(میدان_2, "name") ||
                       istreq(میدان_2, "groupname"))) {
        ret = سیٹ_گروہ_Name(info, arrayNdx, stmt->قدر, stmt->ملاؤ);
    }
    ورنہ اگر (!elem && (istreq(میدان_2, "groupswrap") ||
                       istreq(میدان_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ورنہ اگر (!elem && (istreq(میدان_2, "groupsclamp") ||
                       istreq(میدان_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ورنہ اگر (!elem && (istreq(میدان_2, "groupsredirect") ||
                       istreq(میدان_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ورنہ اگر (!elem && istreq(میدان_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_میدان,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ورنہ اگر (elem) {
        ret = (سیٹ_Default_Action_میدان(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, میدان_2, arrayNdx,
                                     &stmt->قدر, stmt->ملاؤ) !=
               PARSER_FATAL_ERROR);
    } ورنہ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        واپس !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    واپس ret;
}

ساکن bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, کلید_Info *keyi)
{
    bool all_valid_entries = true;

    برائے (; def; def = (VarDef *) def->common.اگلا) {
        ثابت حرف *میدان_2;
        اظہاریہ_Def *arrayNdx;
        bool ok = true;

        اگر (!def->name) {
            اگر (unlikely(!def->قدر) || def->قدر->common.type != STMT_اظہاریہ_ACTION_LIST)
                میدان_2 = "symbols"; /* Default to symbols field */
            ورنہ
                میدان_2 = "actions";
            arrayNdx = تہی;
        }
        ورنہ {
            ثابت حرف *elem;
            ok = اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &میدان_2,
                                &arrayNdx);
            اگر (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, میدان_2);
                ok = false;
            }
        }

        اگر (unlikely(!def->قدر)) {
            log_err(info->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", میدان_2);
            ok = false;
        }

        اگر (!ok || !سیٹ_Symbols_میدان(info, keyi, میدان_2, arrayNdx, &def->قدر))
            all_valid_entries = false;
    }

    واپس all_valid_entries;
}

ساکن bool
سیٹ_Explicit_گروہ(SymbolsInfo *info, کلید_Info *keyi)
{
    xkb_layout_اشاریہ_t i;
    گروہ_Info_2 *groupi;
    bool warn = false;

    اگر (info->explicit_گروہ == XKB_LAYOUT_INVALID)
        واپس true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        اگر (groupi->defined) {
            warn = true;
            Clear_گروہ_Info(groupi);
            Init_گروہ_Info(groupi);
        }
    }

    اگر (warn) {
        log_warn(info->ctx, XKB_WARNING_مضرب_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_گروہ + 1, کلید_Info_متن(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_گروہ + 1);
    اگر (info->explicit_گروہ > 0) {
        darray_شے(keyi->groups, info->explicit_گروہ) =
            darray_شے(keyi->groups, 0);
        Init_گروہ_Info(&darray_شے(keyi->groups, 0));
    }

    واپس true;
}

ساکن bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    کلید_Info keyi;

    keyi = info->default_کلید;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_کلید.groups);
    برائے (xkb_layout_اشاریہ_t i = 0; i < darray_سائز(keyi.groups); i++)
        Copy_گروہ_Info(&darray_شے(keyi.groups, i),
                      &darray_شے(info->default_کلید.groups, i));
    keyi.ملاؤ = stmt->ملاؤ;
    keyi.name = stmt->کلید_Name;

    اگر (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        سیٹ_Explicit_گروہ(info, &keyi) &&
        جوڑو_کلید_Symbols(info, &keyi, true)) {
        واپس true;
    }

    Clear_کلید_Info(&keyi);
    info->error_گنتی++;
    واپس false;
}

ساکن bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_اندراج tmp;
    ساخت xkb_context *ctx = info->ctx;

    اگر (!اظہاریہ_Resolve_Mod_نقاب(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        واپس false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    أكد(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_متن_V1 ||
           def->modifiers->common.type == STMT_اظہاریہ_IDENT);

    bool ok = true;
    tmp.ملاؤ = def->ملاؤ;

    برائے (اظہاریہ_Def *کلید = def->کلیدیں; کلید; کلید = (اظہاریہ_Def *) کلید->common.اگلا) {
        اگر (کلید->common.type == STMT_اظہاریہ_KEYNAME_LITERAL) {
            tmp.have_علامت = false;
            tmp.u.کلید_Name = کلید->کلید_name.کلید_name;
        }
        ورنہ اگر (کلید->common.type == STMT_اظہاریہ_KEYSYM_LITERAL) {
            اگر (کلید->keysym.keysym == XKB_کلید_No_علامت) {
                /* Invalid keysym: ignore. Error message already printed */
                جاری;
            } ورنہ {
                tmp.have_علامت = true;
                tmp.u.کلید_Sym = کلید->keysym.keysym;
            }
        }
        ورنہ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_اندراج,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            جاری;
        }

        ok = جوڑو_Mod_Map_اندراج(info, &tmp) && ok;
    }
    واپس ok;
}

ساکن خالی
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_محفوظ(file->name);

    برائے (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            توڑو;
        حالت STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            توڑو;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            توڑو;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            توڑو;
        حالت STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            توڑو;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            توڑو;
        طےشدہ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            توڑو;
        }

        اگر (!ok)
            info->error_گنتی++;

        اگر (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    محفوظ_map_name(file->name));
            توڑو;
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
ساکن ساخت xkb_کلید *
ڈھونڈو_کلید_For_علامت(ساخت xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_گروہ;
    xkb_layout_اشاریہ_t گروہ = 0;
    کرو {
        xkb_level_اشاریہ_t level = 0;
        got_one_گروہ = false;
        bool got_one_level;
        کرو {
            got_one_level = false;
            ساخت xkb_کلید *کلید;
            xkb_کلیدیں_foreach(کلید, keymap) {
                اگر (گروہ < کلید->عدد_groups &&
                    level < Xkb_کلید_عدد_سطحیں(کلید, گروہ)) {
                    got_one_گروہ = got_one_level = true;
                    ثابت xkb_keysym_گنتی_t عدد_syms =
                        کلید->groups[گروہ].سطحیں[level].عدد_syms;
                    اگر (عدد_syms > 1) {
                        برائے (xkb_keysym_گنتی_t k = 0; k < عدد_syms; k++) {
                            اگر (کلید->groups[گروہ].سطحیں[level].s.syms[k] == sym)
                                واپس کلید;
                        }
                    } ورنہ اگر (عدد_syms &&
                               کلید->groups[گروہ].سطحیں[level].s.sym == sym) {
                        واپس کلید;
                    }
                }
            }
            level++;
        } جب (got_one_level);
        گروہ++;
    } جب (got_one_گروہ);

    واپس تہی;
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
ساکن xkb_atom_t
ڈھونڈو_Automatic_Type(ساخت xkb_context *ctx, گروہ_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ثابت xkb_level_اشاریہ_t چوڑائی = darray_سائز(groupi->سطحیں);

#تعریف_کرو GET_SYM(level) \
    (darray_شے(groupi->سطحیں, level).عدد_syms == 0 ? \
        XKB_کلید_No_علامت : \
     darray_شے(groupi->سطحیں, level).عدد_syms == 1 ? \
        darray_شے(groupi->سطحیں, level).s.sym : \
     /* num_syms > 1 */ \
        darray_شے(groupi->سطحیں, level).s.syms[0])

    اگر (چوڑائی == 1 || چوڑائی <= 0)
        واپس xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    اگر (چوڑائی == 2) {
        اگر (xkb_keysym_is_زیریں(sym0) && xkb_keysym_is_بالائی_or_title(sym1))
            واپس xkb_atom_intern_literal(ctx, "ALPHABETIC");

        اگر (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            واپس xkb_atom_intern_literal(ctx, "KEYPAD");

        واپس xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    اگر (چوڑائی <= 4) {
        اگر (xkb_keysym_is_زیریں(sym0) && xkb_keysym_is_بالائی_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (چوڑائی == 4 ? GET_SYM(3) : XKB_کلید_No_علامت);

            اگر (xkb_keysym_is_زیریں(sym2) && xkb_keysym_is_بالائی_or_title(sym3))
                واپس xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            واپس xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        اگر (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            واپس xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        واپس xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    واپس XKB_ATOM_NONE;

#تعریف_منسوخ GET_SYM
}

ساکن ثابت ساخت xkb_کلید_type *
ڈھونڈو_Type_For_گروہ(ساخت xkb_keymap *keymap, کلید_Info *keyi,
                 xkb_layout_اشاریہ_t گروہ, bool *explicit_type)
{
    گروہ_Info_2 *groupi = &darray_شے(keyi->groups, گروہ);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    اگر (type_name == XKB_ATOM_NONE) {
        اگر (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ورنہ {
            type_name = ڈھونڈو_Automatic_Type(keymap->ctx, groupi);
            اگر (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    اگر (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_کلید_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 کلید_Name_متن(keymap->ctx, keyi->name), گروہ + 1,
                 darray_سائز(groupi->سطحیں));
        جاؤ use_default;
    }

    darray_سائز_t i;
    برائے (i = 0; i < keymap->عدد_types; i++)
        اگر (keymap->types[i].name == type_name)
            توڑو;

    اگر (i >= keymap->عدد_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_کلید_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_متن(keymap->ctx, type_name),
                 کلید_Name_متن(keymap->ctx, keyi->name), گروہ + 1);
        جاؤ use_default;
    }

    keymap->types[i].required = true;
    واپس &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    واپس &keymap->types[0];
}

ساکن bool
Copy_Symbols_Def_مقصد_Keymap(ساخت xkb_keymap *keymap, SymbolsInfo *info,
                       کلید_Info *keyi)
{
    ساخت xkb_کلید *کلید;
    گروہ_Info_2 *groupi;
    ثابت گروہ_Info_2 *group0;
    xkb_layout_اشاریہ_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    کلید = Xkb_کلید_By_Name(keymap, keyi->name, false);
    اگر (!کلید) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                کلید_Info_متن(info, keyi));
        واپس false;
    }

    /* Find the range of groups we need. */
    کلید->عدد_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ثابت bool has_explicit_type = (keyi->defined & کلید_میدان_DEFAULT_TYPE)
                                    || (groupi->defined & گروہ_میدان_TYPE);
        اگر (darray_سائز(groupi->سطحیں) > 0 || has_explicit_type)
            کلید->عدد_groups = i + 1;
        اگر (has_explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    اگر (کلید->عدد_groups <= 0) {
        /* A key with no group may still have other fields defined */
        اگر (keyi->defined)
            جاؤ کلید_fields;
        ورنہ
            واپس false;
    }

    darray_resize(keyi->groups, کلید->عدد_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_شے(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        اگر (groupi->defined)
            جاری;

        Copy_گروہ_Info(groupi, group0);
    }

    کلید->groups = صفر_حافظہ_دو(کلید->عدد_groups, حجم(*کلید->groups));
    أكد(darray_سائز(keyi->groups) == کلید->عدد_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #اگر_تعریف_شدہ __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < کلید->عدد_groups);
        #اگر_ختم
        bool explicit_type = false;
        ثابت ساخت xkb_کلید_type * ثابت type =
            ڈھونڈو_Type_For_گروہ(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        اگر (type->عدد_سطحیں < darray_سائز(groupi->سطحیں)) {
            ساخت xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_متن(keymap->ctx, type->name), type->عدد_سطحیں,
                    کلید_Info_متن(info, keyi),
                    darray_سائز(groupi->سطحیں));

            darray_foreach_from(leveli, groupi->سطحیں, type->عدد_سطحیں)
                clear_level(leveli);
        }
        darray_resize0(groupi->سطحیں, type->عدد_سطحیں);

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
        ساخت xkb_level *leveli;
        darray_foreach(leveli, groupi->سطحیں) {
            انتخاب (leveli->عدد_syms) {
            حالت 0:
                leveli->بالائی = XKB_کلید_No_علامت;
                توڑو;
            حالت 1:
                leveli->بالائی = xkb_keysym_مقصد_بالائی(leveli->s.sym);
                توڑو;
            طےشدہ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_بالائی = false;
                برائے (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++) {
                    ثابت xkb_keysym_t بالائی =
                        xkb_keysym_مقصد_بالائی(leveli->s.syms[k]);
                    اگر (بالائی != leveli->s.syms[k]) {
                        leveli->has_بالائی = true;
                        توڑو;
                    }
                }
                اگر (leveli->has_بالائی) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (سائز_t) 2 * leveli->عدد_syms *
                                             حجم(*leveli->s.syms));
                    اگر (!leveli->s.syms)
                        واپس false; /* FIXME: better handling? */
                    برائے (xkb_keysym_گنتی_t k = 0; k < leveli->عدد_syms; k++) {
                        leveli->s.syms[leveli->عدد_syms + k] =
                            xkb_keysym_مقصد_بالائی(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->سطحیں, &کلید->groups[i].سطحیں, تہی);
        اگر (کلید->groups[i].type->عدد_سطحیں > 1 ||
            کلید->groups[i].سطحیں[0].عدد_syms > 0) {
            کلید->groups[i].explicit_symbols = true;
            کلید->explicit |= EXPLICIT_SYMBOLS;
        }
        اگر (groupi->defined & گروہ_میدان_ACTS) { // FIXME
            کلید->groups[i].explicit_actions = true;
            کلید->explicit |= EXPLICIT_INTERP;
        }
        اگر (کلید->groups[i].explicit_type)
            کلید->explicit |= EXPLICIT_TYPES;
    }

    کلید->برآمد_کا_range_pending_گروہ = keyi->برآمد_کا_range_pending_گروہ;
    کلید->برآمد_کا_range_گروہ_نمبر = keyi->برآمد_کا_range_گروہ_نمبر;
    کلید->برآمد_کا_range_گروہ_policy = keyi->برآمد_کا_range_گروہ_policy;

کلید_fields:
    اگر (keyi->defined & کلید_میدان_VMODMAP) {
        کلید->vmodmap = keyi->vmodmap;
        کلید->explicit |= EXPLICIT_VMODMAP;
    }

    اگر (keyi->repeat != کلید_REPEAT_UNDEFINED) {
        کلید->repeats = (keyi->repeat == کلید_REPEAT_YES);
        کلید->explicit |= EXPLICIT_REPEAT;
    }

    اگر ((keyi->defined & کلید_میدان_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        کلید->overlays = keyi->overlays;
        اگر (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_نقاب_t بقایا = کلید->overlays;
            ثابت ساخت xkb_کلید **overlays_کلیدیں = keyi->overlays_کلیدیں;
            جب (بقایا) {
                /* isolate lowest set bit */
                ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                    بقایا &
                    (xkb_overlay_نقاب_t)((غیرعلامتی)~بقایا + 1u)
                );
                بقایا = (بقایا & ~lsb);
                اگر (*overlays_کلیدیں) {
                    overlays_کلیدیں++;
                } ورنہ {
                    /* drop current null value (overlayN=none) */
                    کلید->overlays &= ~lsb;
                }
            }

            اگر (کلید->overlays) {
                /* Steal */
                کلید->overlays_کلیدیں = keyi->overlays_کلیدیں;
                keyi->overlays_کلیدیں = تہی;
                keyi->overlays_alloc = 0;

                کلید->overlays_inline = false;
                کلید->explicit |= EXPLICIT_OVERLAY;
            }
        } ورنہ {
            کلید->overlay_کلید = keyi->overlay_کلید;
            کلید->overlays_inline = true;
            کلید->explicit |= EXPLICIT_OVERLAY;
        }
    }

    واپس true;
}

ساکن bool
Copy_Mod_Map_Def_مقصد_Keymap(ساخت xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_اندراج *اندراج_2)
{
    ساخت xkb_کلید *کلید;

    اگر (!اندراج_2->have_علامت) {
        کلید = Xkb_کلید_By_Name(keymap, اندراج_2->u.کلید_Name, true);
        اگر (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    کلید_Name_متن(info->ctx, اندراج_2->u.کلید_Name),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, اندراج_2->mods));
            اندراج_2->mods = 0; /* disable */
            واپس false;
        }
    }
    ورنہ {
        کلید = ڈھونڈو_کلید_For_علامت(keymap, اندراج_2->u.کلید_Sym);
        اگر (!کلید) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_علامت,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_متن(info->ctx, اندراج_2->u.کلید_Sym),
                    Mod_نقاب_متن(info->ctx, MOD_REAL, &info->mods, اندراج_2->mods));
            اندراج_2->mods = 0; /* disable */
            واپس false;
        }
    }

    /* Skip modMap None */
    اگر (اندراج_2->mods) {
        کلید->modmap |= اندراج_2->mods;
        اندراج_2->کلید_رمز = کلید->keycode;
    }

    واپس true;
}

ساکن bool
Copy_Symbols_مقصد_Keymap(ساخت xkb_keymap *keymap, SymbolsInfo *info)
{
    کلید_Info *keyi;

    keymap->symbols_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->عدد_گروہ_نام = (xkb_layout_اشاریہ_t)darray_سائز(info->گروہ_نام);
    darray_steal(info->گروہ_نام, &keymap->گروہ_نام, تہی);

    darray_foreach(keyi, info->کلیدیں)
        اگر (!Copy_Symbols_Def_مقصد_Keymap(keymap, info, keyi))
            info->error_گنتی++;

    اگر (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ساخت xkb_کلید *کلید;

        xkb_کلیدیں_foreach(کلید, keymap) {
            اگر (کلید->name == XKB_ATOM_NONE)
                جاری;

            اگر (کلید->عدد_groups < 1)
                log_info(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "No symbols defined for %s\n",
                         کلید_Name_متن(info->ctx, کلید->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_اندراج *mm;
    darray_foreach(mm, info->modmaps)
        اگر (!Copy_Mod_Map_Def_مقصد_Keymap(keymap, info, mm))
            info->error_گنتی++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->عدد_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    واپس true;
}

bool
CompileSymbols(XkbFile *file, ساخت xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    اگر (file !=تہی)
        HandleSymbolsFile(&info, file);

    اگر (info.error_گنتی != 0)
        جاؤ err_info;

    اگر (!Copy_Symbols_مقصد_Keymap(&keymap_info->keymap, &info))
        جاؤ err_info;

    ClearSymbolsInfo(&info);
    واپس true;

err_info:
    ClearSymbolsInfo(&info);
    واپس false;
}
