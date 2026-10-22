/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-names.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "messages-codes.h"

സ്ഥിതം ശൂന്യം
update_builtin_keymap_fields(ഘടന xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    വേണ്ടി (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.അക്കം_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

ഘടന xkb_keymap *
xkb_keymap_new(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *func,
               എണ്ണൽ xkb_keymap_format format,
               എണ്ണൽ xkb_keymap_compile_flags flags)
{
    സ്ഥിരം എണ്ണൽ xkb_keymap_compile_flags invalid_flags = (
        flags & ~(എണ്ണൽ xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_മൂല്യങ്ങൾ
    );
    എങ്കിൽ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_keymap * സ്ഥിരം keymap = calloc(1, വലുപ്പം(*keymap));
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    തിരികെ keymap;
}

ശൂന്യം
XkbEscapeMapName(അക്ഷരം *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    എങ്കിൽ (!name)
        തിരികെ;

    ആകുമ്പോൾ (*name) {
        unsigned അക്ഷരം c = (unsigned അക്ഷരം)*name;
        എങ്കിൽ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(സ്ഥിരം ഘടന xkb_mod_set *mods, xkb_atom_t name,
                  എണ്ണൽ mod_type type)
{
    xkb_mod_index_t i;
    സ്ഥിരം ഘടന xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        എങ്കിൽ ((mod->type & type) && name == mod->name)
            തിരികെ i;

    തിരികെ XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(സ്ഥിരം ഘടന xkb_level *a, സ്ഥിരം ഘടന xkb_level *b)
{
    എങ്കിൽ (a->അക്കം_syms != b->അക്കം_syms)
        തിരികെ false;
    എങ്കിൽ (a->അക്കം_syms <= 1)
        തിരികെ a->s.sym == b->s.sym;
    തിരികെ memcmp(a->s.syms, b->s.syms, വലുപ്പം(*a->s.syms) * a->അക്കം_syms) == 0;
}

bool
action_equal(സ്ഥിരം സംഘം xkb_action *a, സ്ഥിരം സംഘം xkb_action *b)
{
    എങ്കിൽ (a->type != b->type)
        തിരികെ false;

    തിരഞ്ഞെടുപ്പ് (a->type) {
    അവസ്ഥ ACTION_TYPE_NONE:
    അവസ്ഥ ACTION_TYPE_VOID:
        തിരികെ true;
    അവസ്ഥ ACTION_TYPE_MOD_SET:
    അവസ്ഥ ACTION_TYPE_MOD_LATCH:
    അവസ്ഥ ACTION_TYPE_MOD_LOCK:
        തിരികെ (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    അവസ്ഥ ACTION_TYPE_GROUP_SET:
    അവസ്ഥ ACTION_TYPE_GROUP_LATCH:
    അവസ്ഥ ACTION_TYPE_GROUP_LOCK:
        തിരികെ (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    അവസ്ഥ ACTION_TYPE_PTR_MOVE:
        തിരികെ (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    അവസ്ഥ ACTION_TYPE_PTR_BUTTON:
    അവസ്ഥ ACTION_TYPE_PTR_LOCK:
        തിരികെ (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.എണ്ണം == b->btn.എണ്ണം);
    അവസ്ഥ ACTION_TYPE_PTR_DEFAULT:
        തിരികെ (a->dflt.flags == b->dflt.flags &&
                a->dflt.മൂല്യം == b->dflt.മൂല്യം);
    അവസ്ഥ ACTION_TYPE_TERMINATE:
        തിരികെ true;
    അവസ്ഥ ACTION_TYPE_SWITCH_VT:
        തിരികെ (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    അവസ്ഥ ACTION_TYPE_CTRL_SET:
    അവസ്ഥ ACTION_TYPE_CTRL_LOCK:
        തിരികെ (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ:
        തിരികെ (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    അവസ്ഥ ACTION_TYPE_UNSUPPORTED_LEGACY:
    അവസ്ഥ ACTION_TYPE_UNKNOWN:
        തിരികെ true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    അവസ്ഥ ACTION_TYPE_INTERNAL:
        തിരികെ (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    സ്ഥിരസ്ഥിതി:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        തിരികെ memcmp(a->priv.data, b->priv.data, വലുപ്പം(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(സ്ഥിരം ഘടന xkb_level *a, സ്ഥിരം ഘടന xkb_level *b)
{
    എങ്കിൽ (a->അക്കം_actions != b->അക്കം_actions)
        തിരികെ false;
    എങ്കിൽ (a->അക്കം_actions <= 1)
        തിരികെ action_equal(&a->a.action, &b->a.action);
    വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < a->അക്കം_actions; k++) {
        എങ്കിൽ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            തിരികെ false;
    }
    തിരികെ true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t അക്കം_groups,
                      എണ്ണൽ xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_സംഖ്യ)
{
    എങ്കിൽ (അക്കം_groups == 0)
        തിരികെ XKB_LAYOUT_INVALID;

    എങ്കിൽ (group >= 0 && (xkb_layout_index_t)group < അക്കം_groups)
        തിരികെ (xkb_layout_index_t)group;

    തിരഞ്ഞെടുപ്പ് (out_of_range_group_policy) {
    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        എങ്കിൽ (out_of_range_group_സംഖ്യ >= അക്കം_groups)
            തിരികെ 0;
        തിരികെ out_of_range_group_സംഖ്യ;

    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        എങ്കിൽ (group < 0)
            തിരികെ 0;
        അല്ലെങ്കിൽ
            തിരികെ അക്കം_groups - 1;

    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    സ്ഥിരസ്ഥിതി: {
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
        സ്ഥിരം int32_t rem = group % (int32_t) അക്കം_groups;
        തിരികെ (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)അക്കം_groups);
    }
    }
}

xkb_action_എണ്ണം_t
xkb_keymap_താക്കോൽ_get_actions_by_level(സ്ഥിരം ഘടന xkb_keymap *keymap,
                                    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    സ്ഥിരം സംഘം xkb_action **actions)
{
    എങ്കിൽ (!താക്കോൽ)
        പോകുക err;

    layout = xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, layout);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID)
        പോകുക err;

    എങ്കിൽ (level >= Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, layout))
        പോകുക err;

    സ്ഥിരം xkb_action_എണ്ണം_t എണ്ണം =
        താക്കോൽ->groups[layout].levels[level].അക്കം_actions;
    തിരഞ്ഞെടുപ്പ് (എണ്ണം) {
        അവസ്ഥ 0:
            പോകുക err;
        അവസ്ഥ 1:
            *actions = &താക്കോൽ->groups[layout].levels[level].a.action;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            *actions = താക്കോൽ->groups[layout].levels[level].a.actions;
    }
    തിരികെ എണ്ണം;

err:
    *actions = ശൂന്യ;
    തിരികെ 0;
}
