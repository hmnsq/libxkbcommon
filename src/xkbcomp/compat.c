/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "action.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "vmod.h"
#அந்தர்பாவ "util-mem.h"

எண்ணிக்கை si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

வகைவரை கட்டமைப்பு {
    எண்ணிக்கை si_field defined;
    எண்ணிக்கை merge_mode merge;

    கட்டமைப்பு xkb_sym_interpret interp;
} SymInterpInfo;

எண்ணிக்கை led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

வகைவரை கட்டமைப்பு {
    எண்ணிக்கை led_field defined;
    எண்ணிக்கை merge_mode merge;

    கட்டமைப்பு xkb_led led;
} LedInfo;

வகைவரை கட்டமைப்பு {
    எழுத்து *name;
    முழு error_மொத்தம்;
    unsigned முழு include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t இலக்கம்_leds;
    ActionsInfo default_actions;
    கட்டமைப்பு xkb_mod_set mods;

    மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info;
    கட்டமைப்பு xkb_context *ctx;
} CompatInfo;

நிலையான மாறிலி எழுத்து *
siText(SymInterpInfo *si, CompatInfo *info)
{
    எழுத்து *buf = xkb_context_get_buffer(info->ctx, 128);

    எனில் (si == &info->default_interp)
        திரும்பு "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    திரும்பு buf;
}

நிலையான உள்வரி bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, மாறிலி எழுத்து *field)
{
    திரும்பு ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

நிலையான உள்வரி bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, மாறிலி எழுத்து *field,
                மாறிலி எழுத்து *wanted)
{
    திரும்பு ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

நிலையான மாறிலி எழுத்து*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    எனில் (ledi == &info->default_led) {
        ப்ரதிஜ்ஞா(xkb_atom_text(info->ctx, ledi->led.name) == வெற்று);
        திரும்பு "default";
    } இல்லையெனில் {
        ப்ரதிஜ்ஞா(xkb_atom_text(info->ctx, ledi->led.name) != வெற்று);
        திரும்பு xkb_atom_text(info->ctx, ledi->led.name);
    }
}

நிலையான உள்வரி bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, மாறிலி எழுத்து *field,
                 மாறிலி எழுத்து *wanted)
{
    திரும்பு ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

நிலையான உள்வரி bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, மாறிலி எழுத்து *field)
{
    திரும்பு ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

நிலையான உள்வரி வெறுமை
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_சாவி_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_சாவி_REPEAT, "invalid initial value");
}

நிலையான உள்வரி வெறுமை
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

நிலையான வெறுமை
InitCompatInfo(CompatInfo *info, மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
               unsigned முழு include_depth, மாறிலி கட்டமைப்பு xkb_mod_set *mods)
{
    memset(info, 0, அளவு(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

நிலையான வெறுமை
ClearCompatInfo(CompatInfo *info)
{
    விடுவி(info->name);
    darray_free(info->interps);
}

நிலையான SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        எனில் (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            திரும்பு old;

    திரும்பு வெற்று;
}

நிலையான bool
UseNewInterpField(எண்ணிக்கை si_field field, எண்ணிக்கை si_field old, எண்ணிக்கை si_field new,
                  bool clobber, bool report, எண்ணிக்கை si_field *collide)
{
    எனில் (!(old & field))
        திரும்பு (new & field);

    எனில் (new & field) {
        எனில் (report)
            *collide |= field;

        திரும்பு clobber;
    }

    திரும்பு false;
}

நிலையான bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    மாறிலி bool clobber = (new->merge != MERGE_AUGMENT);
    மாறிலி முழு verbosity = xkb_context_get_log_verbosity(info->ctx);
    மாறிலி bool report = (same_file && verbosity > 0) || verbosity > 9;
    எண்ணிக்கை si_field collide = 0;

    எனில் (new->merge == MERGE_REPLACE) {
        எனில் (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        திரும்பு true;
    }

    எனில் (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    எனில் (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        எனில் (old->interp.இலக்கம்_actions > 1) {
                விடுவி(old->interp.a.actions);
            }
            old->interp.இலக்கம்_actions = new->interp.இலக்கம்_actions;
            எனில் (new->interp.இலக்கம்_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (ஒன்றிப்பு xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.இலக்கம்_actions = 0;
            } இல்லையெனில் {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    எனில் (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    எனில் (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    எனில் (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    திரும்பு true;
}

நிலையான bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    எனில் (old)
        திரும்பு MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    திரும்பு true;
}

/***====================================================================***/

// TODO: more lenient?
நிலையான bool
ResolveStateAndPredicate(கோவை_Def *கோவை, எண்ணிக்கை xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    எனில் (கோவை == வெற்று) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        திரும்பு true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    எனில் (கோவை->common.type == STMT_கோவை_ACTION_DECL) {
        மாறிலி எழுத்து *pred_txt = xkb_atom_text(info->ctx, கோவை->action.name);
        unsigned முழு pred = 0;
        எனில் (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !கோவை->action.args || கோவை->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            திரும்பு false;
        }
        *pred_rtrn = (எண்ணிக்கை xkb_match_operation) pred;
        கோவை = கோவை->action.args;
    }
    இல்லையெனில் எனில் (கோவை->common.type == STMT_கோவை_IDENT) {
        மாறிலி எழுத்து *pred_txt = xkb_atom_text(info->ctx, கோவை->ident.ident);
        எனில் (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            திரும்பு true;
        }
    }

    திரும்பு கோவை_Resolve_Mod_Mask(info->ctx, கோவை, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

நிலையான bool
UseNewLEDField(எண்ணிக்கை led_field field, எண்ணிக்கை led_field old, எண்ணிக்கை led_field new,
               bool clobber, bool report, எண்ணிக்கை led_field *collide)
{
    எனில் (!(old & field))
        திரும்பு (new & field);

    எனில் (new & field) {
        எனில் (report)
            *collide |= field;

        திரும்பு clobber;
    }

    திரும்பு false;
}

நிலையான bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    எண்ணிக்கை led_field collide;
    மாறிலி bool clobber = (new->merge != MERGE_AUGMENT);
    மாறிலி முழு verbosity = xkb_context_get_log_verbosity(info->ctx);
    மாறிலி bool report = (same_file && verbosity > 0) || verbosity > 9;

    எனில் (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        திரும்பு true;
    }

    எனில் (new->merge == MERGE_REPLACE) {
        எனில் (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        திரும்பு true;
    }

    collide = 0;
    எனில் (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    எனில் (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    எனில் (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    எனில் (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    திரும்பு true;
}

நிலையான bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    க்கு (xkb_led_index_t i = 0; i < info->இலக்கம்_leds; i++) {
        LedInfo *old = &info->leds[i];

        எனில் (old->led.name != new->led.name)
            தொடர்;

        திரும்பு MergeLedMap(info, old, new, same_file);
    }

    எனில் (info->இலக்கம்_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        திரும்பு false;
    }
    info->leds[info->இலக்கம்_leds++] = *new;
    திரும்பு true;
}

நிலையான வெறுமை
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        எண்ணிக்கை merge_mode merge)
{
    எனில் (from->error_மொத்தம் > 0) {
        into->error_மொத்தம் += from->error_மொத்தம்;
        திரும்பு;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    எனில் (into->name == வெற்று) {
        into->name = steal(&from->name);
    }

    எனில் (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    இல்லையெனில் {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            எனில் (!AddInterp(into, si, false))
                into->error_மொத்தம்++;
        }
    }

    எனில் (into->இலக்கம்_leds == 0) {
        memcpy(into->leds, from->leds, அளவு(*from->leds) * from->இலக்கம்_leds);
        into->இலக்கம்_leds = from->இலக்கம்_leds;
        from->இலக்கம்_leds = 0;
    }
    இல்லையெனில் {
        க்கு (xkb_led_index_t i = 0; i < from->இலக்கம்_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            எனில் (!AddLedMap(into, ledi, false))
                into->error_மொத்தம்++;
        }
    }
}

நிலையான வெறுமை
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

நிலையான bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    எனில் (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_மொத்தம் += 10;
        திரும்பு false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    க்கு (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        எழுத்து path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, அளவு(path));
        எனில் (!file) {
            info->error_மொத்தம் += 10;
            ClearCompatInfo(&included);
            திரும்பு false;
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

    திரும்பு (info->error_மொத்தம் == 0);
}

நிலையான bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, மாறிலி எழுத்து *field,
               கோவை_Def *arrayNdx, கோவை_Def *மதிப்பு)
{
    எனில் (istreq(field, "action")) {
        எனில் (arrayNdx)
            திரும்பு ReportSINotArray(info, si, field);

        எனில் (மதிப்பு->common.type == STMT_கோவை_ACTION_LIST) {
            unsigned முழு இலக்கம்_actions = 0;
            க்கு (கோவை_Def *act = மதிப்பு->actions.actions;
                 act; act = (கோவை_Def *) act->common.next)
                 இலக்கம்_actions++;

            எனில் (இலக்கம்_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, இலக்கம்_actions);
                திரும்பு false;
            }

            si->interp.இலக்கம்_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(ஒன்றிப்பு xkb_action) actions = darray_new();
            க்கு (கோவை_Def *act = மதிப்பு->actions.actions;
                 act; act = (கோவை_Def *) act->common.next) {
                ஒன்றிப்பு xkb_action toAct = { 0 };
                தேர்வு (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                நிலை PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    நிறுத்து;
                நிலை PARSER_FATAL_ERROR:
                    darray_free(actions);
                    திரும்பு false;
                இயல்பு:
                    ;
                }
                எனில் (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } இல்லையெனில் எனில் (likely(இலக்கம்_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.இலக்கம்_actions = 1;
                    si->interp.a.action = toAct;
                } இல்லையெனில் {
                    darray_append(actions, toAct);
                }
            }
            தேர்வு (darray_size(actions)) {
            நிலை 0:
                /* No action or exactly one action: already processed */
                ப்ரதிஜ்ஞா(si->interp.இலக்கம்_actions <= 1);
                நிறுத்து;
            நிலை 1:
                /* One action: some actions were dropped */
                si->interp.இலக்கம்_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                நிறுத்து;
            இயல்பு:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.இலக்கம்_actions =
                    (xkb_action_மொத்தம்_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, வெற்று);
            }
        }
        இல்லையெனில் {
            தேர்வு (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, மதிப்பு, &si->interp.a.action)) {
            நிலை PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.இலக்கம்_actions = 0;
                நிறுத்து;
            நிலை PARSER_FATAL_ERROR:
                திரும்பு false;
            இயல்பு:
                si->interp.இலக்கம்_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    இல்லையெனில் எனில் (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        எனில் (arrayNdx)
            திரும்பு ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        எனில் (!கோவை_Resolve_Mod(info->ctx, மதிப்பு, MOD_VIRT, &info->mods, &ndx))
            திரும்பு ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    இல்லையெனில் எனில் (istreq(field, "repeat")) {
        bool set = false;

        எனில் (arrayNdx)
            திரும்பு ReportSINotArray(info, si, field);

        எனில் (!கோவை_Resolve_Boolean(info->ctx, மதிப்பு, &set))
            திரும்பு ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    இல்லையெனில் எனில் (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    இல்லையெனில் எனில் (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        எனில் (arrayNdx)
            திரும்பு ReportSINotArray(info, si, field);

        எனில் (!கோவை_Resolve_Enum(info->ctx, மதிப்பு, &val, use_Mod_Map_மதிப்பு_Names))
            திரும்பு ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    இல்லையெனில் {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        திரும்பு !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    திரும்பு true;
}

நிலையான bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, மாறிலி எழுத்து *field,
               கோவை_Def *arrayNdx, கோவை_Def **மதிப்பு_ptr)
{
    கோவை_Def * restrict மாறிலி மதிப்பு = *மதிப்பு_ptr;

    எனில் (istreq(field, "modifiers") || istreq(field, "mods")) {
        எனில் (arrayNdx)
            திரும்பு ReportLedNotArray(info, ledi, field);

        எனில் (!கோவை_Resolve_Mod_Mask(info->ctx, மதிப்பு, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            திரும்பு ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    இல்லையெனில் எனில் (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        எனில் (arrayNdx)
            திரும்பு ReportLedNotArray(info, ledi, field);

        bool pending = false;
        எனில் (!கோவை_Resolve_Group_Mask(info->keymap_info, மதிப்பு, &mask, &pending)) {
            எனில் (pending) {
                ledi->led.pending_groups = true;
                மாறிலி darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (கட்டமைப்பு pending_computation) {
                        .கோவை = *மதிப்பு_ptr,
                        .computed = false,
                        .மதிப்பு = 0,
                    }
                );
                *மதிப்பு_ptr = வெற்று;
                static_assert(அளவு(pending_index) == அளவு(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } இல்லையெனில் {
                திரும்பு ReportLedBadType(info, ledi, field, "group mask");
            }
        } இல்லையெனில் {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    இல்லையெனில் எனில் (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        எனில் (arrayNdx)
            திரும்பு ReportLedNotArray(info, ledi, field);

        மாறிலி uint8_t offset = info->keymap_info->features.controls_name_offset;
        எனில் (!கோவை_Resolve_Mask(info->ctx, மதிப்பு, &mask, ctrlMaskNames + offset))
            திரும்பு ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    இல்லையெனில் எனில் (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    இல்லையெனில் எனில் (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        எனில் (arrayNdx)
            திரும்பு ReportLedNotArray(info, ledi, field);

        எனில் (!கோவை_Resolve_Mask(info->ctx, மதிப்பு, &mask,
                             modComponentMaskNames))
            திரும்பு ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    இல்லையெனில் எனில் (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        எனில் (arrayNdx)
            திரும்பு ReportLedNotArray(info, ledi, field);

        எனில் (!கோவை_Resolve_Mask(info->ctx, மதிப்பு, &mask,
                             groupComponentMaskNames))
            திரும்பு ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    இல்லையெனில் எனில் (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    இல்லையெனில் எனில் (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    இல்லையெனில் {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        திரும்பு !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    திரும்பு true;
}

நிலையான bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    மாறிலி எழுத்து *elem, *field;
    கோவை_Def *ndx;
    bool ret;

    எனில் (!கோவை_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    இல்லையெனில் எனில் (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->மதிப்பு);
        எனில் (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    இல்லையெனில் எனில் (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->மதிப்பு);
        எனில் (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    இல்லையெனில் எனில் (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->மதிப்பு, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    இல்லையெனில் {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        திரும்பு !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    திரும்பு ret;
}

நிலையான bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    மாறிலி எழுத்து *elem, *field;
    கோவை_Def *arrayNdx;

    க்கு (; def; def = (VarDef *) def->common.next) {
        எனில் (!கோவை_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            தொடர்;
        }
        எனில் (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            தொடர்;
        }
        எனில் (!SetInterpField(info, si, field, arrayNdx, def->மதிப்பு))
            ok = false;
    }

    திரும்பு ok;
}

நிலையான bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    எண்ணிக்கை xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    எனில் (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        திரும்பு false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    எனில் (!HandleInterpBody(info, def->def, &si)) {
        info->error_மொத்தம்++;
        திரும்பு false;
    }

    எனில் (!AddInterp(info, &si, true)) {
        info->error_மொத்தம்++;
        திரும்பு false;
    }

    திரும்பு true;
}

நிலையான bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    க்கு (VarDef *var = def->body; var != வெற்று; var = (VarDef *) var->common.next) {
        மாறிலி எழுத்து *elem, *field;
        கோவை_Def *arrayNdx;
        எனில் (!கோவை_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            தொடர்;
        }

        எனில் (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        இல்லையெனில் {
            எனில் (!SetLedMapField(info, &ledi, field, arrayNdx, &var->மதிப்பு))
                ok = false;
        }
    }

    திரும்பு ok && AddLedMap(info, &ledi, true);
}

நிலையான வெறுமை
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    விடுவி(info->name);
    info->name = strdup_safe(file->name);

    க்கு (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        தேர்வு (stmt->type) {
        நிலை STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            நிறுத்து;
        நிலை STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            நிறுத்து;
        நிலை STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            நிறுத்து;
        நிலை STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            நிறுத்து;
        நிலை STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            நிறுத்து;
        நிலை STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            நிறுத்து;
        நிலை STMT_UNKNOWN_DECLARATION:
        நிலை STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            நிறுத்து;
        இயல்பு:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            நிறுத்து;
        }

        எனில் (!ok)
            info->error_மொத்தம்++;

        எனில் (info->error_மொத்தம் > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            நிறுத்து;
        }
    }
}

/* Temporary struct for CopyInterps. */
கட்டமைப்பு collect {
    darray(கட்டமைப்பு xkb_sym_interpret) sym_interprets;
};

நிலையான வெறுமை
CopyInterps(CompatInfo *info, bool need_குறியீடு, எண்ணிக்கை xkb_match_operation pred,
            கட்டமைப்பு collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        எனில் (si->interp.match == pred &&
            (si->interp.sym != XKB_சாவி_No_குறியீடு) == need_குறியீடு)
            darray_append(collect->sym_interprets, si->interp);
}

நிலையான வெறுமை
CopyLedMapDefsToKeymap(கட்டமைப்பு xkb_keymap *keymap, CompatInfo *info)
{
    க்கு (xkb_led_index_t idx = 0; idx < info->இலக்கம்_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        கட்டமைப்பு xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            எனில் (led->name == ledi->led.name)
                நிறுத்து;

        /* Not previously declared; create it with next free index. */
        எனில் (i >= keymap->இலக்கம்_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                எனில் (led->name == XKB_ATOM_NONE)
                    நிறுத்து;

            எனில் (i >= keymap->இலக்கம்_leds) {
                /* Not place to put it; ignore. */
                எனில் (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    தொடர்;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->இலக்கம்_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        எனில் (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        எனில் (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

நிலையான bool
CopyCompatToKeymap(கட்டமைப்பு xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    எனில் (!darray_empty(info->interps)) {
        கட்டமைப்பு collect collect;
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
                     &keymap->sym_interprets, &keymap->இலக்கம்_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    திரும்பு true;
}

bool
CompileCompatMap(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    எனில் (file != வெற்று)
        HandleCompatMapFile(&info, file);

    எனில் (info.error_மொத்தம் != 0)
        செல் err_info;

    எனில் (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        செல் err_info;

    ClearCompatInfo(&info);
    திரும்பு true;

err_info:
    ClearCompatInfo(&info);
    திரும்பு false;
}
