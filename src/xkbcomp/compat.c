/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "ast.h"
#అన్తర్భావ "action.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "vmod.h"
#అన్తర్భావ "util-mem.h"

గణన si_క్షేత్రం {
    SI_క్షేత్రం_VIRTUAL_MOD = (1 << 0),
    SI_క్షేత్రం_ACTION = (1 << 1),
    SI_క్షేత్రం_AUTO_REPEAT = (1 << 2),
    SI_క్షేత్రం_LEVEL_ONE_ONLY = (1 << 3),
};

రకనిర్వచనం నిర్మాణం {
    గణన si_క్షేత్రం defined;
    గణన విలీనం_బహుళకం విలీనం;

    నిర్మాణం xkb_sym_interpret interp;
} SymInterpInfo;

గణన led_క్షేత్రం {
    LED_క్షేత్రం_MODS = (1 << 0),
    LED_క్షేత్రం_GROUPS = (1 << 1),
    LED_క్షేత్రం_CTRLS = (1 << 2),
};

రకనిర్వచనం నిర్మాణం {
    గణన led_క్షేత్రం defined;
    గణన విలీనం_బహుళకం విలీనం;

    నిర్మాణం xkb_led led;
} LedInfo;

రకనిర్వచనం నిర్మాణం {
    అక్షరం *name;
    పూర్ణాంకం error_లెక్క;
    గుర్తులేని పూర్ణాంకం include_లోతు;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_సూచీ_t అంకె_leds;
    ActionsInfo default_actions;
    నిర్మాణం xkb_mod_సమితి mods;

    స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info;
    నిర్మాణం xkb_context *ctx;
} CompatInfo;

స్థిర స్థిరరాశి అక్షరం *
si_పాఠ్యం(SymInterpInfo *si, CompatInfo *info)
{
    అక్షరం *buf = xkb_context_get_బఫరు(info->ctx, 128);

    ఒకవేళ (si == &info->default_interp)
        తిరిగి "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_పాఠ్యం(info->ctx, si->interp.sym),
             SIMatch_పాఠ్యం(si->interp.సరిపోలిక),
             Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    తిరిగి buf;
}

స్థిర అంతఃవరుస bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, స్థిరరాశి అక్షరం *క్షేత్రం_2)
{
    తిరిగి ReportNotArray(info->ctx, "symbol interpretation", క్షేత్రం_2,
                          si_పాఠ్యం(si, info));
}

స్థిర అంతఃవరుస bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, స్థిరరాశి అక్షరం *క్షేత్రం_2,
                స్థిరరాశి అక్షరం *wanted)
{
    తిరిగి ReportBadType(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                         "symbol interpretation", క్షేత్రం_2,
                         si_పాఠ్యం(si, info), wanted);
}

స్థిర స్థిరరాశి అక్షరం*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ఒకవేళ (ledi == &info->default_led) {
        ప్రతిజ్ఞా(xkb_atom_పాఠ్యం(info->ctx, ledi->led.name) == ఖాళీ);
        తిరిగి "default";
    } లేకపోతే {
        ప్రతిజ్ఞా(xkb_atom_పాఠ్యం(info->ctx, ledi->led.name) != ఖాళీ);
        తిరిగి xkb_atom_పాఠ్యం(info->ctx, ledi->led.name);
    }
}

స్థిర అంతఃవరుస bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, స్థిరరాశి అక్షరం *క్షేత్రం_2,
                 స్థిరరాశి అక్షరం *wanted)
{
    తిరిగి ReportBadType(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                         "indicator map",
                         క్షేత్రం_2, LEDText(info, ledi), wanted);
}

స్థిర అంతఃవరుస bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, స్థిరరాశి అక్షరం *క్షేత్రం_2)
{
    తిరిగి ReportNotArray(info->ctx, "indicator map", క్షేత్రం_2,
                          LEDText(info, ledi));
}

స్థిర అంతఃవరుస శూన్య
InitInterp(SymInterpInfo *info)
{
    info->విలీనం = విలీనం_DEFAULT; /*వాడని*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_కీలకం_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_కీలకం_REPEAT, "invalid initial value");
}

స్థిర అంతఃవరుస శూన్య
InitLED(LedInfo *info)
{
    info->విలీనం = విలీనం_DEFAULT; /*వాడని*/
}

స్థిర శూన్య
InitCompatInfo(CompatInfo *info, స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
               గుర్తులేని పూర్ణాంకం include_లోతు, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods)
{
    memset(info, 0, పరిమాణం(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_లోతు = include_లోతు;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_లోతు > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

స్థిర శూన్య
ClearCompatInfo(CompatInfo *info)
{
    విడుదల(info->name);
    darray_free(info->interps);
}

స్థిర SymInterpInfo *
కనుగొను_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ఒకవేళ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.సరిపోలిక == new->interp.సరిపోలిక)
            తిరిగి old;

    తిరిగి ఖాళీ;
}

స్థిర bool
Use_New_Interp_క్షేత్రం(గణన si_క్షేత్రం క్షేత్రం_2, గణన si_క్షేత్రం old, గణన si_క్షేత్రం new,
                  bool clobber, bool report, గణన si_క్షేత్రం *collide)
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
విలీనం_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    స్థిరరాశి bool clobber = (new->విలీనం != విలీనం_AUGMENT);
    స్థిరరాశి పూర్ణాంకం verbosity = xkb_context_get_log_verbosity(info->ctx);
    స్థిరరాశి bool report = (same_file && verbosity > 0) || verbosity > 9;
    గణన si_క్షేత్రం collide = 0;

    ఒకవేళ (new->విలీనం == విలీనం_REPLACE) {
        ఒకవేళ (report)
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_పాఠ్యం(new, info));
        *old = *new;
        తిరిగి true;
    }

    ఒకవేళ (Use_New_Interp_క్షేత్రం(SI_క్షేత్రం_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_క్షేత్రం_VIRTUAL_MOD;
    }
    ఒకవేళ (Use_New_Interp_క్షేత్రం(SI_క్షేత్రం_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ఒకవేళ (old->interp.అంకె_actions > 1) {
                విడుదల(old->interp.a.actions);
            }
            old->interp.అంకె_actions = new->interp.అంకె_actions;
            ఒకవేళ (new->interp.అంకె_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (సంఘం xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.అంకె_actions = 0;
            } లేకపోతే {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_క్షేత్రం_ACTION;
    }
    ఒకవేళ (Use_New_Interp_క్షేత్రం(SI_క్షేత్రం_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_క్షేత్రం_AUTO_REPEAT;
    }
    ఒకవేళ (Use_New_Interp_క్షేత్రం(SI_క్షేత్రం_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_క్షేత్రం_LEVEL_ONE_ONLY;
    }

    ఒకవేళ (collide) {
        log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_పాఠ్యం(old, info),
                 (clobber ? "last" : "first"));
    }

    తిరిగి true;
}

స్థిర bool
కూడు_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = కనుగొను_Matching_Interp(info, new);
    ఒకవేళ (old)
        తిరిగి విలీనం_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    తిరిగి true;
}

/***====================================================================***/

// TODO: more lenient?
స్థిర bool
ResolveStateAndPredicate(సమాసం_Def *సమాసం, గణన xkb_సరిపోలిక_operation *pred_rtrn,
                         xkb_mod_ముసుగు_t *mods_rtrn, CompatInfo *info)
{
    ఒకవేళ (సమాసం == ఖాళీ) {
        *pred_rtrn = సరిపోలిక_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_ముసుగు_ALL;
        తిరిగి true;
    }

    *pred_rtrn = సరిపోలిక_EXACTLY;
    ఒకవేళ (సమాసం->common.type == STMT_సమాసం_ACTION_DECL) {
        స్థిరరాశి అక్షరం *pred_txt = xkb_atom_పాఠ్యం(info->ctx, సమాసం->action.name);
        గుర్తులేని పూర్ణాంకం pred = 0;
        ఒకవేళ (!LookupString(sym_Interpret_సరిపోలిక_ముసుగు_పేర్లు, pred_txt, &pred) ||
            !సమాసం->action.వాదనలు || సమాసం->action.వాదనలు->common.తదుపరి) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            తిరిగి false;
        }
        *pred_rtrn = (గణన xkb_సరిపోలిక_operation) pred;
        సమాసం = సమాసం->action.వాదనలు;
    }
    లేకపోతే ఒకవేళ (సమాసం->common.type == STMT_సమాసం_IDENT) {
        స్థిరరాశి అక్షరం *pred_txt = xkb_atom_పాఠ్యం(info->ctx, సమాసం->ident.ident);
        ఒకవేళ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = సరిపోలిక_ANY;
            *mods_rtrn = MOD_REAL_ముసుగు_ALL;
            తిరిగి true;
        }
    }

    తిరిగి సమాసం_Resolve_Mod_ముసుగు(info->ctx, సమాసం, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

స్థిర bool
UseNewLEDField(గణన led_క్షేత్రం క్షేత్రం_2, గణన led_క్షేత్రం old, గణన led_క్షేత్రం new,
               bool clobber, bool report, గణన led_క్షేత్రం *collide)
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
విలీనం_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    గణన led_క్షేత్రం collide;
    స్థిరరాశి bool clobber = (new->విలీనం != విలీనం_AUGMENT);
    స్థిరరాశి పూర్ణాంకం verbosity = xkb_context_get_log_verbosity(info->ctx);
    స్థిరరాశి bool report = (same_file && verbosity > 0) || verbosity > 9;

    ఒకవేళ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        తిరిగి true;
    }

    ఒకవేళ (new->విలీనం == విలీనం_REPLACE) {
        ఒకవేళ (report)
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        తిరిగి true;
    }

    collide = 0;
    ఒకవేళ (UseNewLEDField(LED_క్షేత్రం_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_క్షేత్రం_MODS;
    }
    ఒకవేళ (UseNewLEDField(LED_క్షేత్రం_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_క్షేత్రం_GROUPS;
    }
    ఒకవేళ (UseNewLEDField(LED_క్షేత్రం_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_క్షేత్రం_CTRLS;
    }

    ఒకవేళ (collide) {
        log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    తిరిగి true;
}

స్థిర bool
కూడు_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    కొరకు (xkb_led_సూచీ_t i = 0; i < info->అంకె_leds; i++) {
        LedInfo *old = &info->leds[i];

        ఒకవేళ (old->led.name != new->led.name)
            కొనసాగు;

        తిరిగి విలీనం_Led_Map(info, old, new, same_file);
    }

    ఒకవేళ (info->అంకె_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        తిరిగి false;
    }
    info->leds[info->అంకె_leds++] = *new;
    తిరిగి true;
}

స్థిర శూన్య
విలీనం_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        గణన విలీనం_బహుళకం విలీనం)
{
    ఒకవేళ (from->error_లెక్క > 0) {
        into->error_లెక్క += from->error_లెక్క;
        తిరిగి;
    }

    విలీనం_Mod_Sets(into->ctx, &into->mods, &from->mods, విలీనం);

    ఒకవేళ (into->name == ఖాళీ) {
        into->name = steal(&from->name);
    }

    ఒకవేళ (darray_ఖాళీలు(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    లేకపోతే {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_Interp(into, si, false))
                into->error_లెక్క++;
        }
    }

    ఒకవేళ (into->అంకె_leds == 0) {
        జ్ఞాపకం_నకలు(into->leds, from->leds, పరిమాణం(*from->leds) * from->అంకె_leds);
        into->అంకె_leds = from->అంకె_leds;
        from->అంకె_leds = 0;
    }
    లేకపోతే {
        కొరకు (xkb_led_సూచీ_t i = 0; i < from->అంకె_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_Led_Map(into, ledi, false))
                into->error_లెక్క++;
        }
    }
}

స్థిర శూన్య
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

స్థిర bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ఒకవేళ (Exceeds_Include_Max_లోతు(info->ctx, info->include_లోతు)) {
        info->error_లెక్క += 10;
        తిరిగి false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_లోతు + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    కొరకు (IncludeStmt *stmt = include; stmt; stmt = stmt->తదుపరి_incl) {
        CompatInfo తదుపరి_incl;
        XkbFile *file;

        అక్షరం మార్గం[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  మార్గం, పరిమాణం(మార్గం));
        ఒకవేళ (!file) {
            info->error_లెక్క += 10;
            ClearCompatInfo(&included);
            తిరిగి false;
        }

        InitCompatInfo(&తదుపరి_incl, info->keymap_info, info->include_లోతు + 1,
                       &included.mods);
        తదుపరి_incl.default_interp = info->default_interp;
        తదుపరి_incl.default_led = info->default_led;

        HandleCompatMapFile(&తదుపరి_incl, file);

        విలీనం_Included_Compat_Maps(&included, &తదుపరి_incl, stmt->విలీనం);

        ClearCompatInfo(&తదుపరి_incl);
        FreeXkbFile(file);
    }

    విలీనం_Included_Compat_Maps(info, &included, include->విలీనం);
    ClearCompatInfo(&included);

    తిరిగి (info->error_లెక్క == 0);
}

స్థిర bool
సమితి_Interp_క్షేత్రం(CompatInfo *info, SymInterpInfo *si, స్థిరరాశి అక్షరం *క్షేత్రం_2,
               సమాసం_Def *arrayNdx, సమాసం_Def *విలువ)
{
    ఒకవేళ (istreq(క్షేత్రం_2, "action")) {
        ఒకవేళ (arrayNdx)
            తిరిగి ReportSINotArray(info, si, క్షేత్రం_2);

        ఒకవేళ (విలువ->common.type == STMT_సమాసం_ACTION_LIST) {
            గుర్తులేని పూర్ణాంకం అంకె_actions = 0;
            కొరకు (సమాసం_Def *act = విలువ->actions.actions;
                 act; act = (సమాసం_Def *) act->common.తదుపరి)
                 అంకె_actions++;

            ఒకవేళ (అంకె_actions > MAX_ACTIONS_చొప్పున_LEVEL) {
                log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_పాఠ్యం(si, info), MAX_ACTIONS_చొప్పున_LEVEL, అంకె_actions);
                తిరిగి false;
            }

            si->interp.అంకె_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(సంఘం xkb_action) actions = darray_new();
            కొరకు (సమాసం_Def *act = విలువ->actions.actions;
                 act; act = (సమాసం_Def *) act->common.తదుపరి) {
                సంఘం xkb_action గమ్యం_Act = { 0 };
                ఎంపిక (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &గమ్యం_Act)) {
                స్థితి PARSER_RECOVERABLE_ERROR:
                    గమ్యం_Act.type = ACTION_TYPE_NONE;
                    ఆపు;
                స్థితి PARSER_FATAL_ERROR:
                    darray_free(actions);
                    తిరిగి false;
                అప్రమేయ:
                    ;
                }
                ఒకవేళ (గమ్యం_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } లేకపోతే ఒకవేళ (likely(అంకె_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.అంకె_actions = 1;
                    si->interp.a.action = గమ్యం_Act;
                } లేకపోతే {
                    darray_append(actions, గమ్యం_Act);
                }
            }
            ఎంపిక (darray_కొలత(actions)) {
            స్థితి 0:
                /* No action or exactly one action: already processed */
                ప్రతిజ్ఞా(si->interp.అంకె_actions <= 1);
                ఆపు;
            స్థితి 1:
                /* One action: some actions were dropped */
                si->interp.అంకె_actions = 1;
                si->interp.a.action = darray_అంశం(actions, 1);
                darray_free(actions);
                ఆపు;
            అప్రమేయ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.అంకె_actions =
                    (xkb_action_లెక్క_t) darray_కొలత(actions);
                darray_steal(actions, &si->interp.a.actions, ఖాళీ);
            }
        }
        లేకపోతే {
            ఎంపిక (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, విలువ, &si->interp.a.action)) {
            స్థితి PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.అంకె_actions = 0;
                ఆపు;
            స్థితి PARSER_FATAL_ERROR:
                తిరిగి false;
            అప్రమేయ:
                si->interp.అంకె_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_క్షేత్రం_ACTION;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "virtualmodifier") ||
             istreq(క్షేత్రం_2, "virtualmod")) {
        ఒకవేళ (arrayNdx)
            తిరిగి ReportSINotArray(info, si, క్షేత్రం_2);

        xkb_mod_సూచీ_t ndx = 0;
        ఒకవేళ (!సమాసం_Resolve_Mod(info->ctx, విలువ, MOD_VIRT, &info->mods, &ndx))
            తిరిగి ReportSIBadType(info, si, క్షేత్రం_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_క్షేత్రం_VIRTUAL_MOD;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "repeat")) {
        bool సమితి = false;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportSINotArray(info, si, క్షేత్రం_2);

        ఒకవేళ (!సమాసం_Resolve_Boolean(info->ctx, విలువ, &సమితి))
            తిరిగి ReportSIBadType(info, si, క్షేత్రం_2, "boolean");

        si->interp.repeat = సమితి;

        si->defined |= SI_క్షేత్రం_AUTO_REPEAT;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "usemodmap") ||
             istreq(క్షేత్రం_2, "usemodmapmods")) {
        uint32_t val = 0;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportSINotArray(info, si, క్షేత్రం_2);

        ఒకవేళ (!సమాసం_Resolve_Enum(info->ctx, విలువ, &val, use_Mod_Map_విలువ_పేర్లు))
            తిరిగి ReportSIBadType(info, si, క్షేత్రం_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_క్షేత్రం_LEVEL_ONE_ONLY;
    }
    లేకపోతే {
        Report_Bad_క్షేత్రం(info->ctx, "symbol interpretation", క్షేత్రం_2,
                       si_పాఠ్యం(si, info));
        తిరిగి !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    తిరిగి true;
}

స్థిర bool
సమితి_Led_Map_క్షేత్రం(CompatInfo *info, LedInfo *ledi, స్థిరరాశి అక్షరం *క్షేత్రం_2,
               సమాసం_Def *arrayNdx, సమాసం_Def **విలువ_ptr)
{
    సమాసం_Def * పరిమిత స్థిరరాశి విలువ = *విలువ_ptr;

    ఒకవేళ (istreq(క్షేత్రం_2, "modifiers") || istreq(క్షేత్రం_2, "mods")) {
        ఒకవేళ (arrayNdx)
            తిరిగి ReportLedNotArray(info, ledi, క్షేత్రం_2);

        ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, విలువ, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            తిరిగి ReportLedBadType(info, ledi, క్షేత్రం_2, "modifier mask");

        ledi->defined |= LED_క్షేత్రం_MODS;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "groups")) {
        xkb_layout_ముసుగు_t ముసుగు = 0;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportLedNotArray(info, ledi, క్షేత్రం_2);

        bool pending = false;
        ఒకవేళ (!సమాసం_Resolve_సమూహం_ముసుగు(info->keymap_info, విలువ, &ముసుగు, &pending)) {
            ఒకవేళ (pending) {
                ledi->led.pending_groups = true;
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
                static_assert(పరిమాణం(pending_సూచీ) == పరిమాణం(ముసుగు),
                              "Cannot save pending computation");
                ముసుగు = pending_సూచీ;
            } లేకపోతే {
                తిరిగి ReportLedBadType(info, ledi, క్షేత్రం_2, "group mask");
            }
        } లేకపోతే {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = ముసుగు;
        ledi->defined |= LED_క్షేత్రం_GROUPS;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "controls") || istreq(క్షేత్రం_2, "ctrls")) {
        uint32_t ముసుగు = 0;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportLedNotArray(info, ledi, క్షేత్రం_2);

        స్థిరరాశి uint8_t స్థానభ్రంశం = info->keymap_info->features.controls_name_స్థానభ్రంశం;
        ఒకవేళ (!సమాసం_Resolve_ముసుగు(info->ctx, విలువ, &ముసుగు, ctrl_ముసుగు_పేర్లు + స్థానభ్రంశం))
            తిరిగి ReportLedBadType(info, ledi, క్షేత్రం_2, "controls mask");

        ledi->led.ctrls = ముసుగు;
        ledi->defined |= LED_క్షేత్రం_CTRLS;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "whichmodstate") ||
             istreq(క్షేత్రం_2, "whichmodifierstate")) {
        uint32_t ముసుగు = 0;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportLedNotArray(info, ledi, క్షేత్రం_2);

        ఒకవేళ (!సమాసం_Resolve_ముసుగు(info->ctx, విలువ, &ముసుగు,
                             mod_Component_ముసుగు_పేర్లు))
            తిరిగి ReportLedBadType(info, ledi, క్షేత్రం_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = ముసుగు;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "whichgroupstate")) {
        uint32_t ముసుగు = 0;

        ఒకవేళ (arrayNdx)
            తిరిగి ReportLedNotArray(info, ledi, క్షేత్రం_2);

        ఒకవేళ (!సమాసం_Resolve_ముసుగు(info->ctx, విలువ, &ముసుగు,
                             సమూహం_Component_ముసుగు_పేర్లు))
            తిరిగి ReportLedBadType(info, ledi, క్షేత్రం_2,
                                    "mask of group state components");

        ledi->led.which_groups = ముసుగు;
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "driveskbd") ||
             istreq(క్షేత్రం_2, "driveskeyboard") ||
             istreq(క్షేత్రం_2, "leddriveskbd") ||
             istreq(క్షేత్రం_2, "leddriveskeyboard") ||
             istreq(క్షేత్రం_2, "indicatordriveskbd") ||
             istreq(క్షేత్రం_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", క్షేత్రం_2);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    లేకపోతే {
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                క్షేత్రం_2, LEDText(info, ledi));
        తిరిగి !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    తిరిగి true;
}

స్థిర bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *ndx;
    bool ret;

    ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, stmt->name, &elem, &క్షేత్రం_2, &ndx)) {
        ret = false;
    }
    లేకపోతే ఒకవేళ (elem && istreq(elem, "interpret")) {
        SymInterpInfo తాత్కాలిక = {0};
        InitInterp(&తాత్కాలిక);
        /* Do not replace the whole interpret, only the current field */
        తాత్కాలిక.విలీనం = (తాత్కాలిక.విలీనం == విలీనం_REPLACE)
            ? విలీనం_OVERRIDE
            : stmt->విలీనం;
        ret = సమితి_Interp_క్షేత్రం(info, &తాత్కాలిక, క్షేత్రం_2, ndx, stmt->విలువ);
        ఒకవేళ (ret)
            విలీనం_Interp(info, &info->default_interp, &తాత్కాలిక, true);
    }
    లేకపోతే ఒకవేళ (elem && istreq(elem, "indicator")) {
        LedInfo తాత్కాలిక = {0};
        InitLED(&తాత్కాలిక);
        /* Do not replace the whole LED, only the current field */
        తాత్కాలిక.విలీనం = (తాత్కాలిక.విలీనం == విలీనం_REPLACE)
            ? విలీనం_OVERRIDE
            : stmt->విలీనం;
        ret = సమితి_Led_Map_క్షేత్రం(info, &తాత్కాలిక, క్షేత్రం_2, ndx, &stmt->విలువ);
        ఒకవేళ (ret)
            విలీనం_Led_Map(info, &info->default_led, &తాత్కాలిక, true);
    }
    లేకపోతే ఒకవేళ (elem) {
        ret = (సమితి_Default_Action_క్షేత్రం(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, క్షేత్రం_2, ndx,
                                     &stmt->విలువ, stmt->విలీనం) !=
               PARSER_FATAL_ERROR);
    }
    లేకపోతే {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_క్షేత్రం,
                "Default defined for unknown field \"%s\"; Ignored\n", క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    తిరిగి ret;
}

స్థిర bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *arrayNdx;

    కొరకు (; def; def = (VarDef *) def->common.తదుపరి) {
        ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, def->name, &elem, &క్షేత్రం_2, &arrayNdx)) {
            ok = false;
            కొనసాగు;
        }
        ఒకవేళ (elem) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, క్షేత్రం_2);
            ok = false;
            కొనసాగు;
        }
        ఒకవేళ (!సమితి_Interp_క్షేత్రం(info, si, క్షేత్రం_2, arrayNdx, def->విలువ))
            ok = false;
    }

    తిరిగి ok;
}

స్థిర bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    గణన xkb_సరిపోలిక_operation pred;
    xkb_mod_ముసుగు_t mods;
    SymInterpInfo si;

    ఒకవేళ (!ResolveStateAndPredicate(def->సరిపోలిక, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        తిరిగి false;
    }

    si = info->default_interp;
    si.విలీనం = def->విలీనం;
    si.interp.sym = def->sym;
    si.interp.సరిపోలిక = pred;
    si.interp.mods = mods;

    ఒకవేళ (!HandleInterpBody(info, def->def, &si)) {
        info->error_లెక్క++;
        తిరిగి false;
    }

    ఒకవేళ (!కూడు_Interp(info, &si, true)) {
        info->error_లెక్క++;
        తిరిగి false;
    }

    తిరిగి true;
}

స్థిర bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.విలీనం = def->విలీనం;
    ledi.led.name = def->name;

    bool ok = true;
    కొరకు (VarDef *var = def->body; var != ఖాళీ; var = (VarDef *) var->common.తదుపరి) {
        స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
        సమాసం_Def *arrayNdx;
        ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, var->name, &elem, &క్షేత్రం_2, &arrayNdx)) {
            ok = false;
            కొనసాగు;
        }

        ఒకవేళ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, క్షేత్రం_2);
            ok = false;
        }
        లేకపోతే {
            ఒకవేళ (!సమితి_Led_Map_క్షేత్రం(info, &ledi, క్షేత్రం_2, arrayNdx, &var->విలువ))
                ok = false;
        }
    }

    తిరిగి ok && కూడు_Led_Map(info, &ledi, true);
}

స్థిర శూన్య
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    విడుదల(info->name);
    info->name = strdup_సురక్షిత(file->name);

    కొరకు (ParseCommon *stmt = file->defs; stmt; stmt = stmt->తదుపరి) {
        ఎంపిక (stmt->type) {
        స్థితి STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ఆపు;
        స్థితి STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ఆపు;
        స్థితి STMT_సమూహం_COMPAT:
            log_dbg(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ఆపు;
        స్థితి STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ఆపు;
        స్థితి STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ఆపు;
        స్థితి STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ఆపు;
        స్థితి STMT_UNKNOWN_DECLARATION:
        స్థితి STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ఆపు;
        అప్రమేయ:
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_గమ్యం_string(stmt->type));
            ok = false;
            ఆపు;
        }

        ఒకవేళ (!ok)
            info->error_లెక్క++;

        ఒకవేళ (info->error_లెక్క > 10) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    సురక్షిత_map_name(file->name));
            ఆపు;
        }
    }
}

/* Temporary struct for CopyInterps. */
నిర్మాణం collect {
    darray(నిర్మాణం xkb_sym_interpret) sym_interprets;
};

స్థిర శూన్య
CopyInterps(CompatInfo *info, bool need_చిహ్నం, గణన xkb_సరిపోలిక_operation pred,
            నిర్మాణం collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ఒకవేళ (si->interp.సరిపోలిక == pred &&
            (si->interp.sym != XKB_కీలకం_No_చిహ్నం) == need_చిహ్నం)
            darray_append(collect->sym_interprets, si->interp);
}

స్థిర శూన్య
Copy_Led_Map_Defs_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, CompatInfo *info)
{
    కొరకు (xkb_led_సూచీ_t idx = 0; idx < info->అంకె_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_సూచీ_t i;
        నిర్మాణం xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ఒకవేళ (led->name == ledi->led.name)
                ఆపు;

        /* Not previously declared; create it with next free index. */
        ఒకవేళ (i >= keymap->అంకె_leds) {
            log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ఒకవేళ (led->name == XKB_ATOM_NONE)
                    ఆపు;

            ఒకవేళ (i >= keymap->అంకె_leds) {
                /* Not place to put it; ignore. */
                ఒకవేళ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    కొనసాగు;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->అంకె_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ఒకవేళ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ఒకవేళ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

స్థిర bool
Copy_Compat_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_సురక్షిత(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ఒకవేళ (!darray_ఖాళీలు(info->interps)) {
        నిర్మాణం collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, సరిపోలిక_EXACTLY, &collect);
        CopyInterps(info, true, సరిపోలిక_ALL, &collect);
        CopyInterps(info, true, సరిపోలిక_NONE, &collect);
        CopyInterps(info, true, సరిపోలిక_ANY, &collect);
        CopyInterps(info, true, సరిపోలిక_ANY_OR_NONE, &collect);
        CopyInterps(info, false, సరిపోలిక_EXACTLY, &collect);
        CopyInterps(info, false, సరిపోలిక_ALL, &collect);
        CopyInterps(info, false, సరిపోలిక_NONE, &collect);
        CopyInterps(info, false, సరిపోలిక_ANY, &collect);
        CopyInterps(info, false, సరిపోలిక_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->అంకె_sym_interprets);
    }

    Copy_Led_Map_Defs_గమ్యం_Keymap(keymap, info);

    తిరిగి true;
}

bool
CompileCompatMap(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ఒకవేళ (file != ఖాళీ)
        HandleCompatMapFile(&info, file);

    ఒకవేళ (info.error_లెక్క != 0)
        వెళ్ళు err_info;

    ఒకవేళ (!Copy_Compat_గమ్యం_Keymap(&keymap_info->keymap, &info))
        వెళ్ళు err_info;

    ClearCompatInfo(&info);
    తిరిగి true;

err_info:
    ClearCompatInfo(&info);
    తిరిగి false;
}
