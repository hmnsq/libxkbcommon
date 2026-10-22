/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-names.h"
#شامل "features/enums.h"
#شامل "keymap.h"
#شامل "messages-codes.h"

ساکن خالی
update_builtin_keymap_fields(ساخت xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ساکن ثابت حرف *ثابت builtin_mods[] = {
        [XKB_MOD_اشاریہ_سرکاؤ] = XKB_MOD_NAME_سرکاؤ,
        [XKB_MOD_اشاریہ_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_اشاریہ_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_اشاریہ_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_اشاریہ_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_اشاریہ_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_اشاریہ_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_اشاریہ_MOD5]  = XKB_MOD_NAME_MOD5
    };

    برائے (xkb_mod_اشاریہ_t i = 0; i < ARRAY_سائز(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    لڑی_لمبائی(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.عدد_mods = ARRAY_سائز(builtin_mods);
    keymap->canonical_state_نقاب = MOD_REAL_نقاب_ALL;
}

ساخت xkb_keymap *
xkb_keymap_new(ساخت xkb_context *ctx, ثابت حرف *func,
               شمار xkb_keymap_format format,
               شمار xkb_keymap_compile_flags flags)
{
    ثابت شمار xkb_keymap_compile_flags invalid_flags = (
        flags & ~(شمار xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_اقدار
    );
    اگر (invalid_flags) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        واپس تہی;
    }

    ساخت xkb_keymap * ثابت keymap = صفر_حافظہ_دو(1, حجم(*keymap));
    اگر (!keymap)
        واپس تہی;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    واپس keymap;
}

خالی
XkbEscapeMapName(حرف *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ساکن ثابت غیرعلامتی حرف legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    اگر (!name)
        واپس;

    جب (*name) {
        غیرعلامتی حرف c = (غیرعلامتی حرف)*name;
        اگر (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_اشاریہ_t
Xkb_Mod_Name_مقصد_اشاریہ(ثابت ساخت xkb_mod_سیٹ *mods, xkb_atom_t name,
                  شمار mod_type type)
{
    xkb_mod_اشاریہ_t i;
    ثابت ساخت xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        اگر ((mod->type & type) && name == mod->name)
            واپس i;

    واپس XKB_MOD_INVALID;
}

bool
Xkb_سطحیں_Same_Syms(ثابت ساخت xkb_level *a, ثابت ساخت xkb_level *b)
{
    اگر (a->عدد_syms != b->عدد_syms)
        واپس false;
    اگر (a->عدد_syms <= 1)
        واپس a->s.sym == b->s.sym;
    واپس memcmp(a->s.syms, b->s.syms, حجم(*a->s.syms) * a->عدد_syms) == 0;
}

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b)
{
    اگر (a->type != b->type)
        واپس false;

    انتخاب (a->type) {
    حالت ACTION_TYPE_NONE:
    حالت ACTION_TYPE_VOID:
        واپس true;
    حالت ACTION_TYPE_MOD_سیٹ:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_تالا:
        واپس (a->mods.flags == b->mods.flags &&
                a->mods.mods.نقاب == b->mods.mods.نقاب &&
                a->mods.mods.mods == b->mods.mods.mods);
    حالت ACTION_TYPE_گروہ_سیٹ:
    حالت ACTION_TYPE_گروہ_LATCH:
    حالت ACTION_TYPE_گروہ_تالا:
        واپس (a->گروہ.flags == b->گروہ.flags &&
                a->گروہ.گروہ == b->گروہ.گروہ);
    حالت ACTION_TYPE_PTR_MOVE:
        واپس (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    حالت ACTION_TYPE_PTR_BUTTON:
    حالت ACTION_TYPE_PTR_تالا:
        واپس (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.گنتی == b->btn.گنتی);
    حالت ACTION_TYPE_PTR_DEFAULT:
        واپس (a->dflt.flags == b->dflt.flags &&
                a->dflt.قدر == b->dflt.قدر);
    حالت ACTION_TYPE_TERMINATE:
        واپس true;
    حالت ACTION_TYPE_SWITCH_VT:
        واپس (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    حالت ACTION_TYPE_CTRL_سیٹ:
    حالت ACTION_TYPE_CTRL_تالا:
        واپس (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    حالت ACTION_TYPE_REDIRECT_کلید:
        واپس (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    حالت ACTION_TYPE_UNSUPPORTED_LEGACY:
    حالت ACTION_TYPE_UNKNOWN:
        واپس true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    حالت ACTION_TYPE_اندرونی:
        واپس (a->اندرونی_2.flags == b->اندرونی_2.flags) &&
               (a->اندرونی_2.clear_latched_mods == b->اندرونی_2.clear_latched_mods);
    طےشدہ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        واپس memcmp(a->priv.ڈیٹا, b->priv.ڈیٹا, حجم(a->priv.ڈیٹا)) == 0;
    }
}

bool
Xkb_سطحیں_Same_Actions(ثابت ساخت xkb_level *a, ثابت ساخت xkb_level *b)
{
    اگر (a->عدد_actions != b->عدد_actions)
        واپس false;
    اگر (a->عدد_actions <= 1)
        واپس action_equal(&a->a.action, &b->a.action);
    برائے (xkb_action_گنتی_t k = 0; k < a->عدد_actions; k++) {
        اگر (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            واپس false;
    }
    واپس true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_اشاریہ_t
Xkb_Wrap_گروہ_Into_Range(int32_t گروہ,
                      xkb_layout_اشاریہ_t عدد_groups,
                      شمار xkb_layout_برآمد_کا_range_policy برآمد_کا_range_گروہ_policy,
                      xkb_layout_اشاریہ_t برآمد_کا_range_گروہ_نمبر)
{
    اگر (عدد_groups == 0)
        واپس XKB_LAYOUT_INVALID;

    اگر (گروہ >= 0 && (xkb_layout_اشاریہ_t)گروہ < عدد_groups)
        واپس (xkb_layout_اشاریہ_t)گروہ;

    انتخاب (برآمد_کا_range_گروہ_policy) {
    حالت XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT:
        اگر (برآمد_کا_range_گروہ_نمبر >= عدد_groups)
            واپس 0;
        واپس برآمد_کا_range_گروہ_نمبر;

    حالت XKB_LAYOUT_برآمد_کا_RANGE_CLAMP:
        اگر (گروہ < 0)
            واپس 0;
        ورنہ
            واپس عدد_groups - 1;

    حالت XKB_LAYOUT_برآمد_کا_RANGE_WRAP:
    طےشدہ: {
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
        ثابت int32_t rem = گروہ % (int32_t) عدد_groups;
        واپس (rem >= 0)
            ? (xkb_layout_اشاریہ_t)rem
            : (xkb_layout_اشاریہ_t)(rem + (int32_t)عدد_groups);
    }
    }
}

xkb_action_گنتی_t
xkb_keymap_کلید_get_actions_by_level(ثابت ساخت xkb_keymap *keymap,
                                    ثابت ساخت xkb_کلید *کلید,
                                    xkb_layout_اشاریہ_t layout,
                                    xkb_level_اشاریہ_t level,
                                    ثابت اتحاد xkb_action **actions)
{
    اگر (!کلید)
        جاؤ err;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        جاؤ err;

    اگر (level >= Xkb_کلید_عدد_سطحیں(کلید, layout))
        جاؤ err;

    ثابت xkb_action_گنتی_t گنتی =
        کلید->groups[layout].سطحیں[level].عدد_actions;
    انتخاب (گنتی) {
        حالت 0:
            جاؤ err;
        حالت 1:
            *actions = &کلید->groups[layout].سطحیں[level].a.action;
            توڑو;
        طےشدہ:
            *actions = کلید->groups[layout].سطحیں[level].a.actions;
    }
    واپس گنتی;

err:
    *actions = تہی;
    واپس 0;
}
