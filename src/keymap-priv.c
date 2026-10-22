/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-names.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "messages-codes.h"

স্থির শূন্য
update_builtin_keymap_fields(কাঠামো xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_mods[] = {
        [XKB_MOD_সূচক_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_সূচক_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_সূচক_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_সূচক_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_সূচক_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_সূচক_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_সূচক_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_সূচক_MOD5]  = XKB_MOD_NAME_MOD5
    };

    জন্য (xkb_mod_সূচক_t i = 0; i < ARRAY_আকার(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.অঙ্ক_mods = ARRAY_আকার(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

কাঠামো xkb_keymap *
xkb_keymap_new(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *func,
               গণনা xkb_keymap_format format,
               গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুবক গণনা xkb_keymap_compile_flags invalid_flags = (
        flags & ~(গণনা xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_মানসমূহ
    );
    যদি (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_keymap * ধ্রুবক keymap = calloc(1, আকার(*keymap));
    যদি (!keymap)
        ফেরত ফাঁকা;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    ফেরত keymap;
}

শূন্য
XkbEscapeMapName(অক্ষর *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    স্থির ধ্রুবক unsigned অক্ষর legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    যদি (!name)
        ফেরত;

    যখন (*name) {
        unsigned অক্ষর c = (unsigned অক্ষর)*name;
        যদি (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_সূচক_t
Xkb_Mod_Name_To_সূচক(ধ্রুবক কাঠামো xkb_mod_set *mods, xkb_atom_t name,
                  গণনা mod_type type)
{
    xkb_mod_সূচক_t i;
    ধ্রুবক কাঠামো xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        যদি ((mod->type & type) && name == mod->name)
            ফেরত i;

    ফেরত XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(ধ্রুবক কাঠামো xkb_level *a, ধ্রুবক কাঠামো xkb_level *b)
{
    যদি (a->অঙ্ক_syms != b->অঙ্ক_syms)
        ফেরত false;
    যদি (a->অঙ্ক_syms <= 1)
        ফেরত a->s.sym == b->s.sym;
    ফেরত memcmp(a->s.syms, b->s.syms, আকার(*a->s.syms) * a->অঙ্ক_syms) == 0;
}

bool
action_equal(ধ্রুবক সংঘ xkb_action *a, ধ্রুবক সংঘ xkb_action *b)
{
    যদি (a->type != b->type)
        ফেরত false;

    নির্বাচন (a->type) {
    ক্ষেত্র ACTION_TYPE_NONE:
    ক্ষেত্র ACTION_TYPE_VOID:
        ফেরত true;
    ক্ষেত্র ACTION_TYPE_MOD_SET:
    ক্ষেত্র ACTION_TYPE_MOD_LATCH:
    ক্ষেত্র ACTION_TYPE_MOD_LOCK:
        ফেরত (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    ক্ষেত্র ACTION_TYPE_GROUP_SET:
    ক্ষেত্র ACTION_TYPE_GROUP_LATCH:
    ক্ষেত্র ACTION_TYPE_GROUP_LOCK:
        ফেরত (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    ক্ষেত্র ACTION_TYPE_PTR_MOVE:
        ফেরত (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    ক্ষেত্র ACTION_TYPE_PTR_BUTTON:
    ক্ষেত্র ACTION_TYPE_PTR_LOCK:
        ফেরত (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.গুনতি == b->btn.গুনতি);
    ক্ষেত্র ACTION_TYPE_PTR_DEFAULT:
        ফেরত (a->dflt.flags == b->dflt.flags &&
                a->dflt.মান == b->dflt.মান);
    ক্ষেত্র ACTION_TYPE_TERMINATE:
        ফেরত true;
    ক্ষেত্র ACTION_TYPE_SWITCH_VT:
        ফেরত (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    ক্ষেত্র ACTION_TYPE_CTRL_SET:
    ক্ষেত্র ACTION_TYPE_CTRL_LOCK:
        ফেরত (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    ক্ষেত্র ACTION_TYPE_REDIRECT_চাবি:
        ফেরত (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    ক্ষেত্র ACTION_TYPE_UNSUPPORTED_LEGACY:
    ক্ষেত্র ACTION_TYPE_UNKNOWN:
        ফেরত true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    ক্ষেত্র ACTION_TYPE_INTERNAL:
        ফেরত (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    পূর্বনির্ধারিত:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        ফেরত memcmp(a->priv.তথ্য, b->priv.তথ্য, আকার(a->priv.তথ্য)) == 0;
    }
}

bool
XkbLevelsSameActions(ধ্রুবক কাঠামো xkb_level *a, ধ্রুবক কাঠামো xkb_level *b)
{
    যদি (a->অঙ্ক_actions != b->অঙ্ক_actions)
        ফেরত false;
    যদি (a->অঙ্ক_actions <= 1)
        ফেরত action_equal(&a->a.action, &b->a.action);
    জন্য (xkb_action_গুনতি_t k = 0; k < a->অঙ্ক_actions; k++) {
        যদি (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            ফেরত false;
    }
    ফেরত true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_সূচক_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_সূচক_t অঙ্ক_groups,
                      গণনা xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_সূচক_t out_of_range_group_সংখ্যা)
{
    যদি (অঙ্ক_groups == 0)
        ফেরত XKB_LAYOUT_INVALID;

    যদি (group >= 0 && (xkb_layout_সূচক_t)group < অঙ্ক_groups)
        ফেরত (xkb_layout_সূচক_t)group;

    নির্বাচন (out_of_range_group_policy) {
    ক্ষেত্র XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        যদি (out_of_range_group_সংখ্যা >= অঙ্ক_groups)
            ফেরত 0;
        ফেরত out_of_range_group_সংখ্যা;

    ক্ষেত্র XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        যদি (group < 0)
            ফেরত 0;
        অন্যথা
            ফেরত অঙ্ক_groups - 1;

    ক্ষেত্র XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    পূর্বনির্ধারিত: {
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
        ধ্রুবক int32_t rem = group % (int32_t) অঙ্ক_groups;
        ফেরত (rem >= 0)
            ? (xkb_layout_সূচক_t)rem
            : (xkb_layout_সূচক_t)(rem + (int32_t)অঙ্ক_groups);
    }
    }
}

xkb_action_গুনতি_t
xkb_keymap_চাবি_get_actions_by_level(ধ্রুবক কাঠামো xkb_keymap *keymap,
                                    ধ্রুবক কাঠামো xkb_চাবি *চাবি,
                                    xkb_layout_সূচক_t layout,
                                    xkb_level_সূচক_t level,
                                    ধ্রুবক সংঘ xkb_action **actions)
{
    যদি (!চাবি)
        যাও err;

    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        যাও err;

    যদি (level >= Xkb_চাবি_অঙ্ক_Levels(চাবি, layout))
        যাও err;

    ধ্রুবক xkb_action_গুনতি_t গুনতি =
        চাবি->groups[layout].levels[level].অঙ্ক_actions;
    নির্বাচন (গুনতি) {
        ক্ষেত্র 0:
            যাও err;
        ক্ষেত্র 1:
            *actions = &চাবি->groups[layout].levels[level].a.action;
            বিরতি;
        পূর্বনির্ধারিত:
            *actions = চাবি->groups[layout].levels[level].a.actions;
    }
    ফেরত গুনতি;

err:
    *actions = ফাঁকা;
    ফেরত 0;
}
