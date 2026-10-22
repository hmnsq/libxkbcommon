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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "ast-build.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcomp-priv.h"

நிலையான bool
has_unbound_vmods(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    கட்டமைப்பு xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        எனில் ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            திரும்பு true;
    திரும்பு false;
}

நிலையான உள்வரி வெறுமை
ComputeEffectiveMask(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    மாறிலி xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.இலக்கம்_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

நிலையான வெறுமை
UpdateActionMods(கட்டமைப்பு xkb_keymap *keymap, ஒன்றிப்பு xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    தேர்வு (act->type) {
    நிலை ACTION_TYPE_MOD_SET:
    நிலை ACTION_TYPE_MOD_LATCH:
    நிலை ACTION_TYPE_MOD_LOCK:
        எனில் (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        நிறுத்து;
    இயல்பு:
        நிறுத்து;
    }
}

நிலையான மாறிலி கட்டமைப்பு xkb_sym_interpret default_interpret = {
    .sym = XKB_சாவி_No_குறியீடு /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_சாவி_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .இலக்கம்_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

வகைவரை darray(மாறிலி கட்டமைப்பு xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
நிலையான bool
Find_Interp_For_சாவி(கட்டமைப்பு xkb_keymap *keymap, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    மாறிலி xkb_keysym_t *syms;
    முழு இலக்கம்_syms;

    இலக்கம்_syms = xkb_keymap_சாவி_get_syms_by_level(keymap, சாவி->keycode, group,
                                                level, &syms);
    எனில் (இலக்கம்_syms <= 0)
        திரும்பு false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    க்கு (முழு s = 0; s < இலக்கம்_syms; s++) {
        bool found = false;
        க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_sym_interprets; i++) {
            கட்டமைப்பு xkb_sym_interpret * மாறிலி interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            எனில் (interp->sym != syms[s] && interp->sym != XKB_சாவி_No_குறியீடு)
                தொடர்;

            எனில் (interp->level_one_only && level != 0)
                mods = 0;
            இல்லையெனில்
                mods = சாவி->modmap;

            தேர்வு (interp->match) {
            நிலை MATCH_NONE:
                found = !(interp->mods & mods);
                நிறுத்து;
            நிலை MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                நிறுத்து;
            நிலை MATCH_ANY:
                found = (interp->mods & mods);
                நிறுத்து;
            நிலை MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                நிறுத்து;
            நிலை MATCH_EXACTLY:
                found = (interp->mods == mods);
                நிறுத்து;
            }

            எனில் (found && i > 0 && interp->sym == XKB_சாவி_No_குறியீடு) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                கட்டமைப்பு xkb_sym_interpret மாறிலி **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    எனில் (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 சாவி_Name_Text(keymap->ctx, சாவி->name));
                        செல் not_found;
                    }
                }
            }
            எனில் (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                நிறுத்து;
            }
        }
        எனில் (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    திரும்பு true;
}

நிலையான bool
Apply_Interps_To_சாவி(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு xkb_சாவி *சாவி)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(ஒன்றிப்பு xkb_action) actions = darray_new();

    க்கு (xkb_layout_index_t group = 0; group < சாவி->இலக்கம்_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        எனில் (சாவி->groups[group].explicit_actions)
            தொடர்;

        க்கு (level = 0; level < Xkb_சாவி_இலக்கம்_Levels(சாவி, group); level++) {
            ப்ரதிஜ்ஞா(சாவி->groups[group].levels[level].இலக்கம்_actions == 0);

            மாறிலி கட்டமைப்பு xkb_sym_interpret **interp_iter;
            மாறிலி கட்டமைப்பு xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            மாறிலி bool found = Find_Interp_For_சாவி(keymap, சாவி, group, level, &interprets);
            எனில் (!found)
                தொடர்;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                எனில் (group == 0 && level == 0 &&
                    !(சாவி->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    சாவி->repeats = true;
                }

                எனில் ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    எனில் (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                தேர்வு (interp->இலக்கம்_actions) {
                நிலை 0:
                    நிறுத்து;
                நிலை 1:
                    darray_append(actions, interp->a.action);
                    நிறுத்து;
                இயல்பு:
                    darray_append_பொருட்கள்(actions, interp->a.actions,
                                        interp->இலக்கம்_actions);
                }
            }

            /* Copy the actions */
            எனில் (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         சாவி_Name_Text(keymap->ctx, சாவி->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                சாவி->groups[group].levels[level].இலக்கம்_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } இல்லையெனில் {
                சாவி->groups[group].levels[level].இலக்கம்_actions =
                    (xkb_action_மொத்தம்_t) darray_size(actions);
            }
            தேர்வு (darray_size(actions)) {
                நிலை 0:
                    சாவி->groups[group].levels[level].a.action =
                        (ஒன்றிப்பு xkb_action) { .type = ACTION_TYPE_NONE };
                    நிறுத்து;
                நிலை 1:
                    சாவி->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    நிறுத்து;
                இயல்பு:
                    சாவி->groups[group].levels[level].a.actions =
                        memdup(darray_பொருட்கள்(actions),
                               சாவி->groups[group].levels[level].இலக்கம்_actions,
                               அளவு(*darray_பொருட்கள்(actions)));
                    எனில் (!சாவி->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        திரும்பு false;
                    }
            }

            எனில் (!darray_empty(actions))
                சாவி->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        எனில் (சாவி->groups[group].implicit_actions)
            சாவி->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    எனில் (!(சாவி->explicit & EXPLICIT_VMODMAP))
        சாவி->vmodmap = vmodmap;

    திரும்பு true;
}

நிலையான உள்வரி bool
is_mod_action(ஒன்றிப்பு xkb_action *action)
{
    திரும்பு action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

நிலையான உள்வரி bool
is_group_action(ஒன்றிப்பு xkb_action *action)
{
    திரும்பு action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
நிலையான வெறுமை
CheckMultipleActionsCategories(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு xkb_சாவி *சாவி)
{
    க்கு (xkb_layout_index_t g = 0; g < சாவி->இலக்கம்_groups; g++) {
        க்கு (xkb_level_index_t l = 0; l < Xkb_சாவி_இலக்கம்_Levels(சாவி, g); l++) {
            கட்டமைப்பு xkb_level *level = &சாவி->groups[g].levels[l];
            எனில் (level->இலக்கம்_actions <= 1)
                தொடர்;
            க்கு (xkb_action_மொத்தம்_t i = 0; i < level->இலக்கம்_actions; i++) {
                ஒன்றிப்பு xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                எனில் (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_சாவி))
                    தொடர்;
                க்கு (xkb_action_மொத்தம்_t j = i + 1; j < level->இலக்கம்_actions; j++) {
                    ஒன்றிப்பு xkb_action *action2 = &level->a.actions[j];
                    எனில் (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        மாறிலி எழுத்து * மாறிலி type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, சாவி_Name_Text(keymap->ctx, சாவி->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

நிலையான வெறுமை
add_சாவி_aliases(கட்டமைப்பு xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                கட்டமைப்பு xkb_சாவி_alias *aliases)
{
    க்கு (darray_size_t alias = min; alias <= max; alias++) {
        மாறிலி KeycodeMatch entry = keymap->சாவி_names[alias];
        எனில் (entry.is_alias && entry.found) {
            *aliases = (கட்டமைப்பு xkb_சாவி_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

நிலையான bool
update_pending_சாவி_fields(கட்டமைப்பு xkb_keymap_info *info, கட்டமைப்பு xkb_சாவி *சாவி)
{
    எனில் (சாவி->out_of_range_pending_group) {
        கட்டமைப்பு pending_computation * மாறிலி pc = &darray_item(
            *info->pending_computations, சாவி->out_of_range_group_எண்
        );
        எனில் (!pc->computed) {
            xkb_layout_index_t group = 0;
            தேர்வு (கோவை_Resolve_Group(info, pc->கோவை, true, &group, வெற்று)) {
            நிலை PARSER_SUCCESS:
                pc->computed = true;
                pc->மதிப்பு = group - 1;
                நிறுத்து;
            நிலை PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                திரும்பு (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            இயல்பு:
                /* Recover from error: ignore */;
            }
        }
        சாவி->out_of_range_pending_group = false;
        சாவி->out_of_range_group_எண் = pc->மதிப்பு;
    }
    திரும்பு true;
}

நிலையான bool
update_pending_action_fields(கட்டமைப்பு xkb_keymap_info *info,
                             xkb_keycode_t keycode, ஒன்றிப்பு xkb_action *act)
{
    தேர்வு (act->type) {
    நிலை ACTION_TYPE_GROUP_SET:
    நிலை ACTION_TYPE_GROUP_LATCH:
    நிலை ACTION_TYPE_GROUP_LOCK:
        எனில் (act->group.flags & ACTION_PENDING_COMPUTATION) {
            கட்டமைப்பு pending_computation * மாறிலி pc =
                &darray_item(*info->pending_computations, act->group.group);
            எனில் (!pc->computed) {
                xkb_layout_index_t group = 0;
                மாறிலி bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                தேர்வு (கோவை_Resolve_Group(info, pc->கோவை, absolute, &group, வெற்று)) {
                நிலை PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    திரும்பு false;
                நிலை PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    நிறுத்து;
                இயல்பு:
                    pc->computed = true;
                    எனில் (absolute) {
                        pc->மதிப்பு = group - 1;
                    } இல்லையெனில் {
                        pc->மதிப்பு = group;
                        எனில் (pc->கோவை->common.type == STMT_கோவை_NEGATE)
                            pc->மதிப்பு = (uint32_t) (-(int32_t) pc->மதிப்பு);
                    }
                }
            }
            act->group.group = (int32_t) pc->மதிப்பு;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        திரும்பு true;
    நிலை ACTION_TYPE_REDIRECT_சாவி: {
        எனில் (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_சாவி_auto) {
            /* No auto value to set */
            திரும்பு true;
        } இல்லையெனில் {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        திரும்பு true;
    }
    இயல்பு:
        திரும்பு true;
    }
}

நிலையான bool
update_pending_led_fields(கட்டமைப்பு xkb_keymap_info *info, கட்டமைப்பு xkb_led *led)
{
    எனில் (led->pending_groups) {
        கட்டமைப்பு pending_computation * மாறிலி pc =
            &darray_item(*info->pending_computations, led->groups);
        எனில் (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            எனில் (!கோவை_Resolve_Group_Mask(info, pc->கோவை, &mask, வெற்று)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                திரும்பு false;
            }
            pc->computed = true;
            pc->மதிப்பு = mask;
        }
        led->pending_groups = false;
        led->groups = pc->மதிப்பு;
    }
    திரும்பு true;
}

/** Indices for the array "groupIndexNames" */
எண்ணிக்கை {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
எண்ணிக்கை {
    GROUP_MASK_NAME_LAST = 3,
};

நிலையான வெறுமை
update_auto_group_lock_on_modifier_release(கட்டமைப்பு xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           ஒன்றிப்பு xkb_action * restrict action)
{
    எனில் (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].இலக்கம்_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->இலக்கம்_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        திரும்பு;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
நிலையான bool
UpdateDerivedKeymapFields(கட்டமைப்பு xkb_keymap_info *info)
{
    கட்டமைப்பு xkb_keymap * restrict மாறிலி keymap = &info->keymap;

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
    static_assert(அளவு(கட்டமைப்பு xkb_சாவி_alias) == அளவு(KeycodeMatch), "");
#இல்லையெனில்
    static_assert(அளவு(கட்டமைப்பு xkb_சாவி_alias) == 2 * அளவு(KeycodeMatch), "");
#endif
    darray_size_t இலக்கம்_சாவி_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    க்கு (xkb_atom_t alias = 0; alias < keymap->இலக்கம்_சாவி_names; alias++) {
        மாறிலி KeycodeMatch entry = keymap->சாவி_names[alias];
        எனில் (entry.is_alias && entry.found) {
            எனில் (!இலக்கம்_சாவி_aliases)
                min_alias = alias;
            max_alias = alias;
            இலக்கம்_சாவி_aliases++;
        }
    }
    எனில் (இலக்கம்_சாவி_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        மாறிலி darray_size_t required_space = அளவு(கட்டமைப்பு xkb_சாவி_alias)
                                           / அளவு(KeycodeMatch)
                                           * இலக்கம்_சாவி_aliases;
        ப்ரதிஜ்ஞா(இலக்கம்_சாவி_aliases <= keymap->இலக்கம்_சாவி_names);
        எனில் (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_சாவி_aliases(keymap, min_alias, max_alias, keymap->சாவி_aliases);
            /* Shrink */
            கட்டமைப்பு xkb_சாவி_alias * மாறிலி r =
                realloc(keymap->சாவி_aliases,
                        இலக்கம்_சாவி_aliases * அளவு(*keymap->சாவி_aliases));
            எனில் (!r)
                திரும்பு false;
            keymap->சாவி_aliases = r;
        } இல்லையெனில் எனில் (keymap->இலக்கம்_சாவி_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            கட்டமைப்பு xkb_சாவி_alias * மாறிலி aliases = (கட்டமைப்பு xkb_சாவி_alias *) (
                keymap->சாவி_names + max_alias + 1 +
                !is_aligned(keymap->சாவி_names + max_alias + 1, அளவு(*aliases))
            );
            add_சாவி_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->சாவி_aliases, aliases,
                   இலக்கம்_சாவி_aliases * அளவு(*keymap->சாவி_aliases));
            /* Shrink */
            கட்டமைப்பு xkb_சாவி_alias * மாறிலி r =
                realloc(keymap->சாவி_aliases,
                        இலக்கம்_சாவி_aliases * அளவு(*keymap->சாவி_aliases));
            எனில் (!r)
                திரும்பு false;
            keymap->சாவி_aliases = r;
        } இல்லையெனில் {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            கட்டமைப்பு xkb_சாவி_alias * மாறிலி aliases = calloc(இலக்கம்_சாவி_aliases,
                                                          அளவு(*aliases));
            எனில் (!aliases)
                திரும்பு false;
            add_சாவி_aliases(keymap, min_alias, max_alias, aliases);
            விடுவி(keymap->சாவி_names);
            keymap->சாவி_aliases = aliases;
        }
    }
    keymap->இலக்கம்_சாவி_aliases = இலக்கம்_சாவி_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    கட்டமைப்பு xkb_சாவி *சாவி;
    xkb_சாவிகள்_foreach(சாவி, keymap)
        keymap->இலக்கம்_groups = MAX(keymap->இலக்கம்_groups, சாவி->இலக்கம்_groups);

    மாறிலி bool pending_computations = !darray_empty(*info->pending_computations);

    எனில் (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        மாறிலி xkb_layout_index_t இலக்கம்_groups
            = (keymap->இலக்கம்_groups)
            ? keymap->இலக்கம்_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .மதிப்பு = இலக்கம்_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .மதிப்பு = UINT32_C(1) << (இலக்கம்_groups - 1)
            };

        /* Update interpret fields with pending computations */
        க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_sym_interprets; i++) {
            கட்டமைப்பு xkb_sym_interpret * restrict மாறிலி interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                எனில் (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    திரும்பு false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_சாவிகள்_foreach(சாவி, keymap) {
        எனில் (!Apply_Interps_To_சாவி(keymap, சாவி))
            திரும்பு false;
        CheckMultipleActionsCategories(keymap, சாவி);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    கட்டமைப்பு xkb_mod *mod;
    xkb_சாவிகள்_foreach(சாவி, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            எனில் (சாவி->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= சாவி->modmap;

    எனில் (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            மாறிலி xkb_mod_mask_t mask = UINT32_C(1) << idx;
            எனில் (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    ப்ரதிஜ்ஞா(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        க்கு (darray_size_t j = 0; j < keymap->types[i].இலக்கம்_entries; j++) {
            எனில் (has_unbound_vmods(keymap,
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
                தொடர்;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_சாவிகள்_foreach(சாவி, keymap) {
        எனில் (!update_pending_சாவி_fields(info, சாவி))
            திரும்பு false;
        க்கு (xkb_layout_index_t i = 0; i < சாவி->இலக்கம்_groups; i++) {
            க்கு (xkb_level_index_t j = 0; j < Xkb_சாவி_இலக்கம்_Levels(சாவி, i); j++) {
                கட்டமைப்பு xkb_level * மாறிலி level = &சாவி->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, சாவி->modmap);
                    எனில் ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_சாவி) &&
                        !update_pending_action_fields(info, சாவி->keycode, act))
                    {
                        திரும்பு false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        சாவி->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    கட்டமைப்பு xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        எனில் (pending_computations &&
            !update_pending_led_fields(info, led))
            திரும்பு false;
    }

    திரும்பு true;
}

வகைவரை bool (*compile_file_fn)(XkbFile *file, கட்டமைப்பு xkb_keymap_info *info);

நிலையான மாறிலி compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_சாவி_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

நிலையான வெறுமை
pending_computations_array_free(pending_computation_array *p)
{
    கட்டமைப்பு pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->கோவை);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, கட்டமைப்பு xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { வெற்று };
    எண்ணிக்கை xkb_file_type type;
    கட்டமைப்பு xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    க்கு (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        எனில் (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            எனில் (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } இல்லையெனில் {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            தொடர்;
        }

        எனில் (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            தொடர்;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    கட்டமைப்பு xkb_keymap_info info = {
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
                    keymap->இலக்கம்_groups ? GROUP_LAST_INDEX_NAME : வெற்று,
                    keymap->இலக்கம்_groups /* 1-indexed */
                },
                { வெற்று, 0 }
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
                    keymap->இலக்கம்_groups ? GROUP_LAST_INDEX_NAME : வெற்று,
                    /* Be extra cautious */
                    keymap->இலக்கம்_groups && keymap->இலக்கம்_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->இலக்கம்_groups - 1))
                        : 0
                },
                { வெற்று, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    க்கு (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        எனில் (files[type] == வெற்று) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } இல்லையெனில் {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        மாறிலி bool ok = compile_file_fns[type](files[type], &info);
        எனில் (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            திரும்பு false;
        }
    }

    மாறிலி bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    திரும்பு ok;
}
