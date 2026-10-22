/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-names.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"

ସ୍ଥିର ଶୂନ୍ୟ
update_builtin_keymap_fields(ସଂରଚନା xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    ପାଇଁ (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.ଅଙ୍କ_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *func,
               ଗଣନା xkb_keymap_format format,
               ଗଣନା xkb_keymap_compile_flags flags)
{
    ଅଚଳ ଗଣନା xkb_keymap_compile_flags invalid_flags = (
        flags & ~(ଗଣନା xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ
    );
    ଯଦି (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_keymap * ଅଚଳ keymap = calloc(1, ଆକାର(*keymap));
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    ଫେରନ୍ତୁ keymap;
}

ଶୂନ୍ୟ
XkbEscapeMapName(ଅକ୍ଷର *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    ଯଦି (!name)
        ଫେରନ୍ତୁ;

    ଯେତେବେଳେ (*name) {
        unsigned ଅକ୍ଷର c = (unsigned ଅକ୍ଷର)*name;
        ଯଦି (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(ଅଚଳ ସଂରଚନା xkb_mod_set *mods, xkb_atom_t name,
                  ଗଣନା mod_type type)
{
    xkb_mod_index_t i;
    ଅଚଳ ସଂରଚନା xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        ଯଦି ((mod->type & type) && name == mod->name)
            ଫେରନ୍ତୁ i;

    ଫେରନ୍ତୁ XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(ଅଚଳ ସଂରଚନା xkb_level *a, ଅଚଳ ସଂରଚନା xkb_level *b)
{
    ଯଦି (a->ଅଙ୍କ_syms != b->ଅଙ୍କ_syms)
        ଫେରନ୍ତୁ false;
    ଯଦି (a->ଅଙ୍କ_syms <= 1)
        ଫେରନ୍ତୁ a->s.sym == b->s.sym;
    ଫେରନ୍ତୁ memcmp(a->s.syms, b->s.syms, ଆକାର(*a->s.syms) * a->ଅଙ୍କ_syms) == 0;
}

bool
action_equal(ଅଚଳ ସଂଘ xkb_action *a, ଅଚଳ ସଂଘ xkb_action *b)
{
    ଯଦି (a->type != b->type)
        ଫେରନ୍ତୁ false;

    ବାଛ (a->type) {
    ସ୍ଥିତି ACTION_TYPE_NONE:
    ସ୍ଥିତି ACTION_TYPE_VOID:
        ଫେରନ୍ତୁ true;
    ସ୍ଥିତି ACTION_TYPE_MOD_SET:
    ସ୍ଥିତି ACTION_TYPE_MOD_LATCH:
    ସ୍ଥିତି ACTION_TYPE_MOD_LOCK:
        ଫେରନ୍ତୁ (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    ସ୍ଥିତି ACTION_TYPE_GROUP_SET:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LATCH:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LOCK:
        ଫେରନ୍ତୁ (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    ସ୍ଥିତି ACTION_TYPE_PTR_MOVE:
        ଫେରନ୍ତୁ (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    ସ୍ଥିତି ACTION_TYPE_PTR_BUTTON:
    ସ୍ଥିତି ACTION_TYPE_PTR_LOCK:
        ଫେରନ୍ତୁ (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.ଗଣନା_2 == b->btn.ଗଣନା_2);
    ସ୍ଥିତି ACTION_TYPE_PTR_DEFAULT:
        ଫେରନ୍ତୁ (a->dflt.flags == b->dflt.flags &&
                a->dflt.ମୂଲ୍ୟ == b->dflt.ମୂଲ୍ୟ);
    ସ୍ଥିତି ACTION_TYPE_TERMINATE:
        ଫେରନ୍ତୁ true;
    ସ୍ଥିତି ACTION_TYPE_SWITCH_VT:
        ଫେରନ୍ତୁ (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    ସ୍ଥିତି ACTION_TYPE_CTRL_SET:
    ସ୍ଥିତି ACTION_TYPE_CTRL_LOCK:
        ଫେରନ୍ତୁ (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି:
        ଫେରନ୍ତୁ (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    ସ୍ଥିତି ACTION_TYPE_UNSUPPORTED_LEGACY:
    ସ୍ଥିତି ACTION_TYPE_UNKNOWN:
        ଫେରନ୍ତୁ true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    ସ୍ଥିତି ACTION_TYPE_INTERNAL:
        ଫେରନ୍ତୁ (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        ଫେରନ୍ତୁ memcmp(a->priv.data, b->priv.data, ଆକାର(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(ଅଚଳ ସଂରଚନା xkb_level *a, ଅଚଳ ସଂରଚନା xkb_level *b)
{
    ଯଦି (a->ଅଙ୍କ_actions != b->ଅଙ୍କ_actions)
        ଫେରନ୍ତୁ false;
    ଯଦି (a->ଅଙ୍କ_actions <= 1)
        ଫେରନ୍ତୁ action_equal(&a->a.action, &b->a.action);
    ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < a->ଅଙ୍କ_actions; k++) {
        ଯଦି (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            ଫେରନ୍ତୁ false;
    }
    ଫେରନ୍ତୁ true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t ଅଙ୍କ_groups,
                      ଗଣନା xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_ସଂଖ୍ୟା)
{
    ଯଦି (ଅଙ୍କ_groups == 0)
        ଫେରନ୍ତୁ XKB_LAYOUT_INVALID;

    ଯଦି (group >= 0 && (xkb_layout_index_t)group < ଅଙ୍କ_groups)
        ଫେରନ୍ତୁ (xkb_layout_index_t)group;

    ବାଛ (out_of_range_group_policy) {
    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        ଯଦି (out_of_range_group_ସଂଖ୍ୟା >= ଅଙ୍କ_groups)
            ଫେରନ୍ତୁ 0;
        ଫେରନ୍ତୁ out_of_range_group_ସଂଖ୍ୟା;

    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        ଯଦି (group < 0)
            ଫେରନ୍ତୁ 0;
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ ଅଙ୍କ_groups - 1;

    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
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
        ଅଚଳ int32_t rem = group % (int32_t) ଅଙ୍କ_groups;
        ଫେରନ୍ତୁ (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)ଅଙ୍କ_groups);
    }
    }
}

xkb_action_ଗଣନା_t
xkb_keymap_ଚାବି_get_actions_by_level(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
                                    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ଅଚଳ ସଂଘ xkb_action **actions)
{
    ଯଦି (!ଚାବି)
        ଯାଅ err;

    layout = xkb_keymap_ଚାବି_effective_layout(ଚାବି, layout);
    ଯଦି (layout == XKB_LAYOUT_INVALID)
        ଯାଅ err;

    ଯଦି (level >= Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, layout))
        ଯାଅ err;

    ଅଚଳ xkb_action_ଗଣନା_t ଗଣନା_2 =
        ଚାବି->groups[layout].levels[level].ଅଙ୍କ_actions;
    ବାଛ (ଗଣନା_2) {
        ସ୍ଥିତି 0:
            ଯାଅ err;
        ସ୍ଥିତି 1:
            *actions = &ଚାବି->groups[layout].levels[level].a.action;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            *actions = ଚାବି->groups[layout].levels[level].a.actions;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;

err:
    *actions = ଖାଲି;
    ଫେରନ୍ତୁ 0;
}
