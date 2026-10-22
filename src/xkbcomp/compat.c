/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "xkbcomp-priv.h"
#شامل "ast.h"
#شامل "action.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "text.h"
#شامل "vmod.h"
#شامل "util-mem.h"

شمار si_میدان {
    SI_میدان_VIRTUAL_MOD = (1 << 0),
    SI_میدان_ACTION = (1 << 1),
    SI_میدان_AUTO_REPEAT = (1 << 2),
    SI_میدان_LEVEL_ONE_ONLY = (1 << 3),
};

قسمتعریف ساخت {
    شمار si_میدان defined;
    شمار ملاؤ_منوال ملاؤ;

    ساخت xkb_sym_interpret interp;
} SymInterpInfo;

شمار led_میدان {
    LED_میدان_MODS = (1 << 0),
    LED_میدان_GROUPS = (1 << 1),
    LED_میدان_CTRLS = (1 << 2),
};

قسمتعریف ساخت {
    شمار led_میدان defined;
    شمار ملاؤ_منوال ملاؤ;

    ساخت xkb_led led;
} LedInfo;

قسمتعریف ساخت {
    حرف *name;
    صحیح error_گنتی;
    غیرعلامتی صحیح include_گہرائی;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_اشاریہ_t عدد_leds;
    ActionsInfo default_actions;
    ساخت xkb_mod_سیٹ mods;

    ثابت ساخت xkb_keymap_info *keymap_info;
    ساخت xkb_context *ctx;
} CompatInfo;

ساکن ثابت حرف *
si_متن(SymInterpInfo *si, CompatInfo *info)
{
    حرف *buf = xkb_context_get_بفر(info->ctx, 128);

    اگر (si == &info->default_interp)
        واپس "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_متن(info->ctx, si->interp.sym),
             SIMatch_متن(si->interp.مطابقت),
             Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    واپس buf;
}

ساکن اندرونی bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت حرف *میدان_2)
{
    واپس ReportNotArray(info->ctx, "symbol interpretation", میدان_2,
                          si_متن(si, info));
}

ساکن اندرونی bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت حرف *میدان_2,
                ثابت حرف *wanted)
{
    واپس ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                         "symbol interpretation", میدان_2,
                         si_متن(si, info), wanted);
}

ساکن ثابت حرف*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    اگر (ledi == &info->default_led) {
        أكد(xkb_atom_متن(info->ctx, ledi->led.name) == تہی);
        واپس "default";
    } ورنہ {
        أكد(xkb_atom_متن(info->ctx, ledi->led.name) != تہی);
        واپس xkb_atom_متن(info->ctx, ledi->led.name);
    }
}

ساکن اندرونی bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت حرف *میدان_2,
                 ثابت حرف *wanted)
{
    واپس ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                         "indicator map",
                         میدان_2, LEDText(info, ledi), wanted);
}

ساکن اندرونی bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت حرف *میدان_2)
{
    واپس ReportNotArray(info->ctx, "indicator map", میدان_2,
                          LEDText(info, ledi));
}

ساکن اندرونی خالی
InitInterp(SymInterpInfo *info)
{
    info->ملاؤ = ملاؤ_DEFAULT; /*غیر_مستعمل*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_کلید_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_کلید_REPEAT, "invalid initial value");
}

ساکن اندرونی خالی
InitLED(LedInfo *info)
{
    info->ملاؤ = ملاؤ_DEFAULT; /*غیر_مستعمل*/
}

ساکن خالی
InitCompatInfo(CompatInfo *info, ثابت ساخت xkb_keymap_info *keymap_info,
               غیرعلامتی صحیح include_گہرائی, ثابت ساخت xkb_mod_سیٹ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_گہرائی = include_گہرائی;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_گہرائی > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ساکن خالی
ClearCompatInfo(CompatInfo *info)
{
    آزاد(info->name);
    darray_free(info->interps);
}

ساکن SymInterpInfo *
ڈھونڈو_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        اگر (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.مطابقت == new->interp.مطابقت)
            واپس old;

    واپس تہی;
}

ساکن bool
Use_New_Interp_میدان(شمار si_میدان میدان_2, شمار si_میدان old, شمار si_میدان new,
                  bool clobber, bool report, شمار si_میدان *collide)
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
ملاؤ_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->ملاؤ != ملاؤ_AUGMENT);
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    شمار si_میدان collide = 0;

    اگر (new->ملاؤ == ملاؤ_REPLACE) {
        اگر (report)
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_متن(new, info));
        *old = *new;
        واپس true;
    }

    اگر (Use_New_Interp_میدان(SI_میدان_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_میدان_VIRTUAL_MOD;
    }
    اگر (Use_New_Interp_میدان(SI_میدان_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        اگر (old->interp.عدد_actions > 1) {
                آزاد(old->interp.a.actions);
            }
            old->interp.عدد_actions = new->interp.عدد_actions;
            اگر (new->interp.عدد_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.عدد_actions = 0;
            } ورنہ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_میدان_ACTION;
    }
    اگر (Use_New_Interp_میدان(SI_میدان_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_میدان_AUTO_REPEAT;
    }
    اگر (Use_New_Interp_میدان(SI_میدان_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_میدان_LEVEL_ONE_ONLY;
    }

    اگر (collide) {
        log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_متن(old, info),
                 (clobber ? "last" : "first"));
    }

    واپس true;
}

ساکن bool
جوڑو_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = ڈھونڈو_Matching_Interp(info, new);
    اگر (old)
        واپس ملاؤ_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    واپس true;
}

/***====================================================================***/

// TODO: more lenient?
ساکن bool
ResolveStateAndPredicate(اظہاریہ_Def *اظہاریہ, شمار xkb_مطابقت_operation *pred_rtrn,
                         xkb_mod_نقاب_t *mods_rtrn, CompatInfo *info)
{
    اگر (اظہاریہ == تہی) {
        *pred_rtrn = مطابقت_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_نقاب_ALL;
        واپس true;
    }

    *pred_rtrn = مطابقت_EXACTLY;
    اگر (اظہاریہ->common.type == STMT_اظہاریہ_ACTION_DECL) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, اظہاریہ->action.name);
        غیرعلامتی صحیح pred = 0;
        اگر (!LookupString(sym_Interpret_مطابقت_نقاب_نام, pred_txt, &pred) ||
            !اظہاریہ->action.دلائل || اظہاریہ->action.دلائل->common.اگلا) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            واپس false;
        }
        *pred_rtrn = (شمار xkb_مطابقت_operation) pred;
        اظہاریہ = اظہاریہ->action.دلائل;
    }
    ورنہ اگر (اظہاریہ->common.type == STMT_اظہاریہ_IDENT) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, اظہاریہ->ident.ident);
        اگر (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = مطابقت_ANY;
            *mods_rtrn = MOD_REAL_نقاب_ALL;
            واپس true;
        }
    }

    واپس اظہاریہ_Resolve_Mod_نقاب(info->ctx, اظہاریہ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ساکن bool
UseNewLEDField(شمار led_میدان میدان_2, شمار led_میدان old, شمار led_میدان new,
               bool clobber, bool report, شمار led_میدان *collide)
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
ملاؤ_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    شمار led_میدان collide;
    ثابت bool clobber = (new->ملاؤ != ملاؤ_AUGMENT);
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    اگر (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        واپس true;
    }

    اگر (new->ملاؤ == ملاؤ_REPLACE) {
        اگر (report)
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        واپس true;
    }

    collide = 0;
    اگر (UseNewLEDField(LED_میدان_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_میدان_MODS;
    }
    اگر (UseNewLEDField(LED_میدان_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_میدان_GROUPS;
    }
    اگر (UseNewLEDField(LED_میدان_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_میدان_CTRLS;
    }

    اگر (collide) {
        log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    واپس true;
}

ساکن bool
جوڑو_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    برائے (xkb_led_اشاریہ_t i = 0; i < info->عدد_leds; i++) {
        LedInfo *old = &info->leds[i];

        اگر (old->led.name != new->led.name)
            جاری;

        واپس ملاؤ_Led_Map(info, old, new, same_file);
    }

    اگر (info->عدد_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        واپس false;
    }
    info->leds[info->عدد_leds++] = *new;
    واپس true;
}

ساکن خالی
ملاؤ_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        شمار ملاؤ_منوال ملاؤ)
{
    اگر (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        واپس;
    }

    ملاؤ_Mod_Sets(into->ctx, &into->mods, &from->mods, ملاؤ);

    اگر (into->name == تہی) {
        into->name = steal(&from->name);
    }

    اگر (darray_خالی_خانے(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ورنہ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->ملاؤ = ملاؤ;
            اگر (!جوڑو_Interp(into, si, false))
                into->error_گنتی++;
        }
    }

    اگر (into->عدد_leds == 0) {
        حافظہ_نقل(into->leds, from->leds, حجم(*from->leds) * from->عدد_leds);
        into->عدد_leds = from->عدد_leds;
        from->عدد_leds = 0;
    }
    ورنہ {
        برائے (xkb_led_اشاریہ_t i = 0; i < from->عدد_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->ملاؤ = ملاؤ;
            اگر (!جوڑو_Led_Map(into, ledi, false))
                into->error_گنتی++;
        }
    }
}

ساکن خالی
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ساکن bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    اگر (Exceeds_Include_Max_گہرائی(info->ctx, info->include_گہرائی)) {
        info->error_گنتی += 10;
        واپس false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_گہرائی + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساخت parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برائے (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        CompatInfo اگلا_incl;
        XkbFile *file;

        حرف راستہ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  راستہ, حجم(راستہ));
        اگر (!file) {
            info->error_گنتی += 10;
            ClearCompatInfo(&included);
            واپس false;
        }

        InitCompatInfo(&اگلا_incl, info->keymap_info, info->include_گہرائی + 1,
                       &included.mods);
        اگلا_incl.default_interp = info->default_interp;
        اگلا_incl.default_led = info->default_led;

        HandleCompatMapFile(&اگلا_incl, file);

        ملاؤ_Included_Compat_Maps(&included, &اگلا_incl, stmt->ملاؤ);

        ClearCompatInfo(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_Compat_Maps(info, &included, include->ملاؤ);
    ClearCompatInfo(&included);

    واپس (info->error_گنتی == 0);
}

ساکن bool
سیٹ_Interp_میدان(CompatInfo *info, SymInterpInfo *si, ثابت حرف *میدان_2,
               اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر)
{
    اگر (istreq(میدان_2, "action")) {
        اگر (arrayNdx)
            واپس ReportSINotArray(info, si, میدان_2);

        اگر (قدر->common.type == STMT_اظہاریہ_ACTION_LIST) {
            غیرعلامتی صحیح عدد_actions = 0;
            برائے (اظہاریہ_Def *act = قدر->actions.actions;
                 act; act = (اظہاریہ_Def *) act->common.اگلا)
                 عدد_actions++;

            اگر (عدد_actions > MAX_ACTIONS_فی_LEVEL) {
                log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_متن(si, info), MAX_ACTIONS_فی_LEVEL, عدد_actions);
                واپس false;
            }

            si->interp.عدد_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اتحاد xkb_action) actions = darray_new();
            برائے (اظہاریہ_Def *act = قدر->actions.actions;
                 act; act = (اظہاریہ_Def *) act->common.اگلا) {
                اتحاد xkb_action مقصد_Act = { 0 };
                انتخاب (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &مقصد_Act)) {
                حالت PARSER_RECOVERABLE_ERROR:
                    مقصد_Act.type = ACTION_TYPE_NONE;
                    توڑو;
                حالت PARSER_FATAL_ERROR:
                    darray_free(actions);
                    واپس false;
                طےشدہ:
                    ;
                }
                اگر (مقصد_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ورنہ اگر (likely(عدد_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.عدد_actions = 1;
                    si->interp.a.action = مقصد_Act;
                } ورنہ {
                    darray_append(actions, مقصد_Act);
                }
            }
            انتخاب (darray_سائز(actions)) {
            حالت 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.عدد_actions <= 1);
                توڑو;
            حالت 1:
                /* One action: some actions were dropped */
                si->interp.عدد_actions = 1;
                si->interp.a.action = darray_شے(actions, 1);
                darray_free(actions);
                توڑو;
            طےشدہ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.عدد_actions =
                    (xkb_action_گنتی_t) darray_سائز(actions);
                darray_steal(actions, &si->interp.a.actions, تہی);
            }
        }
        ورنہ {
            انتخاب (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, قدر, &si->interp.a.action)) {
            حالت PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.عدد_actions = 0;
                توڑو;
            حالت PARSER_FATAL_ERROR:
                واپس false;
            طےشدہ:
                si->interp.عدد_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_میدان_ACTION;
    }
    ورنہ اگر (istreq(میدان_2, "virtualmodifier") ||
             istreq(میدان_2, "virtualmod")) {
        اگر (arrayNdx)
            واپس ReportSINotArray(info, si, میدان_2);

        xkb_mod_اشاریہ_t ndx = 0;
        اگر (!اظہاریہ_Resolve_Mod(info->ctx, قدر, MOD_VIRT, &info->mods, &ndx))
            واپس ReportSIBadType(info, si, میدان_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_میدان_VIRTUAL_MOD;
    }
    ورنہ اگر (istreq(میدان_2, "repeat")) {
        bool سیٹ = false;

        اگر (arrayNdx)
            واپس ReportSINotArray(info, si, میدان_2);

        اگر (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &سیٹ))
            واپس ReportSIBadType(info, si, میدان_2, "boolean");

        si->interp.repeat = سیٹ;

        si->defined |= SI_میدان_AUTO_REPEAT;
    }
    ورنہ اگر (istreq(میدان_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ورنہ اگر (istreq(میدان_2, "usemodmap") ||
             istreq(میدان_2, "usemodmapmods")) {
        uint32_t val = 0;

        اگر (arrayNdx)
            واپس ReportSINotArray(info, si, میدان_2);

        اگر (!اظہاریہ_Resolve_Enum(info->ctx, قدر, &val, use_Mod_Map_قدر_نام))
            واپس ReportSIBadType(info, si, میدان_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_میدان_LEVEL_ONE_ONLY;
    }
    ورنہ {
        Report_Bad_میدان(info->ctx, "symbol interpretation", میدان_2,
                       si_متن(si, info));
        واپس !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    واپس true;
}

ساکن bool
سیٹ_Led_Map_میدان(CompatInfo *info, LedInfo *ledi, ثابت حرف *میدان_2,
               اظہاریہ_Def *arrayNdx, اظہاریہ_Def **قدر_ptr)
{
    اظہاریہ_Def * محدود ثابت قدر = *قدر_ptr;

    اگر (istreq(میدان_2, "modifiers") || istreq(میدان_2, "mods")) {
        اگر (arrayNdx)
            واپس ReportLedNotArray(info, ledi, میدان_2);

        اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            واپس ReportLedBadType(info, ledi, میدان_2, "modifier mask");

        ledi->defined |= LED_میدان_MODS;
    }
    ورنہ اگر (istreq(میدان_2, "groups")) {
        xkb_layout_نقاب_t نقاب = 0;

        اگر (arrayNdx)
            واپس ReportLedNotArray(info, ledi, میدان_2);

        bool pending = false;
        اگر (!اظہاریہ_Resolve_گروہ_نقاب(info->keymap_info, قدر, &نقاب, &pending)) {
            اگر (pending) {
                ledi->led.pending_groups = true;
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
                static_assert(حجم(pending_اشاریہ) == حجم(نقاب),
                              "Cannot save pending computation");
                نقاب = pending_اشاریہ;
            } ورنہ {
                واپس ReportLedBadType(info, ledi, میدان_2, "group mask");
            }
        } ورنہ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = نقاب;
        ledi->defined |= LED_میدان_GROUPS;
    }
    ورنہ اگر (istreq(میدان_2, "controls") || istreq(میدان_2, "ctrls")) {
        uint32_t نقاب = 0;

        اگر (arrayNdx)
            واپس ReportLedNotArray(info, ledi, میدان_2);

        ثابت uint8_t انحراف = info->keymap_info->features.controls_name_انحراف;
        اگر (!اظہاریہ_Resolve_نقاب(info->ctx, قدر, &نقاب, ctrl_نقاب_نام + انحراف))
            واپس ReportLedBadType(info, ledi, میدان_2, "controls mask");

        ledi->led.ctrls = نقاب;
        ledi->defined |= LED_میدان_CTRLS;
    }
    ورنہ اگر (istreq(میدان_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ورنہ اگر (istreq(میدان_2, "whichmodstate") ||
             istreq(میدان_2, "whichmodifierstate")) {
        uint32_t نقاب = 0;

        اگر (arrayNdx)
            واپس ReportLedNotArray(info, ledi, میدان_2);

        اگر (!اظہاریہ_Resolve_نقاب(info->ctx, قدر, &نقاب,
                             mod_Component_نقاب_نام))
            واپس ReportLedBadType(info, ledi, میدان_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = نقاب;
    }
    ورنہ اگر (istreq(میدان_2, "whichgroupstate")) {
        uint32_t نقاب = 0;

        اگر (arrayNdx)
            واپس ReportLedNotArray(info, ledi, میدان_2);

        اگر (!اظہاریہ_Resolve_نقاب(info->ctx, قدر, &نقاب,
                             گروہ_Component_نقاب_نام))
            واپس ReportLedBadType(info, ledi, میدان_2,
                                    "mask of group state components");

        ledi->led.which_groups = نقاب;
    }
    ورنہ اگر (istreq(میدان_2, "driveskbd") ||
             istreq(میدان_2, "driveskeyboard") ||
             istreq(میدان_2, "leddriveskbd") ||
             istreq(میدان_2, "leddriveskeyboard") ||
             istreq(میدان_2, "indicatordriveskbd") ||
             istreq(میدان_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", میدان_2);
    }
    ورنہ اگر (istreq(میدان_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ورنہ {
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                میدان_2, LEDText(info, ledi));
        واپس !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    واپس true;
}

ساکن bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *ndx;
    bool ret;

    اگر (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2, &ndx)) {
        ret = false;
    }
    ورنہ اگر (elem && istreq(elem, "interpret")) {
        SymInterpInfo عارضی = {0};
        InitInterp(&عارضی);
        /* Do not replace the whole interpret, only the current field */
        عارضی.ملاؤ = (عارضی.ملاؤ == ملاؤ_REPLACE)
            ? ملاؤ_OVERRIDE
            : stmt->ملاؤ;
        ret = سیٹ_Interp_میدان(info, &عارضی, میدان_2, ndx, stmt->قدر);
        اگر (ret)
            ملاؤ_Interp(info, &info->default_interp, &عارضی, true);
    }
    ورنہ اگر (elem && istreq(elem, "indicator")) {
        LedInfo عارضی = {0};
        InitLED(&عارضی);
        /* Do not replace the whole LED, only the current field */
        عارضی.ملاؤ = (عارضی.ملاؤ == ملاؤ_REPLACE)
            ? ملاؤ_OVERRIDE
            : stmt->ملاؤ;
        ret = سیٹ_Led_Map_میدان(info, &عارضی, میدان_2, ndx, &stmt->قدر);
        اگر (ret)
            ملاؤ_Led_Map(info, &info->default_led, &عارضی, true);
    }
    ورنہ اگر (elem) {
        ret = (سیٹ_Default_Action_میدان(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, میدان_2, ndx,
                                     &stmt->قدر, stmt->ملاؤ) !=
               PARSER_FATAL_ERROR);
    }
    ورنہ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        واپس !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    واپس ret;
}

ساکن bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    برائے (; def; def = (VarDef *) def->common.اگلا) {
        اگر (!اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &میدان_2, &arrayNdx)) {
            ok = false;
            جاری;
        }
        اگر (elem) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, میدان_2);
            ok = false;
            جاری;
        }
        اگر (!سیٹ_Interp_میدان(info, si, میدان_2, arrayNdx, def->قدر))
            ok = false;
    }

    واپس ok;
}

ساکن bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    شمار xkb_مطابقت_operation pred;
    xkb_mod_نقاب_t mods;
    SymInterpInfo si;

    اگر (!ResolveStateAndPredicate(def->مطابقت, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        واپس false;
    }

    si = info->default_interp;
    si.ملاؤ = def->ملاؤ;
    si.interp.sym = def->sym;
    si.interp.مطابقت = pred;
    si.interp.mods = mods;

    اگر (!HandleInterpBody(info, def->def, &si)) {
        info->error_گنتی++;
        واپس false;
    }

    اگر (!جوڑو_Interp(info, &si, true)) {
        info->error_گنتی++;
        واپس false;
    }

    واپس true;
}

ساکن bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.ملاؤ = def->ملاؤ;
    ledi.led.name = def->name;

    bool ok = true;
    برائے (VarDef *var = def->body; var != تہی; var = (VarDef *) var->common.اگلا) {
        ثابت حرف *elem, *میدان_2;
        اظہاریہ_Def *arrayNdx;
        اگر (!اظہاریہ_Resolve_Lhs(info->ctx, var->name, &elem, &میدان_2, &arrayNdx)) {
            ok = false;
            جاری;
        }

        اگر (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, میدان_2);
            ok = false;
        }
        ورنہ {
            اگر (!سیٹ_Led_Map_میدان(info, &ledi, میدان_2, arrayNdx, &var->قدر))
                ok = false;
        }
    }

    واپس ok && جوڑو_Led_Map(info, &ledi, true);
}

ساکن خالی
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_محفوظ(file->name);

    برائے (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            توڑو;
        حالت STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            توڑو;
        حالت STMT_گروہ_COMPAT:
            log_dbg(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            توڑو;
        حالت STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            توڑو;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            توڑو;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            توڑو;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            توڑو;
        طےشدہ:
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            توڑو;
        }

        اگر (!ok)
            info->error_گنتی++;

        اگر (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    محفوظ_map_name(file->name));
            توڑو;
        }
    }
}

/* Temporary struct for CopyInterps. */
ساخت collect {
    darray(ساخت xkb_sym_interpret) sym_interprets;
};

ساکن خالی
CopyInterps(CompatInfo *info, bool need_علامت, شمار xkb_مطابقت_operation pred,
            ساخت collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        اگر (si->interp.مطابقت == pred &&
            (si->interp.sym != XKB_کلید_No_علامت) == need_علامت)
            darray_append(collect->sym_interprets, si->interp);
}

ساکن خالی
Copy_Led_Map_Defs_مقصد_Keymap(ساخت xkb_keymap *keymap, CompatInfo *info)
{
    برائے (xkb_led_اشاریہ_t idx = 0; idx < info->عدد_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_اشاریہ_t i;
        ساخت xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            اگر (led->name == ledi->led.name)
                توڑو;

        /* Not previously declared; create it with next free index. */
        اگر (i >= keymap->عدد_leds) {
            log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                اگر (led->name == XKB_ATOM_NONE)
                    توڑو;

            اگر (i >= keymap->عدد_leds) {
                /* Not place to put it; ignore. */
                اگر (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    جاری;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->عدد_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        اگر (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        اگر (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ساکن bool
Copy_Compat_مقصد_Keymap(ساخت xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    اگر (!darray_خالی_خانے(info->interps)) {
        ساخت collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, مطابقت_EXACTLY, &collect);
        CopyInterps(info, true, مطابقت_ALL, &collect);
        CopyInterps(info, true, مطابقت_NONE, &collect);
        CopyInterps(info, true, مطابقت_ANY, &collect);
        CopyInterps(info, true, مطابقت_ANY_OR_NONE, &collect);
        CopyInterps(info, false, مطابقت_EXACTLY, &collect);
        CopyInterps(info, false, مطابقت_ALL, &collect);
        CopyInterps(info, false, مطابقت_NONE, &collect);
        CopyInterps(info, false, مطابقت_ANY, &collect);
        CopyInterps(info, false, مطابقت_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->عدد_sym_interprets);
    }

    Copy_Led_Map_Defs_مقصد_Keymap(keymap, info);

    واپس true;
}

bool
CompileCompatMap(XkbFile *file, ساخت xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    اگر (file != تہی)
        HandleCompatMapFile(&info, file);

    اگر (info.error_گنتی != 0)
        جاؤ err_info;

    اگر (!Copy_Compat_مقصد_Keymap(&keymap_info->keymap, &info))
        جاؤ err_info;

    ClearCompatInfo(&info);
    واپس true;

err_info:
    ClearCompatInfo(&info);
    واپس false;
}
