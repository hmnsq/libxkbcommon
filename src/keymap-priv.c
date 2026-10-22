/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-names.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "messages-codes.h"

நிலையான வெறுமை
update_builtin_keymap_fields(கட்டமைப்பு xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    நிலையான மாறிலி எழுத்து *மாறிலி builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    க்கு (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.இலக்கம்_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *func,
               எண்ணிக்கை xkb_keymap_format format,
               எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    மாறிலி எண்ணிக்கை xkb_keymap_compile_flags invalid_flags = (
        flags & ~(எண்ணிக்கை xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_மதிப்புகள்
    );
    எனில் (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_keymap * மாறிலி keymap = calloc(1, அளவு(*keymap));
    எனில் (!keymap)
        திரும்பு வெற்று;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    திரும்பு keymap;
}

வெறுமை
XkbEscapeMapName(எழுத்து *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    நிலையான மாறிலி unsigned எழுத்து legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    எனில் (!name)
        திரும்பு;

    போது (*name) {
        unsigned எழுத்து c = (unsigned எழுத்து)*name;
        எனில் (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(மாறிலி கட்டமைப்பு xkb_mod_set *mods, xkb_atom_t name,
                  எண்ணிக்கை mod_type type)
{
    xkb_mod_index_t i;
    மாறிலி கட்டமைப்பு xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        எனில் ((mod->type & type) && name == mod->name)
            திரும்பு i;

    திரும்பு XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(மாறிலி கட்டமைப்பு xkb_level *a, மாறிலி கட்டமைப்பு xkb_level *b)
{
    எனில் (a->இலக்கம்_syms != b->இலக்கம்_syms)
        திரும்பு false;
    எனில் (a->இலக்கம்_syms <= 1)
        திரும்பு a->s.sym == b->s.sym;
    திரும்பு memcmp(a->s.syms, b->s.syms, அளவு(*a->s.syms) * a->இலக்கம்_syms) == 0;
}

bool
action_equal(மாறிலி ஒன்றிப்பு xkb_action *a, மாறிலி ஒன்றிப்பு xkb_action *b)
{
    எனில் (a->type != b->type)
        திரும்பு false;

    தேர்வு (a->type) {
    நிலை ACTION_TYPE_NONE:
    நிலை ACTION_TYPE_VOID:
        திரும்பு true;
    நிலை ACTION_TYPE_MOD_SET:
    நிலை ACTION_TYPE_MOD_LATCH:
    நிலை ACTION_TYPE_MOD_LOCK:
        திரும்பு (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    நிலை ACTION_TYPE_GROUP_SET:
    நிலை ACTION_TYPE_GROUP_LATCH:
    நிலை ACTION_TYPE_GROUP_LOCK:
        திரும்பு (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    நிலை ACTION_TYPE_PTR_MOVE:
        திரும்பு (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    நிலை ACTION_TYPE_PTR_BUTTON:
    நிலை ACTION_TYPE_PTR_LOCK:
        திரும்பு (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.மொத்தம் == b->btn.மொத்தம்);
    நிலை ACTION_TYPE_PTR_DEFAULT:
        திரும்பு (a->dflt.flags == b->dflt.flags &&
                a->dflt.மதிப்பு == b->dflt.மதிப்பு);
    நிலை ACTION_TYPE_TERMINATE:
        திரும்பு true;
    நிலை ACTION_TYPE_SWITCH_VT:
        திரும்பு (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    நிலை ACTION_TYPE_CTRL_SET:
    நிலை ACTION_TYPE_CTRL_LOCK:
        திரும்பு (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    நிலை ACTION_TYPE_REDIRECT_சாவி:
        திரும்பு (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    நிலை ACTION_TYPE_UNSUPPORTED_LEGACY:
    நிலை ACTION_TYPE_UNKNOWN:
        திரும்பு true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    நிலை ACTION_TYPE_INTERNAL:
        திரும்பு (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    இயல்பு:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        திரும்பு memcmp(a->priv.data, b->priv.data, அளவு(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(மாறிலி கட்டமைப்பு xkb_level *a, மாறிலி கட்டமைப்பு xkb_level *b)
{
    எனில் (a->இலக்கம்_actions != b->இலக்கம்_actions)
        திரும்பு false;
    எனில் (a->இலக்கம்_actions <= 1)
        திரும்பு action_equal(&a->a.action, &b->a.action);
    க்கு (xkb_action_மொத்தம்_t k = 0; k < a->இலக்கம்_actions; k++) {
        எனில் (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            திரும்பு false;
    }
    திரும்பு true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t இலக்கம்_groups,
                      எண்ணிக்கை xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_எண்)
{
    எனில் (இலக்கம்_groups == 0)
        திரும்பு XKB_LAYOUT_INVALID;

    எனில் (group >= 0 && (xkb_layout_index_t)group < இலக்கம்_groups)
        திரும்பு (xkb_layout_index_t)group;

    தேர்வு (out_of_range_group_policy) {
    நிலை XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        எனில் (out_of_range_group_எண் >= இலக்கம்_groups)
            திரும்பு 0;
        திரும்பு out_of_range_group_எண்;

    நிலை XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        எனில் (group < 0)
            திரும்பு 0;
        இல்லையெனில்
            திரும்பு இலக்கம்_groups - 1;

    நிலை XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    இயல்பு: {
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
        மாறிலி int32_t rem = group % (int32_t) இலக்கம்_groups;
        திரும்பு (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)இலக்கம்_groups);
    }
    }
}

xkb_action_மொத்தம்_t
xkb_keymap_சாவி_get_actions_by_level(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                                    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    மாறிலி ஒன்றிப்பு xkb_action **actions)
{
    எனில் (!சாவி)
        செல் err;

    layout = xkb_keymap_சாவி_effective_layout(சாவி, layout);
    எனில் (layout == XKB_LAYOUT_INVALID)
        செல் err;

    எனில் (level >= Xkb_சாவி_இலக்கம்_Levels(சாவி, layout))
        செல் err;

    மாறிலி xkb_action_மொத்தம்_t மொத்தம் =
        சாவி->groups[layout].levels[level].இலக்கம்_actions;
    தேர்வு (மொத்தம்) {
        நிலை 0:
            செல் err;
        நிலை 1:
            *actions = &சாவி->groups[layout].levels[level].a.action;
            நிறுத்து;
        இயல்பு:
            *actions = சாவி->groups[layout].levels[level].a.actions;
    }
    திரும்பு மொத்தம்;

err:
    *actions = வெற்று;
    திரும்பு 0;
}
