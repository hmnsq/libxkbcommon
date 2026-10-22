/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-names.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"

ಸ್ಥಿರ ಶೂನ್ಯ
update_builtin_keymap_fields(ರಚನೆ xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_mods[] = {
        [XKB_MOD_ಸೂಚ್ಯಂಕ_ಸರಿಸು] = XKB_MOD_NAME_ಸರಿಸು,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_ಸೂಚ್ಯಂಕ_MOD5]  = XKB_MOD_NAME_MOD5
    };

    ಗಾಗಿ (xkb_mod_ಸೂಚ್ಯಂಕ_t i = 0; i < ARRAY_ಗಾತ್ರ(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.ಅಂಕೆ_mods = ARRAY_ಗಾತ್ರ(builtin_mods);
    keymap->canonical_state_ಮುಖವಾಡ = MOD_REAL_ಮುಖವಾಡ_ALL;
}

ರಚನೆ xkb_keymap *
xkb_keymap_new(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func,
               ಎಣಿಕೆ xkb_keymap_format format,
               ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_compile_flags invalid_flags = (
        flags & ~(ಎಣಿಕೆ xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು
    );
    ಆದರೆ (invalid_flags) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_keymap * ಸ್ಥಿರಾಂಕ keymap = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*keymap));
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    ಹಿಂತಿರುಗಿ keymap;
}

ಶೂನ್ಯ
XkbEscapeMapName(ಅಕ್ಷರ *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    ಆದರೆ (!name)
        ಹಿಂತಿರುಗಿ;

    ಆಗ (*name) {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*name;
        ಆದರೆ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_ಸೂಚ್ಯಂಕ_t
Xkb_Mod_Name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, xkb_atom_t name,
                  ಎಣಿಕೆ mod_type type)
{
    xkb_mod_ಸೂಚ್ಯಂಕ_t i;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        ಆದರೆ ((mod->type & type) && name == mod->name)
            ಹಿಂತಿರುಗಿ i;

    ಹಿಂತಿರುಗಿ XKB_MOD_INVALID;
}

bool
Xkb_ಮಟ್ಟಗಳು_Same_Syms(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *a, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *b)
{
    ಆದರೆ (a->ಅಂಕೆ_syms != b->ಅಂಕೆ_syms)
        ಹಿಂತಿರುಗಿ false;
    ಆದರೆ (a->ಅಂಕೆ_syms <= 1)
        ಹಿಂತಿರುಗಿ a->s.sym == b->s.sym;
    ಹಿಂತಿರುಗಿ memcmp(a->s.syms, b->s.syms, ಗಾತ್ರ(*a->s.syms) * a->ಅಂಕೆ_syms) == 0;
}

bool
action_equal(ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *a, ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *b)
{
    ಆದರೆ (a->type != b->type)
        ಹಿಂತಿರುಗಿ false;

    ಆಯ್ಕೆ (a->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_NONE:
    ಸ್ಥಿತಿ ACTION_TYPE_VOID:
        ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಬೀಗ:
        ಹಿಂತಿರುಗಿ (a->mods.flags == b->mods.flags &&
                a->mods.mods.ಮುಖವಾಡ == b->mods.mods.ಮುಖವಾಡ &&
                a->mods.mods.mods == b->mods.mods.mods);
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಬೀಗ:
        ಹಿಂತಿರುಗಿ (a->ಗುಂಪು.flags == b->ಗುಂಪು.flags &&
                a->ಗುಂಪು.ಗುಂಪು == b->ಗುಂಪು.ಗುಂಪು);
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_MOVE:
        ಹಿಂತಿರುಗಿ (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_BUTTON:
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_ಬೀಗ:
        ಹಿಂತಿರುಗಿ (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.ಎಣಿಕೆ_2 == b->btn.ಎಣಿಕೆ_2);
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_DEFAULT:
        ಹಿಂತಿರುಗಿ (a->dflt.flags == b->dflt.flags &&
                a->dflt.ಮೌಲ್ಯ == b->dflt.ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ ACTION_TYPE_TERMINATE:
        ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ ACTION_TYPE_SWITCH_VT:
        ಹಿಂತಿರುಗಿ (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಬೀಗ:
        ಹಿಂತಿರುಗಿ (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
        ಹಿಂತಿರುಗಿ (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    ಸ್ಥಿತಿ ACTION_TYPE_UNSUPPORTED_LEGACY:
    ಸ್ಥಿತಿ ACTION_TYPE_UNKNOWN:
        ಹಿಂತಿರುಗಿ true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    ಸ್ಥಿತಿ ACTION_TYPE_ಆಂತರಿಕ:
        ಹಿಂತಿರುಗಿ (a->ಆಂತರಿಕ.flags == b->ಆಂತರಿಕ.flags) &&
               (a->ಆಂತರಿಕ.clear_latched_mods == b->ಆಂತರಿಕ.clear_latched_mods);
    ಪೂರ್ವನಿಯೋಜಿತ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
                      ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        ಹಿಂತಿರುಗಿ memcmp(a->priv.ದತ್ತಾಂಶ, b->priv.ದತ್ತಾಂಶ, ಗಾತ್ರ(a->priv.ದತ್ತಾಂಶ)) == 0;
    }
}

bool
Xkb_ಮಟ್ಟಗಳು_Same_Actions(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *a, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *b)
{
    ಆದರೆ (a->ಅಂಕೆ_actions != b->ಅಂಕೆ_actions)
        ಹಿಂತಿರುಗಿ false;
    ಆದರೆ (a->ಅಂಕೆ_actions <= 1)
        ಹಿಂತಿರುಗಿ action_equal(&a->a.action, &b->a.action);
    ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < a->ಅಂಕೆ_actions; k++) {
        ಆದರೆ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            ಹಿಂತಿರುಗಿ false;
    }
    ಹಿಂತಿರುಗಿ true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_ಸೂಚ್ಯಂಕ_t
Xkb_Wrap_ಗುಂಪು_Into_Range(int32_t ಗುಂಪು,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups,
                      ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ)
{
    ಆದರೆ (ಅಂಕೆ_groups == 0)
        ಹಿಂತಿರುಗಿ XKB_LAYOUT_INVALID;

    ಆದರೆ (ಗುಂಪು >= 0 && (xkb_layout_ಸೂಚ್ಯಂಕ_t)ಗುಂಪು < ಅಂಕೆ_groups)
        ಹಿಂತಿರುಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t)ಗುಂಪು;

    ಆಯ್ಕೆ (ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy) {
    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT:
        ಆದರೆ (ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ >= ಅಂಕೆ_groups)
            ಹಿಂತಿರುಗಿ 0;
        ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ;

    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP:
        ಆದರೆ (ಗುಂಪು < 0)
            ಹಿಂತಿರುಗಿ 0;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ ಅಂಕೆ_groups - 1;

    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP:
    ಪೂರ್ವನಿಯೋಜಿತ: {
        /*
         * Ensure conversion xkb_layout_index_t → int32_t is lossless.
         *
         * NOTE: C11 requires a compound statement because it does not allow
         * a declaration after a label (C23 does allow it).
         */
        static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups max don't fit");
        /*
         * % returns the *remainder* of the division, not the modulus (see C11
         * standard 6.5.5 “Multiplicative operators”: a % b = a - (a/b)*b. While
         * both operators return the same result for positive operands, they do
         * not for e.g. a negative dividend: remainder may be negative (in the
         * open interval ]-num_groups, num_groups[) while the modulus is always
         * positive. So if the remainder is negative, we must add `num_groups`
         * to get the modulus.
         */
        ಸ್ಥಿರಾಂಕ int32_t rem = ಗುಂಪು % (int32_t) ಅಂಕೆ_groups;
        ಹಿಂತಿರುಗಿ (rem >= 0)
            ? (xkb_layout_ಸೂಚ್ಯಂಕ_t)rem
            : (xkb_layout_ಸೂಚ್ಯಂಕ_t)(rem + (int32_t)ಅಂಕೆ_groups);
    }
    }
}

xkb_action_ಎಣಿಕೆ_t
xkb_keymap_ಕೀಲಿ_get_actions_by_level(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    xkb_layout_ಸೂಚ್ಯಂಕ_t layout,
                                    xkb_level_ಸೂಚ್ಯಂಕ_t level,
                                    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action **actions)
{
    ಆದರೆ (!ಕೀಲಿ)
        ಹೋಗು err;

    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID)
        ಹೋಗು err;

    ಆದರೆ (level >= Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, layout))
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 =
        ಕೀಲಿ->groups[layout].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions;
    ಆಯ್ಕೆ (ಎಣಿಕೆ_2) {
        ಸ್ಥಿತಿ 0:
            ಹೋಗು err;
        ಸ್ಥಿತಿ 1:
            *actions = &ಕೀಲಿ->groups[layout].ಮಟ್ಟಗಳು[level].a.action;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            *actions = ಕೀಲಿ->groups[layout].ಮಟ್ಟಗಳು[level].a.actions;
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;

err:
    *actions = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ 0;
}
