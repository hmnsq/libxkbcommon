/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "action.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "vmod.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"

ಗಣನಾ si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಗಣನಾ si_field defined;
    ಗಣನಾ merge_mode merge;

    ಸಂರಚನಾ xkb_sym_interpret interp;
} SymInterpInfo;

ಗಣನಾ led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಗಣನಾ led_field defined;
    ಗಣನಾ merge_mode merge;

    ಸಂರಚನಾ xkb_led led;
} LedInfo;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ *name;
    ಪೂರ್ಣಾಙ್ಕ error_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t ಅಂಕೆ_leds;
    ActionsInfo default_actions;
    ಸಂರಚನಾ xkb_mod_set mods;

    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info;
    ಸಂರಚನಾ xkb_context *ctx;
} CompatInfo;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
siText(SymInterpInfo *si, CompatInfo *info)
{
    ವರ್ಣ *buf = xkb_context_get_buffer(info->ctx, 128);

    ಯದಿ (si == &info->default_interp)
        ಪ್ರತ್ಯಾಗಮ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ಪ್ರತ್ಯಾಗಮ buf;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ಧ್ರುವ ವರ್ಣ *field)
{
    ಪ್ರತ್ಯಾಗಮ ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ಧ್ರುವ ವರ್ಣ *field,
                ಧ್ರುವ ವರ್ಣ *wanted)
{
    ಪ್ರತ್ಯಾಗಮ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ಯದಿ (ledi == &info->default_led) {
        ಪ್ರತಿಜ್ಞಾ(xkb_atom_text(info->ctx, ledi->led.name) == ರಿಕ್ತ);
        ಪ್ರತ್ಯಾಗಮ "default";
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿಜ್ಞಾ(xkb_atom_text(info->ctx, ledi->led.name) != ರಿಕ್ತ);
        ಪ್ರತ್ಯಾಗಮ xkb_atom_text(info->ctx, ledi->led.name);
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ಧ್ರುವ ವರ್ಣ *field,
                 ಧ್ರುವ ವರ್ಣ *wanted)
{
    ಪ್ರತ್ಯಾಗಮ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ಧ್ರುವ ವರ್ಣ *field)
{
    ಪ್ರತ್ಯಾಗಮ ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ಕೀಲಿ_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ಕೀಲಿ_REPEAT, "invalid initial value");
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

ಸ್ಥಿರ ಶೂನ್ಯ
InitCompatInfo(CompatInfo *info, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
               unsigned ಪೂರ್ಣಾಙ್ಕ include_depth, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods)
{
    memset(info, 0, ಪರಿಮಾಣ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ClearCompatInfo(CompatInfo *info)
{
    ಮುಕ್ತ(info->name);
    darray_free(info->interps);
}

ಸ್ಥಿರ SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ಯದಿ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            ಪ್ರತ್ಯಾಗಮ old;

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ bool
UseNewInterpField(ಗಣನಾ si_field field, ಗಣನಾ si_field old, ಗಣನಾ si_field new,
                  bool clobber, bool report, ಗಣನಾ si_field *collide)
{
    ಯದಿ (!(old & field))
        ಪ್ರತ್ಯಾಗಮ (new & field);

    ಯದಿ (new & field) {
        ಯದಿ (report)
            *collide |= field;

        ಪ್ರತ್ಯಾಗಮ clobber;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ಧ್ರುವ bool clobber = (new->merge != MERGE_AUGMENT);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಧ್ರುವ bool report = (same_file && verbosity > 0) || verbosity > 9;
    ಗಣನಾ si_field collide = 0;

    ಯದಿ (new->merge == MERGE_REPLACE) {
        ಯದಿ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ಯದಿ (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ಯದಿ (old->interp.ಅಂಕೆ_actions > 1) {
                ಮುಕ್ತ(old->interp.a.actions);
            }
            old->interp.ಅಂಕೆ_actions = new->interp.ಅಂಕೆ_actions;
            ಯದಿ (new->interp.ಅಂಕೆ_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (ಸಙ್ಘ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.ಅಂಕೆ_actions = 0;
            } ಅನ್ಯಥಾ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    ಯದಿ (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ಯದಿ (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    ಯದಿ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    ಯದಿ (old)
        ಪ್ರತ್ಯಾಗಮ MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

// TODO: more lenient?
ಸ್ಥಿರ bool
ResolveStateAndPredicate(ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಗಣನಾ xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ == ರಿಕ್ತ) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL) {
        ಧ್ರುವ ವರ್ಣ *pred_txt = xkb_atom_text(info->ctx, ಅಭಿವ್ಯಕ್ತಿ->action.name);
        unsigned ಪೂರ್ಣಾಙ್ಕ pred = 0;
        ಯದಿ (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !ಅಭಿವ್ಯಕ್ತಿ->action.args || ಅಭಿವ್ಯಕ್ತಿ->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ಪ್ರತ್ಯಾಗಮ false;
        }
        *pred_rtrn = (ಗಣನಾ xkb_match_operation) pred;
        ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ->action.args;
    }
    ಅನ್ಯಥಾ ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        ಧ್ರುವ ವರ್ಣ *pred_txt = xkb_atom_text(info->ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಯದಿ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, ಅಭಿವ್ಯಕ್ತಿ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ಸ್ಥಿರ bool
UseNewLEDField(ಗಣನಾ led_field field, ಗಣನಾ led_field old, ಗಣನಾ led_field new,
               bool clobber, bool report, ಗಣನಾ led_field *collide)
{
    ಯದಿ (!(old & field))
        ಪ್ರತ್ಯಾಗಮ (new & field);

    ಯದಿ (new & field) {
        ಯದಿ (report)
            *collide |= field;

        ಪ್ರತ್ಯಾಗಮ clobber;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ಗಣನಾ led_field collide;
    ಧ್ರುವ bool clobber = (new->merge != MERGE_AUGMENT);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಧ್ರುವ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ಯದಿ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (new->merge == MERGE_REPLACE) {
        ಯದಿ (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    collide = 0;
    ಯದಿ (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    ಯದಿ (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    ಯದಿ (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    ಯದಿ (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    ಪ್ರತಿ (xkb_led_index_t i = 0; i < info->ಅಂಕೆ_leds; i++) {
        LedInfo *old = &info->leds[i];

        ಯದಿ (old->led.name != new->led.name)
            ಅನುವರ್ತ;

        ಪ್ರತ್ಯಾಗಮ MergeLedMap(info, old, new, same_file);
    }

    ಯದಿ (info->ಅಂಕೆ_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ಪ್ರತ್ಯಾಗಮ false;
    }
    info->leds[info->ಅಂಕೆ_leds++] = *new;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        ಗಣನಾ merge_mode merge)
{
    ಯದಿ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ಯದಿ (into->name == ರಿಕ್ತ) {
        into->name = steal(&from->name);
    }

    ಯದಿ (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ಅನ್ಯಥಾ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            ಯದಿ (!AddInterp(into, si, false))
                into->error_ಎಣಿಕೆ++;
        }
    }

    ಯದಿ (into->ಅಂಕೆ_leds == 0) {
        memcpy(into->leds, from->leds, ಪರಿಮಾಣ(*from->leds) * from->ಅಂಕೆ_leds);
        into->ಅಂಕೆ_leds = from->ಅಂಕೆ_leds;
        from->ಅಂಕೆ_leds = 0;
    }
    ಅನ್ಯಥಾ {
        ಪ್ರತಿ (xkb_led_index_t i = 0; i < from->ಅಂಕೆ_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            ಯದಿ (!AddLedMap(into, ledi, false))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ಸ್ಥಿರ bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ಯದಿ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ಎಣಿಕೆ += 10;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಪ್ರತಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        ವರ್ಣ path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, ಪರಿಮಾಣ(path));
        ಯದಿ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            ClearCompatInfo(&included);
            ಪ್ರತ್ಯಾಗಮ false;
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

    ಪ್ರತ್ಯಾಗಮ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ಧ್ರುವ ವರ್ಣ *field,
               ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ಯದಿ (istreq(field, "action")) {
        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportSINotArray(info, si, field);

        ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST) {
            unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_actions = 0;
            ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = ಮೌಲ್ಯ->actions.actions;
                 act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.next)
                 ಅಂಕೆ_actions++;

            ಯದಿ (ಅಂಕೆ_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, ಅಂಕೆ_actions);
                ಪ್ರತ್ಯಾಗಮ false;
            }

            si->interp.ಅಂಕೆ_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(ಸಙ್ಘ xkb_action) actions = darray_new();
            ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = ಮೌಲ್ಯ->actions.actions;
                 act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.next) {
                ಸಙ್ಘ xkb_action toAct = { 0 };
                ವಿಕಲ್ಪ (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                ಪಕ್ಷ PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    ಭಙ್ಗ;
                ಪಕ್ಷ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ಪ್ರತ್ಯಾಗಮ false;
                ಸ್ವಭಾವ:
                    ;
                }
                ಯದಿ (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ಅನ್ಯಥಾ ಯದಿ (likely(ಅಂಕೆ_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.ಅಂಕೆ_actions = 1;
                    si->interp.a.action = toAct;
                } ಅನ್ಯಥಾ {
                    darray_append(actions, toAct);
                }
            }
            ವಿಕಲ್ಪ (darray_size(actions)) {
            ಪಕ್ಷ 0:
                /* No action or exactly one action: already processed */
                ಪ್ರತಿಜ್ಞಾ(si->interp.ಅಂಕೆ_actions <= 1);
                ಭಙ್ಗ;
            ಪಕ್ಷ 1:
                /* One action: some actions were dropped */
                si->interp.ಅಂಕೆ_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                ಭಙ್ಗ;
            ಸ್ವಭಾವ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.ಅಂಕೆ_actions =
                    (xkb_action_ಎಣಿಕೆ_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, ರಿಕ್ತ);
            }
        }
        ಅನ್ಯಥಾ {
            ವಿಕಲ್ಪ (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, ಮೌಲ್ಯ, &si->interp.a.action)) {
            ಪಕ್ಷ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.ಅಂಕೆ_actions = 0;
                ಭಙ್ಗ;
            ಪಕ್ಷ PARSER_FATAL_ERROR:
                ಪ್ರತ್ಯಾಗಮ false;
            ಸ್ವಭಾವ:
                si->interp.ಅಂಕೆ_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(info->ctx, ಮೌಲ್ಯ, MOD_VIRT, &info->mods, &ndx))
            ಪ್ರತ್ಯಾಗಮ ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "repeat")) {
        bool set = false;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportSINotArray(info, si, field);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &set))
            ಪ್ರತ್ಯಾಗಮ ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportSINotArray(info, si, field);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(info->ctx, ಮೌಲ್ಯ, &val, use_Mod_Map_ಮೌಲ್ಯ_Names))
            ಪ್ರತ್ಯಾಗಮ ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    ಅನ್ಯಥಾ {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ಧ್ರುವ ವರ್ಣ *field,
               ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def * restrict ಧ್ರುವ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;

    ಯದಿ (istreq(field, "modifiers") || istreq(field, "mods")) {
        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportLedNotArray(info, ledi, field);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, ಮೌಲ್ಯ, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ಪ್ರತ್ಯಾಗಮ ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportLedNotArray(info, ledi, field);

        bool pending = false;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group_Mask(info->keymap_info, ಮೌಲ್ಯ, &mask, &pending)) {
            ಯದಿ (pending) {
                ledi->led.pending_groups = true;
                ಧ್ರುವ darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ಸಂರಚನಾ pending_computation) {
                        .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                        .computed = false,
                        .ಮೌಲ್ಯ = 0,
                    }
                );
                *ಮೌಲ್ಯ_ptr = ರಿಕ್ತ;
                static_assert(ಪರಿಮಾಣ(pending_index) == ಪರಿಮಾಣ(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ ReportLedBadType(info, ledi, field, "group mask");
            }
        } ಅನ್ಯಥಾ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportLedNotArray(info, ledi, field);

        ಧ್ರುವ uint8_t offset = info->keymap_info->features.controls_name_offset;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(info->ctx, ಮೌಲ್ಯ, &mask, ctrlMaskNames + offset))
            ಪ್ರತ್ಯಾಗಮ ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportLedNotArray(info, ledi, field);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(info->ctx, ಮೌಲ್ಯ, &mask,
                             modComponentMaskNames))
            ಪ್ರತ್ಯಾಗಮ ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        ಯದಿ (arrayNdx)
            ಪ್ರತ್ಯಾಗಮ ReportLedNotArray(info, ledi, field);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(info->ctx, ಮೌಲ್ಯ, &mask,
                             groupComponentMaskNames))
            ಪ್ರತ್ಯಾಗಮ ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ಅನ್ಯಥಾ {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ndx;
    bool ret;

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    ಅನ್ಯಥಾ ಯದಿ (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->ಮೌಲ್ಯ);
        ಯದಿ (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    ಅನ್ಯಥಾ ಯದಿ (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->ಮೌಲ್ಯ);
        ಯದಿ (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    ಅನ್ಯಥಾ ಯದಿ (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->ಮೌಲ್ಯ, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    ಅನ್ಯಥಾ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಪ್ರತಿ (; def; def = (VarDef *) def->common.next) {
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ಅನುವರ್ತ;
        }
        ಯದಿ (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            ಅನುವರ್ತ;
        }
        ಯದಿ (!SetInterpField(info, si, field, arrayNdx, def->ಮೌಲ್ಯ))
            ok = false;
    }

    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ಗಣನಾ xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    ಯದಿ (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    ಯದಿ (!HandleInterpBody(info, def->def, &si)) {
        info->error_ಎಣಿಕೆ++;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (!AddInterp(info, &si, true)) {
        info->error_ಎಣಿಕೆ++;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    ಪ್ರತಿ (VarDef *var = def->body; var != ರಿಕ್ತ; var = (VarDef *) var->common.next) {
        ಧ್ರುವ ವರ್ಣ *elem, *field;
        ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ಅನುವರ್ತ;
        }

        ಯದಿ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        ಅನ್ಯಥಾ {
            ಯದಿ (!SetLedMapField(info, &ledi, field, arrayNdx, &var->ಮೌಲ್ಯ))
                ok = false;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ok && AddLedMap(info, &ledi, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    ಮುಕ್ತ(info->name);
    info->name = strdup_safe(file->name);

    ಪ್ರತಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ವಿಕಲ್ಪ (stmt->type) {
        ಪಕ್ಷ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_UNKNOWN_DECLARATION:
        ಪಕ್ಷ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ಭಙ್ಗ;
        }

        ಯದಿ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಯದಿ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            ಭಙ್ಗ;
        }
    }
}

/* Temporary struct for CopyInterps. */
ಸಂರಚನಾ collect {
    darray(ಸಂರಚನಾ xkb_sym_interpret) sym_interprets;
};

ಸ್ಥಿರ ಶೂನ್ಯ
CopyInterps(CompatInfo *info, bool need_ಚಿಹ್ನೆ, ಗಣನಾ xkb_match_operation pred,
            ಸಂರಚನಾ collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ಯದಿ (si->interp.match == pred &&
            (si->interp.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) == need_ಚಿಹ್ನೆ)
            darray_append(collect->sym_interprets, si->interp);
}

ಸ್ಥಿರ ಶೂನ್ಯ
CopyLedMapDefsToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, CompatInfo *info)
{
    ಪ್ರತಿ (xkb_led_index_t idx = 0; idx < info->ಅಂಕೆ_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        ಸಂರಚನಾ xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ಯದಿ (led->name == ledi->led.name)
                ಭಙ್ಗ;

        /* Not previously declared; create it with next free index. */
        ಯದಿ (i >= keymap->ಅಂಕೆ_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ಯದಿ (led->name == XKB_ATOM_NONE)
                    ಭಙ್ಗ;

            ಯದಿ (i >= keymap->ಅಂಕೆ_leds) {
                /* Not place to put it; ignore. */
                ಯದಿ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ಅನುವರ್ತ;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->ಅಂಕೆ_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ಯದಿ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ಯದಿ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ಸ್ಥಿರ bool
CopyCompatToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ಯದಿ (!darray_empty(info->interps)) {
        ಸಂರಚನಾ collect collect;
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
                     &keymap->sym_interprets, &keymap->ಅಂಕೆ_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    ಪ್ರತ್ಯಾಗಮ true;
}

bool
CompileCompatMap(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ಯದಿ (file != ರಿಕ್ತ)
        HandleCompatMapFile(&info, file);

    ಯದಿ (info.error_ಎಣಿಕೆ != 0)
        ಗಚ್ಛ err_info;

    ಯದಿ (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        ಗಚ್ಛ err_info;

    ClearCompatInfo(&info);
    ಪ್ರತ್ಯಾಗಮ true;

err_info:
    ClearCompatInfo(&info);
    ಪ್ರತ್ಯಾಗಮ false;
}
