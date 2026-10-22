/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "action.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "vmod.h"
#অন্তর্ভাব "util-mem.h"

গণনা si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

প্রকারলেখ কাঠামো {
    গণনা si_field defined;
    গণনা merge_mode merge;

    কাঠামো xkb_sym_interpret interp;
} SymInterpInfo;

গণনা led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

প্রকারলেখ কাঠামো {
    গণনা led_field defined;
    গণনা merge_mode merge;

    কাঠামো xkb_led led;
} LedInfo;

প্রকারলেখ কাঠামো {
    অক্ষর *name;
    পূর্ণসংখ্যা error_গুনতি;
    unsigned পূর্ণসংখ্যা include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_সূচক_t অঙ্ক_leds;
    ActionsInfo default_actions;
    কাঠামো xkb_mod_set mods;

    ধ্রুবক কাঠামো xkb_keymap_info *keymap_info;
    কাঠামো xkb_context *ctx;
} CompatInfo;

স্থির ধ্রুবক অক্ষর *
siText(SymInterpInfo *si, CompatInfo *info)
{
    অক্ষর *buf = xkb_context_get_বাফার(info->ctx, 128);

    যদি (si == &info->default_interp)
        ফেরত "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ফেরত buf;
}

স্থির অন্তঃরেখ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ধ্রুবক অক্ষর *field)
{
    ফেরত ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

স্থির অন্তঃরেখ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ধ্রুবক অক্ষর *field,
                ধ্রুবক অক্ষর *wanted)
{
    ফেরত ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

স্থির ধ্রুবক অক্ষর*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    যদি (ledi == &info->default_led) {
        প্রতিজ্ঞা(xkb_atom_text(info->ctx, ledi->led.name) == ফাঁকা);
        ফেরত "default";
    } অন্যথা {
        প্রতিজ্ঞা(xkb_atom_text(info->ctx, ledi->led.name) != ফাঁকা);
        ফেরত xkb_atom_text(info->ctx, ledi->led.name);
    }
}

স্থির অন্তঃরেখ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ধ্রুবক অক্ষর *field,
                 ধ্রুবক অক্ষর *wanted)
{
    ফেরত ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

স্থির অন্তঃরেখ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ধ্রুবক অক্ষর *field)
{
    ফেরত ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

স্থির অন্তঃরেখ শূন্য
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_চাবি_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_চাবি_REPEAT, "invalid initial value");
}

স্থির অন্তঃরেখ শূন্য
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

স্থির শূন্য
InitCompatInfo(CompatInfo *info, ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
               unsigned পূর্ণসংখ্যা include_depth, ধ্রুবক কাঠামো xkb_mod_set *mods)
{
    memset(info, 0, আকার(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

স্থির শূন্য
ClearCompatInfo(CompatInfo *info)
{
    মুক্ত(info->name);
    darray_free(info->interps);
}

স্থির SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        যদি (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            ফেরত old;

    ফেরত ফাঁকা;
}

স্থির bool
UseNewInterpField(গণনা si_field field, গণনা si_field old, গণনা si_field new,
                  bool clobber, bool report, গণনা si_field *collide)
{
    যদি (!(old & field))
        ফেরত (new & field);

    যদি (new & field) {
        যদি (report)
            *collide |= field;

        ফেরত clobber;
    }

    ফেরত false;
}

স্থির bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ধ্রুবক bool clobber = (new->merge != MERGE_AUGMENT);
    ধ্রুবক পূর্ণসংখ্যা verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুবক bool report = (same_file && verbosity > 0) || verbosity > 9;
    গণনা si_field collide = 0;

    যদি (new->merge == MERGE_REPLACE) {
        যদি (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ফেরত true;
    }

    যদি (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    যদি (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        যদি (old->interp.অঙ্ক_actions > 1) {
                মুক্ত(old->interp.a.actions);
            }
            old->interp.অঙ্ক_actions = new->interp.অঙ্ক_actions;
            যদি (new->interp.অঙ্ক_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (সংঘ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.অঙ্ক_actions = 0;
            } অন্যথা {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    যদি (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    যদি (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    যদি (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    ফেরত true;
}

স্থির bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    যদি (old)
        ফেরত MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ফেরত true;
}

/***====================================================================***/

// TODO: more lenient?
স্থির bool
ResolveStateAndPredicate(রাশিমালা_Def *রাশিমালা, গণনা xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    যদি (রাশিমালা == ফাঁকা) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ফেরত true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    যদি (রাশিমালা->common.type == STMT_রাশিমালা_ACTION_DECL) {
        ধ্রুবক অক্ষর *pred_txt = xkb_atom_text(info->ctx, রাশিমালা->action.name);
        unsigned পূর্ণসংখ্যা pred = 0;
        যদি (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !রাশিমালা->action.args || রাশিমালা->action.args->common.পরবর্তী) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ফেরত false;
        }
        *pred_rtrn = (গণনা xkb_match_operation) pred;
        রাশিমালা = রাশিমালা->action.args;
    }
    অন্যথা যদি (রাশিমালা->common.type == STMT_রাশিমালা_IDENT) {
        ধ্রুবক অক্ষর *pred_txt = xkb_atom_text(info->ctx, রাশিমালা->ident.ident);
        যদি (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ফেরত true;
        }
    }

    ফেরত রাশিমালা_Resolve_Mod_Mask(info->ctx, রাশিমালা, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

স্থির bool
UseNewLEDField(গণনা led_field field, গণনা led_field old, গণনা led_field new,
               bool clobber, bool report, গণনা led_field *collide)
{
    যদি (!(old & field))
        ফেরত (new & field);

    যদি (new & field) {
        যদি (report)
            *collide |= field;

        ফেরত clobber;
    }

    ফেরত false;
}

স্থির bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    গণনা led_field collide;
    ধ্রুবক bool clobber = (new->merge != MERGE_AUGMENT);
    ধ্রুবক পূর্ণসংখ্যা verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুবক bool report = (same_file && verbosity > 0) || verbosity > 9;

    যদি (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ফেরত true;
    }

    যদি (new->merge == MERGE_REPLACE) {
        যদি (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ফেরত true;
    }

    collide = 0;
    যদি (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    যদি (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    যদি (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    যদি (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ফেরত true;
}

স্থির bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    জন্য (xkb_led_সূচক_t i = 0; i < info->অঙ্ক_leds; i++) {
        LedInfo *old = &info->leds[i];

        যদি (old->led.name != new->led.name)
            চালু;

        ফেরত MergeLedMap(info, old, new, same_file);
    }

    যদি (info->অঙ্ক_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ফেরত false;
    }
    info->leds[info->অঙ্ক_leds++] = *new;
    ফেরত true;
}

স্থির শূন্য
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        গণনা merge_mode merge)
{
    যদি (from->error_গুনতি > 0) {
        into->error_গুনতি += from->error_গুনতি;
        ফেরত;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    যদি (into->name == ফাঁকা) {
        into->name = steal(&from->name);
    }

    যদি (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    অন্যথা {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            যদি (!AddInterp(into, si, false))
                into->error_গুনতি++;
        }
    }

    যদি (into->অঙ্ক_leds == 0) {
        memcpy(into->leds, from->leds, আকার(*from->leds) * from->অঙ্ক_leds);
        into->অঙ্ক_leds = from->অঙ্ক_leds;
        from->অঙ্ক_leds = 0;
    }
    অন্যথা {
        জন্য (xkb_led_সূচক_t i = 0; i < from->অঙ্ক_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            যদি (!AddLedMap(into, ledi, false))
                into->error_গুনতি++;
        }
    }
}

স্থির শূন্য
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

স্থির bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    যদি (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_গুনতি += 10;
        ফেরত false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    জন্য (IncludeStmt *stmt = include; stmt; stmt = stmt->পরবর্তী_incl) {
        CompatInfo পরবর্তী_incl;
        XkbFile *file;

        অক্ষর path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, আকার(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            ClearCompatInfo(&included);
            ফেরত false;
        }

        InitCompatInfo(&পরবর্তী_incl, info->keymap_info, info->include_depth + 1,
                       &included.mods);
        পরবর্তী_incl.default_interp = info->default_interp;
        পরবর্তী_incl.default_led = info->default_led;

        HandleCompatMapFile(&পরবর্তী_incl, file);

        MergeIncludedCompatMaps(&included, &পরবর্তী_incl, stmt->merge);

        ClearCompatInfo(&পরবর্তী_incl);
        FreeXkbFile(file);
    }

    MergeIncludedCompatMaps(info, &included, include->merge);
    ClearCompatInfo(&included);

    ফেরত (info->error_গুনতি == 0);
}

স্থির bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ধ্রুবক অক্ষর *field,
               রাশিমালা_Def *arrayNdx, রাশিমালা_Def *মান)
{
    যদি (istreq(field, "action")) {
        যদি (arrayNdx)
            ফেরত ReportSINotArray(info, si, field);

        যদি (মান->common.type == STMT_রাশিমালা_ACTION_LIST) {
            unsigned পূর্ণসংখ্যা অঙ্ক_actions = 0;
            জন্য (রাশিমালা_Def *act = মান->actions.actions;
                 act; act = (রাশিমালা_Def *) act->common.পরবর্তী)
                 অঙ্ক_actions++;

            যদি (অঙ্ক_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, অঙ্ক_actions);
                ফেরত false;
            }

            si->interp.অঙ্ক_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(সংঘ xkb_action) actions = darray_new();
            জন্য (রাশিমালা_Def *act = মান->actions.actions;
                 act; act = (রাশিমালা_Def *) act->common.পরবর্তী) {
                সংঘ xkb_action toAct = { 0 };
                নির্বাচন (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                ক্ষেত্র PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    বিরতি;
                ক্ষেত্র PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ফেরত false;
                পূর্বনির্ধারিত:
                    ;
                }
                যদি (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } অন্যথা যদি (likely(অঙ্ক_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.অঙ্ক_actions = 1;
                    si->interp.a.action = toAct;
                } অন্যথা {
                    darray_append(actions, toAct);
                }
            }
            নির্বাচন (darray_আকার(actions)) {
            ক্ষেত্র 0:
                /* No action or exactly one action: already processed */
                প্রতিজ্ঞা(si->interp.অঙ্ক_actions <= 1);
                বিরতি;
            ক্ষেত্র 1:
                /* One action: some actions were dropped */
                si->interp.অঙ্ক_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                বিরতি;
            পূর্বনির্ধারিত:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.অঙ্ক_actions =
                    (xkb_action_গুনতি_t) darray_আকার(actions);
                darray_steal(actions, &si->interp.a.actions, ফাঁকা);
            }
        }
        অন্যথা {
            নির্বাচন (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, মান, &si->interp.a.action)) {
            ক্ষেত্র PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.অঙ্ক_actions = 0;
                বিরতি;
            ক্ষেত্র PARSER_FATAL_ERROR:
                ফেরত false;
            পূর্বনির্ধারিত:
                si->interp.অঙ্ক_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    অন্যথা যদি (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        যদি (arrayNdx)
            ফেরত ReportSINotArray(info, si, field);

        xkb_mod_সূচক_t ndx = 0;
        যদি (!রাশিমালা_Resolve_Mod(info->ctx, মান, MOD_VIRT, &info->mods, &ndx))
            ফেরত ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    অন্যথা যদি (istreq(field, "repeat")) {
        bool set = false;

        যদি (arrayNdx)
            ফেরত ReportSINotArray(info, si, field);

        যদি (!রাশিমালা_Resolve_Boolean(info->ctx, মান, &set))
            ফেরত ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    অন্যথা যদি (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    অন্যথা যদি (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        যদি (arrayNdx)
            ফেরত ReportSINotArray(info, si, field);

        যদি (!রাশিমালা_Resolve_Enum(info->ctx, মান, &val, use_Mod_Map_মান_Names))
            ফেরত ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    অন্যথা {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ফেরত !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ফেরত true;
}

স্থির bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ধ্রুবক অক্ষর *field,
               রাশিমালা_Def *arrayNdx, রাশিমালা_Def **মান_ptr)
{
    রাশিমালা_Def * restrict ধ্রুবক মান = *মান_ptr;

    যদি (istreq(field, "modifiers") || istreq(field, "mods")) {
        যদি (arrayNdx)
            ফেরত ReportLedNotArray(info, ledi, field);

        যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, মান, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ফেরত ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    অন্যথা যদি (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        যদি (arrayNdx)
            ফেরত ReportLedNotArray(info, ledi, field);

        bool pending = false;
        যদি (!রাশিমালা_Resolve_Group_Mask(info->keymap_info, মান, &mask, &pending)) {
            যদি (pending) {
                ledi->led.pending_groups = true;
                ধ্রুবক darray_আকার_t pending_সূচক =
                    darray_আকার(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (কাঠামো pending_computation) {
                        .রাশিমালা = *মান_ptr,
                        .computed = false,
                        .মান = 0,
                    }
                );
                *মান_ptr = ফাঁকা;
                static_assert(আকার(pending_সূচক) == আকার(mask),
                              "Cannot save pending computation");
                mask = pending_সূচক;
            } অন্যথা {
                ফেরত ReportLedBadType(info, ledi, field, "group mask");
            }
        } অন্যথা {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    অন্যথা যদি (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        যদি (arrayNdx)
            ফেরত ReportLedNotArray(info, ledi, field);

        ধ্রুবক uint8_t offset = info->keymap_info->features.controls_name_offset;
        যদি (!রাশিমালা_Resolve_Mask(info->ctx, মান, &mask, ctrlMaskNames + offset))
            ফেরত ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    অন্যথা যদি (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    অন্যথা যদি (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        যদি (arrayNdx)
            ফেরত ReportLedNotArray(info, ledi, field);

        যদি (!রাশিমালা_Resolve_Mask(info->ctx, মান, &mask,
                             modComponentMaskNames))
            ফেরত ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    অন্যথা যদি (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        যদি (arrayNdx)
            ফেরত ReportLedNotArray(info, ledi, field);

        যদি (!রাশিমালা_Resolve_Mask(info->ctx, মান, &mask,
                             groupComponentMaskNames))
            ফেরত ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    অন্যথা যদি (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    অন্যথা যদি (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    অন্যথা {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        ফেরত !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ফেরত true;
}

স্থির bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ধ্রুবক অক্ষর *elem, *field;
    রাশিমালা_Def *ndx;
    bool ret;

    যদি (!রাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    অন্যথা যদি (elem && istreq(elem, "interpret")) {
        SymInterpInfo অস্থায়ী = {0};
        InitInterp(&অস্থায়ী);
        /* Do not replace the whole interpret, only the current field */
        অস্থায়ী.merge = (অস্থায়ী.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &অস্থায়ী, field, ndx, stmt->মান);
        যদি (ret)
            MergeInterp(info, &info->default_interp, &অস্থায়ী, true);
    }
    অন্যথা যদি (elem && istreq(elem, "indicator")) {
        LedInfo অস্থায়ী = {0};
        InitLED(&অস্থায়ী);
        /* Do not replace the whole LED, only the current field */
        অস্থায়ী.merge = (অস্থায়ী.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &অস্থায়ী, field, ndx, &stmt->মান);
        যদি (ret)
            MergeLedMap(info, &info->default_led, &অস্থায়ী, true);
    }
    অন্যথা যদি (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->মান, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    অন্যথা {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ফেরত !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ফেরত ret;
}

স্থির bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ধ্রুবক অক্ষর *elem, *field;
    রাশিমালা_Def *arrayNdx;

    জন্য (; def; def = (VarDef *) def->common.পরবর্তী) {
        যদি (!রাশিমালা_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            চালু;
        }
        যদি (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            চালু;
        }
        যদি (!SetInterpField(info, si, field, arrayNdx, def->মান))
            ok = false;
    }

    ফেরত ok;
}

স্থির bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    গণনা xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    যদি (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ফেরত false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    যদি (!HandleInterpBody(info, def->def, &si)) {
        info->error_গুনতি++;
        ফেরত false;
    }

    যদি (!AddInterp(info, &si, true)) {
        info->error_গুনতি++;
        ফেরত false;
    }

    ফেরত true;
}

স্থির bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    জন্য (VarDef *var = def->body; var != ফাঁকা; var = (VarDef *) var->common.পরবর্তী) {
        ধ্রুবক অক্ষর *elem, *field;
        রাশিমালা_Def *arrayNdx;
        যদি (!রাশিমালা_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            চালু;
        }

        যদি (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        অন্যথা {
            যদি (!SetLedMapField(info, &ledi, field, arrayNdx, &var->মান))
                ok = false;
        }
    }

    ফেরত ok && AddLedMap(info, &ledi, true);
}

স্থির শূন্য
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    জন্য (ParseCommon *stmt = file->defs; stmt; stmt = stmt->পরবর্তী) {
        নির্বাচন (stmt->type) {
        ক্ষেত্র STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            বিরতি;
        ক্ষেত্র STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            বিরতি;
        ক্ষেত্র STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            বিরতি;
        ক্ষেত্র STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            বিরতি;
        ক্ষেত্র STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            বিরতি;
        ক্ষেত্র STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            বিরতি;
        ক্ষেত্র STMT_UNKNOWN_DECLARATION:
        ক্ষেত্র STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            বিরতি;
        পূর্বনির্ধারিত:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            বিরতি;
        }

        যদি (!ok)
            info->error_গুনতি++;

        যদি (info->error_গুনতি > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            বিরতি;
        }
    }
}

/* Temporary struct for CopyInterps. */
কাঠামো collect {
    darray(কাঠামো xkb_sym_interpret) sym_interprets;
};

স্থির শূন্য
CopyInterps(CompatInfo *info, bool need_প্রতীক, গণনা xkb_match_operation pred,
            কাঠামো collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        যদি (si->interp.match == pred &&
            (si->interp.sym != XKB_চাবি_No_প্রতীক) == need_প্রতীক)
            darray_append(collect->sym_interprets, si->interp);
}

স্থির শূন্য
CopyLedMapDefsToKeymap(কাঠামো xkb_keymap *keymap, CompatInfo *info)
{
    জন্য (xkb_led_সূচক_t idx = 0; idx < info->অঙ্ক_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_সূচক_t i;
        কাঠামো xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            যদি (led->name == ledi->led.name)
                বিরতি;

        /* Not previously declared; create it with next free index. */
        যদি (i >= keymap->অঙ্ক_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                যদি (led->name == XKB_ATOM_NONE)
                    বিরতি;

            যদি (i >= keymap->অঙ্ক_leds) {
                /* Not place to put it; ignore. */
                যদি (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    চালু;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->অঙ্ক_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        যদি (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        যদি (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

স্থির bool
CopyCompatToKeymap(কাঠামো xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    যদি (!darray_empty(info->interps)) {
        কাঠামো collect collect;
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
                     &keymap->sym_interprets, &keymap->অঙ্ক_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    ফেরত true;
}

bool
CompileCompatMap(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    যদি (file != ফাঁকা)
        HandleCompatMapFile(&info, file);

    যদি (info.error_গুনতি != 0)
        যাও err_info;

    যদি (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        যাও err_info;

    ClearCompatInfo(&info);
    ফেরত true;

err_info:
    ClearCompatInfo(&info);
    ফেরত false;
}
