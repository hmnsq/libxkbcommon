/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "xkbcomp-priv.h"
#شامل "ast.h"
#شامل "action.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "text.h"
#شامل "vmod.h"
#شامل "util-mem.h"

شمارش si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

تعریفنوع ساختار {
    شمارش si_field defined;
    شمارش merge_mode merge;

    ساختار xkb_sym_interpret interp;
} SymInterpInfo;

شمارش led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

تعریفنوع ساختار {
    شمارش led_field defined;
    شمارش merge_mode merge;

    ساختار xkb_led led;
} LedInfo;

تعریفنوع ساختار {
    نویسه *name;
    صحیح error_شمار;
    unsigned صحیح include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t رقم_leds;
    ActionsInfo default_actions;
    ساختار xkb_mod_set mods;

    ثابت ساختار xkb_keymap_info *keymap_info;
    ساختار xkb_context *ctx;
} CompatInfo;

ایستا ثابت نویسه *
siText(SymInterpInfo *si, CompatInfo *info)
{
    نویسه *buf = xkb_context_get_buffer(info->ctx, 128);

    اگر (si == &info->default_interp)
        بازگشت "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    بازگشت buf;
}

ایستا درونخطی bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت نویسه *field)
{
    بازگشت ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

ایستا درونخطی bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت نویسه *field,
                ثابت نویسه *wanted)
{
    بازگشت ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

ایستا ثابت نویسه*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    اگر (ledi == &info->default_led) {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) == پوچ);
        بازگشت "default";
    } وگرنه {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) != پوچ);
        بازگشت xkb_atom_text(info->ctx, ledi->led.name);
    }
}

ایستا درونخطی bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت نویسه *field,
                 ثابت نویسه *wanted)
{
    بازگشت ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

ایستا درونخطی bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت نویسه *field)
{
    بازگشت ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

ایستا درونخطی تهی
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_کلید_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_کلید_REPEAT, "invalid initial value");
}

ایستا درونخطی تهی
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

ایستا تهی
InitCompatInfo(CompatInfo *info, ثابت ساختار xkb_keymap_info *keymap_info,
               unsigned صحیح include_depth, ثابت ساختار xkb_mod_set *mods)
{
    memset(info, 0, اندازه(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ایستا تهی
ClearCompatInfo(CompatInfo *info)
{
    آزاد(info->name);
    darray_free(info->interps);
}

ایستا SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        اگر (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            بازگشت old;

    بازگشت پوچ;
}

ایستا bool
UseNewInterpField(شمارش si_field field, شمارش si_field old, شمارش si_field new,
                  bool clobber, bool report, شمارش si_field *collide)
{
    اگر (!(old & field))
        بازگشت (new & field);

    اگر (new & field) {
        اگر (report)
            *collide |= field;

        بازگشت clobber;
    }

    بازگشت false;
}

ایستا bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    شمارش si_field collide = 0;

    اگر (new->merge == MERGE_REPLACE) {
        اگر (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        بازگشت true;
    }

    اگر (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    اگر (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        اگر (old->interp.رقم_actions > 1) {
                آزاد(old->interp.a.actions);
            }
            old->interp.رقم_actions = new->interp.رقم_actions;
            اگر (new->interp.رقم_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اجتماع xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.رقم_actions = 0;
            } وگرنه {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    اگر (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    اگر (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    اگر (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    بازگشت true;
}

ایستا bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    اگر (old)
        بازگشت MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    بازگشت true;
}

/***====================================================================***/

// TODO: more lenient?
ایستا bool
ResolveStateAndPredicate(عبارت_Def *عبارت, شمارش xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    اگر (عبارت == پوچ) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        بازگشت true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    اگر (عبارت->common.type == STMT_عبارت_ACTION_DECL) {
        ثابت نویسه *pred_txt = xkb_atom_text(info->ctx, عبارت->action.name);
        unsigned صحیح pred = 0;
        اگر (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !عبارت->action.args || عبارت->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            بازگشت false;
        }
        *pred_rtrn = (شمارش xkb_match_operation) pred;
        عبارت = عبارت->action.args;
    }
    وگرنه اگر (عبارت->common.type == STMT_عبارت_IDENT) {
        ثابت نویسه *pred_txt = xkb_atom_text(info->ctx, عبارت->ident.ident);
        اگر (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            بازگشت true;
        }
    }

    بازگشت عبارت_Resolve_Mod_Mask(info->ctx, عبارت, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ایستا bool
UseNewLEDField(شمارش led_field field, شمارش led_field old, شمارش led_field new,
               bool clobber, bool report, شمارش led_field *collide)
{
    اگر (!(old & field))
        بازگشت (new & field);

    اگر (new & field) {
        اگر (report)
            *collide |= field;

        بازگشت clobber;
    }

    بازگشت false;
}

ایستا bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    شمارش led_field collide;
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    اگر (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        بازگشت true;
    }

    اگر (new->merge == MERGE_REPLACE) {
        اگر (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        بازگشت true;
    }

    collide = 0;
    اگر (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    اگر (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    اگر (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    اگر (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    بازگشت true;
}

ایستا bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    برای (xkb_led_index_t i = 0; i < info->رقم_leds; i++) {
        LedInfo *old = &info->leds[i];

        اگر (old->led.name != new->led.name)
            ادامه;

        بازگشت MergeLedMap(info, old, new, same_file);
    }

    اگر (info->رقم_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        بازگشت false;
    }
    info->leds[info->رقم_leds++] = *new;
    بازگشت true;
}

ایستا تهی
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        شمارش merge_mode merge)
{
    اگر (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        بازگشت;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    اگر (into->name == پوچ) {
        into->name = steal(&from->name);
    }

    اگر (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    وگرنه {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            اگر (!AddInterp(into, si, false))
                into->error_شمار++;
        }
    }

    اگر (into->رقم_leds == 0) {
        memcpy(into->leds, from->leds, اندازه(*from->leds) * from->رقم_leds);
        into->رقم_leds = from->رقم_leds;
        from->رقم_leds = 0;
    }
    وگرنه {
        برای (xkb_led_index_t i = 0; i < from->رقم_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            اگر (!AddLedMap(into, ledi, false))
                into->error_شمار++;
        }
    }
}

ایستا تهی
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ایستا bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    اگر (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_شمار += 10;
        بازگشت false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساختار parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برای (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        نویسه path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, اندازه(path));
        اگر (!file) {
            info->error_شمار += 10;
            ClearCompatInfo(&included);
            بازگشت false;
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

    بازگشت (info->error_شمار == 0);
}

ایستا bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ثابت نویسه *field,
               عبارت_Def *arrayNdx, عبارت_Def *مقدار)
{
    اگر (istreq(field, "action")) {
        اگر (arrayNdx)
            بازگشت ReportSINotArray(info, si, field);

        اگر (مقدار->common.type == STMT_عبارت_ACTION_LIST) {
            unsigned صحیح رقم_actions = 0;
            برای (عبارت_Def *act = مقدار->actions.actions;
                 act; act = (عبارت_Def *) act->common.next)
                 رقم_actions++;

            اگر (رقم_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, رقم_actions);
                بازگشت false;
            }

            si->interp.رقم_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اجتماع xkb_action) actions = darray_new();
            برای (عبارت_Def *act = مقدار->actions.actions;
                 act; act = (عبارت_Def *) act->common.next) {
                اجتماع xkb_action toAct = { 0 };
                انتخاب (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                حالت PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    بشکن;
                حالت PARSER_FATAL_ERROR:
                    darray_free(actions);
                    بازگشت false;
                پیشفرض:
                    ;
                }
                اگر (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } وگرنه اگر (likely(رقم_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.رقم_actions = 1;
                    si->interp.a.action = toAct;
                } وگرنه {
                    darray_append(actions, toAct);
                }
            }
            انتخاب (darray_size(actions)) {
            حالت 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.رقم_actions <= 1);
                بشکن;
            حالت 1:
                /* One action: some actions were dropped */
                si->interp.رقم_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                بشکن;
            پیشفرض:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.رقم_actions =
                    (xkb_action_شمار_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, پوچ);
            }
        }
        وگرنه {
            انتخاب (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, مقدار, &si->interp.a.action)) {
            حالت PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.رقم_actions = 0;
                بشکن;
            حالت PARSER_FATAL_ERROR:
                بازگشت false;
            پیشفرض:
                si->interp.رقم_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    وگرنه اگر (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        اگر (arrayNdx)
            بازگشت ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        اگر (!عبارت_Resolve_Mod(info->ctx, مقدار, MOD_VIRT, &info->mods, &ndx))
            بازگشت ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    وگرنه اگر (istreq(field, "repeat")) {
        bool set = false;

        اگر (arrayNdx)
            بازگشت ReportSINotArray(info, si, field);

        اگر (!عبارت_Resolve_Boolean(info->ctx, مقدار, &set))
            بازگشت ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    وگرنه اگر (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    وگرنه اگر (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        اگر (arrayNdx)
            بازگشت ReportSINotArray(info, si, field);

        اگر (!عبارت_Resolve_Enum(info->ctx, مقدار, &val, use_Mod_Map_مقدار_Names))
            بازگشت ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    وگرنه {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        بازگشت !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    بازگشت true;
}

ایستا bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ثابت نویسه *field,
               عبارت_Def *arrayNdx, عبارت_Def **مقدار_ptr)
{
    عبارت_Def * restrict ثابت مقدار = *مقدار_ptr;

    اگر (istreq(field, "modifiers") || istreq(field, "mods")) {
        اگر (arrayNdx)
            بازگشت ReportLedNotArray(info, ledi, field);

        اگر (!عبارت_Resolve_Mod_Mask(info->ctx, مقدار, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            بازگشت ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    وگرنه اگر (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        اگر (arrayNdx)
            بازگشت ReportLedNotArray(info, ledi, field);

        bool pending = false;
        اگر (!عبارت_Resolve_Group_Mask(info->keymap_info, مقدار, &mask, &pending)) {
            اگر (pending) {
                ledi->led.pending_groups = true;
                ثابت darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ساختار pending_computation) {
                        .عبارت = *مقدار_ptr,
                        .computed = false,
                        .مقدار = 0,
                    }
                );
                *مقدار_ptr = پوچ;
                static_assert(اندازه(pending_index) == اندازه(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } وگرنه {
                بازگشت ReportLedBadType(info, ledi, field, "group mask");
            }
        } وگرنه {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    وگرنه اگر (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        اگر (arrayNdx)
            بازگشت ReportLedNotArray(info, ledi, field);

        ثابت uint8_t offset = info->keymap_info->features.controls_name_offset;
        اگر (!عبارت_Resolve_Mask(info->ctx, مقدار, &mask, ctrlMaskNames + offset))
            بازگشت ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    وگرنه اگر (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وگرنه اگر (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        اگر (arrayNdx)
            بازگشت ReportLedNotArray(info, ledi, field);

        اگر (!عبارت_Resolve_Mask(info->ctx, مقدار, &mask,
                             modComponentMaskNames))
            بازگشت ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    وگرنه اگر (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        اگر (arrayNdx)
            بازگشت ReportLedNotArray(info, ledi, field);

        اگر (!عبارت_Resolve_Mask(info->ctx, مقدار, &mask,
                             groupComponentMaskNames))
            بازگشت ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    وگرنه اگر (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    وگرنه اگر (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وگرنه {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        بازگشت !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    بازگشت true;
}

ایستا bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت نویسه *elem, *field;
    عبارت_Def *ndx;
    bool ret;

    اگر (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    وگرنه اگر (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->مقدار);
        اگر (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    وگرنه اگر (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->مقدار);
        اگر (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    وگرنه اگر (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->مقدار, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    وگرنه {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        بازگشت !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    بازگشت ret;
}

ایستا bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت نویسه *elem, *field;
    عبارت_Def *arrayNdx;

    برای (; def; def = (VarDef *) def->common.next) {
        اگر (!عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ادامه;
        }
        اگر (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            ادامه;
        }
        اگر (!SetInterpField(info, si, field, arrayNdx, def->مقدار))
            ok = false;
    }

    بازگشت ok;
}

ایستا bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    شمارش xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    اگر (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        بازگشت false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    اگر (!HandleInterpBody(info, def->def, &si)) {
        info->error_شمار++;
        بازگشت false;
    }

    اگر (!AddInterp(info, &si, true)) {
        info->error_شمار++;
        بازگشت false;
    }

    بازگشت true;
}

ایستا bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    برای (VarDef *var = def->body; var != پوچ; var = (VarDef *) var->common.next) {
        ثابت نویسه *elem, *field;
        عبارت_Def *arrayNdx;
        اگر (!عبارت_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ادامه;
        }

        اگر (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        وگرنه {
            اگر (!SetLedMapField(info, &ledi, field, arrayNdx, &var->مقدار))
                ok = false;
        }
    }

    بازگشت ok && AddLedMap(info, &ledi, true);
}

ایستا تهی
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_safe(file->name);

    برای (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            بشکن;
        حالت STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            بشکن;
        حالت STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            بشکن;
        حالت STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            بشکن;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            بشکن;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            بشکن;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            بشکن;
        پیشفرض:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            بشکن;
        }

        اگر (!ok)
            info->error_شمار++;

        اگر (info->error_شمار > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            بشکن;
        }
    }
}

/* Temporary struct for CopyInterps. */
ساختار collect {
    darray(ساختار xkb_sym_interpret) sym_interprets;
};

ایستا تهی
CopyInterps(CompatInfo *info, bool need_نماد, شمارش xkb_match_operation pred,
            ساختار collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        اگر (si->interp.match == pred &&
            (si->interp.sym != XKB_کلید_No_نماد) == need_نماد)
            darray_append(collect->sym_interprets, si->interp);
}

ایستا تهی
CopyLedMapDefsToKeymap(ساختار xkb_keymap *keymap, CompatInfo *info)
{
    برای (xkb_led_index_t idx = 0; idx < info->رقم_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        ساختار xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            اگر (led->name == ledi->led.name)
                بشکن;

        /* Not previously declared; create it with next free index. */
        اگر (i >= keymap->رقم_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                اگر (led->name == XKB_ATOM_NONE)
                    بشکن;

            اگر (i >= keymap->رقم_leds) {
                /* Not place to put it; ignore. */
                اگر (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ادامه;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->رقم_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        اگر (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        اگر (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ایستا bool
CopyCompatToKeymap(ساختار xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    اگر (!darray_empty(info->interps)) {
        ساختار collect collect;
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
                     &keymap->sym_interprets, &keymap->رقم_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    بازگشت true;
}

bool
CompileCompatMap(XkbFile *file, ساختار xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    اگر (file != پوچ)
        HandleCompatMapFile(&info, file);

    اگر (info.error_شمار != 0)
        برو err_info;

    اگر (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        برو err_info;

    ClearCompatInfo(&info);
    بازگشت true;

err_info:
    ClearCompatInfo(&info);
    بازگشت false;
}
