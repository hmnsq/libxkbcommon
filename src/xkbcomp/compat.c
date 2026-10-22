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

प्रकारनिर्देश संरचना {
    गणना si_field defined;
    गणना merge_mode merge;

    संरचना xkb_sym_interpret interp;
} SymInterpInfo;

गणना led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

प्रकारनिर्देश संरचना {
    गणना led_field defined;
    गणना merge_mode merge;

    संरचना xkb_led led;
} LedInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    unsigned पूर्णाङ्क include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t अङ्कः_leds;
    ActionsInfo default_actions;
    संरचना xkb_mod_set mods;

    ध्रुव संरचना xkb_keymap_info *keymap_info;
    संरचना xkb_context *ctx;
} CompatInfo;

स्थिर ध्रुव वर्ण *
siText(SymInterpInfo *si, CompatInfo *info)
{
    वर्ण *buf = xkb_context_get_buffer(info->ctx, 128);

    सचे (si == &info->default_interp)
        पटिनिवत्त "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    पटिनिवत्त buf;
}

स्थिर अन्तःस्थ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *field)
{
    पटिनिवत्त ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

स्थिर अन्तःस्थ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *field,
                ध्रुव वर्ण *wanted)
{
    पटिनिवत्त ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

स्थिर ध्रुव वर्ण*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    सचे (ledi == &info->default_led) {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) == रिक्त);
        पटिनिवत्त "default";
    } अञ्ञथा {
        प्रतिज्ञा(xkb_atom_text(info->ctx, ledi->led.name) != रिक्त);
        पटिनिवत्त xkb_atom_text(info->ctx, ledi->led.name);
    }
}

स्थिर अन्तःस्थ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *field,
                 ध्रुव वर्ण *wanted)
{
    पटिनिवत्त ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

स्थिर अन्तःस्थ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *field)
{
    पटिनिवत्त ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

स्थिर अन्तःस्थ शून्य
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_कुञ्चिका_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_कुञ्चिका_REPEAT, "invalid initial value");
}

स्थिर अन्तःस्थ शून्य
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

स्थिर शून्य
InitCompatInfo(CompatInfo *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
               unsigned पूर्णाङ्क include_depth, ध्रुव संरचना xkb_mod_set *mods)
{
    memset(info, 0, परिमाण(*info));
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
        सचे (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            पटिनिवत्त old;

    पटिनिवत्त रिक्त;
}

स्थिर bool
UseNewInterpField(गणना si_field field, गणना si_field old, गणना si_field new,
                  bool clobber, bool report, गणना si_field *collide)
{
    सचे (!(old & field))
        पटिनिवत्त (new & field);

    सचे (new & field) {
        सचे (report)
            *collide |= field;

        पटिनिवत्त clobber;
    }

    पटिनिवत्त false;
}

स्थिर bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;
    गणना si_field collide = 0;

    सचे (new->merge == MERGE_REPLACE) {
        सचे (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        पटिनिवत्त true;
    }

    सचे (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    सचे (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        सचे (old->interp.अङ्कः_actions > 1) {
                मुक्त(old->interp.a.actions);
            }
            old->interp.अङ्कः_actions = new->interp.अङ्कः_actions;
            सचे (new->interp.अङ्कः_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (सङ्घ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.अङ्कः_actions = 0;
            } अञ्ञथा {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    सचे (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    सचे (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    सचे (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    पटिनिवत्त true;
}

स्थिर bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    सचे (old)
        पटिनिवत्त MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    पटिनिवत्त true;
}

/***====================================================================***/

// TODO: more lenient?
स्थिर bool
ResolveStateAndPredicate(व्यञ्जकम्_Def *व्यञ्जकम्, गणना xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    सचे (व्यञ्जकम् == रिक्त) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        पटिनिवत्त true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    सचे (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_ACTION_DECL) {
        ध्रुव वर्ण *pred_txt = xkb_atom_text(info->ctx, व्यञ्जकम्->action.name);
        unsigned पूर्णाङ्क pred = 0;
        सचे (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !व्यञ्जकम्->action.args || व्यञ्जकम्->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            पटिनिवत्त false;
        }
        *pred_rtrn = (गणना xkb_match_operation) pred;
        व्यञ्जकम् = व्यञ्जकम्->action.args;
    }
    अञ्ञथा सचे (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_IDENT) {
        ध्रुव वर्ण *pred_txt = xkb_atom_text(info->ctx, व्यञ्जकम्->ident.ident);
        सचे (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            पटिनिवत्त true;
        }
    }

    पटिनिवत्त व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, व्यञ्जकम्, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

स्थिर bool
UseNewLEDField(गणना led_field field, गणना led_field old, गणना led_field new,
               bool clobber, bool report, गणना led_field *collide)
{
    सचे (!(old & field))
        पटिनिवत्त (new & field);

    सचे (new & field) {
        सचे (report)
            *collide |= field;

        पटिनिवत्त clobber;
    }

    पटिनिवत्त false;
}

स्थिर bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    गणना led_field collide;
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;

    सचे (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        पटिनिवत्त true;
    }

    सचे (new->merge == MERGE_REPLACE) {
        सचे (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        पटिनिवत्त true;
    }

    collide = 0;
    सचे (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    सचे (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    सचे (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    सचे (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    पटिनिवत्त true;
}

स्थिर bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    अत्थाय (xkb_led_index_t i = 0; i < info->अङ्कः_leds; i++) {
        LedInfo *old = &info->leds[i];

        सचे (old->led.name != new->led.name)
            अनुवर्त;

        पटिनिवत्त MergeLedMap(info, old, new, same_file);
    }

    सचे (info->अङ्कः_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        पटिनिवत्त false;
    }
    info->leds[info->अङ्कः_leds++] = *new;
    पटिनिवत्त true;
}

स्थिर शून्य
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        गणना merge_mode merge)
{
    सचे (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        पटिनिवत्त;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    सचे (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    सचे (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    अञ्ञथा {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            सचे (!AddInterp(into, si, false))
                into->error_गणनम्++;
        }
    }

    सचे (into->अङ्कः_leds == 0) {
        memcpy(into->leds, from->leds, परिमाण(*from->leds) * from->अङ्कः_leds);
        into->अङ्कः_leds = from->अङ्कः_leds;
        from->अङ्कः_leds = 0;
    }
    अञ्ञथा {
        अत्थाय (xkb_led_index_t i = 0; i < from->अङ्कः_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            सचे (!AddLedMap(into, ledi, false))
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

    सचे (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गणनम् += 10;
        पटिनिवत्त false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    अत्थाय (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, परिमाण(path));
        सचे (!file) {
            info->error_गणनम् += 10;
            ClearCompatInfo(&included);
            पटिनिवत्त false;
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

    पटिनिवत्त (info->error_गणनम् == 0);
}

स्थिर bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ध्रुव वर्ण *field,
               व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्)
{
    सचे (istreq(field, "action")) {
        सचे (arrayNdx)
            पटिनिवत्त ReportSINotArray(info, si, field);

        सचे (मानम्->common.type == STMT_व्यञ्जकम्_ACTION_LIST) {
            unsigned पूर्णाङ्क अङ्कः_actions = 0;
            अत्थाय (व्यञ्जकम्_Def *act = मानम्->actions.actions;
                 act; act = (व्यञ्जकम्_Def *) act->common.next)
                 अङ्कः_actions++;

            सचे (अङ्कः_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, अङ्कः_actions);
                पटिनिवत्त false;
            }

            si->interp.अङ्कः_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(सङ्घ xkb_action) actions = darray_new();
            अत्थाय (व्यञ्जकम्_Def *act = मानम्->actions.actions;
                 act; act = (व्यञ्जकम्_Def *) act->common.next) {
                सङ्घ xkb_action toAct = { 0 };
                विकल्प (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                पक्ष PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    भङ्ग;
                पक्ष PARSER_FATAL_ERROR:
                    darray_free(actions);
                    पटिनिवत्त false;
                स्वभाव:
                    ;
                }
                सचे (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } अञ्ञथा सचे (likely(अङ्कः_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.अङ्कः_actions = 1;
                    si->interp.a.action = toAct;
                } अञ्ञथा {
                    darray_append(actions, toAct);
                }
            }
            विकल्प (darray_size(actions)) {
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
                    (xkb_action_गणनम्_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, रिक्त);
            }
        }
        अञ्ञथा {
            विकल्प (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, मानम्, &si->interp.a.action)) {
            पक्ष PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.अङ्कः_actions = 0;
                भङ्ग;
            पक्ष PARSER_FATAL_ERROR:
                पटिनिवत्त false;
            स्वभाव:
                si->interp.अङ्कः_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    अञ्ञथा सचे (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        सचे (arrayNdx)
            पटिनिवत्त ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        सचे (!व्यञ्जकम्_Resolve_Mod(info->ctx, मानम्, MOD_VIRT, &info->mods, &ndx))
            पटिनिवत्त ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    अञ्ञथा सचे (istreq(field, "repeat")) {
        bool set = false;

        सचे (arrayNdx)
            पटिनिवत्त ReportSINotArray(info, si, field);

        सचे (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &set))
            पटिनिवत्त ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    अञ्ञथा सचे (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    अञ्ञथा सचे (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        सचे (arrayNdx)
            पटिनिवत्त ReportSINotArray(info, si, field);

        सचे (!व्यञ्जकम्_Resolve_Enum(info->ctx, मानम्, &val, use_Mod_Map_मानम्_Names))
            पटिनिवत्त ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    अञ्ञथा {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    पटिनिवत्त true;
}

स्थिर bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ध्रुव वर्ण *field,
               व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def **मानम्_ptr)
{
    व्यञ्जकम्_Def * restrict ध्रुव मानम् = *मानम्_ptr;

    सचे (istreq(field, "modifiers") || istreq(field, "mods")) {
        सचे (arrayNdx)
            पटिनिवत्त ReportLedNotArray(info, ledi, field);

        सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, मानम्, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            पटिनिवत्त ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    अञ्ञथा सचे (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        सचे (arrayNdx)
            पटिनिवत्त ReportLedNotArray(info, ledi, field);

        bool pending = false;
        सचे (!व्यञ्जकम्_Resolve_Group_Mask(info->keymap_info, मानम्, &mask, &pending)) {
            सचे (pending) {
                ledi->led.pending_groups = true;
                ध्रुव darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (संरचना pending_computation) {
                        .व्यञ्जकम् = *मानम्_ptr,
                        .computed = false,
                        .मानम् = 0,
                    }
                );
                *मानम्_ptr = रिक्त;
                static_assert(परिमाण(pending_index) == परिमाण(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } अञ्ञथा {
                पटिनिवत्त ReportLedBadType(info, ledi, field, "group mask");
            }
        } अञ्ञथा {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    अञ्ञथा सचे (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        सचे (arrayNdx)
            पटिनिवत्त ReportLedNotArray(info, ledi, field);

        ध्रुव uint8_t offset = info->keymap_info->features.controls_name_offset;
        सचे (!व्यञ्जकम्_Resolve_Mask(info->ctx, मानम्, &mask, ctrlMaskNames + offset))
            पटिनिवत्त ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    अञ्ञथा सचे (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अञ्ञथा सचे (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        सचे (arrayNdx)
            पटिनिवत्त ReportLedNotArray(info, ledi, field);

        सचे (!व्यञ्जकम्_Resolve_Mask(info->ctx, मानम्, &mask,
                             modComponentMaskNames))
            पटिनिवत्त ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    अञ्ञथा सचे (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        सचे (arrayNdx)
            पटिनिवत्त ReportLedNotArray(info, ledi, field);

        सचे (!व्यञ्जकम्_Resolve_Mask(info->ctx, मानम्, &mask,
                             groupComponentMaskNames))
            पटिनिवत्त ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    अञ्ञथा सचे (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    अञ्ञथा सचे (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    अञ्ञथा {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    पटिनिवत्त true;
}

स्थिर bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *ndx;
    bool ret;

    सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    अञ्ञथा सचे (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->मानम्);
        सचे (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    अञ्ञथा सचे (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->मानम्);
        सचे (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    अञ्ञथा सचे (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->मानम्, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    अञ्ञथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        पटिनिवत्त !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    पटिनिवत्त ret;
}

स्थिर bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;

    अत्थाय (; def; def = (VarDef *) def->common.next) {
        सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            अनुवर्त;
        }
        सचे (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            अनुवर्त;
        }
        सचे (!SetInterpField(info, si, field, arrayNdx, def->मानम्))
            ok = false;
    }

    पटिनिवत्त ok;
}

स्थिर bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    गणना xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    सचे (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        पटिनिवत्त false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    सचे (!HandleInterpBody(info, def->def, &si)) {
        info->error_गणनम्++;
        पटिनिवत्त false;
    }

    सचे (!AddInterp(info, &si, true)) {
        info->error_गणनम्++;
        पटिनिवत्त false;
    }

    पटिनिवत्त true;
}

स्थिर bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    अत्थाय (VarDef *var = def->body; var != रिक्त; var = (VarDef *) var->common.next) {
        ध्रुव वर्ण *elem, *field;
        व्यञ्जकम्_Def *arrayNdx;
        सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            अनुवर्त;
        }

        सचे (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        अञ्ञथा {
            सचे (!SetLedMapField(info, &ledi, field, arrayNdx, &var->मानम्))
                ok = false;
        }
    }

    पटिनिवत्त ok && AddLedMap(info, &ledi, true);
}

स्थिर शून्य
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    अत्थाय (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            भङ्ग;
        पक्ष STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            भङ्ग;
        पक्ष STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
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
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        सचे (!ok)
            info->error_गणनम्++;

        सचे (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
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
CopyInterps(CompatInfo *info, bool need_चिह्नम्, गणना xkb_match_operation pred,
            संरचना collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        सचे (si->interp.match == pred &&
            (si->interp.sym != XKB_कुञ्चिका_No_चिह्नम्) == need_चिह्नम्)
            darray_append(collect->sym_interprets, si->interp);
}

स्थिर शून्य
CopyLedMapDefsToKeymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    अत्थाय (xkb_led_index_t idx = 0; idx < info->अङ्कः_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        संरचना xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            सचे (led->name == ledi->led.name)
                भङ्ग;

        /* Not previously declared; create it with next free index. */
        सचे (i >= keymap->अङ्कः_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                सचे (led->name == XKB_ATOM_NONE)
                    भङ्ग;

            सचे (i >= keymap->अङ्कः_leds) {
                /* Not place to put it; ignore. */
                सचे (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
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
        सचे (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        सचे (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

स्थिर bool
CopyCompatToKeymap(संरचना xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    सचे (!darray_empty(info->interps)) {
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
                     &keymap->sym_interprets, &keymap->अङ्कः_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    पटिनिवत्त true;
}

bool
CompileCompatMap(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    सचे (file != रिक्त)
        HandleCompatMapFile(&info, file);

    सचे (info.error_गणनम् != 0)
        गच्छ err_info;

    सचे (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearCompatInfo(&info);
    पटिनिवत्त true;

err_info:
    ClearCompatInfo(&info);
    पटिनिवत्त false;
}
