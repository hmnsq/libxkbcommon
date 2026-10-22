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

تعداد si_ميدان {
    SI_ميدان_VIRTUAL_MOD = (1 << 0),
    SI_ميدان_ACTION = (1 << 1),
    SI_ميدان_AUTO_REPEAT = (1 << 2),
    SI_ميدان_LEVEL_ONE_ONLY = (1 << 3),
};

تعريفنوع بنية {
    تعداد si_ميدان defined;
    تعداد ملايو_گهڻائي ملايو;

    بنية xkb_sym_interpret interp;
} SymInterpInfo;

تعداد led_ميدان {
    LED_ميدان_MODS = (1 << 0),
    LED_ميدان_GROUPS = (1 << 1),
    LED_ميدان_CTRLS = (1 << 2),
};

تعريفنوع بنية {
    تعداد led_ميدان defined;
    تعداد ملايو_گهڻائي ملايو;

    بنية xkb_led led;
} LedInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_ڳڻپ;
    بنانشان صحيح include_اونهائي;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_اشاريو_t انگ_leds;
    ActionsInfo default_actions;
    بنية xkb_mod_سيٽ mods;

    ثابت بنية xkb_keymap_info *keymap_info;
    بنية xkb_context *ctx;
} CompatInfo;

ساكن ثابت حرف *
si_متن(SymInterpInfo *si, CompatInfo *info)
{
    حرف *buf = xkb_context_get_بفر(info->ctx, 128);

    إذا (si == &info->default_interp)
        أرجع "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_متن(info->ctx, si->interp.sym),
             SIMatch_متن(si->interp.ميلاپ),
             Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    أرجع buf;
}

ساكن مضمن bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت حرف *ميدان_2)
{
    أرجع ReportNotArray(info->ctx, "symbol interpretation", ميدان_2,
                          si_متن(si, info));
}

ساكن مضمن bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت حرف *ميدان_2,
                ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                         "symbol interpretation", ميدان_2,
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
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت حرف *ميدان_2,
                 ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                         "indicator map",
                         ميدان_2, LEDText(info, ledi), wanted);
}

ساكن مضمن bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت حرف *ميدان_2)
{
    أرجع ReportNotArray(info->ctx, "indicator map", ميدان_2,
                          LEDText(info, ledi));
}

ساكن مضمن فراغ
InitInterp(SymInterpInfo *info)
{
    info->ملايو = ملايو_DEFAULT; /*اڻ_استعمال_ٿيل*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ڪنجي_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ڪنجي_REPEAT, "invalid initial value");
}

ساكن مضمن فراغ
InitLED(LedInfo *info)
{
    info->ملايو = ملايو_DEFAULT; /*اڻ_استعمال_ٿيل*/
}

ساكن فراغ
InitCompatInfo(CompatInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
               بنانشان صحيح include_اونهائي, ثابت بنية xkb_mod_سيٽ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_اونهائي = include_اونهائي;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_اونهائي > 0);
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
ڳوليو_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        إذا (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.ميلاپ == new->interp.ميلاپ)
            أرجع old;

    أرجع عدم;
}

ساكن bool
Use_New_Interp_ميدان(تعداد si_ميدان ميدان_2, تعداد si_ميدان old, تعداد si_ميدان new,
                  bool clobber, bool report, تعداد si_ميدان *collide)
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
ملايو_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->ملايو != ملايو_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    تعداد si_ميدان collide = 0;

    إذا (new->ملايو == ملايو_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_متن(new, info));
        *old = *new;
        أرجع true;
    }

    إذا (Use_New_Interp_ميدان(SI_ميدان_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_ميدان_VIRTUAL_MOD;
    }
    إذا (Use_New_Interp_ميدان(SI_ميدان_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        إذا (old->interp.انگ_actions > 1) {
                حرر(old->interp.a.actions);
            }
            old->interp.انگ_actions = new->interp.انگ_actions;
            إذا (new->interp.انگ_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.انگ_actions = 0;
            } وإلا {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_ميدان_ACTION;
    }
    إذا (Use_New_Interp_ميدان(SI_ميدان_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_ميدان_AUTO_REPEAT;
    }
    إذا (Use_New_Interp_ميدان(SI_ميدان_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_ميدان_LEVEL_ONE_ONLY;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_متن(old, info),
                 (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
جوڙيو_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = ڳوليو_Matching_Interp(info, new);
    إذا (old)
        أرجع ملايو_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    أرجع true;
}

/***====================================================================***/

// TODO: more lenient?
ساكن bool
ResolveStateAndPredicate(اظهار_Def *اظهار, تعداد xkb_ميلاپ_operation *pred_rtrn,
                         xkb_mod_نقاب_t *mods_rtrn, CompatInfo *info)
{
    إذا (اظهار == عدم) {
        *pred_rtrn = ميلاپ_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_نقاب_ALL;
        أرجع true;
    }

    *pred_rtrn = ميلاپ_EXACTLY;
    إذا (اظهار->common.type == STMT_اظهار_ACTION_DECL) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, اظهار->action.name);
        بنانشان صحيح pred = 0;
        إذا (!LookupString(sym_Interpret_ميلاپ_نقاب_نالا, pred_txt, &pred) ||
            !اظهار->action.دليلون || اظهار->action.دليلون->common.ايندڙ) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            أرجع false;
        }
        *pred_rtrn = (تعداد xkb_ميلاپ_operation) pred;
        اظهار = اظهار->action.دليلون;
    }
    وإلا إذا (اظهار->common.type == STMT_اظهار_IDENT) {
        ثابت حرف *pred_txt = xkb_atom_متن(info->ctx, اظهار->ident.ident);
        إذا (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = ميلاپ_ANY;
            *mods_rtrn = MOD_REAL_نقاب_ALL;
            أرجع true;
        }
    }

    أرجع اظهار_Resolve_Mod_نقاب(info->ctx, اظهار, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ساكن bool
UseNewLEDField(تعداد led_ميدان ميدان_2, تعداد led_ميدان old, تعداد led_ميدان new,
               bool clobber, bool report, تعداد led_ميدان *collide)
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
ملايو_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    تعداد led_ميدان collide;
    ثابت bool clobber = (new->ملايو != ملايو_AUGMENT);
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

    إذا (new->ملايو == ملايو_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        أرجع true;
    }

    collide = 0;
    إذا (UseNewLEDField(LED_ميدان_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_ميدان_MODS;
    }
    إذا (UseNewLEDField(LED_ميدان_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_ميدان_GROUPS;
    }
    إذا (UseNewLEDField(LED_ميدان_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_ميدان_CTRLS;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
جوڙيو_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    لكل (xkb_led_اشاريو_t i = 0; i < info->انگ_leds; i++) {
        LedInfo *old = &info->leds[i];

        إذا (old->led.name != new->led.name)
            استمر;

        أرجع ملايو_Led_Map(info, old, new, same_file);
    }

    إذا (info->انگ_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        أرجع false;
    }
    info->leds[info->انگ_leds++] = *new;
    أرجع true;
}

ساكن فراغ
ملايو_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        تعداد ملايو_گهڻائي ملايو)
{
    إذا (from->error_ڳڻپ > 0) {
        into->error_ڳڻپ += from->error_ڳڻپ;
        أرجع;
    }

    ملايو_Mod_Sets(into->ctx, &into->mods, &from->mods, ملايو);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_خالي_جايون(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    وإلا {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->ملايو = ملايو;
            إذا (!جوڙيو_Interp(into, si, false))
                into->error_ڳڻپ++;
        }
    }

    إذا (into->انگ_leds == 0) {
        يادگيري_نقل(into->leds, from->leds, حجم(*from->leds) * from->انگ_leds);
        into->انگ_leds = from->انگ_leds;
        from->انگ_leds = 0;
    }
    وإلا {
        لكل (xkb_led_اشاريو_t i = 0; i < from->انگ_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->ملايو = ملايو;
            إذا (!جوڙيو_Led_Map(into, ledi, false))
                into->error_ڳڻپ++;
        }
    }
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ساكن bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    إذا (Exceeds_Include_Max_اونهائي(info->ctx, info->include_اونهائي)) {
        info->error_ڳڻپ += 10;
        أرجع false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_اونهائي + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->ايندڙ_incl) {
        CompatInfo ايندڙ_incl;
        XkbFile *file;

        حرف رستو[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  رستو, حجم(رستو));
        إذا (!file) {
            info->error_ڳڻپ += 10;
            ClearCompatInfo(&included);
            أرجع false;
        }

        InitCompatInfo(&ايندڙ_incl, info->keymap_info, info->include_اونهائي + 1,
                       &included.mods);
        ايندڙ_incl.default_interp = info->default_interp;
        ايندڙ_incl.default_led = info->default_led;

        HandleCompatMapFile(&ايندڙ_incl, file);

        ملايو_Included_Compat_Maps(&included, &ايندڙ_incl, stmt->ملايو);

        ClearCompatInfo(&ايندڙ_incl);
        FreeXkbFile(file);
    }

    ملايو_Included_Compat_Maps(info, &included, include->ملايو);
    ClearCompatInfo(&included);

    أرجع (info->error_ڳڻپ == 0);
}

ساكن bool
سيٽ_Interp_ميدان(CompatInfo *info, SymInterpInfo *si, ثابت حرف *ميدان_2,
               اظهار_Def *arrayNdx, اظهار_Def *قدر)
{
    إذا (istreq(ميدان_2, "action")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, ميدان_2);

        إذا (قدر->common.type == STMT_اظهار_ACTION_LIST) {
            بنانشان صحيح انگ_actions = 0;
            لكل (اظهار_Def *act = قدر->actions.actions;
                 act; act = (اظهار_Def *) act->common.ايندڙ)
                 انگ_actions++;

            إذا (انگ_actions > MAX_ACTIONS_في_LEVEL) {
                log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_متن(si, info), MAX_ACTIONS_في_LEVEL, انگ_actions);
                أرجع false;
            }

            si->interp.انگ_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اتحاد xkb_action) actions = darray_new();
            لكل (اظهار_Def *act = قدر->actions.actions;
                 act; act = (اظهار_Def *) act->common.ايندڙ) {
                اتحاد xkb_action منزل_Act = { 0 };
                بدل (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &منزل_Act)) {
                حالة PARSER_RECOVERABLE_ERROR:
                    منزل_Act.type = ACTION_TYPE_NONE;
                    اكسر;
                حالة PARSER_FATAL_ERROR:
                    darray_free(actions);
                    أرجع false;
                افتراضي:
                    ;
                }
                إذا (منزل_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } وإلا إذا (likely(انگ_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.انگ_actions = 1;
                    si->interp.a.action = منزل_Act;
                } وإلا {
                    darray_append(actions, منزل_Act);
                }
            }
            بدل (darray_سائيز(actions)) {
            حالة 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.انگ_actions <= 1);
                اكسر;
            حالة 1:
                /* One action: some actions were dropped */
                si->interp.انگ_actions = 1;
                si->interp.a.action = darray_شي(actions, 1);
                darray_free(actions);
                اكسر;
            افتراضي:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.انگ_actions =
                    (xkb_action_ڳڻپ_t) darray_سائيز(actions);
                darray_steal(actions, &si->interp.a.actions, عدم);
            }
        }
        وإلا {
            بدل (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, قدر, &si->interp.a.action)) {
            حالة PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.انگ_actions = 0;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                أرجع false;
            افتراضي:
                si->interp.انگ_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_ميدان_ACTION;
    }
    وإلا إذا (istreq(ميدان_2, "virtualmodifier") ||
             istreq(ميدان_2, "virtualmod")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, ميدان_2);

        xkb_mod_اشاريو_t ndx = 0;
        إذا (!اظهار_Resolve_Mod(info->ctx, قدر, MOD_VIRT, &info->mods, &ndx))
            أرجع ReportSIBadType(info, si, ميدان_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_ميدان_VIRTUAL_MOD;
    }
    وإلا إذا (istreq(ميدان_2, "repeat")) {
        bool سيٽ = false;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, ميدان_2);

        إذا (!اظهار_Resolve_Boolean(info->ctx, قدر, &سيٽ))
            أرجع ReportSIBadType(info, si, ميدان_2, "boolean");

        si->interp.repeat = سيٽ;

        si->defined |= SI_ميدان_AUTO_REPEAT;
    }
    وإلا إذا (istreq(ميدان_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_پيغام_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(ميدان_2, "usemodmap") ||
             istreq(ميدان_2, "usemodmapmods")) {
        uint32_t val = 0;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, ميدان_2);

        إذا (!اظهار_Resolve_Enum(info->ctx, قدر, &val, use_Mod_Map_قدر_نالا))
            أرجع ReportSIBadType(info, si, ميدان_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_ميدان_LEVEL_ONE_ONLY;
    }
    وإلا {
        Report_Bad_ميدان(info->ctx, "symbol interpretation", ميدان_2,
                       si_متن(si, info));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    أرجع true;
}

ساكن bool
سيٽ_Led_Map_ميدان(CompatInfo *info, LedInfo *ledi, ثابت حرف *ميدان_2,
               اظهار_Def *arrayNdx, اظهار_Def **قدر_ptr)
{
    اظهار_Def * محدود ثابت قدر = *قدر_ptr;

    إذا (istreq(ميدان_2, "modifiers") || istreq(ميدان_2, "mods")) {
        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, ميدان_2);

        إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            أرجع ReportLedBadType(info, ledi, ميدان_2, "modifier mask");

        ledi->defined |= LED_ميدان_MODS;
    }
    وإلا إذا (istreq(ميدان_2, "groups")) {
        xkb_layout_نقاب_t نقاب = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, ميدان_2);

        bool pending = false;
        إذا (!اظهار_Resolve_گروهه_نقاب(info->keymap_info, قدر, &نقاب, &pending)) {
            إذا (pending) {
                ledi->led.pending_groups = true;
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
                static_assert(حجم(pending_اشاريو) == حجم(نقاب),
                              "Cannot save pending computation");
                نقاب = pending_اشاريو;
            } وإلا {
                أرجع ReportLedBadType(info, ledi, ميدان_2, "group mask");
            }
        } وإلا {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = نقاب;
        ledi->defined |= LED_ميدان_GROUPS;
    }
    وإلا إذا (istreq(ميدان_2, "controls") || istreq(ميدان_2, "ctrls")) {
        uint32_t نقاب = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, ميدان_2);

        ثابت uint8_t وٿي = info->keymap_info->features.controls_name_وٿي;
        إذا (!اظهار_Resolve_نقاب(info->ctx, قدر, &نقاب, ctrl_نقاب_نالا + وٿي))
            أرجع ReportLedBadType(info, ledi, ميدان_2, "controls mask");

        ledi->led.ctrls = نقاب;
        ledi->defined |= LED_ميدان_CTRLS;
    }
    وإلا إذا (istreq(ميدان_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_پيغام_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(ميدان_2, "whichmodstate") ||
             istreq(ميدان_2, "whichmodifierstate")) {
        uint32_t نقاب = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, ميدان_2);

        إذا (!اظهار_Resolve_نقاب(info->ctx, قدر, &نقاب,
                             mod_Component_نقاب_نالا))
            أرجع ReportLedBadType(info, ledi, ميدان_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = نقاب;
    }
    وإلا إذا (istreq(ميدان_2, "whichgroupstate")) {
        uint32_t نقاب = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, ميدان_2);

        إذا (!اظهار_Resolve_نقاب(info->ctx, قدر, &نقاب,
                             گروهه_Component_نقاب_نالا))
            أرجع ReportLedBadType(info, ledi, ميدان_2,
                                    "mask of group state components");

        ledi->led.which_groups = نقاب;
    }
    وإلا إذا (istreq(ميدان_2, "driveskbd") ||
             istreq(ميدان_2, "driveskeyboard") ||
             istreq(ميدان_2, "leddriveskbd") ||
             istreq(ميدان_2, "leddriveskeyboard") ||
             istreq(ميدان_2, "indicatordriveskbd") ||
             istreq(ميدان_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_پيغام_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", ميدان_2);
    }
    وإلا إذا (istreq(ميدان_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا {
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                ميدان_2, LEDText(info, ledi));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    أرجع true;
}

ساكن bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *ndx;
    bool ret;

    إذا (!اظهار_Resolve_Lhs(info->ctx, stmt->name, &elem, &ميدان_2, &ndx)) {
        ret = false;
    }
    وإلا إذا (elem && istreq(elem, "interpret")) {
        SymInterpInfo عارضي = {0};
        InitInterp(&عارضي);
        /* Do not replace the whole interpret, only the current field */
        عارضي.ملايو = (عارضي.ملايو == ملايو_REPLACE)
            ? ملايو_OVERRIDE
            : stmt->ملايو;
        ret = سيٽ_Interp_ميدان(info, &عارضي, ميدان_2, ndx, stmt->قدر);
        إذا (ret)
            ملايو_Interp(info, &info->default_interp, &عارضي, true);
    }
    وإلا إذا (elem && istreq(elem, "indicator")) {
        LedInfo عارضي = {0};
        InitLED(&عارضي);
        /* Do not replace the whole LED, only the current field */
        عارضي.ملايو = (عارضي.ملايو == ملايو_REPLACE)
            ? ملايو_OVERRIDE
            : stmt->ملايو;
        ret = سيٽ_Led_Map_ميدان(info, &عارضي, ميدان_2, ndx, &stmt->قدر);
        إذا (ret)
            ملايو_Led_Map(info, &info->default_led, &عارضي, true);
    }
    وإلا إذا (elem) {
        ret = (سيٽ_Default_Action_ميدان(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ميدان_2, ndx,
                                     &stmt->قدر, stmt->ملايو) !=
               PARSER_FATAL_ERROR);
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ميدان,
                "Default defined for unknown field \"%s\"; Ignored\n", ميدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    أرجع ret;
}

ساكن bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.ايندڙ) {
        إذا (!اظهار_Resolve_Lhs(info->ctx, def->name, &elem, &ميدان_2, &arrayNdx)) {
            ok = false;
            استمر;
        }
        إذا (elem) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, ميدان_2);
            ok = false;
            استمر;
        }
        إذا (!سيٽ_Interp_ميدان(info, si, ميدان_2, arrayNdx, def->قدر))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    تعداد xkb_ميلاپ_operation pred;
    xkb_mod_نقاب_t mods;
    SymInterpInfo si;

    إذا (!ResolveStateAndPredicate(def->ميلاپ, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        أرجع false;
    }

    si = info->default_interp;
    si.ملايو = def->ملايو;
    si.interp.sym = def->sym;
    si.interp.ميلاپ = pred;
    si.interp.mods = mods;

    إذا (!HandleInterpBody(info, def->def, &si)) {
        info->error_ڳڻپ++;
        أرجع false;
    }

    إذا (!جوڙيو_Interp(info, &si, true)) {
        info->error_ڳڻپ++;
        أرجع false;
    }

    أرجع true;
}

ساكن bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.ملايو = def->ملايو;
    ledi.led.name = def->name;

    bool ok = true;
    لكل (VarDef *var = def->body; var != عدم; var = (VarDef *) var->common.ايندڙ) {
        ثابت حرف *elem, *ميدان_2;
        اظهار_Def *arrayNdx;
        إذا (!اظهار_Resolve_Lhs(info->ctx, var->name, &elem, &ميدان_2, &arrayNdx)) {
            ok = false;
            استمر;
        }

        إذا (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, ميدان_2);
            ok = false;
        }
        وإلا {
            إذا (!سيٽ_Led_Map_ميدان(info, &ledi, ميدان_2, arrayNdx, &var->قدر))
                ok = false;
        }
    }

    أرجع ok && جوڙيو_Led_Map(info, &ledi, true);
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ايندڙ) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            اكسر;
        حالة STMT_گروهه_COMPAT:
            log_dbg(info->ctx, XKB_LOG_پيغام_NO_ID,
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
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_ڳڻپ++;

        إذا (info->error_ڳڻپ > 10) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    محفوظ_map_name(file->name));
            اكسر;
        }
    }
}

/* Temporary struct for CopyInterps. */
بنية collect {
    darray(بنية xkb_sym_interpret) sym_interprets;
};

ساكن فراغ
CopyInterps(CompatInfo *info, bool need_نشان, تعداد xkb_ميلاپ_operation pred,
            بنية collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        إذا (si->interp.ميلاپ == pred &&
            (si->interp.sym != XKB_ڪنجي_No_نشان) == need_نشان)
            darray_append(collect->sym_interprets, si->interp);
}

ساكن فراغ
Copy_Led_Map_Defs_منزل_Keymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    لكل (xkb_led_اشاريو_t idx = 0; idx < info->انگ_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_اشاريو_t i;
        بنية xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            إذا (led->name == ledi->led.name)
                اكسر;

        /* Not previously declared; create it with next free index. */
        إذا (i >= keymap->انگ_leds) {
            log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                إذا (led->name == XKB_ATOM_NONE)
                    اكسر;

            إذا (i >= keymap->انگ_leds) {
                /* Not place to put it; ignore. */
                إذا (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    استمر;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->انگ_leds++];
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
Copy_Compat_منزل_Keymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    إذا (!darray_خالي_جايون(info->interps)) {
        بنية collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, ميلاپ_EXACTLY, &collect);
        CopyInterps(info, true, ميلاپ_ALL, &collect);
        CopyInterps(info, true, ميلاپ_NONE, &collect);
        CopyInterps(info, true, ميلاپ_ANY, &collect);
        CopyInterps(info, true, ميلاپ_ANY_OR_NONE, &collect);
        CopyInterps(info, false, ميلاپ_EXACTLY, &collect);
        CopyInterps(info, false, ميلاپ_ALL, &collect);
        CopyInterps(info, false, ميلاپ_NONE, &collect);
        CopyInterps(info, false, ميلاپ_ANY, &collect);
        CopyInterps(info, false, ميلاپ_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->انگ_sym_interprets);
    }

    Copy_Led_Map_Defs_منزل_Keymap(keymap, info);

    أرجع true;
}

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    إذا (file != عدم)
        HandleCompatMapFile(&info, file);

    إذا (info.error_ڳڻپ != 0)
        اذهب err_info;

    إذا (!Copy_Compat_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearCompatInfo(&info);
    أرجع true;

err_info:
    ClearCompatInfo(&info);
    أرجع false;
}
