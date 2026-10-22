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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "ast-build.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "xkbcomp-priv.h"

স্থির bool
has_unbound_vmods(কাঠামো xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_সূচক_t k;
    কাঠামো xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        যদি ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            ফেরত true;
    ফেরত false;
}

স্থির অন্তঃরেখ শূন্য
ComputeEffectiveMask(কাঠামো xkb_keymap *keymap, কাঠামো xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ধ্রুবক xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.অঙ্ক_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

স্থির শূন্য
UpdateActionMods(কাঠামো xkb_keymap *keymap, সংঘ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    নির্বাচন (act->type) {
    ক্ষেত্র ACTION_TYPE_MOD_SET:
    ক্ষেত্র ACTION_TYPE_MOD_LATCH:
    ক্ষেত্র ACTION_TYPE_MOD_LOCK:
        যদি (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        বিরতি;
    পূর্বনির্ধারিত:
        বিরতি;
    }
}

স্থির ধ্রুবক কাঠামো xkb_sym_interpret default_interpret = {
    .sym = XKB_চাবি_No_প্রতীক /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_চাবি_REPEAT,
    .virtual_mod = (xkb_mod_সূচক_t) DEFAULT_INTERPRET_VMOD,
    .অঙ্ক_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

প্রকারলেখ darray(ধ্রুবক কাঠামো xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
স্থির bool
Find_Interp_For_চাবি(কাঠামো xkb_keymap *keymap, ধ্রুবক কাঠামো xkb_চাবি *চাবি,
                 xkb_layout_সূচক_t group, xkb_level_সূচক_t level,
                 xkb_sym_interprets *interprets)
{
    ধ্রুবক xkb_keysym_t *syms;
    পূর্ণসংখ্যা অঙ্ক_syms;

    অঙ্ক_syms = xkb_keymap_চাবি_get_syms_by_level(keymap, চাবি->keycode, group,
                                                level, &syms);
    যদি (অঙ্ক_syms <= 0)
        ফেরত false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    জন্য (পূর্ণসংখ্যা s = 0; s < অঙ্ক_syms; s++) {
        bool found = false;
        জন্য (darray_আকার_t i = 0; i < keymap->অঙ্ক_sym_interprets; i++) {
            কাঠামো xkb_sym_interpret * ধ্রুবক interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            যদি (interp->sym != syms[s] && interp->sym != XKB_চাবি_No_প্রতীক)
                চালু;

            যদি (interp->level_one_only && level != 0)
                mods = 0;
            অন্যথা
                mods = চাবি->modmap;

            নির্বাচন (interp->match) {
            ক্ষেত্র MATCH_NONE:
                found = !(interp->mods & mods);
                বিরতি;
            ক্ষেত্র MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                বিরতি;
            ক্ষেত্র MATCH_ANY:
                found = (interp->mods & mods);
                বিরতি;
            ক্ষেত্র MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                বিরতি;
            ক্ষেত্র MATCH_EXACTLY:
                found = (interp->mods == mods);
                বিরতি;
            }

            যদি (found && i > 0 && interp->sym == XKB_চাবি_No_প্রতীক) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                কাঠামো xkb_sym_interpret ধ্রুবক **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    যদি (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 চাবি_Name_Text(keymap->ctx, চাবি->name));
                        যাও not_found;
                    }
                }
            }
            যদি (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                বিরতি;
            }
        }
        যদি (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    ফেরত true;
}

স্থির bool
Apply_Interps_To_চাবি(কাঠামো xkb_keymap *keymap, কাঠামো xkb_চাবি *চাবি)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_সূচক_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(সংঘ xkb_action) actions = darray_new();

    জন্য (xkb_layout_সূচক_t group = 0; group < চাবি->অঙ্ক_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        যদি (চাবি->groups[group].explicit_actions)
            চালু;

        জন্য (level = 0; level < Xkb_চাবি_অঙ্ক_Levels(চাবি, group); level++) {
            প্রতিজ্ঞা(চাবি->groups[group].levels[level].অঙ্ক_actions == 0);

            ধ্রুবক কাঠামো xkb_sym_interpret **interp_iter;
            ধ্রুবক কাঠামো xkb_sym_interpret *interp;
            আকার_t k;
            darray_resize(interprets, 0);

            ধ্রুবক bool found = Find_Interp_For_চাবি(keymap, চাবি, group, level, &interprets);
            যদি (!found)
                চালু;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                যদি (group == 0 && level == 0 &&
                    !(চাবি->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    চাবি->repeats = true;
                }

                যদি ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    যদি (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                নির্বাচন (interp->অঙ্ক_actions) {
                ক্ষেত্র 0:
                    বিরতি;
                ক্ষেত্র 1:
                    darray_append(actions, interp->a.action);
                    বিরতি;
                পূর্বনির্ধারিত:
                    darray_append_বস্তুসমূহ(actions, interp->a.actions,
                                        interp->অঙ্ক_actions);
                }
            }

            /* Copy the actions */
            যদি (unlikely(darray_আকার(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         চাবি_Name_Text(keymap->ctx, চাবি->name),
                         MAX_ACTIONS_PER_LEVEL, darray_আকার(actions));
                চাবি->groups[group].levels[level].অঙ্ক_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } অন্যথা {
                চাবি->groups[group].levels[level].অঙ্ক_actions =
                    (xkb_action_গুনতি_t) darray_আকার(actions);
            }
            নির্বাচন (darray_আকার(actions)) {
                ক্ষেত্র 0:
                    চাবি->groups[group].levels[level].a.action =
                        (সংঘ xkb_action) { .type = ACTION_TYPE_NONE };
                    বিরতি;
                ক্ষেত্র 1:
                    চাবি->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    বিরতি;
                পূর্বনির্ধারিত:
                    চাবি->groups[group].levels[level].a.actions =
                        memdup(darray_বস্তুসমূহ(actions),
                               চাবি->groups[group].levels[level].অঙ্ক_actions,
                               আকার(*darray_বস্তুসমূহ(actions)));
                    যদি (!চাবি->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ফেরত false;
                    }
            }

            যদি (!darray_empty(actions))
                চাবি->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        যদি (চাবি->groups[group].implicit_actions)
            চাবি->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    যদি (!(চাবি->explicit & EXPLICIT_VMODMAP))
        চাবি->vmodmap = vmodmap;

    ফেরত true;
}

স্থির অন্তঃরেখ bool
is_mod_action(সংঘ xkb_action *action)
{
    ফেরত action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

স্থির অন্তঃরেখ bool
is_group_action(সংঘ xkb_action *action)
{
    ফেরত action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
স্থির শূন্য
CheckMultipleActionsCategories(কাঠামো xkb_keymap *keymap, কাঠামো xkb_চাবি *চাবি)
{
    জন্য (xkb_layout_সূচক_t g = 0; g < চাবি->অঙ্ক_groups; g++) {
        জন্য (xkb_level_সূচক_t l = 0; l < Xkb_চাবি_অঙ্ক_Levels(চাবি, g); l++) {
            কাঠামো xkb_level *level = &চাবি->groups[g].levels[l];
            যদি (level->অঙ্ক_actions <= 1)
                চালু;
            জন্য (xkb_action_গুনতি_t i = 0; i < level->অঙ্ক_actions; i++) {
                সংঘ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                যদি (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_চাবি))
                    চালু;
                জন্য (xkb_action_গুনতি_t j = i + 1; j < level->অঙ্ক_actions; j++) {
                    সংঘ xkb_action *action2 = &level->a.actions[j];
                    যদি (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ধ্রুবক অক্ষর * ধ্রুবক type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, চাবি_Name_Text(keymap->ctx, চাবি->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

স্থির শূন্য
add_চাবি_aliases(কাঠামো xkb_keymap *keymap, darray_আকার_t min, darray_আকার_t max,
                কাঠামো xkb_চাবি_alias *aliases)
{
    জন্য (darray_আকার_t alias = min; alias <= max; alias++) {
        ধ্রুবক KeycodeMatch entry = keymap->চাবি_names[alias];
        যদি (entry.is_alias && entry.found) {
            *aliases = (কাঠামো xkb_চাবি_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

স্থির bool
update_pending_চাবি_fields(কাঠামো xkb_keymap_info *info, কাঠামো xkb_চাবি *চাবি)
{
    যদি (চাবি->out_of_range_pending_group) {
        কাঠামো pending_computation * ধ্রুবক pc = &darray_item(
            *info->pending_computations, চাবি->out_of_range_group_সংখ্যা
        );
        যদি (!pc->computed) {
            xkb_layout_সূচক_t group = 0;
            নির্বাচন (রাশিমালা_Resolve_Group(info, pc->রাশিমালা, true, &group, ফাঁকা)) {
            ক্ষেত্র PARSER_SUCCESS:
                pc->computed = true;
                pc->মান = group - 1;
                বিরতি;
            ক্ষেত্র PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                        "Invalid key redirect group index\n");
                ফেরত (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            পূর্বনির্ধারিত:
                /* Recover from error: ignore */;
            }
        }
        চাবি->out_of_range_pending_group = false;
        চাবি->out_of_range_group_সংখ্যা = pc->মান;
    }
    ফেরত true;
}

স্থির bool
update_pending_action_fields(কাঠামো xkb_keymap_info *info,
                             xkb_keycode_t keycode, সংঘ xkb_action *act)
{
    নির্বাচন (act->type) {
    ক্ষেত্র ACTION_TYPE_GROUP_SET:
    ক্ষেত্র ACTION_TYPE_GROUP_LATCH:
    ক্ষেত্র ACTION_TYPE_GROUP_LOCK:
        যদি (act->group.flags & ACTION_PENDING_COMPUTATION) {
            কাঠামো pending_computation * ধ্রুবক pc =
                &darray_item(*info->pending_computations, act->group.group);
            যদি (!pc->computed) {
                xkb_layout_সূচক_t group = 0;
                ধ্রুবক bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                নির্বাচন (রাশিমালা_Resolve_Group(info, pc->রাশিমালা, absolute, &group, ফাঁকা)) {
                ক্ষেত্র PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                            "Invalid action group index\n");
                    ফেরত false;
                ক্ষেত্র PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    বিরতি;
                পূর্বনির্ধারিত:
                    pc->computed = true;
                    যদি (absolute) {
                        pc->মান = group - 1;
                    } অন্যথা {
                        pc->মান = group;
                        যদি (pc->রাশিমালা->common.type == STMT_রাশিমালা_NEGATE)
                            pc->মান = (uint32_t) (-(int32_t) pc->মান);
                    }
                }
            }
            act->group.group = (int32_t) pc->মান;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ফেরত true;
    ক্ষেত্র ACTION_TYPE_REDIRECT_চাবি: {
        যদি (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_চাবি_auto) {
            /* No auto value to set */
            ফেরত true;
        } অন্যথা {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ফেরত true;
    }
    পূর্বনির্ধারিত:
        ফেরত true;
    }
}

স্থির bool
update_pending_led_fields(কাঠামো xkb_keymap_info *info, কাঠামো xkb_led *led)
{
    যদি (led->pending_groups) {
        কাঠামো pending_computation * ধ্রুবক pc =
            &darray_item(*info->pending_computations, led->groups);
        যদি (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            যদি (!রাশিমালা_Resolve_Group_Mask(info, pc->রাশিমালা, &mask, ফাঁকা)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                        "Invalid LED group mask\n");
                ফেরত false;
            }
            pc->computed = true;
            pc->মান = mask;
        }
        led->pending_groups = false;
        led->groups = pc->মান;
    }
    ফেরত true;
}

/** Indices for the array "groupIndexNames" */
গণনা {
    GROUP_সূচক_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
গণনা {
    GROUP_MASK_NAME_LAST = 3,
};

স্থির শূন্য
update_auto_group_lock_on_modifier_release(কাঠামো xkb_level * restrict level0,
                                           xkb_level_সূচক_t l,
                                           সংঘ xkb_action * restrict action)
{
    যদি (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].অঙ্ক_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->অঙ্ক_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        ফেরত;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
স্থির bool
UpdateDerivedKeymapFields(কাঠামো xkb_keymap_info *info)
{
    কাঠামো xkb_keymap * restrict ধ্রুবক keymap = &info->keymap;

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
    static_assert(আকার(কাঠামো xkb_চাবি_alias) == আকার(KeycodeMatch), "");
#অন্যথা
    static_assert(আকার(কাঠামো xkb_চাবি_alias) == 2 * আকার(KeycodeMatch), "");
#endif
    darray_আকার_t অঙ্ক_চাবি_aliases = 0;
    darray_আকার_t min_alias = 0;
    darray_আকার_t max_alias = 0;
    জন্য (xkb_atom_t alias = 0; alias < keymap->অঙ্ক_চাবি_names; alias++) {
        ধ্রুবক KeycodeMatch entry = keymap->চাবি_names[alias];
        যদি (entry.is_alias && entry.found) {
            যদি (!অঙ্ক_চাবি_aliases)
                min_alias = alias;
            max_alias = alias;
            অঙ্ক_চাবি_aliases++;
        }
    }
    যদি (অঙ্ক_চাবি_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ধ্রুবক darray_আকার_t required_space = আকার(কাঠামো xkb_চাবি_alias)
                                           / আকার(KeycodeMatch)
                                           * অঙ্ক_চাবি_aliases;
        প্রতিজ্ঞা(অঙ্ক_চাবি_aliases <= keymap->অঙ্ক_চাবি_names);
        যদি (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_চাবি_aliases(keymap, min_alias, max_alias, keymap->চাবি_aliases);
            /* Shrink */
            কাঠামো xkb_চাবি_alias * ধ্রুবক r =
                realloc(keymap->চাবি_aliases,
                        অঙ্ক_চাবি_aliases * আকার(*keymap->চাবি_aliases));
            যদি (!r)
                ফেরত false;
            keymap->চাবি_aliases = r;
        } অন্যথা যদি (keymap->অঙ্ক_চাবি_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            কাঠামো xkb_চাবি_alias * ধ্রুবক aliases = (কাঠামো xkb_চাবি_alias *) (
                keymap->চাবি_names + max_alias + 1 +
                !is_aligned(keymap->চাবি_names + max_alias + 1, আকার(*aliases))
            );
            add_চাবি_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->চাবি_aliases, aliases,
                   অঙ্ক_চাবি_aliases * আকার(*keymap->চাবি_aliases));
            /* Shrink */
            কাঠামো xkb_চাবি_alias * ধ্রুবক r =
                realloc(keymap->চাবি_aliases,
                        অঙ্ক_চাবি_aliases * আকার(*keymap->চাবি_aliases));
            যদি (!r)
                ফেরত false;
            keymap->চাবি_aliases = r;
        } অন্যথা {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            কাঠামো xkb_চাবি_alias * ধ্রুবক aliases = calloc(অঙ্ক_চাবি_aliases,
                                                          আকার(*aliases));
            যদি (!aliases)
                ফেরত false;
            add_চাবি_aliases(keymap, min_alias, max_alias, aliases);
            মুক্ত(keymap->চাবি_names);
            keymap->চাবি_aliases = aliases;
        }
    }
    keymap->অঙ্ক_চাবি_aliases = অঙ্ক_চাবি_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    কাঠামো xkb_চাবি *চাবি;
    xkb_চাবিগুলি_foreach(চাবি, keymap)
        keymap->অঙ্ক_groups = MAX(keymap->অঙ্ক_groups, চাবি->অঙ্ক_groups);

    ধ্রুবক bool pending_computations = !darray_empty(*info->pending_computations);

    যদি (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ধ্রুবক xkb_layout_সূচক_t অঙ্ক_groups
            = (keymap->অঙ্ক_groups)
            ? keymap->অঙ্ক_groups
            : 1; /* default to the “first” layout */
        info->lookup.group_সূচক_Names[GROUP_সূচক_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_সূচক_NAME,
                .মান = অঙ্ক_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_সূচক_NAME,
                .মান = UINT32_C(1) << (অঙ্ক_groups - 1)
            };

        /* Update interpret fields with pending computations */
        জন্য (darray_আকার_t i = 0; i < keymap->অঙ্ক_sym_interprets; i++) {
            কাঠামো xkb_sym_interpret * restrict ধ্রুবক interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                যদি (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ফেরত false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (!Apply_Interps_To_চাবি(keymap, চাবি))
            ফেরত false;
        CheckMultipleActionsCategories(keymap, চাবি);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_সূচক_t idx;
    কাঠামো xkb_mod *mod;
    xkb_চাবিগুলি_foreach(চাবি, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            যদি (চাবি->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= চাবি->modmap;

    যদি (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            ধ্রুবক xkb_mod_mask_t mask = UINT32_C(1) << idx;
            যদি (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    প্রতিজ্ঞা(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    জন্য (darray_আকার_t i = 0; i < keymap->অঙ্ক_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        জন্য (darray_আকার_t j = 0; j < keymap->types[i].অঙ্ক_entries; j++) {
            যদি (has_unbound_vmods(keymap,
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
                চালু;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (!update_pending_চাবি_fields(info, চাবি))
            ফেরত false;
        জন্য (xkb_layout_সূচক_t i = 0; i < চাবি->অঙ্ক_groups; i++) {
            জন্য (xkb_level_সূচক_t j = 0; j < Xkb_চাবি_অঙ্ক_Levels(চাবি, i); j++) {
                কাঠামো xkb_level * ধ্রুবক level = &চাবি->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, চাবি->modmap);
                    যদি ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_চাবি) &&
                        !update_pending_action_fields(info, চাবি->keycode, act))
                    {
                        ফেরত false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        চাবি->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    কাঠামো xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        যদি (pending_computations &&
            !update_pending_led_fields(info, led))
            ফেরত false;
    }

    ফেরত true;
}

প্রকারলেখ bool (*compile_file_fn)(XkbFile *file, কাঠামো xkb_keymap_info *info);

স্থির ধ্রুবক compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_চাবি_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

স্থির শূন্য
pending_computations_array_free(pending_computation_array *p)
{
    কাঠামো pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->রাশিমালা);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, কাঠামো xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { ফাঁকা };
    গণনা xkb_file_type type;
    কাঠামো xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    জন্য (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.পরবর্তী) {
        যদি (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            যদি (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } অন্যথা {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            চালু;
        }

        যদি (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            চালু;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    কাঠামো xkb_keymap_info info = {
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
            .group_সূচক_Names = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [GROUP_সূচক_NAME_LAST] = {
                    keymap->অঙ্ক_groups ? GROUP_LAST_সূচক_NAME : ফাঁকা,
                    keymap->অঙ্ক_groups /* 1-indexed */
                },
                { ফাঁকা, 0 }
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
                    keymap->অঙ্ক_groups ? GROUP_LAST_সূচক_NAME : ফাঁকা,
                    /* Be extra cautious */
                    keymap->অঙ্ক_groups && keymap->অঙ্ক_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->অঙ্ক_groups - 1))
                        : 0
                },
                { ফাঁকা, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    জন্য (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        যদি (files[type] == ফাঁকা) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } অন্যথা {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ধ্রুবক bool ok = compile_file_fns[type](files[type], &info);
        যদি (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ফেরত false;
        }
    }

    ধ্রুবক bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ফেরত ok;
}
