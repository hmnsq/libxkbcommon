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
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    لكل (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.رقم_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

بنية xkb_keymap *
xkb_keymap_new(بنية xkb_context *ctx, ثابت حرف *func,
               تعداد xkb_keymap_format format,
               تعداد xkb_keymap_compile_flags flags)
{
    ثابت تعداد xkb_keymap_compile_flags invalid_flags = (
        flags & ~(تعداد xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_قيم
    );
    إذا (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        إرجاع عدم;
    }

    بنية xkb_keymap * ثابت keymap = calloc(1, حجم(*keymap));
    إذا (!keymap)
        إرجاع عدم;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    إرجاع keymap;
}

فراغ
XkbEscapeMapName(حرف *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ساكن ثابت unsigned حرف legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    إذا (!name)
        إرجاع;

    بينما (*name) {
        unsigned حرف c = (unsigned حرف)*name;
        إذا (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(ثابت بنية xkb_mod_set *mods, xkb_atom_t name,
                  تعداد mod_type type)
{
    xkb_mod_index_t i;
    ثابت بنية xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        إذا ((mod->type & type) && name == mod->name)
            إرجاع i;

    إرجاع XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b)
{
    إذا (a->رقم_syms != b->رقم_syms)
        إرجاع false;
    إذا (a->رقم_syms <= 1)
        إرجاع a->s.sym == b->s.sym;
    إرجاع memcmp(a->s.syms, b->s.syms, حجم(*a->s.syms) * a->رقم_syms) == 0;
}

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b)
{
    إذا (a->type != b->type)
        إرجاع false;

    بدل (a->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
        إرجاع true;
    حالة ACTION_TYPE_MOD_SET:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_LOCK:
        إرجاع (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    حالة ACTION_TYPE_GROUP_SET:
    حالة ACTION_TYPE_GROUP_LATCH:
    حالة ACTION_TYPE_GROUP_LOCK:
        إرجاع (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    حالة ACTION_TYPE_PTR_MOVE:
        إرجاع (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_LOCK:
        إرجاع (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.تعداد_2 == b->btn.تعداد_2);
    حالة ACTION_TYPE_PTR_DEFAULT:
        إرجاع (a->dflt.flags == b->dflt.flags &&
                a->dflt.قيمة == b->dflt.قيمة);
    حالة ACTION_TYPE_TERMINATE:
        إرجاع true;
    حالة ACTION_TYPE_SWITCH_VT:
        إرجاع (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    حالة ACTION_TYPE_CTRL_SET:
    حالة ACTION_TYPE_CTRL_LOCK:
        إرجاع (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    حالة ACTION_TYPE_REDIRECT_مفتاح:
        إرجاع (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    حالة ACTION_TYPE_UNSUPPORTED_LEGACY:
    حالة ACTION_TYPE_UNKNOWN:
        إرجاع true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    حالة ACTION_TYPE_INTERNAL:
        إرجاع (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        إرجاع memcmp(a->priv.data, b->priv.data, حجم(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b)
{
    إذا (a->رقم_actions != b->رقم_actions)
        إرجاع false;
    إذا (a->رقم_actions <= 1)
        إرجاع action_equal(&a->a.action, &b->a.action);
    لكل (xkb_action_تعداد_t k = 0; k < a->رقم_actions; k++) {
        إذا (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            إرجاع false;
    }
    إرجاع true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t رقم_groups,
                      تعداد xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_العدد)
{
    إذا (رقم_groups == 0)
        إرجاع XKB_LAYOUT_INVALID;

    إذا (group >= 0 && (xkb_layout_index_t)group < رقم_groups)
        إرجاع (xkb_layout_index_t)group;

    بدل (out_of_range_group_policy) {
    حالة XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        إذا (out_of_range_group_العدد >= رقم_groups)
            إرجاع 0;
        إرجاع out_of_range_group_العدد;

    حالة XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        إذا (group < 0)
            إرجاع 0;
        وإلا
            إرجاع رقم_groups - 1;

    حالة XKB_LAYOUT_OUT_OF_RANGE_WRAP:
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
        ثابت int32_t rem = group % (int32_t) رقم_groups;
        إرجاع (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)رقم_groups);
    }
    }
}

xkb_action_تعداد_t
xkb_keymap_مفتاح_get_actions_by_level(ثابت بنية xkb_keymap *keymap,
                                    ثابت بنية xkb_مفتاح *مفتاح,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ثابت اتحاد xkb_action **actions)
{
    إذا (!مفتاح)
        اذهب err;

    layout = xkb_keymap_مفتاح_effective_layout(مفتاح, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    إذا (level >= Xkb_مفتاح_رقم_Levels(مفتاح, layout))
        اذهب err;

    ثابت xkb_action_تعداد_t تعداد_2 =
        مفتاح->groups[layout].levels[level].رقم_actions;
    بدل (تعداد_2) {
        حالة 0:
            اذهب err;
        حالة 1:
            *actions = &مفتاح->groups[layout].levels[level].a.action;
            اكسر;
        افتراضي:
            *actions = مفتاح->groups[layout].levels[level].a.actions;
    }
    إرجاع تعداد_2;

err:
    *actions = عدم;
    إرجاع 0;
}
