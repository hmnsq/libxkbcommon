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

गणना si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

प्रकारव्याख्या संरचना {
    गणना si_field defined;
    गणना merge_mode merge;

    संरचना xkb_sym_interpret interp;
} SymInterpInfo;

गणना led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

प्रकारव्याख्या संरचना {
    गणना led_field defined;
    गणना merge_mode merge;

    संरचना xkb_led led;
} LedInfo;

प्रकारव्याख्या संरचना {
    अक्षर *name;
    पूर्णांक error_मोजणी;
    अचिन्हित पूर्णांक include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t अंक_leds;
    ActionsInfo default_actions;
    संरचना xkb_mod_set mods;

    स्थिरांक संरचना xkb_keymap_info *keymap_info;
    संरचना xkb_context *ctx;
} CompatInfo;

स्थिर स्थिरांक अक्षर *
siText(SymInterpInfo *si, CompatInfo *info)
{
    अक्षर *buf = xkb_context_get_buffer(info->ctx, 128);

    जर (si == &info->default_interp)
        परत "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    परत buf;
}

स्थिर ओळीत bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, स्थिरांक अक्षर *field)
{
    परत ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

स्थिर ओळीत bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, स्थिरांक अक्षर *field,
                स्थिरांक अक्षर *wanted)
{
    परत ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

स्थिर स्थिरांक अक्षर*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    जर (ledi == &info->default_led) {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) == रिक्त);
        परत "default";
    } अन्यथा {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) != रिक्त);
        परत xkb_atom_text(info->ctx, ledi->led.name);
    }
}

स्थिर ओळीत bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, स्थिरांक अक्षर *field,
                 स्थिरांक अक्षर *wanted)
{
    परत ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

स्थिर ओळीत bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, स्थिरांक अक्षर *field)
{
    परत ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

स्थिर ओळीत शून्य
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_किल्ली_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_किल्ली_REPEAT, "invalid initial value");
}

स्थिर ओळीत शून्य
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

स्थिर शून्य
InitCompatInfo(CompatInfo *info, स्थिरांक संरचना xkb_keymap_info *keymap_info,
               अचिन्हित पूर्णांक include_depth, स्थिरांक संरचना xkb_mod_set *mods)
{
    स_रखो(info, 0, आकार(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
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
        जर (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            परत old;

    परत रिक्त;
}

स्थिर bool
UseNewInterpField(गणना si_field field, गणना si_field old, गणना si_field new,
                  bool clobber, bool report, गणना si_field *collide)
{
    जर (!(old & field))
        परत (new & field);

    जर (new & field) {
        जर (report)
            *collide |= field;

        परत clobber;
    }

    परत false;
}

स्थिर bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    स्थिरांक bool clobber = (new->merge != MERGE_AUGMENT);
    स्थिरांक पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिरांक bool report = (same_file && verbosity > 0) || verbosity > 9;
    गणना si_field collide = 0;

    जर (new->merge == MERGE_REPLACE) {
        जर (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        परत true;
    }

    जर (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    जर (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        जर (old->interp.अंक_actions > 1) {
                मुक्त(old->interp.a.actions);
            }
            old->interp.अंक_actions = new->interp.अंक_actions;
            जर (new->interp.अंक_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (संघ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.अंक_actions = 0;
            } अन्यथा {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    जर (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    जर (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    जर (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    परत true;
}

स्थिर bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    जर (old)
        परत MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    परत true;
}

/***====================================================================***/

// TODO: more lenient?
स्थिर bool
ResolveStateAndPredicate(अभिव्यक्ती_Def *अभिव्यक्ती, गणना xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    जर (अभिव्यक्ती == रिक्त) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        परत true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    जर (अभिव्यक्ती->common.type == STMT_अभिव्यक्ती_ACTION_DECL) {
        स्थिरांक अक्षर *pred_txt = xkb_atom_text(info->ctx, अभिव्यक्ती->action.name);
        अचिन्हित पूर्णांक pred = 0;
        जर (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !अभिव्यक्ती->action.args || अभिव्यक्ती->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            परत false;
        }
        *pred_rtrn = (गणना xkb_match_operation) pred;
        अभिव्यक्ती = अभिव्यक्ती->action.args;
    }
    अन्यथा जर (अभिव्यक्ती->common.type == STMT_अभिव्यक्ती_IDENT) {
        स्थिरांक अक्षर *pred_txt = xkb_atom_text(info->ctx, अभिव्यक्ती->ident.ident);
        जर (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            परत true;
        }
    }

    परत अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, अभिव्यक्ती, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

स्थिर bool
UseNewLEDField(गणना led_field field, गणना led_field old, गणना led_field new,
               bool clobber, bool report, गणना led_field *collide)
{
    जर (!(old & field))
        परत (new & field);

    जर (new & field) {
        जर (report)
            *collide |= field;

        परत clobber;
    }

    परत false;
}

स्थिर bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    गणना led_field collide;
    स्थिरांक bool clobber = (new->merge != MERGE_AUGMENT);
    स्थिरांक पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिरांक bool report = (same_file && verbosity > 0) || verbosity > 9;

    जर (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        परत true;
    }

    जर (new->merge == MERGE_REPLACE) {
        जर (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        परत true;
    }

    collide = 0;
    जर (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    जर (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    जर (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    जर (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    परत true;
}

स्थिर bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    साठी (xkb_led_index_t i = 0; i < info->अंक_leds; i++) {
        LedInfo *old = &info->leds[i];

        जर (old->led.name != new->led.name)
            सुरूठेवा;

        परत MergeLedMap(info, old, new, same_file);
    }

    जर (info->अंक_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        परत false;
    }
    info->leds[info->अंक_leds++] = *new;
    परत true;
}

स्थिर शून्य
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        गणना merge_mode merge)
{
    जर (from->error_मोजणी > 0) {
        into->error_मोजणी += from->error_मोजणी;
        परत;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    जर (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    जर (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    अन्यथा {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            जर (!AddInterp(into, si, false))
                into->error_मोजणी++;
        }
    }

    जर (into->अंक_leds == 0) {
        स_नकल(into->leds, from->leds, आकार(*from->leds) * from->अंक_leds);
        into->अंक_leds = from->अंक_leds;
        from->अंक_leds = 0;
    }
    अन्यथा {
        साठी (xkb_led_index_t i = 0; i < from->अंक_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            जर (!AddLedMap(into, ledi, false))
                into->error_मोजणी++;
        }
    }
}

स्थिर शून्य
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    जर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_मोजणी += 10;
        परत false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    स्थिरांक संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    साठी (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, आकार(path));
        जर (!file) {
            info->error_मोजणी += 10;
            ClearCompatInfo(&included);
            परत false;
        }

        InitCompatInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                       &included.mods);
        next_incl.default_interp = info->default_interp;
        next_incl.default_led = info->default_led;

        HandleCompatMapFile(&next_incl, file);

        MergeIncludedCompatMaps(&included, &next_incl, stmt->merge);

        ClearCompatInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedCompatMaps(info, &included, include->merge);
    ClearCompatInfo(&included);

    परत (info->error_मोजणी == 0);
}

स्थिर bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, स्थिरांक अक्षर *field,
               अभिव्यक्ती_Def *arrayNdx, अभिव्यक्ती_Def *मूल्य)
{
    जर (istreq(field, "action")) {
        जर (arrayNdx)
            परत ReportSINotArray(info, si, field);

        जर (मूल्य->common.type == STMT_अभिव्यक्ती_ACTION_LIST) {
            अचिन्हित पूर्णांक अंक_actions = 0;
            साठी (अभिव्यक्ती_Def *act = मूल्य->actions.actions;
                 act; act = (अभिव्यक्ती_Def *) act->common.next)
                 अंक_actions++;

            जर (अंक_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, अंक_actions);
                परत false;
            }

            si->interp.अंक_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(संघ xkb_action) actions = darray_new();
            साठी (अभिव्यक्ती_Def *act = मूल्य->actions.actions;
                 act; act = (अभिव्यक्ती_Def *) act->common.next) {
                संघ xkb_action toAct = { 0 };
                निवड (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                स्थिती PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    थांबा;
                स्थिती PARSER_FATAL_ERROR:
                    darray_free(actions);
                    परत false;
                पूर्वनिर्धारित:
                    ;
                }
                जर (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } अन्यथा जर (likely(अंक_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.अंक_actions = 1;
                    si->interp.a.action = toAct;
                } अन्यथा {
                    darray_append(actions, toAct);
                }
            }
            निवड (darray_size(actions)) {
            स्थिती 0:
                /* No action or exactly one action: already processed */
                प्रतिज्ञा(si->interp.अंक_actions <= 1);
                थांबा;
            स्थिती 1:
                /* One action: some actions were dropped */
                si->interp.अंक_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                थांबा;
            पूर्वनिर्धारित:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.अंक_actions =
                    (xkb_action_मोजणी_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, रिक्त);
            }
        }
        अन्यथा {
            निवड (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, मूल्य, &si->interp.a.action)) {
            स्थिती PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.अंक_actions = 0;
                थांबा;
            स्थिती PARSER_FATAL_ERROR:
                परत false;
            पूर्वनिर्धारित:
                si->interp.अंक_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    अन्यथा जर (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        जर (arrayNdx)
            परत ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        जर (!अभिव्यक्ती_Resolve_Mod(info->ctx, मूल्य, MOD_VIRT, &info->mods, &ndx))
            परत ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    अन्यथा जर (istreq(field, "repeat")) {
        bool set = false;

        जर (arrayNdx)
            परत ReportSINotArray(info, si, field);

        जर (!अभिव्यक्ती_Resolve_Boolean(info->ctx, मूल्य, &set))
            परत ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    अन्यथा जर (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    अन्यथा जर (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        जर (arrayNdx)
            परत ReportSINotArray(info, si, field);

        जर (!अभिव्यक्ती_Resolve_Enum(info->ctx, मूल्य, &val, use_Mod_Map_मूल्य_Names))
            परत ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    अन्यथा {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        परत !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    परत true;
}

स्थिर bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, स्थिरांक अक्षर *field,
               अभिव्यक्ती_Def *arrayNdx, अभिव्यक्ती_Def **मूल्य_ptr)
{
    अभिव्यक्ती_Def * restrict स्थिरांक मूल्य = *मूल्य_ptr;

    जर (istreq(field, "modifiers") || istreq(field, "mods")) {
        जर (arrayNdx)
            परत ReportLedNotArray(info, ledi, field);

        जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            परत ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    अन्यथा जर (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        जर (arrayNdx)
            परत ReportLedNotArray(info, ledi, field);

        bool pending = false;
        जर (!अभिव्यक्ती_Resolve_Group_Mask(info->keymap_info, मूल्य, &mask, &pending)) {
            जर (pending) {
                ledi->led.pending_groups = true;
                स्थिरांक darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (संरचना pending_computation) {
                        .अभिव्यक्ती = *मूल्य_ptr,
                        .computed = false,
                        .मूल्य = 0,
                    }
                );
                *मूल्य_ptr = रिक्त;
                static_assert(आकार(pending_index) == आकार(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } अन्यथा {
                परत ReportLedBadType(info, ledi, field, "group mask");
            }
        } अन्यथा {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    अन्यथा जर (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        जर (arrayNdx)
            परत ReportLedNotArray(info, ledi, field);

        स्थिरांक uint8_t offset = info->keymap_info->features.controls_name_offset;
        जर (!अभिव्यक्ती_Resolve_Mask(info->ctx, मूल्य, &mask, ctrlMaskNames + offset))
            परत ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    अन्यथा जर (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अन्यथा जर (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        जर (arrayNdx)
            परत ReportLedNotArray(info, ledi, field);

        जर (!अभिव्यक्ती_Resolve_Mask(info->ctx, मूल्य, &mask,
                             modComponentMaskNames))
            परत ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    अन्यथा जर (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        जर (arrayNdx)
            परत ReportLedNotArray(info, ledi, field);

        जर (!अभिव्यक्ती_Resolve_Mask(info->ctx, मूल्य, &mask,
                             groupComponentMaskNames))
            परत ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    अन्यथा जर (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    अन्यथा जर (istreq(field, "index")) {
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
        परत !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    परत true;
}

स्थिर bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    स्थिरांक अक्षर *elem, *field;
    अभिव्यक्ती_Def *ndx;
    bool ret;

    जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    अन्यथा जर (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->मूल्य);
        जर (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    अन्यथा जर (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->मूल्य);
        जर (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    अन्यथा जर (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->मूल्य, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        परत !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    परत ret;
}

स्थिर bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    स्थिरांक अक्षर *elem, *field;
    अभिव्यक्ती_Def *arrayNdx;

    साठी (; def; def = (VarDef *) def->common.next) {
        जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            सुरूठेवा;
        }
        जर (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            सुरूठेवा;
        }
        जर (!SetInterpField(info, si, field, arrayNdx, def->मूल्य))
            ok = false;
    }

    परत ok;
}

स्थिर bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    गणना xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    जर (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        परत false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    जर (!HandleInterpBody(info, def->def, &si)) {
        info->error_मोजणी++;
        परत false;
    }

    जर (!AddInterp(info, &si, true)) {
        info->error_मोजणी++;
        परत false;
    }

    परत true;
}

स्थिर bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    साठी (VarDef *var = def->body; var != रिक्त; var = (VarDef *) var->common.next) {
        स्थिरांक अक्षर *elem, *field;
        अभिव्यक्ती_Def *arrayNdx;
        जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            सुरूठेवा;
        }

        जर (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        अन्यथा {
            जर (!SetLedMapField(info, &ledi, field, arrayNdx, &var->मूल्य))
                ok = false;
        }
    }

    परत ok && AddLedMap(info, &ledi, true);
}

स्थिर शून्य
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    साठी (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        निवड (stmt->type) {
        स्थिती STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            थांबा;
        स्थिती STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            थांबा;
        स्थिती STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            थांबा;
        स्थिती STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            थांबा;
        स्थिती STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            थांबा;
        स्थिती STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            थांबा;
        स्थिती STMT_UNKNOWN_DECLARATION:
        स्थिती STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            थांबा;
        पूर्वनिर्धारित:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            थांबा;
        }

        जर (!ok)
            info->error_मोजणी++;

        जर (info->error_मोजणी > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            थांबा;
        }
    }
}

/* Temporary struct for CopyInterps. */
संरचना collect {
    darray(संरचना xkb_sym_interpret) sym_interprets;
};

स्थिर शून्य
CopyInterps(CompatInfo *info, bool need_चिन्ह, गणना xkb_match_operation pred,
            संरचना collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        जर (si->interp.match == pred &&
            (si->interp.sym != XKB_किल्ली_No_चिन्ह) == need_चिन्ह)
            darray_append(collect->sym_interprets, si->interp);
}

स्थिर शून्य
CopyLedMapDefsToKeymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    साठी (xkb_led_index_t idx = 0; idx < info->अंक_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        संरचना xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            जर (led->name == ledi->led.name)
                थांबा;

        /* Not previously declared; create it with next free index. */
        जर (i >= keymap->अंक_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                जर (led->name == XKB_ATOM_NONE)
                    थांबा;

            जर (i >= keymap->अंक_leds) {
                /* Not place to put it; ignore. */
                जर (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    सुरूठेवा;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->अंक_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        जर (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        जर (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

स्थिर bool
CopyCompatToKeymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    जर (!darray_empty(info->interps)) {
        संरचना collect collect;
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

    परत true;
}

bool
CompileCompatMap(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    जर (file != रिक्त)
        HandleCompatMapFile(&info, file);

    जर (info.error_मोजणी != 0)
        जा err_info;

    जर (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        जा err_info;

    ClearCompatInfo(&info);
    परत true;

err_info:
    ClearCompatInfo(&info);
    परत false;
}
