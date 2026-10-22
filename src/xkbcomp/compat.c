/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "action.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "vmod.h"
#अन्तर्भाव "util-mem.h"

गणना si_क्षेत्रम् {
    SI_क्षेत्रम्_VIRTUAL_MOD = (1 << 0),
    SI_क्षेत्रम्_ACTION = (1 << 1),
    SI_क्षेत्रम्_AUTO_REPEAT = (1 << 2),
    SI_क्षेत्रम्_LEVEL_ONE_ONLY = (1 << 3),
};

प्रकारनिर्देश संरचना {
    गणना si_क्षेत्रम् defined;
    गणना merge_बहुलकम् merge;

    संरचना xkb_sym_interpret interp;
} SymInterpInfo;

गणना led_क्षेत्रम् {
    LED_क्षेत्रम्_MODS = (1 << 0),
    LED_क्षेत्रम्_GROUPS = (1 << 1),
    LED_क्षेत्रम्_CTRLS = (1 << 2),
};

प्रकारनिर्देश संरचना {
    गणना led_क्षेत्रम् defined;
    गणना merge_बहुलकम् merge;

    संरचना xkb_led led;
} LedInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    अचिह्नित पूर्णाङ्क include_गभीरता;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_सूचकांक_t अङ्कः_leds;
    ActionsInfo default_actions;
    संरचना xkb_mod_समुच्चयः mods;

    ध्रुव संरचना xkb_keymap_info *keymap_info;
    संरचना xkb_context *ctx;
} CompatInfo;

स्थिर ध्रुव वर्ण *
siText(SymInterpInfo *si, CompatInfo *info)
{
    वर्ण *buf = xkb_context_get_बफर(info->ctx, 128);

    यदि (si == &info->default_interp)
        प्रत्यागम "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.मेलनम्),
             Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    प्रत्यागम buf;
}

स्थिर अन्तःस्थ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *क्षेत्रम्_2)
{
    प्रत्यागम ReportNotArray(info->ctx, "symbol interpretation", क्षेत्रम्_2,
                          siText(si, info));
}

स्थिर अन्तःस्थ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *क्षेत्रम्_2,
                ध्रुव वर्ण *wanted)
{
    प्रत्यागम ReportBadType(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                         "symbol interpretation", क्षेत्रम्_2,
                         siText(si, info), wanted);
}

स्थिर ध्रुव वर्ण*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    यदि (ledi == &info->default_led) {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) == रिक्त);
        प्रत्यागम "default";
    } अन्यथा {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) != रिक्त);
        प्रत्यागम xkb_atom_text(info->ctx, ledi->led.name);
    }
}

स्थिर अन्तःस्थ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *क्षेत्रम्_2,
                 ध्रुव वर्ण *wanted)
{
    प्रत्यागम ReportBadType(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                         "indicator map",
                         क्षेत्रम्_2, LEDText(info, ledi), wanted);
}

स्थिर अन्तःस्थ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *क्षेत्रम्_2)
{
    प्रत्यागम ReportNotArray(info->ctx, "indicator map", क्षेत्रम्_2,
                          LEDText(info, ledi));
}

स्थिर अन्तःस्थ शून्य
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /*अप्रयुक्तम्*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_कुञ्चिका_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_कुञ्चिका_REPEAT, "invalid initial value");
}

स्थिर अन्तःस्थ शून्य
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /*अप्रयुक्तम्*/
}

स्थिर शून्य
InitCompatInfo(CompatInfo *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
               अचिह्नित पूर्णाङ्क include_गभीरता, ध्रुव संरचना xkb_mod_समुच्चयः *mods)
{
    स्मृतिं_पूरय(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_गभीरता = include_गभीरता;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_गभीरता > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

स्थिर शून्य
ClearCompatInfo(CompatInfo *info)
{
    मुक्त(info->name);
    darray_free(info->interps);
}

स्थिर SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        यदि (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.मेलनम् == new->interp.मेलनम्)
            प्रत्यागम old;

    प्रत्यागम रिक्त;
}

स्थिर bool
Use_New_Interp_क्षेत्रम्(गणना si_क्षेत्रम् क्षेत्रम्_2, गणना si_क्षेत्रम् old, गणना si_क्षेत्रम् new,
                  bool clobber, bool report, गणना si_क्षेत्रम् *collide)
{
    यदि (!(old & क्षेत्रम्_2))
        प्रत्यागम (new & क्षेत्रम्_2);

    यदि (new & क्षेत्रम्_2) {
        यदि (report)
            *collide |= क्षेत्रम्_2;

        प्रत्यागम clobber;
    }

    प्रत्यागम false;
}

स्थिर bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;
    गणना si_क्षेत्रम् collide = 0;

    यदि (new->merge == MERGE_REPLACE) {
        यदि (report)
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        प्रत्यागम true;
    }

    यदि (Use_New_Interp_क्षेत्रम्(SI_क्षेत्रम्_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_क्षेत्रम्_VIRTUAL_MOD;
    }
    यदि (Use_New_Interp_क्षेत्रम्(SI_क्षेत्रम्_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        यदि (old->interp.अङ्कः_actions > 1) {
                मुक्त(old->interp.a.actions);
            }
            old->interp.अङ्कः_actions = new->interp.अङ्कः_actions;
            यदि (new->interp.अङ्कः_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (सङ्घ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.अङ्कः_actions = 0;
            } अन्यथा {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_क्षेत्रम्_ACTION;
    }
    यदि (Use_New_Interp_क्षेत्रम्(SI_क्षेत्रम्_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_क्षेत्रम्_AUTO_REPEAT;
    }
    यदि (Use_New_Interp_क्षेत्रम्(SI_क्षेत्रम्_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_क्षेत्रम्_LEVEL_ONE_ONLY;
    }

    यदि (collide) {
        log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    प्रत्यागम true;
}

स्थिर bool
सङ्कलय_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    यदि (old)
        प्रत्यागम MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    प्रत्यागम true;
}

/***====================================================================***/

// TODO: more lenient?
स्थिर bool
ResolveStateAndPredicate(व्यञ्जकम्_Def *व्यञ्जकम्, गणना xkb_मेलनम्_operation *pred_rtrn,
                         xkb_mod_आवरणम्_t *mods_rtrn, CompatInfo *info)
{
    यदि (व्यञ्जकम् == रिक्त) {
        *pred_rtrn = मेलनम्_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_आवरणम्_ALL;
        प्रत्यागम true;
    }

    *pred_rtrn = मेलनम्_EXACTLY;
    यदि (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_ACTION_DECL) {
        ध्रुव वर्ण *pred_txt = xkb_atom_text(info->ctx, व्यञ्जकम्->action.name);
        अचिह्नित पूर्णाङ्क pred = 0;
        यदि (!LookupString(sym_Interpret_मेलनम्_आवरणम्_नामानि, pred_txt, &pred) ||
            !व्यञ्जकम्->action.तर्काः || व्यञ्जकम्->action.तर्काः->common.अग्र) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            प्रत्यागम false;
        }
        *pred_rtrn = (गणना xkb_मेलनम्_operation) pred;
        व्यञ्जकम् = व्यञ्जकम्->action.तर्काः;
    }
    अन्यथा यदि (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_IDENT) {
        ध्रुव वर्ण *pred_txt = xkb_atom_text(info->ctx, व्यञ्जकम्->ident.ident);
        यदि (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = मेलनम्_ANY;
            *mods_rtrn = MOD_REAL_आवरणम्_ALL;
            प्रत्यागम true;
        }
    }

    प्रत्यागम व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, व्यञ्जकम्, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

स्थिर bool
UseNewLEDField(गणना led_क्षेत्रम् क्षेत्रम्_2, गणना led_क्षेत्रम् old, गणना led_क्षेत्रम् new,
               bool clobber, bool report, गणना led_क्षेत्रम् *collide)
{
    यदि (!(old & क्षेत्रम्_2))
        प्रत्यागम (new & क्षेत्रम्_2);

    यदि (new & क्षेत्रम्_2) {
        यदि (report)
            *collide |= क्षेत्रम्_2;

        प्रत्यागम clobber;
    }

    प्रत्यागम false;
}

स्थिर bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    गणना led_क्षेत्रम् collide;
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;

    यदि (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        प्रत्यागम true;
    }

    यदि (new->merge == MERGE_REPLACE) {
        यदि (report)
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        प्रत्यागम true;
    }

    collide = 0;
    यदि (UseNewLEDField(LED_क्षेत्रम्_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_क्षेत्रम्_MODS;
    }
    यदि (UseNewLEDField(LED_क्षेत्रम्_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_क्षेत्रम्_GROUPS;
    }
    यदि (UseNewLEDField(LED_क्षेत्रम्_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_क्षेत्रम्_CTRLS;
    }

    यदि (collide) {
        log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    प्रत्यागम true;
}

स्थिर bool
सङ्कलय_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    प्रति (xkb_led_सूचकांक_t i = 0; i < info->अङ्कः_leds; i++) {
        LedInfo *old = &info->leds[i];

        यदि (old->led.name != new->led.name)
            अनुवर्त;

        प्रत्यागम MergeLedMap(info, old, new, same_file);
    }

    यदि (info->अङ्कः_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        प्रत्यागम false;
    }
    info->leds[info->अङ्कः_leds++] = *new;
    प्रत्यागम true;
}

स्थिर शून्य
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        गणना merge_बहुलकम् merge)
{
    यदि (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        प्रत्यागम;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    यदि (darray_रिक्तानि(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    अन्यथा {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            यदि (!सङ्कलय_Interp(into, si, false))
                into->error_गणनम्++;
        }
    }

    यदि (into->अङ्कः_leds == 0) {
        स्मृतिं_प्रतिलिख(into->leds, from->leds, परिमाण(*from->leds) * from->अङ्कः_leds);
        into->अङ्कः_leds = from->अङ्कः_leds;
        from->अङ्कः_leds = 0;
    }
    अन्यथा {
        प्रति (xkb_led_सूचकांक_t i = 0; i < from->अङ्कः_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            यदि (!सङ्कलय_Led_Map(into, ledi, false))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    यदि (Exceeds_Include_Max_गभीरता(info->ctx, info->include_गभीरता)) {
        info->error_गणनम् += 10;
        प्रत्यागम false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_गभीरता + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        CompatInfo अग्र_incl;
        XkbFile *file;

        वर्ण मार्गः[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  मार्गः, परिमाण(मार्गः));
        यदि (!file) {
            info->error_गणनम् += 10;
            ClearCompatInfo(&included);
            प्रत्यागम false;
        }

        InitCompatInfo(&अग्र_incl, info->keymap_info, info->include_गभीरता + 1,
                       &included.mods);
        अग्र_incl.default_interp = info->default_interp;
        अग्र_incl.default_led = info->default_led;

        HandleCompatMapFile(&अग्र_incl, file);

        MergeIncludedCompatMaps(&included, &अग्र_incl, stmt->merge);

        ClearCompatInfo(&अग्र_incl);
        FreeXkbFile(file);
    }

    MergeIncludedCompatMaps(info, &included, include->merge);
    ClearCompatInfo(&included);

    प्रत्यागम (info->error_गणनम् == 0);
}

स्थिर bool
समुच्चयः_Interp_क्षेत्रम्(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *क्षेत्रम्_2,
               व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्)
{
    यदि (istreq(क्षेत्रम्_2, "action")) {
        यदि (arrayNdx)
            प्रत्यागम ReportSINotArray(info, si, क्षेत्रम्_2);

        यदि (मानम्->common.type == STMT_व्यञ्जकम्_ACTION_LIST) {
            अचिह्नित पूर्णाङ्क अङ्कः_actions = 0;
            प्रति (व्यञ्जकम्_Def *act = मानम्->actions.actions;
                 act; act = (व्यञ्जकम्_Def *) act->common.अग्र)
                 अङ्कः_actions++;

            यदि (अङ्कः_actions > MAX_ACTIONS_प्रत्येकम्_LEVEL) {
                log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_प्रत्येकम्_LEVEL, अङ्कः_actions);
                प्रत्यागम false;
            }

            si->interp.अङ्कः_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(सङ्घ xkb_action) actions = darray_new();
            प्रति (व्यञ्जकम्_Def *act = मानम्->actions.actions;
                 act; act = (व्यञ्जकम्_Def *) act->common.अग्र) {
                सङ्घ xkb_action गन्तव्यम्_Act = { 0 };
                विकल्प (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &गन्तव्यम्_Act)) {
                पक्ष PARSER_RECOVERABLE_ERROR:
                    गन्तव्यम्_Act.type = ACTION_TYPE_NONE;
                    भङ्ग;
                पक्ष PARSER_FATAL_ERROR:
                    darray_free(actions);
                    प्रत्यागम false;
                स्वभाव:
                    ;
                }
                यदि (गन्तव्यम्_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } अन्यथा यदि (likely(अङ्कः_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.अङ्कः_actions = 1;
                    si->interp.a.action = गन्तव्यम्_Act;
                } अन्यथा {
                    darray_append(actions, गन्तव्यम्_Act);
                }
            }
            विकल्प (darray_आकार(actions)) {
            पक्ष 0:
                /* No action or exactly one action: already processed */
                प्रतिज्ञा(si->interp.अङ्कः_actions <= 1);
                भङ्ग;
            पक्ष 1:
                /* One action: some actions were dropped */
                si->interp.अङ्कः_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                भङ्ग;
            स्वभाव:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.अङ्कः_actions =
                    (xkb_action_गणनम्_t) darray_आकार(actions);
                darray_steal(actions, &si->interp.a.actions, रिक्त);
            }
        }
        अन्यथा {
            विकल्प (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, मानम्, &si->interp.a.action)) {
            पक्ष PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.अङ्कः_actions = 0;
                भङ्ग;
            पक्ष PARSER_FATAL_ERROR:
                प्रत्यागम false;
            स्वभाव:
                si->interp.अङ्कः_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_क्षेत्रम्_ACTION;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "virtualmodifier") ||
             istreq(क्षेत्रम्_2, "virtualmod")) {
        यदि (arrayNdx)
            प्रत्यागम ReportSINotArray(info, si, क्षेत्रम्_2);

        xkb_mod_सूचकांक_t ndx = 0;
        यदि (!व्यञ्जकम्_Resolve_Mod(info->ctx, मानम्, MOD_VIRT, &info->mods, &ndx))
            प्रत्यागम ReportSIBadType(info, si, क्षेत्रम्_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_क्षेत्रम्_VIRTUAL_MOD;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "repeat")) {
        bool समुच्चयः = false;

        यदि (arrayNdx)
            प्रत्यागम ReportSINotArray(info, si, क्षेत्रम्_2);

        यदि (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &समुच्चयः))
            प्रत्यागम ReportSIBadType(info, si, क्षेत्रम्_2, "boolean");

        si->interp.repeat = समुच्चयः;

        si->defined |= SI_क्षेत्रम्_AUTO_REPEAT;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "usemodmap") ||
             istreq(क्षेत्रम्_2, "usemodmapmods")) {
        uint32_t val = 0;

        यदि (arrayNdx)
            प्रत्यागम ReportSINotArray(info, si, क्षेत्रम्_2);

        यदि (!व्यञ्जकम्_Resolve_Enum(info->ctx, मानम्, &val, use_Mod_Map_मानम्_नामानि))
            प्रत्यागम ReportSIBadType(info, si, क्षेत्रम्_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_क्षेत्रम्_LEVEL_ONE_ONLY;
    }
    अन्यथा {
        Report_Bad_क्षेत्रम्(info->ctx, "symbol interpretation", क्षेत्रम्_2,
                       siText(si, info));
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    प्रत्यागम true;
}

स्थिर bool
समुच्चयः_Led_Map_क्षेत्रम्(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *क्षेत्रम्_2,
               व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def **मानम्_ptr)
{
    व्यञ्जकम्_Def * सीमित ध्रुव मानम् = *मानम्_ptr;

    यदि (istreq(क्षेत्रम्_2, "modifiers") || istreq(क्षेत्रम्_2, "mods")) {
        यदि (arrayNdx)
            प्रत्यागम ReportLedNotArray(info, ledi, क्षेत्रम्_2);

        यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, मानम्, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            प्रत्यागम ReportLedBadType(info, ledi, क्षेत्रम्_2, "modifier mask");

        ledi->defined |= LED_क्षेत्रम्_MODS;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "groups")) {
        xkb_layout_आवरणम्_t आवरणम् = 0;

        यदि (arrayNdx)
            प्रत्यागम ReportLedNotArray(info, ledi, क्षेत्रम्_2);

        bool pending = false;
        यदि (!व्यञ्जकम्_Resolve_गणः_आवरणम्(info->keymap_info, मानम्, &आवरणम्, &pending)) {
            यदि (pending) {
                ledi->led.pending_groups = true;
                ध्रुव darray_आकार_t pending_सूचकांक =
                    darray_आकार(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (संरचना pending_computation) {
                        .व्यञ्जकम् = *मानम्_ptr,
                        .computed = false,
                        .मानम् = 0,
                    }
                );
                *मानम्_ptr = रिक्त;
                static_assert(परिमाण(pending_सूचकांक) == परिमाण(आवरणम्),
                              "Cannot save pending computation");
                आवरणम् = pending_सूचकांक;
            } अन्यथा {
                प्रत्यागम ReportLedBadType(info, ledi, क्षेत्रम्_2, "group mask");
            }
        } अन्यथा {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = आवरणम्;
        ledi->defined |= LED_क्षेत्रम्_GROUPS;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "controls") || istreq(क्षेत्रम्_2, "ctrls")) {
        uint32_t आवरणम् = 0;

        यदि (arrayNdx)
            प्रत्यागम ReportLedNotArray(info, ledi, क्षेत्रम्_2);

        ध्रुव uint8_t विस्थापनम् = info->keymap_info->features.controls_name_विस्थापनम्;
        यदि (!व्यञ्जकम्_Resolve_आवरणम्(info->ctx, मानम्, &आवरणम्, ctrl_आवरणम्_नामानि + विस्थापनम्))
            प्रत्यागम ReportLedBadType(info, ledi, क्षेत्रम्_2, "controls mask");

        ledi->led.ctrls = आवरणम्;
        ledi->defined |= LED_क्षेत्रम्_CTRLS;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "whichmodstate") ||
             istreq(क्षेत्रम्_2, "whichmodifierstate")) {
        uint32_t आवरणम् = 0;

        यदि (arrayNdx)
            प्रत्यागम ReportLedNotArray(info, ledi, क्षेत्रम्_2);

        यदि (!व्यञ्जकम्_Resolve_आवरणम्(info->ctx, मानम्, &आवरणम्,
                             mod_Component_आवरणम्_नामानि))
            प्रत्यागम ReportLedBadType(info, ledi, क्षेत्रम्_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = आवरणम्;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "whichgroupstate")) {
        uint32_t आवरणम् = 0;

        यदि (arrayNdx)
            प्रत्यागम ReportLedNotArray(info, ledi, क्षेत्रम्_2);

        यदि (!व्यञ्जकम्_Resolve_आवरणम्(info->ctx, मानम्, &आवरणम्,
                             गणः_Component_आवरणम्_नामानि))
            प्रत्यागम ReportLedBadType(info, ledi, क्षेत्रम्_2,
                                    "mask of group state components");

        ledi->led.which_groups = आवरणम्;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "driveskbd") ||
             istreq(क्षेत्रम्_2, "driveskeyboard") ||
             istreq(क्षेत्रम्_2, "leddriveskbd") ||
             istreq(क्षेत्रम्_2, "leddriveskeyboard") ||
             istreq(क्षेत्रम्_2, "indicatordriveskbd") ||
             istreq(क्षेत्रम्_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", क्षेत्रम्_2);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अन्यथा {
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                क्षेत्रम्_2, LEDText(info, ledi));
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    प्रत्यागम true;
}

स्थिर bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *ndx;
    bool ret;

    यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &क्षेत्रम्_2, &ndx)) {
        ret = false;
    }
    अन्यथा यदि (elem && istreq(elem, "interpret")) {
        SymInterpInfo अस्थायी = {0};
        InitInterp(&अस्थायी);
        /* Do not replace the whole interpret, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = समुच्चयः_Interp_क्षेत्रम्(info, &अस्थायी, क्षेत्रम्_2, ndx, stmt->मानम्);
        यदि (ret)
            MergeInterp(info, &info->default_interp, &अस्थायी, true);
    }
    अन्यथा यदि (elem && istreq(elem, "indicator")) {
        LedInfo अस्थायी = {0};
        InitLED(&अस्थायी);
        /* Do not replace the whole LED, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = समुच्चयः_Led_Map_क्षेत्रम्(info, &अस्थायी, क्षेत्रम्_2, ndx, &stmt->मानम्);
        यदि (ret)
            MergeLedMap(info, &info->default_led, &अस्थायी, true);
    }
    अन्यथा यदि (elem) {
        ret = (समुच्चयः_Default_Action_क्षेत्रम्(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, क्षेत्रम्_2, ndx,
                                     &stmt->मानम्, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_क्षेत्रम्,
                "Default defined for unknown field \"%s\"; Ignored\n", क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    प्रत्यागम ret;
}

स्थिर bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *arrayNdx;

    प्रति (; def; def = (VarDef *) def->common.अग्र) {
        यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &क्षेत्रम्_2, &arrayNdx)) {
            ok = false;
            अनुवर्त;
        }
        यदि (elem) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, क्षेत्रम्_2);
            ok = false;
            अनुवर्त;
        }
        यदि (!समुच्चयः_Interp_क्षेत्रम्(info, si, क्षेत्रम्_2, arrayNdx, def->मानम्))
            ok = false;
    }

    प्रत्यागम ok;
}

स्थिर bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    गणना xkb_मेलनम्_operation pred;
    xkb_mod_आवरणम्_t mods;
    SymInterpInfo si;

    यदि (!ResolveStateAndPredicate(def->मेलनम्, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        प्रत्यागम false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.मेलनम् = pred;
    si.interp.mods = mods;

    यदि (!HandleInterpBody(info, def->def, &si)) {
        info->error_गणनम्++;
        प्रत्यागम false;
    }

    यदि (!सङ्कलय_Interp(info, &si, true)) {
        info->error_गणनम्++;
        प्रत्यागम false;
    }

    प्रत्यागम true;
}

स्थिर bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    प्रति (VarDef *var = def->body; var != रिक्त; var = (VarDef *) var->common.अग्र) {
        ध्रुव वर्ण *elem, *क्षेत्रम्_2;
        व्यञ्जकम्_Def *arrayNdx;
        यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, var->name, &elem, &क्षेत्रम्_2, &arrayNdx)) {
            ok = false;
            अनुवर्त;
        }

        यदि (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, क्षेत्रम्_2);
            ok = false;
        }
        अन्यथा {
            यदि (!समुच्चयः_Led_Map_क्षेत्रम्(info, &ledi, क्षेत्रम्_2, arrayNdx, &var->मानम्))
                ok = false;
        }
    }

    प्रत्यागम ok && सङ्कलय_Led_Map(info, &ledi, true);
}

स्थिर शून्य
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            भङ्ग;
        पक्ष STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            भङ्ग;
        पक्ष STMT_गणः_COMPAT:
            log_dbg(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            भङ्ग;
        पक्ष STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            भङ्ग;
        पक्ष STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            भङ्ग;
        स्वभाव:
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_गन्तव्यम्_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        यदि (!ok)
            info->error_गणनम्++;

        यदि (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/* Temporary struct for CopyInterps. */
संरचना collect {
    darray(संरचना xkb_sym_interpret) sym_interprets;
};

स्थिर शून्य
CopyInterps(CompatInfo *info, bool need_चिह्नम्, गणना xkb_मेलनम्_operation pred,
            संरचना collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        यदि (si->interp.मेलनम् == pred &&
            (si->interp.sym != XKB_कुञ्चिका_No_चिह्नम्) == need_चिह्नम्)
            darray_append(collect->sym_interprets, si->interp);
}

स्थिर शून्य
Copy_Led_Map_Defs_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    प्रति (xkb_led_सूचकांक_t idx = 0; idx < info->अङ्कः_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_सूचकांक_t i;
        संरचना xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            यदि (led->name == ledi->led.name)
                भङ्ग;

        /* Not previously declared; create it with next free index. */
        यदि (i >= keymap->अङ्कः_leds) {
            log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                यदि (led->name == XKB_ATOM_NONE)
                    भङ्ग;

            यदि (i >= keymap->अङ्कः_leds) {
                /* Not place to put it; ignore. */
                यदि (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    अनुवर्त;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->अङ्कः_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        यदि (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        यदि (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

स्थिर bool
Copy_Compat_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    यदि (!darray_रिक्तानि(info->interps)) {
        संरचना collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, मेलनम्_EXACTLY, &collect);
        CopyInterps(info, true, मेलनम्_ALL, &collect);
        CopyInterps(info, true, मेलनम्_NONE, &collect);
        CopyInterps(info, true, मेलनम्_ANY, &collect);
        CopyInterps(info, true, मेलनम्_ANY_OR_NONE, &collect);
        CopyInterps(info, false, मेलनम्_EXACTLY, &collect);
        CopyInterps(info, false, मेलनम्_ALL, &collect);
        CopyInterps(info, false, मेलनम्_NONE, &collect);
        CopyInterps(info, false, मेलनम्_ANY, &collect);
        CopyInterps(info, false, मेलनम्_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->अङ्कः_sym_interprets);
    }

    Copy_Led_Map_Defs_गन्तव्यम्_Keymap(keymap, info);

    प्रत्यागम true;
}

bool
CompileCompatMap(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    यदि (file != रिक्त)
        HandleCompatMapFile(&info, file);

    यदि (info.error_गणनम् != 0)
        गच्छ err_info;

    यदि (!Copy_Compat_गन्तव्यम्_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearCompatInfo(&info);
    प्रत्यागम true;

err_info:
    ClearCompatInfo(&info);
    प्रत्यागम false;
}
