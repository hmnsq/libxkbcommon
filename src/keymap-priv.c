/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-names.h"
#ضمن "features/enums.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"

ساكن فراغ
update_builtin_keymap_fields(بنية xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ساكن ثابت حرف *ثابت builtin_mods[] = {
        [XKB_MOD_شاخص_لغزش] = XKB_MOD_NAME_لغزش,
        [XKB_MOD_شاخص_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_شاخص_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_شاخص_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_شاخص_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_شاخص_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_شاخص_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_شاخص_MOD5]  = XKB_MOD_NAME_MOD5
    };

    لكل (xkb_mod_شاخص_t i = 0; i < ARRAY_اندازه(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    طول_رشته(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.رقم_mods = ARRAY_اندازه(builtin_mods);
    keymap->canonical_state_ماسک = MOD_REAL_ماسک_ALL;
}

بنية xkb_keymap *
xkb_keymap_new(بنية xkb_context *ctx, ثابت حرف *func,
               تعداد xkb_keymap_format format,
               تعداد xkb_keymap_compile_flags flags)
{
    ثابت تعداد xkb_keymap_compile_flags invalid_flags = (
        flags & ~(تعداد xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_مقادیر
    );
    إذا (invalid_flags) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        أرجع عدم;
    }

    بنية xkb_keymap * ثابت keymap = حافظه_صفر_بده(1, حجم(*keymap));
    إذا (!keymap)
        أرجع عدم;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    أرجع keymap;
}

فراغ
XkbEscapeMapName(حرف *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ساكن ثابت بیعلامت حرف legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    إذا (!name)
        أرجع;

    طالما (*name) {
        بیعلامت حرف c = (بیعلامت حرف)*name;
        إذا (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_شاخص_t
Xkb_Mod_Name_مقصد_شاخص(ثابت بنية xkb_mod_مجموعه *mods, xkb_atom_t name,
                  تعداد mod_type type)
{
    xkb_mod_شاخص_t i;
    ثابت بنية xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        إذا ((mod->type & type) && name == mod->name)
            أرجع i;

    أرجع XKB_MOD_INVALID;
}

bool
Xkb_سطوح_Same_Syms(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b)
{
    إذا (a->رقم_syms != b->رقم_syms)
        أرجع false;
    إذا (a->رقم_syms <= 1)
        أرجع a->s.sym == b->s.sym;
    أرجع memcmp(a->s.syms, b->s.syms, حجم(*a->s.syms) * a->رقم_syms) == 0;
}

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b)
{
    إذا (a->type != b->type)
        أرجع false;

    بدل (a->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
        أرجع true;
    حالة ACTION_TYPE_MOD_مجموعه:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_قفل:
        أرجع (a->mods.flags == b->mods.flags &&
                a->mods.mods.ماسک == b->mods.mods.ماسک &&
                a->mods.mods.mods == b->mods.mods.mods);
    حالة ACTION_TYPE_گروه_مجموعه:
    حالة ACTION_TYPE_گروه_LATCH:
    حالة ACTION_TYPE_گروه_قفل:
        أرجع (a->گروه.flags == b->گروه.flags &&
                a->گروه.گروه == b->گروه.گروه);
    حالة ACTION_TYPE_PTR_MOVE:
        أرجع (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_قفل:
        أرجع (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.شمار == b->btn.شمار);
    حالة ACTION_TYPE_PTR_DEFAULT:
        أرجع (a->dflt.flags == b->dflt.flags &&
                a->dflt.مقدار == b->dflt.مقدار);
    حالة ACTION_TYPE_TERMINATE:
        أرجع true;
    حالة ACTION_TYPE_SWITCH_VT:
        أرجع (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    حالة ACTION_TYPE_CTRL_مجموعه:
    حالة ACTION_TYPE_CTRL_قفل:
        أرجع (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    حالة ACTION_TYPE_REDIRECT_کلید:
        أرجع (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    حالة ACTION_TYPE_UNSUPPORTED_LEGACY:
    حالة ACTION_TYPE_UNKNOWN:
        أرجع true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    حالة ACTION_TYPE_داخلی:
        أرجع (a->داخلی.flags == b->داخلی.flags) &&
               (a->داخلی.clear_latched_mods == b->داخلی.clear_latched_mods);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_داخلی == 20 &&
                      ACTION_TYPE_داخلی + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        أرجع memcmp(a->priv.داده, b->priv.داده, حجم(a->priv.داده)) == 0;
    }
}

bool
Xkb_سطوح_Same_Actions(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b)
{
    إذا (a->رقم_actions != b->رقم_actions)
        أرجع false;
    إذا (a->رقم_actions <= 1)
        أرجع action_equal(&a->a.action, &b->a.action);
    لكل (xkb_action_شمار_t k = 0; k < a->رقم_actions; k++) {
        إذا (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            أرجع false;
    }
    أرجع true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_شاخص_t
Xkb_Wrap_گروه_Into_Range(int32_t گروه,
                      xkb_layout_شاخص_t رقم_groups,
                      تعداد xkb_layout_بیرون_از_range_policy بیرون_از_range_گروه_policy,
                      xkb_layout_شاخص_t بیرون_از_range_گروه_عدد)
{
    إذا (رقم_groups == 0)
        أرجع XKB_LAYOUT_INVALID;

    إذا (گروه >= 0 && (xkb_layout_شاخص_t)گروه < رقم_groups)
        أرجع (xkb_layout_شاخص_t)گروه;

    بدل (بیرون_از_range_گروه_policy) {
    حالة XKB_LAYOUT_بیرون_از_RANGE_REDIRECT:
        إذا (بیرون_از_range_گروه_عدد >= رقم_groups)
            أرجع 0;
        أرجع بیرون_از_range_گروه_عدد;

    حالة XKB_LAYOUT_بیرون_از_RANGE_CLAMP:
        إذا (گروه < 0)
            أرجع 0;
        وإلا
            أرجع رقم_groups - 1;

    حالة XKB_LAYOUT_بیرون_از_RANGE_WRAP:
    افتراضي: {
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
        ثابت int32_t rem = گروه % (int32_t) رقم_groups;
        أرجع (rem >= 0)
            ? (xkb_layout_شاخص_t)rem
            : (xkb_layout_شاخص_t)(rem + (int32_t)رقم_groups);
    }
    }
}

xkb_action_شمار_t
xkb_keymap_کلید_get_actions_by_level(ثابت بنية xkb_keymap *keymap,
                                    ثابت بنية xkb_کلید *کلید,
                                    xkb_layout_شاخص_t layout,
                                    xkb_level_شاخص_t level,
                                    ثابت اتحاد xkb_action **actions)
{
    إذا (!کلید)
        اذهب err;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    إذا (level >= Xkb_کلید_رقم_سطوح(کلید, layout))
        اذهب err;

    ثابت xkb_action_شمار_t شمار =
        کلید->groups[layout].سطوح[level].رقم_actions;
    بدل (شمار) {
        حالة 0:
            اذهب err;
        حالة 1:
            *actions = &کلید->groups[layout].سطوح[level].a.action;
            اكسر;
        افتراضي:
            *actions = کلید->groups[layout].سطوح[level].a.actions;
    }
    أرجع شمار;

err:
    *actions = عدم;
    أرجع 0;
}
