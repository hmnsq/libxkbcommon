/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-names.h"
#include "features/enums.h"
#include "keymap.h"
#include "messages-codes.h"

static void
update_builtin_keymap_fields(sa_nracanaa xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    static const var_na *const builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    ສຳລັບ (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    strlen(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.num_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

sa_nracanaa xkb_keymap *
xkb_keymap_new(sa_nracanaa xkb_context *ctx, const var_na *func,
               ga_nanaa xkb_keymap_format format,
               ga_nanaa xkb_keymap_compile_flags flags)
{
    const ga_nanaa xkb_keymap_compile_flags invalid_flags = (
        flags & ~(ga_nanaa xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_VALUES
    );
    ຖ້າ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        ສົ່ງຄືນ NULL;
    }

    sa_nracanaa xkb_keymap * const keymap = calloc(1, parimaa_na(*keymap));
    ຖ້າ (!keymap)
        ສົ່ງຄືນ NULL;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    ສົ່ງຄືນ keymap;
}

void
XkbEscapeMapName(var_na *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    static const unsigned var_na legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    ຖ້າ (!name)
        ສົ່ງຄືນ;

    ໃນຂະນະທີ່ (*name) {
        unsigned var_na c = (unsigned var_na)*name;
        ຖ້າ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(const sa_nracanaa xkb_mod_set *mods, xkb_atom_t name,
                  ga_nanaa mod_type type)
{
    xkb_mod_index_t i;
    const sa_nracanaa xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        ຖ້າ ((mod->type & type) && name == mod->name)
            ສົ່ງຄືນ i;

    ສົ່ງຄືນ XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(const sa_nracanaa xkb_level *a, const sa_nracanaa xkb_level *b)
{
    ຖ້າ (a->num_syms != b->num_syms)
        ສົ່ງຄືນ false;
    ຖ້າ (a->num_syms <= 1)
        ສົ່ງຄືນ a->s.sym == b->s.sym;
    ສົ່ງຄືນ memcmp(a->s.syms, b->s.syms, parimaa_na(*a->s.syms) * a->num_syms) == 0;
}

bool
action_equal(const sa_ngha xkb_action *a, const sa_ngha xkb_action *b)
{
    ຖ້າ (a->type != b->type)
        ສົ່ງຄືນ false;

    switch (a->type) {
    pak_sa ACTION_TYPE_NONE:
    pak_sa ACTION_TYPE_VOID:
        ສົ່ງຄືນ true;
    pak_sa ACTION_TYPE_MOD_SET:
    pak_sa ACTION_TYPE_MOD_LATCH:
    pak_sa ACTION_TYPE_MOD_LOCK:
        ສົ່ງຄືນ (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    pak_sa ACTION_TYPE_GROUP_SET:
    pak_sa ACTION_TYPE_GROUP_LATCH:
    pak_sa ACTION_TYPE_GROUP_LOCK:
        ສົ່ງຄືນ (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    pak_sa ACTION_TYPE_PTR_MOVE:
        ສົ່ງຄືນ (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    pak_sa ACTION_TYPE_PTR_BUTTON:
    pak_sa ACTION_TYPE_PTR_LOCK:
        ສົ່ງຄືນ (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.count == b->btn.count);
    pak_sa ACTION_TYPE_PTR_DEFAULT:
        ສົ່ງຄືນ (a->dflt.flags == b->dflt.flags &&
                a->dflt.value == b->dflt.value);
    pak_sa ACTION_TYPE_TERMINATE:
        ສົ່ງຄືນ true;
    pak_sa ACTION_TYPE_SWITCH_VT:
        ສົ່ງຄືນ (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    pak_sa ACTION_TYPE_CTRL_SET:
    pak_sa ACTION_TYPE_CTRL_LOCK:
        ສົ່ງຄືນ (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    pak_sa ACTION_TYPE_REDIRECT_KEY:
        ສົ່ງຄືນ (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    pak_sa ACTION_TYPE_UNSUPPORTED_LEGACY:
    pak_sa ACTION_TYPE_UNKNOWN:
        ສົ່ງຄືນ true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    pak_sa ACTION_TYPE_INTERNAL:
        ສົ່ງຄືນ (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    default:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == _ACTION_TYPE_NUM_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        ສົ່ງຄືນ memcmp(a->priv.data, b->priv.data, parimaa_na(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(const sa_nracanaa xkb_level *a, const sa_nracanaa xkb_level *b)
{
    ຖ້າ (a->num_actions != b->num_actions)
        ສົ່ງຄືນ false;
    ຖ້າ (a->num_actions <= 1)
        ສົ່ງຄືນ action_equal(&a->a.action, &b->a.action);
    ສຳລັບ (xkb_action_count_t k = 0; k < a->num_actions; k++) {
        ຖ້າ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            ສົ່ງຄືນ false;
    }
    ສົ່ງຄືນ true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t num_groups,
                      ga_nanaa xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_number)
{
    ຖ້າ (num_groups == 0)
        ສົ່ງຄືນ XKB_LAYOUT_INVALID;

    ຖ້າ (group >= 0 && (xkb_layout_index_t)group < num_groups)
        ສົ່ງຄືນ (xkb_layout_index_t)group;

    switch (out_of_range_group_policy) {
    pak_sa XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        ຖ້າ (out_of_range_group_number >= num_groups)
            ສົ່ງຄືນ 0;
        ສົ່ງຄືນ out_of_range_group_number;

    pak_sa XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        ຖ້າ (group < 0)
            ສົ່ງຄືນ 0;
        ບໍ່ດັ່ງນັ້ນ
            ສົ່ງຄືນ num_groups - 1;

    pak_sa XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    default: {
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
        const int32_t rem = group % (int32_t) num_groups;
        ສົ່ງຄືນ (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)num_groups);
    }
    }
}

xkb_action_count_t
xkb_keymap_key_get_actions_by_level(const sa_nracanaa xkb_keymap *keymap,
                                    const sa_nracanaa xkb_key *key,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    const sa_ngha xkb_action **actions)
{
    ຖ້າ (!key)
        goto err;

    layout = xkb_keymap_key_effective_layout(key, layout);
    ຖ້າ (layout == XKB_LAYOUT_INVALID)
        goto err;

    ຖ້າ (level >= XkbKeyNumLevels(key, layout))
        goto err;

    const xkb_action_count_t count =
        key->groups[layout].levels[level].num_actions;
    switch (count) {
        pak_sa 0:
            goto err;
        pak_sa 1:
            *actions = &key->groups[layout].levels[level].a.action;
            bha_nga;
        default:
            *actions = key->groups[layout].levels[level].a.actions;
    }
    ສົ່ງຄືນ count;

err:
    *actions = NULL;
    ສົ່ງຄືນ 0;
}
