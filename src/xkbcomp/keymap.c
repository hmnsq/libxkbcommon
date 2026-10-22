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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "ast-build.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

സ്ഥിതം bool
has_unbound_vmods(ഘടന xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    ഘടന xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        എങ്കിൽ ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            തിരികെ true;
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
ComputeEffectiveMask(ഘടന xkb_keymap *keymap, ഘടന xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    സ്ഥിരം xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.അക്കം_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

സ്ഥിതം ശൂന്യം
UpdateActionMods(ഘടന xkb_keymap *keymap, സംഘം xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    തിരഞ്ഞെടുപ്പ് (act->type) {
    അവസ്ഥ ACTION_TYPE_MOD_SET:
    അവസ്ഥ ACTION_TYPE_MOD_LATCH:
    അവസ്ഥ ACTION_TYPE_MOD_LOCK:
        എങ്കിൽ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        നിർത്തുക;
    }
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_sym_interpret default_interpret = {
    .sym = XKB_താക്കോൽ_No_ചിഹ്നം /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_താക്കോൽ_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .അക്കം_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

തരനിർവചനം darray(സ്ഥിരം ഘടന xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
സ്ഥിതം bool
Find_Interp_For_താക്കോൽ(ഘടന xkb_keymap *keymap, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    സ്ഥിരം xkb_keysym_t *syms;
    പൂർണ്ണസംഖ്യ അക്കം_syms;

    അക്കം_syms = xkb_keymap_താക്കോൽ_get_syms_by_level(keymap, താക്കോൽ->keycode, group,
                                                level, &syms);
    എങ്കിൽ (അക്കം_syms <= 0)
        തിരികെ false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < അക്കം_syms; s++) {
        bool found = false;
        വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_sym_interprets; i++) {
            ഘടന xkb_sym_interpret * സ്ഥിരം interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            എങ്കിൽ (interp->sym != syms[s] && interp->sym != XKB_താക്കോൽ_No_ചിഹ്നം)
                തുടരുക;

            എങ്കിൽ (interp->level_one_only && level != 0)
                mods = 0;
            അല്ലെങ്കിൽ
                mods = താക്കോൽ->modmap;

            തിരഞ്ഞെടുപ്പ് (interp->match) {
            അവസ്ഥ MATCH_NONE:
                found = !(interp->mods & mods);
                നിർത്തുക;
            അവസ്ഥ MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                നിർത്തുക;
            അവസ്ഥ MATCH_ANY:
                found = (interp->mods & mods);
                നിർത്തുക;
            അവസ്ഥ MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                നിർത്തുക;
            അവസ്ഥ MATCH_EXACTLY:
                found = (interp->mods == mods);
                നിർത്തുക;
            }

            എങ്കിൽ (found && i > 0 && interp->sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ഘടന xkb_sym_interpret സ്ഥിരം **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    എങ്കിൽ (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 താക്കോൽ_Name_Text(keymap->ctx, താക്കോൽ->name));
                        പോകുക not_found;
                    }
                }
            }
            എങ്കിൽ (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    തിരികെ true;
}

സ്ഥിതം bool
Apply_Interps_To_താക്കോൽ(ഘടന xkb_keymap *keymap, ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(സംഘം xkb_action) actions = darray_new();

    വേണ്ടി (xkb_layout_index_t group = 0; group < താക്കോൽ->അക്കം_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        എങ്കിൽ (താക്കോൽ->groups[group].explicit_actions)
            തുടരുക;

        വേണ്ടി (level = 0; level < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, group); level++) {
            പ്രതിജ്ഞാ(താക്കോൽ->groups[group].levels[level].അക്കം_actions == 0);

            സ്ഥിരം ഘടന xkb_sym_interpret **interp_iter;
            സ്ഥിരം ഘടന xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            സ്ഥിരം bool found = Find_Interp_For_താക്കോൽ(keymap, താക്കോൽ, group, level, &interprets);
            എങ്കിൽ (!found)
                തുടരുക;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                എങ്കിൽ (group == 0 && level == 0 &&
                    !(താക്കോൽ->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    താക്കോൽ->repeats = true;
                }

                എങ്കിൽ ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    എങ്കിൽ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                തിരഞ്ഞെടുപ്പ് (interp->അക്കം_actions) {
                അവസ്ഥ 0:
                    നിർത്തുക;
                അവസ്ഥ 1:
                    darray_append(actions, interp->a.action);
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    darray_append_ഇനങ്ങൾ(actions, interp->a.actions,
                                        interp->അക്കം_actions);
                }
            }

            /* Copy the actions */
            എങ്കിൽ (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         താക്കോൽ_Name_Text(keymap->ctx, താക്കോൽ->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                താക്കോൽ->groups[group].levels[level].അക്കം_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } അല്ലെങ്കിൽ {
                താക്കോൽ->groups[group].levels[level].അക്കം_actions =
                    (xkb_action_എണ്ണം_t) darray_size(actions);
            }
            തിരഞ്ഞെടുപ്പ് (darray_size(actions)) {
                അവസ്ഥ 0:
                    താക്കോൽ->groups[group].levels[level].a.action =
                        (സംഘം xkb_action) { .type = ACTION_TYPE_NONE };
                    നിർത്തുക;
                അവസ്ഥ 1:
                    താക്കോൽ->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    താക്കോൽ->groups[group].levels[level].a.actions =
                        memdup(darray_ഇനങ്ങൾ(actions),
                               താക്കോൽ->groups[group].levels[level].അക്കം_actions,
                               വലുപ്പം(*darray_ഇനങ്ങൾ(actions)));
                    എങ്കിൽ (!താക്കോൽ->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        തിരികെ false;
                    }
            }

            എങ്കിൽ (!darray_empty(actions))
                താക്കോൽ->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        എങ്കിൽ (താക്കോൽ->groups[group].implicit_actions)
            താക്കോൽ->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    എങ്കിൽ (!(താക്കോൽ->explicit & EXPLICIT_VMODMAP))
        താക്കോൽ->vmodmap = vmodmap;

    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
is_mod_action(സംഘം xkb_action *action)
{
    തിരികെ action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

സ്ഥിതം വരിയിൽ bool
is_group_action(സംഘം xkb_action *action)
{
    തിരികെ action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
സ്ഥിതം ശൂന്യം
CheckMultipleActionsCategories(ഘടന xkb_keymap *keymap, ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    വേണ്ടി (xkb_layout_index_t g = 0; g < താക്കോൽ->അക്കം_groups; g++) {
        വേണ്ടി (xkb_level_index_t l = 0; l < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, g); l++) {
            ഘടന xkb_level *level = &താക്കോൽ->groups[g].levels[l];
            എങ്കിൽ (level->അക്കം_actions <= 1)
                തുടരുക;
            വേണ്ടി (xkb_action_എണ്ണം_t i = 0; i < level->അക്കം_actions; i++) {
                സംഘം xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                എങ്കിൽ (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_താക്കോൽ))
                    തുടരുക;
                വേണ്ടി (xkb_action_എണ്ണം_t j = i + 1; j < level->അക്കം_actions; j++) {
                    സംഘം xkb_action *action2 = &level->a.actions[j];
                    എങ്കിൽ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        സ്ഥിരം അക്ഷരം * സ്ഥിരം type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, താക്കോൽ_Name_Text(keymap->ctx, താക്കോൽ->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

സ്ഥിതം ശൂന്യം
add_താക്കോൽ_aliases(ഘടന xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                ഘടന xkb_താക്കോൽ_alias *aliases)
{
    വേണ്ടി (darray_size_t alias = min; alias <= max; alias++) {
        സ്ഥിരം KeycodeMatch entry = keymap->താക്കോൽ_names[alias];
        എങ്കിൽ (entry.is_alias && entry.found) {
            *aliases = (ഘടന xkb_താക്കോൽ_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

സ്ഥിതം bool
update_pending_താക്കോൽ_fields(ഘടന xkb_keymap_info *info, ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    എങ്കിൽ (താക്കോൽ->out_of_range_pending_group) {
        ഘടന pending_computation * സ്ഥിരം pc = &darray_item(
            *info->pending_computations, താക്കോൽ->out_of_range_group_സംഖ്യ
        );
        എങ്കിൽ (!pc->computed) {
            xkb_layout_index_t group = 0;
            തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം_Resolve_Group(info, pc->വ്യഞ്ജകം, true, &group, ശൂന്യ)) {
            അവസ്ഥ PARSER_SUCCESS:
                pc->computed = true;
                pc->മൂല്യം = group - 1;
                നിർത്തുക;
            അവസ്ഥ PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                തിരികെ (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            സ്ഥിരസ്ഥിതി:
                /* Recover from error: ignore */;
            }
        }
        താക്കോൽ->out_of_range_pending_group = false;
        താക്കോൽ->out_of_range_group_സംഖ്യ = pc->മൂല്യം;
    }
    തിരികെ true;
}

സ്ഥിതം bool
update_pending_action_fields(ഘടന xkb_keymap_info *info,
                             xkb_keycode_t keycode, സംഘം xkb_action *act)
{
    തിരഞ്ഞെടുപ്പ് (act->type) {
    അവസ്ഥ ACTION_TYPE_GROUP_SET:
    അവസ്ഥ ACTION_TYPE_GROUP_LATCH:
    അവസ്ഥ ACTION_TYPE_GROUP_LOCK:
        എങ്കിൽ (act->group.flags & ACTION_PENDING_COMPUTATION) {
            ഘടന pending_computation * സ്ഥിരം pc =
                &darray_item(*info->pending_computations, act->group.group);
            എങ്കിൽ (!pc->computed) {
                xkb_layout_index_t group = 0;
                സ്ഥിരം bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം_Resolve_Group(info, pc->വ്യഞ്ജകം, absolute, &group, ശൂന്യ)) {
                അവസ്ഥ PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    തിരികെ false;
                അവസ്ഥ PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    pc->computed = true;
                    എങ്കിൽ (absolute) {
                        pc->മൂല്യം = group - 1;
                    } അല്ലെങ്കിൽ {
                        pc->മൂല്യം = group;
                        എങ്കിൽ (pc->വ്യഞ്ജകം->common.type == STMT_വ്യഞ്ജകം_NEGATE)
                            pc->മൂല്യം = (uint32_t) (-(int32_t) pc->മൂല്യം);
                    }
                }
            }
            act->group.group = (int32_t) pc->മൂല്യം;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        തിരികെ true;
    അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ: {
        എങ്കിൽ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_താക്കോൽ_auto) {
            /* No auto value to set */
            തിരികെ true;
        } അല്ലെങ്കിൽ {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        തിരികെ true;
    }
    സ്ഥിരസ്ഥിതി:
        തിരികെ true;
    }
}

സ്ഥിതം bool
update_pending_led_fields(ഘടന xkb_keymap_info *info, ഘടന xkb_led *led)
{
    എങ്കിൽ (led->pending_groups) {
        ഘടന pending_computation * സ്ഥിരം pc =
            &darray_item(*info->pending_computations, led->groups);
        എങ്കിൽ (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Group_Mask(info, pc->വ്യഞ്ജകം, &mask, ശൂന്യ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                തിരികെ false;
            }
            pc->computed = true;
            pc->മൂല്യം = mask;
        }
        led->pending_groups = false;
        led->groups = pc->മൂല്യം;
    }
    തിരികെ true;
}

/** Indices for the array "groupIndexNames" */
എണ്ണൽ {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
എണ്ണൽ {
    GROUP_MASK_NAME_LAST = 3,
};

സ്ഥിതം ശൂന്യം
update_auto_group_lock_on_modifier_release(ഘടന xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           സംഘം xkb_action * restrict action)
{
    എങ്കിൽ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].അക്കം_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->അക്കം_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        തിരികെ;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
സ്ഥിതം bool
UpdateDerivedKeymapFields(ഘടന xkb_keymap_info *info)
{
    ഘടന xkb_keymap * restrict സ്ഥിരം keymap = &info->keymap;

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
    static_assert(വലുപ്പം(ഘടന xkb_താക്കോൽ_alias) == വലുപ്പം(KeycodeMatch), "");
#അല്ലെങ്കിൽ
    static_assert(വലുപ്പം(ഘടന xkb_താക്കോൽ_alias) == 2 * വലുപ്പം(KeycodeMatch), "");
#endif
    darray_size_t അക്കം_താക്കോൽ_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    വേണ്ടി (xkb_atom_t alias = 0; alias < keymap->അക്കം_താക്കോൽ_names; alias++) {
        സ്ഥിരം KeycodeMatch entry = keymap->താക്കോൽ_names[alias];
        എങ്കിൽ (entry.is_alias && entry.found) {
            എങ്കിൽ (!അക്കം_താക്കോൽ_aliases)
                min_alias = alias;
            max_alias = alias;
            അക്കം_താക്കോൽ_aliases++;
        }
    }
    എങ്കിൽ (അക്കം_താക്കോൽ_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        സ്ഥിരം darray_size_t required_space = വലുപ്പം(ഘടന xkb_താക്കോൽ_alias)
                                           / വലുപ്പം(KeycodeMatch)
                                           * അക്കം_താക്കോൽ_aliases;
        പ്രതിജ്ഞാ(അക്കം_താക്കോൽ_aliases <= keymap->അക്കം_താക്കോൽ_names);
        എങ്കിൽ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_താക്കോൽ_aliases(keymap, min_alias, max_alias, keymap->താക്കോൽ_aliases);
            /* Shrink */
            ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം r =
                realloc(keymap->താക്കോൽ_aliases,
                        അക്കം_താക്കോൽ_aliases * വലുപ്പം(*keymap->താക്കോൽ_aliases));
            എങ്കിൽ (!r)
                തിരികെ false;
            keymap->താക്കോൽ_aliases = r;
        } അല്ലെങ്കിൽ എങ്കിൽ (keymap->അക്കം_താക്കോൽ_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം aliases = (ഘടന xkb_താക്കോൽ_alias *) (
                keymap->താക്കോൽ_names + max_alias + 1 +
                !is_aligned(keymap->താക്കോൽ_names + max_alias + 1, വലുപ്പം(*aliases))
            );
            add_താക്കോൽ_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->താക്കോൽ_aliases, aliases,
                   അക്കം_താക്കോൽ_aliases * വലുപ്പം(*keymap->താക്കോൽ_aliases));
            /* Shrink */
            ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം r =
                realloc(keymap->താക്കോൽ_aliases,
                        അക്കം_താക്കോൽ_aliases * വലുപ്പം(*keymap->താക്കോൽ_aliases));
            എങ്കിൽ (!r)
                തിരികെ false;
            keymap->താക്കോൽ_aliases = r;
        } അല്ലെങ്കിൽ {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം aliases = calloc(അക്കം_താക്കോൽ_aliases,
                                                          വലുപ്പം(*aliases));
            എങ്കിൽ (!aliases)
                തിരികെ false;
            add_താക്കോൽ_aliases(keymap, min_alias, max_alias, aliases);
            സ്വതന്ത്രം(keymap->താക്കോൽ_names);
            keymap->താക്കോൽ_aliases = aliases;
        }
    }
    keymap->അക്കം_താക്കോൽ_aliases = അക്കം_താക്കോൽ_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ഘടന xkb_താക്കോൽ *താക്കോൽ;
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap)
        keymap->അക്കം_groups = MAX(keymap->അക്കം_groups, താക്കോൽ->അക്കം_groups);

    സ്ഥിരം bool pending_computations = !darray_empty(*info->pending_computations);

    എങ്കിൽ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        സ്ഥിരം xkb_layout_index_t അക്കം_groups
            = (keymap->അക്കം_groups)
            ? keymap->അക്കം_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .മൂല്യം = അക്കം_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .മൂല്യം = UINT32_C(1) << (അക്കം_groups - 1)
            };

        /* Update interpret fields with pending computations */
        വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_sym_interprets; i++) {
            ഘടന xkb_sym_interpret * restrict സ്ഥിരം interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                എങ്കിൽ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    തിരികെ false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        എങ്കിൽ (!Apply_Interps_To_താക്കോൽ(keymap, താക്കോൽ))
            തിരികെ false;
        CheckMultipleActionsCategories(keymap, താക്കോൽ);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    ഘടന xkb_mod *mod;
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            എങ്കിൽ (താക്കോൽ->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= താക്കോൽ->modmap;

    എങ്കിൽ (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            സ്ഥിരം xkb_mod_mask_t mask = UINT32_C(1) << idx;
            എങ്കിൽ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    പ്രതിജ്ഞാ(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        വേണ്ടി (darray_size_t j = 0; j < keymap->types[i].അക്കം_entries; j++) {
            എങ്കിൽ (has_unbound_vmods(keymap,
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
                തുടരുക;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        എങ്കിൽ (!update_pending_താക്കോൽ_fields(info, താക്കോൽ))
            തിരികെ false;
        വേണ്ടി (xkb_layout_index_t i = 0; i < താക്കോൽ->അക്കം_groups; i++) {
            വേണ്ടി (xkb_level_index_t j = 0; j < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, i); j++) {
                ഘടന xkb_level * സ്ഥിരം level = &താക്കോൽ->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, താക്കോൽ->modmap);
                    എങ്കിൽ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_താക്കോൽ) &&
                        !update_pending_action_fields(info, താക്കോൽ->keycode, act))
                    {
                        തിരികെ false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        താക്കോൽ->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ഘടന xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        എങ്കിൽ (pending_computations &&
            !update_pending_led_fields(info, led))
            തിരികെ false;
    }

    തിരികെ true;
}

തരനിർവചനം bool (*compile_file_fn)(XkbFile *file, ഘടന xkb_keymap_info *info);

സ്ഥിതം സ്ഥിരം compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_താക്കോൽ_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

സ്ഥിതം ശൂന്യം
pending_computations_array_free(pending_computation_array *p)
{
    ഘടന pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->വ്യഞ്ജകം);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ഘടന xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { ശൂന്യ };
    എണ്ണൽ xkb_file_type type;
    ഘടന xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    വേണ്ടി (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        എങ്കിൽ (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            എങ്കിൽ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } അല്ലെങ്കിൽ {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            തുടരുക;
        }

        എങ്കിൽ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            തുടരുക;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ഘടന xkb_keymap_info info = {
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
                    keymap->അക്കം_groups ? GROUP_LAST_INDEX_NAME : ശൂന്യ,
                    keymap->അക്കം_groups /* 1-indexed */
                },
                { ശൂന്യ, 0 }
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
                    keymap->അക്കം_groups ? GROUP_LAST_INDEX_NAME : ശൂന്യ,
                    /* Be extra cautious */
                    keymap->അക്കം_groups && keymap->അക്കം_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->അക്കം_groups - 1))
                        : 0
                },
                { ശൂന്യ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    വേണ്ടി (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        എങ്കിൽ (files[type] == ശൂന്യ) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } അല്ലെങ്കിൽ {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        സ്ഥിരം bool ok = compile_file_fns[type](files[type], &info);
        എങ്കിൽ (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            തിരികെ false;
        }
    }

    സ്ഥിരം bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    തിരികെ ok;
}
