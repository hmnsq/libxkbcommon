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

enum si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

typedef struct {
    enum si_field defined;
    enum merge_mode merge;

    struct xkb_sym_interpret interp;
} SymInterpInfo;

enum led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

typedef struct {
    enum led_field defined;
    enum merge_mode merge;

    struct xkb_led led;
} LedInfo;

typedef struct {
    char *name;
    số_nguyên errorCount;
    unsigned số_nguyên include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t num_leds;
    ActionsInfo default_actions;
    struct xkb_mod_set mods;

    const struct xkb_keymap_info *keymap_info;
    struct xkb_context *ctx;
} CompatInfo;

static const char *
siText(SymInterpInfo *si, CompatInfo *info)
{
    char *buf = xkb_context_get_buffer(info->ctx, 128);

    nếu (si == &info->default_interp)
        trả_về "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    trả_về buf;
}

static inline bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, const char *field)
{
    trả_về ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

static inline bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, const char *field,
                const char *wanted)
{
    trả_về ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

static const char*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    nếu (ledi == &info->default_led) {
        assert(xkb_atom_text(info->ctx, ledi->led.name) == NULL);
        trả_về "default";
    } không_thì {
        assert(xkb_atom_text(info->ctx, ledi->led.name) != NULL);
        trả_về xkb_atom_text(info->ctx, ledi->led.name);
    }
}

static inline bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, const char *field,
                 const char *wanted)
{
    trả_về ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

static inline bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, const char *field)
{
    trả_về ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

static inline void
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_KEY_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_KEY_REPEAT, "invalid initial value");
}

static inline void
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

static void
InitCompatInfo(CompatInfo *info, const struct xkb_keymap_info *keymap_info,
               unsigned số_nguyên include_depth, const struct xkb_mod_set *mods)
{
    memset(info, 0, sizeof(*info));
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
        nếu (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            trả_về old;

    trả_về NULL;
}

static bool
UseNewInterpField(enum si_field field, enum si_field old, enum si_field new,
                  bool clobber, bool report, enum si_field *collide)
{
    nếu (!(old & field))
        trả_về (new & field);

    nếu (new & field) {
        nếu (report)
            *collide |= field;

        trả_về clobber;
    }

    trả_về false;
}

static bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    const bool clobber = (new->merge != MERGE_AUGMENT);
    const số_nguyên verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;
    enum si_field collide = 0;

    nếu (new->merge == MERGE_REPLACE) {
        nếu (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        trả_về true;
    }

    nếu (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    nếu (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        nếu (old->interp.num_actions > 1) {
                free(old->interp.a.actions);
            }
            old->interp.num_actions = new->interp.num_actions;
            nếu (new->interp.num_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (union xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.num_actions = 0;
            } không_thì {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    nếu (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    nếu (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    nếu (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    trả_về true;
}

static bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    nếu (old)
        trả_về MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    trả_về true;
}

/***====================================================================***/

// TODO: more lenient?
static bool
ResolveStateAndPredicate(ExprDef *expr, enum xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    nếu (expr == NULL) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        trả_về true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    nếu (expr->common.type == STMT_EXPR_ACTION_DECL) {
        const char *pred_txt = xkb_atom_text(info->ctx, expr->action.name);
        unsigned số_nguyên pred = 0;
        nếu (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !expr->action.args || expr->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            trả_về false;
        }
        *pred_rtrn = (enum xkb_match_operation) pred;
        expr = expr->action.args;
    }
    không_thì nếu (expr->common.type == STMT_EXPR_IDENT) {
        const char *pred_txt = xkb_atom_text(info->ctx, expr->ident.ident);
        nếu (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            trả_về true;
        }
    }

    trả_về ExprResolveModMask(info->ctx, expr, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

static bool
UseNewLEDField(enum led_field field, enum led_field old, enum led_field new,
               bool clobber, bool report, enum led_field *collide)
{
    nếu (!(old & field))
        trả_về (new & field);

    nếu (new & field) {
        nếu (report)
            *collide |= field;

        trả_về clobber;
    }

    trả_về false;
}

static bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    enum led_field collide;
    const bool clobber = (new->merge != MERGE_AUGMENT);
    const số_nguyên verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;

    nếu (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        trả_về true;
    }

    nếu (new->merge == MERGE_REPLACE) {
        nếu (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        trả_về true;
    }

    collide = 0;
    nếu (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    nếu (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    nếu (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    nếu (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    trả_về true;
}

static bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    cho (xkb_led_index_t i = 0; i < info->num_leds; i++) {
        LedInfo *old = &info->leds[i];

        nếu (old->led.name != new->led.name)
            continue;

        trả_về MergeLedMap(info, old, new, same_file);
    }

    nếu (info->num_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        trả_về false;
    }
    info->leds[info->num_leds++] = *new;
    trả_về true;
}

static void
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        enum merge_mode merge)
{
    nếu (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        trả_về;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    nếu (into->name == NULL) {
        into->name = steal(&from->name);
    }

    nếu (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    không_thì {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            nếu (!AddInterp(into, si, false))
                into->errorCount++;
        }
    }

    nếu (into->num_leds == 0) {
        memcpy(into->leds, from->leds, sizeof(*from->leds) * from->num_leds);
        into->num_leds = from->num_leds;
        from->num_leds = 0;
    }
    không_thì {
        cho (xkb_led_index_t i = 0; i < from->num_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            nếu (!AddLedMap(into, ledi, false))
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

    nếu (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        trả_về false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    const struct parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    cho (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        char path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, sizeof(path));
        nếu (!file) {
            info->errorCount += 10;
            ClearCompatInfo(&included);
            trả_về false;
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

    trả_về (info->errorCount == 0);
}

static bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, const char *field,
               ExprDef *arrayNdx, ExprDef *value)
{
    nếu (istreq(field, "action")) {
        nếu (arrayNdx)
            trả_về ReportSINotArray(info, si, field);

        nếu (value->common.type == STMT_EXPR_ACTION_LIST) {
            unsigned số_nguyên num_actions = 0;
            cho (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next)
                 num_actions++;

            nếu (num_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, num_actions);
                trả_về false;
            }

            si->interp.num_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(union xkb_action) actions = darray_new();
            cho (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next) {
                union xkb_action toAct = { 0 };
                switch (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                case PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    break;
                case PARSER_FATAL_ERROR:
                    darray_free(actions);
                    trả_về false;
                default:
                    ;
                }
                nếu (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } không_thì nếu (likely(num_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.num_actions = 1;
                    si->interp.a.action = toAct;
                } không_thì {
                    darray_append(actions, toAct);
                }
            }
            switch (darray_size(actions)) {
            case 0:
                /* No action or exactly one action: already processed */
                assert(si->interp.num_actions <= 1);
                break;
            case 1:
                /* One action: some actions were dropped */
                si->interp.num_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                break;
            default:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.num_actions =
                    (xkb_action_count_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, NULL);
            }
        }
        không_thì {
            switch (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, value, &si->interp.a.action)) {
            case PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.num_actions = 0;
                break;
            case PARSER_FATAL_ERROR:
                trả_về false;
            default:
                si->interp.num_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    không_thì nếu (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        nếu (arrayNdx)
            trả_về ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        nếu (!ExprResolveMod(info->ctx, value, MOD_VIRT, &info->mods, &ndx))
            trả_về ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    không_thì nếu (istreq(field, "repeat")) {
        bool set = false;

        nếu (arrayNdx)
            trả_về ReportSINotArray(info, si, field);

        nếu (!ExprResolveBoolean(info->ctx, value, &set))
            trả_về ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    không_thì nếu (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    không_thì nếu (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        nếu (arrayNdx)
            trả_về ReportSINotArray(info, si, field);

        nếu (!ExprResolveEnum(info->ctx, value, &val, useModMapValueNames))
            trả_về ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    không_thì {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        trả_về !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    trả_về true;
}

static bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, const char *field,
               ExprDef *arrayNdx, ExprDef **value_ptr)
{
    ExprDef * restrict const value = *value_ptr;

    nếu (istreq(field, "modifiers") || istreq(field, "mods")) {
        nếu (arrayNdx)
            trả_về ReportLedNotArray(info, ledi, field);

        nếu (!ExprResolveModMask(info->ctx, value, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            trả_về ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    không_thì nếu (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        nếu (arrayNdx)
            trả_về ReportLedNotArray(info, ledi, field);

        bool pending = false;
        nếu (!ExprResolveGroupMask(info->keymap_info, value, &mask, &pending)) {
            nếu (pending) {
                ledi->led.pending_groups = true;
                const darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (struct pending_computation) {
                        .expr = *value_ptr,
                        .computed = false,
                        .value = 0,
                    }
                );
                *value_ptr = NULL;
                static_assert(sizeof(pending_index) == sizeof(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } không_thì {
                trả_về ReportLedBadType(info, ledi, field, "group mask");
            }
        } không_thì {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    không_thì nếu (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        nếu (arrayNdx)
            trả_về ReportLedNotArray(info, ledi, field);

        const uint8_t offset = info->keymap_info->features.controls_name_offset;
        nếu (!ExprResolveMask(info->ctx, value, &mask, ctrlMaskNames + offset))
            trả_về ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    không_thì nếu (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    không_thì nếu (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        nếu (arrayNdx)
            trả_về ReportLedNotArray(info, ledi, field);

        nếu (!ExprResolveMask(info->ctx, value, &mask,
                             modComponentMaskNames))
            trả_về ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    không_thì nếu (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        nếu (arrayNdx)
            trả_về ReportLedNotArray(info, ledi, field);

        nếu (!ExprResolveMask(info->ctx, value, &mask,
                             groupComponentMaskNames))
            trả_về ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    không_thì nếu (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    không_thì nếu (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    không_thì {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        trả_về !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    trả_về true;
}

static bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *ndx;
    bool ret;

    nếu (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    không_thì nếu (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->value);
        nếu (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    không_thì nếu (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->value);
        nếu (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    không_thì nếu (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->value, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    không_thì {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        trả_về !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    trả_về ret;
}

static bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    const char *elem, *field;
    ExprDef *arrayNdx;

    cho (; def; def = (VarDef *) def->common.next) {
        nếu (!ExprResolveLhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            continue;
        }
        nếu (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            continue;
        }
        nếu (!SetInterpField(info, si, field, arrayNdx, def->value))
            ok = false;
    }

    trả_về ok;
}

static bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    enum xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    nếu (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        trả_về false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    nếu (!HandleInterpBody(info, def->def, &si)) {
        info->errorCount++;
        trả_về false;
    }

    nếu (!AddInterp(info, &si, true)) {
        info->errorCount++;
        trả_về false;
    }

    trả_về true;
}

static bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    cho (VarDef *var = def->body; var != NULL; var = (VarDef *) var->common.next) {
        const char *elem, *field;
        ExprDef *arrayNdx;
        nếu (!ExprResolveLhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            continue;
        }

        nếu (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        không_thì {
            nếu (!SetLedMapField(info, &ledi, field, arrayNdx, &var->value))
                ok = false;
        }
    }

    trả_về ok && AddLedMap(info, &ledi, true);
}

static void
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    cho (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            break;
        case STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            break;
        case STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            break;
        case STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            break;
        case STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            break;
        case STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            break;
        case STMT_UNKNOWN_DECLARATION:
        case STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            break;
        default:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            break;
        }

        nếu (!ok)
            info->errorCount++;

        nếu (info->errorCount > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            break;
        }
    }
}

/* Temporary struct for CopyInterps. */
struct collect {
    darray(struct xkb_sym_interpret) sym_interprets;
};

static void
CopyInterps(CompatInfo *info, bool needSymbol, enum xkb_match_operation pred,
            struct collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        nếu (si->interp.match == pred &&
            (si->interp.sym != XKB_KEY_NoSymbol) == needSymbol)
            darray_append(collect->sym_interprets, si->interp);
}

static void
CopyLedMapDefsToKeymap(struct xkb_keymap *keymap, CompatInfo *info)
{
    cho (xkb_led_index_t idx = 0; idx < info->num_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        struct xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            nếu (led->name == ledi->led.name)
                break;

        /* Not previously declared; create it with next free index. */
        nếu (i >= keymap->num_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                nếu (led->name == XKB_ATOM_NONE)
                    break;

            nếu (i >= keymap->num_leds) {
                /* Not place to put it; ignore. */
                nếu (i >= XKB_MAX_LEDS) {
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
        nếu (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        nếu (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

static bool
CopyCompatToKeymap(struct xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    nếu (!darray_empty(info->interps)) {
        struct collect collect;
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

    trả_về true;
}

bool
CompileCompatMap(XkbFile *file, struct xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    nếu (file != NULL)
        HandleCompatMapFile(&info, file);

    nếu (info.errorCount != 0)
        goto err_info;

    nếu (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearCompatInfo(&info);
    trả_về true;

err_info:
    ClearCompatInfo(&info);
    trả_về false;
}
