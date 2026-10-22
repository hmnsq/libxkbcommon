/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#include "config.h"

#include <assert.h>
#include <limits.h>

#include "xkbcommon/xkbcommon.h"
#include "darray.h"
#include "keymap.h"
#include "xkbcomp-priv.h"
#include "ast.h"
#include "action.h"
#include "expr.h"
#include "include.h"
#include "text.h"
#include "vmod.h"
#include "util-mem.h"

ga_nanaa si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

typedef sa_nracanaa {
    ga_nanaa si_field defined;
    ga_nanaa merge_mode merge;

    sa_nracanaa xkb_sym_interpret interp;
} SymInterpInfo;

ga_nanaa led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

typedef sa_nracanaa {
    ga_nanaa led_field defined;
    ga_nanaa merge_mode merge;

    sa_nracanaa xkb_led led;
} LedInfo;

typedef sa_nracanaa {
    var_na *name;
    puur_naa_nka errorCount;
    unsigned puur_naa_nka include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t num_leds;
    ActionsInfo default_actions;
    sa_nracanaa xkb_mod_set mods;

    const sa_nracanaa xkb_keymap_info *keymap_info;
    sa_nracanaa xkb_context *ctx;
} CompatInfo;

static const var_na *
siText(SymInterpInfo *si, CompatInfo *info)
{
    var_na *buf = xkb_context_get_buffer(info->ctx, 128);

    ᰀᰧ (si == &info->default_interp)
        ᰀᰧᰌ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ᰀᰧᰌ buf;
}

static anta_hstha bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, const var_na *field)
{
    ᰀᰧᰌ ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

static anta_hstha bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, const var_na *field,
                const var_na *wanted)
{
    ᰀᰧᰌ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

static const var_na*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ᰀᰧ (ledi == &info->default_led) {
        assert(xkb_atom_text(info->ctx, ledi->led.name) == NULL);
        ᰀᰧᰌ "default";
    } ᰀᰧᰀ {
        assert(xkb_atom_text(info->ctx, ledi->led.name) != NULL);
        ᰀᰧᰌ xkb_atom_text(info->ctx, ledi->led.name);
    }
}

static anta_hstha bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, const var_na *field,
                 const var_na *wanted)
{
    ᰀᰧᰌ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

static anta_hstha bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, const var_na *field)
{
    ᰀᰧᰌ ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

static anta_hstha void
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_KEY_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_KEY_REPEAT, "invalid initial value");
}

static anta_hstha void
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

static void
InitCompatInfo(CompatInfo *info, const sa_nracanaa xkb_keymap_info *keymap_info,
               unsigned puur_naa_nka include_depth, const sa_nracanaa xkb_mod_set *mods)
{
    memset(info, 0, parimaa_na(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

static void
ClearCompatInfo(CompatInfo *info)
{
    free(info->name);
    darray_free(info->interps);
}

static SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ᰀᰧ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            ᰀᰧᰌ old;

    ᰀᰧᰌ NULL;
}

static bool
UseNewInterpField(ga_nanaa si_field field, ga_nanaa si_field old, ga_nanaa si_field new,
                  bool clobber, bool report, ga_nanaa si_field *collide)
{
    ᰀᰧ (!(old & field))
        ᰀᰧᰌ (new & field);

    ᰀᰧ (new & field) {
        ᰀᰧ (report)
            *collide |= field;

        ᰀᰧᰌ clobber;
    }

    ᰀᰧᰌ false;
}

static bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    const bool clobber = (new->merge != MERGE_AUGMENT);
    const puur_naa_nka verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;
    ga_nanaa si_field collide = 0;

    ᰀᰧ (new->merge == MERGE_REPLACE) {
        ᰀᰧ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ᰀᰧᰌ true;
    }

    ᰀᰧ (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ᰀᰧ (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ᰀᰧ (old->interp.num_actions > 1) {
                free(old->interp.a.actions);
            }
            old->interp.num_actions = new->interp.num_actions;
            ᰀᰧ (new->interp.num_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (sa_ngha xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.num_actions = 0;
            } ᰀᰧᰀ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    ᰀᰧ (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ᰀᰧ (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    ᰀᰧ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    ᰀᰧᰌ true;
}

static bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    ᰀᰧ (old)
        ᰀᰧᰌ MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ᰀᰧᰌ true;
}

/***====================================================================***/

// TODO: more lenient?
static bool
ResolveStateAndPredicate(ExprDef *expr, ga_nanaa xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    ᰀᰧ (expr == NULL) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ᰀᰧᰌ true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    ᰀᰧ (expr->common.type == STMT_EXPR_ACTION_DECL) {
        const var_na *pred_txt = xkb_atom_text(info->ctx, expr->action.name);
        unsigned puur_naa_nka pred = 0;
        ᰀᰧ (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !expr->action.args || expr->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ᰀᰧᰌ false;
        }
        *pred_rtrn = (ga_nanaa xkb_match_operation) pred;
        expr = expr->action.args;
    }
    ᰀᰧᰀ ᰀᰧ (expr->common.type == STMT_EXPR_IDENT) {
        const var_na *pred_txt = xkb_atom_text(info->ctx, expr->ident.ident);
        ᰀᰧ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ᰀᰧᰌ true;
        }
    }

    ᰀᰧᰌ ExprResolveModMask(info->ctx, expr, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

static bool
UseNewLEDField(ga_nanaa led_field field, ga_nanaa led_field old, ga_nanaa led_field new,
               bool clobber, bool report, ga_nanaa led_field *collide)
{
    ᰀᰧ (!(old & field))
        ᰀᰧᰌ (new & field);

    ᰀᰧ (new & field) {
        ᰀᰧ (report)
            *collide |= field;

        ᰀᰧᰌ clobber;
    }

    ᰀᰧᰌ false;
}

static bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ga_nanaa led_field collide;
    const bool clobber = (new->merge != MERGE_AUGMENT);
    const puur_naa_nka verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;

    ᰀᰧ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ᰀᰧᰌ true;
    }

    ᰀᰧ (new->merge == MERGE_REPLACE) {
        ᰀᰧ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ᰀᰧᰌ true;
    }

    collide = 0;
    ᰀᰧ (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    ᰀᰧ (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    ᰀᰧ (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    ᰀᰧ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ᰀᰧᰌ true;
}

static bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    for (xkb_led_index_t i = 0; i < info->num_leds; i++) {
        LedInfo *old = &info->leds[i];

        ᰀᰧ (old->led.name != new->led.name)
            continue;

        ᰀᰧᰌ MergeLedMap(info, old, new, same_file);
    }

    ᰀᰧ (info->num_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ᰀᰧᰌ false;
    }
    info->leds[info->num_leds++] = *new;
    ᰀᰧᰌ true;
}

static void
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        ga_nanaa merge_mode merge)
{
    ᰀᰧ (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ᰀᰧᰌ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ᰀᰧ (into->name == NULL) {
        into->name = steal(&from->name);
    }

    ᰀᰧ (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ᰀᰧᰀ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            ᰀᰧ (!AddInterp(into, si, false))
                into->errorCount++;
        }
    }

    ᰀᰧ (into->num_leds == 0) {
        memcpy(into->leds, from->leds, parimaa_na(*from->leds) * from->num_leds);
        into->num_leds = from->num_leds;
        from->num_leds = 0;
    }
    ᰀᰧᰀ {
        for (xkb_led_index_t i = 0; i < from->num_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            ᰀᰧ (!AddLedMap(into, ledi, false))
                into->errorCount++;
        }
    }
}

static void
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

static bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ᰀᰧ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ᰀᰧᰌ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    const sa_nracanaa parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    for (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        var_na path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, parimaa_na(path));
        ᰀᰧ (!file) {
            info->errorCount += 10;
            ClearCompatInfo(&included);
            ᰀᰧᰌ false;
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

    ᰀᰧᰌ (info->errorCount == 0);
}

static bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, const var_na *field,
               ExprDef *arrayNdx, ExprDef *value)
{
    ᰀᰧ (istreq(field, "action")) {
        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportSINotArray(info, si, field);

        ᰀᰧ (value->common.type == STMT_EXPR_ACTION_LIST) {
            unsigned puur_naa_nka num_actions = 0;
            for (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next)
                 num_actions++;

            ᰀᰧ (num_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, num_actions);
                ᰀᰧᰌ false;
            }

            si->interp.num_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(sa_ngha xkb_action) actions = darray_new();
            for (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next) {
                sa_ngha xkb_action toAct = { 0 };
                switch (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                pak_sa PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    bha_nga;
                pak_sa PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ᰀᰧᰌ false;
                default:
                    ;
                }
                ᰀᰧ (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ᰀᰧᰀ ᰀᰧ (likely(num_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.num_actions = 1;
                    si->interp.a.action = toAct;
                } ᰀᰧᰀ {
                    darray_append(actions, toAct);
                }
            }
            switch (darray_size(actions)) {
            pak_sa 0:
                /* No action or exactly one action: already processed */
                assert(si->interp.num_actions <= 1);
                bha_nga;
            pak_sa 1:
                /* One action: some actions were dropped */
                si->interp.num_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                bha_nga;
            default:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.num_actions =
                    (xkb_action_count_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, NULL);
            }
        }
        ᰀᰧᰀ {
            switch (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, value, &si->interp.a.action)) {
            pak_sa PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.num_actions = 0;
                bha_nga;
            pak_sa PARSER_FATAL_ERROR:
                ᰀᰧᰌ false;
            default:
                si->interp.num_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        ᰀᰧ (!ExprResolveMod(info->ctx, value, MOD_VIRT, &info->mods, &ndx))
            ᰀᰧᰌ ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "repeat")) {
        bool set = false;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportSINotArray(info, si, field);

        ᰀᰧ (!ExprResolveBoolean(info->ctx, value, &set))
            ᰀᰧᰌ ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportSINotArray(info, si, field);

        ᰀᰧ (!ExprResolveEnum(info->ctx, value, &val, useModMapValueNames))
            ᰀᰧᰌ ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    ᰀᰧᰀ {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ᰀᰧᰌ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ᰀᰧᰌ true;
}

static bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, const var_na *field,
               ExprDef *arrayNdx, ExprDef **value_ptr)
{
    ExprDef * restrict const value = *value_ptr;

    ᰀᰧ (istreq(field, "modifiers") || istreq(field, "mods")) {
        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportLedNotArray(info, ledi, field);

        ᰀᰧ (!ExprResolveModMask(info->ctx, value, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ᰀᰧᰌ ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportLedNotArray(info, ledi, field);

        bool pending = false;
        ᰀᰧ (!ExprResolveGroupMask(info->keymap_info, value, &mask, &pending)) {
            ᰀᰧ (pending) {
                ledi->led.pending_groups = true;
                const darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (sa_nracanaa pending_computation) {
                        .expr = *value_ptr,
                        .computed = false,
                        .value = 0,
                    }
                );
                *value_ptr = NULL;
                static_assert(parimaa_na(pending_index) == parimaa_na(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } ᰀᰧᰀ {
                ᰀᰧᰌ ReportLedBadType(info, ledi, field, "group mask");
            }
        } ᰀᰧᰀ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportLedNotArray(info, ledi, field);

        const uint8_t offset = info->keymap_info->features.controls_name_offset;
        ᰀᰧ (!ExprResolveMask(info->ctx, value, &mask, ctrlMaskNames + offset))
            ᰀᰧᰌ ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportLedNotArray(info, ledi, field);

        ᰀᰧ (!ExprResolveMask(info->ctx, value, &mask,
                             modComponentMaskNames))
            ᰀᰧᰌ ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        ᰀᰧ (arrayNdx)
            ᰀᰧᰌ ReportLedNotArray(info, ledi, field);

        ᰀᰧ (!ExprResolveMask(info->ctx, value, &mask,
                             groupComponentMaskNames))
            ᰀᰧᰌ ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    ᰀᰧᰀ ᰀᰧ (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ᰀᰧᰀ {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        ᰀᰧᰌ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ᰀᰧᰌ true;
}

static bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    const var_na *elem, *field;
    ExprDef *ndx;
    bool ret;

    ᰀᰧ (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    ᰀᰧᰀ ᰀᰧ (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->value);
        ᰀᰧ (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    ᰀᰧᰀ ᰀᰧ (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->value);
        ᰀᰧ (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    ᰀᰧᰀ ᰀᰧ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->value, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    ᰀᰧᰀ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ᰀᰧᰌ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ᰀᰧᰌ ret;
}

static bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    const var_na *elem, *field;
    ExprDef *arrayNdx;

    for (; def; def = (VarDef *) def->common.next) {
        ᰀᰧ (!ExprResolveLhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            continue;
        }
        ᰀᰧ (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            continue;
        }
        ᰀᰧ (!SetInterpField(info, si, field, arrayNdx, def->value))
            ok = false;
    }

    ᰀᰧᰌ ok;
}

static bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ga_nanaa xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    ᰀᰧ (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ᰀᰧᰌ false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    ᰀᰧ (!HandleInterpBody(info, def->def, &si)) {
        info->errorCount++;
        ᰀᰧᰌ false;
    }

    ᰀᰧ (!AddInterp(info, &si, true)) {
        info->errorCount++;
        ᰀᰧᰌ false;
    }

    ᰀᰧᰌ true;
}

static bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    for (VarDef *var = def->body; var != NULL; var = (VarDef *) var->common.next) {
        const var_na *elem, *field;
        ExprDef *arrayNdx;
        ᰀᰧ (!ExprResolveLhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            continue;
        }

        ᰀᰧ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        ᰀᰧᰀ {
            ᰀᰧ (!SetLedMapField(info, &ledi, field, arrayNdx, &var->value))
                ok = false;
        }
    }

    ᰀᰧᰌ ok && AddLedMap(info, &ledi, true);
}

static void
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    for (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        pak_sa STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            bha_nga;
        pak_sa STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            bha_nga;
        pak_sa STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            bha_nga;
        pak_sa STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            bha_nga;
        pak_sa STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            bha_nga;
        pak_sa STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            bha_nga;
        pak_sa STMT_UNKNOWN_DECLARATION:
        pak_sa STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            bha_nga;
        default:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            bha_nga;
        }

        ᰀᰧ (!ok)
            info->errorCount++;

        ᰀᰧ (info->errorCount > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            bha_nga;
        }
    }
}

/* Temporary struct for CopyInterps. */
sa_nracanaa collect {
    darray(sa_nracanaa xkb_sym_interpret) sym_interprets;
};

static void
CopyInterps(CompatInfo *info, bool needSymbol, ga_nanaa xkb_match_operation pred,
            sa_nracanaa collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ᰀᰧ (si->interp.match == pred &&
            (si->interp.sym != XKB_KEY_NoSymbol) == needSymbol)
            darray_append(collect->sym_interprets, si->interp);
}

static void
CopyLedMapDefsToKeymap(sa_nracanaa xkb_keymap *keymap, CompatInfo *info)
{
    for (xkb_led_index_t idx = 0; idx < info->num_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        sa_nracanaa xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ᰀᰧ (led->name == ledi->led.name)
                bha_nga;

        /* Not previously declared; create it with next free index. */
        ᰀᰧ (i >= keymap->num_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ᰀᰧ (led->name == XKB_ATOM_NONE)
                    bha_nga;

            ᰀᰧ (i >= keymap->num_leds) {
                /* Not place to put it; ignore. */
                ᰀᰧ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    continue;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->num_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ᰀᰧ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ᰀᰧ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

static bool
CopyCompatToKeymap(sa_nracanaa xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ᰀᰧ (!darray_empty(info->interps)) {
        sa_nracanaa collect collect;
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
                     &keymap->sym_interprets, &keymap->num_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    ᰀᰧᰌ true;
}

bool
CompileCompatMap(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ᰀᰧ (file != NULL)
        HandleCompatMapFile(&info, file);

    ᰀᰧ (info.errorCount != 0)
        goto err_info;

    ᰀᰧ (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearCompatInfo(&info);
    ᰀᰧᰌ true;

err_info:
    ClearCompatInfo(&info);
    ᰀᰧᰌ false;
}
