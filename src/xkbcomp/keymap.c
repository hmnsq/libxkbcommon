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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "ast-build.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

ସ୍ଥିର bool
has_unbound_vmods(ସଂରଚନା xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    ସଂରଚନା xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        ଯଦି ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            ଫେରନ୍ତୁ true;
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
ComputeEffectiveMask(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ଅଚଳ xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.ଅଙ୍କ_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ସ୍ଥିର ଶୂନ୍ୟ
UpdateActionMods(ସଂରଚନା xkb_keymap *keymap, ସଂଘ xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    ବାଛ (act->type) {
    ସ୍ଥିତି ACTION_TYPE_MOD_SET:
    ସ୍ଥିତି ACTION_TYPE_MOD_LATCH:
    ସ୍ଥିତି ACTION_TYPE_MOD_LOCK:
        ଯଦି (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଭାଙ୍ଗ;
    }
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_sym_interpret default_interpret = {
    .sym = XKB_ଚାବି_No_ପ୍ରତୀକ /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ଚାବି_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .ଅଙ୍କ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ଅଚଳ ସଂରଚନା xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ସ୍ଥିର bool
Find_Interp_For_ଚାବି(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    ଅଚଳ xkb_keysym_t *syms;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_syms;

    ଅଙ୍କ_syms = xkb_keymap_ଚାବି_get_syms_by_level(keymap, ଚାବି->keycode, group,
                                                level, &syms);
    ଯଦି (ଅଙ୍କ_syms <= 0)
        ଫେରନ୍ତୁ false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < ଅଙ୍କ_syms; s++) {
        bool found = false;
        ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_sym_interprets; i++) {
            ସଂରଚନା xkb_sym_interpret * ଅଚଳ interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            ଯଦି (interp->sym != syms[s] && interp->sym != XKB_ଚାବି_No_ପ୍ରତୀକ)
                ଜାରି;

            ଯଦି (interp->level_one_only && level != 0)
                mods = 0;
            ଅନ୍ୟଥା
                mods = ଚାବି->modmap;

            ବାଛ (interp->match) {
            ସ୍ଥିତି MATCH_NONE:
                found = !(interp->mods & mods);
                ଭାଙ୍ଗ;
            ସ୍ଥିତି MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                ଭାଙ୍ଗ;
            ସ୍ଥିତି MATCH_ANY:
                found = (interp->mods & mods);
                ଭାଙ୍ଗ;
            ସ୍ଥିତି MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                ଭାଙ୍ଗ;
            ସ୍ଥିତି MATCH_EXACTLY:
                found = (interp->mods == mods);
                ଭାଙ୍ଗ;
            }

            ଯଦି (found && i > 0 && interp->sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ସଂରଚନା xkb_sym_interpret ଅଚଳ **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    ଯଦି (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 ଚାବି_Name_Text(keymap->ctx, ଚାବି->name));
                        ଯାଅ not_found;
                    }
                }
            }
            ଯଦି (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Apply_Interps_To_ଚାବି(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(ସଂଘ xkb_action) actions = darray_new();

    ପାଇଁ (xkb_layout_index_t group = 0; group < ଚାବି->ଅଙ୍କ_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        ଯଦି (ଚାବି->groups[group].explicit_actions)
            ଜାରି;

        ପାଇଁ (level = 0; level < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, group); level++) {
            ପ୍ରତିଜ୍ଞା(ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions == 0);

            ଅଚଳ ସଂରଚନା xkb_sym_interpret **interp_iter;
            ଅଚଳ ସଂରଚନା xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            ଅଚଳ bool found = Find_Interp_For_ଚାବି(keymap, ଚାବି, group, level, &interprets);
            ଯଦି (!found)
                ଜାରି;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                ଯଦି (group == 0 && level == 0 &&
                    !(ଚାବି->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ଚାବି->repeats = true;
                }

                ଯଦି ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    ଯଦି (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                ବାଛ (interp->ଅଙ୍କ_actions) {
                ସ୍ଥିତି 0:
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 1:
                    darray_append(actions, interp->a.action);
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    darray_append_ବସ୍ତୁଗୁଡ଼ିକ(actions, interp->a.actions,
                                        interp->ଅଙ୍କ_actions);
                }
            }

            /* Copy the actions */
            ଯଦି (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ଚାବି_Name_Text(keymap->ctx, ଚାବି->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } ଅନ୍ୟଥା {
                ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions =
                    (xkb_action_ଗଣନା_t) darray_size(actions);
            }
            ବାଛ (darray_size(actions)) {
                ସ୍ଥିତି 0:
                    ଚାବି->groups[group].levels[level].a.action =
                        (ସଂଘ xkb_action) { .type = ACTION_TYPE_NONE };
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 1:
                    ଚାବି->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    ଚାବି->groups[group].levels[level].a.actions =
                        memdup(darray_ବସ୍ତୁଗୁଡ଼ିକ(actions),
                               ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions,
                               ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(actions)));
                    ଯଦି (!ଚାବି->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ଫେରନ୍ତୁ false;
                    }
            }

            ଯଦି (!darray_empty(actions))
                ଚାବି->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        ଯଦି (ଚାବି->groups[group].implicit_actions)
            ଚାବି->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    ଯଦି (!(ଚାବି->explicit & EXPLICIT_VMODMAP))
        ଚାବି->vmodmap = vmodmap;

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_mod_action(ସଂଘ xkb_action *action)
{
    ଫେରନ୍ତୁ action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_group_action(ସଂଘ xkb_action *action)
{
    ଫେରନ୍ତୁ action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ସ୍ଥିର ଶୂନ୍ୟ
CheckMultipleActionsCategories(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    ପାଇଁ (xkb_layout_index_t g = 0; g < ଚାବି->ଅଙ୍କ_groups; g++) {
        ପାଇଁ (xkb_level_index_t l = 0; l < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, g); l++) {
            ସଂରଚନା xkb_level *level = &ଚାବି->groups[g].levels[l];
            ଯଦି (level->ଅଙ୍କ_actions <= 1)
                ଜାରି;
            ପାଇଁ (xkb_action_ଗଣନା_t i = 0; i < level->ଅଙ୍କ_actions; i++) {
                ସଂଘ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                ଯଦି (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ଚାବି))
                    ଜାରି;
                ପାଇଁ (xkb_action_ଗଣନା_t j = i + 1; j < level->ଅଙ୍କ_actions; j++) {
                    ସଂଘ xkb_action *action2 = &level->a.actions[j];
                    ଯଦି (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ଅଚଳ ଅକ୍ଷର * ଅଚଳ type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ଚାବି_Name_Text(keymap->ctx, ଚାବି->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
add_ଚାବି_aliases(ସଂରଚନା xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                ସଂରଚନା xkb_ଚାବି_alias *aliases)
{
    ପାଇଁ (darray_size_t alias = min; alias <= max; alias++) {
        ଅଚଳ KeycodeMatch entry = keymap->ଚାବି_names[alias];
        ଯଦି (entry.is_alias && entry.found) {
            *aliases = (ସଂରଚନା xkb_ଚାବି_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

ସ୍ଥିର bool
update_pending_ଚାବି_fields(ସଂରଚନା xkb_keymap_info *info, ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    ଯଦି (ଚାବି->out_of_range_pending_group) {
        ସଂରଚନା pending_computation * ଅଚଳ pc = &darray_item(
            *info->pending_computations, ଚାବି->out_of_range_group_ସଂଖ୍ୟା
        );
        ଯଦି (!pc->computed) {
            xkb_layout_index_t group = 0;
            ବାଛ (ପଦସମଷ୍ଟି_Resolve_Group(info, pc->ପଦସମଷ୍ଟି, true, &group, ଖାଲି)) {
            ସ୍ଥିତି PARSER_SUCCESS:
                pc->computed = true;
                pc->ମୂଲ୍ୟ = group - 1;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                ଫେରନ୍ତୁ (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                /* Recover from error: ignore */;
            }
        }
        ଚାବି->out_of_range_pending_group = false;
        ଚାବି->out_of_range_group_ସଂଖ୍ୟା = pc->ମୂଲ୍ୟ;
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
update_pending_action_fields(ସଂରଚନା xkb_keymap_info *info,
                             xkb_keycode_t keycode, ସଂଘ xkb_action *act)
{
    ବାଛ (act->type) {
    ସ୍ଥିତି ACTION_TYPE_GROUP_SET:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LATCH:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LOCK:
        ଯଦି (act->group.flags & ACTION_PENDING_COMPUTATION) {
            ସଂରଚନା pending_computation * ଅଚଳ pc =
                &darray_item(*info->pending_computations, act->group.group);
            ଯଦି (!pc->computed) {
                xkb_layout_index_t group = 0;
                ଅଚଳ bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                ବାଛ (ପଦସମଷ୍ଟି_Resolve_Group(info, pc->ପଦସମଷ୍ଟି, absolute, &group, ଖାଲି)) {
                ସ୍ଥିତି PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    ଫେରନ୍ତୁ false;
                ସ୍ଥିତି PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    pc->computed = true;
                    ଯଦି (absolute) {
                        pc->ମୂଲ୍ୟ = group - 1;
                    } ଅନ୍ୟଥା {
                        pc->ମୂଲ୍ୟ = group;
                        ଯଦି (pc->ପଦସମଷ୍ଟି->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE)
                            pc->ମୂଲ୍ୟ = (uint32_t) (-(int32_t) pc->ମୂଲ୍ୟ);
                    }
                }
            }
            act->group.group = (int32_t) pc->ମୂଲ୍ୟ;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ଫେରନ୍ତୁ true;
    ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି: {
        ଯଦି (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ଚାବି_auto) {
            /* No auto value to set */
            ଫେରନ୍ତୁ true;
        } ଅନ୍ୟଥା {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ଫେରନ୍ତୁ true;
    }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ true;
    }
}

ସ୍ଥିର bool
update_pending_led_fields(ସଂରଚନା xkb_keymap_info *info, ସଂରଚନା xkb_led *led)
{
    ଯଦି (led->pending_groups) {
        ସଂରଚନା pending_computation * ଅଚଳ pc =
            &darray_item(*info->pending_computations, led->groups);
        ଯଦି (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Group_Mask(info, pc->ପଦସମଷ୍ଟି, &mask, ଖାଲି)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                ଫେରନ୍ତୁ false;
            }
            pc->computed = true;
            pc->ମୂଲ୍ୟ = mask;
        }
        led->pending_groups = false;
        led->groups = pc->ମୂଲ୍ୟ;
    }
    ଫେରନ୍ତୁ true;
}

/** Indices for the array "groupIndexNames" */
ଗଣନା {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
ଗଣନା {
    GROUP_MASK_NAME_LAST = 3,
};

ସ୍ଥିର ଶୂନ୍ୟ
update_auto_group_lock_on_modifier_release(ସଂରଚନା xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           ସଂଘ xkb_action * restrict action)
{
    ଯଦି (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].ଅଙ୍କ_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->ଅଙ୍କ_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        ଫେରନ୍ତୁ;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ସ୍ଥିର bool
UpdateDerivedKeymapFields(ସଂରଚନା xkb_keymap_info *info)
{
    ସଂରଚନା xkb_keymap * restrict ଅଚଳ keymap = &info->keymap;

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
    static_assert(ଆକାର(ସଂରଚନା xkb_ଚାବି_alias) == ଆକାର(KeycodeMatch), "");
#ଅନ୍ୟଥା
    static_assert(ଆକାର(ସଂରଚନା xkb_ଚାବି_alias) == 2 * ଆକାର(KeycodeMatch), "");
#endif
    darray_size_t ଅଙ୍କ_ଚାବି_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    ପାଇଁ (xkb_atom_t alias = 0; alias < keymap->ଅଙ୍କ_ଚାବି_names; alias++) {
        ଅଚଳ KeycodeMatch entry = keymap->ଚାବି_names[alias];
        ଯଦି (entry.is_alias && entry.found) {
            ଯଦି (!ଅଙ୍କ_ଚାବି_aliases)
                min_alias = alias;
            max_alias = alias;
            ଅଙ୍କ_ଚାବି_aliases++;
        }
    }
    ଯଦି (ଅଙ୍କ_ଚାବି_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ଅଚଳ darray_size_t required_space = ଆକାର(ସଂରଚନା xkb_ଚାବି_alias)
                                           / ଆକାର(KeycodeMatch)
                                           * ଅଙ୍କ_ଚାବି_aliases;
        ପ୍ରତିଜ୍ଞା(ଅଙ୍କ_ଚାବି_aliases <= keymap->ଅଙ୍କ_ଚାବି_names);
        ଯଦି (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_ଚାବି_aliases(keymap, min_alias, max_alias, keymap->ଚାବି_aliases);
            /* Shrink */
            ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ r =
                realloc(keymap->ଚାବି_aliases,
                        ଅଙ୍କ_ଚାବି_aliases * ଆକାର(*keymap->ଚାବି_aliases));
            ଯଦି (!r)
                ଫେରନ୍ତୁ false;
            keymap->ଚାବି_aliases = r;
        } ଅନ୍ୟଥା ଯଦି (keymap->ଅଙ୍କ_ଚାବି_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ aliases = (ସଂରଚନା xkb_ଚାବି_alias *) (
                keymap->ଚାବି_names + max_alias + 1 +
                !is_aligned(keymap->ଚାବି_names + max_alias + 1, ଆକାର(*aliases))
            );
            add_ଚାବି_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->ଚାବି_aliases, aliases,
                   ଅଙ୍କ_ଚାବି_aliases * ଆକାର(*keymap->ଚାବି_aliases));
            /* Shrink */
            ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ r =
                realloc(keymap->ଚାବି_aliases,
                        ଅଙ୍କ_ଚାବି_aliases * ଆକାର(*keymap->ଚାବି_aliases));
            ଯଦି (!r)
                ଫେରନ୍ତୁ false;
            keymap->ଚାବି_aliases = r;
        } ଅନ୍ୟଥା {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ aliases = calloc(ଅଙ୍କ_ଚାବି_aliases,
                                                          ଆକାର(*aliases));
            ଯଦି (!aliases)
                ଫେରନ୍ତୁ false;
            add_ଚାବି_aliases(keymap, min_alias, max_alias, aliases);
            ମୁକ୍ତ(keymap->ଚାବି_names);
            keymap->ଚାବି_aliases = aliases;
        }
    }
    keymap->ଅଙ୍କ_ଚାବି_aliases = ଅଙ୍କ_ଚାବି_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ସଂରଚନା xkb_ଚାବି *ଚାବି;
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap)
        keymap->ଅଙ୍କ_groups = MAX(keymap->ଅଙ୍କ_groups, ଚାବି->ଅଙ୍କ_groups);

    ଅଚଳ bool pending_computations = !darray_empty(*info->pending_computations);

    ଯଦି (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ଅଚଳ xkb_layout_index_t ଅଙ୍କ_groups
            = (keymap->ଅଙ୍କ_groups)
            ? keymap->ଅଙ୍କ_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .ମୂଲ୍ୟ = ଅଙ୍କ_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .ମୂଲ୍ୟ = UINT32_C(1) << (ଅଙ୍କ_groups - 1)
            };

        /* Update interpret fields with pending computations */
        ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_sym_interprets; i++) {
            ସଂରଚନା xkb_sym_interpret * restrict ଅଚଳ interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                ଯଦି (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ଫେରନ୍ତୁ false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        ଯଦି (!Apply_Interps_To_ଚାବି(keymap, ଚାବି))
            ଫେରନ୍ତୁ false;
        CheckMultipleActionsCategories(keymap, ଚାବି);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    ସଂରଚନା xkb_mod *mod;
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            ଯଦି (ଚାବି->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ଚାବି->modmap;

    ଯଦି (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            ଅଚଳ xkb_mod_mask_t mask = UINT32_C(1) << idx;
            ଯଦି (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    ପ୍ରତିଜ୍ଞା(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        ପାଇଁ (darray_size_t j = 0; j < keymap->types[i].ଅଙ୍କ_entries; j++) {
            ଯଦି (has_unbound_vmods(keymap,
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
                ଜାରି;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        ଯଦି (!update_pending_ଚାବି_fields(info, ଚାବି))
            ଫେରନ୍ତୁ false;
        ପାଇଁ (xkb_layout_index_t i = 0; i < ଚାବି->ଅଙ୍କ_groups; i++) {
            ପାଇଁ (xkb_level_index_t j = 0; j < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, i); j++) {
                ସଂରଚନା xkb_level * ଅଚଳ level = &ଚାବି->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ଚାବି->modmap);
                    ଯଦି ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ଚାବି) &&
                        !update_pending_action_fields(info, ଚାବି->keycode, act))
                    {
                        ଫେରନ୍ତୁ false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        ଚାବି->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ସଂରଚନା xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        ଯଦି (pending_computations &&
            !update_pending_led_fields(info, led))
            ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା bool (*compile_file_fn)(XkbFile *file, ସଂରଚନା xkb_keymap_info *info);

ସ୍ଥିର ଅଚଳ compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ଚାବି_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ସ୍ଥିର ଶୂନ୍ୟ
pending_computations_array_free(pending_computation_array *p)
{
    ସଂରଚନା pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->ପଦସମଷ୍ଟି);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ସଂରଚନା xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { ଖାଲି };
    ଗଣନା xkb_file_type type;
    ସଂରଚନା xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    ପାଇଁ (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        ଯଦି (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            ଯଦି (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } ଅନ୍ୟଥା {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            ଜାରି;
        }

        ଯଦି (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            ଜାରି;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ସଂରଚନା xkb_keymap_info info = {
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
                    keymap->ଅଙ୍କ_groups ? GROUP_LAST_INDEX_NAME : ଖାଲି,
                    keymap->ଅଙ୍କ_groups /* 1-indexed */
                },
                { ଖାଲି, 0 }
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
                    keymap->ଅଙ୍କ_groups ? GROUP_LAST_INDEX_NAME : ଖାଲି,
                    /* Be extra cautious */
                    keymap->ଅଙ୍କ_groups && keymap->ଅଙ୍କ_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->ଅଙ୍କ_groups - 1))
                        : 0
                },
                { ଖାଲି, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    ପାଇଁ (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        ଯଦି (files[type] == ଖାଲି) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } ଅନ୍ୟଥା {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ଅଚଳ bool ok = compile_file_fns[type](files[type], &info);
        ଯଦି (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ଫେରନ୍ତୁ false;
        }
    }

    ଅଚଳ bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ଫେରନ୍ତୁ ok;
}
