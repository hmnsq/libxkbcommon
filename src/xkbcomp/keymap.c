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

#समावेश "config.h"

#समावेश <string.h>
#समावेश <stdbool.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "ast-build.h"
#समावेश "darray.h"
#समावेश "expr.h"
#समावेश "keymap.h"
#समावेश "text.h"
#समावेश "utils.h"
#समावेश "xkbcomp-priv.h"

अटल bool
has_unbound_vmods(काष्ठा xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_सूचकांक_t k;
    काष्ठा xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        अगर ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            वापस true;
    वापस false;
}

अटल अंतरभूत व्योम
ComputeEffectiveMask(काष्ठा xkb_keymap *keymap, काष्ठा xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    स्थिर xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.अंक_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

अटल व्योम
UpdateActionMods(काष्ठा xkb_keymap *keymap, जोड़ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    चयन (act->type) {
    हाल ACTION_TYPE_MOD_SET:
    हाल ACTION_TYPE_MOD_LATCH:
    हाल ACTION_TYPE_MOD_LOCK:
        अगर (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        अवरोध;
    शेष:
        अवरोध;
    }
}

अटल स्थिर काष्ठा xkb_sym_interpret default_interpret = {
    .sym = XKB_कुंजी_No_प्रतीक /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_कुंजी_REPEAT,
    .virtual_mod = (xkb_mod_सूचकांक_t) DEFAULT_INTERPRET_VMOD,
    .अंक_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

प्रकार darray(स्थिर काष्ठा xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
अटल bool
Find_Interp_For_कुंजी(काष्ठा xkb_keymap *keymap, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                 xkb_layout_सूचकांक_t group, xkb_level_सूचकांक_t level,
                 xkb_sym_interprets *interprets)
{
    स्थिर xkb_keysym_t *syms;
    पूर्णांक अंक_syms;

    अंक_syms = xkb_keymap_कुंजी_get_syms_by_level(keymap, कुंजी->keycode, group,
                                                level, &syms);
    अगर (अंक_syms <= 0)
        वापस false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    क्रम (पूर्णांक s = 0; s < अंक_syms; s++) {
        bool found = false;
        क्रम (darray_आकार_t i = 0; i < keymap->अंक_sym_interprets; i++) {
            काष्ठा xkb_sym_interpret * स्थिर interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            अगर (interp->sym != syms[s] && interp->sym != XKB_कुंजी_No_प्रतीक)
                जारी;

            अगर (interp->level_one_only && level != 0)
                mods = 0;
            अन्यथा
                mods = कुंजी->modmap;

            चयन (interp->match) {
            हाल MATCH_NONE:
                found = !(interp->mods & mods);
                अवरोध;
            हाल MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                अवरोध;
            हाल MATCH_ANY:
                found = (interp->mods & mods);
                अवरोध;
            हाल MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                अवरोध;
            हाल MATCH_EXACTLY:
                found = (interp->mods == mods);
                अवरोध;
            }

            अगर (found && i > 0 && interp->sym == XKB_कुंजी_No_प्रतीक) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                काष्ठा xkb_sym_interpret स्थिर **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    अगर (*previous_interp == interp) {
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
                        जाओ not_found;
                    }
                }
            }
            अगर (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                अवरोध;
            }
        }
        अगर (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    वापस true;
}

अटल bool
Apply_Interps_To_कुंजी(काष्ठा xkb_keymap *keymap, काष्ठा xkb_कुंजी *कुंजी)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_सूचकांक_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(जोड़ xkb_action) actions = darray_new();

    क्रम (xkb_layout_सूचकांक_t group = 0; group < कुंजी->अंक_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        अगर (कुंजी->groups[group].explicit_actions)
            जारी;

        क्रम (level = 0; level < Xkb_कुंजी_अंक_Levels(कुंजी, group); level++) {
            निश्चित(कुंजी->groups[group].levels[level].अंक_actions == 0);

            स्थिर काष्ठा xkb_sym_interpret **interp_iter;
            स्थिर काष्ठा xkb_sym_interpret *interp;
            माप_प्रकार k;
            darray_resize(interprets, 0);

            स्थिर bool found = Find_Interp_For_कुंजी(keymap, कुंजी, group, level, &interprets);
            अगर (!found)
                जारी;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                अगर (group == 0 && level == 0 &&
                    !(कुंजी->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    कुंजी->repeats = true;
                }

                अगर ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    अगर (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                चयन (interp->अंक_actions) {
                हाल 0:
                    अवरोध;
                हाल 1:
                    darray_append(actions, interp->a.action);
                    अवरोध;
                शेष:
                    darray_append_वस्तुएँ(actions, interp->a.actions,
                                        interp->अंक_actions);
                }
            }

            /* Copy the actions */
            अगर (unlikely(darray_आकार(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         कुंजी_Name_Text(keymap->ctx, कुंजी->name),
                         MAX_ACTIONS_PER_LEVEL, darray_आकार(actions));
                कुंजी->groups[group].levels[level].अंक_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } अन्यथा {
                कुंजी->groups[group].levels[level].अंक_actions =
                    (xkb_action_गिनती_t) darray_आकार(actions);
            }
            चयन (darray_आकार(actions)) {
                हाल 0:
                    कुंजी->groups[group].levels[level].a.action =
                        (जोड़ xkb_action) { .type = ACTION_TYPE_NONE };
                    अवरोध;
                हाल 1:
                    कुंजी->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    अवरोध;
                शेष:
                    कुंजी->groups[group].levels[level].a.actions =
                        memdup(darray_वस्तुएँ(actions),
                               कुंजी->groups[group].levels[level].अंक_actions,
                               माप(*darray_वस्तुएँ(actions)));
                    अगर (!कुंजी->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        वापस false;
                    }
            }

            अगर (!darray_empty(actions))
                कुंजी->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        अगर (कुंजी->groups[group].implicit_actions)
            कुंजी->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    अगर (!(कुंजी->explicit & EXPLICIT_VMODMAP))
        कुंजी->vmodmap = vmodmap;

    वापस true;
}

अटल अंतरभूत bool
is_mod_action(जोड़ xkb_action *action)
{
    वापस action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

अटल अंतरभूत bool
is_group_action(जोड़ xkb_action *action)
{
    वापस action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
अटल व्योम
CheckMultipleActionsCategories(काष्ठा xkb_keymap *keymap, काष्ठा xkb_कुंजी *कुंजी)
{
    क्रम (xkb_layout_सूचकांक_t g = 0; g < कुंजी->अंक_groups; g++) {
        क्रम (xkb_level_सूचकांक_t l = 0; l < Xkb_कुंजी_अंक_Levels(कुंजी, g); l++) {
            काष्ठा xkb_level *level = &कुंजी->groups[g].levels[l];
            अगर (level->अंक_actions <= 1)
                जारी;
            क्रम (xkb_action_गिनती_t i = 0; i < level->अंक_actions; i++) {
                जोड़ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                अगर (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_कुंजी))
                    जारी;
                क्रम (xkb_action_गिनती_t j = i + 1; j < level->अंक_actions; j++) {
                    जोड़ xkb_action *action2 = &level->a.actions[j];
                    अगर (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        स्थिर अक्षर * स्थिर type = (mod_action)
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

अटल व्योम
add_कुंजी_aliases(काष्ठा xkb_keymap *keymap, darray_आकार_t min, darray_आकार_t max,
                काष्ठा xkb_कुंजी_alias *aliases)
{
    क्रम (darray_आकार_t alias = min; alias <= max; alias++) {
        स्थिर KeycodeMatch entry = keymap->कुंजी_names[alias];
        अगर (entry.is_alias && entry.found) {
            *aliases = (काष्ठा xkb_कुंजी_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

अटल bool
update_pending_कुंजी_fields(काष्ठा xkb_keymap_info *info, काष्ठा xkb_कुंजी *कुंजी)
{
    अगर (कुंजी->out_of_range_pending_group) {
        काष्ठा pending_computation * स्थिर pc = &darray_item(
            *info->pending_computations, कुंजी->out_of_range_group_संख्या
        );
        अगर (!pc->computed) {
            xkb_layout_सूचकांक_t group = 0;
            चयन (व्यंजक_Resolve_Group(info, pc->व्यंजक, true, &group, शून्य)) {
            हाल PARSER_SUCCESS:
                pc->computed = true;
                pc->मूल्य = group - 1;
                अवरोध;
            हाल PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid key redirect group index\n");
                वापस (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            शेष:
                /* Recover from error: ignore */;
            }
        }
        कुंजी->out_of_range_pending_group = false;
        कुंजी->out_of_range_group_संख्या = pc->मूल्य;
    }
    वापस true;
}

अटल bool
update_pending_action_fields(काष्ठा xkb_keymap_info *info,
                             xkb_keycode_t keycode, जोड़ xkb_action *act)
{
    चयन (act->type) {
    हाल ACTION_TYPE_GROUP_SET:
    हाल ACTION_TYPE_GROUP_LATCH:
    हाल ACTION_TYPE_GROUP_LOCK:
        अगर (act->group.flags & ACTION_PENDING_COMPUTATION) {
            काष्ठा pending_computation * स्थिर pc =
                &darray_item(*info->pending_computations, act->group.group);
            अगर (!pc->computed) {
                xkb_layout_सूचकांक_t group = 0;
                स्थिर bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                चयन (व्यंजक_Resolve_Group(info, pc->व्यंजक, absolute, &group, शून्य)) {
                हाल PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                            "Invalid action group index\n");
                    वापस false;
                हाल PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    अवरोध;
                शेष:
                    pc->computed = true;
                    अगर (absolute) {
                        pc->मूल्य = group - 1;
                    } अन्यथा {
                        pc->मूल्य = group;
                        अगर (pc->व्यंजक->common.type == STMT_व्यंजक_NEGATE)
                            pc->मूल्य = (uint32_t) (-(int32_t) pc->मूल्य);
                    }
                }
            }
            act->group.group = (int32_t) pc->मूल्य;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        वापस true;
    हाल ACTION_TYPE_REDIRECT_कुंजी: {
        अगर (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_कुंजी_auto) {
            /* No auto value to set */
            वापस true;
        } अन्यथा {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        वापस true;
    }
    शेष:
        वापस true;
    }
}

अटल bool
update_pending_led_fields(काष्ठा xkb_keymap_info *info, काष्ठा xkb_led *led)
{
    अगर (led->pending_groups) {
        काष्ठा pending_computation * स्थिर pc =
            &darray_item(*info->pending_computations, led->groups);
        अगर (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            अगर (!व्यंजक_Resolve_Group_Mask(info, pc->व्यंजक, &mask, शून्य)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid LED group mask\n");
                वापस false;
            }
            pc->computed = true;
            pc->मूल्य = mask;
        }
        led->pending_groups = false;
        led->groups = pc->मूल्य;
    }
    वापस true;
}

/** Indices for the array "groupIndexNames" */
क्रमागत {
    GROUP_सूचकांक_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
क्रमागत {
    GROUP_MASK_NAME_LAST = 3,
};

अटल व्योम
update_auto_group_lock_on_modifier_release(काष्ठा xkb_level * restrict level0,
                                           xkb_level_सूचकांक_t l,
                                           जोड़ xkb_action * restrict action)
{
    अगर (!(/* Must be level index 1+ */
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
        वापस;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
अटल bool
UpdateDerivedKeymapFields(काष्ठा xkb_keymap_info *info)
{
    काष्ठा xkb_keymap * restrict स्थिर keymap = &info->keymap;

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
#अगर_घोषित _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(माप(काष्ठा xkb_कुंजी_alias) == माप(KeycodeMatch), "");
#अन्यथा
    static_assert(माप(काष्ठा xkb_कुंजी_alias) == 2 * माप(KeycodeMatch), "");
#पूर्ण_अगर
    darray_आकार_t अंक_कुंजी_aliases = 0;
    darray_आकार_t min_alias = 0;
    darray_आकार_t max_alias = 0;
    क्रम (xkb_atom_t alias = 0; alias < keymap->अंक_कुंजी_names; alias++) {
        स्थिर KeycodeMatch entry = keymap->कुंजी_names[alias];
        अगर (entry.is_alias && entry.found) {
            अगर (!अंक_कुंजी_aliases)
                min_alias = alias;
            max_alias = alias;
            अंक_कुंजी_aliases++;
        }
    }
    अगर (अंक_कुंजी_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        स्थिर darray_आकार_t required_space = माप(काष्ठा xkb_कुंजी_alias)
                                           / माप(KeycodeMatch)
                                           * अंक_कुंजी_aliases;
        निश्चित(अंक_कुंजी_aliases <= keymap->अंक_कुंजी_names);
        अगर (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_कुंजी_aliases(keymap, min_alias, max_alias, keymap->कुंजी_aliases);
            /* Shrink */
            काष्ठा xkb_कुंजी_alias * स्थिर r =
                पुनः_स्मृति(keymap->कुंजी_aliases,
                        अंक_कुंजी_aliases * माप(*keymap->कुंजी_aliases));
            अगर (!r)
                वापस false;
            keymap->कुंजी_aliases = r;
        } अन्यथा अगर (keymap->अंक_कुंजी_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            काष्ठा xkb_कुंजी_alias * स्थिर aliases = (काष्ठा xkb_कुंजी_alias *) (
                keymap->कुंजी_names + max_alias + 1 +
                !is_aligned(keymap->कुंजी_names + max_alias + 1, माप(*aliases))
            );
            add_कुंजी_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            स_नकल(keymap->कुंजी_aliases, aliases,
                   अंक_कुंजी_aliases * माप(*keymap->कुंजी_aliases));
            /* Shrink */
            काष्ठा xkb_कुंजी_alias * स्थिर r =
                पुनः_स्मृति(keymap->कुंजी_aliases,
                        अंक_कुंजी_aliases * माप(*keymap->कुंजी_aliases));
            अगर (!r)
                वापस false;
            keymap->कुंजी_aliases = r;
        } अन्यथा {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            काष्ठा xkb_कुंजी_alias * स्थिर aliases = सुस्मृति(अंक_कुंजी_aliases,
                                                          माप(*aliases));
            अगर (!aliases)
                वापस false;
            add_कुंजी_aliases(keymap, min_alias, max_alias, aliases);
            मुक्त(keymap->कुंजी_names);
            keymap->कुंजी_aliases = aliases;
        }
    }
    keymap->अंक_कुंजी_aliases = अंक_कुंजी_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    काष्ठा xkb_कुंजी *कुंजी;
    xkb_कुंजियाँ_foreach(कुंजी, keymap)
        keymap->अंक_groups = MAX(keymap->अंक_groups, कुंजी->अंक_groups);

    स्थिर bool pending_computations = !darray_empty(*info->pending_computations);

    अगर (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        स्थिर xkb_layout_सूचकांक_t अंक_groups
            = (keymap->अंक_groups)
            ? keymap->अंक_groups
            : 1; /* default to the “first” layout */
        info->lookup.group_सूचकांक_Names[GROUP_सूचकांक_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_सूचकांक_NAME,
                .मूल्य = अंक_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_सूचकांक_NAME,
                .मूल्य = UINT32_C(1) << (अंक_groups - 1)
            };

        /* Update interpret fields with pending computations */
        क्रम (darray_आकार_t i = 0; i < keymap->अंक_sym_interprets; i++) {
            काष्ठा xkb_sym_interpret * restrict स्थिर interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                अगर (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    वापस false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        अगर (!Apply_Interps_To_कुंजी(keymap, कुंजी))
            वापस false;
        CheckMultipleActionsCategories(keymap, कुंजी);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_सूचकांक_t idx;
    काष्ठा xkb_mod *mod;
    xkb_कुंजियाँ_foreach(कुंजी, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            अगर (कुंजी->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= कुंजी->modmap;

    अगर (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            स्थिर xkb_mod_mask_t mask = UINT32_C(1) << idx;
            अगर (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    निश्चित(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    क्रम (darray_आकार_t i = 0; i < keymap->अंक_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        क्रम (darray_आकार_t j = 0; j < keymap->types[i].अंक_entries; j++) {
            अगर (has_unbound_vmods(keymap,
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
                जारी;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        अगर (!update_pending_कुंजी_fields(info, कुंजी))
            वापस false;
        क्रम (xkb_layout_सूचकांक_t i = 0; i < कुंजी->अंक_groups; i++) {
            क्रम (xkb_level_सूचकांक_t j = 0; j < Xkb_कुंजी_अंक_Levels(कुंजी, i); j++) {
                काष्ठा xkb_level * स्थिर level = &कुंजी->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, कुंजी->modmap);
                    अगर ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_कुंजी) &&
                        !update_pending_action_fields(info, कुंजी->keycode, act))
                    {
                        वापस false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        कुंजी->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    काष्ठा xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        अगर (pending_computations &&
            !update_pending_led_fields(info, led))
            वापस false;
    }

    वापस true;
}

प्रकार bool (*compile_file_fn)(XkbFile *file, काष्ठा xkb_keymap_info *info);

अटल स्थिर compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_कुंजी_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

अटल व्योम
pending_computations_array_free(pending_computation_array *p)
{
    काष्ठा pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->व्यंजक);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, काष्ठा xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { शून्य };
    क्रमागत xkb_file_type type;
    काष्ठा xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    क्रम (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.अग्र) {
        अगर (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            अगर (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } अन्यथा {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            जारी;
        }

        अगर (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            जारी;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    काष्ठा xkb_keymap_info info = {
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
            .group_सूचकांक_Names = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [GROUP_सूचकांक_NAME_LAST] = {
                    keymap->अंक_groups ? GROUP_LAST_सूचकांक_NAME : शून्य,
                    keymap->अंक_groups /* 1-indexed */
                },
                { शून्य, 0 }
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
                    keymap->अंक_groups ? GROUP_LAST_सूचकांक_NAME : शून्य,
                    /* Be extra cautious */
                    keymap->अंक_groups && keymap->अंक_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->अंक_groups - 1))
                        : 0
                },
                { शून्य, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    क्रम (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        अगर (files[type] == शून्य) {
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
        स्थिर bool ok = compile_file_fns[type](files[type], &info);
        अगर (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            वापस false;
        }
    }

    स्थिर bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    वापस ok;
}
