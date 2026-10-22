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

#شامل "config.h"

#شامل <string.h>
#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "ast-build.h"
#شامل "darray.h"
#شامل "expr.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "utils.h"
#شامل "xkbcomp-priv.h"

ایستا bool
has_unbound_vmods(ساختار xkb_keymap *keymap, xkb_mod_mask_t mask)
{
    xkb_mod_index_t k;
    ساختار xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        اگر ((mask & (UINT32_C(1) << k)) && mod->mapping == 0)
            بازگشت true;
    بازگشت false;
}

ایستا درونخطی تهی
ComputeEffectiveMask(ساختار xkb_keymap *keymap, ساختار xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ثابت xkb_mod_mask_t unknown_mods = ~(xkb_mod_mask_t)
        ((UINT64_C(1) << keymap->mods.رقم_mods) - UINT64_C(1));
    mods->mask = mod_mask_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ایستا تهی
UpdateActionMods(ساختار xkb_keymap *keymap, اجتماع xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    انتخاب (act->type) {
    حالت ACTION_TYPE_MOD_SET:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_LOCK:
        اگر (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        بشکن;
    پیشفرض:
        بشکن;
    }
}

ایستا ثابت ساختار xkb_sym_interpret default_interpret = {
    .sym = XKB_کلید_No_نماد /* unused */,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_کلید_REPEAT,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .رقم_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

تعریفنوع darray(ثابت ساختار xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ایستا bool
Find_Interp_For_کلید(ساختار xkb_keymap *keymap, ثابت ساختار xkb_کلید *کلید,
                 xkb_layout_index_t group, xkb_level_index_t level,
                 xkb_sym_interprets *interprets)
{
    ثابت xkb_keysym_t *syms;
    صحیح رقم_syms;

    رقم_syms = xkb_keymap_کلید_get_syms_by_level(keymap, کلید->keycode, group,
                                                level, &syms);
    اگر (رقم_syms <= 0)
        بازگشت false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    برای (صحیح s = 0; s < رقم_syms; s++) {
        bool found = false;
        برای (darray_size_t i = 0; i < keymap->رقم_sym_interprets; i++) {
            ساختار xkb_sym_interpret * ثابت interp = &keymap->sym_interprets[i];
            xkb_mod_mask_t mods;

            found = false;

            اگر (interp->sym != syms[s] && interp->sym != XKB_کلید_No_نماد)
                ادامه;

            اگر (interp->level_one_only && level != 0)
                mods = 0;
            وگرنه
                mods = کلید->modmap;

            انتخاب (interp->match) {
            حالت MATCH_NONE:
                found = !(interp->mods & mods);
                بشکن;
            حالت MATCH_ANY_OR_NONE:
                found = (!mods || (interp->mods & mods));
                بشکن;
            حالت MATCH_ANY:
                found = (interp->mods & mods);
                بشکن;
            حالت MATCH_ALL:
                found = ((interp->mods & mods) == interp->mods);
                بشکن;
            حالت MATCH_EXACTLY:
                found = (interp->mods == mods);
                بشکن;
            }

            اگر (found && i > 0 && interp->sym == XKB_کلید_No_نماد) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ساختار xkb_sym_interpret ثابت **previous_interp;
                darray_foreach(previous_interp, *interprets) {
                    اگر (*previous_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        found = false;
                        log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, group + 1,
                                 کلید_Name_Text(keymap->ctx, کلید->name));
                        برو not_found;
                    }
                }
            }
            اگر (found) {
                darray_append(*interprets, interp);
                interp->required = true;
                بشکن;
            }
        }
        اگر (!found)
not_found:
            darray_append(*interprets, &default_interpret);
    }
    بازگشت true;
}

ایستا bool
Apply_Interps_To_کلید(ساختار xkb_keymap *keymap, ساختار xkb_کلید *کلید)
{
    xkb_mod_mask_t vmodmap = 0;
    xkb_level_index_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(اجتماع xkb_action) actions = darray_new();

    برای (xkb_layout_index_t group = 0; group < کلید->رقم_groups; group++) {
        /* Skip any interpretation for this group if it has explicit actions */
        اگر (کلید->groups[group].explicit_actions)
            ادامه;

        برای (level = 0; level < Xkb_کلید_رقم_Levels(کلید, group); level++) {
            أكد(کلید->groups[group].levels[level].رقم_actions == 0);

            ثابت ساختار xkb_sym_interpret **interp_iter;
            ثابت ساختار xkb_sym_interpret *interp;
            size_t k;
            darray_resize(interprets, 0);

            ثابت bool found = Find_Interp_For_کلید(keymap, کلید, group, level, &interprets);
            اگر (!found)
                ادامه;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                اگر (group == 0 && level == 0 &&
                    !(کلید->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    کلید->repeats = true;
                }

                اگر ((group == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    اگر (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                انتخاب (interp->رقم_actions) {
                حالت 0:
                    بشکن;
                حالت 1:
                    darray_append(actions, interp->a.action);
                    بشکن;
                پیشفرض:
                    darray_append_اقلام(actions, interp->a.actions,
                                        interp->رقم_actions);
                }
            }

            /* Copy the actions */
            اگر (unlikely(darray_size(actions)) > MAX_ACTIONS_PER_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         کلید_Name_Text(keymap->ctx, کلید->name),
                         MAX_ACTIONS_PER_LEVEL, darray_size(actions));
                کلید->groups[group].levels[level].رقم_actions =
                    MAX_ACTIONS_PER_LEVEL;
            } وگرنه {
                کلید->groups[group].levels[level].رقم_actions =
                    (xkb_action_شمار_t) darray_size(actions);
            }
            انتخاب (darray_size(actions)) {
                حالت 0:
                    کلید->groups[group].levels[level].a.action =
                        (اجتماع xkb_action) { .type = ACTION_TYPE_NONE };
                    بشکن;
                حالت 1:
                    کلید->groups[group].levels[level].a.action =
                        darray_item(actions, 0);
                    بشکن;
                پیشفرض:
                    کلید->groups[group].levels[level].a.actions =
                        memdup(darray_اقلام(actions),
                               کلید->groups[group].levels[level].رقم_actions,
                               اندازه(*darray_اقلام(actions)));
                    اگر (!کلید->groups[group].levels[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        بازگشت false;
                    }
            }

            اگر (!darray_empty(actions))
                کلید->groups[group].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        اگر (کلید->groups[group].implicit_actions)
            کلید->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    اگر (!(کلید->explicit & EXPLICIT_VMODMAP))
        کلید->vmodmap = vmodmap;

    بازگشت true;
}

ایستا درونخطی bool
is_mod_action(اجتماع xkb_action *action)
{
    بازگشت action->type == ACTION_TYPE_MOD_SET ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_LOCK;
}

ایستا درونخطی bool
is_group_action(اجتماع xkb_action *action)
{
    بازگشت action->type == ACTION_TYPE_GROUP_SET ||
           action->type == ACTION_TYPE_GROUP_LATCH ||
           action->type == ACTION_TYPE_GROUP_LOCK;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ایستا تهی
CheckMultipleActionsCategories(ساختار xkb_keymap *keymap, ساختار xkb_کلید *کلید)
{
    برای (xkb_layout_index_t g = 0; g < کلید->رقم_groups; g++) {
        برای (xkb_level_index_t l = 0; l < Xkb_کلید_رقم_Levels(کلید, g); l++) {
            ساختار xkb_level *level = &کلید->groups[g].levels[l];
            اگر (level->رقم_actions <= 1)
                ادامه;
            برای (xkb_action_شمار_t i = 0; i < level->رقم_actions; i++) {
                اجتماع xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool group_action = is_group_action(action1);
                اگر (!(mod_action || group_action ||
                      action1->type == ACTION_TYPE_REDIRECT_کلید))
                    ادامه;
                برای (xkb_action_شمار_t j = i + 1; j < level->رقم_actions; j++) {
                    اجتماع xkb_action *action2 = &level->a.actions[j];
                    اگر (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (group_action && is_group_action(action2)))
                    {
                        ثابت نویسه * ثابت type = (mod_action)
                            ? "modifiers"
                            : (group_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, کلید_Name_Text(keymap->ctx, کلید->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ایستا تهی
add_کلید_aliases(ساختار xkb_keymap *keymap, darray_size_t min, darray_size_t max,
                ساختار xkb_کلید_alias *aliases)
{
    برای (darray_size_t alias = min; alias <= max; alias++) {
        ثابت KeycodeMatch entry = keymap->کلید_names[alias];
        اگر (entry.is_alias && entry.found) {
            *aliases = (ساختار xkb_کلید_alias) {
                .alias = alias,
                .real = entry.alias.real
            };
            aliases++;
        }
    }
}

ایستا bool
update_pending_کلید_fields(ساختار xkb_keymap_info *info, ساختار xkb_کلید *کلید)
{
    اگر (کلید->out_of_range_pending_group) {
        ساختار pending_computation * ثابت pc = &darray_item(
            *info->pending_computations, کلید->out_of_range_group_عدد
        );
        اگر (!pc->computed) {
            xkb_layout_index_t group = 0;
            انتخاب (عبارت_Resolve_Group(info, pc->عبارت, true, &group, پوچ)) {
            حالت PARSER_SUCCESS:
                pc->computed = true;
                pc->مقدار = group - 1;
                بشکن;
            حالت PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid key redirect group index\n");
                بازگشت (info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
            پیشفرض:
                /* Recover from error: ignore */;
            }
        }
        کلید->out_of_range_pending_group = false;
        کلید->out_of_range_group_عدد = pc->مقدار;
    }
    بازگشت true;
}

ایستا bool
update_pending_action_fields(ساختار xkb_keymap_info *info,
                             xkb_keycode_t keycode, اجتماع xkb_action *act)
{
    انتخاب (act->type) {
    حالت ACTION_TYPE_GROUP_SET:
    حالت ACTION_TYPE_GROUP_LATCH:
    حالت ACTION_TYPE_GROUP_LOCK:
        اگر (act->group.flags & ACTION_PENDING_COMPUTATION) {
            ساختار pending_computation * ثابت pc =
                &darray_item(*info->pending_computations, act->group.group);
            اگر (!pc->computed) {
                xkb_layout_index_t group = 0;
                ثابت bool absolute =
                    (act->group.flags & ACTION_ABSOLUTE_SWITCH);
                انتخاب (عبارت_Resolve_Group(info, pc->عبارت, absolute, &group, پوچ)) {
                حالت PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid action group index\n");
                    بازگشت false;
                حالت PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    بشکن;
                پیشفرض:
                    pc->computed = true;
                    اگر (absolute) {
                        pc->مقدار = group - 1;
                    } وگرنه {
                        pc->مقدار = group;
                        اگر (pc->عبارت->common.type == STMT_عبارت_NEGATE)
                            pc->مقدار = (uint32_t) (-(int32_t) pc->مقدار);
                    }
                }
            }
            act->group.group = (int32_t) pc->مقدار;
            act->group.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        بازگشت true;
    حالت ACTION_TYPE_REDIRECT_کلید: {
        اگر (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_کلید_auto) {
            /* No auto value to set */
            بازگشت true;
        } وگرنه {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        بازگشت true;
    }
    پیشفرض:
        بازگشت true;
    }
}

ایستا bool
update_pending_led_fields(ساختار xkb_keymap_info *info, ساختار xkb_led *led)
{
    اگر (led->pending_groups) {
        ساختار pending_computation * ثابت pc =
            &darray_item(*info->pending_computations, led->groups);
        اگر (!pc->computed) {
            xkb_layout_mask_t mask = 0;
            اگر (!عبارت_Resolve_Group_Mask(info, pc->عبارت, &mask, پوچ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid LED group mask\n");
                بازگشت false;
            }
            pc->computed = true;
            pc->مقدار = mask;
        }
        led->pending_groups = false;
        led->groups = pc->مقدار;
    }
    بازگشت true;
}

/** Indices for the array "groupIndexNames" */
شمارش {
    GROUP_INDEX_NAME_LAST = 1,
};

/** Indices for the array "groupMaskNames" */
شمارش {
    GROUP_MASK_NAME_LAST = 3,
};

ایستا تهی
update_auto_group_lock_on_modifier_release(ساختار xkb_level * restrict level0,
                                           xkb_level_index_t l,
                                           اجتماع xkb_action * restrict action)
{
    اگر (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].رقم_actions == 1 &&
        action->type == ACTION_TYPE_GROUP_LOCK &&
        /* lockOnRelease not set explicitly */
        !(action->group.flags & ACTION_EXPLICIT_LOCK_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->رقم_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_SET))
    {
        بازگشت;
    }

    action->group.flags |= ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ایستا bool
UpdateDerivedKeymapFields(ساختار xkb_keymap_info *info)
{
    ساختار xkb_keymap * restrict ثابت keymap = &info->keymap;

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
    static_assert(اندازه(ساختار xkb_کلید_alias) == اندازه(KeycodeMatch), "");
#وگرنه
    static_assert(اندازه(ساختار xkb_کلید_alias) == 2 * اندازه(KeycodeMatch), "");
#endif
    darray_size_t رقم_کلید_aliases = 0;
    darray_size_t min_alias = 0;
    darray_size_t max_alias = 0;
    برای (xkb_atom_t alias = 0; alias < keymap->رقم_کلید_names; alias++) {
        ثابت KeycodeMatch entry = keymap->کلید_names[alias];
        اگر (entry.is_alias && entry.found) {
            اگر (!رقم_کلید_aliases)
                min_alias = alias;
            max_alias = alias;
            رقم_کلید_aliases++;
        }
    }
    اگر (رقم_کلید_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ثابت darray_size_t required_space = اندازه(ساختار xkb_کلید_alias)
                                           / اندازه(KeycodeMatch)
                                           * رقم_کلید_aliases;
        أكد(رقم_کلید_aliases <= keymap->رقم_کلید_names);
        اگر (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            add_کلید_aliases(keymap, min_alias, max_alias, keymap->کلید_aliases);
            /* Shrink */
            ساختار xkb_کلید_alias * ثابت r =
                realloc(keymap->کلید_aliases,
                        رقم_کلید_aliases * اندازه(*keymap->کلید_aliases));
            اگر (!r)
                بازگشت false;
            keymap->کلید_aliases = r;
        } وگرنه اگر (keymap->رقم_کلید_names - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ساختار xkb_کلید_alias * ثابت aliases = (ساختار xkb_کلید_alias *) (
                keymap->کلید_names + max_alias + 1 +
                !is_aligned(keymap->کلید_names + max_alias + 1, اندازه(*aliases))
            );
            add_کلید_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            memcpy(keymap->کلید_aliases, aliases,
                   رقم_کلید_aliases * اندازه(*keymap->کلید_aliases));
            /* Shrink */
            ساختار xkb_کلید_alias * ثابت r =
                realloc(keymap->کلید_aliases,
                        رقم_کلید_aliases * اندازه(*keymap->کلید_aliases));
            اگر (!r)
                بازگشت false;
            keymap->کلید_aliases = r;
        } وگرنه {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ساختار xkb_کلید_alias * ثابت aliases = calloc(رقم_کلید_aliases,
                                                          اندازه(*aliases));
            اگر (!aliases)
                بازگشت false;
            add_کلید_aliases(keymap, min_alias, max_alias, aliases);
            آزاد(keymap->کلید_names);
            keymap->کلید_aliases = aliases;
        }
    }
    keymap->رقم_کلید_aliases = رقم_کلید_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ساختار xkb_کلید *کلید;
    xkb_کلیدها_foreach(کلید, keymap)
        keymap->رقم_groups = MAX(keymap->رقم_groups, کلید->رقم_groups);

    ثابت bool pending_computations = !darray_empty(*info->pending_computations);

    اگر (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ثابت xkb_layout_index_t رقم_groups
            = (keymap->رقم_groups)
            ? keymap->رقم_groups
            : 1; /* default to the “first” layout */
        info->lookup.groupIndexNames[GROUP_INDEX_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .مقدار = رقم_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.groupMaskNames[GROUP_MASK_NAME_LAST] =
            (LookupEntry) {
                .name = GROUP_LAST_INDEX_NAME,
                .مقدار = UINT32_C(1) << (رقم_groups - 1)
            };

        /* Update interpret fields with pending computations */
        برای (darray_size_t i = 0; i < keymap->رقم_sym_interprets; i++) {
            ساختار xkb_sym_interpret * restrict ثابت interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                اگر (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    بازگشت false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_کلیدها_foreach(کلید, keymap) {
        اگر (!Apply_Interps_To_کلید(keymap, کلید))
            بازگشت false;
        CheckMultipleActionsCategories(keymap, کلید);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_index_t idx;
    ساختار xkb_mod *mod;
    xkb_کلیدها_foreach(کلید, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            اگر (کلید->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= کلید->modmap;

    اگر (keymap->format >= XKB_KEYMAP_FORMAT_TEXT_V2) {
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
            اگر (mod->mapping == 0 && !(keymap->mods.explicit_vmods & mask)) {
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
    برای (darray_size_t i = 0; i < keymap->رقم_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        برای (darray_size_t j = 0; j < keymap->types[i].رقم_entries; j++) {
            اگر (has_unbound_vmods(keymap,
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
                ادامه;
            }
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_کلیدها_foreach(کلید, keymap) {
        اگر (!update_pending_کلید_fields(info, کلید))
            بازگشت false;
        برای (xkb_layout_index_t i = 0; i < کلید->رقم_groups; i++) {
            برای (xkb_level_index_t j = 0; j < Xkb_کلید_رقم_Levels(کلید, i); j++) {
                ساختار xkb_level * ثابت level = &کلید->groups[i].levels[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, کلید->modmap);
                    اگر ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_کلید) &&
                        !update_pending_action_fields(info, کلید->keycode, act))
                    {
                        بازگشت false;
                    }
                    update_auto_group_lock_on_modifier_release(
                        کلید->groups[i].levels, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ساختار xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ComputeEffectiveMask(keymap, &led->mods); /* vmod -> led maps */
        اگر (pending_computations &&
            !update_pending_led_fields(info, led))
            بازگشت false;
    }

    بازگشت true;
}

تعریفنوع bool (*compile_file_fn)(XkbFile *file, ساختار xkb_keymap_info *info);

ایستا ثابت compile_file_fn compile_file_fns[LAST_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_کلید_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ایستا تهی
pending_computations_array_free(pending_computation_array *p)
{
    ساختار pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->عبارت);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ساختار xkb_keymap *keymap)
{
    XkbFile *files[LAST_KEYMAP_FILE_TYPE + 1] = { پوچ };
    شمارش xkb_file_type type;
    ساختار xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    برای (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.next) {
        اگر (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > LAST_KEYMAP_FILE_TYPE) {
            اگر (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } وگرنه {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_to_string(file->file_type));
            }
            ادامه;
        }

        اگر (files[file->file_type]) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_to_string(file->file_type));
            ادامه;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ساختار xkb_keymap_info info = {
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
                    keymap->رقم_groups ? GROUP_LAST_INDEX_NAME : پوچ,
                    keymap->رقم_groups /* 1-indexed */
                },
                { پوچ, 0 }
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
                    keymap->رقم_groups ? GROUP_LAST_INDEX_NAME : پوچ,
                    /* Be extra cautious */
                    keymap->رقم_groups && keymap->رقم_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->رقم_groups - 1))
                        : 0
                },
                { پوچ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    برای (type = FIRST_KEYMAP_FILE_TYPE;
         type <= LAST_KEYMAP_FILE_TYPE;
         type++) {
        اگر (files[type] == پوچ) {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_to_string(type));
        } وگرنه {
            log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_to_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ثابت bool ok = compile_file_fns[type](files[type], &info);
        اگر (!ok) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_to_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            بازگشت false;
        }
    }

    ثابت bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    بازگشت ok;
}
