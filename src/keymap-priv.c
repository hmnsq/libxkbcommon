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

ایستا تهی
update_builtin_keymap_fields(ساختار xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ایستا ثابت نویسه *ثابت builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    برای (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
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

ساختار xkb_keymap *
xkb_keymap_new(ساختار xkb_context *ctx, ثابت نویسه *func,
               شمارش xkb_keymap_format format,
               شمارش xkb_keymap_compile_flags flags)
{
    ثابت شمارش xkb_keymap_compile_flags invalid_flags = (
        flags & ~(شمارش xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_مقادیر
    );
    اگر (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        بازگشت پوچ;
    }

    ساختار xkb_keymap * ثابت keymap = calloc(1, اندازه(*keymap));
    اگر (!keymap)
        بازگشت پوچ;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    بازگشت keymap;
}

تهی
XkbEscapeMapName(نویسه *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ایستا ثابت unsigned نویسه legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    اگر (!name)
        بازگشت;

    وقتی (*name) {
        unsigned نویسه c = (unsigned نویسه)*name;
        اگر (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(ثابت ساختار xkb_mod_set *mods, xkb_atom_t name,
                  شمارش mod_type type)
{
    xkb_mod_index_t i;
    ثابت ساختار xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        اگر ((mod->type & type) && name == mod->name)
            بازگشت i;

    بازگشت XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(ثابت ساختار xkb_level *a, ثابت ساختار xkb_level *b)
{
    اگر (a->رقم_syms != b->رقم_syms)
        بازگشت false;
    اگر (a->رقم_syms <= 1)
        بازگشت a->s.sym == b->s.sym;
    بازگشت memcmp(a->s.syms, b->s.syms, اندازه(*a->s.syms) * a->رقم_syms) == 0;
}

bool
action_equal(ثابت اجتماع xkb_action *a, ثابت اجتماع xkb_action *b)
{
    اگر (a->type != b->type)
        بازگشت false;

    انتخاب (a->type) {
    حالت ACTION_TYPE_NONE:
    حالت ACTION_TYPE_VOID:
        بازگشت true;
    حالت ACTION_TYPE_MOD_SET:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_LOCK:
        بازگشت (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    حالت ACTION_TYPE_GROUP_SET:
    حالت ACTION_TYPE_GROUP_LATCH:
    حالت ACTION_TYPE_GROUP_LOCK:
        بازگشت (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    حالت ACTION_TYPE_PTR_MOVE:
        بازگشت (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    حالت ACTION_TYPE_PTR_BUTTON:
    حالت ACTION_TYPE_PTR_LOCK:
        بازگشت (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.شمار == b->btn.شمار);
    حالت ACTION_TYPE_PTR_DEFAULT:
        بازگشت (a->dflt.flags == b->dflt.flags &&
                a->dflt.مقدار == b->dflt.مقدار);
    حالت ACTION_TYPE_TERMINATE:
        بازگشت true;
    حالت ACTION_TYPE_SWITCH_VT:
        بازگشت (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    حالت ACTION_TYPE_CTRL_SET:
    حالت ACTION_TYPE_CTRL_LOCK:
        بازگشت (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    حالت ACTION_TYPE_REDIRECT_کلید:
        بازگشت (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    حالت ACTION_TYPE_UNSUPPORTED_LEGACY:
    حالت ACTION_TYPE_UNKNOWN:
        بازگشت true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    حالت ACTION_TYPE_INTERNAL:
        بازگشت (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    پیشفرض:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        بازگشت memcmp(a->priv.data, b->priv.data, اندازه(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(ثابت ساختار xkb_level *a, ثابت ساختار xkb_level *b)
{
    اگر (a->رقم_actions != b->رقم_actions)
        بازگشت false;
    اگر (a->رقم_actions <= 1)
        بازگشت action_equal(&a->a.action, &b->a.action);
    برای (xkb_action_شمار_t k = 0; k < a->رقم_actions; k++) {
        اگر (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            بازگشت false;
    }
    بازگشت true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t رقم_groups,
                      شمارش xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_عدد)
{
    اگر (رقم_groups == 0)
        بازگشت XKB_LAYOUT_INVALID;

    اگر (group >= 0 && (xkb_layout_index_t)group < رقم_groups)
        بازگشت (xkb_layout_index_t)group;

    انتخاب (out_of_range_group_policy) {
    حالت XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        اگر (out_of_range_group_عدد >= رقم_groups)
            بازگشت 0;
        بازگشت out_of_range_group_عدد;

    حالت XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        اگر (group < 0)
            بازگشت 0;
        وگرنه
            بازگشت رقم_groups - 1;

    حالت XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    پیشفرض: {
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
        بازگشت (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)رقم_groups);
    }
    }
}

xkb_action_شمار_t
xkb_keymap_کلید_get_actions_by_level(ثابت ساختار xkb_keymap *keymap,
                                    ثابت ساختار xkb_کلید *کلید,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ثابت اجتماع xkb_action **actions)
{
    اگر (!کلید)
        برو err;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        برو err;

    اگر (level >= Xkb_کلید_رقم_Levels(کلید, layout))
        برو err;

    ثابت xkb_action_شمار_t شمار =
        کلید->groups[layout].levels[level].رقم_actions;
    انتخاب (شمار) {
        حالت 0:
            برو err;
        حالت 1:
            *actions = &کلید->groups[layout].levels[level].a.action;
            بشکن;
        پیشفرض:
            *actions = کلید->groups[layout].levels[level].a.actions;
    }
    بازگشت شمار;

err:
    *actions = پوچ;
    بازگشت 0;
}
