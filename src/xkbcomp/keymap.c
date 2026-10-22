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

#అన్తర్భావ "config.h"

#అన్తర్భావ <string.h>
#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "ast-build.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcomp-priv.h"

స్థిర bool
has_unbound_vmods(నిర్మాణం xkb_keymap *keymap, xkb_mod_ముసుగు_t ముసుగు)
{
    xkb_mod_సూచీ_t k;
    నిర్మాణం xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        ఒకవేళ ((ముసుగు & (UINT32_C(1) << k)) && mod->mapping == 0)
            తిరిగి true;
    తిరిగి false;
}

స్థిర అంతఃవరుస శూన్య
Compute_Effective_ముసుగు(నిర్మాణం xkb_keymap *keymap, నిర్మాణం xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    స్థిరరాశి xkb_mod_ముసుగు_t unknown_mods = ~(xkb_mod_ముసుగు_t)
        ((UINT64_C(1) << keymap->mods.అంకె_mods) - UINT64_C(1));
    mods->ముసుగు = mod_ముసుగు_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

స్థిర శూన్య
UpdateActionMods(నిర్మాణం xkb_keymap *keymap, సంఘం xkb_action *act,
                 xkb_mod_ముసుగు_t modmap)
{
    ఎంపిక (act->type) {
    స్థితి ACTION_TYPE_MOD_సమితి:
    స్థితి ACTION_TYPE_MOD_LATCH:
    స్థితి ACTION_TYPE_MOD_తాళం:
        ఒకవేళ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_ముసుగు(keymap, &act->mods.mods);
        ఆపు;
    అప్రమేయ:
        ఆపు;
    }
}

స్థిర స్థిరరాశి నిర్మాణం xkb_sym_interpret default_interpret = {
    .sym = XKB_కీలకం_No_చిహ్నం /* unused */,
    .సరిపోలిక = సరిపోలిక_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_కీలకం_REPEAT,
    .virtual_mod = (xkb_mod_సూచీ_t) DEFAULT_INTERPRET_VMOD,
    .అంకె_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

రకనిర్వచనం darray(స్థిరరాశి నిర్మాణం xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
స్థిర bool
కనుగొను_Interp_For_కీలకం(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                 xkb_layout_సూచీ_t సమూహం, xkb_level_సూచీ_t level,
                 xkb_sym_interprets *interprets)
{
    స్థిరరాశి xkb_keysym_t *syms;
    పూర్ణాంకం అంకె_syms;

    అంకె_syms = xkb_keymap_కీలకం_get_syms_by_level(keymap, కీలకం->keycode, సమూహం,
                                                level, &syms);
    ఒకవేళ (అంకె_syms <= 0)
        తిరిగి false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    కొరకు (పూర్ణాంకం s = 0; s < అంకె_syms; s++) {
        bool దొరికింది = false;
        కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_sym_interprets; i++) {
            నిర్మాణం xkb_sym_interpret * స్థిరరాశి interp = &keymap->sym_interprets[i];
            xkb_mod_ముసుగు_t mods;

            దొరికింది = false;

            ఒకవేళ (interp->sym != syms[s] && interp->sym != XKB_కీలకం_No_చిహ్నం)
                కొనసాగు;

            ఒకవేళ (interp->level_one_only && level != 0)
                mods = 0;
            లేకపోతే
                mods = కీలకం->modmap;

            ఎంపిక (interp->సరిపోలిక) {
            స్థితి సరిపోలిక_NONE:
                దొరికింది = !(interp->mods & mods);
                ఆపు;
            స్థితి సరిపోలిక_ANY_OR_NONE:
                దొరికింది = (!mods || (interp->mods & mods));
                ఆపు;
            స్థితి సరిపోలిక_ANY:
                దొరికింది = (interp->mods & mods);
                ఆపు;
            స్థితి సరిపోలిక_ALL:
                దొరికింది = ((interp->mods & mods) == interp->mods);
                ఆపు;
            స్థితి సరిపోలిక_EXACTLY:
                దొరికింది = (interp->mods == mods);
                ఆపు;
            }

            ఒకవేళ (దొరికింది && i > 0 && interp->sym == XKB_కీలకం_No_చిహ్నం) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                నిర్మాణం xkb_sym_interpret స్థిరరాశి **మునుపటి_interp;
                darray_foreach(మునుపటి_interp, *interprets) {
                    ఒకవేళ (*మునుపటి_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        దొరికింది = false;
                        log_warn(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_పాఠ్యం(keymap->ctx, syms[s]),
                                 level + 1, సమూహం + 1,
                                 కీలకం_Name_పాఠ్యం(keymap->ctx, కీలకం->name));
                        వెళ్ళు not_దొరికింది;
                    }
                }
            }
            ఒకవేళ (దొరికింది) {
                darray_append(*interprets, interp);
                interp->required = true;
                ఆపు;
            }
        }
        ఒకవేళ (!దొరికింది)
not_దొరికింది:
            darray_append(*interprets, &default_interpret);
    }
    తిరిగి true;
}

స్థిర bool
Apply_Interps_గమ్యం_కీలకం(నిర్మాణం xkb_keymap *keymap, నిర్మాణం xkb_కీలకం *కీలకం)
{
    xkb_mod_ముసుగు_t vmodmap = 0;
    xkb_level_సూచీ_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(సంఘం xkb_action) actions = darray_new();

    కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
        /* Skip any interpretation for this group if it has explicit actions */
        ఒకవేళ (కీలకం->groups[సమూహం].explicit_actions)
            కొనసాగు;

        కొరకు (level = 0; level < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, సమూహం); level++) {
            ప్రతిజ్ఞా(కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions == 0);

            స్థిరరాశి నిర్మాణం xkb_sym_interpret **interp_iter;
            స్థిరరాశి నిర్మాణం xkb_sym_interpret *interp;
            కొలత_t k;
            darray_resize(interprets, 0);

            స్థిరరాశి bool దొరికింది = కనుగొను_Interp_For_కీలకం(keymap, కీలకం, సమూహం, level, &interprets);
            ఒకవేళ (!దొరికింది)
                కొనసాగు;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                ఒకవేళ (సమూహం == 0 && level == 0 &&
                    !(కీలకం->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    కీలకం->repeats = true;
                }

                ఒకవేళ ((సమూహం == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    ఒకవేళ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                ఎంపిక (interp->అంకె_actions) {
                స్థితి 0:
                    ఆపు;
                స్థితి 1:
                    darray_append(actions, interp->a.action);
                    ఆపు;
                అప్రమేయ:
                    darray_append_వస్తువులు(actions, interp->a.actions,
                                        interp->అంకె_actions);
                }
            }

            /* Copy the actions */
            ఒకవేళ (unlikely(darray_కొలత(actions)) > MAX_ACTIONS_చొప్పున_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         కీలకం_Name_పాఠ్యం(keymap->ctx, కీలకం->name),
                         MAX_ACTIONS_చొప్పున_LEVEL, darray_కొలత(actions));
                కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions =
                    MAX_ACTIONS_చొప్పున_LEVEL;
            } లేకపోతే {
                కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions =
                    (xkb_action_లెక్క_t) darray_కొలత(actions);
            }
            ఎంపిక (darray_కొలత(actions)) {
                స్థితి 0:
                    కీలకం->groups[సమూహం].స్థాయిలు[level].a.action =
                        (సంఘం xkb_action) { .type = ACTION_TYPE_NONE };
                    ఆపు;
                స్థితి 1:
                    కీలకం->groups[సమూహం].స్థాయిలు[level].a.action =
                        darray_అంశం(actions, 0);
                    ఆపు;
                అప్రమేయ:
                    కీలకం->groups[సమూహం].స్థాయిలు[level].a.actions =
                        memdup(darray_వస్తువులు(actions),
                               కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions,
                               పరిమాణం(*darray_వస్తువులు(actions)));
                    ఒకవేళ (!కీలకం->groups[సమూహం].స్థాయిలు[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        తిరిగి false;
                    }
            }

            ఒకవేళ (!darray_ఖాళీలు(actions))
                కీలకం->groups[సమూహం].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        ఒకవేళ (కీలకం->groups[సమూహం].implicit_actions)
            కీలకం->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    ఒకవేళ (!(కీలకం->explicit & EXPLICIT_VMODMAP))
        కీలకం->vmodmap = vmodmap;

    తిరిగి true;
}

స్థిర అంతఃవరుస bool
is_mod_action(సంఘం xkb_action *action)
{
    తిరిగి action->type == ACTION_TYPE_MOD_సమితి ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_తాళం;
}

స్థిర అంతఃవరుస bool
is_సమూహం_action(సంఘం xkb_action *action)
{
    తిరిగి action->type == ACTION_TYPE_సమూహం_సమితి ||
           action->type == ACTION_TYPE_సమూహం_LATCH ||
           action->type == ACTION_TYPE_సమూహం_తాళం;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
స్థిర శూన్య
Check_గుణిజం_Actions_Categories(నిర్మాణం xkb_keymap *keymap, నిర్మాణం xkb_కీలకం *కీలకం)
{
    కొరకు (xkb_layout_సూచీ_t g = 0; g < కీలకం->అంకె_groups; g++) {
        కొరకు (xkb_level_సూచీ_t l = 0; l < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, g); l++) {
            నిర్మాణం xkb_level *level = &కీలకం->groups[g].స్థాయిలు[l];
            ఒకవేళ (level->అంకె_actions <= 1)
                కొనసాగు;
            కొరకు (xkb_action_లెక్క_t i = 0; i < level->అంకె_actions; i++) {
                సంఘం xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool సమూహం_action = is_సమూహం_action(action1);
                ఒకవేళ (!(mod_action || సమూహం_action ||
                      action1->type == ACTION_TYPE_REDIRECT_కీలకం))
                    కొనసాగు;
                కొరకు (xkb_action_లెక్క_t j = i + 1; j < level->అంకె_actions; j++) {
                    సంఘం xkb_action *action2 = &level->a.actions[j];
                    ఒకవేళ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (సమూహం_action && is_సమూహం_action(action2)))
                    {
                        స్థిరరాశి అక్షరం * స్థిరరాశి type = (mod_action)
                            ? "modifiers"
                            : (సమూహం_action
                                ? "group"
                                : Action_Type_పాఠ్యం(action1->type));
                        log_err(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, కీలకం_Name_పాఠ్యం(keymap->ctx, కీలకం->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

స్థిర శూన్య
కూడు_కీలకం_aliases(నిర్మాణం xkb_keymap *keymap, darray_కొలత_t min, darray_కొలత_t max,
                నిర్మాణం xkb_కీలకం_alias *aliases)
{
    కొరకు (darray_కొలత_t alias = min; alias <= max; alias++) {
        స్థిరరాశి Keycode_సరిపోలిక నమోదు_2 = keymap->కీలకం_పేర్లు[alias];
        ఒకవేళ (నమోదు_2.is_alias && నమోదు_2.దొరికింది) {
            *aliases = (నిర్మాణం xkb_కీలకం_alias) {
                .alias = alias,
                .real = నమోదు_2.alias.real
            };
            aliases++;
        }
    }
}

స్థిర bool
update_pending_కీలకం_fields(నిర్మాణం xkb_keymap_info *info, నిర్మాణం xkb_కీలకం *కీలకం)
{
    ఒకవేళ (కీలకం->బహిర్గతం_యొక్క_range_pending_సమూహం) {
        నిర్మాణం pending_computation * స్థిరరాశి pc = &darray_అంశం(
            *info->pending_computations, కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య
        );
        ఒకవేళ (!pc->computed) {
            xkb_layout_సూచీ_t సమూహం = 0;
            ఎంపిక (సమాసం_Resolve_సమూహం(info, pc->సమాసం, true, &సమూహం, ఖాళీ)) {
            స్థితి PARSER_SUCCESS:
                pc->computed = true;
                pc->విలువ = సమూహం - 1;
                ఆపు;
            స్థితి PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                        "Invalid key redirect group index\n");
                తిరిగి (info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH);
            అప్రమేయ:
                /* Recover from error: ignore */;
            }
        }
        కీలకం->బహిర్గతం_యొక్క_range_pending_సమూహం = false;
        కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = pc->విలువ;
    }
    తిరిగి true;
}

స్థిర bool
update_pending_action_fields(నిర్మాణం xkb_keymap_info *info,
                             xkb_keycode_t keycode, సంఘం xkb_action *act)
{
    ఎంపిక (act->type) {
    స్థితి ACTION_TYPE_సమూహం_సమితి:
    స్థితి ACTION_TYPE_సమూహం_LATCH:
    స్థితి ACTION_TYPE_సమూహం_తాళం:
        ఒకవేళ (act->సమూహం.flags & ACTION_PENDING_COMPUTATION) {
            నిర్మాణం pending_computation * స్థిరరాశి pc =
                &darray_అంశం(*info->pending_computations, act->సమూహం.సమూహం);
            ఒకవేళ (!pc->computed) {
                xkb_layout_సూచీ_t సమూహం = 0;
                స్థిరరాశి bool absolute =
                    (act->సమూహం.flags & ACTION_ABSOLUTE_SWITCH);
                ఎంపిక (సమాసం_Resolve_సమూహం(info, pc->సమాసం, absolute, &సమూహం, ఖాళీ)) {
                స్థితి PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                            "Invalid action group index\n");
                    తిరిగి false;
                స్థితి PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    ఆపు;
                అప్రమేయ:
                    pc->computed = true;
                    ఒకవేళ (absolute) {
                        pc->విలువ = సమూహం - 1;
                    } లేకపోతే {
                        pc->విలువ = సమూహం;
                        ఒకవేళ (pc->సమాసం->common.type == STMT_సమాసం_NEGATE)
                            pc->విలువ = (uint32_t) (-(int32_t) pc->విలువ);
                    }
                }
            }
            act->సమూహం.సమూహం = (int32_t) pc->విలువ;
            act->సమూహం.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        తిరిగి true;
    స్థితి ACTION_TYPE_REDIRECT_కీలకం: {
        ఒకవేళ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_కీలకం_auto) {
            /* No auto value to set */
            తిరిగి true;
        } లేకపోతే {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        తిరిగి true;
    }
    అప్రమేయ:
        తిరిగి true;
    }
}

స్థిర bool
update_pending_led_fields(నిర్మాణం xkb_keymap_info *info, నిర్మాణం xkb_led *led)
{
    ఒకవేళ (led->pending_groups) {
        నిర్మాణం pending_computation * స్థిరరాశి pc =
            &darray_అంశం(*info->pending_computations, led->groups);
        ఒకవేళ (!pc->computed) {
            xkb_layout_ముసుగు_t ముసుగు = 0;
            ఒకవేళ (!సమాసం_Resolve_సమూహం_ముసుగు(info, pc->సమాసం, &ముసుగు, ఖాళీ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                        "Invalid LED group mask\n");
                తిరిగి false;
            }
            pc->computed = true;
            pc->విలువ = ముసుగు;
        }
        led->pending_groups = false;
        led->groups = pc->విలువ;
    }
    తిరిగి true;
}

/** Indices for the array "groupIndexNames" */
గణన {
    సమూహం_సూచీ_NAME_చివరి = 1,
};

/** Indices for the array "groupMaskNames" */
గణన {
    సమూహం_ముసుగు_NAME_చివరి = 3,
};

స్థిర శూన్య
update_auto_సమూహం_తాళం_on_modifier_release(నిర్మాణం xkb_level * పరిమిత level0,
                                           xkb_level_సూచీ_t l,
                                           సంఘం xkb_action * పరిమిత action)
{
    ఒకవేళ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].అంకె_actions == 1 &&
        action->type == ACTION_TYPE_సమూహం_తాళం &&
        /* lockOnRelease not set explicitly */
        !(action->సమూహం.flags & ACTION_EXPLICIT_తాళం_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->అంకె_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_సమితి))
    {
        తిరిగి;
    }

    action->సమూహం.flags |= ACTION_AUTO_సమూహం_తాళం_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
స్థిర bool
UpdateDerivedKeymapFields(నిర్మాణం xkb_keymap_info *info)
{
    నిర్మాణం xkb_keymap * పరిమిత స్థిరరాశి keymap = &info->keymap;

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
#నిర్వచితమైతే _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(పరిమాణం(నిర్మాణం xkb_కీలకం_alias) == పరిమాణం(Keycode_సరిపోలిక), "");
#లేకపోతే
    static_assert(పరిమాణం(నిర్మాణం xkb_కీలకం_alias) == 2 * పరిమాణం(Keycode_సరిపోలిక), "");
#అయితే_ముగింపు
    darray_కొలత_t అంకె_కీలకం_aliases = 0;
    darray_కొలత_t min_alias = 0;
    darray_కొలత_t max_alias = 0;
    కొరకు (xkb_atom_t alias = 0; alias < keymap->అంకె_కీలకం_పేర్లు; alias++) {
        స్థిరరాశి Keycode_సరిపోలిక నమోదు_2 = keymap->కీలకం_పేర్లు[alias];
        ఒకవేళ (నమోదు_2.is_alias && నమోదు_2.దొరికింది) {
            ఒకవేళ (!అంకె_కీలకం_aliases)
                min_alias = alias;
            max_alias = alias;
            అంకె_కీలకం_aliases++;
        }
    }
    ఒకవేళ (అంకె_కీలకం_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        స్థిరరాశి darray_కొలత_t required_space = పరిమాణం(నిర్మాణం xkb_కీలకం_alias)
                                           / పరిమాణం(Keycode_సరిపోలిక)
                                           * అంకె_కీలకం_aliases;
        ప్రతిజ్ఞా(అంకె_కీలకం_aliases <= keymap->అంకె_కీలకం_పేర్లు);
        ఒకవేళ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            కూడు_కీలకం_aliases(keymap, min_alias, max_alias, keymap->కీలకం_aliases);
            /* Shrink */
            నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి r =
                realloc(keymap->కీలకం_aliases,
                        అంకె_కీలకం_aliases * పరిమాణం(*keymap->కీలకం_aliases));
            ఒకవేళ (!r)
                తిరిగి false;
            keymap->కీలకం_aliases = r;
        } లేకపోతే ఒకవేళ (keymap->అంకె_కీలకం_పేర్లు - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి aliases = (నిర్మాణం xkb_కీలకం_alias *) (
                keymap->కీలకం_పేర్లు + max_alias + 1 +
                !is_aligned(keymap->కీలకం_పేర్లు + max_alias + 1, పరిమాణం(*aliases))
            );
            కూడు_కీలకం_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            జ్ఞాపకం_నకలు(keymap->కీలకం_aliases, aliases,
                   అంకె_కీలకం_aliases * పరిమాణం(*keymap->కీలకం_aliases));
            /* Shrink */
            నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి r =
                realloc(keymap->కీలకం_aliases,
                        అంకె_కీలకం_aliases * పరిమాణం(*keymap->కీలకం_aliases));
            ఒకవేళ (!r)
                తిరిగి false;
            keymap->కీలకం_aliases = r;
        } లేకపోతే {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి aliases = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె_కీలకం_aliases,
                                                          పరిమాణం(*aliases));
            ఒకవేళ (!aliases)
                తిరిగి false;
            కూడు_కీలకం_aliases(keymap, min_alias, max_alias, aliases);
            విడుదల(keymap->కీలకం_పేర్లు);
            keymap->కీలకం_aliases = aliases;
        }
    }
    keymap->అంకె_కీలకం_aliases = అంకె_కీలకం_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    నిర్మాణం xkb_కీలకం *కీలకం;
    xkb_కీలకాలు_foreach(కీలకం, keymap)
        keymap->అంకె_groups = MAX(keymap->అంకె_groups, కీలకం->అంకె_groups);

    స్థిరరాశి bool pending_computations = !darray_ఖాళీలు(*info->pending_computations);

    ఒకవేళ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        స్థిరరాశి xkb_layout_సూచీ_t అంకె_groups
            = (keymap->అంకె_groups)
            ? keymap->అంకె_groups
            : 1; /* default to the “first” layout */
        info->lookup.సమూహం_సూచీ_పేర్లు[సమూహం_సూచీ_NAME_చివరి] =
            (Lookup_నమోదు) {
                .name = సమూహం_చివరి_సూచీ_NAME,
                .విలువ = అంకె_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.సమూహం_ముసుగు_పేర్లు[సమూహం_ముసుగు_NAME_చివరి] =
            (Lookup_నమోదు) {
                .name = సమూహం_చివరి_సూచీ_NAME,
                .విలువ = UINT32_C(1) << (అంకె_groups - 1)
            };

        /* Update interpret fields with pending computations */
        కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_sym_interprets; i++) {
            నిర్మాణం xkb_sym_interpret * పరిమిత స్థిరరాశి interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                ఒకవేళ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    తిరిగి false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        ఒకవేళ (!Apply_Interps_గమ్యం_కీలకం(keymap, కీలకం))
            తిరిగి false;
        Check_గుణిజం_Actions_Categories(keymap, కీలకం);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_సూచీ_t idx;
    నిర్మాణం xkb_mod *mod;
    xkb_కీలకాలు_foreach(కీలకం, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            ఒకవేళ (కీలకం->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= కీలకం->modmap;

    ఒకవేళ (keymap->format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2) {
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
            స్థిరరాశి xkb_mod_ముసుగు_t ముసుగు = UINT32_C(1) << idx;
            ఒకవేళ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & ముసుగు)) {
                mod->mapping = ముసుగు;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= ముసుగు;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    ప్రతిజ్ఞా(keymap->canonical_state_ముసుగు == MOD_REAL_ముసుగు_ALL);
    xkb_mod_ముసుగు_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_ముసుగు |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_types; i++) {
        Compute_Effective_ముసుగు(keymap, &keymap->types[i].mods);

        కొరకు (darray_కొలత_t j = 0; j < keymap->types[i].అంకె_entries; j++) {
            ఒకవేళ (has_unbound_vmods(keymap,
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
                keymap->types[i].entries[j].mods.ముసుగు = 0;
                కొనసాగు;
            }
            Compute_Effective_ముసుగు(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_ముసుగు(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        ఒకవేళ (!update_pending_కీలకం_fields(info, కీలకం))
            తిరిగి false;
        కొరకు (xkb_layout_సూచీ_t i = 0; i < కీలకం->అంకె_groups; i++) {
            కొరకు (xkb_level_సూచీ_t j = 0; j < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, i); j++) {
                నిర్మాణం xkb_level * స్థిరరాశి level = &కీలకం->groups[i].స్థాయిలు[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, కీలకం->modmap);
                    ఒకవేళ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_కీలకం) &&
                        !update_pending_action_fields(info, కీలకం->keycode, act))
                    {
                        తిరిగి false;
                    }
                    update_auto_సమూహం_తాళం_on_modifier_release(
                        కీలకం->groups[i].స్థాయిలు, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    నిర్మాణం xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_ముసుగు(keymap, &led->mods); /* vmod -> led maps */
        ఒకవేళ (pending_computations &&
            !update_pending_led_fields(info, led))
            తిరిగి false;
    }

    తిరిగి true;
}

రకనిర్వచనం bool (*compile_file_fn)(XkbFile *file, నిర్మాణం xkb_keymap_info *info);

స్థిర స్థిరరాశి compile_file_fn compile_file_fns[చివరి_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_కీలకం_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

స్థిర శూన్య
pending_computations_array_free(pending_computation_array *p)
{
    నిర్మాణం pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->సమాసం);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, నిర్మాణం xkb_keymap *keymap)
{
    XkbFile *files[చివరి_KEYMAP_FILE_TYPE + 1] = { ఖాళీ };
    గణన xkb_file_type type;
    నిర్మాణం xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    కొరకు (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.తదుపరి) {
        ఒకవేళ (file->file_type < మొదటి_KEYMAP_FILE_TYPE ||
            file->file_type > చివరి_KEYMAP_FILE_TYPE) {
            ఒకవేళ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } లేకపోతే {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_గమ్యం_string(file->file_type));
            }
            కొనసాగు;
        }

        ఒకవేళ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_గమ్యం_string(file->file_type));
            కొనసాగు;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    నిర్మాణం xkb_keymap_info info = {
        /* Copy the keymap */
        .keymap = *keymap,
        .strict = parser_strict_flags_from_keymap(keymap),
        .features = {
            .max_groups = format_max_groups(keymap->format),
            .max_overlays = format_max_overlays(keymap->format),
            .controls_name_స్థానభ్రంశం = format_control_పేర్లు_స్థానభ్రంశం(keymap->format),
            .సమూహం_తాళం_on_release =
                is_సమూహం_తాళం_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_తాళం_On_Press_Supported(keymap->format),
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
            .సమూహం_సూచీ_పేర్లు = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [సమూహం_సూచీ_NAME_చివరి] = {
                    keymap->అంకె_groups ? సమూహం_చివరి_సూచీ_NAME : ఖాళీ,
                    keymap->అంకె_groups /* 1-indexed */
                },
                { ఖాళీ, 0 }
            },
            .సమూహం_ముసుగు_పేర్లు = {
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
                [సమూహం_ముసుగు_NAME_చివరి] = {
                    keymap->అంకె_groups ? సమూహం_చివరి_సూచీ_NAME : ఖాళీ,
                    /* Be extra cautious */
                    keymap->అంకె_groups && keymap->అంకె_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->అంకె_groups - 1))
                        : 0
                },
                { ఖాళీ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    కొరకు (type = మొదటి_KEYMAP_FILE_TYPE;
         type <= చివరి_KEYMAP_FILE_TYPE;
         type++) {
        ఒకవేళ (files[type] == ఖాళీ) {
            log_dbg(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_గమ్యం_string(type));
        } లేకపోతే {
            log_dbg(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_గమ్యం_string(type),
                    సురక్షిత_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        స్థిరరాశి bool ok = compile_file_fns[type](files[type], &info);
        ఒకవేళ (!ok) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_గమ్యం_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            తిరిగి false;
        }
    }

    స్థిరరాశి bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    తిరిగి ok;
}
