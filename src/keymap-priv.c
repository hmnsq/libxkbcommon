/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-names.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"

स्थिर शून्य
update_builtin_keymap_fields(संरचना xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    स्थिर ध्रुव वर्ण *ध्रुव builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    प्रति (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.अङ्कः_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

संरचना xkb_keymap *
xkb_keymap_new(संरचना xkb_context *ctx, ध्रुव वर्ण *func,
               गणना xkb_keymap_format format,
               गणना xkb_keymap_compile_flags flags)
{
    ध्रुव गणना xkb_keymap_compile_flags invalid_flags = (
        flags & ~(गणना xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_मानानि
    );
    जइ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_keymap * ध्रुव keymap = calloc(1, परिमाण(*keymap));
    जइ (!keymap)
        प्रत्यागम रिक्त;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    प्रत्यागम keymap;
}

शून्य
XkbEscapeMapName(वर्ण *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    स्थिर ध्रुव unsigned वर्ण legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    जइ (!name)
        प्रत्यागम;

    जाव (*name) {
        unsigned वर्ण c = (unsigned वर्ण)*name;
        जइ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(ध्रुव संरचना xkb_mod_set *mods, xkb_atom_t name,
                  गणना mod_type type)
{
    xkb_mod_index_t i;
    ध्रुव संरचना xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        जइ ((mod->type & type) && name == mod->name)
            प्रत्यागम i;

    प्रत्यागम XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(ध्रुव संरचना xkb_level *a, ध्रुव संरचना xkb_level *b)
{
    जइ (a->अङ्कः_syms != b->अङ्कः_syms)
        प्रत्यागम false;
    जइ (a->अङ्कः_syms <= 1)
        प्रत्यागम a->s.sym == b->s.sym;
    प्रत्यागम memcmp(a->s.syms, b->s.syms, परिमाण(*a->s.syms) * a->अङ्कः_syms) == 0;
}

bool
action_equal(ध्रुव सङ्घ xkb_action *a, ध्रुव सङ्घ xkb_action *b)
{
    जइ (a->type != b->type)
        प्रत्यागम false;

    विकल्प (a->type) {
    पक्ष ACTION_TYPE_NONE:
    पक्ष ACTION_TYPE_VOID:
        प्रत्यागम true;
    पक्ष ACTION_TYPE_MOD_SET:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_LOCK:
        प्रत्यागम (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    पक्ष ACTION_TYPE_GROUP_SET:
    पक्ष ACTION_TYPE_GROUP_LATCH:
    पक्ष ACTION_TYPE_GROUP_LOCK:
        प्रत्यागम (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    पक्ष ACTION_TYPE_PTR_MOVE:
        प्रत्यागम (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    पक्ष ACTION_TYPE_PTR_BUTTON:
    पक्ष ACTION_TYPE_PTR_LOCK:
        प्रत्यागम (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.गणनम् == b->btn.गणनम्);
    पक्ष ACTION_TYPE_PTR_DEFAULT:
        प्रत्यागम (a->dflt.flags == b->dflt.flags &&
                a->dflt.मानम् == b->dflt.मानम्);
    पक्ष ACTION_TYPE_TERMINATE:
        प्रत्यागम true;
    पक्ष ACTION_TYPE_SWITCH_VT:
        प्रत्यागम (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    पक्ष ACTION_TYPE_CTRL_SET:
    पक्ष ACTION_TYPE_CTRL_LOCK:
        प्रत्यागम (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
        प्रत्यागम (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    पक्ष ACTION_TYPE_UNSUPPORTED_LEGACY:
    पक्ष ACTION_TYPE_UNKNOWN:
        प्रत्यागम true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    पक्ष ACTION_TYPE_INTERNAL:
        प्रत्यागम (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        प्रत्यागम memcmp(a->priv.data, b->priv.data, परिमाण(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(ध्रुव संरचना xkb_level *a, ध्रुव संरचना xkb_level *b)
{
    जइ (a->अङ्कः_actions != b->अङ्कः_actions)
        प्रत्यागम false;
    जइ (a->अङ्कः_actions <= 1)
        प्रत्यागम action_equal(&a->a.action, &b->a.action);
    प्रति (xkb_action_गणनम्_t k = 0; k < a->अङ्कः_actions; k++) {
        जइ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            प्रत्यागम false;
    }
    प्रत्यागम true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t अङ्कः_groups,
                      गणना xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_सङ्ख्या)
{
    जइ (अङ्कः_groups == 0)
        प्रत्यागम XKB_LAYOUT_INVALID;

    जइ (group >= 0 && (xkb_layout_index_t)group < अङ्कः_groups)
        प्रत्यागम (xkb_layout_index_t)group;

    विकल्प (out_of_range_group_policy) {
    पक्ष XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        जइ (out_of_range_group_सङ्ख्या >= अङ्कः_groups)
            प्रत्यागम 0;
        प्रत्यागम out_of_range_group_सङ्ख्या;

    पक्ष XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        जइ (group < 0)
            प्रत्यागम 0;
        अण्णहा
            प्रत्यागम अङ्कः_groups - 1;

    पक्ष XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    स्वभाव: {
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
        ध्रुव int32_t rem = group % (int32_t) अङ्कः_groups;
        प्रत्यागम (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)अङ्कः_groups);
    }
    }
}

xkb_action_गणनम्_t
xkb_keymap_कुञ्चिका_get_actions_by_level(ध्रुव संरचना xkb_keymap *keymap,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ध्रुव सङ्घ xkb_action **actions)
{
    जइ (!कुञ्चिका)
        गच्छ err;

    layout = xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, layout);
    जइ (layout == XKB_LAYOUT_INVALID)
        गच्छ err;

    जइ (level >= Xkb_कुञ्चिका_अङ्कः_Levels(कुञ्चिका, layout))
        गच्छ err;

    ध्रुव xkb_action_गणनम्_t गणनम् =
        कुञ्चिका->groups[layout].levels[level].अङ्कः_actions;
    विकल्प (गणनम्) {
        पक्ष 0:
            गच्छ err;
        पक्ष 1:
            *actions = &कुञ्चिका->groups[layout].levels[level].a.action;
            भङ्ग;
        स्वभाव:
            *actions = कुञ्चिका->groups[layout].levels[level].a.actions;
    }
    प्रत्यागम गणनम्;

err:
    *actions = रिक्त;
    प्रत्यागम 0;
}
