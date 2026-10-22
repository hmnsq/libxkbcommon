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
has_unbound_vmods(بنية xkb_keymap *keymap, xkb_mod_ماسک_t ماسک)
{
    xkb_mod_شاخص_t k;
    بنية xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        إذا ((ماسک & (UINT32_C(1) << k)) && mod->mapping == 0)
            أرجع true;
    أرجع false;
}

ساكن مضمن فراغ
Compute_Effective_ماسک(بنية xkb_keymap *keymap, بنية xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ثابت xkb_mod_ماسک_t unknown_mods = ~(xkb_mod_ماسک_t)
        ((UINT64_C(1) << keymap->mods.عدد_mods) - UINT64_C(1));
    mods->ماسک = mod_ماسک_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ساكن فراغ
UpdateActionMods(بنية xkb_keymap *keymap, اتحاد xkb_action *act,
                 xkb_mod_ماسک_t modmap)
{
    بدل (act->type) {
    حالة ACTION_TYPE_MOD_سېټ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_قلف:
        إذا (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_ماسک(keymap, &act->mods.mods);
        اكسر;
    افتراضي:
        اكسر;
    }
}

ساكن ثابت بنية xkb_sym_interpret default_interpret = {
    .sym = XKB_کلی_No_نښه /* unused */,
    .سمون = سمون_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_کلی_REPEAT,
    .virtual_mod = (xkb_mod_شاخص_t) DEFAULT_INTERPRET_VMOD,
    .عدد_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

تعريفنوع darray(ثابت بنية xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ساكن bool
پیدا_کړه_Interp_For_کلی(بنية xkb_keymap *keymap, ثابت بنية xkb_کلی *کلی,
                 xkb_layout_شاخص_t ډله, xkb_level_شاخص_t level,
                 xkb_sym_interprets *interprets)
{
    ثابت xkb_keysym_t *syms;
    صحيح عدد_syms;

    عدد_syms = xkb_keymap_کلی_get_syms_by_level(keymap, کلی->keycode, ډله,
                                                level, &syms);
    إذا (عدد_syms <= 0)
        أرجع false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    لكل (صحيح s = 0; s < عدد_syms; s++) {
        bool وموندل = false;
        لكل (darray_اندازه_t i = 0; i < keymap->عدد_sym_interprets; i++) {
            بنية xkb_sym_interpret * ثابت interp = &keymap->sym_interprets[i];
            xkb_mod_ماسک_t mods;

            وموندل = false;

            إذا (interp->sym != syms[s] && interp->sym != XKB_کلی_No_نښه)
                استمر;

            إذا (interp->level_one_only && level != 0)
                mods = 0;
            وإلا
                mods = کلی->modmap;

            بدل (interp->سمون) {
            حالة سمون_NONE:
                وموندل = !(interp->mods & mods);
                اكسر;
            حالة سمون_ANY_OR_NONE:
                وموندل = (!mods || (interp->mods & mods));
                اكسر;
            حالة سمون_ANY:
                وموندل = (interp->mods & mods);
                اكسر;
            حالة سمون_ALL:
                وموندل = ((interp->mods & mods) == interp->mods);
                اكسر;
            حالة سمون_EXACTLY:
                وموندل = (interp->mods == mods);
                اكسر;
            }

            إذا (وموندل && i > 0 && interp->sym == XKB_کلی_No_نښه) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                بنية xkb_sym_interpret ثابت **پخوانی_interp;
                darray_foreach(پخوانی_interp, *interprets) {
                    إذا (*پخوانی_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        وموندل = false;
                        log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_متن(keymap->ctx, syms[s]),
                                 level + 1, ډله + 1,
                                 کلی_Name_متن(keymap->ctx, کلی->name));
                        اذهب not_وموندل;
                    }
                }
            }
            إذا (وموندل) {
                darray_append(*interprets, interp);
                interp->required = true;
                اكسر;
            }
        }
        إذا (!وموندل)
not_وموندل:
            darray_append(*interprets, &default_interpret);
    }
    أرجع true;
}

ساكن bool
Apply_Interps_منزل_کلی(بنية xkb_keymap *keymap, بنية xkb_کلی *کلی)
{
    xkb_mod_ماسک_t vmodmap = 0;
    xkb_level_شاخص_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(اتحاد xkb_action) actions = darray_new();

    لكل (xkb_layout_شاخص_t ډله = 0; ډله < کلی->عدد_groups; ډله++) {
        /* Skip any interpretation for this group if it has explicit actions */
        إذا (کلی->groups[ډله].explicit_actions)
            استمر;

        لكل (level = 0; level < Xkb_کلی_عدد_پوړونه(کلی, ډله); level++) {
            أكد(کلی->groups[ډله].پوړونه[level].عدد_actions == 0);

            ثابت بنية xkb_sym_interpret **interp_iter;
            ثابت بنية xkb_sym_interpret *interp;
            اندازه_t k;
            darray_resize(interprets, 0);

            ثابت bool وموندل = پیدا_کړه_Interp_For_کلی(keymap, کلی, ډله, level, &interprets);
            إذا (!وموندل)
                استمر;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                إذا (ډله == 0 && level == 0 &&
                    !(کلی->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    کلی->repeats = true;
                }

                إذا ((ډله == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    إذا (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                بدل (interp->عدد_actions) {
                حالة 0:
                    اكسر;
                حالة 1:
                    darray_append(actions, interp->a.action);
                    اكسر;
                افتراضي:
                    darray_append_توکي(actions, interp->a.actions,
                                        interp->عدد_actions);
                }
            }

            /* Copy the actions */
            إذا (unlikely(darray_اندازه(actions)) > MAX_ACTIONS_فی_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         کلی_Name_متن(keymap->ctx, کلی->name),
                         MAX_ACTIONS_فی_LEVEL, darray_اندازه(actions));
                کلی->groups[ډله].پوړونه[level].عدد_actions =
                    MAX_ACTIONS_فی_LEVEL;
            } وإلا {
                کلی->groups[ډله].پوړونه[level].عدد_actions =
                    (xkb_action_شمېر_t) darray_اندازه(actions);
            }
            بدل (darray_اندازه(actions)) {
                حالة 0:
                    کلی->groups[ډله].پوړونه[level].a.action =
                        (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                    اكسر;
                حالة 1:
                    کلی->groups[ډله].پوړونه[level].a.action =
                        darray_توکی(actions, 0);
                    اكسر;
                افتراضي:
                    کلی->groups[ډله].پوړونه[level].a.actions =
                        memdup(darray_توکي(actions),
                               کلی->groups[ډله].پوړونه[level].عدد_actions,
                               حجم(*darray_توکي(actions)));
                    إذا (!کلی->groups[ډله].پوړونه[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        أرجع false;
                    }
            }

            إذا (!darray_تشه(actions))
                کلی->groups[ډله].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        إذا (کلی->groups[ډله].implicit_actions)
            کلی->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    إذا (!(کلی->explicit & EXPLICIT_VMODMAP))
        کلی->vmodmap = vmodmap;

    أرجع true;
}

ساكن مضمن bool
is_mod_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_MOD_سېټ ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_قلف;
}

ساكن مضمن bool
is_ډله_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_ډله_سېټ ||
           action->type == ACTION_TYPE_ډله_LATCH ||
           action->type == ACTION_TYPE_ډله_قلف;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ساكن فراغ
Check_مضرب_Actions_Categories(بنية xkb_keymap *keymap, بنية xkb_کلی *کلی)
{
    لكل (xkb_layout_شاخص_t g = 0; g < کلی->عدد_groups; g++) {
        لكل (xkb_level_شاخص_t l = 0; l < Xkb_کلی_عدد_پوړونه(کلی, g); l++) {
            بنية xkb_level *level = &کلی->groups[g].پوړونه[l];
            إذا (level->عدد_actions <= 1)
                استمر;
            لكل (xkb_action_شمېر_t i = 0; i < level->عدد_actions; i++) {
                اتحاد xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool ډله_action = is_ډله_action(action1);
                إذا (!(mod_action || ډله_action ||
                      action1->type == ACTION_TYPE_REDIRECT_کلی))
                    استمر;
                لكل (xkb_action_شمېر_t j = i + 1; j < level->عدد_actions; j++) {
                    اتحاد xkb_action *action2 = &level->a.actions[j];
                    إذا (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (ډله_action && is_ډله_action(action2)))
                    {
                        ثابت حرف * ثابت type = (mod_action)
                            ? "modifiers"
                            : (ډله_action
                                ? "group"
                                : Action_Type_متن(action1->type));
                        log_err(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, کلی_Name_متن(keymap->ctx, کلی->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ساكن فراغ
جمع_کړه_کلی_aliases(بنية xkb_keymap *keymap, darray_اندازه_t min, darray_اندازه_t max,
                بنية xkb_کلی_alias *aliases)
{
    لكل (darray_اندازه_t alias = min; alias <= max; alias++) {
        ثابت Keycode_سمون ثبت = keymap->کلی_نومونه[alias];
        إذا (ثبت.is_alias && ثبت.وموندل) {
            *aliases = (بنية xkb_کلی_alias) {
                .alias = alias,
                .real = ثبت.alias.real
            };
            aliases++;
        }
    }
}

ساكن bool
update_pending_کلی_fields(بنية xkb_keymap_info *info, بنية xkb_کلی *کلی)
{
    إذا (کلی->بهر_د_range_pending_ډله) {
        بنية pending_computation * ثابت pc = &darray_توکی(
            *info->pending_computations, کلی->بهر_د_range_ډله_شمېره
        );
        إذا (!pc->computed) {
            xkb_layout_شاخص_t ډله = 0;
            بدل (بیان_Resolve_ډله(info, pc->بیان, true, &ډله, عدم)) {
            حالة PARSER_SUCCESS:
                pc->computed = true;
                pc->ارزښت = ډله - 1;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Invalid key redirect group index\n");
                أرجع (info->strict & PARSER_NO_ډګر_TYPE_MISMATCH);
            افتراضي:
                /* Recover from error: ignore */;
            }
        }
        کلی->بهر_د_range_pending_ډله = false;
        کلی->بهر_د_range_ډله_شمېره = pc->ارزښت;
    }
    أرجع true;
}

ساكن bool
update_pending_action_fields(بنية xkb_keymap_info *info,
                             xkb_keycode_t keycode, اتحاد xkb_action *act)
{
    بدل (act->type) {
    حالة ACTION_TYPE_ډله_سېټ:
    حالة ACTION_TYPE_ډله_LATCH:
    حالة ACTION_TYPE_ډله_قلف:
        إذا (act->ډله.flags & ACTION_PENDING_COMPUTATION) {
            بنية pending_computation * ثابت pc =
                &darray_توکی(*info->pending_computations, act->ډله.ډله);
            إذا (!pc->computed) {
                xkb_layout_شاخص_t ډله = 0;
                ثابت bool absolute =
                    (act->ډله.flags & ACTION_ABSOLUTE_SWITCH);
                بدل (بیان_Resolve_ډله(info, pc->بیان, absolute, &ډله, عدم)) {
                حالة PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                            "Invalid action group index\n");
                    أرجع false;
                حالة PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    اكسر;
                افتراضي:
                    pc->computed = true;
                    إذا (absolute) {
                        pc->ارزښت = ډله - 1;
                    } وإلا {
                        pc->ارزښت = ډله;
                        إذا (pc->بیان->common.type == STMT_بیان_NEGATE)
                            pc->ارزښت = (uint32_t) (-(int32_t) pc->ارزښت);
                    }
                }
            }
            act->ډله.ډله = (int32_t) pc->ارزښت;
            act->ډله.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        أرجع true;
    حالة ACTION_TYPE_REDIRECT_کلی: {
        إذا (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_کلی_auto) {
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
            &darray_توکی(*info->pending_computations, led->groups);
        إذا (!pc->computed) {
            xkb_layout_ماسک_t ماسک = 0;
            إذا (!بیان_Resolve_ډله_ماسک(info, pc->بیان, &ماسک, عدم)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Invalid LED group mask\n");
                أرجع false;
            }
            pc->computed = true;
            pc->ارزښت = ماسک;
        }
        led->pending_groups = false;
        led->groups = pc->ارزښت;
    }
    أرجع true;
}

/** Indices for the array "groupIndexNames" */
تعداد {
    ډله_شاخص_NAME_وروستی = 1,
};

/** Indices for the array "groupMaskNames" */
تعداد {
    ډله_ماسک_NAME_وروستی = 3,
};

ساكن فراغ
update_auto_ډله_قلف_on_modifier_release(بنية xkb_level * محدود level0,
                                           xkb_level_شاخص_t l,
                                           اتحاد xkb_action * محدود action)
{
    إذا (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].عدد_actions == 1 &&
        action->type == ACTION_TYPE_ډله_قلف &&
        /* lockOnRelease not set explicitly */
        !(action->ډله.flags & ACTION_EXPLICIT_قلف_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->عدد_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_سېټ))
    {
        أرجع;
    }

    action->ډله.flags |= ACTION_AUTO_ډله_قلف_ON_MODIFIER_RELEASE;
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
    بنية xkb_keymap * محدود ثابت keymap = &info->keymap;

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
#که_تعریف_شوی _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(حجم(بنية xkb_کلی_alias) == حجم(Keycode_سمون), "");
#وإلا
    static_assert(حجم(بنية xkb_کلی_alias) == 2 * حجم(Keycode_سمون), "");
#د_که_پای
    darray_اندازه_t عدد_کلی_aliases = 0;
    darray_اندازه_t min_alias = 0;
    darray_اندازه_t max_alias = 0;
    لكل (xkb_atom_t alias = 0; alias < keymap->عدد_کلی_نومونه; alias++) {
        ثابت Keycode_سمون ثبت = keymap->کلی_نومونه[alias];
        إذا (ثبت.is_alias && ثبت.وموندل) {
            إذا (!عدد_کلی_aliases)
                min_alias = alias;
            max_alias = alias;
            عدد_کلی_aliases++;
        }
    }
    إذا (عدد_کلی_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ثابت darray_اندازه_t required_space = حجم(بنية xkb_کلی_alias)
                                           / حجم(Keycode_سمون)
                                           * عدد_کلی_aliases;
        أكد(عدد_کلی_aliases <= keymap->عدد_کلی_نومونه);
        إذا (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            جمع_کړه_کلی_aliases(keymap, min_alias, max_alias, keymap->کلی_aliases);
            /* Shrink */
            بنية xkb_کلی_alias * ثابت r =
                realloc(keymap->کلی_aliases,
                        عدد_کلی_aliases * حجم(*keymap->کلی_aliases));
            إذا (!r)
                أرجع false;
            keymap->کلی_aliases = r;
        } وإلا إذا (keymap->عدد_کلی_نومونه - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            بنية xkb_کلی_alias * ثابت aliases = (بنية xkb_کلی_alias *) (
                keymap->کلی_نومونه + max_alias + 1 +
                !is_aligned(keymap->کلی_نومونه + max_alias + 1, حجم(*aliases))
            );
            جمع_کړه_کلی_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            حافظه_نقل(keymap->کلی_aliases, aliases,
                   عدد_کلی_aliases * حجم(*keymap->کلی_aliases));
            /* Shrink */
            بنية xkb_کلی_alias * ثابت r =
                realloc(keymap->کلی_aliases,
                        عدد_کلی_aliases * حجم(*keymap->کلی_aliases));
            إذا (!r)
                أرجع false;
            keymap->کلی_aliases = r;
        } وإلا {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            بنية xkb_کلی_alias * ثابت aliases = حافظه_صفر_ورکړه(عدد_کلی_aliases,
                                                          حجم(*aliases));
            إذا (!aliases)
                أرجع false;
            جمع_کړه_کلی_aliases(keymap, min_alias, max_alias, aliases);
            حرر(keymap->کلی_نومونه);
            keymap->کلی_aliases = aliases;
        }
    }
    keymap->عدد_کلی_aliases = عدد_کلی_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    بنية xkb_کلی *کلی;
    xkb_کلیګانې_foreach(کلی, keymap)
        keymap->عدد_groups = MAX(keymap->عدد_groups, کلی->عدد_groups);

    ثابت bool pending_computations = !darray_تشه(*info->pending_computations);

    إذا (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ثابت xkb_layout_شاخص_t عدد_groups
            = (keymap->عدد_groups)
            ? keymap->عدد_groups
            : 1; /* default to the “first” layout */
        info->lookup.ډله_شاخص_نومونه[ډله_شاخص_NAME_وروستی] =
            (Lookup_ثبت) {
                .name = ډله_وروستی_شاخص_NAME,
                .ارزښت = عدد_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.ډله_ماسک_نومونه[ډله_ماسک_NAME_وروستی] =
            (Lookup_ثبت) {
                .name = ډله_وروستی_شاخص_NAME,
                .ارزښت = UINT32_C(1) << (عدد_groups - 1)
            };

        /* Update interpret fields with pending computations */
        لكل (darray_اندازه_t i = 0; i < keymap->عدد_sym_interprets; i++) {
            بنية xkb_sym_interpret * محدود ثابت interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                إذا (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    أرجع false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_کلیګانې_foreach(کلی, keymap) {
        إذا (!Apply_Interps_منزل_کلی(keymap, کلی))
            أرجع false;
        Check_مضرب_Actions_Categories(keymap, کلی);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_شاخص_t idx;
    بنية xkb_mod *mod;
    xkb_کلیګانې_foreach(کلی, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            إذا (کلی->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= کلی->modmap;

    إذا (keymap->format >= XKB_KEYMAP_FORMAT_متن_V2) {
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
            ثابت xkb_mod_ماسک_t ماسک = UINT32_C(1) << idx;
            إذا (mod->mapping == 0 && !(keymap->mods.explicit_vmods & ماسک)) {
                mod->mapping = ماسک;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= ماسک;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    أكد(keymap->canonical_state_ماسک == MOD_REAL_ماسک_ALL);
    xkb_mod_ماسک_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_ماسک |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    لكل (darray_اندازه_t i = 0; i < keymap->عدد_types; i++) {
        Compute_Effective_ماسک(keymap, &keymap->types[i].mods);

        لكل (darray_اندازه_t j = 0; j < keymap->types[i].عدد_entries; j++) {
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
                keymap->types[i].entries[j].mods.ماسک = 0;
                استمر;
            }
            Compute_Effective_ماسک(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_ماسک(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_کلیګانې_foreach(کلی, keymap) {
        إذا (!update_pending_کلی_fields(info, کلی))
            أرجع false;
        لكل (xkb_layout_شاخص_t i = 0; i < کلی->عدد_groups; i++) {
            لكل (xkb_level_شاخص_t j = 0; j < Xkb_کلی_عدد_پوړونه(کلی, i); j++) {
                بنية xkb_level * ثابت level = &کلی->groups[i].پوړونه[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, کلی->modmap);
                    إذا ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_کلی) &&
                        !update_pending_action_fields(info, کلی->keycode, act))
                    {
                        أرجع false;
                    }
                    update_auto_ډله_قلف_on_modifier_release(
                        کلی->groups[i].پوړونه, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    بنية xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_ماسک(keymap, &led->mods); /* vmod -> led maps */
        إذا (pending_computations &&
            !update_pending_led_fields(info, led))
            أرجع false;
    }

    أرجع true;
}

تعريفنوع bool (*compile_file_fn)(XkbFile *file, بنية xkb_keymap_info *info);

ساكن ثابت compile_file_fn compile_file_fns[وروستی_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_کلی_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ساكن فراغ
pending_computations_array_free(pending_computation_array *p)
{
    بنية pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->بیان);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, بنية xkb_keymap *keymap)
{
    XkbFile *files[وروستی_KEYMAP_FILE_TYPE + 1] = { عدم };
    تعداد xkb_file_type type;
    بنية xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    لكل (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.بل) {
        إذا (file->file_type < لومړی_KEYMAP_FILE_TYPE ||
            file->file_type > وروستی_KEYMAP_FILE_TYPE) {
            إذا (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } وإلا {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_منزل_string(file->file_type));
            }
            استمر;
        }

        إذا (files[file->file_type]) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_منزل_string(file->file_type));
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
            .controls_name_دننه_واټن = format_control_نومونه_دننه_واټن(keymap->format),
            .ډله_قلف_on_release =
                is_ډله_قلف_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_قلف_On_Press_Supported(keymap->format),
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
            .ډله_شاخص_نومونه = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [ډله_شاخص_NAME_وروستی] = {
                    keymap->عدد_groups ? ډله_وروستی_شاخص_NAME : عدم,
                    keymap->عدد_groups /* 1-indexed */
                },
                { عدم, 0 }
            },
            .ډله_ماسک_نومونه = {
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
                [ډله_ماسک_NAME_وروستی] = {
                    keymap->عدد_groups ? ډله_وروستی_شاخص_NAME : عدم,
                    /* Be extra cautious */
                    keymap->عدد_groups && keymap->عدد_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->عدد_groups - 1))
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
    لكل (type = لومړی_KEYMAP_FILE_TYPE;
         type <= وروستی_KEYMAP_FILE_TYPE;
         type++) {
        إذا (files[type] == عدم) {
            log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_منزل_string(type));
        } وإلا {
            log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_منزل_string(type),
                    خوندي_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ثابت bool ok = compile_file_fns[type](files[type], &info);
        إذا (!ok) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_منزل_string(type));
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
