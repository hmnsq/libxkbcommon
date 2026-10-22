/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "action.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "vmod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"

ਗਿਣਤੀ si_ਖੇਤਰ {
    SI_ਖੇਤਰ_VIRTUAL_MOD = (1 << 0),
    SI_ਖੇਤਰ_ACTION = (1 << 1),
    SI_ਖੇਤਰ_AUTO_REPEAT = (1 << 2),
    SI_ਖੇਤਰ_LEVEL_ONE_ONLY = (1 << 3),
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਗਿਣਤੀ si_ਖੇਤਰ defined;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;

    ਬਣਤਰ xkb_sym_interpret interp;
} SymInterpInfo;

ਗਿਣਤੀ led_ਖੇਤਰ {
    LED_ਖੇਤਰ_MODS = (1 << 0),
    LED_ਖੇਤਰ_GROUPS = (1 << 1),
    LED_ਖੇਤਰ_CTRLS = (1 << 2),
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਗਿਣਤੀ led_ਖੇਤਰ defined;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;

    ਬਣਤਰ xkb_led led;
} LedInfo;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ *name;
    ਪੂਰਨ error_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_ਸੂਚਕਾਂਕ_t ਅੰਕ_leds;
    ActionsInfo default_actions;
    ਬਣਤਰ xkb_mod_ਸੈੱਟ mods;

    ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info;
    ਬਣਤਰ xkb_context *ctx;
} CompatInfo;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
si_ਪਾਠ(SymInterpInfo *si, CompatInfo *info)
{
    ਅੱਖਰ *buf = xkb_context_get_ਬਫਰ(info->ctx, 128);

    ਜੇ (si == &info->default_interp)
        ਵਾਪਸ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             Keysym_ਪਾਠ(info->ctx, si->interp.sym),
             SIMatch_ਪਾਠ(si->interp.ਮੇਲ),
             Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ਵਾਪਸ buf;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2)
{
    ਵਾਪਸ ReportNotArray(info->ctx, "symbol interpretation", ਖੇਤਰ_2,
                          si_ਪਾਠ(si, info));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
                ਅਚਲ ਅੱਖਰ *wanted)
{
    ਵਾਪਸ ReportBadType(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                         "symbol interpretation", ਖੇਤਰ_2,
                         si_ਪਾਠ(si, info), wanted);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ਜੇ (ledi == &info->default_led) {
        ਪ੍ਰਤਿਜ੍ਞਾ(xkb_atom_ਪਾਠ(info->ctx, ledi->led.name) == ਨਿਰਾ);
        ਵਾਪਸ "default";
    } ਨਹੀਂਤਾਂ {
        ਪ੍ਰਤਿਜ੍ਞਾ(xkb_atom_ਪਾਠ(info->ctx, ledi->led.name) != ਨਿਰਾ);
        ਵਾਪਸ xkb_atom_ਪਾਠ(info->ctx, ledi->led.name);
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
                 ਅਚਲ ਅੱਖਰ *wanted)
{
    ਵਾਪਸ ReportBadType(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                         "indicator map",
                         ਖੇਤਰ_2, LEDText(info, ledi), wanted);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2)
{
    ਵਾਪਸ ReportNotArray(info->ctx, "indicator map", ਖੇਤਰ_2,
                          LEDText(info, ledi));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
InitInterp(SymInterpInfo *info)
{
    info->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT; /*ਅਣਵਰਤਿਆ*/
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ਕੁੰਜੀ_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ਕੁੰਜੀ_REPEAT, "invalid initial value");
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
InitLED(LedInfo *info)
{
    info->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT; /*ਅਣਵਰਤਿਆ*/
}

ਸਥਿਰ ਖਾਲੀ
InitCompatInfo(CompatInfo *info, ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
               ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods)
{
    memset(info, 0, ਆਕਾਰ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ਡੂੰਘਾਈ = include_ਡੂੰਘਾਈ;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ਡੂੰਘਾਈ > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ਸਥਿਰ ਖਾਲੀ
ClearCompatInfo(CompatInfo *info)
{
    ਮੁਕਤ(info->name);
    darray_free(info->interps);
}

ਸਥਿਰ SymInterpInfo *
ਲੱਭੋ_Matching_Interp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ਜੇ (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.ਮੇਲ == new->interp.ਮੇਲ)
            ਵਾਪਸ old;

    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ bool
Use_New_Interp_ਖੇਤਰ(ਗਿਣਤੀ si_ਖੇਤਰ ਖੇਤਰ_2, ਗਿਣਤੀ si_ਖੇਤਰ old, ਗਿਣਤੀ si_ਖੇਤਰ new,
                  bool clobber, bool report, ਗਿਣਤੀ si_ਖੇਤਰ *collide)
{
    ਜੇ (!(old & ਖੇਤਰ_2))
        ਵਾਪਸ (new & ਖੇਤਰ_2);

    ਜੇ (new & ਖੇਤਰ_2) {
        ਜੇ (report)
            *collide |= ਖੇਤਰ_2;

        ਵਾਪਸ clobber;
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ bool
ਮਿਲਾਓ_Interp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ਅਚਲ bool clobber = (new->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
    ਅਚਲ ਪੂਰਨ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ਅਚਲ bool report = (same_file && verbosity > 0) || verbosity > 9;
    ਗਿਣਤੀ si_ਖੇਤਰ collide = 0;

    ਜੇ (new->ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE) {
        ਜੇ (report)
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     si_ਪਾਠ(new, info));
        *old = *new;
        ਵਾਪਸ true;
    }

    ਜੇ (Use_New_Interp_ਖੇਤਰ(SI_ਖੇਤਰ_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_ਖੇਤਰ_VIRTUAL_MOD;
    }
    ਜੇ (Use_New_Interp_ਖੇਤਰ(SI_ਖੇਤਰ_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ਜੇ (old->interp.ਅੰਕ_actions > 1) {
                ਮੁਕਤ(old->interp.a.actions);
            }
            old->interp.ਅੰਕ_actions = new->interp.ਅੰਕ_actions;
            ਜੇ (new->interp.ਅੰਕ_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (ਸੰਘ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.ਅੰਕ_actions = 0;
            } ਨਹੀਂਤਾਂ {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_ਖੇਤਰ_ACTION;
    }
    ਜੇ (Use_New_Interp_ਖੇਤਰ(SI_ਖੇਤਰ_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_ਖੇਤਰ_AUTO_REPEAT;
    }
    ਜੇ (Use_New_Interp_ਖੇਤਰ(SI_ਖੇਤਰ_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_ਖੇਤਰ_LEVEL_ONE_ONLY;
    }

    ਜੇ (collide) {
        log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 si_ਪਾਠ(old, info),
                 (clobber ? "last" : "first"));
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਜੋੜੋ_Interp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = ਲੱਭੋ_Matching_Interp(info, new);
    ਜੇ (old)
        ਵਾਪਸ ਮਿਲਾਓ_Interp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ਵਾਪਸ true;
}

/***====================================================================***/

// TODO: more lenient?
ਸਥਿਰ bool
ResolveStateAndPredicate(ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, ਗਿਣਤੀ xkb_ਮੇਲ_operation *pred_rtrn,
                         xkb_mod_ਨਕਾਬ_t *mods_rtrn, CompatInfo *info)
{
    ਜੇ (ਵਿਅੰਜਕ == ਨਿਰਾ) {
        *pred_rtrn = ਮੇਲ_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_ਨਕਾਬ_ALL;
        ਵਾਪਸ true;
    }

    *pred_rtrn = ਮੇਲ_EXACTLY;
    ਜੇ (ਵਿਅੰਜਕ->common.type == STMT_ਵਿਅੰਜਕ_ACTION_DECL) {
        ਅਚਲ ਅੱਖਰ *pred_txt = xkb_atom_ਪਾਠ(info->ctx, ਵਿਅੰਜਕ->action.name);
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pred = 0;
        ਜੇ (!LookupString(sym_Interpret_ਮੇਲ_ਨਕਾਬ_ਨਾਂ, pred_txt, &pred) ||
            !ਵਿਅੰਜਕ->action.ਦਲੀਲਾਂ || ਵਿਅੰਜਕ->action.ਦਲੀਲਾਂ->common.ਅਗਲਾ) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ਵਾਪਸ false;
        }
        *pred_rtrn = (ਗਿਣਤੀ xkb_ਮੇਲ_operation) pred;
        ਵਿਅੰਜਕ = ਵਿਅੰਜਕ->action.ਦਲੀਲਾਂ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਵਿਅੰਜਕ->common.type == STMT_ਵਿਅੰਜਕ_IDENT) {
        ਅਚਲ ਅੱਖਰ *pred_txt = xkb_atom_ਪਾਠ(info->ctx, ਵਿਅੰਜਕ->ident.ident);
        ਜੇ (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = ਮੇਲ_ANY;
            *mods_rtrn = MOD_REAL_ਨਕਾਬ_ALL;
            ਵਾਪਸ true;
        }
    }

    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, ਵਿਅੰਜਕ, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ਸਥਿਰ bool
UseNewLEDField(ਗਿਣਤੀ led_ਖੇਤਰ ਖੇਤਰ_2, ਗਿਣਤੀ led_ਖੇਤਰ old, ਗਿਣਤੀ led_ਖੇਤਰ new,
               bool clobber, bool report, ਗਿਣਤੀ led_ਖੇਤਰ *collide)
{
    ਜੇ (!(old & ਖੇਤਰ_2))
        ਵਾਪਸ (new & ਖੇਤਰ_2);

    ਜੇ (new & ਖੇਤਰ_2) {
        ਜੇ (report)
            *collide |= ਖੇਤਰ_2;

        ਵਾਪਸ clobber;
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ bool
ਮਿਲਾਓ_Led_Map(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ਗਿਣਤੀ led_ਖੇਤਰ collide;
    ਅਚਲ bool clobber = (new->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
    ਅਚਲ ਪੂਰਨ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ਅਚਲ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ਜੇ (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ਵਾਪਸ true;
    }

    ਜੇ (new->ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE) {
        ਜੇ (report)
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ਵਾਪਸ true;
    }

    collide = 0;
    ਜੇ (UseNewLEDField(LED_ਖੇਤਰ_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_ਖੇਤਰ_MODS;
    }
    ਜੇ (UseNewLEDField(LED_ਖੇਤਰ_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_ਖੇਤਰ_GROUPS;
    }
    ਜੇ (UseNewLEDField(LED_ਖੇਤਰ_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_ਖੇਤਰ_CTRLS;
    }

    ਜੇ (collide) {
        log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਜੋੜੋ_Led_Map(CompatInfo *info, LedInfo *new, bool same_file)
{
    ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t i = 0; i < info->ਅੰਕ_leds; i++) {
        LedInfo *old = &info->leds[i];

        ਜੇ (old->led.name != new->led.name)
            ਜਾਰੀ;

        ਵਾਪਸ ਮਿਲਾਓ_Led_Map(info, old, new, same_file);
    }

    ਜੇ (info->ਅੰਕ_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ਵਾਪਸ false;
    }
    info->leds[info->ਅੰਕ_leds++] = *new;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_Included_Compat_Maps(CompatInfo *into, CompatInfo *from,
                        ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    ਜੇ (from->error_ਗਿਣਤੀ > 0) {
        into->error_ਗਿਣਤੀ += from->error_ਗਿਣਤੀ;
        ਵਾਪਸ;
    }

    ਮਿਲਾਓ_Mod_Sets(into->ctx, &into->mods, &from->mods, ਮਿਲਾਓ);

    ਜੇ (into->name == ਨਿਰਾ) {
        into->name = steal(&from->name);
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ਨਹੀਂਤਾਂ {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_Interp(into, si, false))
                into->error_ਗਿਣਤੀ++;
        }
    }

    ਜੇ (into->ਅੰਕ_leds == 0) {
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(into->leds, from->leds, ਆਕਾਰ(*from->leds) * from->ਅੰਕ_leds);
        into->ਅੰਕ_leds = from->ਅੰਕ_leds;
        from->ਅੰਕ_leds = 0;
    }
    ਨਹੀਂਤਾਂ {
        ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t i = 0; i < from->ਅੰਕ_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_Led_Map(into, ledi, false))
                into->error_ਗਿਣਤੀ++;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ਸਥਿਰ bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ਜੇ (Exceeds_Include_Max_ਡੂੰਘਾਈ(info->ctx, info->include_ਡੂੰਘਾਈ)) {
        info->error_ਗਿਣਤੀ += 10;
        ਵਾਪਸ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ਲਈ (IncludeStmt *stmt = include; stmt; stmt = stmt->ਅਗਲਾ_incl) {
        CompatInfo ਅਗਲਾ_incl;
        XkbFile *file;

        ਅੱਖਰ ਰਸਤਾ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ));
        ਜੇ (!file) {
            info->error_ਗਿਣਤੀ += 10;
            ClearCompatInfo(&included);
            ਵਾਪਸ false;
        }

        InitCompatInfo(&ਅਗਲਾ_incl, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                       &included.mods);
        ਅਗਲਾ_incl.default_interp = info->default_interp;
        ਅਗਲਾ_incl.default_led = info->default_led;

        HandleCompatMapFile(&ਅਗਲਾ_incl, file);

        ਮਿਲਾਓ_Included_Compat_Maps(&included, &ਅਗਲਾ_incl, stmt->ਮਿਲਾਓ);

        ClearCompatInfo(&ਅਗਲਾ_incl);
        FreeXkbFile(file);
    }

    ਮਿਲਾਓ_Included_Compat_Maps(info, &included, include->ਮਿਲਾਓ);
    ClearCompatInfo(&included);

    ਵਾਪਸ (info->error_ਗਿਣਤੀ == 0);
}

ਸਥਿਰ bool
ਸੈੱਟ_Interp_ਖੇਤਰ(CompatInfo *info, SymInterpInfo *si, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
               ਵਿਅੰਜਕ_Def *arrayNdx, ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    ਜੇ (istreq(ਖੇਤਰ_2, "action")) {
        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportSINotArray(info, si, ਖੇਤਰ_2);

        ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_ACTION_LIST) {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_actions = 0;
            ਲਈ (ਵਿਅੰਜਕ_Def *act = ਮੁੱਲ->actions.actions;
                 act; act = (ਵਿਅੰਜਕ_Def *) act->common.ਅਗਲਾ)
                 ਅੰਕ_actions++;

            ਜੇ (ਅੰਕ_actions > MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL) {
                log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        si_ਪਾਠ(si, info), MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL, ਅੰਕ_actions);
                ਵਾਪਸ false;
            }

            si->interp.ਅੰਕ_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(ਸੰਘ xkb_action) actions = darray_new();
            ਲਈ (ਵਿਅੰਜਕ_Def *act = ਮੁੱਲ->actions.actions;
                 act; act = (ਵਿਅੰਜਕ_Def *) act->common.ਅਗਲਾ) {
                ਸੰਘ xkb_action ਮੰਜ਼ਿਲ_Act = { 0 };
                ਚੋਣ (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &ਮੰਜ਼ਿਲ_Act)) {
                ਹਾਲਤ PARSER_RECOVERABLE_ERROR:
                    ਮੰਜ਼ਿਲ_Act.type = ACTION_TYPE_NONE;
                    ਰੋਕੋ;
                ਹਾਲਤ PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ਵਾਪਸ false;
                ਮੂਲ:
                    ;
                }
                ਜੇ (ਮੰਜ਼ਿਲ_Act.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ਨਹੀਂਤਾਂ ਜੇ (likely(ਅੰਕ_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.ਅੰਕ_actions = 1;
                    si->interp.a.action = ਮੰਜ਼ਿਲ_Act;
                } ਨਹੀਂਤਾਂ {
                    darray_append(actions, ਮੰਜ਼ਿਲ_Act);
                }
            }
            ਚੋਣ (darray_ਮਾਪ(actions)) {
            ਹਾਲਤ 0:
                /* No action or exactly one action: already processed */
                ਪ੍ਰਤਿਜ੍ਞਾ(si->interp.ਅੰਕ_actions <= 1);
                ਰੋਕੋ;
            ਹਾਲਤ 1:
                /* One action: some actions were dropped */
                si->interp.ਅੰਕ_actions = 1;
                si->interp.a.action = darray_ਵਸਤੂ(actions, 1);
                darray_free(actions);
                ਰੋਕੋ;
            ਮੂਲ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.ਅੰਕ_actions =
                    (xkb_action_ਗਿਣਤੀ_t) darray_ਮਾਪ(actions);
                darray_steal(actions, &si->interp.a.actions, ਨਿਰਾ);
            }
        }
        ਨਹੀਂਤਾਂ {
            ਚੋਣ (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, ਮੁੱਲ, &si->interp.a.action)) {
            ਹਾਲਤ PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.ਅੰਕ_actions = 0;
                ਰੋਕੋ;
            ਹਾਲਤ PARSER_FATAL_ERROR:
                ਵਾਪਸ false;
            ਮੂਲ:
                si->interp.ਅੰਕ_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_ਖੇਤਰ_ACTION;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "virtualmodifier") ||
             istreq(ਖੇਤਰ_2, "virtualmod")) {
        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportSINotArray(info, si, ਖੇਤਰ_2);

        xkb_mod_ਸੂਚਕਾਂਕ_t ndx = 0;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod(info->ctx, ਮੁੱਲ, MOD_VIRT, &info->mods, &ndx))
            ਵਾਪਸ ReportSIBadType(info, si, ਖੇਤਰ_2, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_ਖੇਤਰ_VIRTUAL_MOD;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "repeat")) {
        bool ਸੈੱਟ = false;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportSINotArray(info, si, ਖੇਤਰ_2);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Boolean(info->ctx, ਮੁੱਲ, &ਸੈੱਟ))
            ਵਾਪਸ ReportSIBadType(info, si, ਖੇਤਰ_2, "boolean");

        si->interp.repeat = ਸੈੱਟ;

        si->defined |= SI_ਖੇਤਰ_AUTO_REPEAT;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "locking")) {
        log_dbg(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "usemodmap") ||
             istreq(ਖੇਤਰ_2, "usemodmapmods")) {
        uint32_t val = 0;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportSINotArray(info, si, ਖੇਤਰ_2);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Enum(info->ctx, ਮੁੱਲ, &val, use_Mod_Map_ਮੁੱਲ_ਨਾਂ))
            ਵਾਪਸ ReportSIBadType(info, si, ਖੇਤਰ_2, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_ਖੇਤਰ_LEVEL_ONE_ONLY;
    }
    ਨਹੀਂਤਾਂ {
        Report_Bad_ਖੇਤਰ(info->ctx, "symbol interpretation", ਖੇਤਰ_2,
                       si_ਪਾਠ(si, info));
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸੈੱਟ_Led_Map_ਖੇਤਰ(CompatInfo *info, LedInfo *ledi, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
               ਵਿਅੰਜਕ_Def *arrayNdx, ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਵਿਅੰਜਕ_Def * ਸੀਮਿਤ ਅਚਲ ਮੁੱਲ = *ਮੁੱਲ_ptr;

    ਜੇ (istreq(ਖੇਤਰ_2, "modifiers") || istreq(ਖੇਤਰ_2, "mods")) {
        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportLedNotArray(info, ledi, ਖੇਤਰ_2);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, ਮੁੱਲ, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ਵਾਪਸ ReportLedBadType(info, ledi, ਖੇਤਰ_2, "modifier mask");

        ledi->defined |= LED_ਖੇਤਰ_MODS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "groups")) {
        xkb_layout_ਨਕਾਬ_t ਨਕਾਬ = 0;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportLedNotArray(info, ledi, ਖੇਤਰ_2);

        bool pending = false;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ_ਨਕਾਬ(info->keymap_info, ਮੁੱਲ, &ਨਕਾਬ, &pending)) {
            ਜੇ (pending) {
                ledi->led.pending_groups = true;
                ਅਚਲ darray_ਮਾਪ_t pending_ਸੂਚਕਾਂਕ =
                    darray_ਮਾਪ(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ਬਣਤਰ pending_computation) {
                        .ਵਿਅੰਜਕ = *ਮੁੱਲ_ptr,
                        .computed = false,
                        .ਮੁੱਲ = 0,
                    }
                );
                *ਮੁੱਲ_ptr = ਨਿਰਾ;
                static_assert(ਆਕਾਰ(pending_ਸੂਚਕਾਂਕ) == ਆਕਾਰ(ਨਕਾਬ),
                              "Cannot save pending computation");
                ਨਕਾਬ = pending_ਸੂਚਕਾਂਕ;
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ ReportLedBadType(info, ledi, ਖੇਤਰ_2, "group mask");
            }
        } ਨਹੀਂਤਾਂ {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = ਨਕਾਬ;
        ledi->defined |= LED_ਖੇਤਰ_GROUPS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "controls") || istreq(ਖੇਤਰ_2, "ctrls")) {
        uint32_t ਨਕਾਬ = 0;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportLedNotArray(info, ledi, ਖੇਤਰ_2);

        ਅਚਲ uint8_t ਵਿੱਥ = info->keymap_info->features.controls_name_ਵਿੱਥ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(info->ctx, ਮੁੱਲ, &ਨਕਾਬ, ctrl_ਨਕਾਬ_ਨਾਂ + ਵਿੱਥ))
            ਵਾਪਸ ReportLedBadType(info, ledi, ਖੇਤਰ_2, "controls mask");

        ledi->led.ctrls = ਨਕਾਬ;
        ledi->defined |= LED_ਖੇਤਰ_CTRLS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "whichmodstate") ||
             istreq(ਖੇਤਰ_2, "whichmodifierstate")) {
        uint32_t ਨਕਾਬ = 0;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportLedNotArray(info, ledi, ਖੇਤਰ_2);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(info->ctx, ਮੁੱਲ, &ਨਕਾਬ,
                             mod_Component_ਨਕਾਬ_ਨਾਂ))
            ਵਾਪਸ ReportLedBadType(info, ledi, ਖੇਤਰ_2,
                                    "mask of modifier state components");

        ledi->led.which_mods = ਨਕਾਬ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "whichgroupstate")) {
        uint32_t ਨਕਾਬ = 0;

        ਜੇ (arrayNdx)
            ਵਾਪਸ ReportLedNotArray(info, ledi, ਖੇਤਰ_2);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(info->ctx, ਮੁੱਲ, &ਨਕਾਬ,
                             ਸਮੂਹ_Component_ਨਕਾਬ_ਨਾਂ))
            ਵਾਪਸ ReportLedBadType(info, ledi, ਖੇਤਰ_2,
                                    "mask of group state components");

        ledi->led.which_groups = ਨਕਾਬ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "driveskbd") ||
             istreq(ਖੇਤਰ_2, "driveskeyboard") ||
             istreq(ਖੇਤਰ_2, "leddriveskbd") ||
             istreq(ਖੇਤਰ_2, "leddriveskeyboard") ||
             istreq(ਖੇਤਰ_2, "indicatordriveskbd") ||
             istreq(ਖੇਤਰ_2, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", ਖੇਤਰ_2);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ਨਹੀਂਤਾਂ {
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                ਖੇਤਰ_2, LEDText(info, ledi));
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *ndx;
    bool ret;

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ਖੇਤਰ_2, &ndx)) {
        ret = false;
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem && istreq(elem, "interpret")) {
        SymInterpInfo ਆਰਜ਼ੀ = {0};
        InitInterp(&ਆਰਜ਼ੀ);
        /* Do not replace the whole interpret, only the current field */
        ਆਰਜ਼ੀ.ਮਿਲਾਓ = (ਆਰਜ਼ੀ.ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE)
            ? ਮਿਲਾਓ_OVERRIDE
            : stmt->ਮਿਲਾਓ;
        ret = ਸੈੱਟ_Interp_ਖੇਤਰ(info, &ਆਰਜ਼ੀ, ਖੇਤਰ_2, ndx, stmt->ਮੁੱਲ);
        ਜੇ (ret)
            ਮਿਲਾਓ_Interp(info, &info->default_interp, &ਆਰਜ਼ੀ, true);
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem && istreq(elem, "indicator")) {
        LedInfo ਆਰਜ਼ੀ = {0};
        InitLED(&ਆਰਜ਼ੀ);
        /* Do not replace the whole LED, only the current field */
        ਆਰਜ਼ੀ.ਮਿਲਾਓ = (ਆਰਜ਼ੀ.ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE)
            ? ਮਿਲਾਓ_OVERRIDE
            : stmt->ਮਿਲਾਓ;
        ret = ਸੈੱਟ_Led_Map_ਖੇਤਰ(info, &ਆਰਜ਼ੀ, ਖੇਤਰ_2, ndx, &stmt->ਮੁੱਲ);
        ਜੇ (ret)
            ਮਿਲਾਓ_Led_Map(info, &info->default_led, &ਆਰਜ਼ੀ, true);
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem) {
        ret = (ਸੈੱਟ_Default_Action_ਖੇਤਰ(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ਖੇਤਰ_2, ndx,
                                     &stmt->ਮੁੱਲ, stmt->ਮਿਲਾਓ) !=
               PARSER_FATAL_ERROR);
    }
    ਨਹੀਂਤਾਂ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ਖੇਤਰ,
                "Default defined for unknown field \"%s\"; Ignored\n", ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ਵਾਪਸ ret;
}

ਸਥਿਰ bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *arrayNdx;

    ਲਈ (; def; def = (VarDef *) def->common.ਅਗਲਾ) {
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, def->name, &elem, &ਖੇਤਰ_2, &arrayNdx)) {
            ok = false;
            ਜਾਰੀ;
        }
        ਜੇ (elem) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, ਖੇਤਰ_2);
            ok = false;
            ਜਾਰੀ;
        }
        ਜੇ (!ਸੈੱਟ_Interp_ਖੇਤਰ(info, si, ਖੇਤਰ_2, arrayNdx, def->ਮੁੱਲ))
            ok = false;
    }

    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ਗਿਣਤੀ xkb_ਮੇਲ_operation pred;
    xkb_mod_ਨਕਾਬ_t mods;
    SymInterpInfo si;

    ਜੇ (!ResolveStateAndPredicate(def->ਮੇਲ, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ਵਾਪਸ false;
    }

    si = info->default_interp;
    si.ਮਿਲਾਓ = def->ਮਿਲਾਓ;
    si.interp.sym = def->sym;
    si.interp.ਮੇਲ = pred;
    si.interp.mods = mods;

    ਜੇ (!HandleInterpBody(info, def->def, &si)) {
        info->error_ਗਿਣਤੀ++;
        ਵਾਪਸ false;
    }

    ਜੇ (!ਜੋੜੋ_Interp(info, &si, true)) {
        info->error_ਗਿਣਤੀ++;
        ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.ਮਿਲਾਓ = def->ਮਿਲਾਓ;
    ledi.led.name = def->name;

    bool ok = true;
    ਲਈ (VarDef *var = def->body; var != ਨਿਰਾ; var = (VarDef *) var->common.ਅਗਲਾ) {
        ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
        ਵਿਅੰਜਕ_Def *arrayNdx;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, var->name, &elem, &ਖੇਤਰ_2, &arrayNdx)) {
            ok = false;
            ਜਾਰੀ;
        }

        ਜੇ (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, ਖੇਤਰ_2);
            ok = false;
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ (!ਸੈੱਟ_Led_Map_ਖੇਤਰ(info, &ledi, ਖੇਤਰ_2, arrayNdx, &var->ਮੁੱਲ))
                ok = false;
        }
    }

    ਵਾਪਸ ok && ਜੋੜੋ_Led_Map(info, &ledi, true);
}

ਸਥਿਰ ਖਾਲੀ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    ਮੁਕਤ(info->name);
    info->name = strdup_ਸੁਰੱਖਿਅਤ(file->name);

    ਲਈ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ਅਗਲਾ) {
        ਚੋਣ (stmt->type) {
        ਹਾਲਤ STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਸਮੂਹ_COMPAT:
            log_dbg(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ਰੋਕੋ;
        ਹਾਲਤ STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_UNKNOWN_DECLARATION:
        ਹਾਲਤ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ਰੋਕੋ;
        ਮੂਲ:
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_ਮੰਜ਼ਿਲ_string(stmt->type));
            ok = false;
            ਰੋਕੋ;
        }

        ਜੇ (!ok)
            info->error_ਗਿਣਤੀ++;

        ਜੇ (info->error_ਗਿਣਤੀ > 10) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    ਸੁਰੱਖਿਅਤ_map_name(file->name));
            ਰੋਕੋ;
        }
    }
}

/* Temporary struct for CopyInterps. */
ਬਣਤਰ collect {
    darray(ਬਣਤਰ xkb_sym_interpret) sym_interprets;
};

ਸਥਿਰ ਖਾਲੀ
CopyInterps(CompatInfo *info, bool need_ਚਿੰਨ੍ਹ, ਗਿਣਤੀ xkb_ਮੇਲ_operation pred,
            ਬਣਤਰ collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ਜੇ (si->interp.ਮੇਲ == pred &&
            (si->interp.sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) == need_ਚਿੰਨ੍ਹ)
            darray_append(collect->sym_interprets, si->interp);
}

ਸਥਿਰ ਖਾਲੀ
Copy_Led_Map_Defs_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, CompatInfo *info)
{
    ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t idx = 0; idx < info->ਅੰਕ_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_ਸੂਚਕਾਂਕ_t i;
        ਬਣਤਰ xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ਜੇ (led->name == ledi->led.name)
                ਰੋਕੋ;

        /* Not previously declared; create it with next free index. */
        ਜੇ (i >= keymap->ਅੰਕ_leds) {
            log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ਜੇ (led->name == XKB_ATOM_NONE)
                    ਰੋਕੋ;

            ਜੇ (i >= keymap->ਅੰਕ_leds) {
                /* Not place to put it; ignore. */
                ਜੇ (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ਜਾਰੀ;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->ਅੰਕ_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ਜੇ (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ਜੇ (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ਸਥਿਰ bool
Copy_Compat_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_ਸੁਰੱਖਿਅਤ(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ਜੇ (!darray_ਖਾਲੀ_ਥਾਵਾਂ(info->interps)) {
        ਬਣਤਰ collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, ਮੇਲ_EXACTLY, &collect);
        CopyInterps(info, true, ਮੇਲ_ALL, &collect);
        CopyInterps(info, true, ਮੇਲ_NONE, &collect);
        CopyInterps(info, true, ਮੇਲ_ANY, &collect);
        CopyInterps(info, true, ਮੇਲ_ANY_OR_NONE, &collect);
        CopyInterps(info, false, ਮੇਲ_EXACTLY, &collect);
        CopyInterps(info, false, ਮੇਲ_ALL, &collect);
        CopyInterps(info, false, ਮੇਲ_NONE, &collect);
        CopyInterps(info, false, ਮੇਲ_ANY, &collect);
        CopyInterps(info, false, ਮੇਲ_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->ਅੰਕ_sym_interprets);
    }

    Copy_Led_Map_Defs_ਮੰਜ਼ਿਲ_Keymap(keymap, info);

    ਵਾਪਸ true;
}

bool
CompileCompatMap(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ਜੇ (file != ਨਿਰਾ)
        HandleCompatMapFile(&info, file);

    ਜੇ (info.error_ਗਿਣਤੀ != 0)
        ਜਾਓ err_info;

    ਜੇ (!Copy_Compat_ਮੰਜ਼ਿਲ_Keymap(&keymap_info->keymap, &info))
        ਜਾਓ err_info;

    ClearCompatInfo(&info);
    ਵਾਪਸ true;

err_info:
    ClearCompatInfo(&info);
    ਵਾਪਸ false;
}
