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

#अन्तर्भाव "config.h"

#अन्तर्भाव <string.h>
#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "ast-build.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp-priv.h"

स्थिर bool
has_unbound_vmods(संरचना xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    संरचना xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        यदि ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            प्रत्यागम true;
    प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य
ComputeEffectiveMask(संरचना xkb_keymap *keymap, संरचना xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ध्रुव xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.अंक_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

स्थिर शून्य
UpdateActionMods(संरचना xkb_keymap *keymap, सङ्घ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    विकल्प (act->type) {
    पक्ष ACTION_TYPE_MOD_SET:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_LOCK:
        यदि (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        भङ्ग;
    स्वभाव:
        भङ्ग;
    }
}

स्थिर ध्रुव संरचना xkb_sym_interpret default_interpret = {
    .sym = XKB_कुंजी_No_प्रतीक /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_कुंजी_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .अंक_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

प्रकारनिर्देश darray(ध्रुव संरचना xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
स्थिर bool
Find_Interp_For_कुंजी(संरचना xkb_keymap *keymap, ध्रुव संरचना xkb_कुंजी *कुंजी,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    ध्रुव xkb_keysym_t *syms;
    पूर्णाङ्क अंक_syms;

    अंक_syms = xkb_keymap_कुंजी_get_syms_by_level(keymap, कुंजी->keycode, group,
                                                level, &syms);
    यदि (अंक_syms <= 0)
        प्रत्यागम false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    प्रति (पूर्णाङ्क s = 0; s < अंक_syms; s++) {
        bool found = false;
        प्रति (darray_size_t i = 0; i < keymap->अंक_sym_interprets; i++) {
            संरचना xkb_sym_interpret * ध्रुव interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            यदि (interp->sym != syms[s] && interp->sym != XKB_कुंजी_No_प्रतीक)
                अनुवर्त;

            यदि (interp->level_one_only && level != 0)
                mods = 0;
            अन्यथा
                mods = कुंजी->modmap;

            विकल्प (interp->match) {
            पक्ष MATCH_NONE:
                found = !(interp->mods & mods);
                भङ्ग;
            पक्ष MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                भङ्ग;
            पक्ष MATCH_ANY:
                found = (interp->mods & mods);
                भङ्ग;
            पक्ष MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                भङ्ग;
            पक्ष MATCH_EXACTLY:
                found = (interp->mods == mods);
                भङ्ग;
            }

            यदि (found && i > 0 && interp->sym == XKB_कुंजी_No_प्रतीक) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                संरचना xkb_sym_interpret ध्रुव **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    यदि (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 कुंजी_Name_Text(keymap->ctx, कुंजी->name));
                        गच्छ not_found;
                    }
                }
            }
            यदि (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                भङ्ग;
            }
        }
        यदि (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    प्रत्यागम true;
}

स्थिर bool
Apply_Interps_To_कुंजी(संरचना xkb_keymap *keymap, संरचना xkb_कुंजी *कुंजी)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(सङ्घ xkb_action) actions = darray_new();

    प्रति (xkb_layout_index_t group = 0; group < कुंजी->अंक_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        यदि (कुंजी->groups[group].explicit_actions)
            अनुवर्त;

        प्रति (level = 0; level < Xkb_कुंजी_अंक_Levels(कुंजी, group); level++) {
            प्रतिज्ञा(कुंजी->groups[group].levels[level].अंक_actions == 0);

            ध्रुव संरचना xkb_sym_interpret **interp_iter;
            ध्रुव संरचना xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            ध्रुव bool found = Find_Interp_For_कुंजी(keymap, कुंजी, group, level, &interprets);
            यदि (!found)
                अनुवर्त;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                यदि (group == 0 && level == 0 &&
                    !(कुंजी->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    कुंजी->repeats = true;
                }

                यदि ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    यदि (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                विकल्प (interp->अंक_actions) {
                पक्ष 0:
                    भङ्ग;
                पक्ष 1:
                    darray_append(actions, interp->a.action);
                    भङ्ग;
                स्वभाव:
                    darray_append_वस्तुएँ(actions, interp->a.actions,
                                        interp->अंक_actions);
                }
            }

            /* Copy the actions */
            यदि (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         कुंजी_Name_Text(keymap->ctx, कुंजी->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                कुंजी->groups[group].levels[level].अंक_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } अन्यथा {
                कुंजी->groups[group].levels[level].अंक_actions =
                    (xkb_action_गिनती_t) darray_size(actions);
            }
            विकल्प (darray_size(actions)) {
                पक्ष 0:
                    कुंजी->groups[group].levels[level].a.action =
                        (सङ्घ xkb_action) { .type = ACTION_TYPE_NONE };
                    भङ्ग;
                पक्ष 1:
                    कुंजी->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    भङ्ग;
                स्वभाव:
                    कुंजी->groups[group].levels[level].a.actions =
                        memdup(darray_वस्तुएँ(actions),
                               कुंजी->groups[group].levels[level].अंक_actions,
                               परिमाण(*darray_वस्तुएँ(actions)));
                    यदि (!कुंजी->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        प्रत्यागम false;
                    }
            }

            यदि (!darray_empty(actions))
                कुंजी->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        यदि (कुंजी->groups[group].implicit_actions)
            कुंजी->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    यदि (!(कुंजी->explicit & EXPLICIT_VMODMAP))
        कुंजी->vmodmap = vmodmap;

    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
is_mod_action(सङ्घ xkb_action *action)
{
    प्रत्यागम action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

स्थिर अन्तःस्थ bool
is_group_action(सङ्घ xkb_action *action)
{
    प्रत्यागम action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
स्थिर शून्य
CheckMultipleActionsCategories(संरचना xkb_keymap *keymap, संरचना xkb_कुंजी *कुंजी)
{
    प्रति (xkb_layout_index_t g = 0; g < कुंजी->अंक_groups; g++) {
        प्रति (xkb_level_index_t l = 0; l < Xkb_कुंजी_अंक_Levels(कुंजी, g); l++) {
            संरचना xkb_level *level = &कुंजी->groups[g].levels[l];
            यदि (level->अंक_actions <= 1)
                अनुवर्त;
            प्रति (xkb_action_गिनती_t i = 0; i < level->अंक_actions; i++) {
                सङ्घ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                यदि (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_कुंजी))
                    अनुवर्त;
                प्रति (xkb_action_गिनती_t j = i + 1; j < level->अंक_actions; j++) {
                    सङ्घ xkb_action *action2 = &level->a.actions[j];
                    यदि (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ध्रुव वर्ण * ध्रुव type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, कुंजी_Name_Text(keymap->ctx, कुंजी->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

स्थिर शून्य
add_कुंजी_aliases(संरचना xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                संरचना xkb_कुंजी_alias *aliases)
{
    प्रति (darray_size_t alias = min; alias <= max; alias++) {
        ध्रुव KeycodeMatch entry = keymap->कुंजी_names[alias];
        यदि (entry.is_alias && entry.found) {
            *aliases = (संरचना xkb_कुंजी_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

स्थिर bool
update_pending_कुंजी_fields(संरचना xkb_keymap_info *info, संरचना xkb_कुंजी *कुंजी)
{
    यदि (कुंजी->out_of_range_pending_group) {
        संरचना pending_computation * ध्रुव pc = &darray_item(
            *info->pending_computations, कुंजी->out_of_range_group_संख्या
        );
        यदि (!pc->computed) {
            xkb_layout_index_t group = 0;
            विकल्प (व्यंजक_Resolve_Group(info, pc->व्यंजक, true, &group, रिक्त)) {
            पक्ष PARSER_SUCCESS:
                pc->computed = true;
                pc->मूल्य = group - 1;
                भङ्ग;
            पक्ष PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                प्रत्यागम (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            स्वभाव:
                /* Recover from error: ignore */;
            }
        }
        कुंजी->out_of_range_pending_group = false;
        कुंजी->out_of_range_group_संख्या = pc->मूल्य;
    }
    प्रत्यागम true;
}

स्थिर bool
update_pending_action_fields(संरचना xkb_keymap_info *info,
                             xkb_keycode_t keycode, सङ्घ xkb_action *act)
{
    विकल्प (act->type) {
    पक्ष ACTION_TYPE_GROUP_SET:
    पक्ष ACTION_TYPE_GROUP_LATCH:
    पक्ष ACTION_TYPE_GROUP_LOCK:
        यदि (act->group.flags & ACTION_PENDING_COMPUTATION) {
            संरचना pending_computation * ध्रुव pc =
                &darray_item(*info->pending_computations, act->group.group);
            यदि (!pc->computed) {
                xkb_layout_index_t group = 0;
                ध्रुव bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                विकल्प (व्यंजक_Resolve_Group(info, pc->व्यंजक, absolute, &group, रिक्त)) {
                पक्ष PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    प्रत्यागम false;
                पक्ष PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    भङ्ग;
                स्वभाव:
                    pc->computed = true;
                    यदि (absolute) {
                        pc->मूल्य = group - 1;
                    } अन्यथा {
                        pc->मूल्य = group;
                        यदि (pc->व्यंजक->common.type == STMT_व्यंजक_NEGATE)
                            pc->मूल्य = (uint32_t) (-(int32_t) pc->मूल्य);
                    }
                }
            }
            act->group.group = (int32_t) pc->मूल्य;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        प्रत्यागम true;
    पक्ष ACTION_TYPE_REDIRECT_कुंजी: {
        यदि (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_कुंजी_auto) {
            /* No auto value to set */
            प्रत्यागम true;
        } अन्यथा {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        प्रत्यागम true;
    }
    स्वभाव:
        प्रत्यागम true;
    }
}

स्थिर bool
update_pending_led_fields(संरचना xkb_keymap_info *info, संरचना xkb_led *led)
{
    यदि (led->pending_groups) {
        संरचना pending_computation * ध्रुव pc =
            &darray_item(*info->pending_computations, led->groups);
        यदि (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            यदि (!व्यंजक_Resolve_Group_Mask(info, pc->व्यंजक, &mask, रिक्त)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                प्रत्यागम false;
            }
            pc->computed = true;
            pc->मूल्य = mask;
        }
        led->pending_groups = false;
        led->groups = pc->मूल्य;
    }
    प्रत्यागम true;
}

/** Indices for the array "groupIndexNames" */
गणना {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
गणना {
    GROUP_MASK_NAME_LAST = 3,
};

स्थिर शून्य
update_auto_group_lock_on_modifier_release(संरचना xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           सङ्घ xkb_action * restrict action)
{
    यदि (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].अंक_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->अंक_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        प्रत्यागम;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
स्थिर bool
UpdateDerivedKeymapFields(संरचना xkb_keymap_info *info)
{
    संरचना xkb_keymap * restrict ध्रुव keymap = &info->keymap;

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
    static_assert(परिमाण(संरचना xkb_कुंजी_alias) == परिमाण(KeycodeMatch), "");
#अन्यथा
    static_assert(परिमाण(संरचना xkb_कुंजी_alias) == 2 * परिमाण(KeycodeMatch), "");
#endif
    darray_size_t अंक_कुंजी_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    प्रति (xkb_atom_t alias = 0; alias < keymap->अंक_कुंजी_names; alias++) {
        ध्रुव KeycodeMatch entry = keymap->कुंजी_names[alias];
        यदि (entry.is_alias && entry.found) {
            यदि (!अंक_कुंजी_aliases)
                min_alias = alias;
            max_alias = alias;
            अंक_कुंजी_aliases++;
        }
    }
    यदि (अंक_कुंजी_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ध्रुव darray_size_t required_space = परिमाण(संरचना xkb_कुंजी_alias)
                                           / परिमाण(KeycodeMatch)
                                           * अंक_कुंजी_aliases;
        प्रतिज्ञा(अंक_कुंजी_aliases <= keymap->अंक_कुंजी_names);
        यदि (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_कुंजी_aliases(keymap, min_alias, max_alias, keymap->कुंजी_aliases);
            /* Shrink */
            संरचना xkb_कुंजी_alias * ध्रुव r =
                realloc(keymap->कुंजी_aliases,
                        अंक_कुंजी_aliases * परिमाण(*keymap->कुंजी_aliases));
            यदि (!r)
                प्रत्यागम false;
            keymap->कुंजी_aliases = r;
        } अन्यथा यदि (keymap->अंक_कुंजी_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            संरचना xkb_कुंजी_alias * ध्रुव aliases = (संरचना xkb_कुंजी_alias *) (
                keymap->कुंजी_names + max_alias + 1 +
                !is_aligned(keymap->कुंजी_names + max_alias + 1, परिमाण(*aliases))
            );
            add_कुंजी_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->कुंजी_aliases, aliases,
                   अंक_कुंजी_aliases * परिमाण(*keymap->कुंजी_aliases));
            /* Shrink */
            संरचना xkb_कुंजी_alias * ध्रुव r =
                realloc(keymap->कुंजी_aliases,
                        अंक_कुंजी_aliases * परिमाण(*keymap->कुंजी_aliases));
            यदि (!r)
                प्रत्यागम false;
            keymap->कुंजी_aliases = r;
        } अन्यथा {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            संरचना xkb_कुंजी_alias * ध्रुव aliases = calloc(अंक_कुंजी_aliases,
                                                          परिमाण(*aliases));
            यदि (!aliases)
                प्रत्यागम false;
            add_कुंजी_aliases(keymap, min_alias, max_alias, aliases);
            मुक्त(keymap->कुंजी_names);
            keymap->कुंजी_aliases = aliases;
        }
    }
    keymap->अंक_कुंजी_aliases = अंक_कुंजी_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    संरचना xkb_कुंजी *कुंजी;
    xkb_कुंजियाँ_foreach(कुंजी, keymap)
        keymap->अंक_groups = MAX(keymap->अंक_groups, कुंजी->अंक_groups);

    ध्रुव bool pending_computations = !darray_empty(*info->pending_computations);

    यदि (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ध्रुव xkb_layout_index_t अंक_groups
            = (keymap->अंक_groups)
            ? keymap->अंक_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .मूल्य = अंक_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .मूल्य = UINT32_C(1) << (अंक_groups - 1)
            };

        /* Update interpret fields with pending computations */
        प्रति (darray_size_t i = 0; i < keymap->अंक_sym_interprets; i++) {
            संरचना xkb_sym_interpret * restrict ध्रुव interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                यदि (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    प्रत्यागम false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        यदि (!Apply_Interps_To_कुंजी(keymap, कुंजी))
            प्रत्यागम false;
        CheckMultipleActionsCategories(keymap, कुंजी);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    संरचना xkb_mod *mod;
    xkb_कुंजियाँ_foreach(कुंजी, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            यदि (कुंजी->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= कुंजी->modmap;

    यदि (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            ध्रुव xkb_mod_mask_t mask = UINT32_C(1) << idx;
            यदि (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    प्रतिज्ञा(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    प्रति (darray_size_t i = 0; i < keymap->अंक_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        प्रति (darray_size_t j = 0; j < keymap->types[i].अंक_entries; j++) {
            यदि (has_unbound_vmods(keymap,
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
                अनुवर्त;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        यदि (!update_pending_कुंजी_fields(info, कुंजी))
            प्रत्यागम false;
        प्रति (xkb_layout_index_t i = 0; i < कुंजी->अंक_groups; i++) {
            प्रति (xkb_level_index_t j = 0; j < Xkb_कुंजी_अंक_Levels(कुंजी, i); j++) {
                संरचना xkb_level * ध्रुव level = &कुंजी->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, कुंजी->modmap);
                    यदि ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_कुंजी) &&
                        !update_pending_action_fields(info, कुंजी->keycode, act))
                    {
                        प्रत्यागम false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        कुंजी->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    संरचना xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        यदि (pending_computations &&
            !update_pending_led_fields(info, led))
            प्रत्यागम false;
    }

    प्रत्यागम true;
}

प्रकारनिर्देश bool (*compile_file_fn)(XkbFile *file, संरचना xkb_keymap_info *info);

स्थिर ध्रुव compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_कुंजी_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

स्थिर शून्य
pending_computations_array_free(pending_computation_array *p)
{
    संरचना pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->व्यंजक);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, संरचना xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { रिक्त };
    गणना xkb_file_type type;
    संरचना xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    प्रति (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        यदि (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            यदि (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } अन्यथा {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            अनुवर्त;
        }

        यदि (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            अनुवर्त;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    संरचना xkb_keymap_info info = {
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
                    keymap->अंक_groups ? GROUP_LAST_INDEX_NAME : रिक्त,
                    keymap->अंक_groups /* 1-indexed */
                },
                { रिक्त, 0 }
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
                    keymap->अंक_groups ? GROUP_LAST_INDEX_NAME : रिक्त,
                    /* Be extra cautious */
                    keymap->अंक_groups && keymap->अंक_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->अंक_groups - 1))
                        : 0
                },
                { रिक्त, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    प्रति (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        यदि (files[type] == रिक्त) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } अन्यथा {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ध्रुव bool ok = compile_file_fns[type](files[type], &info);
        यदि (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            प्रत्यागम false;
        }
    }

    ध्रुव bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    प्रत्यागम ok;
}
