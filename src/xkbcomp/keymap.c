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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "ast-build.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

ಸ್ಥಿರ bool
has_unbound_vmods(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    ಸಂರಚನಾ xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        ಯದಿ ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            ಪ್ರತ್ಯಾಗಮ true;
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
ComputeEffectiveMask(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ಧ್ರುವ xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.ಅಂಕೆ_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ಸ್ಥಿರ ಶೂನ್ಯ
UpdateActionMods(ಸಂರಚನಾ xkb_keymap *keymap, ಸಙ್ಘ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    ವಿಕಲ್ಪ (act->type) {
    ಪಕ್ಷ ACTION_TYPE_MOD_SET:
    ಪಕ್ಷ ACTION_TYPE_MOD_LATCH:
    ಪಕ್ಷ ACTION_TYPE_MOD_LOCK:
        ಯದಿ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret default_interpret = {
    .sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .ಅಂಕೆ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ಸ್ಥಿರ bool
Find_Interp_For_ಕೀಲಿ(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    ಧ್ರುವ xkb_keysym_t *syms;
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_syms;

    ಅಂಕೆ_syms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(keymap, ಕೀಲಿ->keycode, group,
                                                level, &syms);
    ಯದಿ (ಅಂಕೆ_syms <= 0)
        ಪ್ರತ್ಯಾಗಮ false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < ಅಂಕೆ_syms; s++) {
        bool found = false;
        ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_sym_interprets; i++) {
            ಸಂರಚನಾ xkb_sym_interpret * ಧ್ರುವ interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            ಯದಿ (interp->sym != syms[s] && interp->sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                ಅನುವರ್ತ;

            ಯದಿ (interp->level_one_only && level != 0)
                mods = 0;
            ಅನ್ಯಥಾ
                mods = ಕೀಲಿ->modmap;

            ವಿಕಲ್ಪ (interp->match) {
            ಪಕ್ಷ MATCH_NONE:
                found = !(interp->mods & mods);
                ಭಙ್ಗ;
            ಪಕ್ಷ MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                ಭಙ್ಗ;
            ಪಕ್ಷ MATCH_ANY:
                found = (interp->mods & mods);
                ಭಙ್ಗ;
            ಪಕ್ಷ MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                ಭಙ್ಗ;
            ಪಕ್ಷ MATCH_EXACTLY:
                found = (interp->mods == mods);
                ಭಙ್ಗ;
            }

            ಯದಿ (found && i > 0 && interp->sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ಸಂರಚನಾ xkb_sym_interpret ಧ್ರುವ **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    ಯದಿ (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 ಕೀಲಿ_Name_Text(keymap->ctx, ಕೀಲಿ->name));
                        ಗಚ್ಛ not_found;
                    }
                }
            }
            ಯದಿ (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Apply_Interps_To_ಕೀಲಿ(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(ಸಙ್ಘ xkb_action) actions = darray_new();

    ಪ್ರತಿ (xkb_layout_index_t group = 0; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        ಯದಿ (ಕೀಲಿ->groups[group].explicit_actions)
            ಅನುವರ್ತ;

        ಪ್ರತಿ (level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, group); level++) {
            ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions == 0);

            ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret **interp_iter;
            ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            ಧ್ರುವ bool found = Find_Interp_For_ಕೀಲಿ(keymap, ಕೀಲಿ, group, level, &interprets);
            ಯದಿ (!found)
                ಅನುವರ್ತ;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                ಯದಿ (group == 0 && level == 0 &&
                    !(ಕೀಲಿ->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ಕೀಲಿ->repeats = true;
                }

                ಯದಿ ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    ಯದಿ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                ವಿಕಲ್ಪ (interp->ಅಂಕೆ_actions) {
                ಪಕ್ಷ 0:
                    ಭಙ್ಗ;
                ಪಕ್ಷ 1:
                    darray_append(actions, interp->a.action);
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    darray_append_ವಸ್ತುಗಳು(actions, interp->a.actions,
                                        interp->ಅಂಕೆ_actions);
                }
            }

            /* Copy the actions */
            ಯದಿ (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ಕೀಲಿ_Name_Text(keymap->ctx, ಕೀಲಿ->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } ಅನ್ಯಥಾ {
                ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions =
                    (xkb_action_ಎಣಿಕೆ_t) darray_size(actions);
            }
            ವಿಕಲ್ಪ (darray_size(actions)) {
                ಪಕ್ಷ 0:
                    ಕೀಲಿ->groups[group].levels[level].a.action =
                        (ಸಙ್ಘ xkb_action) { .type = ACTION_TYPE_NONE };
                    ಭಙ್ಗ;
                ಪಕ್ಷ 1:
                    ಕೀಲಿ->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    ಕೀಲಿ->groups[group].levels[level].a.actions =
                        memdup(darray_ವಸ್ತುಗಳು(actions),
                               ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions,
                               ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(actions)));
                    ಯದಿ (!ಕೀಲಿ->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ಪ್ರತ್ಯಾಗಮ false;
                    }
            }

            ಯದಿ (!darray_empty(actions))
                ಕೀಲಿ->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        ಯದಿ (ಕೀಲಿ->groups[group].implicit_actions)
            ಕೀಲಿ->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    ಯದಿ (!(ಕೀಲಿ->explicit & EXPLICIT_VMODMAP))
        ಕೀಲಿ->vmodmap = vmodmap;

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_mod_action(ಸಙ್ಘ xkb_action *action)
{
    ಪ್ರತ್ಯಾಗಮ action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_group_action(ಸಙ್ಘ xkb_action *action)
{
    ಪ್ರತ್ಯಾಗಮ action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ಸ್ಥಿರ ಶೂನ್ಯ
CheckMultipleActionsCategories(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಪ್ರತಿ (xkb_layout_index_t g = 0; g < ಕೀಲಿ->ಅಂಕೆ_groups; g++) {
        ಪ್ರತಿ (xkb_level_index_t l = 0; l < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, g); l++) {
            ಸಂರಚನಾ xkb_level *level = &ಕೀಲಿ->groups[g].levels[l];
            ಯದಿ (level->ಅಂಕೆ_actions <= 1)
                ಅನುವರ್ತ;
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t i = 0; i < level->ಅಂಕೆ_actions; i++) {
                ಸಙ್ಘ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                ಯದಿ (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ಕೀಲಿ))
                    ಅನುವರ್ತ;
                ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t j = i + 1; j < level->ಅಂಕೆ_actions; j++) {
                    ಸಙ್ಘ xkb_action *action2 = &level->a.actions[j];
                    ಯದಿ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ಕೀಲಿ_Name_Text(keymap->ctx, ಕೀಲಿ->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
add_ಕೀಲಿ_aliases(ಸಂರಚನಾ xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                ಸಂರಚನಾ xkb_ಕೀಲಿ_alias *aliases)
{
    ಪ್ರತಿ (darray_size_t alias = min; alias <= max; alias++) {
        ಧ್ರುವ KeycodeMatch entry = keymap->ಕೀಲಿ_names[alias];
        ಯದಿ (entry.is_alias && entry.found) {
            *aliases = (ಸಂರಚನಾ xkb_ಕೀಲಿ_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

ಸ್ಥಿರ bool
update_pending_ಕೀಲಿ_fields(ಸಂರಚನಾ xkb_keymap_info *info, ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಯದಿ (ಕೀಲಿ->out_of_range_pending_group) {
        ಸಂರಚನಾ pending_computation * ಧ್ರುವ pc = &darray_item(
            *info->pending_computations, ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ
        );
        ಯದಿ (!pc->computed) {
            xkb_layout_index_t group = 0;
            ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info, pc->ಅಭಿವ್ಯಕ್ತಿ, true, &group, ರಿಕ್ತ)) {
            ಪಕ್ಷ PARSER_SUCCESS:
                pc->computed = true;
                pc->ಮೌಲ್ಯ = group - 1;
                ಭಙ್ಗ;
            ಪಕ್ಷ PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                ಪ್ರತ್ಯಾಗಮ (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            ಸ್ವಭಾವ:
                /* Recover from error: ignore */;
            }
        }
        ಕೀಲಿ->out_of_range_pending_group = false;
        ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ = pc->ಮೌಲ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
update_pending_action_fields(ಸಂರಚನಾ xkb_keymap_info *info,
                             xkb_keycode_t keycode, ಸಙ್ಘ xkb_action *act)
{
    ವಿಕಲ್ಪ (act->type) {
    ಪಕ್ಷ ACTION_TYPE_GROUP_SET:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LATCH:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LOCK:
        ಯದಿ (act->group.flags & ACTION_PENDING_COMPUTATION) {
            ಸಂರಚನಾ pending_computation * ಧ್ರುವ pc =
                &darray_item(*info->pending_computations, act->group.group);
            ಯದಿ (!pc->computed) {
                xkb_layout_index_t group = 0;
                ಧ್ರುವ bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(info, pc->ಅಭಿವ್ಯಕ್ತಿ, absolute, &group, ರಿಕ್ತ)) {
                ಪಕ್ಷ PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    ಪ್ರತ್ಯಾಗಮ false;
                ಪಕ್ಷ PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    pc->computed = true;
                    ಯದಿ (absolute) {
                        pc->ಮೌಲ್ಯ = group - 1;
                    } ಅನ್ಯಥಾ {
                        pc->ಮೌಲ್ಯ = group;
                        ಯದಿ (pc->ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE)
                            pc->ಮೌಲ್ಯ = (uint32_t) (-(int32_t) pc->ಮೌಲ್ಯ);
                    }
                }
            }
            act->group.group = (int32_t) pc->ಮೌಲ್ಯ;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ ACTION_TYPE_REDIRECT_ಕೀಲಿ: {
        ಯದಿ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ಕೀಲಿ_auto) {
            /* No auto value to set */
            ಪ್ರತ್ಯಾಗಮ true;
        } ಅನ್ಯಥಾ {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ಪ್ರತ್ಯಾಗಮ true;
    }
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ true;
    }
}

ಸ್ಥಿರ bool
update_pending_led_fields(ಸಂರಚನಾ xkb_keymap_info *info, ಸಂರಚನಾ xkb_led *led)
{
    ಯದಿ (led->pending_groups) {
        ಸಂರಚನಾ pending_computation * ಧ್ರುವ pc =
            &darray_item(*info->pending_computations, led->groups);
        ಯದಿ (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group_Mask(info, pc->ಅಭಿವ್ಯಕ್ತಿ, &mask, ರಿಕ್ತ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                ಪ್ರತ್ಯಾಗಮ false;
            }
            pc->computed = true;
            pc->ಮೌಲ್ಯ = mask;
        }
        led->pending_groups = false;
        led->groups = pc->ಮೌಲ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

/** Indices for the array "groupIndexNames" */
ಗಣನಾ {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
ಗಣನಾ {
    GROUP_MASK_NAME_LAST = 3,
};

ಸ್ಥಿರ ಶೂನ್ಯ
update_auto_group_lock_on_modifier_release(ಸಂರಚನಾ xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           ಸಙ್ಘ xkb_action * restrict action)
{
    ಯದಿ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].ಅಂಕೆ_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->ಅಂಕೆ_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ಸ್ಥಿರ bool
UpdateDerivedKeymapFields(ಸಂರಚನಾ xkb_keymap_info *info)
{
    ಸಂರಚನಾ xkb_keymap * restrict ಧ್ರುವ keymap = &info->keymap;

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
    static_assert(ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_ಕೀಲಿ_alias) == ಪರಿಮಾಣ(KeycodeMatch), "");
#ಅನ್ಯಥಾ
    static_assert(ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_ಕೀಲಿ_alias) == 2 * ಪರಿಮಾಣ(KeycodeMatch), "");
#endif
    darray_size_t ಅಂಕೆ_ಕೀಲಿ_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    ಪ್ರತಿ (xkb_atom_t alias = 0; alias < keymap->ಅಂಕೆ_ಕೀಲಿ_names; alias++) {
        ಧ್ರುವ KeycodeMatch entry = keymap->ಕೀಲಿ_names[alias];
        ಯದಿ (entry.is_alias && entry.found) {
            ಯದಿ (!ಅಂಕೆ_ಕೀಲಿ_aliases)
                min_alias = alias;
            max_alias = alias;
            ಅಂಕೆ_ಕೀಲಿ_aliases++;
        }
    }
    ಯದಿ (ಅಂಕೆ_ಕೀಲಿ_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ಧ್ರುವ darray_size_t required_space = ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_ಕೀಲಿ_alias)
                                           / ಪರಿಮಾಣ(KeycodeMatch)
                                           * ಅಂಕೆ_ಕೀಲಿ_aliases;
        ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_ಕೀಲಿ_aliases <= keymap->ಅಂಕೆ_ಕೀಲಿ_names);
        ಯದಿ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, keymap->ಕೀಲಿ_aliases);
            /* Shrink */
            ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ r =
                realloc(keymap->ಕೀಲಿ_aliases,
                        ಅಂಕೆ_ಕೀಲಿ_aliases * ಪರಿಮಾಣ(*keymap->ಕೀಲಿ_aliases));
            ಯದಿ (!r)
                ಪ್ರತ್ಯಾಗಮ false;
            keymap->ಕೀಲಿ_aliases = r;
        } ಅನ್ಯಥಾ ಯದಿ (keymap->ಅಂಕೆ_ಕೀಲಿ_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ aliases = (ಸಂರಚನಾ xkb_ಕೀಲಿ_alias *) (
                keymap->ಕೀಲಿ_names + max_alias + 1 +
                !is_aligned(keymap->ಕೀಲಿ_names + max_alias + 1, ಪರಿಮಾಣ(*aliases))
            );
            add_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->ಕೀಲಿ_aliases, aliases,
                   ಅಂಕೆ_ಕೀಲಿ_aliases * ಪರಿಮಾಣ(*keymap->ಕೀಲಿ_aliases));
            /* Shrink */
            ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ r =
                realloc(keymap->ಕೀಲಿ_aliases,
                        ಅಂಕೆ_ಕೀಲಿ_aliases * ಪರಿಮಾಣ(*keymap->ಕೀಲಿ_aliases));
            ಯದಿ (!r)
                ಪ್ರತ್ಯಾಗಮ false;
            keymap->ಕೀಲಿ_aliases = r;
        } ಅನ್ಯಥಾ {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ aliases = calloc(ಅಂಕೆ_ಕೀಲಿ_aliases,
                                                          ಪರಿಮಾಣ(*aliases));
            ಯದಿ (!aliases)
                ಪ್ರತ್ಯಾಗಮ false;
            add_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, aliases);
            ಮುಕ್ತ(keymap->ಕೀಲಿ_names);
            keymap->ಕೀಲಿ_aliases = aliases;
        }
    }
    keymap->ಅಂಕೆ_ಕೀಲಿ_aliases = ಅಂಕೆ_ಕೀಲಿ_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        keymap->ಅಂಕೆ_groups = MAX(keymap->ಅಂಕೆ_groups, ಕೀಲಿ->ಅಂಕೆ_groups);

    ಧ್ರುವ bool pending_computations = !darray_empty(*info->pending_computations);

    ಯದಿ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ಧ್ರುವ xkb_layout_index_t ಅಂಕೆ_groups
            = (keymap->ಅಂಕೆ_groups)
            ? keymap->ಅಂಕೆ_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .ಮೌಲ್ಯ = ಅಂಕೆ_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .ಮೌಲ್ಯ = UINT32_C(1) << (ಅಂಕೆ_groups - 1)
            };

        /* Update interpret fields with pending computations */
        ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_sym_interprets; i++) {
            ಸಂರಚನಾ xkb_sym_interpret * restrict ಧ್ರುವ interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                ಯದಿ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ಪ್ರತ್ಯಾಗಮ false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಯದಿ (!Apply_Interps_To_ಕೀಲಿ(keymap, ಕೀಲಿ))
            ಪ್ರತ್ಯಾಗಮ false;
        CheckMultipleActionsCategories(keymap, ಕೀಲಿ);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    ಸಂರಚನಾ xkb_mod *mod;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            ಯದಿ (ಕೀಲಿ->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ಕೀಲಿ->modmap;

    ಯದಿ (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            ಧ್ರುವ xkb_mod_mask_t mask = UINT32_C(1) << idx;
            ಯದಿ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    ಪ್ರತಿಜ್ಞಾ(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        ಪ್ರತಿ (darray_size_t j = 0; j < keymap->types[i].ಅಂಕೆ_entries; j++) {
            ಯದಿ (has_unbound_vmods(keymap,
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
                ಅನುವರ್ತ;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಯದಿ (!update_pending_ಕೀಲಿ_fields(info, ಕೀಲಿ))
            ಪ್ರತ್ಯಾಗಮ false;
        ಪ್ರತಿ (xkb_layout_index_t i = 0; i < ಕೀಲಿ->ಅಂಕೆ_groups; i++) {
            ಪ್ರತಿ (xkb_level_index_t j = 0; j < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, i); j++) {
                ಸಂರಚನಾ xkb_level * ಧ್ರುವ level = &ಕೀಲಿ->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ಕೀಲಿ->modmap);
                    ಯದಿ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ಕೀಲಿ) &&
                        !update_pending_action_fields(info, ಕೀಲಿ->keycode, act))
                    {
                        ಪ್ರತ್ಯಾಗಮ false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        ಕೀಲಿ->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ಸಂರಚನಾ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        ಯದಿ (pending_computations &&
            !update_pending_led_fields(info, led))
            ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಪ್ರಕಾರನಿರ್ದೇಶ bool (*compile_file_fn)(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *info);

ಸ್ಥಿರ ಧ್ರುವ compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ಕೀಲಿ_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ಸ್ಥಿರ ಶೂನ್ಯ
pending_computations_array_free(pending_computation_array *p)
{
    ಸಂರಚನಾ pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->ಅಭಿವ್ಯಕ್ತಿ);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ಸಂರಚನಾ xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { ರಿಕ್ತ };
    ಗಣನಾ xkb_file_type type;
    ಸಂರಚನಾ xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    ಪ್ರತಿ (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        ಯದಿ (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            ಯದಿ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } ಅನ್ಯಥಾ {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            ಅನುವರ್ತ;
        }

        ಯದಿ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            ಅನುವರ್ತ;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ಸಂರಚನಾ xkb_keymap_info info = {
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
                    keymap->ಅಂಕೆ_groups ? GROUP_LAST_INDEX_NAME : ರಿಕ್ತ,
                    keymap->ಅಂಕೆ_groups /* 1-indexed */
                },
                { ರಿಕ್ತ, 0 }
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
                    keymap->ಅಂಕೆ_groups ? GROUP_LAST_INDEX_NAME : ರಿಕ್ತ,
                    /* Be extra cautious */
                    keymap->ಅಂಕೆ_groups && keymap->ಅಂಕೆ_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->ಅಂಕೆ_groups - 1))
                        : 0
                },
                { ರಿಕ್ತ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    ಪ್ರತಿ (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        ಯದಿ (files[type] == ರಿಕ್ತ) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } ಅನ್ಯಥಾ {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ಧ್ರುವ bool ok = compile_file_fns[type](files[type], &info);
        ಯದಿ (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

    ಧ್ರುವ bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ಪ್ರತ್ಯಾಗಮ ok;
}
