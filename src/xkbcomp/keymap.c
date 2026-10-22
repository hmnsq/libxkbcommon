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

#ضمن "config.h"

#ضمن <string.h>
#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "ast-build.h"
#ضمن "darray.h"
#ضمن "expr.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

ساكن bool
has_unbound_vmods(بنية xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    بنية xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        إذا ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            أرجع true;
    أرجع false;
}

ساكن مضمن فراغ
ComputeEffectiveMask(بنية xkb_keymap *keymap, بنية xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ثابت xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.اڠک_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ساكن فراغ
UpdateActionMods(بنية xkb_keymap *keymap, اتحاد xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    بدل (act->type) {
    حالة ACTION_TYPE_MOD_SET:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_LOCK:
        إذا (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        اكسر;
    افتراضي:
        اكسر;
    }
}

ساكن ثابت بنية xkb_sym_interpret default_interpret = {
    .sym = XKB_کونچي_No_سيمبول /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_کونچي_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .اڠک_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

تعريفنوع darray(ثابت بنية xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ساكن bool
Find_Interp_For_کونچي(بنية xkb_keymap *keymap, ثابت بنية xkb_کونچي *کونچي,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    ثابت xkb_keysym_t *syms;
    صحيح اڠک_syms;

    اڠک_syms = xkb_keymap_کونچي_get_syms_by_level(keymap, کونچي->keycode, group,
                                                level, &syms);
    إذا (اڠک_syms <= 0)
        أرجع false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    لكل (صحيح s = 0; s < اڠک_syms; s++) {
        bool found = false;
        لكل (darray_size_t i = 0; i < keymap->اڠک_sym_interprets; i++) {
            بنية xkb_sym_interpret * ثابت interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            إذا (interp->sym != syms[s] && interp->sym != XKB_کونچي_No_سيمبول)
                استمر;

            إذا (interp->level_one_only && level != 0)
                mods = 0;
            وإلا
                mods = کونچي->modmap;

            بدل (interp->match) {
            حالة MATCH_NONE:
                found = !(interp->mods & mods);
                اكسر;
            حالة MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                اكسر;
            حالة MATCH_ANY:
                found = (interp->mods & mods);
                اكسر;
            حالة MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                اكسر;
            حالة MATCH_EXACTLY:
                found = (interp->mods == mods);
                اكسر;
            }

            إذا (found && i > 0 && interp->sym == XKB_کونچي_No_سيمبول) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                بنية xkb_sym_interpret ثابت **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    إذا (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 کونچي_Name_Text(keymap->ctx, کونچي->name));
                        اذهب not_found;
                    }
                }
            }
            إذا (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                اكسر;
            }
        }
        إذا (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    أرجع true;
}

ساكن bool
Apply_Interps_To_کونچي(بنية xkb_keymap *keymap, بنية xkb_کونچي *کونچي)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(اتحاد xkb_action) actions = darray_new();

    لكل (xkb_layout_index_t group = 0; group < کونچي->اڠک_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        إذا (کونچي->groups[group].explicit_actions)
            استمر;

        لكل (level = 0; level < Xkb_کونچي_اڠک_Levels(کونچي, group); level++) {
            أكد(کونچي->groups[group].levels[level].اڠک_actions == 0);

            ثابت بنية xkb_sym_interpret **interp_iter;
            ثابت بنية xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            ثابت bool found = Find_Interp_For_کونچي(keymap, کونچي, group, level, &interprets);
            إذا (!found)
                استمر;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                إذا (group == 0 && level == 0 &&
                    !(کونچي->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    کونچي->repeats = true;
                }

                إذا ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    إذا (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                بدل (interp->اڠک_actions) {
                حالة 0:
                    اكسر;
                حالة 1:
                    darray_append(actions, interp->a.action);
                    اكسر;
                افتراضي:
                    darray_append_بارڠن(actions, interp->a.actions,
                                        interp->اڠک_actions);
                }
            }

            /* Copy the actions */
            إذا (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         کونچي_Name_Text(keymap->ctx, کونچي->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                کونچي->groups[group].levels[level].اڠک_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } وإلا {
                کونچي->groups[group].levels[level].اڠک_actions =
                    (xkb_action_کيراءن_t) darray_size(actions);
            }
            بدل (darray_size(actions)) {
                حالة 0:
                    کونچي->groups[group].levels[level].a.action =
                        (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                    اكسر;
                حالة 1:
                    کونچي->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    اكسر;
                افتراضي:
                    کونچي->groups[group].levels[level].a.actions =
                        memdup(darray_بارڠن(actions),
                               کونچي->groups[group].levels[level].اڠک_actions,
                               حجم(*darray_بارڠن(actions)));
                    إذا (!کونچي->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        أرجع false;
                    }
            }

            إذا (!darray_empty(actions))
                کونچي->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        إذا (کونچي->groups[group].implicit_actions)
            کونچي->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    إذا (!(کونچي->explicit & EXPLICIT_VMODMAP))
        کونچي->vmodmap = vmodmap;

    أرجع true;
}

ساكن مضمن bool
is_mod_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

ساكن مضمن bool
is_group_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ساكن فراغ
CheckMultipleActionsCategories(بنية xkb_keymap *keymap, بنية xkb_کونچي *کونچي)
{
    لكل (xkb_layout_index_t g = 0; g < کونچي->اڠک_groups; g++) {
        لكل (xkb_level_index_t l = 0; l < Xkb_کونچي_اڠک_Levels(کونچي, g); l++) {
            بنية xkb_level *level = &کونچي->groups[g].levels[l];
            إذا (level->اڠک_actions <= 1)
                استمر;
            لكل (xkb_action_کيراءن_t i = 0; i < level->اڠک_actions; i++) {
                اتحاد xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                إذا (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_کونچي))
                    استمر;
                لكل (xkb_action_کيراءن_t j = i + 1; j < level->اڠک_actions; j++) {
                    اتحاد xkb_action *action2 = &level->a.actions[j];
                    إذا (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ثابت حرف * ثابت type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, کونچي_Name_Text(keymap->ctx, کونچي->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ساكن فراغ
add_کونچي_aliases(بنية xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                بنية xkb_کونچي_alias *aliases)
{
    لكل (darray_size_t alias = min; alias <= max; alias++) {
        ثابت KeycodeMatch entry = keymap->کونچي_names[alias];
        إذا (entry.is_alias && entry.found) {
            *aliases = (بنية xkb_کونچي_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

ساكن bool
update_pending_کونچي_fields(بنية xkb_keymap_info *info, بنية xkb_کونچي *کونچي)
{
    إذا (کونچي->out_of_range_pending_group) {
        بنية pending_computation * ثابت pc = &darray_item(
            *info->pending_computations, کونچي->out_of_range_group_نومبور
        );
        إذا (!pc->computed) {
            xkb_layout_index_t group = 0;
            بدل (اوڠکاڤن_Resolve_Group(info, pc->اوڠکاڤن, true, &group, عدم)) {
            حالة PARSER_SUCCESS:
                pc->computed = true;
                pc->نيلاي = group - 1;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                أرجع (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            افتراضي:
                /* Recover from error: ignore */;
            }
        }
        کونچي->out_of_range_pending_group = false;
        کونچي->out_of_range_group_نومبور = pc->نيلاي;
    }
    أرجع true;
}

ساكن bool
update_pending_action_fields(بنية xkb_keymap_info *info,
                             xkb_keycode_t keycode, اتحاد xkb_action *act)
{
    بدل (act->type) {
    حالة ACTION_TYPE_GROUP_SET:
    حالة ACTION_TYPE_GROUP_LATCH:
    حالة ACTION_TYPE_GROUP_LOCK:
        إذا (act->group.flags & ACTION_PENDING_COMPUTATION) {
            بنية pending_computation * ثابت pc =
                &darray_item(*info->pending_computations, act->group.group);
            إذا (!pc->computed) {
                xkb_layout_index_t group = 0;
                ثابت bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                بدل (اوڠکاڤن_Resolve_Group(info, pc->اوڠکاڤن, absolute, &group, عدم)) {
                حالة PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    أرجع false;
                حالة PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    اكسر;
                افتراضي:
                    pc->computed = true;
                    إذا (absolute) {
                        pc->نيلاي = group - 1;
                    } وإلا {
                        pc->نيلاي = group;
                        إذا (pc->اوڠکاڤن->common.type == STMT_اوڠکاڤن_NEGATE)
                            pc->نيلاي = (uint32_t) (-(int32_t) pc->نيلاي);
                    }
                }
            }
            act->group.group = (int32_t) pc->نيلاي;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        أرجع true;
    حالة ACTION_TYPE_REDIRECT_کونچي: {
        إذا (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_کونچي_auto) {
            /* No auto value to set */
            أرجع true;
        } وإلا {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        أرجع true;
    }
    افتراضي:
        أرجع true;
    }
}

ساكن bool
update_pending_led_fields(بنية xkb_keymap_info *info, بنية xkb_led *led)
{
    إذا (led->pending_groups) {
        بنية pending_computation * ثابت pc =
            &darray_item(*info->pending_computations, led->groups);
        إذا (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            إذا (!اوڠکاڤن_Resolve_Group_Mask(info, pc->اوڠکاڤن, &mask, عدم)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                أرجع false;
            }
            pc->computed = true;
            pc->نيلاي = mask;
        }
        led->pending_groups = false;
        led->groups = pc->نيلاي;
    }
    أرجع true;
}

/** Indices for the array "groupIndexNames" */
تعداد {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
تعداد {
    GROUP_MASK_NAME_LAST = 3,
};

ساكن فراغ
update_auto_group_lock_on_modifier_release(بنية xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           اتحاد xkb_action * restrict action)
{
    إذا (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].اڠک_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->اڠک_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        أرجع;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ساكن bool
UpdateDerivedKeymapFields(بنية xkb_keymap_info *info)
{
    بنية xkb_keymap * restrict ثابت keymap = &info->keymap;

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
    static_assert(حجم(بنية xkb_کونچي_alias) == حجم(KeycodeMatch), "");
#وإلا
    static_assert(حجم(بنية xkb_کونچي_alias) == 2 * حجم(KeycodeMatch), "");
#endif
    darray_size_t اڠک_کونچي_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    لكل (xkb_atom_t alias = 0; alias < keymap->اڠک_کونچي_names; alias++) {
        ثابت KeycodeMatch entry = keymap->کونچي_names[alias];
        إذا (entry.is_alias && entry.found) {
            إذا (!اڠک_کونچي_aliases)
                min_alias = alias;
            max_alias = alias;
            اڠک_کونچي_aliases++;
        }
    }
    إذا (اڠک_کونچي_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ثابت darray_size_t required_space = حجم(بنية xkb_کونچي_alias)
                                           / حجم(KeycodeMatch)
                                           * اڠک_کونچي_aliases;
        أكد(اڠک_کونچي_aliases <= keymap->اڠک_کونچي_names);
        إذا (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_کونچي_aliases(keymap, min_alias, max_alias, keymap->کونچي_aliases);
            /* Shrink */
            بنية xkb_کونچي_alias * ثابت r =
                realloc(keymap->کونچي_aliases,
                        اڠک_کونچي_aliases * حجم(*keymap->کونچي_aliases));
            إذا (!r)
                أرجع false;
            keymap->کونچي_aliases = r;
        } وإلا إذا (keymap->اڠک_کونچي_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            بنية xkb_کونچي_alias * ثابت aliases = (بنية xkb_کونچي_alias *) (
                keymap->کونچي_names + max_alias + 1 +
                !is_aligned(keymap->کونچي_names + max_alias + 1, حجم(*aliases))
            );
            add_کونچي_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->کونچي_aliases, aliases,
                   اڠک_کونچي_aliases * حجم(*keymap->کونچي_aliases));
            /* Shrink */
            بنية xkb_کونچي_alias * ثابت r =
                realloc(keymap->کونچي_aliases,
                        اڠک_کونچي_aliases * حجم(*keymap->کونچي_aliases));
            إذا (!r)
                أرجع false;
            keymap->کونچي_aliases = r;
        } وإلا {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            بنية xkb_کونچي_alias * ثابت aliases = calloc(اڠک_کونچي_aliases,
                                                          حجم(*aliases));
            إذا (!aliases)
                أرجع false;
            add_کونچي_aliases(keymap, min_alias, max_alias, aliases);
            حرر(keymap->کونچي_names);
            keymap->کونچي_aliases = aliases;
        }
    }
    keymap->اڠک_کونچي_aliases = اڠک_کونچي_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    بنية xkb_کونچي *کونچي;
    xkb_کونچي_سموا_foreach(کونچي, keymap)
        keymap->اڠک_groups = MAX(keymap->اڠک_groups, کونچي->اڠک_groups);

    ثابت bool pending_computations = !darray_empty(*info->pending_computations);

    إذا (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ثابت xkb_layout_index_t اڠک_groups
            = (keymap->اڠک_groups)
            ? keymap->اڠک_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .نيلاي = اڠک_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .نيلاي = UINT32_C(1) << (اڠک_groups - 1)
            };

        /* Update interpret fields with pending computations */
        لكل (darray_size_t i = 0; i < keymap->اڠک_sym_interprets; i++) {
            بنية xkb_sym_interpret * restrict ثابت interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                إذا (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    أرجع false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_کونچي_سموا_foreach(کونچي, keymap) {
        إذا (!Apply_Interps_To_کونچي(keymap, کونچي))
            أرجع false;
        CheckMultipleActionsCategories(keymap, کونچي);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    بنية xkb_mod *mod;
    xkb_کونچي_سموا_foreach(کونچي, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            إذا (کونچي->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= کونچي->modmap;

    إذا (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            ثابت xkb_mod_mask_t mask = UINT32_C(1) << idx;
            إذا (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    أكد(keymap->canonical_state_mask == MOD_REAL_MASK_ALL);
    xkb_mod_mask_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_mask |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    لكل (darray_size_t i = 0; i < keymap->اڠک_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        لكل (darray_size_t j = 0; j < keymap->types[i].اڠک_entries; j++) {
            إذا (has_unbound_vmods(keymap,
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
                استمر;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_کونچي_سموا_foreach(کونچي, keymap) {
        إذا (!update_pending_کونچي_fields(info, کونچي))
            أرجع false;
        لكل (xkb_layout_index_t i = 0; i < کونچي->اڠک_groups; i++) {
            لكل (xkb_level_index_t j = 0; j < Xkb_کونچي_اڠک_Levels(کونچي, i); j++) {
                بنية xkb_level * ثابت level = &کونچي->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, کونچي->modmap);
                    إذا ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_کونچي) &&
                        !update_pending_action_fields(info, کونچي->keycode, act))
                    {
                        أرجع false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        کونچي->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    بنية xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        إذا (pending_computations &&
            !update_pending_led_fields(info, led))
            أرجع false;
    }

    أرجع true;
}

تعريفنوع bool (*compile_file_fn)(XkbFile *file, بنية xkb_keymap_info *info);

ساكن ثابت compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_کونچي_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ساكن فراغ
pending_computations_array_free(pending_computation_array *p)
{
    بنية pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->اوڠکاڤن);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, بنية xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { عدم };
    تعداد xkb_file_type type;
    بنية xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    لكل (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        إذا (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            إذا (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } وإلا {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            استمر;
        }

        إذا (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            استمر;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    بنية xkb_keymap_info info = {
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
                    keymap->اڠک_groups ? GROUP_LAST_INDEX_NAME : عدم,
                    keymap->اڠک_groups /* 1-indexed */
                },
                { عدم, 0 }
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
                    keymap->اڠک_groups ? GROUP_LAST_INDEX_NAME : عدم,
                    /* Be extra cautious */
                    keymap->اڠک_groups && keymap->اڠک_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->اڠک_groups - 1))
                        : 0
                },
                { عدم, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    لكل (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        إذا (files[type] == عدم) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } وإلا {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ثابت bool ok = compile_file_fns[type](files[type], &info);
        إذا (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            أرجع false;
        }
    }

    ثابت bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    أرجع ok;
}
