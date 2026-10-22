/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "xkbcomp-priv.h"
#ضمن "ast.h"
#ضمن "action.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "text.h"
#ضمن "vmod.h"
#ضمن "util-mem.h"

تعداد si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

تعريفنوع بنية {
    تعداد si_field defined;
    تعداد merge_mode merge;

    بنية xkb_sym_interpret interp;
} SymInterpInfo;

تعداد led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

تعريفنوع بنية {
    تعداد led_field defined;
    تعداد merge_mode merge;

    بنية xkb_led led;
} LedInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح errorCount;
    unsigned صحيح include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t num_leds;
    ActionsInfo default_actions;
    بنية xkb_mod_set mods;

    ثابت بنية xkb_keymap_info *keymap_info;
    بنية xkb_context *ctx;
} CompatInfo;

ساكن ثابت حرف *
siText(SymInterpInfo *si, CompatInfo *info)
{
    حرف *buf = xkb_context_get_buffer(info->ctx, 128);

    ܐܢ (si == &info->default_interp)
        ܦܢܐ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ܦܢܐ buf;
}

ساكن مضمن bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field)
{
    ܦܢܐ ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

ساكن مضمن bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field,
                ثابت حرف *wanted)
{
    ܦܢܐ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

ساكن ثابت حرف*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ܐܢ (ledi == &info->default_led) {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) == عدم);
        ܦܢܐ "default";
    } ܐܚܪܢܐ {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) != عدم);
        ܦܢܐ xkb_atom_text(info->ctx, ledi->led.name);
    }
}

ساكن مضمن bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت حرف *field,
                 ثابت حرف *wanted)
{
    ܦܢܐ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

ساكن مضمن bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت حرف *field)
{
    ܦܢܐ ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

ساكن مضمن فراغ
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_KEY_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_KEY_REPEAT, "invalid initial value");
}

ساكن مضمن فراغ
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

ساكن فراغ
InitCompatInfo(CompatInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
               unsigned صحيح include_depth, ثابت بنية xkb_mod_set *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ساكن فراغ
ClearCompatInfo(CompatInfo *info)
{
    حرر(info->name);
    darray_free(info->interps);
}

ساكن SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ܐܢ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            ܦܢܐ old;

    ܦܢܐ عدم;
}

ساكن bool
UseNewInterpField(تعداد si_field field, تعداد si_field old, تعداد si_field new,
                  bool clobber, bool report, تعداد si_field *collide)
{
    ܐܢ (!(old & field))
        ܦܢܐ (new & field);

    ܐܢ (new & field) {
        ܐܢ (report)
            *collide |= field;

        ܦܢܐ clobber;
    }

    ܦܢܐ false;
}

ساكن bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    تعداد si_field collide = 0;

    ܐܢ (new->merge == MERGE_REPLACE) {
        ܐܢ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ܦܢܐ true;
    }

    ܐܢ (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ܐܢ (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ܐܢ (old->interp.num_actions > 1) {
                حرر(old->interp.a.actions);
            }
            old->interp.num_actions = new->interp.num_actions;
            ܐܢ (new->interp.num_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.num_actions = 0;
            } ܐܚܪܢܐ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    ܐܢ (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ܐܢ (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    ܐܢ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    ܦܢܐ true;
}

ساكن bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    ܐܢ (old)
        ܦܢܐ MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ܦܢܐ true;
}

/***====================================================================***/

// TODO: more lenient?
ساكن bool
ResolveStateAndPredicate(ExprDef *expr, تعداد xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    ܐܢ (expr == عدم) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ܦܢܐ true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    ܐܢ (expr->common.type == STMT_EXPR_ACTION_DECL) {
        ثابت حرف *pred_txt = xkb_atom_text(info->ctx, expr->action.name);
        unsigned صحيح pred = 0;
        ܐܢ (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !expr->action.args || expr->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ܦܢܐ false;
        }
        *pred_rtrn = (تعداد xkb_match_operation) pred;
        expr = expr->action.args;
    }
    ܐܚܪܢܐ ܐܢ (expr->common.type == STMT_EXPR_IDENT) {
        ثابت حرف *pred_txt = xkb_atom_text(info->ctx, expr->ident.ident);
        ܐܢ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ܦܢܐ true;
        }
    }

    ܦܢܐ ExprResolveModMask(info->ctx, expr, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ساكن bool
UseNewLEDField(تعداد led_field field, تعداد led_field old, تعداد led_field new,
               bool clobber, bool report, تعداد led_field *collide)
{
    ܐܢ (!(old & field))
        ܦܢܐ (new & field);

    ܐܢ (new & field) {
        ܐܢ (report)
            *collide |= field;

        ܦܢܐ clobber;
    }

    ܦܢܐ false;
}

ساكن bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    تعداد led_field collide;
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    ܐܢ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ܦܢܐ true;
    }

    ܐܢ (new->merge == MERGE_REPLACE) {
        ܐܢ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ܦܢܐ true;
    }

    collide = 0;
    ܐܢ (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    ܐܢ (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    ܐܢ (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    ܐܢ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ܦܢܐ true;
}

ساكن bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    لكل (xkb_led_index_t i = 0; i < info->num_leds; i++) {
        LedInfo *old = &info->leds[i];

        ܐܢ (old->led.name != new->led.name)
            استمر;

        ܦܢܐ MergeLedMap(info, old, new, same_file);
    }

    ܐܢ (info->num_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ܦܢܐ false;
    }
    info->leds[info->num_leds++] = *new;
    ܦܢܐ true;
}

ساكن فراغ
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        تعداد merge_mode merge)
{
    ܐܢ (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ܦܢܐ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ܐܢ (into->name == عدم) {
        into->name = steal(&from->name);
    }

    ܐܢ (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ܐܚܪܢܐ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            ܐܢ (!AddInterp(into, si, false))
                into->errorCount++;
        }
    }

    ܐܢ (into->num_leds == 0) {
        memcpy(into->leds, from->leds, حجم(*from->leds) * from->num_leds);
        into->num_leds = from->num_leds;
        from->num_leds = 0;
    }
    ܐܚܪܢܐ {
        لكل (xkb_led_index_t i = 0; i < from->num_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            ܐܢ (!AddLedMap(into, ledi, false))
                into->errorCount++;
        }
    }
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ساكن bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ܐܢ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ܦܢܐ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, حجم(path));
        ܐܢ (!file) {
            info->errorCount += 10;
            ClearCompatInfo(&included);
            ܦܢܐ false;
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

    ܦܢܐ (info->errorCount == 0);
}

ساكن bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field,
               ExprDef *arrayNdx, ExprDef *value)
{
    ܐܢ (istreq(field, "action")) {
        ܐܢ (arrayNdx)
            ܦܢܐ ReportSINotArray(info, si, field);

        ܐܢ (value->common.type == STMT_EXPR_ACTION_LIST) {
            unsigned صحيح num_actions = 0;
            لكل (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next)
                 num_actions++;

            ܐܢ (num_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, num_actions);
                ܦܢܐ false;
            }

            si->interp.num_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اتحاد xkb_action) actions = darray_new();
            لكل (ExprDef *act = value->actions.actions;
                 act; act = (ExprDef *) act->common.next) {
                اتحاد xkb_action toAct = { 0 };
                بدل (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                حالة PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    اكسر;
                حالة PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ܦܢܐ false;
                افتراضي:
                    ;
                }
                ܐܢ (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ܐܚܪܢܐ ܐܢ (likely(num_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.num_actions = 1;
                    si->interp.a.action = toAct;
                } ܐܚܪܢܐ {
                    darray_append(actions, toAct);
                }
            }
            بدل (darray_size(actions)) {
            حالة 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.num_actions <= 1);
                اكسر;
            حالة 1:
                /* One action: some actions were dropped */
                si->interp.num_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                اكسر;
            افتراضي:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.num_actions =
                    (xkb_action_count_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, عدم);
            }
        }
        ܐܚܪܢܐ {
            بدل (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, value, &si->interp.a.action)) {
            حالة PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.num_actions = 0;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                ܦܢܐ false;
            افتراضي:
                si->interp.num_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        ܐܢ (arrayNdx)
            ܦܢܐ ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        ܐܢ (!ExprResolveMod(info->ctx, value, MOD_VIRT, &info->mods, &ndx))
            ܦܢܐ ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "repeat")) {
        bool set = false;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportSINotArray(info, si, field);

        ܐܢ (!ExprResolveBoolean(info->ctx, value, &set))
            ܦܢܐ ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportSINotArray(info, si, field);

        ܐܢ (!ExprResolveEnum(info->ctx, value, &val, useModMapValueNames))
            ܦܢܐ ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    ܐܚܪܢܐ {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ܦܢܐ true;
}

ساكن bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ثابت حرف *field,
               ExprDef *arrayNdx, ExprDef **value_ptr)
{
    ExprDef * restrict ثابت value = *value_ptr;

    ܐܢ (istreq(field, "modifiers") || istreq(field, "mods")) {
        ܐܢ (arrayNdx)
            ܦܢܐ ReportLedNotArray(info, ledi, field);

        ܐܢ (!ExprResolveModMask(info->ctx, value, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ܦܢܐ ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportLedNotArray(info, ledi, field);

        bool pending = false;
        ܐܢ (!ExprResolveGroupMask(info->keymap_info, value, &mask, &pending)) {
            ܐܢ (pending) {
                ledi->led.pending_groups = true;
                ثابت darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (بنية pending_computation) {
                        .expr = *value_ptr,
                        .computed = false,
                        .value = 0,
                    }
                );
                *value_ptr = عدم;
                static_assert(حجم(pending_index) == حجم(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } ܐܚܪܢܐ {
                ܦܢܐ ReportLedBadType(info, ledi, field, "group mask");
            }
        } ܐܚܪܢܐ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportLedNotArray(info, ledi, field);

        ثابت uint8_t offset = info->keymap_info->features.controls_name_offset;
        ܐܢ (!ExprResolveMask(info->ctx, value, &mask, ctrlMaskNames + offset))
            ܦܢܐ ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportLedNotArray(info, ledi, field);

        ܐܢ (!ExprResolveMask(info->ctx, value, &mask,
                             modComponentMaskNames))
            ܦܢܐ ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        ܐܢ (arrayNdx)
            ܦܢܐ ReportLedNotArray(info, ledi, field);

        ܐܢ (!ExprResolveMask(info->ctx, value, &mask,
                             groupComponentMaskNames))
            ܦܢܐ ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ܐܚܪܢܐ {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ܦܢܐ true;
}

ساكن bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    ExprDef *ndx;
    bool ret;

    ܐܢ (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    ܐܚܪܢܐ ܐܢ (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->value);
        ܐܢ (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    ܐܚܪܢܐ ܐܢ (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->value);
        ܐܢ (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    ܐܚܪܢܐ ܐܢ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->value, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    ܐܚܪܢܐ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ܦܢܐ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ܦܢܐ ret;
}

ساكن bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت حرف *elem, *field;
    ExprDef *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.next) {
        ܐܢ (!ExprResolveLhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            استمر;
        }
        ܐܢ (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            استمر;
        }
        ܐܢ (!SetInterpField(info, si, field, arrayNdx, def->value))
            ok = false;
    }

    ܦܢܐ ok;
}

ساكن bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    تعداد xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    ܐܢ (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ܦܢܐ false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    ܐܢ (!HandleInterpBody(info, def->def, &si)) {
        info->errorCount++;
        ܦܢܐ false;
    }

    ܐܢ (!AddInterp(info, &si, true)) {
        info->errorCount++;
        ܦܢܐ false;
    }

    ܦܢܐ true;
}

ساكن bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    لكل (VarDef *var = def->body; var != عدم; var = (VarDef *) var->common.next) {
        ثابت حرف *elem, *field;
        ExprDef *arrayNdx;
        ܐܢ (!ExprResolveLhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            استمر;
        }

        ܐܢ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        ܐܚܪܢܐ {
            ܐܢ (!SetLedMapField(info, &ledi, field, arrayNdx, &var->value))
                ok = false;
        }
    }

    ܦܢܐ ok && AddLedMap(info, &ledi, true);
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_safe(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            اكسر;
        حالة STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            اكسر;
        حالة STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            اكسر;
        حالة STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            اكسر;
        حالة STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            اكسر;
        }

        ܐܢ (!ok)
            info->errorCount++;

        ܐܢ (info->errorCount > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
        }
    }
}

/* Temporary struct for CopyInterps. */
بنية collect {
    darray(بنية xkb_sym_interpret) sym_interprets;
};

ساكن فراغ
CopyInterps(CompatInfo *info, bool needSymbol, تعداد xkb_match_operation pred,
            بنية collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ܐܢ (si->interp.match == pred &&
            (si->interp.sym != XKB_KEY_NoSymbol) == needSymbol)
            darray_append(collect->sym_interprets, si->interp);
}

ساكن فراغ
CopyLedMapDefsToKeymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    لكل (xkb_led_index_t idx = 0; idx < info->num_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        بنية xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ܐܢ (led->name == ledi->led.name)
                اكسر;

        /* Not previously declared; create it with next free index. */
        ܐܢ (i >= keymap->num_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ܐܢ (led->name == XKB_ATOM_NONE)
                    اكسر;

            ܐܢ (i >= keymap->num_leds) {
                /* Not place to put it; ignore. */
                ܐܢ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    استمر;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->num_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ܐܢ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ܐܢ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ساكن bool
CopyCompatToKeymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ܐܢ (!darray_empty(info->interps)) {
        بنية collect collect;
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

    ܦܢܐ true;
}

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ܐܢ (file != عدم)
        HandleCompatMapFile(&info, file);

    ܐܢ (info.errorCount != 0)
        اذهب err_info;

    ܐܢ (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearCompatInfo(&info);
    ܦܢܐ true;

err_info:
    ClearCompatInfo(&info);
    ܦܢܐ false;
}
