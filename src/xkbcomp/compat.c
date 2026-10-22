/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <limits.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"
#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "action.h"
#অন্তর্ভাৱ "expr.h"
#অন্তর্ভাৱ "include.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "vmod.h"
#অন্তর্ভাৱ "util-mem.h"

গণনা si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

প্রকারনির্দেশ সংরচনা {
    গণনা si_field defined;
    গণনা merge_mode merge;

    সংরচনা xkb_sym_interpret interp;
} SymInterpInfo;

গণনা led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

প্রকারনির্দেশ সংরচনা {
    গণনা led_field defined;
    গণনা merge_mode merge;

    সংরচনা xkb_led led;
} LedInfo;

প্রকারনির্দেশ সংরচনা {
    ৱর্ণ *name;
    পূর্ণাঙ্ক error_গুনতি;
    unsigned পূর্ণাঙ্ক include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t অঙ্ক_leds;
    ActionsInfo default_actions;
    সংরচনা xkb_mod_set mods;

    ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info;
    সংরচনা xkb_context *ctx;
} CompatInfo;

স্থির ধ্রুৱ ৱর্ণ *
siText(SymInterpInfo *si, CompatInfo *info)
{
    ৱর্ণ *buf = xkb_context_get_buffer(info->ctx, 128);

    যদি (si == &info->default_interp)
        ঘূৰাই "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ঘূৰাই buf;
}

স্থির অন্তঃস্থ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ধ্রুৱ ৱর্ণ *field)
{
    ঘূৰাই ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

স্থির অন্তঃস্থ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ধ্রুৱ ৱর্ণ *field,
                ধ্রুৱ ৱর্ণ *wanted)
{
    ঘূৰাই ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

স্থির ধ্রুৱ ৱর্ণ*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    যদি (ledi == &info->default_led) {
        প্রতিজ্ঞা(xkb_atom_text(info->ctx, ledi->led.name) == রিক্ত);
        ঘূৰাই "default";
    } অন্যথা {
        প্রতিজ্ঞা(xkb_atom_text(info->ctx, ledi->led.name) != রিক্ত);
        ঘূৰাই xkb_atom_text(info->ctx, ledi->led.name);
    }
}

স্থির অন্তঃস্থ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ধ্রুৱ ৱর্ণ *field,
                 ধ্রুৱ ৱর্ণ *wanted)
{
    ঘূৰাই ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

স্থির অন্তঃস্থ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ধ্রুৱ ৱর্ণ *field)
{
    ঘূৰাই ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

স্থির অন্তঃস্থ শূন্য
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_চাবি_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_চাবি_REPEAT, "invalid initial value");
}

স্থির অন্তঃস্থ শূন্য
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

স্থির শূন্য
InitCompatInfo(CompatInfo *info, ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
               unsigned পূর্ণাঙ্ক include_depth, ধ্রুৱ সংরচনা xkb_mod_set *mods)
{
    memset(info, 0, পরিমাণ(*info));
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
            ঘূৰাই old;

    ঘূৰাই রিক্ত;
}

স্থির bool
UseNewInterpField(গণনা si_field field, গণনা si_field old, গণনা si_field new,
                  bool clobber, bool report, গণনা si_field *collide)
{
    যদি (!(old & field))
        ঘূৰাই (new & field);

    যদি (new & field) {
        যদি (report)
            *collide |= field;

        ঘূৰাই clobber;
    }

    ঘূৰাই false;
}

স্থির bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ধ্রুৱ bool clobber = (new->merge != MERGE_AUGMENT);
    ধ্রুৱ পূর্ণাঙ্ক verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুৱ bool report = (same_file && verbosity > 0) || verbosity > 9;
    গণনা si_field collide = 0;

    যদি (new->merge == MERGE_REPLACE) {
        যদি (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ঘূৰাই true;
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
                    (সঙ্ঘ xkb_action) { .type = ACTION_TYPE_NONE };
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

    ঘূৰাই true;
}

স্থির bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    যদি (old)
        ঘূৰাই MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ঘূৰাই true;
}

/***====================================================================***/

// TODO: more lenient?
স্থির bool
ResolveStateAndPredicate(ৰাশিমালা_Def *ৰাশিমালা, গণনা xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    যদি (ৰাশিমালা == রিক্ত) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ঘূৰাই true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    যদি (ৰাশিমালা->common.type == STMT_ৰাশিমালা_ACTION_DECL) {
        ধ্রুৱ ৱর্ণ *pred_txt = xkb_atom_text(info->ctx, ৰাশিমালা->action.name);
        unsigned পূর্ণাঙ্ক pred = 0;
        যদি (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !ৰাশিমালা->action.args || ৰাশিমালা->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ঘূৰাই false;
        }
        *pred_rtrn = (গণনা xkb_match_operation) pred;
        ৰাশিমালা = ৰাশিমালা->action.args;
    }
    অন্যথা যদি (ৰাশিমালা->common.type == STMT_ৰাশিমালা_IDENT) {
        ধ্রুৱ ৱর্ণ *pred_txt = xkb_atom_text(info->ctx, ৰাশিমালা->ident.ident);
        যদি (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ঘূৰাই true;
        }
    }

    ঘূৰাই ৰাশিমালা_Resolve_Mod_Mask(info->ctx, ৰাশিমালা, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

স্থির bool
UseNewLEDField(গণনা led_field field, গণনা led_field old, গণনা led_field new,
               bool clobber, bool report, গণনা led_field *collide)
{
    যদি (!(old & field))
        ঘূৰাই (new & field);

    যদি (new & field) {
        যদি (report)
            *collide |= field;

        ঘূৰাই clobber;
    }

    ঘূৰাই false;
}

স্থির bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    গণনা led_field collide;
    ধ্রুৱ bool clobber = (new->merge != MERGE_AUGMENT);
    ধ্রুৱ পূর্ণাঙ্ক verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুৱ bool report = (same_file && verbosity > 0) || verbosity > 9;

    যদি (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ঘূৰাই true;
    }

    যদি (new->merge == MERGE_REPLACE) {
        যদি (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ঘূৰাই true;
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

    ঘূৰাই true;
}

স্থির bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    বাবে (xkb_led_index_t i = 0; i < info->অঙ্ক_leds; i++) {
        LedInfo *old = &info->leds[i];

        যদি (old->led.name != new->led.name)
            অনুৱর্ত;

        ঘূৰাই MergeLedMap(info, old, new, same_file);
    }

    যদি (info->অঙ্ক_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ঘূৰাই false;
    }
    info->leds[info->অঙ্ক_leds++] = *new;
    ঘূৰাই true;
}

স্থির শূন্য
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        গণনা merge_mode merge)
{
    যদি (from->error_গুনতি > 0) {
        into->error_গুনতি += from->error_গুনতি;
        ঘূৰাই;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    যদি (into->name == রিক্ত) {
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
        memcpy(into->leds, from->leds, পরিমাণ(*from->leds) * from->অঙ্ক_leds);
        into->অঙ্ক_leds = from->অঙ্ক_leds;
        from->অঙ্ক_leds = 0;
    }
    অন্যথা {
        বাবে (xkb_led_index_t i = 0; i < from->অঙ্ক_leds; i++) {
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
        ঘূৰাই false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ধ্রুৱ সংরচনা parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    বাবে (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        ৱর্ণ path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, পরিমাণ(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            ClearCompatInfo(&included);
            ঘূৰাই false;
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

    ঘূৰাই (info->error_গুনতি == 0);
}

স্থির bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ধ্রুৱ ৱর্ণ *field,
               ৰাশিমালা_Def *arrayNdx, ৰাশিমালা_Def *মান)
{
    যদি (istreq(field, "action")) {
        যদি (arrayNdx)
            ঘূৰাই ReportSINotArray(info, si, field);

        যদি (মান->common.type == STMT_ৰাশিমালা_ACTION_LIST) {
            unsigned পূর্ণাঙ্ক অঙ্ক_actions = 0;
            বাবে (ৰাশিমালা_Def *act = মান->actions.actions;
                 act; act = (ৰাশিমালা_Def *) act->common.next)
                 অঙ্ক_actions++;

            যদি (অঙ্ক_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, অঙ্ক_actions);
                ঘূৰাই false;
            }

            si->interp.অঙ্ক_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(সঙ্ঘ xkb_action) actions = darray_new();
            বাবে (ৰাশিমালা_Def *act = মান->actions.actions;
                 act; act = (ৰাশিমালা_Def *) act->common.next) {
                সঙ্ঘ xkb_action toAct = { 0 };
                ৱিকল্প (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                পক্ষ PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    ভঙ্গ;
                পক্ষ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ঘূৰাই false;
                স্ৱভাৱ:
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
            ৱিকল্প (darray_size(actions)) {
            পক্ষ 0:
                /* No action or exactly one action: already processed */
                প্রতিজ্ঞা(si->interp.অঙ্ক_actions <= 1);
                ভঙ্গ;
            পক্ষ 1:
                /* One action: some actions were dropped */
                si->interp.অঙ্ক_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                ভঙ্গ;
            স্ৱভাৱ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.অঙ্ক_actions =
                    (xkb_action_গুনতি_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, রিক্ত);
            }
        }
        অন্যথা {
            ৱিকল্প (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, মান, &si->interp.a.action)) {
            পক্ষ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.অঙ্ক_actions = 0;
                ভঙ্গ;
            পক্ষ PARSER_FATAL_ERROR:
                ঘূৰাই false;
            স্ৱভাৱ:
                si->interp.অঙ্ক_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    অন্যথা যদি (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        যদি (arrayNdx)
            ঘূৰাই ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        যদি (!ৰাশিমালা_Resolve_Mod(info->ctx, মান, MOD_VIRT, &info->mods, &ndx))
            ঘূৰাই ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    অন্যথা যদি (istreq(field, "repeat")) {
        bool set = false;

        যদি (arrayNdx)
            ঘূৰাই ReportSINotArray(info, si, field);

        যদি (!ৰাশিমালা_Resolve_Boolean(info->ctx, মান, &set))
            ঘূৰাই ReportSIBadType(info, si, field, "boolean");

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
            ঘূৰাই ReportSINotArray(info, si, field);

        যদি (!ৰাশিমালা_Resolve_Enum(info->ctx, মান, &val, use_Mod_Map_মান_Names))
            ঘূৰাই ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    অন্যথা {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ঘূৰাই !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ঘূৰাই true;
}

স্থির bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ধ্রুৱ ৱর্ণ *field,
               ৰাশিমালা_Def *arrayNdx, ৰাশিমালা_Def **মান_ptr)
{
    ৰাশিমালা_Def * restrict ধ্রুৱ মান = *মান_ptr;

    যদি (istreq(field, "modifiers") || istreq(field, "mods")) {
        যদি (arrayNdx)
            ঘূৰাই ReportLedNotArray(info, ledi, field);

        যদি (!ৰাশিমালা_Resolve_Mod_Mask(info->ctx, মান, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ঘূৰাই ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    অন্যথা যদি (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        যদি (arrayNdx)
            ঘূৰাই ReportLedNotArray(info, ledi, field);

        bool pending = false;
        যদি (!ৰাশিমালা_Resolve_Group_Mask(info->keymap_info, মান, &mask, &pending)) {
            যদি (pending) {
                ledi->led.pending_groups = true;
                ধ্রুৱ darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (সংরচনা pending_computation) {
                        .ৰাশিমালা = *মান_ptr,
                        .computed = false,
                        .মান = 0,
                    }
                );
                *মান_ptr = রিক্ত;
                static_assert(পরিমাণ(pending_index) == পরিমাণ(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } অন্যথা {
                ঘূৰাই ReportLedBadType(info, ledi, field, "group mask");
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
            ঘূৰাই ReportLedNotArray(info, ledi, field);

        ধ্রুৱ uint8_t offset = info->keymap_info->features.controls_name_offset;
        যদি (!ৰাশিমালা_Resolve_Mask(info->ctx, মান, &mask, ctrlMaskNames + offset))
            ঘূৰাই ReportLedBadType(info, ledi, field, "controls mask");

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
            ঘূৰাই ReportLedNotArray(info, ledi, field);

        যদি (!ৰাশিমালা_Resolve_Mask(info->ctx, মান, &mask,
                             modComponentMaskNames))
            ঘূৰাই ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    অন্যথা যদি (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        যদি (arrayNdx)
            ঘূৰাই ReportLedNotArray(info, ledi, field);

        যদি (!ৰাশিমালা_Resolve_Mask(info->ctx, মান, &mask,
                             groupComponentMaskNames))
            ঘূৰাই ReportLedBadType(info, ledi, field,
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
        ঘূৰাই !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ঘূৰাই true;
}

স্থির bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ধ্রুৱ ৱর্ণ *elem, *field;
    ৰাশিমালা_Def *ndx;
    bool ret;

    যদি (!ৰাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    অন্যথা যদি (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->মান);
        যদি (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    অন্যথা যদি (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->মান);
        যদি (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
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
        ঘূৰাই !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ঘূৰাই ret;
}

স্থির bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ধ্রুৱ ৱর্ণ *elem, *field;
    ৰাশিমালা_Def *arrayNdx;

    বাবে (; def; def = (VarDef *) def->common.next) {
        যদি (!ৰাশিমালা_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            অনুৱর্ত;
        }
        যদি (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            অনুৱর্ত;
        }
        যদি (!SetInterpField(info, si, field, arrayNdx, def->মান))
            ok = false;
    }

    ঘূৰাই ok;
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
        ঘূৰাই false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    যদি (!HandleInterpBody(info, def->def, &si)) {
        info->error_গুনতি++;
        ঘূৰাই false;
    }

    যদি (!AddInterp(info, &si, true)) {
        info->error_গুনতি++;
        ঘূৰাই false;
    }

    ঘূৰাই true;
}

স্থির bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    বাবে (VarDef *var = def->body; var != রিক্ত; var = (VarDef *) var->common.next) {
        ধ্রুৱ ৱর্ণ *elem, *field;
        ৰাশিমালা_Def *arrayNdx;
        যদি (!ৰাশিমালা_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            অনুৱর্ত;
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

    ঘূৰাই ok && AddLedMap(info, &ledi, true);
}

স্থির শূন্য
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    বাবে (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ৱিকল্প (stmt->type) {
        পক্ষ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ভঙ্গ;
        পক্ষ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ভঙ্গ;
        পক্ষ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_UNKNOWN_DECLARATION:
        পক্ষ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ভঙ্গ;
        স্ৱভাৱ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ভঙ্গ;
        }

        যদি (!ok)
            info->error_গুনতি++;

        যদি (info->error_গুনতি > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            ভঙ্গ;
        }
    }
}

/* Temporary struct for CopyInterps. */
সংরচনা collect {
    darray(সংরচনা xkb_sym_interpret) sym_interprets;
};

স্থির শূন্য
CopyInterps(CompatInfo *info, bool need_প্ৰতীক, গণনা xkb_match_operation pred,
            সংরচনা collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        যদি (si->interp.match == pred &&
            (si->interp.sym != XKB_চাবি_No_প্ৰতীক) == need_প্ৰতীক)
            darray_append(collect->sym_interprets, si->interp);
}

স্থির শূন্য
CopyLedMapDefsToKeymap(সংরচনা xkb_keymap *keymap, CompatInfo *info)
{
    বাবে (xkb_led_index_t idx = 0; idx < info->অঙ্ক_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        সংরচনা xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            যদি (led->name == ledi->led.name)
                ভঙ্গ;

        /* Not previously declared; create it with next free index. */
        যদি (i >= keymap->অঙ্ক_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                যদি (led->name == XKB_ATOM_NONE)
                    ভঙ্গ;

            যদি (i >= keymap->অঙ্ক_leds) {
                /* Not place to put it; ignore. */
                যদি (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    অনুৱর্ত;
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
CopyCompatToKeymap(সংরচনা xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    যদি (!darray_empty(info->interps)) {
        সংরচনা collect collect;
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

    ঘূৰাই true;
}

bool
CompileCompatMap(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    যদি (file != রিক্ত)
        HandleCompatMapFile(&info, file);

    যদি (info.error_গুনতি != 0)
        গচ্ছ err_info;

    যদি (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        গচ্ছ err_info;

    ClearCompatInfo(&info);
    ঘূৰাই true;

err_info:
    ClearCompatInfo(&info);
    ঘূৰাই false;
}
