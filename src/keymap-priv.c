/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-names.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"

ਸਥਿਰ ਖਾਲੀ
update_builtin_keymap_fields(ਬਣਤਰ xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_mods[] = {
        [XKB_MOD_ਸੂਚਕਾਂਕ_ਖਿਸਕਾਓ] = XKB_MOD_NAME_ਖਿਸਕਾਓ,
        [XKB_MOD_ਸੂਚਕਾਂਕ_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_ਸੂਚਕਾਂਕ_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_ਸੂਚਕਾਂਕ_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_ਸੂਚਕਾਂਕ_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_ਸੂਚਕਾਂਕ_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_ਸੂਚਕਾਂਕ_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_ਸੂਚਕਾਂਕ_MOD5]  = XKB_MOD_NAME_MOD5
    };

    ਲਈ (xkb_mod_ਸੂਚਕਾਂਕ_t i = 0; i < ARRAY_ਮਾਪ(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    ਲੜੀ_ਲੰਬਾਈ(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.ਅੰਕ_mods = ARRAY_ਮਾਪ(builtin_mods);
    keymap->canonical_state_ਨਕਾਬ = MOD_REAL_ਨਕਾਬ_ALL;
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *func,
               ਗਿਣਤੀ xkb_keymap_format format,
               ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਅਚਲ ਗਿਣਤੀ xkb_keymap_compile_flags invalid_flags = (
        flags & ~(ਗਿਣਤੀ xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_ਮੁੱਲਾਂ
    );
    ਜੇ (invalid_flags) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_keymap * ਅਚਲ keymap = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*keymap));
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    ਵਾਪਸ keymap;
}

ਖਾਲੀ
XkbEscapeMapName(ਅੱਖਰ *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    ਜੇ (!name)
        ਵਾਪਸ;

    ਜਦੋਂ (*name) {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*name;
        ਜੇ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_ਸੂਚਕਾਂਕ_t
Xkb_Mod_Name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, xkb_atom_t name,
                  ਗਿਣਤੀ mod_type type)
{
    xkb_mod_ਸੂਚਕਾਂਕ_t i;
    ਅਚਲ ਬਣਤਰ xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        ਜੇ ((mod->type & type) && name == mod->name)
            ਵਾਪਸ i;

    ਵਾਪਸ XKB_MOD_INVALID;
}

bool
Xkb_ਪੱਧਰ_Same_Syms(ਅਚਲ ਬਣਤਰ xkb_level *a, ਅਚਲ ਬਣਤਰ xkb_level *b)
{
    ਜੇ (a->ਅੰਕ_syms != b->ਅੰਕ_syms)
        ਵਾਪਸ false;
    ਜੇ (a->ਅੰਕ_syms <= 1)
        ਵਾਪਸ a->s.sym == b->s.sym;
    ਵਾਪਸ memcmp(a->s.syms, b->s.syms, ਆਕਾਰ(*a->s.syms) * a->ਅੰਕ_syms) == 0;
}

bool
action_equal(ਅਚਲ ਸੰਘ xkb_action *a, ਅਚਲ ਸੰਘ xkb_action *b)
{
    ਜੇ (a->type != b->type)
        ਵਾਪਸ false;

    ਚੋਣ (a->type) {
    ਹਾਲਤ ACTION_TYPE_NONE:
    ਹਾਲਤ ACTION_TYPE_VOID:
        ਵਾਪਸ true;
    ਹਾਲਤ ACTION_TYPE_MOD_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_MOD_LATCH:
    ਹਾਲਤ ACTION_TYPE_MOD_ਤਾਲਾ:
        ਵਾਪਸ (a->mods.flags == b->mods.flags &&
                a->mods.mods.ਨਕਾਬ == b->mods.mods.ਨਕਾਬ &&
                a->mods.mods.mods == b->mods.mods.mods);
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_LATCH:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ:
        ਵਾਪਸ (a->ਸਮੂਹ.flags == b->ਸਮੂਹ.flags &&
                a->ਸਮੂਹ.ਸਮੂਹ == b->ਸਮੂਹ.ਸਮੂਹ);
    ਹਾਲਤ ACTION_TYPE_PTR_MOVE:
        ਵਾਪਸ (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    ਹਾਲਤ ACTION_TYPE_PTR_BUTTON:
    ਹਾਲਤ ACTION_TYPE_PTR_ਤਾਲਾ:
        ਵਾਪਸ (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.ਗਿਣਤੀ_2 == b->btn.ਗਿਣਤੀ_2);
    ਹਾਲਤ ACTION_TYPE_PTR_DEFAULT:
        ਵਾਪਸ (a->dflt.flags == b->dflt.flags &&
                a->dflt.ਮੁੱਲ == b->dflt.ਮੁੱਲ);
    ਹਾਲਤ ACTION_TYPE_TERMINATE:
        ਵਾਪਸ true;
    ਹਾਲਤ ACTION_TYPE_SWITCH_VT:
        ਵਾਪਸ (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    ਹਾਲਤ ACTION_TYPE_CTRL_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_CTRL_ਤਾਲਾ:
        ਵਾਪਸ (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ:
        ਵਾਪਸ (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    ਹਾਲਤ ACTION_TYPE_UNSUPPORTED_LEGACY:
    ਹਾਲਤ ACTION_TYPE_UNKNOWN:
        ਵਾਪਸ true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    ਹਾਲਤ ACTION_TYPE_ਅੰਦਰੂਨੀ:
        ਵਾਪਸ (a->ਅੰਦਰੂਨੀ.flags == b->ਅੰਦਰੂਨੀ.flags) &&
               (a->ਅੰਦਰੂਨੀ.clear_latched_mods == b->ਅੰਦਰੂਨੀ.clear_latched_mods);
    ਮੂਲ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
                      ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        ਵਾਪਸ memcmp(a->priv.ਅੰਕੜੇ, b->priv.ਅੰਕੜੇ, ਆਕਾਰ(a->priv.ਅੰਕੜੇ)) == 0;
    }
}

bool
Xkb_ਪੱਧਰ_Same_Actions(ਅਚਲ ਬਣਤਰ xkb_level *a, ਅਚਲ ਬਣਤਰ xkb_level *b)
{
    ਜੇ (a->ਅੰਕ_actions != b->ਅੰਕ_actions)
        ਵਾਪਸ false;
    ਜੇ (a->ਅੰਕ_actions <= 1)
        ਵਾਪਸ action_equal(&a->a.action, &b->a.action);
    ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < a->ਅੰਕ_actions; k++) {
        ਜੇ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            ਵਾਪਸ false;
    }
    ਵਾਪਸ true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_ਸੂਚਕਾਂਕ_t
Xkb_Wrap_ਸਮੂਹ_Into_Range(int32_t ਸਮੂਹ,
                      xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups,
                      ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy,
                      xkb_layout_ਸੂਚਕਾਂਕ_t ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ)
{
    ਜੇ (ਅੰਕ_groups == 0)
        ਵਾਪਸ XKB_LAYOUT_INVALID;

    ਜੇ (ਸਮੂਹ >= 0 && (xkb_layout_ਸੂਚਕਾਂਕ_t)ਸਮੂਹ < ਅੰਕ_groups)
        ਵਾਪਸ (xkb_layout_ਸੂਚਕਾਂਕ_t)ਸਮੂਹ;

    ਚੋਣ (ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy) {
    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT:
        ਜੇ (ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ >= ਅੰਕ_groups)
            ਵਾਪਸ 0;
        ਵਾਪਸ ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ;

    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP:
        ਜੇ (ਸਮੂਹ < 0)
            ਵਾਪਸ 0;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ ਅੰਕ_groups - 1;

    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP:
    ਮੂਲ: {
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
        ਅਚਲ int32_t rem = ਸਮੂਹ % (int32_t) ਅੰਕ_groups;
        ਵਾਪਸ (rem >= 0)
            ? (xkb_layout_ਸੂਚਕਾਂਕ_t)rem
            : (xkb_layout_ਸੂਚਕਾਂਕ_t)(rem + (int32_t)ਅੰਕ_groups);
    }
    }
}

xkb_action_ਗਿਣਤੀ_t
xkb_keymap_ਕੁੰਜੀ_get_actions_by_level(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                    xkb_layout_ਸੂਚਕਾਂਕ_t layout,
                                    xkb_level_ਸੂਚਕਾਂਕ_t level,
                                    ਅਚਲ ਸੰਘ xkb_action **actions)
{
    ਜੇ (!ਕੁੰਜੀ)
        ਜਾਓ err;

    layout = xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, layout);
    ਜੇ (layout == XKB_LAYOUT_INVALID)
        ਜਾਓ err;

    ਜੇ (level >= Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, layout))
        ਜਾਓ err;

    ਅਚਲ xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 =
        ਕੁੰਜੀ->groups[layout].ਪੱਧਰ[level].ਅੰਕ_actions;
    ਚੋਣ (ਗਿਣਤੀ_2) {
        ਹਾਲਤ 0:
            ਜਾਓ err;
        ਹਾਲਤ 1:
            *actions = &ਕੁੰਜੀ->groups[layout].ਪੱਧਰ[level].a.action;
            ਰੋਕੋ;
        ਮੂਲ:
            *actions = ਕੁੰਜੀ->groups[layout].ਪੱਧਰ[level].a.actions;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;

err:
    *actions = ਨਿਰਾ;
    ਵਾਪਸ 0;
}
