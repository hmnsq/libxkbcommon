/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "action.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "vmod.h"
#અન્તર્ભાવ "util-mem.h"

ગણના si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

પ્રકારવ્યાખ્યા સંરચના {
    ગણના si_field defined;
    ગણના merge_mode merge;

    સંરચના xkb_sym_interpret interp;
} SymInterpInfo;

ગણના led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

પ્રકારવ્યાખ્યા સંરચના {
    ગણના led_field defined;
    ગણના merge_mode merge;

    સંરચના xkb_led led;
} LedInfo;

પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર *name;
    પૂર્ણાંક error_ગણતરી;
    unsigned પૂર્ણાંક include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t અંક_leds;
    ActionsInfo default_actions;
    સંરચના xkb_mod_set mods;

    અચલ સંરચના xkb_keymap_info *keymap_info;
    સંરચના xkb_context *ctx;
} CompatInfo;

સ્થિર અચલ અક્ષર *
siText(SymInterpInfo *si, CompatInfo *info)
{
    અક્ષર *buf = xkb_context_get_buffer(info->ctx, 128);

    જો (si == &info->default_interp)
        પાછા "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    પાછા buf;
}

સ્થિર અંતઃરેખ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, અચલ અક્ષર *field)
{
    પાછા ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

સ્થિર અંતઃરેખ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, અચલ અક્ષર *field,
                અચલ અક્ષર *wanted)
{
    પાછા ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

સ્થિર અચલ અક્ષર*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    જો (ledi == &info->default_led) {
        પ્રતિજ્ઞા(xkb_atom_text(info->ctx, ledi->led.name) == ખાલી);
        પાછા "default";
    } નહીંતર {
        પ્રતિજ્ઞા(xkb_atom_text(info->ctx, ledi->led.name) != ખાલી);
        પાછા xkb_atom_text(info->ctx, ledi->led.name);
    }
}

સ્થિર અંતઃરેખ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, અચલ અક્ષર *field,
                 અચલ અક્ષર *wanted)
{
    પાછા ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

સ્થિર અંતઃરેખ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, અચલ અક્ષર *field)
{
    પાછા ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

સ્થિર અંતઃરેખ શૂન્ય
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ચાવી_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ચાવી_REPEAT, "invalid initial value");
}

સ્થિર અંતઃરેખ શૂન્ય
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

સ્થિર શૂન્ય
InitCompatInfo(CompatInfo *info, અચલ સંરચના xkb_keymap_info *keymap_info,
               unsigned પૂર્ણાંક include_depth, અચલ સંરચના xkb_mod_set *mods)
{
    memset(info, 0, કદ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

સ્થિર શૂન્ય
ClearCompatInfo(CompatInfo *info)
{
    મુક્ત(info->name);
    darray_free(info->interps);
}

સ્થિર SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        જો (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            પાછા old;

    પાછા ખાલી;
}

સ્થિર bool
UseNewInterpField(ગણના si_field field, ગણના si_field old, ગણના si_field new,
                  bool clobber, bool report, ગણના si_field *collide)
{
    જો (!(old & field))
        પાછા (new & field);

    જો (new & field) {
        જો (report)
            *collide |= field;

        પાછા clobber;
    }

    પાછા false;
}

સ્થિર bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    અચલ bool clobber = (new->merge != MERGE_AUGMENT);
    અચલ પૂર્ણાંક verbosity = xkb_context_get_log_verbosity(info->ctx);
    અચલ bool report = (same_file && verbosity > 0) || verbosity > 9;
    ગણના si_field collide = 0;

    જો (new->merge == MERGE_REPLACE) {
        જો (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        પાછા true;
    }

    જો (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    જો (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        જો (old->interp.અંક_actions > 1) {
                મુક્ત(old->interp.a.actions);
            }
            old->interp.અંક_actions = new->interp.અંક_actions;
            જો (new->interp.અંક_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (સંઘ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.અંક_actions = 0;
            } નહીંતર {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    જો (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    જો (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    જો (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    પાછા true;
}

સ્થિર bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    જો (old)
        પાછા MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    પાછા true;
}

/***====================================================================***/

// TODO: more lenient?
સ્થિર bool
ResolveStateAndPredicate(પદાવલિ_Def *પદાવલિ, ગણના xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    જો (પદાવલિ == ખાલી) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        પાછા true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    જો (પદાવલિ->common.type == STMT_પદાવલિ_ACTION_DECL) {
        અચલ અક્ષર *pred_txt = xkb_atom_text(info->ctx, પદાવલિ->action.name);
        unsigned પૂર્ણાંક pred = 0;
        જો (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !પદાવલિ->action.args || પદાવલિ->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            પાછા false;
        }
        *pred_rtrn = (ગણના xkb_match_operation) pred;
        પદાવલિ = પદાવલિ->action.args;
    }
    નહીંતર જો (પદાવલિ->common.type == STMT_પદાવલિ_IDENT) {
        અચલ અક્ષર *pred_txt = xkb_atom_text(info->ctx, પદાવલિ->ident.ident);
        જો (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            પાછા true;
        }
    }

    પાછા પદાવલિ_Resolve_Mod_Mask(info->ctx, પદાવલિ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

સ્થિર bool
UseNewLEDField(ગણના led_field field, ગણના led_field old, ગણના led_field new,
               bool clobber, bool report, ગણના led_field *collide)
{
    જો (!(old & field))
        પાછા (new & field);

    જો (new & field) {
        જો (report)
            *collide |= field;

        પાછા clobber;
    }

    પાછા false;
}

સ્થિર bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ગણના led_field collide;
    અચલ bool clobber = (new->merge != MERGE_AUGMENT);
    અચલ પૂર્ણાંક verbosity = xkb_context_get_log_verbosity(info->ctx);
    અચલ bool report = (same_file && verbosity > 0) || verbosity > 9;

    જો (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        પાછા true;
    }

    જો (new->merge == MERGE_REPLACE) {
        જો (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        પાછા true;
    }

    collide = 0;
    જો (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    જો (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    જો (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    જો (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    પાછા true;
}

સ્થિર bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    માટે (xkb_led_index_t i = 0; i < info->અંક_leds; i++) {
        LedInfo *old = &info->leds[i];

        જો (old->led.name != new->led.name)
            ચાલુ;

        પાછા MergeLedMap(info, old, new, same_file);
    }

    જો (info->અંક_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        પાછા false;
    }
    info->leds[info->અંક_leds++] = *new;
    પાછા true;
}

સ્થિર શૂન્ય
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        ગણના merge_mode merge)
{
    જો (from->error_ગણતરી > 0) {
        into->error_ગણતરી += from->error_ગણતરી;
        પાછા;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    જો (into->name == ખાલી) {
        into->name = steal(&from->name);
    }

    જો (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    નહીંતર {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            જો (!AddInterp(into, si, false))
                into->error_ગણતરી++;
        }
    }

    જો (into->અંક_leds == 0) {
        memcpy(into->leds, from->leds, કદ(*from->leds) * from->અંક_leds);
        into->અંક_leds = from->અંક_leds;
        from->અંક_leds = 0;
    }
    નહીંતર {
        માટે (xkb_led_index_t i = 0; i < from->અંક_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            જો (!AddLedMap(into, ledi, false))
                into->error_ગણતરી++;
        }
    }
}

સ્થિર શૂન્ય
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

સ્થિર bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    જો (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ગણતરી += 10;
        પાછા false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    અચલ સંરચના parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    માટે (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        અક્ષર path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, કદ(path));
        જો (!file) {
            info->error_ગણતરી += 10;
            ClearCompatInfo(&included);
            પાછા false;
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

    પાછા (info->error_ગણતરી == 0);
}

સ્થિર bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, અચલ અક્ષર *field,
               પદાવલિ_Def *arrayNdx, પદાવલિ_Def *કિંમત)
{
    જો (istreq(field, "action")) {
        જો (arrayNdx)
            પાછા ReportSINotArray(info, si, field);

        જો (કિંમત->common.type == STMT_પદાવલિ_ACTION_LIST) {
            unsigned પૂર્ણાંક અંક_actions = 0;
            માટે (પદાવલિ_Def *act = કિંમત->actions.actions;
                 act; act = (પદાવલિ_Def *) act->common.next)
                 અંક_actions++;

            જો (અંક_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, અંક_actions);
                પાછા false;
            }

            si->interp.અંક_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(સંઘ xkb_action) actions = darray_new();
            માટે (પદાવલિ_Def *act = કિંમત->actions.actions;
                 act; act = (પદાવલિ_Def *) act->common.next) {
                સંઘ xkb_action toAct = { 0 };
                પસંદગી (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                સ્થિતિ PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    તોડો;
                સ્થિતિ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    પાછા false;
                મૂળભૂત:
                    ;
                }
                જો (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } નહીંતર જો (likely(અંક_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.અંક_actions = 1;
                    si->interp.a.action = toAct;
                } નહીંતર {
                    darray_append(actions, toAct);
                }
            }
            પસંદગી (darray_size(actions)) {
            સ્થિતિ 0:
                /* No action or exactly one action: already processed */
                પ્રતિજ્ઞા(si->interp.અંક_actions <= 1);
                તોડો;
            સ્થિતિ 1:
                /* One action: some actions were dropped */
                si->interp.અંક_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                તોડો;
            મૂળભૂત:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.અંક_actions =
                    (xkb_action_ગણતરી_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, ખાલી);
            }
        }
        નહીંતર {
            પસંદગી (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, કિંમત, &si->interp.a.action)) {
            સ્થિતિ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.અંક_actions = 0;
                તોડો;
            સ્થિતિ PARSER_FATAL_ERROR:
                પાછા false;
            મૂળભૂત:
                si->interp.અંક_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    નહીંતર જો (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        જો (arrayNdx)
            પાછા ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        જો (!પદાવલિ_Resolve_Mod(info->ctx, કિંમત, MOD_VIRT, &info->mods, &ndx))
            પાછા ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    નહીંતર જો (istreq(field, "repeat")) {
        bool set = false;

        જો (arrayNdx)
            પાછા ReportSINotArray(info, si, field);

        જો (!પદાવલિ_Resolve_Boolean(info->ctx, કિંમત, &set))
            પાછા ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    નહીંતર જો (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    નહીંતર જો (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        જો (arrayNdx)
            પાછા ReportSINotArray(info, si, field);

        જો (!પદાવલિ_Resolve_Enum(info->ctx, કિંમત, &val, use_Mod_Map_કિંમત_Names))
            પાછા ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    નહીંતર {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        પાછા !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    પાછા true;
}

સ્થિર bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, અચલ અક્ષર *field,
               પદાવલિ_Def *arrayNdx, પદાવલિ_Def **કિંમત_ptr)
{
    પદાવલિ_Def * restrict અચલ કિંમત = *કિંમત_ptr;

    જો (istreq(field, "modifiers") || istreq(field, "mods")) {
        જો (arrayNdx)
            પાછા ReportLedNotArray(info, ledi, field);

        જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, કિંમત, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            પાછા ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    નહીંતર જો (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        જો (arrayNdx)
            પાછા ReportLedNotArray(info, ledi, field);

        bool pending = false;
        જો (!પદાવલિ_Resolve_Group_Mask(info->keymap_info, કિંમત, &mask, &pending)) {
            જો (pending) {
                ledi->led.pending_groups = true;
                અચલ darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (સંરચના pending_computation) {
                        .પદાવલિ = *કિંમત_ptr,
                        .computed = false,
                        .કિંમત = 0,
                    }
                );
                *કિંમત_ptr = ખાલી;
                static_assert(કદ(pending_index) == કદ(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } નહીંતર {
                પાછા ReportLedBadType(info, ledi, field, "group mask");
            }
        } નહીંતર {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    નહીંતર જો (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        જો (arrayNdx)
            પાછા ReportLedNotArray(info, ledi, field);

        અચલ uint8_t offset = info->keymap_info->features.controls_name_offset;
        જો (!પદાવલિ_Resolve_Mask(info->ctx, કિંમત, &mask, ctrlMaskNames + offset))
            પાછા ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    નહીંતર જો (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    નહીંતર જો (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        જો (arrayNdx)
            પાછા ReportLedNotArray(info, ledi, field);

        જો (!પદાવલિ_Resolve_Mask(info->ctx, કિંમત, &mask,
                             modComponentMaskNames))
            પાછા ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    નહીંતર જો (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        જો (arrayNdx)
            પાછા ReportLedNotArray(info, ledi, field);

        જો (!પદાવલિ_Resolve_Mask(info->ctx, કિંમત, &mask,
                             groupComponentMaskNames))
            પાછા ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    નહીંતર જો (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    નહીંતર જો (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    નહીંતર {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        પાછા !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    પાછા true;
}

સ્થિર bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *ndx;
    bool ret;

    જો (!પદાવલિ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    નહીંતર જો (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->કિંમત);
        જો (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    નહીંતર જો (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->કિંમત);
        જો (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    નહીંતર જો (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->કિંમત, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    નહીંતર {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        પાછા !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    પાછા ret;
}

સ્થિર bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *arrayNdx;

    માટે (; def; def = (VarDef *) def->common.next) {
        જો (!પદાવલિ_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ચાલુ;
        }
        જો (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            ચાલુ;
        }
        જો (!SetInterpField(info, si, field, arrayNdx, def->કિંમત))
            ok = false;
    }

    પાછા ok;
}

સ્થિર bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ગણના xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    જો (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        પાછા false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    જો (!HandleInterpBody(info, def->def, &si)) {
        info->error_ગણતરી++;
        પાછા false;
    }

    જો (!AddInterp(info, &si, true)) {
        info->error_ગણતરી++;
        પાછા false;
    }

    પાછા true;
}

સ્થિર bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    માટે (VarDef *var = def->body; var != ખાલી; var = (VarDef *) var->common.next) {
        અચલ અક્ષર *elem, *field;
        પદાવલિ_Def *arrayNdx;
        જો (!પદાવલિ_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ચાલુ;
        }

        જો (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        નહીંતર {
            જો (!SetLedMapField(info, &ledi, field, arrayNdx, &var->કિંમત))
                ok = false;
        }
    }

    પાછા ok && AddLedMap(info, &ledi, true);
}

સ્થિર શૂન્ય
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    મુક્ત(info->name);
    info->name = strdup_safe(file->name);

    માટે (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        પસંદગી (stmt->type) {
        સ્થિતિ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            તોડો;
        સ્થિતિ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            તોડો;
        સ્થિતિ STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            તોડો;
        સ્થિતિ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            તોડો;
        સ્થિતિ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            તોડો;
        સ્થિતિ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            તોડો;
        સ્થિતિ STMT_UNKNOWN_DECLARATION:
        સ્થિતિ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            તોડો;
        મૂળભૂત:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            તોડો;
        }

        જો (!ok)
            info->error_ગણતરી++;

        જો (info->error_ગણતરી > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            તોડો;
        }
    }
}

/* Temporary struct for CopyInterps. */
સંરચના collect {
    darray(સંરચના xkb_sym_interpret) sym_interprets;
};

સ્થિર શૂન્ય
CopyInterps(CompatInfo *info, bool need_પ્રતીક, ગણના xkb_match_operation pred,
            સંરચના collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        જો (si->interp.match == pred &&
            (si->interp.sym != XKB_ચાવી_No_પ્રતીક) == need_પ્રતીક)
            darray_append(collect->sym_interprets, si->interp);
}

સ્થિર શૂન્ય
CopyLedMapDefsToKeymap(સંરચના xkb_keymap *keymap, CompatInfo *info)
{
    માટે (xkb_led_index_t idx = 0; idx < info->અંક_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        સંરચના xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            જો (led->name == ledi->led.name)
                તોડો;

        /* Not previously declared; create it with next free index. */
        જો (i >= keymap->અંક_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                જો (led->name == XKB_ATOM_NONE)
                    તોડો;

            જો (i >= keymap->અંક_leds) {
                /* Not place to put it; ignore. */
                જો (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ચાલુ;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->અંક_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        જો (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        જો (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

સ્થિર bool
CopyCompatToKeymap(સંરચના xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    જો (!darray_empty(info->interps)) {
        સંરચના collect collect;
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
                     &keymap->sym_interprets, &keymap->અંક_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    પાછા true;
}

bool
CompileCompatMap(XkbFile *file, સંરચના xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    જો (file != ખાલી)
        HandleCompatMapFile(&info, file);

    જો (info.error_ગણતરી != 0)
        જાઓ err_info;

    જો (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        જાઓ err_info;

    ClearCompatInfo(&info);
    પાછા true;

err_info:
    ClearCompatInfo(&info);
    પાછા false;
}
