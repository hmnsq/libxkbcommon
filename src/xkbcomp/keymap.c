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
has_unbound_vmods(संरचना xkb_keymap *keymap, xkb_mod_आवरणम्_t आवरणम्)
{
    xkb_mod_सूचकांक_t k;
    संरचना xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        यदि ((आवरणम् & (UINT32_C(1) << k)) && mod->mapping == 0)
            प्रत्यागम true;
    प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य
Compute_Effective_आवरणम्(संरचना xkb_keymap *keymap, संरचना xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ध्रुव xkb_mod_आवरणम्_t unknown_mods = ~(xkb_mod_आवरणम्_t)
        ((UINT64_C(1) << keymap->mods.अङ्कः_mods) - UINT64_C(1));
    mods->आवरणम् = mod_आवरणम्_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

स्थिर शून्य
UpdateActionMods(संरचना xkb_keymap *keymap, सङ्घ xkb_action *act,
                 xkb_mod_आवरणम्_t modmap)
{
    विकल्प (act->type) {
    पक्ष ACTION_TYPE_MOD_समुच्चयः:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_तालकम्:
        यदि (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_आवरणम्(keymap, &act->mods.mods);
        भङ्ग;
    स्वभाव:
        भङ्ग;
    }
}

स्थिर ध्रुव संरचना xkb_sym_interpret default_interpret = {
    .sym = XKB_कुञ्चिका_No_चिह्नम् /* unused */,
    .मेलनम् = मेलनम्_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_कुञ्चिका_REPEAT,
    .virtual_mod = (xkb_mod_सूचकांक_t) DEFAULT_INTERPRET_VMOD,
    .अङ्कः_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

प्रकारनिर्देश darray(ध्रुव संरचना xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
स्थिर bool
Find_Interp_For_कुञ्चिका(संरचना xkb_keymap *keymap, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                 xkb_layout_सूचकांक_t गणः, xkb_level_सूचकांक_t level,
                 xkb_sym_interprets *interprets)
{
    ध्रुव xkb_keysym_t *syms;
    पूर्णाङ्क अङ्कः_syms;

    अङ्कः_syms = xkb_keymap_कुञ्चिका_get_syms_by_level(keymap, कुञ्चिका->keycode, गणः,
                                                level, &syms);
    यदि (अङ्कः_syms <= 0)
        प्रत्यागम false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    प्रति (पूर्णाङ्क s = 0; s < अङ्कः_syms; s++) {
        bool प्राप्तम् = false;
        प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_sym_interprets; i++) {
            संरचना xkb_sym_interpret * ध्रुव interp = &keymap->sym_interprets[i];
            xkb_mod_आवरणम्_t mods;

            प्राप्तम् = false;

            यदि (interp->sym != syms[s] && interp->sym != XKB_कुञ्चिका_No_चिह्नम्)
                अनुवर्त;

            यदि (interp->level_one_only && level != 0)
                mods = 0;
            अन्यथा
                mods = कुञ्चिका->modmap;

            विकल्प (interp->मेलनम्) {
            पक्ष मेलनम्_NONE:
                प्राप्तम् = !(interp->mods & mods);
                भङ्ग;
            पक्ष मेलनम्_ANY_OR_NONE:
                प्राप्तम् = (!mods || (interp->mods & mods));
                भङ्ग;
            पक्ष मेलनम्_ANY:
                प्राप्तम् = (interp->mods & mods);
                भङ्ग;
            पक्ष मेलनम्_ALL:
                प्राप्तम् = ((interp->mods & mods) == interp->mods);
                भङ्ग;
            पक्ष मेलनम्_EXACTLY:
                प्राप्तम् = (interp->mods == mods);
                भङ्ग;
            }

            यदि (प्राप्तम् && i > 0 && interp->sym == XKB_कुञ्चिका_No_चिह्नम्) {
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
                        प्राप्तम् = false;
                        log_warn(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, KeysymText(keymap->ctx, syms[s]),
                                 level + 1, गणः + 1,
                                 कुञ्चिका_Name_Text(keymap->ctx, कुञ्चिका->name));
                        गच्छ not_प्राप्तम्;
                    }
                }
            }
            यदि (प्राप्तम्) {
                darray_append(*interprets, interp);
                interp->required = true;
                भङ्ग;
            }
        }
        यदि (!प्राप्तम्)
not_प्राप्तम्:
            darray_append(*interprets, &default_interpret);
    }
    प्रत्यागम true;
}

स्थिर bool
Apply_Interps_गन्तव्यम्_कुञ्चिका(संरचना xkb_keymap *keymap, संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    xkb_mod_आवरणम्_t vmodmap = 0;
    xkb_level_सूचकांक_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(सङ्घ xkb_action) actions = darray_new();

    प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
        /* Skip any interpretation for this group if it has explicit actions */
        यदि (कुञ्चिका->groups[गणः].explicit_actions)
            अनुवर्त;

        प्रति (level = 0; level < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, गणः); level++) {
            प्रतिज्ञा(कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions == 0);

            ध्रुव संरचना xkb_sym_interpret **interp_iter;
            ध्रुव संरचना xkb_sym_interpret *interp;
            आकार_t k;
            darray_resize(interprets, 0);

            ध्रुव bool प्राप्तम् = Find_Interp_For_कुञ्चिका(keymap, कुञ्चिका, गणः, level, &interprets);
            यदि (!प्राप्तम्)
                अनुवर्त;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                यदि (गणः == 0 && level == 0 &&
                    !(कुञ्चिका->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    कुञ्चिका->repeats = true;
                }

                यदि ((गणः == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    यदि (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                विकल्प (interp->अङ्कः_actions) {
                पक्ष 0:
                    भङ्ग;
                पक्ष 1:
                    darray_append(actions, interp->a.action);
                    भङ्ग;
                स्वभाव:
                    darray_append_वस्तूनि(actions, interp->a.actions,
                                        interp->अङ्कः_actions);
                }
            }

            /* Copy the actions */
            यदि (unlikely(darray_आकार(actions)) > MAX_ACTIONS_प्रत्येकम्_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         कुञ्चिका_Name_Text(keymap->ctx, कुञ्चिका->name),
                         MAX_ACTIONS_प्रत्येकम्_LEVEL, darray_आकार(actions));
                कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions =
                    MAX_ACTIONS_प्रत्येकम्_LEVEL;
            } अन्यथा {
                कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions =
                    (xkb_action_गणनम्_t) darray_आकार(actions);
            }
            विकल्प (darray_आकार(actions)) {
                पक्ष 0:
                    कुञ्चिका->groups[गणः].स्तराः[level].a.action =
                        (सङ्घ xkb_action) { .type = ACTION_TYPE_NONE };
                    भङ्ग;
                पक्ष 1:
                    कुञ्चिका->groups[गणः].स्तराः[level].a.action =
                        darray_item(actions, 0);
                    भङ्ग;
                स्वभाव:
                    कुञ्चिका->groups[गणः].स्तराः[level].a.actions =
                        memdup(darray_वस्तूनि(actions),
                               कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions,
                               परिमाण(*darray_वस्तूनि(actions)));
                    यदि (!कुञ्चिका->groups[गणः].स्तराः[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        प्रत्यागम false;
                    }
            }

            यदि (!darray_रिक्तानि(actions))
                कुञ्चिका->groups[गणः].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        यदि (कुञ्चिका->groups[गणः].implicit_actions)
            कुञ्चिका->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    यदि (!(कुञ्चिका->explicit & EXPLICIT_VMODMAP))
        कुञ्चिका->vmodmap = vmodmap;

    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
is_mod_action(सङ्घ xkb_action *action)
{
    प्रत्यागम action->type == ACTION_TYPE_MOD_समुच्चयः ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_तालकम्;
}

स्थिर अन्तःस्थ bool
is_गणः_action(सङ्घ xkb_action *action)
{
    प्रत्यागम action->type == ACTION_TYPE_गणः_समुच्चयः ||
           action->type == ACTION_TYPE_गणः_LATCH ||
           action->type == ACTION_TYPE_गणः_तालकम्;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
स्थिर शून्य
Check_गुणजः_Actions_Categories(संरचना xkb_keymap *keymap, संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    प्रति (xkb_layout_सूचकांक_t g = 0; g < कुञ्चिका->अङ्कः_groups; g++) {
        प्रति (xkb_level_सूचकांक_t l = 0; l < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, g); l++) {
            संरचना xkb_level *level = &कुञ्चिका->groups[g].स्तराः[l];
            यदि (level->अङ्कः_actions <= 1)
                अनुवर्त;
            प्रति (xkb_action_गणनम्_t i = 0; i < level->अङ्कः_actions; i++) {
                सङ्घ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool गणः_action = is_गणः_action(action1);
                यदि (!(mod_action || गणः_action ||
                      action1->type == ACTION_TYPE_REDIRECT_कुञ्चिका))
                    अनुवर्त;
                प्रति (xkb_action_गणनम्_t j = i + 1; j < level->अङ्कः_actions; j++) {
                    सङ्घ xkb_action *action2 = &level->a.actions[j];
                    यदि (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (गणः_action && is_गणः_action(action2)))
                    {
                        ध्रुव वर्ण * ध्रुव type = (mod_action)
                            ? "modifiers"
                            : (गणः_action
                                ? "group"
                                : ActionTypeText(action1->type));
                        log_err(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, कुञ्चिका_Name_Text(keymap->ctx, कुञ्चिका->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

स्थिर शून्य
सङ्कलय_कुञ्चिका_aliases(संरचना xkb_keymap *keymap, darray_आकार_t min, darray_आकार_t max,
                संरचना xkb_कुञ्चिका_alias *aliases)
{
    प्रति (darray_आकार_t alias = min; alias <= max; alias++) {
        ध्रुव Keycode_मेलनम् प्रविष्टिः = keymap->कुञ्चिका_नामानि[alias];
        यदि (प्रविष्टिः.is_alias && प्रविष्टिः.प्राप्तम्) {
            *aliases = (संरचना xkb_कुञ्चिका_alias) {
                .alias = alias,
                .real = प्रविष्टिः.alias.real
            };
            aliases++;
        }
    }
}

स्थिर bool
update_pending_कुञ्चिका_fields(संरचना xkb_keymap_info *info, संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    यदि (कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_pending_गणः) {
        संरचना pending_computation * ध्रुव pc = &darray_item(
            *info->pending_computations, कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या
        );
        यदि (!pc->computed) {
            xkb_layout_सूचकांक_t गणः = 0;
            विकल्प (व्यञ्जकम्_Resolve_गणः(info, pc->व्यञ्जकम्, true, &गणः, रिक्त)) {
            पक्ष PARSER_SUCCESS:
                pc->computed = true;
                pc->मानम् = गणः - 1;
                भङ्ग;
            पक्ष PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid key redirect group index\n");
                प्रत्यागम (info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH);
            स्वभाव:
                /* Recover from error: ignore */;
            }
        }
        कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_pending_गणः = false;
        कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = pc->मानम्;
    }
    प्रत्यागम true;
}

स्थिर bool
update_pending_action_fields(संरचना xkb_keymap_info *info,
                             xkb_keycode_t keycode, सङ्घ xkb_action *act)
{
    विकल्प (act->type) {
    पक्ष ACTION_TYPE_गणः_समुच्चयः:
    पक्ष ACTION_TYPE_गणः_LATCH:
    पक्ष ACTION_TYPE_गणः_तालकम्:
        यदि (act->गणः.flags & ACTION_PENDING_COMPUTATION) {
            संरचना pending_computation * ध्रुव pc =
                &darray_item(*info->pending_computations, act->गणः.गणः);
            यदि (!pc->computed) {
                xkb_layout_सूचकांक_t गणः = 0;
                ध्रुव bool absolute =
                    (act->गणः.flags & ACTION_ABSOLUTE_SWITCH);
                विकल्प (व्यञ्जकम्_Resolve_गणः(info, pc->व्यञ्जकम्, absolute, &गणः, रिक्त)) {
                पक्ष PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                            "Invalid action group index\n");
                    प्रत्यागम false;
                पक्ष PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    भङ्ग;
                स्वभाव:
                    pc->computed = true;
                    यदि (absolute) {
                        pc->मानम् = गणः - 1;
                    } अन्यथा {
                        pc->मानम् = गणः;
                        यदि (pc->व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_NEGATE)
                            pc->मानम् = (uint32_t) (-(int32_t) pc->मानम्);
                    }
                }
            }
            act->गणः.गणः = (int32_t) pc->मानम्;
            act->गणः.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        प्रत्यागम true;
    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका: {
        यदि (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_कुञ्चिका_auto) {
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
            xkb_layout_आवरणम्_t आवरणम् = 0;
            यदि (!व्यञ्जकम्_Resolve_गणः_आवरणम्(info, pc->व्यञ्जकम्, &आवरणम्, रिक्त)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid LED group mask\n");
                प्रत्यागम false;
            }
            pc->computed = true;
            pc->मानम् = आवरणम्;
        }
        led->pending_groups = false;
        led->groups = pc->मानम्;
    }
    प्रत्यागम true;
}

/** Indices for the array "groupIndexNames" */
गणना {
    गणः_सूचकांक_NAME_चरमः = 1,
};

/** Indices for the array "groupMaskNames" */
गणना {
    गणः_आवरणम्_NAME_चरमः = 3,
};

स्थिर शून्य
update_auto_गणः_तालकम्_on_modifier_release(संरचना xkb_level * सीमित level0,
                                           xkb_level_सूचकांक_t l,
                                           सङ्घ xkb_action * सीमित action)
{
    यदि (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].अङ्कः_actions == 1 &&
        action->type == ACTION_TYPE_गणः_तालकम् &&
        /* lockOnRelease not set explicitly */
        !(action->गणः.flags & ACTION_EXPLICIT_तालकम्_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->अङ्कः_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_समुच्चयः))
    {
        प्रत्यागम;
    }

    action->गणः.flags |= ACTION_AUTO_गणः_तालकम्_ON_MODIFIER_RELEASE;
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
    संरचना xkb_keymap * सीमित ध्रुव keymap = &info->keymap;

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
#यदि_परिभाषितम् _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(परिमाण(संरचना xkb_कुञ्चिका_alias) == परिमाण(Keycode_मेलनम्), "");
#अन्यथा
    static_assert(परिमाण(संरचना xkb_कुञ्चिका_alias) == 2 * परिमाण(Keycode_मेलनम्), "");
#यदि_अन्तः
    darray_आकार_t अङ्कः_कुञ्चिका_aliases = 0;
    darray_आकार_t min_alias = 0;
    darray_आकार_t max_alias = 0;
    प्रति (xkb_atom_t alias = 0; alias < keymap->अङ्कः_कुञ्चिका_नामानि; alias++) {
        ध्रुव Keycode_मेलनम् प्रविष्टिः = keymap->कुञ्चिका_नामानि[alias];
        यदि (प्रविष्टिः.is_alias && प्रविष्टिः.प्राप्तम्) {
            यदि (!अङ्कः_कुञ्चिका_aliases)
                min_alias = alias;
            max_alias = alias;
            अङ्कः_कुञ्चिका_aliases++;
        }
    }
    यदि (अङ्कः_कुञ्चिका_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ध्रुव darray_आकार_t required_space = परिमाण(संरचना xkb_कुञ्चिका_alias)
                                           / परिमाण(Keycode_मेलनम्)
                                           * अङ्कः_कुञ्चिका_aliases;
        प्रतिज्ञा(अङ्कः_कुञ्चिका_aliases <= keymap->अङ्कः_कुञ्चिका_नामानि);
        यदि (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            सङ्कलय_कुञ्चिका_aliases(keymap, min_alias, max_alias, keymap->कुञ्चिका_aliases);
            /* Shrink */
            संरचना xkb_कुञ्चिका_alias * ध्रुव r =
                realloc(keymap->कुञ्चिका_aliases,
                        अङ्कः_कुञ्चिका_aliases * परिमाण(*keymap->कुञ्चिका_aliases));
            यदि (!r)
                प्रत्यागम false;
            keymap->कुञ्चिका_aliases = r;
        } अन्यथा यदि (keymap->अङ्कः_कुञ्चिका_नामानि - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            संरचना xkb_कुञ्चिका_alias * ध्रुव aliases = (संरचना xkb_कुञ्चिका_alias *) (
                keymap->कुञ्चिका_नामानि + max_alias + 1 +
                !is_aligned(keymap->कुञ्चिका_नामानि + max_alias + 1, परिमाण(*aliases))
            );
            सङ्कलय_कुञ्चिका_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            स्मृतिं_प्रतिलिख(keymap->कुञ्चिका_aliases, aliases,
                   अङ्कः_कुञ्चिका_aliases * परिमाण(*keymap->कुञ्चिका_aliases));
            /* Shrink */
            संरचना xkb_कुञ्चिका_alias * ध्रुव r =
                realloc(keymap->कुञ्चिका_aliases,
                        अङ्कः_कुञ्चिका_aliases * परिमाण(*keymap->कुञ्चिका_aliases));
            यदि (!r)
                प्रत्यागम false;
            keymap->कुञ्चिका_aliases = r;
        } अन्यथा {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            संरचना xkb_कुञ्चिका_alias * ध्रुव aliases = शून्यस्मृतिं_देहि(अङ्कः_कुञ्चिका_aliases,
                                                          परिमाण(*aliases));
            यदि (!aliases)
                प्रत्यागम false;
            सङ्कलय_कुञ्चिका_aliases(keymap, min_alias, max_alias, aliases);
            मुक्त(keymap->कुञ्चिका_नामानि);
            keymap->कुञ्चिका_aliases = aliases;
        }
    }
    keymap->अङ्कः_कुञ्चिका_aliases = अङ्कः_कुञ्चिका_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    संरचना xkb_कुञ्चिका *कुञ्चिका;
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap)
        keymap->अङ्कः_groups = MAX(keymap->अङ्कः_groups, कुञ्चिका->अङ्कः_groups);

    ध्रुव bool pending_computations = !darray_रिक्तानि(*info->pending_computations);

    यदि (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ध्रुव xkb_layout_सूचकांक_t अङ्कः_groups
            = (keymap->अङ्कः_groups)
            ? keymap->अङ्कः_groups
            : 1; /* default to the “first” layout */
        info->lookup.गणः_सूचकांक_नामानि[गणः_सूचकांक_NAME_चरमः] =
            (Lookup_प्रविष्टिः) {
                .name = गणः_चरमः_सूचकांक_NAME,
                .मानम् = अङ्कः_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.गणः_आवरणम्_नामानि[गणः_आवरणम्_NAME_चरमः] =
            (Lookup_प्रविष्टिः) {
                .name = गणः_चरमः_सूचकांक_NAME,
                .मानम् = UINT32_C(1) << (अङ्कः_groups - 1)
            };

        /* Update interpret fields with pending computations */
        प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_sym_interprets; i++) {
            संरचना xkb_sym_interpret * सीमित ध्रुव interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                यदि (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    प्रत्यागम false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        यदि (!Apply_Interps_गन्तव्यम्_कुञ्चिका(keymap, कुञ्चिका))
            प्रत्यागम false;
        Check_गुणजः_Actions_Categories(keymap, कुञ्चिका);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_सूचकांक_t idx;
    संरचना xkb_mod *mod;
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            यदि (कुञ्चिका->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= कुञ्चिका->modmap;

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
            ध्रुव xkb_mod_आवरणम्_t आवरणम् = UINT32_C(1) << idx;
            यदि (mod->mapping == 0 && !(keymap->mods.explicit_vmods & आवरणम्)) {
                mod->mapping = आवरणम्;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= आवरणम्;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    प्रतिज्ञा(keymap->canonical_state_आवरणम् == MOD_REAL_आवरणम्_ALL);
    xkb_mod_आवरणम्_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_आवरणम् |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_types; i++) {
        Compute_Effective_आवरणम्(keymap, &keymap->types[i].mods);

        प्रति (darray_आकार_t j = 0; j < keymap->types[i].अङ्कः_entries; j++) {
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
                keymap->types[i].entries[j].mods.आवरणम् = 0;
                अनुवर्त;
            }
            Compute_Effective_आवरणम्(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_आवरणम्(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        यदि (!update_pending_कुञ्चिका_fields(info, कुञ्चिका))
            प्रत्यागम false;
        प्रति (xkb_layout_सूचकांक_t i = 0; i < कुञ्चिका->अङ्कः_groups; i++) {
            प्रति (xkb_level_सूचकांक_t j = 0; j < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, i); j++) {
                संरचना xkb_level * ध्रुव level = &कुञ्चिका->groups[i].स्तराः[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, कुञ्चिका->modmap);
                    यदि ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_कुञ्चिका) &&
                        !update_pending_action_fields(info, कुञ्चिका->keycode, act))
                    {
                        प्रत्यागम false;
                    }
                    update_auto_गणः_तालकम्_on_modifier_release(
                        कुञ्चिका->groups[i].स्तराः, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    संरचना xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_आवरणम्(keymap, &led->mods); /* vmod -> led maps */
        यदि (pending_computations &&
            !update_pending_led_fields(info, led))
            प्रत्यागम false;
    }

    प्रत्यागम true;
}

प्रकारनिर्देश bool (*compile_file_fn)(XkbFile *file, संरचना xkb_keymap_info *info);

स्थिर ध्रुव compile_file_fn compile_file_fns[चरमः_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_कुञ्चिका_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

स्थिर शून्य
pending_computations_array_free(pending_computation_array *p)
{
    संरचना pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->व्यञ्जकम्);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, संरचना xkb_keymap *keymap)
{
    XkbFile *files[चरमः_KEYMAP_FILE_TYPE + 1] = { रिक्त };
    गणना xkb_file_type type;
    संरचना xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    प्रति (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.अग्र) {
        यदि (file->file_type < FIRST_KEYMAP_FILE_TYPE ||
            file->file_type > चरमः_KEYMAP_FILE_TYPE) {
            यदि (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } अन्यथा {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_गन्तव्यम्_string(file->file_type));
            }
            अनुवर्त;
        }

        यदि (files[file->file_type]) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_गन्तव्यम्_string(file->file_type));
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
            .controls_name_विस्थापनम् = format_control_नामानि_विस्थापनम्(keymap->format),
            .गणः_तालकम्_on_release =
                is_गणः_तालकम्_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_तालकम्_On_Press_Supported(keymap->format),
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
            .गणः_सूचकांक_नामानि = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [गणः_सूचकांक_NAME_चरमः] = {
                    keymap->अङ्कः_groups ? गणः_चरमः_सूचकांक_NAME : रिक्त,
                    keymap->अङ्कः_groups /* 1-indexed */
                },
                { रिक्त, 0 }
            },
            .गणः_आवरणम्_नामानि = {
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
                [गणः_आवरणम्_NAME_चरमः] = {
                    keymap->अङ्कः_groups ? गणः_चरमः_सूचकांक_NAME : रिक्त,
                    /* Be extra cautious */
                    keymap->अङ्कः_groups && keymap->अङ्कः_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->अङ्कः_groups - 1))
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
         type <= चरमः_KEYMAP_FILE_TYPE;
         type++) {
        यदि (files[type] == रिक्त) {
            log_dbg(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_गन्तव्यम्_string(type));
        } अन्यथा {
            log_dbg(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_गन्तव्यम्_string(type),
                    safe_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ध्रुव bool ok = compile_file_fns[type](files[type], &info);
        यदि (!ok) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_गन्तव्यम्_string(type));
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
