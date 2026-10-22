/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-names.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "messages-codes.h"

સ્થિર શૂન્ય
update_builtin_keymap_fields(સંરચના xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    સ્થિર અચલ અક્ષર *અચલ builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    માટે (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.અંક_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

સંરચના xkb_keymap *
xkb_keymap_new(સંરચના xkb_context *ctx, અચલ અક્ષર *func,
               ગણના xkb_keymap_format format,
               ગણના xkb_keymap_compile_flags flags)
{
    અચલ ગણના xkb_keymap_compile_flags invalid_flags = (
        flags & ~(ગણના xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_કિંમતો
    );
    જો (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        પાછા ખાલી;
    }

    સંરચના xkb_keymap * અચલ keymap = calloc(1, કદ(*keymap));
    જો (!keymap)
        પાછા ખાલી;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    પાછા keymap;
}

શૂન્ય
XkbEscapeMapName(અક્ષર *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    સ્થિર અચલ unsigned અક્ષર legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    જો (!name)
        પાછા;

    જ્યારે (*name) {
        unsigned અક્ષર c = (unsigned અક્ષર)*name;
        જો (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(અચલ સંરચના xkb_mod_set *mods, xkb_atom_t name,
                  ગણના mod_type type)
{
    xkb_mod_index_t i;
    અચલ સંરચના xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        જો ((mod->type & type) && name == mod->name)
            પાછા i;

    પાછા XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(અચલ સંરચના xkb_level *a, અચલ સંરચના xkb_level *b)
{
    જો (a->અંક_syms != b->અંક_syms)
        પાછા false;
    જો (a->અંક_syms <= 1)
        પાછા a->s.sym == b->s.sym;
    પાછા memcmp(a->s.syms, b->s.syms, કદ(*a->s.syms) * a->અંક_syms) == 0;
}

bool
action_equal(અચલ સંઘ xkb_action *a, અચલ સંઘ xkb_action *b)
{
    જો (a->type != b->type)
        પાછા false;

    પસંદગી (a->type) {
    સ્થિતિ ACTION_TYPE_NONE:
    સ્થિતિ ACTION_TYPE_VOID:
        પાછા true;
    સ્થિતિ ACTION_TYPE_MOD_SET:
    સ્થિતિ ACTION_TYPE_MOD_LATCH:
    સ્થિતિ ACTION_TYPE_MOD_LOCK:
        પાછા (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    સ્થિતિ ACTION_TYPE_GROUP_SET:
    સ્થિતિ ACTION_TYPE_GROUP_LATCH:
    સ્થિતિ ACTION_TYPE_GROUP_LOCK:
        પાછા (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    સ્થિતિ ACTION_TYPE_PTR_MOVE:
        પાછા (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    સ્થિતિ ACTION_TYPE_PTR_BUTTON:
    સ્થિતિ ACTION_TYPE_PTR_LOCK:
        પાછા (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.ગણતરી == b->btn.ગણતરી);
    સ્થિતિ ACTION_TYPE_PTR_DEFAULT:
        પાછા (a->dflt.flags == b->dflt.flags &&
                a->dflt.કિંમત == b->dflt.કિંમત);
    સ્થિતિ ACTION_TYPE_TERMINATE:
        પાછા true;
    સ્થિતિ ACTION_TYPE_SWITCH_VT:
        પાછા (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    સ્થિતિ ACTION_TYPE_CTRL_SET:
    સ્થિતિ ACTION_TYPE_CTRL_LOCK:
        પાછા (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    સ્થિતિ ACTION_TYPE_REDIRECT_ચાવી:
        પાછા (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    સ્થિતિ ACTION_TYPE_UNSUPPORTED_LEGACY:
    સ્થિતિ ACTION_TYPE_UNKNOWN:
        પાછા true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    સ્થિતિ ACTION_TYPE_INTERNAL:
        પાછા (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    મૂળભૂત:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_અંક_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        પાછા memcmp(a->priv.data, b->priv.data, કદ(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(અચલ સંરચના xkb_level *a, અચલ સંરચના xkb_level *b)
{
    જો (a->અંક_actions != b->અંક_actions)
        પાછા false;
    જો (a->અંક_actions <= 1)
        પાછા action_equal(&a->a.action, &b->a.action);
    માટે (xkb_action_ગણતરી_t k = 0; k < a->અંક_actions; k++) {
        જો (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            પાછા false;
    }
    પાછા true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t અંક_groups,
                      ગણના xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_સંખ્યા)
{
    જો (અંક_groups == 0)
        પાછા XKB_LAYOUT_INVALID;

    જો (group >= 0 && (xkb_layout_index_t)group < અંક_groups)
        પાછા (xkb_layout_index_t)group;

    પસંદગી (out_of_range_group_policy) {
    સ્થિતિ XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        જો (out_of_range_group_સંખ્યા >= અંક_groups)
            પાછા 0;
        પાછા out_of_range_group_સંખ્યા;

    સ્થિતિ XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        જો (group < 0)
            પાછા 0;
        નહીંતર
            પાછા અંક_groups - 1;

    સ્થિતિ XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    મૂળભૂત: {
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
        અચલ int32_t rem = group % (int32_t) અંક_groups;
        પાછા (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)અંક_groups);
    }
    }
}

xkb_action_ગણતરી_t
xkb_keymap_ચાવી_get_actions_by_level(અચલ સંરચના xkb_keymap *keymap,
                                    અચલ સંરચના xkb_ચાવી *ચાવી,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    અચલ સંઘ xkb_action **actions)
{
    જો (!ચાવી)
        જાઓ err;

    layout = xkb_keymap_ચાવી_effective_layout(ચાવી, layout);
    જો (layout == XKB_LAYOUT_INVALID)
        જાઓ err;

    જો (level >= Xkb_ચાવી_અંક_Levels(ચાવી, layout))
        જાઓ err;

    અચલ xkb_action_ગણતરી_t ગણતરી =
        ચાવી->groups[layout].levels[level].અંક_actions;
    પસંદગી (ગણતરી) {
        સ્થિતિ 0:
            જાઓ err;
        સ્થિતિ 1:
            *actions = &ચાવી->groups[layout].levels[level].a.action;
            તોડો;
        મૂળભૂત:
            *actions = ચાવી->groups[layout].levels[level].a.actions;
    }
    પાછા ગણતરી;

err:
    *actions = ખાલી;
    પાછા 0;
}
