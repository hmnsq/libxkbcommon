/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "xkbcomp-priv.h"
#ضمن "ast.h"
#ضمن "action.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "text.h"
#ضمن "vmod.h"
#ضمن "util-mem.h"

تعداد si_فیلد {
    SI_فیلد_VIRTUAL_MOD = (1 << 0),
    SI_فیلد_ACTION = (1 << 1),
    SI_فیلد_AUTO_REPEAT = (1 << 2),
    SI_فیلد_LEVEL_ONE_ONLY = (1 << 3),
};

تعريفنوع بنية {
    تعداد si_فیلد defined;
    تعداد ادغام_نما ادغام;

    بنية xkb_sym_interpret interp;
} SymInterpInfo;

تعداد led_فیلد {
    LED_فیلد_MODS = (1 << 0),
    LED_فیلد_GROUPS = (1 << 1),
    LED_فیلد_CTRLS = (1 << 2),
};

تعريفنوع بنية {
    تعداد led_فیلد defined;
    تعداد ادغام_نما ادغام;

    بنية xkb_led led;
} LedInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_شمار;
    بیعلامت صحيح include_عمق;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_شاخص_t رقم_leds;
    ActionsInfo default_actions;
    بنية xkb_mod_مجموعه mods;

    ثابت بنية xkb_keymap_info *keymap_info;
    بنية xkb_context *ctx;
} CompatInfo;

ساكن ثابت حرف *
si_متن(SymInterpInfo *si, CompatInfo *info)
{
    حرف *buf = xkb_context_get_میانگیر(info->ctx, 128);

    إذا (si == &info->default_interp)
        أرجع "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_متن(info->ctx, si->interp.sym),
             SIMatch_متن(si->interp.تطابق),
             Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    أرجع buf;
}

ساكن مضمن bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت حرف *فیلد_2)
{
    أرجع ReportNotArray(info->ctx, "symbol interpretation", فیلد_2,
                          si_متن(si, info));
}

ساكن مضمن bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت حرف *فیلد_2,
                ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                         "symbol interpretation", فیلد_2,
                         si_متن(si, info), wanted);
}

ساكن ثابت حرف*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    إذا (ledi == &info->default_led) {
        أكد(xkb_atom_متن(info->ctx, ledi->led.name) == عدم);
        أرجع "default";
    } وإلا {
        أكد(xkb_atom_متن(info->ctx, ledi->led.name) != عدم);
        أرجع xkb_atom_متن(info->ctx, ledi->led.name);
    }
}

ساكن مضمن bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت حرف *فیلد_2,
                 ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                         "indicator map",
                         فیلد_2, LEDText(info, ledi), wanted);
}

ساكن مضمن bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت حرف *فیلد_2)
{
    أرجع ReportNotArray(info->ctx, "indicator map", فیلد_2,
                          LEDText(info, ledi));
}

ساكن مضمن فراغ
InitInterp(SymInterpInfo *info)
{
    info->ادغام = ادغام_DEFAULT; /*استفادهنشده*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_کلید_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_کلید_REPEAT, "invalid initial value");
}

ساكن مضمن فراغ
InitLED(LedInfo *info)
{
    info->ادغام = ادغام_DEFAULT; /*استفادهنشده*/
}

ساكن فراغ
InitCompatInfo(CompatInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
               بیعلامت صحيح include_عمق, ثابت بنية xkb_mod_مجموعه *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_عمق = include_عمق;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_عمق > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ساكن فراغ
ClearCompatInfo(CompatInfo *info)
{
    حرر(info->name);
    darray_free(info->interps);
}

ساكن SymInterpInfo *
پیدا_کن_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        إذا (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.تطابق == new->interp.تطابق)
            أرجع old;

    أرجع عدم;
}

ساكن bool
Use_New_Interp_فیلد(تعداد si_فیلد فیلد_2, تعداد si_فیلد old, تعداد si_فیلد new,
                  bool clobber, bool report, تعداد si_فیلد *collide)
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
ادغام_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->ادغام != ادغام_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    تعداد si_فیلد collide = 0;

    إذا (new->ادغام == ادغام_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_متن(new, info));
        *old = *new;
        أرجع true;
    }

    إذا (Use_New_Interp_فیلد(SI_فیلد_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_فیلد_VIRTUAL_MOD;
    }
    إذا (Use_New_Interp_فیلد(SI_فیلد_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        إذا (old->interp.رقم_actions > 1) {
                حرر(old->interp.a.actions);
            }
            old->interp.رقم_actions = new->interp.رقم_actions;
            إذا (new->interp.رقم_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.رقم_actions = 0;
            } وإلا {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_فیلد_ACTION;
    }
    إذا (Use_New_Interp_فیلد(SI_فیلد_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_فیلد_AUTO_REPEAT;
    }
    إذا (Use_New_Interp_فیلد(SI_فیلد_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_فیلد_LEVEL_ONE_ONLY;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_متن(old, info),
                 (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
جمع_کن_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = پیدا_کن_Matching_Interp(info, new);
    إذا (old)
        أرجع ادغام_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    أرجع true;
}

/***====================================================================***/

// TODO: more lenient?
ساكن bool
ResolveStateAndPredicate(عبارت_Def *عبارت, تعداد xkb_تطابق_operation *pred_rtrn,
                         xkb_mod_ماسک_t *mods_rtrn, CompatInfo *info)
{
    إذا (عبارت == عدم) {
        *pred_rtrn = تطابق_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_ماسک_ALL;
        أرجع true;
    }

    *pred_rtrn = تطابق_EXACTLY;
    إذا (عبارت->common.type == STMT_عبارت_ACTION_DECL) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, عبارت->action.name);
        بیعلامت صحيح pred = 0;
        إذا (!LookupString(sym_Interpret_تطابق_ماسک_نامها, pred_txt, &pred) ||
            !عبارت->action.آرگومانها || عبارت->action.آرگومانها->common.بعدی) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            أرجع false;
        }
        *pred_rtrn = (تعداد xkb_تطابق_operation) pred;
        عبارت = عبارت->action.آرگومانها;
    }
    وإلا إذا (عبارت->common.type == STMT_عبارت_IDENT) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, عبارت->ident.ident);
        إذا (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = تطابق_ANY;
            *mods_rtrn = MOD_REAL_ماسک_ALL;
            أرجع true;
        }
    }

    أرجع عبارت_Resolve_Mod_ماسک(info->ctx, عبارت, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ساكن bool
UseNewLEDField(تعداد led_فیلد فیلد_2, تعداد led_فیلد old, تعداد led_فیلد new,
               bool clobber, bool report, تعداد led_فیلد *collide)
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
ادغام_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    تعداد led_فیلد collide;
    ثابت bool clobber = (new->ادغام != ادغام_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        أرجع true;
    }

    إذا (new->ادغام == ادغام_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        أرجع true;
    }

    collide = 0;
    إذا (UseNewLEDField(LED_فیلد_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_فیلد_MODS;
    }
    إذا (UseNewLEDField(LED_فیلد_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_فیلد_GROUPS;
    }
    إذا (UseNewLEDField(LED_فیلد_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_فیلد_CTRLS;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
جمع_کن_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    لكل (xkb_led_شاخص_t i = 0; i < info->رقم_leds; i++) {
        LedInfo *old = &info->leds[i];

        إذا (old->led.name != new->led.name)
            استمر;

        أرجع ادغام_Led_Map(info, old, new, same_file);
    }

    إذا (info->رقم_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        أرجع false;
    }
    info->leds[info->رقم_leds++] = *new;
    أرجع true;
}

ساكن فراغ
ادغام_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        تعداد ادغام_نما ادغام)
{
    إذا (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        أرجع;
    }

    ادغام_Mod_Sets(into->ctx, &into->mods, &from->mods, ادغام);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_خالی(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    وإلا {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->ادغام = ادغام;
            إذا (!جمع_کن_Interp(into, si, false))
                into->error_شمار++;
        }
    }

    إذا (into->رقم_leds == 0) {
        کپی_حافظه(into->leds, from->leds, حجم(*from->leds) * from->رقم_leds);
        into->رقم_leds = from->رقم_leds;
        from->رقم_leds = 0;
    }
    وإلا {
        لكل (xkb_led_شاخص_t i = 0; i < from->رقم_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->ادغام = ادغام;
            إذا (!جمع_کن_Led_Map(into, ledi, false))
                into->error_شمار++;
        }
    }
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ساكن bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    إذا (Exceeds_Include_Max_عمق(info->ctx, info->include_عمق)) {
        info->error_شمار += 10;
        أرجع false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_عمق + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بعدی_incl) {
        CompatInfo بعدی_incl;
        XkbFile *file;

        حرف مسیر[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  مسیر, حجم(مسیر));
        إذا (!file) {
            info->error_شمار += 10;
            ClearCompatInfo(&included);
            أرجع false;
        }

        InitCompatInfo(&بعدی_incl, info->keymap_info, info->include_عمق + 1,
                       &included.mods);
        بعدی_incl.default_interp = info->default_interp;
        بعدی_incl.default_led = info->default_led;

        HandleCompatMapFile(&بعدی_incl, file);

        ادغام_Included_Compat_Maps(&included, &بعدی_incl, stmt->ادغام);

        ClearCompatInfo(&بعدی_incl);
        FreeXkbFile(file);
    }

    ادغام_Included_Compat_Maps(info, &included, include->ادغام);
    ClearCompatInfo(&included);

    أرجع (info->error_شمار == 0);
}

ساكن bool
مجموعه_Interp_فیلد(CompatInfo *info, SymInterpInfo *si, ثابت حرف *فیلد_2,
               عبارت_Def *arrayNdx, عبارت_Def *مقدار)
{
    إذا (istreq(فیلد_2, "action")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, فیلد_2);

        إذا (مقدار->common.type == STMT_عبارت_ACTION_LIST) {
            بیعلامت صحيح رقم_actions = 0;
            لكل (عبارت_Def *act = مقدار->actions.actions;
                 act; act = (عبارت_Def *) act->common.بعدی)
                 رقم_actions++;

            إذا (رقم_actions > MAX_ACTIONS_در_هر_LEVEL) {
                log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_متن(si, info), MAX_ACTIONS_در_هر_LEVEL, رقم_actions);
                أرجع false;
            }

            si->interp.رقم_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اتحاد xkb_action) actions = darray_new();
            لكل (عبارت_Def *act = مقدار->actions.actions;
                 act; act = (عبارت_Def *) act->common.بعدی) {
                اتحاد xkb_action مقصد_Act = { 0 };
                بدل (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &مقصد_Act)) {
                حالة PARSER_RECOVERABLE_ERROR:
                    مقصد_Act.type = ACTION_TYPE_NONE;
                    اكسر;
                حالة PARSER_FATAL_ERROR:
                    darray_free(actions);
                    أرجع false;
                افتراضي:
                    ;
                }
                إذا (مقصد_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } وإلا إذا (likely(رقم_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.رقم_actions = 1;
                    si->interp.a.action = مقصد_Act;
                } وإلا {
                    darray_append(actions, مقصد_Act);
                }
            }
            بدل (darray_اندازه(actions)) {
            حالة 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.رقم_actions <= 1);
                اكسر;
            حالة 1:
                /* One action: some actions were dropped */
                si->interp.رقم_actions = 1;
                si->interp.a.action = darray_قلم(actions, 1);
                darray_free(actions);
                اكسر;
            افتراضي:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.رقم_actions =
                    (xkb_action_شمار_t) darray_اندازه(actions);
                darray_steal(actions, &si->interp.a.actions, عدم);
            }
        }
        وإلا {
            بدل (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, مقدار, &si->interp.a.action)) {
            حالة PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.رقم_actions = 0;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                أرجع false;
            افتراضي:
                si->interp.رقم_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_فیلد_ACTION;
    }
    وإلا إذا (istreq(فیلد_2, "virtualmodifier") ||
             istreq(فیلد_2, "virtualmod")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, فیلد_2);

        xkb_mod_شاخص_t ndx = 0;
        إذا (!عبارت_Resolve_Mod(info->ctx, مقدار, MOD_VIRT, &info->mods, &ndx))
            أرجع ReportSIBadType(info, si, فیلد_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_فیلد_VIRTUAL_MOD;
    }
    وإلا إذا (istreq(فیلد_2, "repeat")) {
        bool مجموعه = false;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, فیلد_2);

        إذا (!عبارت_Resolve_Boolean(info->ctx, مقدار, &مجموعه))
            أرجع ReportSIBadType(info, si, فیلد_2, "boolean");

        si->interp.repeat = مجموعه;

        si->defined |= SI_فیلد_AUTO_REPEAT;
    }
    وإلا إذا (istreq(فیلد_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_پیام_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(فیلد_2, "usemodmap") ||
             istreq(فیلد_2, "usemodmapmods")) {
        uint32_t val = 0;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, فیلد_2);

        إذا (!عبارت_Resolve_Enum(info->ctx, مقدار, &val, use_Mod_Map_مقدار_نامها))
            أرجع ReportSIBadType(info, si, فیلد_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_فیلد_LEVEL_ONE_ONLY;
    }
    وإلا {
        Report_Bad_فیلد(info->ctx, "symbol interpretation", فیلد_2,
                       si_متن(si, info));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    أرجع true;
}

ساكن bool
مجموعه_Led_Map_فیلد(CompatInfo *info, LedInfo *ledi, ثابت حرف *فیلد_2,
               عبارت_Def *arrayNdx, عبارت_Def **مقدار_ptr)
{
    عبارت_Def * محدود ثابت مقدار = *مقدار_ptr;

    إذا (istreq(فیلد_2, "modifiers") || istreq(فیلد_2, "mods")) {
        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, فیلد_2);

        إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, مقدار, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            أرجع ReportLedBadType(info, ledi, فیلد_2, "modifier mask");

        ledi->defined |= LED_فیلد_MODS;
    }
    وإلا إذا (istreq(فیلد_2, "groups")) {
        xkb_layout_ماسک_t ماسک = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, فیلد_2);

        bool pending = false;
        إذا (!عبارت_Resolve_گروه_ماسک(info->keymap_info, مقدار, &ماسک, &pending)) {
            إذا (pending) {
                ledi->led.pending_groups = true;
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
                static_assert(حجم(pending_شاخص) == حجم(ماسک),
                              "Cannot save pending computation");
                ماسک = pending_شاخص;
            } وإلا {
                أرجع ReportLedBadType(info, ledi, فیلد_2, "group mask");
            }
        } وإلا {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = ماسک;
        ledi->defined |= LED_فیلد_GROUPS;
    }
    وإلا إذا (istreq(فیلد_2, "controls") || istreq(فیلد_2, "ctrls")) {
        uint32_t ماسک = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, فیلد_2);

        ثابت uint8_t جابه_جایی = info->keymap_info->features.controls_name_جابه_جایی;
        إذا (!عبارت_Resolve_ماسک(info->ctx, مقدار, &ماسک, ctrl_ماسک_نامها + جابه_جایی))
            أرجع ReportLedBadType(info, ledi, فیلد_2, "controls mask");

        ledi->led.ctrls = ماسک;
        ledi->defined |= LED_فیلد_CTRLS;
    }
    وإلا إذا (istreq(فیلد_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_پیام_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(فیلد_2, "whichmodstate") ||
             istreq(فیلد_2, "whichmodifierstate")) {
        uint32_t ماسک = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, فیلد_2);

        إذا (!عبارت_Resolve_ماسک(info->ctx, مقدار, &ماسک,
                             mod_Component_ماسک_نامها))
            أرجع ReportLedBadType(info, ledi, فیلد_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = ماسک;
    }
    وإلا إذا (istreq(فیلد_2, "whichgroupstate")) {
        uint32_t ماسک = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, فیلد_2);

        إذا (!عبارت_Resolve_ماسک(info->ctx, مقدار, &ماسک,
                             گروه_Component_ماسک_نامها))
            أرجع ReportLedBadType(info, ledi, فیلد_2,
                                    "mask of group state components");

        ledi->led.which_groups = ماسک;
    }
    وإلا إذا (istreq(فیلد_2, "driveskbd") ||
             istreq(فیلد_2, "driveskeyboard") ||
             istreq(فیلد_2, "leddriveskbd") ||
             istreq(فیلد_2, "leddriveskeyboard") ||
             istreq(فیلد_2, "indicatordriveskbd") ||
             istreq(فیلد_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_پیام_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", فیلد_2);
    }
    وإلا إذا (istreq(فیلد_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا {
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                فیلد_2, LEDText(info, ledi));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    أرجع true;
}

ساكن bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *ndx;
    bool ret;

    إذا (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &فیلد_2, &ndx)) {
        ret = false;
    }
    وإلا إذا (elem && istreq(elem, "interpret")) {
        SymInterpInfo موقت = {0};
        InitInterp(&موقت);
        /* Do not replace the whole interpret, only the current field */
        موقت.ادغام = (موقت.ادغام == ادغام_REPLACE)
            ? ادغام_OVERRIDE
            : stmt->ادغام;
        ret = مجموعه_Interp_فیلد(info, &موقت, فیلد_2, ndx, stmt->مقدار);
        إذا (ret)
            ادغام_Interp(info, &info->default_interp, &موقت, true);
    }
    وإلا إذا (elem && istreq(elem, "indicator")) {
        LedInfo موقت = {0};
        InitLED(&موقت);
        /* Do not replace the whole LED, only the current field */
        موقت.ادغام = (موقت.ادغام == ادغام_REPLACE)
            ? ادغام_OVERRIDE
            : stmt->ادغام;
        ret = مجموعه_Led_Map_فیلد(info, &موقت, فیلد_2, ndx, &stmt->مقدار);
        إذا (ret)
            ادغام_Led_Map(info, &info->default_led, &موقت, true);
    }
    وإلا إذا (elem) {
        ret = (مجموعه_Default_Action_فیلد(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, فیلد_2, ndx,
                                     &stmt->مقدار, stmt->ادغام) !=
               PARSER_FATAL_ERROR);
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_فیلد,
                "Default defined for unknown field \"%s\"; Ignored\n", فیلد_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    أرجع ret;
}

ساكن bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.بعدی) {
        إذا (!عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &فیلد_2, &arrayNdx)) {
            ok = false;
            استمر;
        }
        إذا (elem) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, فیلد_2);
            ok = false;
            استمر;
        }
        إذا (!مجموعه_Interp_فیلد(info, si, فیلد_2, arrayNdx, def->مقدار))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    تعداد xkb_تطابق_operation pred;
    xkb_mod_ماسک_t mods;
    SymInterpInfo si;

    إذا (!ResolveStateAndPredicate(def->تطابق, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        أرجع false;
    }

    si = info->default_interp;
    si.ادغام = def->ادغام;
    si.interp.sym = def->sym;
    si.interp.تطابق = pred;
    si.interp.mods = mods;

    إذا (!HandleInterpBody(info, def->def, &si)) {
        info->error_شمار++;
        أرجع false;
    }

    إذا (!جمع_کن_Interp(info, &si, true)) {
        info->error_شمار++;
        أرجع false;
    }

    أرجع true;
}

ساكن bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.ادغام = def->ادغام;
    ledi.led.name = def->name;

    bool ok = true;
    لكل (VarDef *var = def->body; var != عدم; var = (VarDef *) var->common.بعدی) {
        ثابت حرف *elem, *فیلد_2;
        عبارت_Def *arrayNdx;
        إذا (!عبارت_Resolve_Lhs(info->ctx, var->name, &elem, &فیلد_2, &arrayNdx)) {
            ok = false;
            استمر;
        }

        إذا (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, فیلد_2);
            ok = false;
        }
        وإلا {
            إذا (!مجموعه_Led_Map_فیلد(info, &ledi, فیلد_2, arrayNdx, &var->مقدار))
                ok = false;
        }
    }

    أرجع ok && جمع_کن_Led_Map(info, &ledi, true);
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_امن(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بعدی) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            اكسر;
        حالة STMT_گروه_COMPAT:
            log_dbg(info->ctx, XKB_LOG_پیام_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            اكسر;
        حالة STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            اكسر;
        حالة STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            اكسر;
        حالة STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_شمار++;

        إذا (info->error_شمار > 10) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    امن_map_name(file->name));
            اكسر;
        }
    }
}

/* Temporary struct for CopyInterps. */
بنية collect {
    darray(بنية xkb_sym_interpret) sym_interprets;
};

ساكن فراغ
CopyInterps(CompatInfo *info, bool need_نماد, تعداد xkb_تطابق_operation pred,
            بنية collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        إذا (si->interp.تطابق == pred &&
            (si->interp.sym != XKB_کلید_No_نماد) == need_نماد)
            darray_append(collect->sym_interprets, si->interp);
}

ساكن فراغ
Copy_Led_Map_Defs_مقصد_Keymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    لكل (xkb_led_شاخص_t idx = 0; idx < info->رقم_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_شاخص_t i;
        بنية xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            إذا (led->name == ledi->led.name)
                اكسر;

        /* Not previously declared; create it with next free index. */
        إذا (i >= keymap->رقم_leds) {
            log_dbg(keymap->ctx, XKB_LOG_پیام_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                إذا (led->name == XKB_ATOM_NONE)
                    اكسر;

            إذا (i >= keymap->رقم_leds) {
                /* Not place to put it; ignore. */
                إذا (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_پیام_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    استمر;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->رقم_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        إذا (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        إذا (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ساكن bool
Copy_Compat_مقصد_Keymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_امن(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    إذا (!darray_خالی(info->interps)) {
        بنية collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, تطابق_EXACTLY, &collect);
        CopyInterps(info, true, تطابق_ALL, &collect);
        CopyInterps(info, true, تطابق_NONE, &collect);
        CopyInterps(info, true, تطابق_ANY, &collect);
        CopyInterps(info, true, تطابق_ANY_OR_NONE, &collect);
        CopyInterps(info, false, تطابق_EXACTLY, &collect);
        CopyInterps(info, false, تطابق_ALL, &collect);
        CopyInterps(info, false, تطابق_NONE, &collect);
        CopyInterps(info, false, تطابق_ANY, &collect);
        CopyInterps(info, false, تطابق_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->رقم_sym_interprets);
    }

    Copy_Led_Map_Defs_مقصد_Keymap(keymap, info);

    أرجع true;
}

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    إذا (file != عدم)
        HandleCompatMapFile(&info, file);

    إذا (info.error_شمار != 0)
        اذهب err_info;

    إذا (!Copy_Compat_مقصد_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearCompatInfo(&info);
    أرجع true;

err_info:
    ClearCompatInfo(&info);
    أرجع false;
}
