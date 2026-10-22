/*
 * Copyright © 2009 Dan Nicholson
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Dan Nicholson <dbn.lists@gmail.com>
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Ran Benita <ran234@gmail.com>
 */

#include "config.h"

#include <string.h>
#include <stdbool.h>

#include "xkbcommon/xkbcommon.h"
#include "ast-build.h"
#include "darray.h"
#include "expr.h"
#include "keymap.h"
#include "text.h"
#include "utils.h"
#include "xkbcomp-priv.h"

static bool
has_unbound_vmods(sa_nracanaa xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    sa_nracanaa xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        གལ་ཏེ ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            ཕྱིར true;
    ཕྱིར false;
}

static anta_hstha void
ComputeEffectiveMask(sa_nracanaa xkb_keymap *keymap, sa_nracanaa xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    const xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.num_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

static void
UpdateActionMods(sa_nracanaa xkb_keymap *keymap, sa_ngha xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    switch (act->type) {
    pak_sa ACTION_TYPE_MOD_SET:
    pak_sa ACTION_TYPE_MOD_LATCH:
    pak_sa ACTION_TYPE_MOD_LOCK:
        གལ་ཏེ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        bha_nga;
    default:
        bha_nga;
    }
}

static const sa_nracanaa xkb_sym_interpret default_interpret = {
    .sym = XKB_KEY_NoSymbol /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_KEY_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .num_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

typedef darray(const sa_nracanaa xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
static bool
FindInterpForKey(sa_nracanaa xkb_keymap *keymap, const sa_nracanaa xkb_key *key,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    const xkb_keysym_t *syms;
    ཧྲིལ num_syms;

    num_syms = xkb_keymap_key_get_syms_by_level(keymap, key->keycode, group,
                                                level, &syms);
    གལ་ཏེ (num_syms <= 0)
        ཕྱིར false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    ཆེད་དུ (ཧྲིལ s = 0; s < num_syms; s++) {
        bool found = false;
        ཆེད་དུ (darray_size_t i = 0; i < keymap->num_sym_interprets; i++) {
            sa_nracanaa xkb_sym_interpret * const interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            གལ་ཏེ (interp->sym != syms[s] && interp->sym != XKB_KEY_NoSymbol)
                continue;

            གལ་ཏེ (interp->level_one_only && level != 0)
                mods = 0;
            དེ་མིན
                mods = key->modmap;

            switch (interp->match) {
            pak_sa MATCH_NONE:
                found = !(interp->mods & mods);
                bha_nga;
            pak_sa MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                bha_nga;
            pak_sa MATCH_ANY:
                found = (interp->mods & mods);
                bha_nga;
            pak_sa MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                bha_nga;
            pak_sa MATCH_EXACTLY:
                found = (interp->mods == mods);
                bha_nga;
            }

            གལ་ཏེ (found && i > 0 && interp->sym == XKB_KEY_NoSymbol) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                sa_nracanaa xkb_sym_interpret const **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    གལ་ཏེ (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 KeyNameText(keymap->ctx, key->name));
                        goto not_found;
                    }
                }
            }
            གལ་ཏེ (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                bha_nga;
            }
        }
        གལ་ཏེ (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    ཕྱིར true;
}

static bool
ApplyInterpsToKey(sa_nracanaa xkb_keymap *keymap, sa_nracanaa xkb_key *key)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(sa_ngha xkb_action) actions = darray_new();

    ཆེད་དུ (xkb_layout_index_t group = 0; group < key->num_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        གལ་ཏེ (key->groups[group].explicit_actions)
            continue;

        ཆེད་དུ (level = 0; level < XkbKeyNumLevels(key, group); level++) {
            assert(key->groups[group].levels[level].num_actions == 0);

            const sa_nracanaa xkb_sym_interpret **interp_iter;
            const sa_nracanaa xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            const bool found = FindInterpForKey(keymap, key, group, level, &interprets);
            གལ་ཏེ (!found)
                continue;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                གལ་ཏེ (group == 0 && level == 0 &&
                    !(key->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    key->repeats = true;
                }

                གལ་ཏེ ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    གལ་ཏེ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                switch (interp->num_actions) {
                pak_sa 0:
                    bha_nga;
                pak_sa 1:
                    darray_append(actions, interp->a.action);
                    bha_nga;
                default:
                    darray_append_items(actions, interp->a.actions,
                                        interp->num_actions);
                }
            }

            /* Copy the actions */
            གལ་ཏེ (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         KeyNameText(keymap->ctx, key->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                key->groups[group].levels[level].num_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } དེ་མིན {
                key->groups[group].levels[level].num_actions =
                    (xkb_action_count_t) darray_size(actions);
            }
            switch (darray_size(actions)) {
                pak_sa 0:
                    key->groups[group].levels[level].a.action =
                        (sa_ngha xkb_action) { .type = ACTION_TYPE_NONE };
                    bha_nga;
                pak_sa 1:
                    key->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    bha_nga;
                default:
                    key->groups[group].levels[level].a.actions =
                        memdup(darray_items(actions),
                               key->groups[group].levels[level].num_actions,
                               parimaa_na(*darray_items(actions)));
                    གལ་ཏེ (!key->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ཕྱིར false;
                    }
            }

            གལ་ཏེ (!darray_empty(actions))
                key->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        གལ་ཏེ (key->groups[group].implicit_actions)
            key->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    གལ་ཏེ (!(key->explicit & EXPLICIT_VMODMAP))
        key->vmodmap = vmodmap;

    ཕྱིར true;
}

static anta_hstha bool
is_mod_action(sa_ngha xkb_action *action)
{
    ཕྱིར action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

static anta_hstha bool
is_group_action(sa_ngha xkb_action *action)
{
    ཕྱིར action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
static void
CheckMultipleActionsCategories(sa_nracanaa xkb_keymap *keymap, sa_nracanaa xkb_key *key)
{
    ཆེད་དུ (xkb_layout_index_t g = 0; g < key->num_groups; g++) {
        ཆེད་དུ (xkb_level_index_t l = 0; l < XkbKeyNumLevels(key, g); l++) {
            sa_nracanaa xkb_level *level = &key->groups[g].levels[l];
            གལ་ཏེ (level->num_actions <= 1)
                continue;
            ཆེད་དུ (xkb_action_count_t i = 0; i < level->num_actions; i++) {
                sa_ngha xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                གལ་ཏེ (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_KEY))
                    continue;
                ཆེད་དུ (xkb_action_count_t j = i + 1; j < level->num_actions; j++) {
                    sa_ngha xkb_action *action2 = &level->a.actions[j];
                    གལ་ཏེ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        const var_na * const type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, KeyNameText(keymap->ctx, key->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

static void
add_key_aliases(sa_nracanaa xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                sa_nracanaa xkb_key_alias *aliases)
{
    ཆེད་དུ (darray_size_t alias = min; alias <= max; alias++) {
        const KeycodeMatch entry = keymap->key_names[alias];
        གལ་ཏེ (entry.is_alias && entry.found) {
            *aliases = (sa_nracanaa xkb_key_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

static bool
update_pending_key_fields(sa_nracanaa xkb_keymap_info *info, sa_nracanaa xkb_key *key)
{
    གལ་ཏེ (key->out_of_range_pending_group) {
        sa_nracanaa pending_computation * const pc = &darray_item(
            *info->pending_computations, key->out_of_range_group_number
        );
        གལ་ཏེ (!pc->computed) {
            xkb_layout_index_t group = 0;
            switch (ExprResolveGroup(info, pc->expr, true, &group, NULL)) {
            pak_sa PARSER_SUCCESS:
                pc->computed = true;
                pc->value = group - 1;
                bha_nga;
            pak_sa PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                ཕྱིར (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            default:
                /* Recover from error: ignore */;
            }
        }
        key->out_of_range_pending_group = false;
        key->out_of_range_group_number = pc->value;
    }
    ཕྱིར true;
}

static bool
update_pending_action_fields(sa_nracanaa xkb_keymap_info *info,
                             xkb_keycode_t keycode, sa_ngha xkb_action *act)
{
    switch (act->type) {
    pak_sa ACTION_TYPE_GROUP_SET:
    pak_sa ACTION_TYPE_GROUP_LATCH:
    pak_sa ACTION_TYPE_GROUP_LOCK:
        གལ་ཏེ (act->group.flags & ACTION_PENDING_COMPUTATION) {
            sa_nracanaa pending_computation * const pc =
                &darray_item(*info->pending_computations, act->group.group);
            གལ་ཏེ (!pc->computed) {
                xkb_layout_index_t group = 0;
                const bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                switch (ExprResolveGroup(info, pc->expr, absolute, &group, NULL)) {
                pak_sa PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    ཕྱིར false;
                pak_sa PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    bha_nga;
                default:
                    pc->computed = true;
                    གལ་ཏེ (absolute) {
                        pc->value = group - 1;
                    } དེ་མིན {
                        pc->value = group;
                        གལ་ཏེ (pc->expr->common.type == STMT_EXPR_NEGATE)
                            pc->value = (uint32_t) (-(int32_t) pc->value);
                    }
                }
            }
            act->group.group = (int32_t) pc->value;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ཕྱིར true;
    pak_sa ACTION_TYPE_REDIRECT_KEY: {
        གལ་ཏེ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_key_auto) {
            /* No auto value to set */
            ཕྱིར true;
        } དེ་མིན {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ཕྱིར true;
    }
    default:
        ཕྱིར true;
    }
}

static bool
update_pending_led_fields(sa_nracanaa xkb_keymap_info *info, sa_nracanaa xkb_led *led)
{
    གལ་ཏེ (led->pending_groups) {
        sa_nracanaa pending_computation * const pc =
            &darray_item(*info->pending_computations, led->groups);
        གལ་ཏེ (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            གལ་ཏེ (!ExprResolveGroupMask(info, pc->expr, &mask, NULL)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                ཕྱིར false;
            }
            pc->computed = true;
            pc->value = mask;
        }
        led->pending_groups = false;
        led->groups = pc->value;
    }
    ཕྱིར true;
}

/** Indices for the array "groupIndexNames" */
ga_nanaa {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
ga_nanaa {
    GROUP_MASK_NAME_LAST = 3,
};

static void
update_auto_group_lock_on_modifier_release(sa_nracanaa xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           sa_ngha xkb_action * restrict action)
{
    གལ་ཏེ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].num_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->num_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        ཕྱིར;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
static bool
UpdateDerivedKeymapFields(sa_nracanaa xkb_keymap_info *info)
{
    sa_nracanaa xkb_keymap * restrict const keymap = &info->keymap;

    /**
     * Create the key alias list
     *
     * Since we already allocated the key name LUT, we try to overwrite it:
     * - An alias entry is exactly 2× the size of a LUT entry (on most compilers).
     * - We ensure that there is a real key for each alias.
     * - So, if each alias maps to a different keycode, we usually have:
     *   current alloc(keymap.key_names) ≥ needed alloc(keymap.key_aliases).
     * - Usual keymaps have enough space in the LUT to enable overwriting real
     *   key entries without overlap with the alias entries.
     * - In the unlikely event that this is not the case, we need to realloc.
     */
#ifdef _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(parimaa_na(sa_nracanaa xkb_key_alias) == parimaa_na(KeycodeMatch), "");
#དེ་མིན
    static_assert(parimaa_na(sa_nracanaa xkb_key_alias) == 2 * parimaa_na(KeycodeMatch), "");
#endif
    darray_size_t num_key_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    ཆེད་དུ (xkb_atom_t alias = 0; alias < keymap->num_key_names; alias++) {
        const KeycodeMatch entry = keymap->key_names[alias];
        གལ་ཏེ (entry.is_alias && entry.found) {
            གལ་ཏེ (!num_key_aliases)
                min_alias = alias;
            max_alias = alias;
            num_key_aliases++;
        }
    }
    གལ་ཏེ (num_key_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        const darray_size_t required_space = parimaa_na(sa_nracanaa xkb_key_alias)
                                           / parimaa_na(KeycodeMatch)
                                           * num_key_aliases;
        assert(num_key_aliases <= keymap->num_key_names);
        གལ་ཏེ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_key_aliases(keymap, min_alias, max_alias, keymap->key_aliases);
            /* Shrink */
            sa_nracanaa xkb_key_alias * const r =
                realloc(keymap->key_aliases,
                        num_key_aliases * parimaa_na(*keymap->key_aliases));
            གལ་ཏེ (!r)
                ཕྱིར false;
            keymap->key_aliases = r;
        } དེ་མིན གལ་ཏེ (keymap->num_key_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            sa_nracanaa xkb_key_alias * const aliases = (sa_nracanaa xkb_key_alias *) (
                keymap->key_names + max_alias + 1 +
                !is_aligned(keymap->key_names + max_alias + 1, parimaa_na(*aliases))
            );
            add_key_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->key_aliases, aliases,
                   num_key_aliases * parimaa_na(*keymap->key_aliases));
            /* Shrink */
            sa_nracanaa xkb_key_alias * const r =
                realloc(keymap->key_aliases,
                        num_key_aliases * parimaa_na(*keymap->key_aliases));
            གལ་ཏེ (!r)
                ཕྱིར false;
            keymap->key_aliases = r;
        } དེ་མིན {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            sa_nracanaa xkb_key_alias * const aliases = calloc(num_key_aliases,
                                                          parimaa_na(*aliases));
            གལ་ཏེ (!aliases)
                ཕྱིར false;
            add_key_aliases(keymap, min_alias, max_alias, aliases);
            free(keymap->key_names);
            keymap->key_aliases = aliases;
        }
    }
    keymap->num_key_aliases = num_key_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    sa_nracanaa xkb_key *key;
    xkb_keys_foreach(key, keymap)
        keymap->num_groups = MAX(keymap->num_groups, key->num_groups);

    const bool pending_computations = !darray_empty(*info->pending_computations);

    གལ་ཏེ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        const xkb_layout_index_t num_groups
            = (keymap->num_groups)
            ? keymap->num_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .value = num_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .value = UINT32_C(1) << (num_groups - 1)
            };

        /* Update interpret fields with pending computations */
        ཆེད་དུ (darray_size_t i = 0; i < keymap->num_sym_interprets; i++) {
            sa_nracanaa xkb_sym_interpret * restrict const interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                གལ་ཏེ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ཕྱིར false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_keys_foreach(key, keymap) {
        གལ་ཏེ (!ApplyInterpsToKey(keymap, key))
            ཕྱིར false;
        CheckMultipleActionsCategories(keymap, key);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    sa_nracanaa xkb_mod *mod;
    xkb_keys_foreach(key, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            གལ་ཏེ (key->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= key->modmap;

    གལ་ཏེ (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
        /*
         * XKB extension: canonical virtual modifiers (incompatible with X11)
         *
         * Virtual modifiers that are not mapped, map to their canonical
         * encoding, i.e. themselves.
         *
         * This excludes:
         * - Modifiers mapped *explicitly*: e.g. `virtual_modifiers M = 0x1;`.
         * - Modifiers mapped *implicitly* via vmodmap/modifier_map, i.e. the
         *   usual X11 way used in xkeyboard-config.
         */
        xkb_vmods_enumerate(idx, mod, &keymap->mods) {
            const xkb_mod_mask_t mask = UINT32_C(1) << idx;
            གལ་ཏེ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
                mod->mapping = mask;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= mask;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    assert(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    ཆེད་དུ (darray_size_t i = 0; i < keymap->num_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        ཆེད་དུ (darray_size_t j = 0; j < keymap->types[i].num_entries; j++) {
            གལ་ཏེ (has_unbound_vmods(keymap,
                                  keymap->types[i].entries[j].mods.mods)) {
                /*
                 * Map entries which specify unbound virtual modifiers are not
                 * considered (see the XKB protocol, section “Determining the
                 * KeySym Associated with a Key Event”).
                 *
                 * Deactivate entry by zeroing its mod mask and skipping further
                 * processing.
                 *
                 * See also: `entry_is_active`.
                 */
                keymap->types[i].entries[j].mods.mask = 0;
                continue;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_keys_foreach(key, keymap) {
        གལ་ཏེ (!update_pending_key_fields(info, key))
            ཕྱིར false;
        ཆེད་དུ (xkb_layout_index_t i = 0; i < key->num_groups; i++) {
            ཆེད་དུ (xkb_level_index_t j = 0; j < XkbKeyNumLevels(key, i); j++) {
                sa_nracanaa xkb_level * const level = &key->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, key->modmap);
                    གལ་ཏེ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_KEY) &&
                        !update_pending_action_fields(info, key->keycode, act))
                    {
                        ཕྱིར false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        key->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    sa_nracanaa xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        གལ་ཏེ (pending_computations &&
            !update_pending_led_fields(info, led))
            ཕྱིར false;
    }

    ཕྱིར true;
}

typedef bool (*compile_file_fn)(XkbFile *file, sa_nracanaa xkb_keymap_info *info);

static const compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = CompileKeyTypes,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

static void
pending_computations_array_free(pending_computation_array *p)
{
    sa_nracanaa pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->expr);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, sa_nracanaa xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { NULL };
    ga_nanaa xkb_file_type type;
    sa_nracanaa xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    ཆེད་དུ (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        གལ་ཏེ (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            གལ་ཏེ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } དེ་མིན {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            continue;
        }

        གལ་ཏེ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            continue;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    sa_nracanaa xkb_keymap_info info = {
        /* Copy the keymap */
        .keymap = *keymap,
        .strict = parser_strict_flags_from_keymap(keymap),
        .features = {
            .max_groups = format_max_groups(keymap->format),
            .max_overlays = format_max_overlays(keymap->format),
            .controls_name_offset = format_control_names_offset(keymap->format),
            .group_lock_on_release =
                isGroupLockOnReleaseSupported(keymap->format),
            .mods_unlock_on_press =
                isModsUnLockOnPressSupported(keymap->format),
            .mods_latch_on_press =
                isModsLatchOnPressSupported(keymap->format),
            .overlapping_overlays =
                areOverlappingOverlaysSupported(keymap->format),
        },
        /*
         * NOTE: `first` and `last` group constants are never used for
         *       serialization, in order to maintain compatibility with
         *       xkbcomp and older xkbcommon versions.
         */
        .lookup = {
            .groupIndexNames = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [GROUP_INDEX_NAME_LAST] = {
                    keymap->num_groups ? GROUP_LAST_INDEX_NAME : NULL,
                    keymap->num_groups /* 1-indexed */
                },
                { NULL, 0 }
            },
            .groupMaskNames = {
                { "none", 0x00 },
                { "first", 0x01 },
                { "all", XKB_ALL_GROUPS },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [GROUP_MASK_NAME_LAST] = {
                    keymap->num_groups ? GROUP_LAST_INDEX_NAME : NULL,
                    /* Be extra cautious */
                    keymap->num_groups && keymap->num_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->num_groups - 1))
                        : 0
                },
                { NULL, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    ཆེད་དུ (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        གལ་ཏེ (files[type] == NULL) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } དེ་མིན {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        const bool ok = compile_file_fns[type](files[type], &info);
        གལ་ཏེ (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ཕྱིར false;
        }
    }

    const bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ཕྱིར ok;
}
