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
    صحيح error_گنتی;
    unsigned صحيح include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t عدد_leds;
    ActionsInfo default_actions;
    بنية xkb_mod_set mods;

    ثابت بنية xkb_keymap_info *keymap_info;
    بنية xkb_context *ctx;
} CompatInfo;

ساكن ثابت حرف *
siText(SymInterpInfo *si, CompatInfo *info)
{
    حرف *buf = xkb_context_get_buffer(info->ctx, 128);

    إذا (si == &info->default_interp)
        أرجع "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    أرجع buf;
}

ساكن مضمن bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field)
{
    أرجع ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

ساكن مضمن bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field,
                ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

ساكن ثابت حرف*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    إذا (ledi == &info->default_led) {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) == عدم);
        أرجع "default";
    } وإلا {
        أكد(xkb_atom_text(info->ctx, ledi->led.name) != عدم);
        أرجع xkb_atom_text(info->ctx, ledi->led.name);
    }
}

ساكن مضمن bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ثابت حرف *field,
                 ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

ساكن مضمن bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ثابت حرف *field)
{
    أرجع ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

ساكن مضمن فراغ
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_کلید_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_کلید_REPEAT, "invalid initial value");
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
        إذا (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            أرجع old;

    أرجع عدم;
}

ساكن bool
UseNewInterpField(تعداد si_field field, تعداد si_field old, تعداد si_field new,
                  bool clobber, bool report, تعداد si_field *collide)
{
    إذا (!(old & field))
        أرجع (new & field);

    إذا (new & field) {
        إذا (report)
            *collide |= field;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;
    تعداد si_field collide = 0;

    إذا (new->merge == MERGE_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        أرجع true;
    }

    إذا (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    إذا (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        إذا (old->interp.عدد_actions > 1) {
                حرر(old->interp.a.actions);
            }
            old->interp.عدد_actions = new->interp.عدد_actions;
            إذا (new->interp.عدد_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.عدد_actions = 0;
            } وإلا {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    إذا (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    إذا (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    إذا (old)
        أرجع MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    أرجع true;
}

/***====================================================================***/

// TODO: more lenient?
ساكن bool
ResolveStateAndPredicate(اظہاریہ_Def *اظہاریہ, تعداد xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    إذا (اظہاریہ == عدم) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        أرجع true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    إذا (اظہاریہ->common.type == STMT_اظہاریہ_ACTION_DECL) {
        ثابت حرف *pred_txt = xkb_atom_text(info->ctx, اظہاریہ->action.name);
        unsigned صحيح pred = 0;
        إذا (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !اظہاریہ->action.args || اظہاریہ->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            أرجع false;
        }
        *pred_rtrn = (تعداد xkb_match_operation) pred;
        اظہاریہ = اظہاریہ->action.args;
    }
    وإلا إذا (اظہاریہ->common.type == STMT_اظہاریہ_IDENT) {
        ثابت حرف *pred_txt = xkb_atom_text(info->ctx, اظہاریہ->ident.ident);
        إذا (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            أرجع true;
        }
    }

    أرجع اظہاریہ_Resolve_Mod_Mask(info->ctx, اظہاریہ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ساكن bool
UseNewLEDField(تعداد led_field field, تعداد led_field old, تعداد led_field new,
               bool clobber, bool report, تعداد led_field *collide)
{
    إذا (!(old & field))
        أرجع (new & field);

    إذا (new & field) {
        إذا (report)
            *collide |= field;

        أرجع clobber;
    }

    أرجع false;
}

ساكن bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    تعداد led_field collide;
    ثابت bool clobber = (new->merge != MERGE_AUGMENT);
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report = (same_file && verbosity > 0) || verbosity > 9;

    إذا (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        أرجع true;
    }

    إذا (new->merge == MERGE_REPLACE) {
        إذا (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        أرجع true;
    }

    collide = 0;
    إذا (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    إذا (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    إذا (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    إذا (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    أرجع true;
}

ساكن bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    لكل (xkb_led_index_t i = 0; i < info->عدد_leds; i++) {
        LedInfo *old = &info->leds[i];

        إذا (old->led.name != new->led.name)
            استمر;

        أرجع MergeLedMap(info, old, new, same_file);
    }

    إذا (info->عدد_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        أرجع false;
    }
    info->leds[info->عدد_leds++] = *new;
    أرجع true;
}

ساكن فراغ
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        تعداد merge_mode merge)
{
    إذا (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        أرجع;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    وإلا {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            إذا (!AddInterp(into, si, false))
                into->error_گنتی++;
        }
    }

    إذا (into->عدد_leds == 0) {
        memcpy(into->leds, from->leds, حجم(*from->leds) * from->عدد_leds);
        into->عدد_leds = from->عدد_leds;
        from->عدد_leds = 0;
    }
    وإلا {
        لكل (xkb_led_index_t i = 0; i < from->عدد_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            إذا (!AddLedMap(into, ledi, false))
                into->error_گنتی++;
        }
    }
}

ساكن فراغ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ساكن bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    إذا (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_گنتی += 10;
        أرجع false;
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
        إذا (!file) {
            info->error_گنتی += 10;
            ClearCompatInfo(&included);
            أرجع false;
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

    أرجع (info->error_گنتی == 0);
}

ساكن bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ثابت حرف *field,
               اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر)
{
    إذا (istreq(field, "action")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, field);

        إذا (قدر->common.type == STMT_اظہاریہ_ACTION_LIST) {
            unsigned صحيح عدد_actions = 0;
            لكل (اظہاریہ_Def *act = قدر->actions.actions;
                 act; act = (اظہاریہ_Def *) act->common.next)
                 عدد_actions++;

            إذا (عدد_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, عدد_actions);
                أرجع false;
            }

            si->interp.عدد_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(اتحاد xkb_action) actions = darray_new();
            لكل (اظہاریہ_Def *act = قدر->actions.actions;
                 act; act = (اظہاریہ_Def *) act->common.next) {
                اتحاد xkb_action toAct = { 0 };
                بدل (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                حالة PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    اكسر;
                حالة PARSER_FATAL_ERROR:
                    darray_free(actions);
                    أرجع false;
                افتراضي:
                    ;
                }
                إذا (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } وإلا إذا (likely(عدد_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.عدد_actions = 1;
                    si->interp.a.action = toAct;
                } وإلا {
                    darray_append(actions, toAct);
                }
            }
            بدل (darray_size(actions)) {
            حالة 0:
                /* No action or exactly one action: already processed */
                أكد(si->interp.عدد_actions <= 1);
                اكسر;
            حالة 1:
                /* One action: some actions were dropped */
                si->interp.عدد_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                اكسر;
            افتراضي:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.عدد_actions =
                    (xkb_action_گنتی_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, عدم);
            }
        }
        وإلا {
            بدل (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, قدر, &si->interp.a.action)) {
            حالة PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.عدد_actions = 0;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                أرجع false;
            افتراضي:
                si->interp.عدد_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    وإلا إذا (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        إذا (!اظہاریہ_Resolve_Mod(info->ctx, قدر, MOD_VIRT, &info->mods, &ndx))
            أرجع ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    وإلا إذا (istreq(field, "repeat")) {
        bool set = false;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, field);

        إذا (!اظہاریہ_Resolve_Boolean(info->ctx, قدر, &set))
            أرجع ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    وإلا إذا (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        إذا (arrayNdx)
            أرجع ReportSINotArray(info, si, field);

        إذا (!اظہاریہ_Resolve_Enum(info->ctx, قدر, &val, use_Mod_Map_قدر_Names))
            أرجع ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    وإلا {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    أرجع true;
}

ساكن bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ثابت حرف *field,
               اظہاریہ_Def *arrayNdx, اظہاریہ_Def **قدر_ptr)
{
    اظہاریہ_Def * restrict ثابت قدر = *قدر_ptr;

    إذا (istreq(field, "modifiers") || istreq(field, "mods")) {
        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, field);

        إذا (!اظہاریہ_Resolve_Mod_Mask(info->ctx, قدر, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            أرجع ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    وإلا إذا (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, field);

        bool pending = false;
        إذا (!اظہاریہ_Resolve_Group_Mask(info->keymap_info, قدر, &mask, &pending)) {
            إذا (pending) {
                ledi->led.pending_groups = true;
                ثابت darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (بنية pending_computation) {
                        .اظہاریہ = *قدر_ptr,
                        .computed = false,
                        .قدر = 0,
                    }
                );
                *قدر_ptr = عدم;
                static_assert(حجم(pending_index) == حجم(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } وإلا {
                أرجع ReportLedBadType(info, ledi, field, "group mask");
            }
        } وإلا {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    وإلا إذا (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, field);

        ثابت uint8_t offset = info->keymap_info->features.controls_name_offset;
        إذا (!اظہاریہ_Resolve_Mask(info->ctx, قدر, &mask, ctrlMaskNames + offset))
            أرجع ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    وإلا إذا (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا إذا (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, field);

        إذا (!اظہاریہ_Resolve_Mask(info->ctx, قدر, &mask,
                             modComponentMaskNames))
            أرجع ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    وإلا إذا (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        إذا (arrayNdx)
            أرجع ReportLedNotArray(info, ledi, field);

        إذا (!اظہاریہ_Resolve_Mask(info->ctx, قدر, &mask,
                             groupComponentMaskNames))
            أرجع ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    وإلا إذا (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    وإلا إذا (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    وإلا {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    أرجع true;
}

ساكن bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    اظہاریہ_Def *ndx;
    bool ret;

    إذا (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    وإلا إذا (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->قدر);
        إذا (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    وإلا إذا (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->قدر);
        إذا (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    وإلا إذا (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->قدر, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    أرجع ret;
}

ساكن bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ثابت حرف *elem, *field;
    اظہاریہ_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.next) {
        إذا (!اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            استمر;
        }
        إذا (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            استمر;
        }
        إذا (!SetInterpField(info, si, field, arrayNdx, def->قدر))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    تعداد xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    إذا (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        أرجع false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    إذا (!HandleInterpBody(info, def->def, &si)) {
        info->error_گنتی++;
        أرجع false;
    }

    إذا (!AddInterp(info, &si, true)) {
        info->error_گنتی++;
        أرجع false;
    }

    أرجع true;
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
        اظہاریہ_Def *arrayNdx;
        إذا (!اظہاریہ_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            استمر;
        }

        إذا (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        وإلا {
            إذا (!SetLedMapField(info, &ledi, field, arrayNdx, &var->قدر))
                ok = false;
        }
    }

    أرجع ok && AddLedMap(info, &ledi, true);
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

        إذا (!ok)
            info->error_گنتی++;

        إذا (info->error_گنتی > 10) {
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
CopyInterps(CompatInfo *info, bool need_علامت, تعداد xkb_match_operation pred,
            بنية collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        إذا (si->interp.match == pred &&
            (si->interp.sym != XKB_کلید_No_علامت) == need_علامت)
            darray_append(collect->sym_interprets, si->interp);
}

ساكن فراغ
CopyLedMapDefsToKeymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    لكل (xkb_led_index_t idx = 0; idx < info->عدد_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        بنية xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            إذا (led->name == ledi->led.name)
                اكسر;

        /* Not previously declared; create it with next free index. */
        إذا (i >= keymap->عدد_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                إذا (led->name == XKB_ATOM_NONE)
                    اكسر;

            إذا (i >= keymap->عدد_leds) {
                /* Not place to put it; ignore. */
                إذا (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    استمر;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->عدد_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        إذا (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        إذا (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ساكن bool
CopyCompatToKeymap(بنية xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    إذا (!darray_empty(info->interps)) {
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
                     &keymap->sym_interprets, &keymap->عدد_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    أرجع true;
}

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    إذا (file != عدم)
        HandleCompatMapFile(&info, file);

    إذا (info.error_گنتی != 0)
        اذهب err_info;

    إذا (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearCompatInfo(&info);
    أرجع true;

err_info:
    ClearCompatInfo(&info);
    أرجع false;
}
