/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "darray.h"
#समावेश "keymap.h"
#समावेश "xkbcomp-priv.h"
#समावेश "ast.h"
#समावेश "action.h"
#समावेश "expr.h"
#समावेश "include.h"
#समावेश "text.h"
#समावेश "vmod.h"
#समावेश "util-mem.h"

क्रमागत si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

प्रकार काष्ठा {
    क्रमागत si_field defined;
    क्रमागत merge_mode merge;

    काष्ठा xkb_sym_interpret interp;
} SymInterpInfo;

क्रमागत led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

प्रकार काष्ठा {
    क्रमागत led_field defined;
    क्रमागत merge_mode merge;

    काष्ठा xkb_led led;
} LedInfo;

प्रकार काष्ठा {
    अक्षर *name;
    पूर्णांक error_गिनती;
    अचिन्हित पूर्णांक include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_सूचकांक_t अंक_leds;
    ActionsInfo default_actions;
    काष्ठा xkb_mod_set mods;

    स्थिर काष्ठा xkb_keymap_info *keymap_info;
    काष्ठा xkb_context *ctx;
} CompatInfo;

अटल स्थिर अक्षर *
siText(SymInterpInfo *si, CompatInfo *info)
{
    अक्षर *buf = xkb_context_get_बफर(info->ctx, 128);

    अगर (si == &info->default_interp)
        वापस "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    वापस buf;
}

अटल अंतरभूत bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, स्थिर अक्षर *field)
{
    वापस ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

अटल अंतरभूत bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, स्थिर अक्षर *field,
                स्थिर अक्षर *wanted)
{
    वापस ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

अटल स्थिर अक्षर*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    अगर (ledi == &info->default_led) {
        निश्चित(xkb_atom_text(info->ctx, ledi->led.name) == शून्य);
        वापस "default";
    } अन्यथा {
        निश्चित(xkb_atom_text(info->ctx, ledi->led.name) != शून्य);
        वापस xkb_atom_text(info->ctx, ledi->led.name);
    }
}

अटल अंतरभूत bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, स्थिर अक्षर *field,
                 स्थिर अक्षर *wanted)
{
    वापस ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

अटल अंतरभूत bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, स्थिर अक्षर *field)
{
    वापस ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

अटल अंतरभूत व्योम
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_कुंजी_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_कुंजी_REPEAT, "invalid initial value");
}

अटल अंतरभूत व्योम
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

अटल व्योम
InitCompatInfo(CompatInfo *info, स्थिर काष्ठा xkb_keymap_info *keymap_info,
               अचिन्हित पूर्णांक include_depth, स्थिर काष्ठा xkb_mod_set *mods)
{
    स_रखो(info, 0, माप(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

अटल व्योम
ClearCompatInfo(CompatInfo *info)
{
    मुक्त(info->name);
    darray_free(info->interps);
}

अटल SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        अगर (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            वापस old;

    वापस शून्य;
}

अटल bool
UseNewInterpField(क्रमागत si_field field, क्रमागत si_field old, क्रमागत si_field new,
                  bool clobber, bool report, क्रमागत si_field *collide)
{
    अगर (!(old & field))
        वापस (new & field);

    अगर (new & field) {
        अगर (report)
            *collide |= field;

        वापस clobber;
    }

    वापस false;
}

अटल bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    स्थिर bool clobber = (new->merge != MERGE_AUGMENT);
    स्थिर पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिर bool report = (same_file && verbosity > 0) || verbosity > 9;
    क्रमागत si_field collide = 0;

    अगर (new->merge == MERGE_REPLACE) {
        अगर (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        वापस true;
    }

    अगर (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    अगर (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        अगर (old->interp.अंक_actions > 1) {
                मुक्त(old->interp.a.actions);
            }
            old->interp.अंक_actions = new->interp.अंक_actions;
            अगर (new->interp.अंक_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (जोड़ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.अंक_actions = 0;
            } अन्यथा {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    अगर (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    अगर (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    अगर (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    वापस true;
}

अटल bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    अगर (old)
        वापस MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    वापस true;
}

/***====================================================================***/

// TODO: more lenient?
अटल bool
ResolveStateAndPredicate(व्यंजक_Def *व्यंजक, क्रमागत xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    अगर (व्यंजक == शून्य) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        वापस true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    अगर (व्यंजक->common.type == STMT_व्यंजक_ACTION_DECL) {
        स्थिर अक्षर *pred_txt = xkb_atom_text(info->ctx, व्यंजक->action.name);
        अचिन्हित पूर्णांक pred = 0;
        अगर (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !व्यंजक->action.args || व्यंजक->action.args->common.अग्र) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            वापस false;
        }
        *pred_rtrn = (क्रमागत xkb_match_operation) pred;
        व्यंजक = व्यंजक->action.args;
    }
    अन्यथा अगर (व्यंजक->common.type == STMT_व्यंजक_IDENT) {
        स्थिर अक्षर *pred_txt = xkb_atom_text(info->ctx, व्यंजक->ident.ident);
        अगर (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            वापस true;
        }
    }

    वापस व्यंजक_Resolve_Mod_Mask(info->ctx, व्यंजक, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

अटल bool
UseNewLEDField(क्रमागत led_field field, क्रमागत led_field old, क्रमागत led_field new,
               bool clobber, bool report, क्रमागत led_field *collide)
{
    अगर (!(old & field))
        वापस (new & field);

    अगर (new & field) {
        अगर (report)
            *collide |= field;

        वापस clobber;
    }

    वापस false;
}

अटल bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    क्रमागत led_field collide;
    स्थिर bool clobber = (new->merge != MERGE_AUGMENT);
    स्थिर पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिर bool report = (same_file && verbosity > 0) || verbosity > 9;

    अगर (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        वापस true;
    }

    अगर (new->merge == MERGE_REPLACE) {
        अगर (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        वापस true;
    }

    collide = 0;
    अगर (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    अगर (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    अगर (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    अगर (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    वापस true;
}

अटल bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    क्रम (xkb_led_सूचकांक_t i = 0; i < info->अंक_leds; i++) {
        LedInfo *old = &info->leds[i];

        अगर (old->led.name != new->led.name)
            जारी;

        वापस MergeLedMap(info, old, new, same_file);
    }

    अगर (info->अंक_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        वापस false;
    }
    info->leds[info->अंक_leds++] = *new;
    वापस true;
}

अटल व्योम
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        क्रमागत merge_mode merge)
{
    अगर (from->error_गिनती > 0) {
        into->error_गिनती += from->error_गिनती;
        वापस;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    अगर (into->name == शून्य) {
        into->name = steal(&from->name);
    }

    अगर (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    अन्यथा {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            अगर (!AddInterp(into, si, false))
                into->error_गिनती++;
        }
    }

    अगर (into->अंक_leds == 0) {
        स_नकल(into->leds, from->leds, माप(*from->leds) * from->अंक_leds);
        into->अंक_leds = from->अंक_leds;
        from->अंक_leds = 0;
    }
    अन्यथा {
        क्रम (xkb_led_सूचकांक_t i = 0; i < from->अंक_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            अगर (!AddLedMap(into, ledi, false))
                into->error_गिनती++;
        }
    }
}

अटल व्योम
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

अटल bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    अगर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गिनती += 10;
        वापस false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    स्थिर काष्ठा parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    क्रम (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        CompatInfo अग्र_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, माप(path));
        अगर (!file) {
            info->error_गिनती += 10;
            ClearCompatInfo(&included);
            वापस false;
        }

        InitCompatInfo(&अग्र_incl, info->keymap_info, info->include_depth + 1,
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

    वापस (info->error_गिनती == 0);
}

अटल bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, स्थिर अक्षर *field,
               व्यंजक_Def *arrayNdx, व्यंजक_Def *मूल्य)
{
    अगर (istreq(field, "action")) {
        अगर (arrayNdx)
            वापस ReportSINotArray(info, si, field);

        अगर (मूल्य->common.type == STMT_व्यंजक_ACTION_LIST) {
            अचिन्हित पूर्णांक अंक_actions = 0;
            क्रम (व्यंजक_Def *act = मूल्य->actions.actions;
                 act; act = (व्यंजक_Def *) act->common.अग्र)
                 अंक_actions++;

            अगर (अंक_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, अंक_actions);
                वापस false;
            }

            si->interp.अंक_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(जोड़ xkb_action) actions = darray_new();
            क्रम (व्यंजक_Def *act = मूल्य->actions.actions;
                 act; act = (व्यंजक_Def *) act->common.अग्र) {
                जोड़ xkb_action toAct = { 0 };
                चयन (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                हाल PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    अवरोध;
                हाल PARSER_FATAL_ERROR:
                    darray_free(actions);
                    वापस false;
                शेष:
                    ;
                }
                अगर (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } अन्यथा अगर (likely(अंक_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.अंक_actions = 1;
                    si->interp.a.action = toAct;
                } अन्यथा {
                    darray_append(actions, toAct);
                }
            }
            चयन (darray_आकार(actions)) {
            हाल 0:
                /* No action or exactly one action: already processed */
                निश्चित(si->interp.अंक_actions <= 1);
                अवरोध;
            हाल 1:
                /* One action: some actions were dropped */
                si->interp.अंक_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                अवरोध;
            शेष:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.अंक_actions =
                    (xkb_action_गिनती_t) darray_आकार(actions);
                darray_steal(actions, &si->interp.a.actions, शून्य);
            }
        }
        अन्यथा {
            चयन (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, मूल्य, &si->interp.a.action)) {
            हाल PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.अंक_actions = 0;
                अवरोध;
            हाल PARSER_FATAL_ERROR:
                वापस false;
            शेष:
                si->interp.अंक_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    अन्यथा अगर (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        अगर (arrayNdx)
            वापस ReportSINotArray(info, si, field);

        xkb_mod_सूचकांक_t ndx = 0;
        अगर (!व्यंजक_Resolve_Mod(info->ctx, मूल्य, MOD_VIRT, &info->mods, &ndx))
            वापस ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    अन्यथा अगर (istreq(field, "repeat")) {
        bool set = false;

        अगर (arrayNdx)
            वापस ReportSINotArray(info, si, field);

        अगर (!व्यंजक_Resolve_Boolean(info->ctx, मूल्य, &set))
            वापस ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    अन्यथा अगर (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    अन्यथा अगर (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        अगर (arrayNdx)
            वापस ReportSINotArray(info, si, field);

        अगर (!व्यंजक_Resolve_Enum(info->ctx, मूल्य, &val, use_Mod_Map_मूल्य_Names))
            वापस ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    अन्यथा {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        वापस !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    वापस true;
}

अटल bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, स्थिर अक्षर *field,
               व्यंजक_Def *arrayNdx, व्यंजक_Def **मूल्य_ptr)
{
    व्यंजक_Def * restrict स्थिर मूल्य = *मूल्य_ptr;

    अगर (istreq(field, "modifiers") || istreq(field, "mods")) {
        अगर (arrayNdx)
            वापस ReportLedNotArray(info, ledi, field);

        अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            वापस ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    अन्यथा अगर (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        अगर (arrayNdx)
            वापस ReportLedNotArray(info, ledi, field);

        bool pending = false;
        अगर (!व्यंजक_Resolve_Group_Mask(info->keymap_info, मूल्य, &mask, &pending)) {
            अगर (pending) {
                ledi->led.pending_groups = true;
                स्थिर darray_आकार_t pending_सूचकांक =
                    darray_आकार(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (काष्ठा pending_computation) {
                        .व्यंजक = *मूल्य_ptr,
                        .computed = false,
                        .मूल्य = 0,
                    }
                );
                *मूल्य_ptr = शून्य;
                static_assert(माप(pending_सूचकांक) == माप(mask),
                              "Cannot save pending computation");
                mask = pending_सूचकांक;
            } अन्यथा {
                वापस ReportLedBadType(info, ledi, field, "group mask");
            }
        } अन्यथा {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    अन्यथा अगर (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        अगर (arrayNdx)
            वापस ReportLedNotArray(info, ledi, field);

        स्थिर uint8_t offset = info->keymap_info->features.controls_name_offset;
        अगर (!व्यंजक_Resolve_Mask(info->ctx, मूल्य, &mask, ctrlMaskNames + offset))
            वापस ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    अन्यथा अगर (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अन्यथा अगर (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        अगर (arrayNdx)
            वापस ReportLedNotArray(info, ledi, field);

        अगर (!व्यंजक_Resolve_Mask(info->ctx, मूल्य, &mask,
                             modComponentMaskNames))
            वापस ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    अन्यथा अगर (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        अगर (arrayNdx)
            वापस ReportLedNotArray(info, ledi, field);

        अगर (!व्यंजक_Resolve_Mask(info->ctx, मूल्य, &mask,
                             groupComponentMaskNames))
            वापस ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    अन्यथा अगर (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    अन्यथा अगर (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अन्यथा {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        वापस !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    वापस true;
}

अटल bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *ndx;
    bool ret;

    अगर (!व्यंजक_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    अन्यथा अगर (elem && istreq(elem, "interpret")) {
        SymInterpInfo अस्थायी = {0};
        InitInterp(&अस्थायी);
        /* Do not replace the whole interpret, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &अस्थायी, field, ndx, stmt->मूल्य);
        अगर (ret)
            MergeInterp(info, &info->default_interp, &अस्थायी, true);
    }
    अन्यथा अगर (elem && istreq(elem, "indicator")) {
        LedInfo अस्थायी = {0};
        InitLED(&अस्थायी);
        /* Do not replace the whole LED, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &अस्थायी, field, ndx, &stmt->मूल्य);
        अगर (ret)
            MergeLedMap(info, &info->default_led, &अस्थायी, true);
    }
    अन्यथा अगर (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->मूल्य, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        वापस !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    वापस ret;
}

अटल bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *arrayNdx;

    क्रम (; def; def = (VarDef *) def->common.अग्र) {
        अगर (!व्यंजक_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            जारी;
        }
        अगर (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            जारी;
        }
        अगर (!SetInterpField(info, si, field, arrayNdx, def->मूल्य))
            ok = false;
    }

    वापस ok;
}

अटल bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    क्रमागत xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    अगर (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        वापस false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    अगर (!HandleInterpBody(info, def->def, &si)) {
        info->error_गिनती++;
        वापस false;
    }

    अगर (!AddInterp(info, &si, true)) {
        info->error_गिनती++;
        वापस false;
    }

    वापस true;
}

अटल bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    क्रम (VarDef *var = def->body; var != शून्य; var = (VarDef *) var->common.अग्र) {
        स्थिर अक्षर *elem, *field;
        व्यंजक_Def *arrayNdx;
        अगर (!व्यंजक_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            जारी;
        }

        अगर (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        अन्यथा {
            अगर (!SetLedMapField(info, &ledi, field, arrayNdx, &var->मूल्य))
                ok = false;
        }
    }

    वापस ok && AddLedMap(info, &ledi, true);
}

अटल व्योम
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    क्रम (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        चयन (stmt->type) {
        हाल STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            अवरोध;
        हाल STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            अवरोध;
        हाल STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            अवरोध;
        हाल STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            अवरोध;
        हाल STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            अवरोध;
        हाल STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            अवरोध;
        हाल STMT_UNKNOWN_DECLARATION:
        हाल STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            अवरोध;
        शेष:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            अवरोध;
        }

        अगर (!ok)
            info->error_गिनती++;

        अगर (info->error_गिनती > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            अवरोध;
        }
    }
}

/* Temporary struct for CopyInterps. */
काष्ठा collect {
    darray(काष्ठा xkb_sym_interpret) sym_interprets;
};

अटल व्योम
CopyInterps(CompatInfo *info, bool need_प्रतीक, क्रमागत xkb_match_operation pred,
            काष्ठा collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        अगर (si->interp.match == pred &&
            (si->interp.sym != XKB_कुंजी_No_प्रतीक) == need_प्रतीक)
            darray_append(collect->sym_interprets, si->interp);
}

अटल व्योम
CopyLedMapDefsToKeymap(काष्ठा xkb_keymap *keymap, CompatInfo *info)
{
    क्रम (xkb_led_सूचकांक_t idx = 0; idx < info->अंक_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_सूचकांक_t i;
        काष्ठा xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            अगर (led->name == ledi->led.name)
                अवरोध;

        /* Not previously declared; create it with next free index. */
        अगर (i >= keymap->अंक_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                अगर (led->name == XKB_ATOM_NONE)
                    अवरोध;

            अगर (i >= keymap->अंक_leds) {
                /* Not place to put it; ignore. */
                अगर (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    जारी;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->अंक_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        अगर (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        अगर (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

अटल bool
CopyCompatToKeymap(काष्ठा xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    अगर (!darray_empty(info->interps)) {
        काष्ठा collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, MATCH_EXACTLY, &collect);
        CopyInterps(info, true, MATCH_ALL, &collect);
        CopyInterps(info, true, MATCH_NONE, &collect);
        CopyInterps(info, true, MATCH_ANY, &collect);
        CopyInterps(info, true, MATCH_ANY_OR_NONE, &collect);
        CopyInterps(info, false, MATCH_EXACTLY, &collect);
        CopyInterps(info, false, MATCH_ALL, &collect);
        CopyInterps(info, false, MATCH_NONE, &collect);
        CopyInterps(info, false, MATCH_ANY, &collect);
        CopyInterps(info, false, MATCH_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->अंक_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    वापस true;
}

bool
CompileCompatMap(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    अगर (file != शून्य)
        HandleCompatMapFile(&info, file);

    अगर (info.error_गिनती != 0)
        जाओ err_info;

    अगर (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        जाओ err_info;

    ClearCompatInfo(&info);
    वापस true;

err_info:
    ClearCompatInfo(&info);
    वापस false;
}
