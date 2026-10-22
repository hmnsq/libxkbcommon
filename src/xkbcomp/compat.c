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

ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ {
    SI_ಕ್ಷೇತ್ರ_VIRTUAL_MOD = (1 << 0),
    SI_ಕ್ಷೇತ್ರ_ACTION = (1 << 1),
    SI_ಕ್ಷೇತ್ರ_AUTO_REPEAT = (1 << 2),
    SI_ಕ್ಷೇತ್ರ_LEVEL_ONE_ONLY = (1 << 3),
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ defined;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;

    ರಚನೆ xkb_sym_interpret interp;
} SymInterpInfo;

ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ {
    LED_ಕ್ಷೇತ್ರ_MODS = (1 << 0),
    LED_ಕ್ಷೇತ್ರ_GROUPS = (1 << 1),
    LED_ಕ್ಷೇತ್ರ_CTRLS = (1 << 2),
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ defined;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;

    ರಚನೆ xkb_led led;
} LedInfo;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ error_ಎಣಿಕೆ;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_leds;
    ActionsInfo default_actions;
    ರಚನೆ xkb_mod_ಗಣ mods;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info;
    ರಚನೆ xkb_context *ctx;
} CompatInfo;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
si_ಪಠ್ಯ(SymInterpInfo *si, CompatInfo *info)
{
    ಅಕ್ಷರ *buf = xkb_context_get_ಬಫರ್(info->ctx, 128);

    ಆದರೆ (si == &info->default_interp)
        ಹಿಂತಿರುಗಿ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_ಪಠ್ಯ(info->ctx, si->interp.sym),
             SIMatch_ಪಠ್ಯ(si->interp.ಹೊಂದಿಕೆ),
             Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ಹಿಂತಿರುಗಿ buf;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ ReportNotArray(info->ctx, "symbol interpretation", ಕ್ಷೇತ್ರ_2,
                          si_ಪಠ್ಯ(si, info));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wanted)
{
    ಹಿಂತಿರುಗಿ ReportBadType(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                         "symbol interpretation", ಕ್ಷೇತ್ರ_2,
                         si_ಪಠ್ಯ(si, info), wanted);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ಆದರೆ (ledi == &info->default_led) {
        ಪ್ರತಿಜ್ಞಾ(xkb_atom_ಪಠ್ಯ(info->ctx, ledi->led.name) == ಖಾಲಿ);
        ಹಿಂತಿರುಗಿ "default";
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪ್ರತಿಜ್ಞಾ(xkb_atom_ಪಠ್ಯ(info->ctx, ledi->led.name) != ಖಾಲಿ);
        ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(info->ctx, ledi->led.name);
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wanted)
{
    ಹಿಂತಿರುಗಿ ReportBadType(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                         "indicator map",
                         ಕ್ಷೇತ್ರ_2, LEDText(info, ledi), wanted);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ ReportNotArray(info->ctx, "indicator map", ಕ್ಷೇತ್ರ_2,
                          LEDText(info, ledi));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
InitInterp(SymInterpInfo *info)
{
    info->ವಿಲೀನ = ವಿಲೀನ_DEFAULT; /*ಬಳಸದ*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ಕೀಲಿ_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ಕೀಲಿ_REPEAT, "invalid initial value");
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
InitLED(LedInfo *info)
{
    info->ವಿಲೀನ = ವಿಲೀನ_DEFAULT; /*ಬಳಸದ*/
}

ಸ್ಥಿರ ಶೂನ್ಯ
InitCompatInfo(CompatInfo *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
               ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods)
{
    memset(info, 0, ಗಾತ್ರ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ಆಳ = include_ಆಳ;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ಆಳ > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ClearCompatInfo(CompatInfo *info)
{
    ಬಿಡುಗಡೆ(info->name);
    darray_free(info->interps);
}

ಸ್ಥಿರ SymInterpInfo *
ಕಂಡುಹಿಡಿ_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ಆದರೆ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.ಹೊಂದಿಕೆ == new->interp.ಹೊಂದಿಕೆ)
            ಹಿಂತಿರುಗಿ old;

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ bool
Use_New_Interp_ಕ್ಷೇತ್ರ(ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2, ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ old, ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ new,
                  bool clobber, bool report, ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ *collide)
{
    ಆದರೆ (!(old & ಕ್ಷೇತ್ರ_2))
        ಹಿಂತಿರುಗಿ (new & ಕ್ಷೇತ್ರ_2);

    ಆದರೆ (new & ಕ್ಷೇತ್ರ_2) {
        ಆದರೆ (report)
            *collide |= ಕ್ಷೇತ್ರ_2;

        ಹಿಂತಿರುಗಿ clobber;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
ವಿಲೀನ_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ಸ್ಥಿರಾಂಕ bool clobber = (new->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಸ್ಥಿರಾಂಕ bool report = (same_file && verbosity > 0) || verbosity > 9;
    ಎಣಿಕೆ si_ಕ್ಷೇತ್ರ collide = 0;

    ಆದರೆ (new->ವಿಲೀನ == ವಿಲೀನ_REPLACE) {
        ಆದರೆ (report)
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_ಪಠ್ಯ(new, info));
        *old = *new;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (Use_New_Interp_ಕ್ಷೇತ್ರ(SI_ಕ್ಷೇತ್ರ_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_ಕ್ಷೇತ್ರ_VIRTUAL_MOD;
    }
    ಆದರೆ (Use_New_Interp_ಕ್ಷೇತ್ರ(SI_ಕ್ಷೇತ್ರ_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ಆದರೆ (old->interp.ಅಂಕೆ_actions > 1) {
                ಬಿಡುಗಡೆ(old->interp.a.actions);
            }
            old->interp.ಅಂಕೆ_actions = new->interp.ಅಂಕೆ_actions;
            ಆದರೆ (new->interp.ಅಂಕೆ_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (ಒಕ್ಕೂಟ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.ಅಂಕೆ_actions = 0;
            } ಇಲ್ಲದಿದ್ದರೆ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_ಕ್ಷೇತ್ರ_ACTION;
    }
    ಆದರೆ (Use_New_Interp_ಕ್ಷೇತ್ರ(SI_ಕ್ಷೇತ್ರ_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_ಕ್ಷೇತ್ರ_AUTO_REPEAT;
    }
    ಆದರೆ (Use_New_Interp_ಕ್ಷೇತ್ರ(SI_ಕ್ಷೇತ್ರ_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_ಕ್ಷೇತ್ರ_LEVEL_ONE_ONLY;
    }

    ಆದರೆ (collide) {
        log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_ಪಠ್ಯ(old, info),
                 (clobber ? "last" : "first"));
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = ಕಂಡುಹಿಡಿ_Matching_Interp(info, new);
    ಆದರೆ (old)
        ಹಿಂತಿರುಗಿ ವಿಲೀನ_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

// TODO: more lenient?
ಸ್ಥಿರ bool
ResolveStateAndPredicate(ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation *pred_rtrn,
                         xkb_mod_ಮುಖವಾಡ_t *mods_rtrn, CompatInfo *info)
{
    ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ == ಖಾಲಿ) {
        *pred_rtrn = ಹೊಂದಿಕೆ_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_ಮುಖವಾಡ_ALL;
        ಹಿಂತಿರುಗಿ true;
    }

    *pred_rtrn = ಹೊಂದಿಕೆ_EXACTLY;
    ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pred_txt = xkb_atom_ಪಠ್ಯ(info->ctx, ಅಭಿವ್ಯಕ್ತಿ->action.name);
        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pred = 0;
        ಆದರೆ (!LookupString(sym_Interpret_ಹೊಂದಿಕೆ_ಮುಖವಾಡ_ಹೆಸರುಗಳು, pred_txt, &pred) ||
            !ಅಭಿವ್ಯಕ್ತಿ->action.ವಾದಗಳು || ಅಭಿವ್ಯಕ್ತಿ->action.ವಾದಗಳು->common.ಮುಂದಿನ) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ಹಿಂತಿರುಗಿ false;
        }
        *pred_rtrn = (ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation) pred;
        ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ->action.ವಾದಗಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pred_txt = xkb_atom_ಪಠ್ಯ(info->ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಆದರೆ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = ಹೊಂದಿಕೆ_ANY;
            *mods_rtrn = MOD_REAL_ಮುಖವಾಡ_ALL;
            ಹಿಂತಿರುಗಿ true;
        }
    }

    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, ಅಭಿವ್ಯಕ್ತಿ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ಸ್ಥಿರ bool
UseNewLEDField(ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2, ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ old, ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ new,
               bool clobber, bool report, ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ *collide)
{
    ಆದರೆ (!(old & ಕ್ಷೇತ್ರ_2))
        ಹಿಂತಿರುಗಿ (new & ಕ್ಷೇತ್ರ_2);

    ಆದರೆ (new & ಕ್ಷೇತ್ರ_2) {
        ಆದರೆ (report)
            *collide |= ಕ್ಷೇತ್ರ_2;

        ಹಿಂತಿರುಗಿ clobber;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
ವಿಲೀನ_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ಎಣಿಕೆ led_ಕ್ಷೇತ್ರ collide;
    ಸ್ಥಿರಾಂಕ bool clobber = (new->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಸ್ಥಿರಾಂಕ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ಆದರೆ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (new->ವಿಲೀನ == ವಿಲೀನ_REPLACE) {
        ಆದರೆ (report)
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ಹಿಂತಿರುಗಿ true;
    }

    collide = 0;
    ಆದರೆ (UseNewLEDField(LED_ಕ್ಷೇತ್ರ_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_ಕ್ಷೇತ್ರ_MODS;
    }
    ಆದರೆ (UseNewLEDField(LED_ಕ್ಷೇತ್ರ_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_ಕ್ಷೇತ್ರ_GROUPS;
    }
    ಆದರೆ (UseNewLEDField(LED_ಕ್ಷೇತ್ರ_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_ಕ್ಷೇತ್ರ_CTRLS;
    }

    ಆದರೆ (collide) {
        log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t i = 0; i < info->ಅಂಕೆ_leds; i++) {
        LedInfo *old = &info->leds[i];

        ಆದರೆ (old->led.name != new->led.name)
            ಮುಂದುವರಿ;

        ಹಿಂತಿರುಗಿ ವಿಲೀನ_Led_Map(info, old, new, same_file);
    }

    ಆದರೆ (info->ಅಂಕೆ_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ಹಿಂತಿರುಗಿ false;
    }
    info->leds[info->ಅಂಕೆ_leds++] = *new;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    ಆದರೆ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಹಿಂತಿರುಗಿ;
    }

    ವಿಲೀನ_Mod_Sets(into->ctx, &into->mods, &from->mods, ವಿಲೀನ);

    ಆದರೆ (into->name == ಖಾಲಿ) {
        into->name = steal(&from->name);
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_Interp(into, si, false))
                into->error_ಎಣಿಕೆ++;
        }
    }

    ಆದರೆ (into->ಅಂಕೆ_leds == 0) {
        ಸ್ಮರಣೆ_ನಕಲು(into->leds, from->leds, ಗಾತ್ರ(*from->leds) * from->ಅಂಕೆ_leds);
        into->ಅಂಕೆ_leds = from->ಅಂಕೆ_leds;
        from->ಅಂಕೆ_leds = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t i = 0; i < from->ಅಂಕೆ_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_Led_Map(into, ledi, false))
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

    ಆದರೆ (Exceeds_Include_Max_ಆಳ(info->ctx, info->include_ಆಳ)) {
        info->error_ಎಣಿಕೆ += 10;
        ಹಿಂತಿರುಗಿ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_ಆಳ + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಗಾಗಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->ಮುಂದಿನ_incl) {
        CompatInfo ಮುಂದಿನ_incl;
        XkbFile *file;

        ಅಕ್ಷರ ಮಾರ್ಗ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ));
        ಆದರೆ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            ClearCompatInfo(&included);
            ಹಿಂತಿರುಗಿ false;
        }

        InitCompatInfo(&ಮುಂದಿನ_incl, info->keymap_info, info->include_ಆಳ + 1,
                       &included.mods);
        ಮುಂದಿನ_incl.default_interp = info->default_interp;
        ಮುಂದಿನ_incl.default_led = info->default_led;

        HandleCompatMapFile(&ಮುಂದಿನ_incl, file);

        ವಿಲೀನ_Included_Compat_Maps(&included, &ಮುಂದಿನ_incl, stmt->ವಿಲೀನ);

        ClearCompatInfo(&ಮುಂದಿನ_incl);
        FreeXkbFile(file);
    }

    ವಿಲೀನ_Included_Compat_Maps(info, &included, include->ವಿಲೀನ);
    ClearCompatInfo(&included);

    ಹಿಂತಿರುಗಿ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
ಗಣ_Interp_ಕ್ಷೇತ್ರ(CompatInfo *info, SymInterpInfo *si, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
               ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "action")) {
        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportSINotArray(info, si, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST) {
            ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_actions = 0;
            ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = ಮೌಲ್ಯ->actions.actions;
                 act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.ಮುಂದಿನ)
                 ಅಂಕೆ_actions++;

            ಆದರೆ (ಅಂಕೆ_actions > MAX_ACTIONS_ಪ್ರತಿ_LEVEL) {
                log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_ಪಠ್ಯ(si, info), MAX_ACTIONS_ಪ್ರತಿ_LEVEL, ಅಂಕೆ_actions);
                ಹಿಂತಿರುಗಿ false;
            }

            si->interp.ಅಂಕೆ_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(ಒಕ್ಕೂಟ xkb_action) actions = darray_new();
            ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = ಮೌಲ್ಯ->actions.actions;
                 act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.ಮುಂದಿನ) {
                ಒಕ್ಕೂಟ xkb_action ಗಮ್ಯ_Act = { 0 };
                ಆಯ್ಕೆ (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &ಗಮ್ಯ_Act)) {
                ಸ್ಥಿತಿ PARSER_RECOVERABLE_ERROR:
                    ಗಮ್ಯ_Act.type = ACTION_TYPE_NONE;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ಹಿಂತಿರುಗಿ false;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    ;
                }
                ಆದರೆ (ಗಮ್ಯ_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (likely(ಅಂಕೆ_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.ಅಂಕೆ_actions = 1;
                    si->interp.a.action = ಗಮ್ಯ_Act;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    darray_append(actions, ಗಮ್ಯ_Act);
                }
            }
            ಆಯ್ಕೆ (darray_ಗಾತ್ರ(actions)) {
            ಸ್ಥಿತಿ 0:
                /* No action or exactly one action: already processed */
                ಪ್ರತಿಜ್ಞಾ(si->interp.ಅಂಕೆ_actions <= 1);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 1:
                /* One action: some actions were dropped */
                si->interp.ಅಂಕೆ_actions = 1;
                si->interp.a.action = darray_ಅಂಶ(actions, 1);
                darray_free(actions);
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.ಅಂಕೆ_actions =
                    (xkb_action_ಎಣಿಕೆ_t) darray_ಗಾತ್ರ(actions);
                darray_steal(actions, &si->interp.a.actions, ಖಾಲಿ);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆಯ್ಕೆ (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, ಮೌಲ್ಯ, &si->interp.a.action)) {
            ಸ್ಥಿತಿ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.ಅಂಕೆ_actions = 0;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ PARSER_FATAL_ERROR:
                ಹಿಂತಿರುಗಿ false;
            ಪೂರ್ವನಿಯೋಜಿತ:
                si->interp.ಅಂಕೆ_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_ಕ್ಷೇತ್ರ_ACTION;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "virtualmodifier") ||
             istreq(ಕ್ಷೇತ್ರ_2, "virtualmod")) {
        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportSINotArray(info, si, ಕ್ಷೇತ್ರ_2);

        xkb_mod_ಸೂಚ್ಯಂಕ_t ndx = 0;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(info->ctx, ಮೌಲ್ಯ, MOD_VIRT, &info->mods, &ndx))
            ಹಿಂತಿರುಗಿ ReportSIBadType(info, si, ಕ್ಷೇತ್ರ_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_ಕ್ಷೇತ್ರ_VIRTUAL_MOD;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "repeat")) {
        bool ಗಣ = false;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportSINotArray(info, si, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &ಗಣ))
            ಹಿಂತಿರುಗಿ ReportSIBadType(info, si, ಕ್ಷೇತ್ರ_2, "boolean");

        si->interp.repeat = ಗಣ;

        si->defined |= SI_ಕ್ಷೇತ್ರ_AUTO_REPEAT;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "usemodmap") ||
             istreq(ಕ್ಷೇತ್ರ_2, "usemodmapmods")) {
        uint32_t val = 0;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportSINotArray(info, si, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(info->ctx, ಮೌಲ್ಯ, &val, use_Mod_Map_ಮೌಲ್ಯ_ಹೆಸರುಗಳು))
            ಹಿಂತಿರುಗಿ ReportSIBadType(info, si, ಕ್ಷೇತ್ರ_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_ಕ್ಷೇತ್ರ_LEVEL_ONE_ONLY;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        Report_Bad_ಕ್ಷೇತ್ರ(info->ctx, "symbol interpretation", ಕ್ಷೇತ್ರ_2,
                       si_ಪಠ್ಯ(si, info));
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಗಣ_Led_Map_ಕ್ಷೇತ್ರ(CompatInfo *info, LedInfo *ledi, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
               ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;

    ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "modifiers") || istreq(ಕ್ಷೇತ್ರ_2, "mods")) {
        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportLedNotArray(info, ledi, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ಹಿಂತಿರುಗಿ ReportLedBadType(info, ledi, ಕ್ಷೇತ್ರ_2, "modifier mask");

        ledi->defined |= LED_ಕ್ಷೇತ್ರ_MODS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "groups")) {
        xkb_layout_ಮುಖವಾಡ_t ಮುಖವಾಡ = 0;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportLedNotArray(info, ledi, ಕ್ಷೇತ್ರ_2);

        bool pending = false;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು_ಮುಖವಾಡ(info->keymap_info, ಮೌಲ್ಯ, &ಮುಖವಾಡ, &pending)) {
            ಆದರೆ (pending) {
                ledi->led.pending_groups = true;
                ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t pending_ಸೂಚ್ಯಂಕ =
                    darray_ಗಾತ್ರ(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ರಚನೆ pending_computation) {
                        .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                        .computed = false,
                        .ಮೌಲ್ಯ = 0,
                    }
                );
                *ಮೌಲ್ಯ_ptr = ಖಾಲಿ;
                static_assert(ಗಾತ್ರ(pending_ಸೂಚ್ಯಂಕ) == ಗಾತ್ರ(ಮುಖವಾಡ),
                              "Cannot save pending computation");
                ಮುಖವಾಡ = pending_ಸೂಚ್ಯಂಕ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ ReportLedBadType(info, ledi, ಕ್ಷೇತ್ರ_2, "group mask");
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = ಮುಖವಾಡ;
        ledi->defined |= LED_ಕ್ಷೇತ್ರ_GROUPS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "controls") || istreq(ಕ್ಷೇತ್ರ_2, "ctrls")) {
        uint32_t ಮುಖವಾಡ = 0;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportLedNotArray(info, ledi, ಕ್ಷೇತ್ರ_2);

        ಸ್ಥಿರಾಂಕ uint8_t ಸ್ಥಾನಾಂತರ = info->keymap_info->features.controls_name_ಸ್ಥಾನಾಂತರ;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, &ಮುಖವಾಡ, ctrl_ಮುಖವಾಡ_ಹೆಸರುಗಳು + ಸ್ಥಾನಾಂತರ))
            ಹಿಂತಿರುಗಿ ReportLedBadType(info, ledi, ಕ್ಷೇತ್ರ_2, "controls mask");

        ledi->led.ctrls = ಮುಖವಾಡ;
        ledi->defined |= LED_ಕ್ಷೇತ್ರ_CTRLS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "whichmodstate") ||
             istreq(ಕ್ಷೇತ್ರ_2, "whichmodifierstate")) {
        uint32_t ಮುಖವಾಡ = 0;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportLedNotArray(info, ledi, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, &ಮುಖವಾಡ,
                             mod_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು))
            ಹಿಂತಿರುಗಿ ReportLedBadType(info, ledi, ಕ್ಷೇತ್ರ_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = ಮುಖವಾಡ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "whichgroupstate")) {
        uint32_t ಮುಖವಾಡ = 0;

        ಆದರೆ (arrayNdx)
            ಹಿಂತಿರುಗಿ ReportLedNotArray(info, ledi, ಕ್ಷೇತ್ರ_2);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, &ಮುಖವಾಡ,
                             ಗುಂಪು_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು))
            ಹಿಂತಿರುಗಿ ReportLedBadType(info, ledi, ಕ್ಷೇತ್ರ_2,
                                    "mask of group state components");

        ledi->led.which_groups = ಮುಖವಾಡ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "driveskbd") ||
             istreq(ಕ್ಷೇತ್ರ_2, "driveskeyboard") ||
             istreq(ಕ್ಷೇತ್ರ_2, "leddriveskbd") ||
             istreq(ಕ್ಷೇತ್ರ_2, "leddriveskeyboard") ||
             istreq(ಕ್ಷೇತ್ರ_2, "indicatordriveskbd") ||
             istreq(ಕ್ಷೇತ್ರ_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", ಕ್ಷೇತ್ರ_2);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                ಕ್ಷೇತ್ರ_2, LEDText(info, ledi));
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ndx;
    bool ret;

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ಕ್ಷೇತ್ರ_2, &ndx)) {
        ret = false;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem && istreq(elem, "interpret")) {
        SymInterpInfo ತಾತ್ಕಾಲಿಕ = {0};
        InitInterp(&ತಾತ್ಕಾಲಿಕ);
        /* Do not replace the whole interpret, only the current field */
        ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ = (ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ == ವಿಲೀನ_REPLACE)
            ? ವಿಲೀನ_OVERRIDE
            : stmt->ವಿಲೀನ;
        ret = ಗಣ_Interp_ಕ್ಷೇತ್ರ(info, &ತಾತ್ಕಾಲಿಕ, ಕ್ಷೇತ್ರ_2, ndx, stmt->ಮೌಲ್ಯ);
        ಆದರೆ (ret)
            ವಿಲೀನ_Interp(info, &info->default_interp, &ತಾತ್ಕಾಲಿಕ, true);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem && istreq(elem, "indicator")) {
        LedInfo ತಾತ್ಕಾಲಿಕ = {0};
        InitLED(&ತಾತ್ಕಾಲಿಕ);
        /* Do not replace the whole LED, only the current field */
        ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ = (ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ == ವಿಲೀನ_REPLACE)
            ? ವಿಲೀನ_OVERRIDE
            : stmt->ವಿಲೀನ;
        ret = ಗಣ_Led_Map_ಕ್ಷೇತ್ರ(info, &ತಾತ್ಕಾಲಿಕ, ಕ್ಷೇತ್ರ_2, ndx, &stmt->ಮೌಲ್ಯ);
        ಆದರೆ (ret)
            ವಿಲೀನ_Led_Map(info, &info->default_led, &ತಾತ್ಕಾಲಿಕ, true);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem) {
        ret = (ಗಣ_Default_Action_ಕ್ಷೇತ್ರ(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ಕ್ಷೇತ್ರ_2, ndx,
                                     &stmt->ಮೌಲ್ಯ, stmt->ವಿಲೀನ) !=
               PARSER_FATAL_ERROR);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ಕ್ಷೇತ್ರ,
                "Default defined for unknown field \"%s\"; Ignored\n", ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಗಾಗಿ (; def; def = (VarDef *) def->common.ಮುಂದಿನ) {
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &ಕ್ಷೇತ್ರ_2, &arrayNdx)) {
            ok = false;
            ಮುಂದುವರಿ;
        }
        ಆದರೆ (elem) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, ಕ್ಷೇತ್ರ_2);
            ok = false;
            ಮುಂದುವರಿ;
        }
        ಆದರೆ (!ಗಣ_Interp_ಕ್ಷೇತ್ರ(info, si, ಕ್ಷೇತ್ರ_2, arrayNdx, def->ಮೌಲ್ಯ))
            ok = false;
    }

    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation pred;
    xkb_mod_ಮುಖವಾಡ_t mods;
    SymInterpInfo si;

    ಆದರೆ (!ResolveStateAndPredicate(def->ಹೊಂದಿಕೆ, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ಹಿಂತಿರುಗಿ false;
    }

    si = info->default_interp;
    si.ವಿಲೀನ = def->ವಿಲೀನ;
    si.interp.sym = def->sym;
    si.interp.ಹೊಂದಿಕೆ = pred;
    si.interp.mods = mods;

    ಆದರೆ (!HandleInterpBody(info, def->def, &si)) {
        info->error_ಎಣಿಕೆ++;
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (!ಕೂಡಿಸು_Interp(info, &si, true)) {
        info->error_ಎಣಿಕೆ++;
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.ವಿಲೀನ = def->ವಿಲೀನ;
    ledi.led.name = def->name;

    bool ok = true;
    ಗಾಗಿ (VarDef *var = def->body; var != ಖಾಲಿ; var = (VarDef *) var->common.ಮುಂದಿನ) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
        ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, var->name, &elem, &ಕ್ಷೇತ್ರ_2, &arrayNdx)) {
            ok = false;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, ಕ್ಷೇತ್ರ_2);
            ok = false;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (!ಗಣ_Led_Map_ಕ್ಷೇತ್ರ(info, &ledi, ಕ್ಷೇತ್ರ_2, arrayNdx, &var->ಮೌಲ್ಯ))
                ok = false;
        }
    }

    ಹಿಂತಿರುಗಿ ok && ಕೂಡಿಸು_Led_Map(info, &ledi, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    ಬಿಡುಗಡೆ(info->name);
    info->name = strdup_ಸುರಕ್ಷಿತ(file->name);

    ಗಾಗಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ಮುಂದಿನ) {
        ಆಯ್ಕೆ (stmt->type) {
        ಸ್ಥಿತಿ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಗುಂಪು_COMPAT:
            log_dbg(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_UNKNOWN_DECLARATION:
        ಸ್ಥಿತಿ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_ಗಮ್ಯ_string(stmt->type));
            ok = false;
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಆದರೆ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    ಸುರಕ್ಷಿತ_map_name(file->name));
            ನಿಲ್ಲಿಸು;
        }
    }
}

/* Temporary struct for CopyInterps. */
ರಚನೆ collect {
    darray(ರಚನೆ xkb_sym_interpret) sym_interprets;
};

ಸ್ಥಿರ ಶೂನ್ಯ
CopyInterps(CompatInfo *info, bool need_ಚಿಹ್ನೆ, ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation pred,
            ರಚನೆ collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ಆದರೆ (si->interp.ಹೊಂದಿಕೆ == pred &&
            (si->interp.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) == need_ಚಿಹ್ನೆ)
            darray_append(collect->sym_interprets, si->interp);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Copy_Led_Map_Defs_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, CompatInfo *info)
{
    ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t idx = 0; idx < info->ಅಂಕೆ_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_ಸೂಚ್ಯಂಕ_t i;
        ರಚನೆ xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ಆದರೆ (led->name == ledi->led.name)
                ನಿಲ್ಲಿಸು;

        /* Not previously declared; create it with next free index. */
        ಆದರೆ (i >= keymap->ಅಂಕೆ_leds) {
            log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ಆದರೆ (led->name == XKB_ATOM_NONE)
                    ನಿಲ್ಲಿಸು;

            ಆದರೆ (i >= keymap->ಅಂಕೆ_leds) {
                /* Not place to put it; ignore. */
                ಆದರೆ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ಮುಂದುವರಿ;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->ಅಂಕೆ_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ಆದರೆ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ಆದರೆ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ಸ್ಥಿರ bool
Copy_Compat_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_ಸುರಕ್ಷಿತ(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ಆದರೆ (!darray_ಖಾಲಿಗಳು(info->interps)) {
        ರಚನೆ collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, ಹೊಂದಿಕೆ_EXACTLY, &collect);
        CopyInterps(info, true, ಹೊಂದಿಕೆ_ALL, &collect);
        CopyInterps(info, true, ಹೊಂದಿಕೆ_NONE, &collect);
        CopyInterps(info, true, ಹೊಂದಿಕೆ_ANY, &collect);
        CopyInterps(info, true, ಹೊಂದಿಕೆ_ANY_OR_NONE, &collect);
        CopyInterps(info, false, ಹೊಂದಿಕೆ_EXACTLY, &collect);
        CopyInterps(info, false, ಹೊಂದಿಕೆ_ALL, &collect);
        CopyInterps(info, false, ಹೊಂದಿಕೆ_NONE, &collect);
        CopyInterps(info, false, ಹೊಂದಿಕೆ_ANY, &collect);
        CopyInterps(info, false, ಹೊಂದಿಕೆ_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->ಅಂಕೆ_sym_interprets);
    }

    Copy_Led_Map_Defs_ಗಮ್ಯ_Keymap(keymap, info);

    ಹಿಂತಿರುಗಿ true;
}

bool
CompileCompatMap(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ಆದರೆ (file != ಖಾಲಿ)
        HandleCompatMapFile(&info, file);

    ಆದರೆ (info.error_ಎಣಿಕೆ != 0)
        ಹೋಗು err_info;

    ಆದರೆ (!Copy_Compat_ಗಮ್ಯ_Keymap(&keymap_info->keymap, &info))
        ಹೋಗು err_info;

    ClearCompatInfo(&info);
    ಹಿಂತಿರುಗಿ true;

err_info:
    ClearCompatInfo(&info);
    ಹಿಂತಿರುಗಿ false;
}
