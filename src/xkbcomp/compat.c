/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "action.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "vmod.h"
#അന്തര്ഭാവ "util-mem.h"

എണ്ണൽ si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

തരനിർവചനം ഘടന {
    എണ്ണൽ si_field defined;
    എണ്ണൽ merge_mode merge;

    ഘടന xkb_sym_interpret interp;
} SymInterpInfo;

എണ്ണൽ led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

തരനിർവചനം ഘടന {
    എണ്ണൽ led_field defined;
    എണ്ണൽ merge_mode merge;

    ഘടന xkb_led led;
} LedInfo;

തരനിർവചനം ഘടന {
    അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ error_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t അക്കം_leds;
    ActionsInfo default_actions;
    ഘടന xkb_mod_set mods;

    സ്ഥിരം ഘടന xkb_keymap_info *keymap_info;
    ഘടന xkb_context *ctx;
} CompatInfo;

സ്ഥിതം സ്ഥിരം അക്ഷരം *
siText(SymInterpInfo *si, CompatInfo *info)
{
    അക്ഷരം *buf = xkb_context_get_buffer(info->ctx, 128);

    എങ്കിൽ (si == &info->default_interp)
        തിരികെ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    തിരികെ buf;
}

സ്ഥിതം വരിയിൽ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, സ്ഥിരം അക്ഷരം *field)
{
    തിരികെ ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

സ്ഥിതം വരിയിൽ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, സ്ഥിരം അക്ഷരം *field,
                സ്ഥിരം അക്ഷരം *wanted)
{
    തിരികെ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    എങ്കിൽ (ledi == &info->default_led) {
        പ്രതിജ്ഞാ(xkb_atom_text(info->ctx, ledi->led.name) == ശൂന്യ);
        തിരികെ "default";
    } അല്ലെങ്കിൽ {
        പ്രതിജ്ഞാ(xkb_atom_text(info->ctx, ledi->led.name) != ശൂന്യ);
        തിരികെ xkb_atom_text(info->ctx, ledi->led.name);
    }
}

സ്ഥിതം വരിയിൽ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, സ്ഥിരം അക്ഷരം *field,
                 സ്ഥിരം അക്ഷരം *wanted)
{
    തിരികെ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

സ്ഥിതം വരിയിൽ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, സ്ഥിരം അക്ഷരം *field)
{
    തിരികെ ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

സ്ഥിതം വരിയിൽ ശൂന്യം
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_താക്കോൽ_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_താക്കോൽ_REPEAT, "invalid initial value");
}

സ്ഥിതം വരിയിൽ ശൂന്യം
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

സ്ഥിതം ശൂന്യം
InitCompatInfo(CompatInfo *info, സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
               unsigned പൂർണ്ണസംഖ്യ include_depth, സ്ഥിരം ഘടന xkb_mod_set *mods)
{
    memset(info, 0, വലുപ്പം(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

സ്ഥിതം ശൂന്യം
ClearCompatInfo(CompatInfo *info)
{
    സ്വതന്ത്രം(info->name);
    darray_free(info->interps);
}

സ്ഥിതം SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        എങ്കിൽ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            തിരികെ old;

    തിരികെ ശൂന്യ;
}

സ്ഥിതം bool
UseNewInterpField(എണ്ണൽ si_field field, എണ്ണൽ si_field old, എണ്ണൽ si_field new,
                  bool clobber, bool report, എണ്ണൽ si_field *collide)
{
    എങ്കിൽ (!(old & field))
        തിരികെ (new & field);

    എങ്കിൽ (new & field) {
        എങ്കിൽ (report)
            *collide |= field;

        തിരികെ clobber;
    }

    തിരികെ false;
}

സ്ഥിതം bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    സ്ഥിരം bool clobber = (new->merge != MERGE_AUGMENT);
    സ്ഥിരം പൂർണ്ണസംഖ്യ verbosity = xkb_context_get_log_verbosity(info->ctx);
    സ്ഥിരം bool report = (same_file && verbosity > 0) || verbosity > 9;
    എണ്ണൽ si_field collide = 0;

    എങ്കിൽ (new->merge == MERGE_REPLACE) {
        എങ്കിൽ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        തിരികെ true;
    }

    എങ്കിൽ (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    എങ്കിൽ (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        എങ്കിൽ (old->interp.അക്കം_actions > 1) {
                സ്വതന്ത്രം(old->interp.a.actions);
            }
            old->interp.അക്കം_actions = new->interp.അക്കം_actions;
            എങ്കിൽ (new->interp.അക്കം_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (സംഘം xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.അക്കം_actions = 0;
            } അല്ലെങ്കിൽ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    എങ്കിൽ (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    എങ്കിൽ (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    എങ്കിൽ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    തിരികെ true;
}

സ്ഥിതം bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    എങ്കിൽ (old)
        തിരികെ MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    തിരികെ true;
}

/***====================================================================***/

// TODO: more lenient?
സ്ഥിതം bool
ResolveStateAndPredicate(വ്യഞ്ജകം_Def *വ്യഞ്ജകം, എണ്ണൽ xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    എങ്കിൽ (വ്യഞ്ജകം == ശൂന്യ) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        തിരികെ true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    എങ്കിൽ (വ്യഞ്ജകം->common.type == STMT_വ്യഞ്ജകം_ACTION_DECL) {
        സ്ഥിരം അക്ഷരം *pred_txt = xkb_atom_text(info->ctx, വ്യഞ്ജകം->action.name);
        unsigned പൂർണ്ണസംഖ്യ pred = 0;
        എങ്കിൽ (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !വ്യഞ്ജകം->action.args || വ്യഞ്ജകം->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            തിരികെ false;
        }
        *pred_rtrn = (എണ്ണൽ xkb_match_operation) pred;
        വ്യഞ്ജകം = വ്യഞ്ജകം->action.args;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (വ്യഞ്ജകം->common.type == STMT_വ്യഞ്ജകം_IDENT) {
        സ്ഥിരം അക്ഷരം *pred_txt = xkb_atom_text(info->ctx, വ്യഞ്ജകം->ident.ident);
        എങ്കിൽ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            തിരികെ true;
        }
    }

    തിരികെ വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, വ്യഞ്ജകം, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

സ്ഥിതം bool
UseNewLEDField(എണ്ണൽ led_field field, എണ്ണൽ led_field old, എണ്ണൽ led_field new,
               bool clobber, bool report, എണ്ണൽ led_field *collide)
{
    എങ്കിൽ (!(old & field))
        തിരികെ (new & field);

    എങ്കിൽ (new & field) {
        എങ്കിൽ (report)
            *collide |= field;

        തിരികെ clobber;
    }

    തിരികെ false;
}

സ്ഥിതം bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    എണ്ണൽ led_field collide;
    സ്ഥിരം bool clobber = (new->merge != MERGE_AUGMENT);
    സ്ഥിരം പൂർണ്ണസംഖ്യ verbosity = xkb_context_get_log_verbosity(info->ctx);
    സ്ഥിരം bool report = (same_file && verbosity > 0) || verbosity > 9;

    എങ്കിൽ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        തിരികെ true;
    }

    എങ്കിൽ (new->merge == MERGE_REPLACE) {
        എങ്കിൽ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        തിരികെ true;
    }

    collide = 0;
    എങ്കിൽ (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    എങ്കിൽ (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    എങ്കിൽ (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    എങ്കിൽ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    തിരികെ true;
}

സ്ഥിതം bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    വേണ്ടി (xkb_led_index_t i = 0; i < info->അക്കം_leds; i++) {
        LedInfo *old = &info->leds[i];

        എങ്കിൽ (old->led.name != new->led.name)
            തുടരുക;

        തിരികെ MergeLedMap(info, old, new, same_file);
    }

    എങ്കിൽ (info->അക്കം_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        തിരികെ false;
    }
    info->leds[info->അക്കം_leds++] = *new;
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        എണ്ണൽ merge_mode merge)
{
    എങ്കിൽ (from->error_എണ്ണം > 0) {
        into->error_എണ്ണം += from->error_എണ്ണം;
        തിരികെ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    എങ്കിൽ (into->name == ശൂന്യ) {
        into->name = steal(&from->name);
    }

    എങ്കിൽ (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    അല്ലെങ്കിൽ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            എങ്കിൽ (!AddInterp(into, si, false))
                into->error_എണ്ണം++;
        }
    }

    എങ്കിൽ (into->അക്കം_leds == 0) {
        memcpy(into->leds, from->leds, വലുപ്പം(*from->leds) * from->അക്കം_leds);
        into->അക്കം_leds = from->അക്കം_leds;
        from->അക്കം_leds = 0;
    }
    അല്ലെങ്കിൽ {
        വേണ്ടി (xkb_led_index_t i = 0; i < from->അക്കം_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            എങ്കിൽ (!AddLedMap(into, ledi, false))
                into->error_എണ്ണം++;
        }
    }
}

സ്ഥിതം ശൂന്യം
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

സ്ഥിതം bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    എങ്കിൽ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_എണ്ണം += 10;
        തിരികെ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    വേണ്ടി (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        അക്ഷരം path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, വലുപ്പം(path));
        എങ്കിൽ (!file) {
            info->error_എണ്ണം += 10;
            ClearCompatInfo(&included);
            തിരികെ false;
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

    തിരികെ (info->error_എണ്ണം == 0);
}

സ്ഥിതം bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, സ്ഥിരം അക്ഷരം *field,
               വ്യഞ്ജകം_Def *arrayNdx, വ്യഞ്ജകം_Def *മൂല്യം)
{
    എങ്കിൽ (istreq(field, "action")) {
        എങ്കിൽ (arrayNdx)
            തിരികെ ReportSINotArray(info, si, field);

        എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_ACTION_LIST) {
            unsigned പൂർണ്ണസംഖ്യ അക്കം_actions = 0;
            വേണ്ടി (വ്യഞ്ജകം_Def *act = മൂല്യം->actions.actions;
                 act; act = (വ്യഞ്ജകം_Def *) act->common.next)
                 അക്കം_actions++;

            എങ്കിൽ (അക്കം_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, അക്കം_actions);
                തിരികെ false;
            }

            si->interp.അക്കം_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(സംഘം xkb_action) actions = darray_new();
            വേണ്ടി (വ്യഞ്ജകം_Def *act = മൂല്യം->actions.actions;
                 act; act = (വ്യഞ്ജകം_Def *) act->common.next) {
                സംഘം xkb_action toAct = { 0 };
                തിരഞ്ഞെടുപ്പ് (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                അവസ്ഥ PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    നിർത്തുക;
                അവസ്ഥ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    തിരികെ false;
                സ്ഥിരസ്ഥിതി:
                    ;
                }
                എങ്കിൽ (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } അല്ലെങ്കിൽ എങ്കിൽ (likely(അക്കം_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.അക്കം_actions = 1;
                    si->interp.a.action = toAct;
                } അല്ലെങ്കിൽ {
                    darray_append(actions, toAct);
                }
            }
            തിരഞ്ഞെടുപ്പ് (darray_size(actions)) {
            അവസ്ഥ 0:
                /* No action or exactly one action: already processed */
                പ്രതിജ്ഞാ(si->interp.അക്കം_actions <= 1);
                നിർത്തുക;
            അവസ്ഥ 1:
                /* One action: some actions were dropped */
                si->interp.അക്കം_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.അക്കം_actions =
                    (xkb_action_എണ്ണം_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, ശൂന്യ);
            }
        }
        അല്ലെങ്കിൽ {
            തിരഞ്ഞെടുപ്പ് (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, മൂല്യം, &si->interp.a.action)) {
            അവസ്ഥ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.അക്കം_actions = 0;
                നിർത്തുക;
            അവസ്ഥ PARSER_FATAL_ERROR:
                തിരികെ false;
            സ്ഥിരസ്ഥിതി:
                si->interp.അക്കം_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        എങ്കിൽ (arrayNdx)
            തിരികെ ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod(info->ctx, മൂല്യം, MOD_VIRT, &info->mods, &ndx))
            തിരികെ ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "repeat")) {
        bool set = false;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportSINotArray(info, si, field);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Boolean(info->ctx, മൂല്യം, &set))
            തിരികെ ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportSINotArray(info, si, field);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Enum(info->ctx, മൂല്യം, &val, use_Mod_Map_മൂല്യം_Names))
            തിരികെ ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    അല്ലെങ്കിൽ {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        തിരികെ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    തിരികെ true;
}

സ്ഥിതം bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, സ്ഥിരം അക്ഷരം *field,
               വ്യഞ്ജകം_Def *arrayNdx, വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    വ്യഞ്ജകം_Def * restrict സ്ഥിരം മൂല്യം = *മൂല്യം_ptr;

    എങ്കിൽ (istreq(field, "modifiers") || istreq(field, "mods")) {
        എങ്കിൽ (arrayNdx)
            തിരികെ ReportLedNotArray(info, ledi, field);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, മൂല്യം, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            തിരികെ ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportLedNotArray(info, ledi, field);

        bool pending = false;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Group_Mask(info->keymap_info, മൂല്യം, &mask, &pending)) {
            എങ്കിൽ (pending) {
                ledi->led.pending_groups = true;
                സ്ഥിരം darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ഘടന pending_computation) {
                        .വ്യഞ്ജകം = *മൂല്യം_ptr,
                        .computed = false,
                        .മൂല്യം = 0,
                    }
                );
                *മൂല്യം_ptr = ശൂന്യ;
                static_assert(വലുപ്പം(pending_index) == വലുപ്പം(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } അല്ലെങ്കിൽ {
                തിരികെ ReportLedBadType(info, ledi, field, "group mask");
            }
        } അല്ലെങ്കിൽ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportLedNotArray(info, ledi, field);

        സ്ഥിരം uint8_t offset = info->keymap_info->features.controls_name_offset;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mask(info->ctx, മൂല്യം, &mask, ctrlMaskNames + offset))
            തിരികെ ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportLedNotArray(info, ledi, field);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mask(info->ctx, മൂല്യം, &mask,
                             modComponentMaskNames))
            തിരികെ ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        എങ്കിൽ (arrayNdx)
            തിരികെ ReportLedNotArray(info, ledi, field);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mask(info->ctx, മൂല്യം, &mask,
                             groupComponentMaskNames))
            തിരികെ ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    അല്ലെങ്കിൽ {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        തിരികെ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    തിരികെ true;
}

സ്ഥിതം bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *ndx;
    bool ret;

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->മൂല്യം);
        എങ്കിൽ (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->മൂല്യം);
        എങ്കിൽ (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->മൂല്യം, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    അല്ലെങ്കിൽ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        തിരികെ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    തിരികെ ret;
}

സ്ഥിതം bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *arrayNdx;

    വേണ്ടി (; def; def = (VarDef *) def->common.next) {
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            തുടരുക;
        }
        എങ്കിൽ (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            തുടരുക;
        }
        എങ്കിൽ (!SetInterpField(info, si, field, arrayNdx, def->മൂല്യം))
            ok = false;
    }

    തിരികെ ok;
}

സ്ഥിതം bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    എണ്ണൽ xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    എങ്കിൽ (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        തിരികെ false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    എങ്കിൽ (!HandleInterpBody(info, def->def, &si)) {
        info->error_എണ്ണം++;
        തിരികെ false;
    }

    എങ്കിൽ (!AddInterp(info, &si, true)) {
        info->error_എണ്ണം++;
        തിരികെ false;
    }

    തിരികെ true;
}

സ്ഥിതം bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    വേണ്ടി (VarDef *var = def->body; var != ശൂന്യ; var = (VarDef *) var->common.next) {
        സ്ഥിരം അക്ഷരം *elem, *field;
        വ്യഞ്ജകം_Def *arrayNdx;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            തുടരുക;
        }

        എങ്കിൽ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        അല്ലെങ്കിൽ {
            എങ്കിൽ (!SetLedMapField(info, &ledi, field, arrayNdx, &var->മൂല്യം))
                ok = false;
        }
    }

    തിരികെ ok && AddLedMap(info, &ledi, true);
}

സ്ഥിതം ശൂന്യം
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    സ്വതന്ത്രം(info->name);
    info->name = strdup_safe(file->name);

    വേണ്ടി (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        തിരഞ്ഞെടുപ്പ് (stmt->type) {
        അവസ്ഥ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            നിർത്തുക;
        അവസ്ഥ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_UNKNOWN_DECLARATION:
        അവസ്ഥ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            നിർത്തുക;
        }

        എങ്കിൽ (!ok)
            info->error_എണ്ണം++;

        എങ്കിൽ (info->error_എണ്ണം > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            നിർത്തുക;
        }
    }
}

/* Temporary struct for CopyInterps. */
ഘടന collect {
    darray(ഘടന xkb_sym_interpret) sym_interprets;
};

സ്ഥിതം ശൂന്യം
CopyInterps(CompatInfo *info, bool need_ചിഹ്നം, എണ്ണൽ xkb_match_operation pred,
            ഘടന collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        എങ്കിൽ (si->interp.match == pred &&
            (si->interp.sym != XKB_താക്കോൽ_No_ചിഹ്നം) == need_ചിഹ്നം)
            darray_append(collect->sym_interprets, si->interp);
}

സ്ഥിതം ശൂന്യം
CopyLedMapDefsToKeymap(ഘടന xkb_keymap *keymap, CompatInfo *info)
{
    വേണ്ടി (xkb_led_index_t idx = 0; idx < info->അക്കം_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        ഘടന xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            എങ്കിൽ (led->name == ledi->led.name)
                നിർത്തുക;

        /* Not previously declared; create it with next free index. */
        എങ്കിൽ (i >= keymap->അക്കം_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                എങ്കിൽ (led->name == XKB_ATOM_NONE)
                    നിർത്തുക;

            എങ്കിൽ (i >= keymap->അക്കം_leds) {
                /* Not place to put it; ignore. */
                എങ്കിൽ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    തുടരുക;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->അക്കം_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        എങ്കിൽ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        എങ്കിൽ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

സ്ഥിതം bool
CopyCompatToKeymap(ഘടന xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    എങ്കിൽ (!darray_empty(info->interps)) {
        ഘടന collect collect;
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
                     &keymap->sym_interprets, &keymap->അക്കം_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    തിരികെ true;
}

bool
CompileCompatMap(XkbFile *file, ഘടന xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    എങ്കിൽ (file != ശൂന്യ)
        HandleCompatMapFile(&info, file);

    എങ്കിൽ (info.error_എണ്ണം != 0)
        പോകുക err_info;

    എങ്കിൽ (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        പോകുക err_info;

    ClearCompatInfo(&info);
    തിരികെ true;

err_info:
    ClearCompatInfo(&info);
    തിരികെ false;
}
