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
has_unbound_vmods(بنية xkb_keymap *keymap, xkb_mod_نقاب_t نقاب)
{
    xkb_mod_اشاريو_t k;
    بنية xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        إذا ((نقاب & (UINT32_C(1) << k)) && mod->mapping == 0)
            أرجع true;
    أرجع false;
}

ساكن مضمن فراغ
Compute_Effective_نقاب(بنية xkb_keymap *keymap, بنية xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ثابت xkb_mod_نقاب_t unknown_mods = ~(xkb_mod_نقاب_t)
        ((UINT64_C(1) << keymap->mods.انگ_mods) - UINT64_C(1));
    mods->نقاب = mod_نقاب_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ساكن فراغ
UpdateActionMods(بنية xkb_keymap *keymap, اتحاد xkb_action *act,
                 xkb_mod_نقاب_t modmap)
{
    بدل (act->type) {
    حالة ACTION_TYPE_MOD_سيٽ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_تالو:
        إذا (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_نقاب(keymap, &act->mods.mods);
        اكسر;
    افتراضي:
        اكسر;
    }
}

ساكن ثابت بنية xkb_sym_interpret default_interpret = {
    .sym = XKB_ڪنجي_No_نشان /* unused */,
    .ميلاپ = ميلاپ_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ڪنجي_REPEAT,
    .virtual_mod = (xkb_mod_اشاريو_t) DEFAULT_INTERPRET_VMOD,
    .انگ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

تعريفنوع darray(ثابت بنية xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ساكن bool
ڳوليو_Interp_For_ڪنجي(بنية xkb_keymap *keymap, ثابت بنية xkb_ڪنجي *ڪنجي,
                 xkb_layout_اشاريو_t گروهه, xkb_level_اشاريو_t level,
                 xkb_sym_interprets *interprets)
{
    ثابت xkb_keysym_t *syms;
    صحيح انگ_syms;

    انگ_syms = xkb_keymap_ڪنجي_get_syms_by_level(keymap, ڪنجي->keycode, گروهه,
                                                level, &syms);
    إذا (انگ_syms <= 0)
        أرجع false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    لكل (صحيح s = 0; s < انگ_syms; s++) {
        bool مليو = false;
        لكل (darray_سائيز_t i = 0; i < keymap->انگ_sym_interprets; i++) {
            بنية xkb_sym_interpret * ثابت interp = &keymap->sym_interprets[i];
            xkb_mod_نقاب_t mods;

            مليو = false;

            إذا (interp->sym != syms[s] && interp->sym != XKB_ڪنجي_No_نشان)
                استمر;

            إذا (interp->level_one_only && level != 0)
                mods = 0;
            وإلا
                mods = ڪنجي->modmap;

            بدل (interp->ميلاپ) {
            حالة ميلاپ_NONE:
                مليو = !(interp->mods & mods);
                اكسر;
            حالة ميلاپ_ANY_OR_NONE:
                مليو = (!mods || (interp->mods & mods));
                اكسر;
            حالة ميلاپ_ANY:
                مليو = (interp->mods & mods);
                اكسر;
            حالة ميلاپ_ALL:
                مليو = ((interp->mods & mods) == interp->mods);
                اكسر;
            حالة ميلاپ_EXACTLY:
                مليو = (interp->mods == mods);
                اكسر;
            }

            إذا (مليو && i > 0 && interp->sym == XKB_ڪنجي_No_نشان) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                بنية xkb_sym_interpret ثابت **اڳيون_interp;
                darray_foreach(اڳيون_interp, *interprets) {
                    إذا (*اڳيون_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        مليو = false;
                        log_warn(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_متن(keymap->ctx, syms[s]),
                                 level + 1, گروهه + 1,
                                 ڪنجي_Name_متن(keymap->ctx, ڪنجي->name));
                        اذهب not_مليو;
                    }
                }
            }
            إذا (مليو) {
                darray_append(*interprets, interp);
                interp->required = true;
                اكسر;
            }
        }
        إذا (!مليو)
not_مليو:
            darray_append(*interprets, &default_interpret);
    }
    أرجع true;
}

ساكن bool
Apply_Interps_منزل_ڪنجي(بنية xkb_keymap *keymap, بنية xkb_ڪنجي *ڪنجي)
{
    xkb_mod_نقاب_t vmodmap = 0;
    xkb_level_اشاريو_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(اتحاد xkb_action) actions = darray_new();

    لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < ڪنجي->انگ_groups; گروهه++) {
        /* Skip any interpretation for this group if it has explicit actions */
        إذا (ڪنجي->groups[گروهه].explicit_actions)
            استمر;

        لكل (level = 0; level < Xkb_ڪنجي_انگ_سطحون(ڪنجي, گروهه); level++) {
            أكد(ڪنجي->groups[گروهه].سطحون[level].انگ_actions == 0);

            ثابت بنية xkb_sym_interpret **interp_iter;
            ثابت بنية xkb_sym_interpret *interp;
            سائيز_t k;
            darray_resize(interprets, 0);

            ثابت bool مليو = ڳوليو_Interp_For_ڪنجي(keymap, ڪنجي, گروهه, level, &interprets);
            إذا (!مليو)
                استمر;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                إذا (گروهه == 0 && level == 0 &&
                    !(ڪنجي->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ڪنجي->repeats = true;
                }

                إذا ((گروهه == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    إذا (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                بدل (interp->انگ_actions) {
                حالة 0:
                    اكسر;
                حالة 1:
                    darray_append(actions, interp->a.action);
                    اكسر;
                افتراضي:
                    darray_append_شيون(actions, interp->a.actions,
                                        interp->انگ_actions);
                }
            }

            /* Copy the actions */
            إذا (unlikely(darray_سائيز(actions)) > MAX_ACTIONS_في_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ڪنجي_Name_متن(keymap->ctx, ڪنجي->name),
                         MAX_ACTIONS_في_LEVEL, darray_سائيز(actions));
                ڪنجي->groups[گروهه].سطحون[level].انگ_actions =
                    MAX_ACTIONS_في_LEVEL;
            } وإلا {
                ڪنجي->groups[گروهه].سطحون[level].انگ_actions =
                    (xkb_action_ڳڻپ_t) darray_سائيز(actions);
            }
            بدل (darray_سائيز(actions)) {
                حالة 0:
                    ڪنجي->groups[گروهه].سطحون[level].a.action =
                        (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                    اكسر;
                حالة 1:
                    ڪنجي->groups[گروهه].سطحون[level].a.action =
                        darray_شي(actions, 0);
                    اكسر;
                افتراضي:
                    ڪنجي->groups[گروهه].سطحون[level].a.actions =
                        memdup(darray_شيون(actions),
                               ڪنجي->groups[گروهه].سطحون[level].انگ_actions,
                               حجم(*darray_شيون(actions)));
                    إذا (!ڪنجي->groups[گروهه].سطحون[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        أرجع false;
                    }
            }

            إذا (!darray_خالي_جايون(actions))
                ڪنجي->groups[گروهه].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        إذا (ڪنجي->groups[گروهه].implicit_actions)
            ڪنجي->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    إذا (!(ڪنجي->explicit & EXPLICIT_VMODMAP))
        ڪنجي->vmodmap = vmodmap;

    أرجع true;
}

ساكن مضمن bool
is_mod_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_MOD_سيٽ ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_تالو;
}

ساكن مضمن bool
is_گروهه_action(اتحاد xkb_action *action)
{
    أرجع action->type == ACTION_TYPE_گروهه_سيٽ ||
           action->type == ACTION_TYPE_گروهه_LATCH ||
           action->type == ACTION_TYPE_گروهه_تالو;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ساكن فراغ
Check_گڻو_Actions_Categories(بنية xkb_keymap *keymap, بنية xkb_ڪنجي *ڪنجي)
{
    لكل (xkb_layout_اشاريو_t g = 0; g < ڪنجي->انگ_groups; g++) {
        لكل (xkb_level_اشاريو_t l = 0; l < Xkb_ڪنجي_انگ_سطحون(ڪنجي, g); l++) {
            بنية xkb_level *level = &ڪنجي->groups[g].سطحون[l];
            إذا (level->انگ_actions <= 1)
                استمر;
            لكل (xkb_action_ڳڻپ_t i = 0; i < level->انگ_actions; i++) {
                اتحاد xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool گروهه_action = is_گروهه_action(action1);
                إذا (!(mod_action || گروهه_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ڪنجي))
                    استمر;
                لكل (xkb_action_ڳڻپ_t j = i + 1; j < level->انگ_actions; j++) {
                    اتحاد xkb_action *action2 = &level->a.actions[j];
                    إذا (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (گروهه_action && is_گروهه_action(action2)))
                    {
                        ثابت حرف * ثابت type = (mod_action)
                            ? "modifiers"
                            : (گروهه_action
                                ? "group"
                                : Action_Type_متن(action1->type));
                        log_err(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ڪنجي_Name_متن(keymap->ctx, ڪنجي->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ساكن فراغ
جوڙيو_ڪنجي_aliases(بنية xkb_keymap *keymap, darray_سائيز_t min, darray_سائيز_t max,
                بنية xkb_ڪنجي_alias *aliases)
{
    لكل (darray_سائيز_t alias = min; alias <= max; alias++) {
        ثابت Keycode_ميلاپ داخلا = keymap->ڪنجي_نالا[alias];
        إذا (داخلا.is_alias && داخلا.مليو) {
            *aliases = (بنية xkb_ڪنجي_alias) {
                .alias = alias,
                .real = داخلا.alias.real
            };
            aliases++;
        }
    }
}

ساكن bool
update_pending_ڪنجي_fields(بنية xkb_keymap_info *info, بنية xkb_ڪنجي *ڪنجي)
{
    إذا (ڪنجي->ٻاهريون_جو_range_pending_گروهه) {
        بنية pending_computation * ثابت pc = &darray_شي(
            *info->pending_computations, ڪنجي->ٻاهريون_جو_range_گروهه_عدد
        );
        إذا (!pc->computed) {
            xkb_layout_اشاريو_t گروهه = 0;
            بدل (اظهار_Resolve_گروهه(info, pc->اظهار, true, &گروهه, عدم)) {
            حالة PARSER_SUCCESS:
                pc->computed = true;
                pc->قدر = گروهه - 1;
                اكسر;
            حالة PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                        "Invalid key redirect group index\n");
                أرجع (info->strict & PARSER_NO_ميدان_TYPE_MISMATCH);
            افتراضي:
                /* Recover from error: ignore */;
            }
        }
        ڪنجي->ٻاهريون_جو_range_pending_گروهه = false;
        ڪنجي->ٻاهريون_جو_range_گروهه_عدد = pc->قدر;
    }
    أرجع true;
}

ساكن bool
update_pending_action_fields(بنية xkb_keymap_info *info,
                             xkb_keycode_t keycode, اتحاد xkb_action *act)
{
    بدل (act->type) {
    حالة ACTION_TYPE_گروهه_سيٽ:
    حالة ACTION_TYPE_گروهه_LATCH:
    حالة ACTION_TYPE_گروهه_تالو:
        إذا (act->گروهه.flags & ACTION_PENDING_COMPUTATION) {
            بنية pending_computation * ثابت pc =
                &darray_شي(*info->pending_computations, act->گروهه.گروهه);
            إذا (!pc->computed) {
                xkb_layout_اشاريو_t گروهه = 0;
                ثابت bool absolute =
                    (act->گروهه.flags & ACTION_ABSOLUTE_SWITCH);
                بدل (اظهار_Resolve_گروهه(info, pc->اظهار, absolute, &گروهه, عدم)) {
                حالة PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                            "Invalid action group index\n");
                    أرجع false;
                حالة PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    اكسر;
                افتراضي:
                    pc->computed = true;
                    إذا (absolute) {
                        pc->قدر = گروهه - 1;
                    } وإلا {
                        pc->قدر = گروهه;
                        إذا (pc->اظهار->common.type == STMT_اظهار_NEGATE)
                            pc->قدر = (uint32_t) (-(int32_t) pc->قدر);
                    }
                }
            }
            act->گروهه.گروهه = (int32_t) pc->قدر;
            act->گروهه.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        أرجع true;
    حالة ACTION_TYPE_REDIRECT_ڪنجي: {
        إذا (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ڪنجي_auto) {
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
            &darray_شي(*info->pending_computations, led->groups);
        إذا (!pc->computed) {
            xkb_layout_نقاب_t نقاب = 0;
            إذا (!اظهار_Resolve_گروهه_نقاب(info, pc->اظهار, &نقاب, عدم)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                        "Invalid LED group mask\n");
                أرجع false;
            }
            pc->computed = true;
            pc->قدر = نقاب;
        }
        led->pending_groups = false;
        led->groups = pc->قدر;
    }
    أرجع true;
}

/** Indices for the array "groupIndexNames" */
تعداد {
    گروهه_اشاريو_NAME_آخري = 1,
};

/** Indices for the array "groupMaskNames" */
تعداد {
    گروهه_نقاب_NAME_آخري = 3,
};

ساكن فراغ
update_auto_گروهه_تالو_on_modifier_release(بنية xkb_level * محدود level0,
                                           xkb_level_اشاريو_t l,
                                           اتحاد xkb_action * محدود action)
{
    إذا (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].انگ_actions == 1 &&
        action->type == ACTION_TYPE_گروهه_تالو &&
        /* lockOnRelease not set explicitly */
        !(action->گروهه.flags & ACTION_EXPLICIT_تالو_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->انگ_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_سيٽ))
    {
        أرجع;
    }

    action->گروهه.flags |= ACTION_AUTO_گروهه_تالو_ON_MODIFIER_RELEASE;
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
#جيڪڏهن_وصف_ٿيل _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(حجم(بنية xkb_ڪنجي_alias) == حجم(Keycode_ميلاپ), "");
#وإلا
    static_assert(حجم(بنية xkb_ڪنجي_alias) == 2 * حجم(Keycode_ميلاپ), "");
#جيڪڏهن_پورو
    darray_سائيز_t انگ_ڪنجي_aliases = 0;
    darray_سائيز_t min_alias = 0;
    darray_سائيز_t max_alias = 0;
    لكل (xkb_atom_t alias = 0; alias < keymap->انگ_ڪنجي_نالا; alias++) {
        ثابت Keycode_ميلاپ داخلا = keymap->ڪنجي_نالا[alias];
        إذا (داخلا.is_alias && داخلا.مليو) {
            إذا (!انگ_ڪنجي_aliases)
                min_alias = alias;
            max_alias = alias;
            انگ_ڪنجي_aliases++;
        }
    }
    إذا (انگ_ڪنجي_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ثابت darray_سائيز_t required_space = حجم(بنية xkb_ڪنجي_alias)
                                           / حجم(Keycode_ميلاپ)
                                           * انگ_ڪنجي_aliases;
        أكد(انگ_ڪنجي_aliases <= keymap->انگ_ڪنجي_نالا);
        إذا (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            جوڙيو_ڪنجي_aliases(keymap, min_alias, max_alias, keymap->ڪنجي_aliases);
            /* Shrink */
            بنية xkb_ڪنجي_alias * ثابت r =
                realloc(keymap->ڪنجي_aliases,
                        انگ_ڪنجي_aliases * حجم(*keymap->ڪنجي_aliases));
            إذا (!r)
                أرجع false;
            keymap->ڪنجي_aliases = r;
        } وإلا إذا (keymap->انگ_ڪنجي_نالا - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            بنية xkb_ڪنجي_alias * ثابت aliases = (بنية xkb_ڪنجي_alias *) (
                keymap->ڪنجي_نالا + max_alias + 1 +
                !is_aligned(keymap->ڪنجي_نالا + max_alias + 1, حجم(*aliases))
            );
            جوڙيو_ڪنجي_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            يادگيري_نقل(keymap->ڪنجي_aliases, aliases,
                   انگ_ڪنجي_aliases * حجم(*keymap->ڪنجي_aliases));
            /* Shrink */
            بنية xkb_ڪنجي_alias * ثابت r =
                realloc(keymap->ڪنجي_aliases,
                        انگ_ڪنجي_aliases * حجم(*keymap->ڪنجي_aliases));
            إذا (!r)
                أرجع false;
            keymap->ڪنجي_aliases = r;
        } وإلا {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            بنية xkb_ڪنجي_alias * ثابت aliases = صفر_يادگيري_ڏيو(انگ_ڪنجي_aliases,
                                                          حجم(*aliases));
            إذا (!aliases)
                أرجع false;
            جوڙيو_ڪنجي_aliases(keymap, min_alias, max_alias, aliases);
            حرر(keymap->ڪنجي_نالا);
            keymap->ڪنجي_aliases = aliases;
        }
    }
    keymap->انگ_ڪنجي_aliases = انگ_ڪنجي_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    بنية xkb_ڪنجي *ڪنجي;
    xkb_ڪنجيون_foreach(ڪنجي, keymap)
        keymap->انگ_groups = MAX(keymap->انگ_groups, ڪنجي->انگ_groups);

    ثابت bool pending_computations = !darray_خالي_جايون(*info->pending_computations);

    إذا (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ثابت xkb_layout_اشاريو_t انگ_groups
            = (keymap->انگ_groups)
            ? keymap->انگ_groups
            : 1; /* default to the “first” layout */
        info->lookup.گروهه_اشاريو_نالا[گروهه_اشاريو_NAME_آخري] =
            (Lookup_داخلا) {
                .name = گروهه_آخري_اشاريو_NAME,
                .قدر = انگ_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.گروهه_نقاب_نالا[گروهه_نقاب_NAME_آخري] =
            (Lookup_داخلا) {
                .name = گروهه_آخري_اشاريو_NAME,
                .قدر = UINT32_C(1) << (انگ_groups - 1)
            };

        /* Update interpret fields with pending computations */
        لكل (darray_سائيز_t i = 0; i < keymap->انگ_sym_interprets; i++) {
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
    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        إذا (!Apply_Interps_منزل_ڪنجي(keymap, ڪنجي))
            أرجع false;
        Check_گڻو_Actions_Categories(keymap, ڪنجي);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_اشاريو_t idx;
    بنية xkb_mod *mod;
    xkb_ڪنجيون_foreach(ڪنجي, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            إذا (ڪنجي->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ڪنجي->modmap;

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
            ثابت xkb_mod_نقاب_t نقاب = UINT32_C(1) << idx;
            إذا (mod->mapping == 0 && !(keymap->mods.explicit_vmods & نقاب)) {
                mod->mapping = نقاب;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= نقاب;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    أكد(keymap->canonical_state_نقاب == MOD_REAL_نقاب_ALL);
    xkb_mod_نقاب_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_نقاب |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    لكل (darray_سائيز_t i = 0; i < keymap->انگ_types; i++) {
        Compute_Effective_نقاب(keymap, &keymap->types[i].mods);

        لكل (darray_سائيز_t j = 0; j < keymap->types[i].انگ_entries; j++) {
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
                keymap->types[i].entries[j].mods.نقاب = 0;
                استمر;
            }
            Compute_Effective_نقاب(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_نقاب(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        إذا (!update_pending_ڪنجي_fields(info, ڪنجي))
            أرجع false;
        لكل (xkb_layout_اشاريو_t i = 0; i < ڪنجي->انگ_groups; i++) {
            لكل (xkb_level_اشاريو_t j = 0; j < Xkb_ڪنجي_انگ_سطحون(ڪنجي, i); j++) {
                بنية xkb_level * ثابت level = &ڪنجي->groups[i].سطحون[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ڪنجي->modmap);
                    إذا ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ڪنجي) &&
                        !update_pending_action_fields(info, ڪنجي->keycode, act))
                    {
                        أرجع false;
                    }
                    update_auto_گروهه_تالو_on_modifier_release(
                        ڪنجي->groups[i].سطحون, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    بنية xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_نقاب(keymap, &led->mods); /* vmod -> led maps */
        إذا (pending_computations &&
            !update_pending_led_fields(info, led))
            أرجع false;
    }

    أرجع true;
}

تعريفنوع bool (*compile_file_fn)(XkbFile *file, بنية xkb_keymap_info *info);

ساكن ثابت compile_file_fn compile_file_fns[آخري_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ڪنجي_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ساكن فراغ
pending_computations_array_free(pending_computation_array *p)
{
    بنية pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->اظهار);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, بنية xkb_keymap *keymap)
{
    XkbFile *files[آخري_KEYMAP_FILE_TYPE + 1] = { عدم };
    تعداد xkb_file_type type;
    بنية xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    لكل (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.ايندڙ) {
        إذا (file->file_type < پهريون_KEYMAP_FILE_TYPE ||
            file->file_type > آخري_KEYMAP_FILE_TYPE) {
            إذا (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } وإلا {
                log_err(ctx, XKB_LOG_پيغام_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_منزل_string(file->file_type));
            }
            استمر;
        }

        إذا (files[file->file_type]) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
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
            .controls_name_وٿي = format_control_نالا_وٿي(keymap->format),
            .گروهه_تالو_on_release =
                is_گروهه_تالو_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_تالو_On_Press_Supported(keymap->format),
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
            .گروهه_اشاريو_نالا = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [گروهه_اشاريو_NAME_آخري] = {
                    keymap->انگ_groups ? گروهه_آخري_اشاريو_NAME : عدم,
                    keymap->انگ_groups /* 1-indexed */
                },
                { عدم, 0 }
            },
            .گروهه_نقاب_نالا = {
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
                [گروهه_نقاب_NAME_آخري] = {
                    keymap->انگ_groups ? گروهه_آخري_اشاريو_NAME : عدم,
                    /* Be extra cautious */
                    keymap->انگ_groups && keymap->انگ_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->انگ_groups - 1))
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
    لكل (type = پهريون_KEYMAP_FILE_TYPE;
         type <= آخري_KEYMAP_FILE_TYPE;
         type++) {
        إذا (files[type] == عدم) {
            log_dbg(ctx, XKB_LOG_پيغام_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_منزل_string(type));
        } وإلا {
            log_dbg(ctx, XKB_LOG_پيغام_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_منزل_string(type),
                    محفوظ_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ثابت bool ok = compile_file_fns[type](files[type], &info);
        إذا (!ok) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
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
