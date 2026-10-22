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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "ast-build.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "xkbcomp-priv.h"

સ્થિર bool
has_unbound_vmods(સંરચના xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    સંરચના xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        જો ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            પાછા true;
    પાછા false;
}

સ્થિર અંતઃરેખ શૂન્ય
ComputeEffectiveMask(સંરચના xkb_keymap *keymap, સંરચના xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    અચલ xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.અંક_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

સ્થિર શૂન્ય
UpdateActionMods(સંરચના xkb_keymap *keymap, સંઘ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    પસંદગી (act->type) {
    સ્થિતિ ACTION_TYPE_MOD_SET:
    સ્થિતિ ACTION_TYPE_MOD_LATCH:
    સ્થિતિ ACTION_TYPE_MOD_LOCK:
        જો (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        તોડો;
    મૂળભૂત:
        તોડો;
    }
}

સ્થિર અચલ સંરચના xkb_sym_interpret default_interpret = {
    .sym = XKB_ચાવી_No_પ્રતીક /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ચાવી_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .અંક_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

પ્રકારવ્યાખ્યા darray(અચલ સંરચના xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
સ્થિર bool
Find_Interp_For_ચાવી(સંરચના xkb_keymap *keymap, અચલ સંરચના xkb_ચાવી *ચાવી,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    અચલ xkb_keysym_t *syms;
    પૂર્ણાંક અંક_syms;

    અંક_syms = xkb_keymap_ચાવી_get_syms_by_level(keymap, ચાવી->keycode, group,
                                                level, &syms);
    જો (અંક_syms <= 0)
        પાછા false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    માટે (પૂર્ણાંક s = 0; s < અંક_syms; s++) {
        bool found = false;
        માટે (darray_size_t i = 0; i < keymap->અંક_sym_interprets; i++) {
            સંરચના xkb_sym_interpret * અચલ interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            જો (interp->sym != syms[s] && interp->sym != XKB_ચાવી_No_પ્રતીક)
                ચાલુ;

            જો (interp->level_one_only && level != 0)
                mods = 0;
            નહીંતર
                mods = ચાવી->modmap;

            પસંદગી (interp->match) {
            સ્થિતિ MATCH_NONE:
                found = !(interp->mods & mods);
                તોડો;
            સ્થિતિ MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                તોડો;
            સ્થિતિ MATCH_ANY:
                found = (interp->mods & mods);
                તોડો;
            સ્થિતિ MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                તોડો;
            સ્થિતિ MATCH_EXACTLY:
                found = (interp->mods == mods);
                તોડો;
            }

            જો (found && i > 0 && interp->sym == XKB_ચાવી_No_પ્રતીક) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                સંરચના xkb_sym_interpret અચલ **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    જો (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 ચાવી_Name_Text(keymap->ctx, ચાવી->name));
                        જાઓ not_found;
                    }
                }
            }
            જો (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                તોડો;
            }
        }
        જો (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    પાછા true;
}

સ્થિર bool
Apply_Interps_To_ચાવી(સંરચના xkb_keymap *keymap, સંરચના xkb_ચાવી *ચાવી)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(સંઘ xkb_action) actions = darray_new();

    માટે (xkb_layout_index_t group = 0; group < ચાવી->અંક_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        જો (ચાવી->groups[group].explicit_actions)
            ચાલુ;

        માટે (level = 0; level < Xkb_ચાવી_અંક_Levels(ચાવી, group); level++) {
            પ્રતિજ્ઞા(ચાવી->groups[group].levels[level].અંક_actions == 0);

            અચલ સંરચના xkb_sym_interpret **interp_iter;
            અચલ સંરચના xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            અચલ bool found = Find_Interp_For_ચાવી(keymap, ચાવી, group, level, &interprets);
            જો (!found)
                ચાલુ;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                જો (group == 0 && level == 0 &&
                    !(ચાવી->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ચાવી->repeats = true;
                }

                જો ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    જો (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                પસંદગી (interp->અંક_actions) {
                સ્થિતિ 0:
                    તોડો;
                સ્થિતિ 1:
                    darray_append(actions, interp->a.action);
                    તોડો;
                મૂળભૂત:
                    darray_append_વસ્તુઓ(actions, interp->a.actions,
                                        interp->અંક_actions);
                }
            }

            /* Copy the actions */
            જો (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ચાવી_Name_Text(keymap->ctx, ચાવી->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                ચાવી->groups[group].levels[level].અંક_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } નહીંતર {
                ચાવી->groups[group].levels[level].અંક_actions =
                    (xkb_action_ગણતરી_t) darray_size(actions);
            }
            પસંદગી (darray_size(actions)) {
                સ્થિતિ 0:
                    ચાવી->groups[group].levels[level].a.action =
                        (સંઘ xkb_action) { .type = ACTION_TYPE_NONE };
                    તોડો;
                સ્થિતિ 1:
                    ચાવી->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    તોડો;
                મૂળભૂત:
                    ચાવી->groups[group].levels[level].a.actions =
                        memdup(darray_વસ્તુઓ(actions),
                               ચાવી->groups[group].levels[level].અંક_actions,
                               કદ(*darray_વસ્તુઓ(actions)));
                    જો (!ચાવી->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        પાછા false;
                    }
            }

            જો (!darray_empty(actions))
                ચાવી->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        જો (ચાવી->groups[group].implicit_actions)
            ચાવી->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    જો (!(ચાવી->explicit & EXPLICIT_VMODMAP))
        ચાવી->vmodmap = vmodmap;

    પાછા true;
}

સ્થિર અંતઃરેખ bool
is_mod_action(સંઘ xkb_action *action)
{
    પાછા action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

સ્થિર અંતઃરેખ bool
is_group_action(સંઘ xkb_action *action)
{
    પાછા action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
સ્થિર શૂન્ય
CheckMultipleActionsCategories(સંરચના xkb_keymap *keymap, સંરચના xkb_ચાવી *ચાવી)
{
    માટે (xkb_layout_index_t g = 0; g < ચાવી->અંક_groups; g++) {
        માટે (xkb_level_index_t l = 0; l < Xkb_ચાવી_અંક_Levels(ચાવી, g); l++) {
            સંરચના xkb_level *level = &ચાવી->groups[g].levels[l];
            જો (level->અંક_actions <= 1)
                ચાલુ;
            માટે (xkb_action_ગણતરી_t i = 0; i < level->અંક_actions; i++) {
                સંઘ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                જો (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ચાવી))
                    ચાલુ;
                માટે (xkb_action_ગણતરી_t j = i + 1; j < level->અંક_actions; j++) {
                    સંઘ xkb_action *action2 = &level->a.actions[j];
                    જો (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        અચલ અક્ષર * અચલ type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ચાવી_Name_Text(keymap->ctx, ચાવી->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

સ્થિર શૂન્ય
add_ચાવી_aliases(સંરચના xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                સંરચના xkb_ચાવી_alias *aliases)
{
    માટે (darray_size_t alias = min; alias <= max; alias++) {
        અચલ KeycodeMatch entry = keymap->ચાવી_names[alias];
        જો (entry.is_alias && entry.found) {
            *aliases = (સંરચના xkb_ચાવી_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

સ્થિર bool
update_pending_ચાવી_fields(સંરચના xkb_keymap_info *info, સંરચના xkb_ચાવી *ચાવી)
{
    જો (ચાવી->out_of_range_pending_group) {
        સંરચના pending_computation * અચલ pc = &darray_item(
            *info->pending_computations, ચાવી->out_of_range_group_સંખ્યા
        );
        જો (!pc->computed) {
            xkb_layout_index_t group = 0;
            પસંદગી (પદાવલિ_Resolve_Group(info, pc->પદાવલિ, true, &group, ખાલી)) {
            સ્થિતિ PARSER_SUCCESS:
                pc->computed = true;
                pc->કિંમત = group - 1;
                તોડો;
            સ્થિતિ PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                પાછા (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            મૂળભૂત:
                /* Recover from error: ignore */;
            }
        }
        ચાવી->out_of_range_pending_group = false;
        ચાવી->out_of_range_group_સંખ્યા = pc->કિંમત;
    }
    પાછા true;
}

સ્થિર bool
update_pending_action_fields(સંરચના xkb_keymap_info *info,
                             xkb_keycode_t keycode, સંઘ xkb_action *act)
{
    પસંદગી (act->type) {
    સ્થિતિ ACTION_TYPE_GROUP_SET:
    સ્થિતિ ACTION_TYPE_GROUP_LATCH:
    સ્થિતિ ACTION_TYPE_GROUP_LOCK:
        જો (act->group.flags & ACTION_PENDING_COMPUTATION) {
            સંરચના pending_computation * અચલ pc =
                &darray_item(*info->pending_computations, act->group.group);
            જો (!pc->computed) {
                xkb_layout_index_t group = 0;
                અચલ bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                પસંદગી (પદાવલિ_Resolve_Group(info, pc->પદાવલિ, absolute, &group, ખાલી)) {
                સ્થિતિ PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    પાછા false;
                સ્થિતિ PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    તોડો;
                મૂળભૂત:
                    pc->computed = true;
                    જો (absolute) {
                        pc->કિંમત = group - 1;
                    } નહીંતર {
                        pc->કિંમત = group;
                        જો (pc->પદાવલિ->common.type == STMT_પદાવલિ_NEGATE)
                            pc->કિંમત = (uint32_t) (-(int32_t) pc->કિંમત);
                    }
                }
            }
            act->group.group = (int32_t) pc->કિંમત;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        પાછા true;
    સ્થિતિ ACTION_TYPE_REDIRECT_ચાવી: {
        જો (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ચાવી_auto) {
            /* No auto value to set */
            પાછા true;
        } નહીંતર {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        પાછા true;
    }
    મૂળભૂત:
        પાછા true;
    }
}

સ્થિર bool
update_pending_led_fields(સંરચના xkb_keymap_info *info, સંરચના xkb_led *led)
{
    જો (led->pending_groups) {
        સંરચના pending_computation * અચલ pc =
            &darray_item(*info->pending_computations, led->groups);
        જો (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            જો (!પદાવલિ_Resolve_Group_Mask(info, pc->પદાવલિ, &mask, ખાલી)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                પાછા false;
            }
            pc->computed = true;
            pc->કિંમત = mask;
        }
        led->pending_groups = false;
        led->groups = pc->કિંમત;
    }
    પાછા true;
}

/** Indices for the array "groupIndexNames" */
ગણના {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
ગણના {
    GROUP_MASK_NAME_LAST = 3,
};

સ્થિર શૂન્ય
update_auto_group_lock_on_modifier_release(સંરચના xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           સંઘ xkb_action * restrict action)
{
    જો (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].અંક_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->અંક_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        પાછા;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
સ્થિર bool
UpdateDerivedKeymapFields(સંરચના xkb_keymap_info *info)
{
    સંરચના xkb_keymap * restrict અચલ keymap = &info->keymap;

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
    static_assert(કદ(સંરચના xkb_ચાવી_alias) == કદ(KeycodeMatch), "");
#નહીંતર
    static_assert(કદ(સંરચના xkb_ચાવી_alias) == 2 * કદ(KeycodeMatch), "");
#endif
    darray_size_t અંક_ચાવી_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    માટે (xkb_atom_t alias = 0; alias < keymap->અંક_ચાવી_names; alias++) {
        અચલ KeycodeMatch entry = keymap->ચાવી_names[alias];
        જો (entry.is_alias && entry.found) {
            જો (!અંક_ચાવી_aliases)
                min_alias = alias;
            max_alias = alias;
            અંક_ચાવી_aliases++;
        }
    }
    જો (અંક_ચાવી_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        અચલ darray_size_t required_space = કદ(સંરચના xkb_ચાવી_alias)
                                           / કદ(KeycodeMatch)
                                           * અંક_ચાવી_aliases;
        પ્રતિજ્ઞા(અંક_ચાવી_aliases <= keymap->અંક_ચાવી_names);
        જો (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_ચાવી_aliases(keymap, min_alias, max_alias, keymap->ચાવી_aliases);
            /* Shrink */
            સંરચના xkb_ચાવી_alias * અચલ r =
                realloc(keymap->ચાવી_aliases,
                        અંક_ચાવી_aliases * કદ(*keymap->ચાવી_aliases));
            જો (!r)
                પાછા false;
            keymap->ચાવી_aliases = r;
        } નહીંતર જો (keymap->અંક_ચાવી_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            સંરચના xkb_ચાવી_alias * અચલ aliases = (સંરચના xkb_ચાવી_alias *) (
                keymap->ચાવી_names + max_alias + 1 +
                !is_aligned(keymap->ચાવી_names + max_alias + 1, કદ(*aliases))
            );
            add_ચાવી_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->ચાવી_aliases, aliases,
                   અંક_ચાવી_aliases * કદ(*keymap->ચાવી_aliases));
            /* Shrink */
            સંરચના xkb_ચાવી_alias * અચલ r =
                realloc(keymap->ચાવી_aliases,
                        અંક_ચાવી_aliases * કદ(*keymap->ચાવી_aliases));
            જો (!r)
                પાછા false;
            keymap->ચાવી_aliases = r;
        } નહીંતર {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            સંરચના xkb_ચાવી_alias * અચલ aliases = calloc(અંક_ચાવી_aliases,
                                                          કદ(*aliases));
            જો (!aliases)
                પાછા false;
            add_ચાવી_aliases(keymap, min_alias, max_alias, aliases);
            મુક્ત(keymap->ચાવી_names);
            keymap->ચાવી_aliases = aliases;
        }
    }
    keymap->અંક_ચાવી_aliases = અંક_ચાવી_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    સંરચના xkb_ચાવી *ચાવી;
    xkb_ચાવીઓ_foreach(ચાવી, keymap)
        keymap->અંક_groups = MAX(keymap->અંક_groups, ચાવી->અંક_groups);

    અચલ bool pending_computations = !darray_empty(*info->pending_computations);

    જો (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        અચલ xkb_layout_index_t અંક_groups
            = (keymap->અંક_groups)
            ? keymap->અંક_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .કિંમત = અંક_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .કિંમત = UINT32_C(1) << (અંક_groups - 1)
            };

        /* Update interpret fields with pending computations */
        માટે (darray_size_t i = 0; i < keymap->અંક_sym_interprets; i++) {
            સંરચના xkb_sym_interpret * restrict અચલ interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                જો (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    પાછા false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_ચાવીઓ_foreach(ચાવી, keymap) {
        જો (!Apply_Interps_To_ચાવી(keymap, ચાવી))
            પાછા false;
        CheckMultipleActionsCategories(keymap, ચાવી);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    સંરચના xkb_mod *mod;
    xkb_ચાવીઓ_foreach(ચાવી, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            જો (ચાવી->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ચાવી->modmap;

    જો (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            અચલ xkb_mod_mask_t mask = UINT32_C(1) << idx;
            જો (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    પ્રતિજ્ઞા(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    માટે (darray_size_t i = 0; i < keymap->અંક_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        માટે (darray_size_t j = 0; j < keymap->types[i].અંક_entries; j++) {
            જો (has_unbound_vmods(keymap,
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
                ચાલુ;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ચાવીઓ_foreach(ચાવી, keymap) {
        જો (!update_pending_ચાવી_fields(info, ચાવી))
            પાછા false;
        માટે (xkb_layout_index_t i = 0; i < ચાવી->અંક_groups; i++) {
            માટે (xkb_level_index_t j = 0; j < Xkb_ચાવી_અંક_Levels(ચાવી, i); j++) {
                સંરચના xkb_level * અચલ level = &ચાવી->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ચાવી->modmap);
                    જો ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ચાવી) &&
                        !update_pending_action_fields(info, ચાવી->keycode, act))
                    {
                        પાછા false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        ચાવી->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    સંરચના xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        જો (pending_computations &&
            !update_pending_led_fields(info, led))
            પાછા false;
    }

    પાછા true;
}

પ્રકારવ્યાખ્યા bool (*compile_file_fn)(XkbFile *file, સંરચના xkb_keymap_info *info);

સ્થિર અચલ compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ચાવી_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

સ્થિર શૂન્ય
pending_computations_array_free(pending_computation_array *p)
{
    સંરચના pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->પદાવલિ);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, સંરચના xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { ખાલી };
    ગણના xkb_file_type type;
    સંરચના xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    માટે (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        જો (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            જો (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } નહીંતર {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            ચાલુ;
        }

        જો (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            ચાલુ;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    સંરચના xkb_keymap_info info = {
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
                    keymap->અંક_groups ? GROUP_LAST_INDEX_NAME : ખાલી,
                    keymap->અંક_groups /* 1-indexed */
                },
                { ખાલી, 0 }
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
                    keymap->અંક_groups ? GROUP_LAST_INDEX_NAME : ખાલી,
                    /* Be extra cautious */
                    keymap->અંક_groups && keymap->અંક_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->અંક_groups - 1))
                        : 0
                },
                { ખાલી, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    માટે (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        જો (files[type] == ખાલી) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } નહીંતર {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        અચલ bool ok = compile_file_fns[type](files[type], &info);
        જો (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            પાછા false;
        }
    }

    અચલ bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    પાછા ok;
}
