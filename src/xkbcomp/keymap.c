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

ساکن bool
has_unbound_vmods(ساخت xkb_keymap *keymap, xkb_mod_نقاب_t نقاب)
{
    xkb_mod_اشاریہ_t k;
    ساخت xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        اگر ((نقاب & (UINT32_C(1) << k)) && mod->mapping == 0)
            واپس true;
    واپس false;
}

ساکن اندرونی خالی
Compute_Effective_نقاب(ساخت xkb_keymap *keymap, ساخت xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ثابت xkb_mod_نقاب_t unknown_mods = ~(xkb_mod_نقاب_t)
        ((UINT64_C(1) << keymap->mods.عدد_mods) - UINT64_C(1));
    mods->نقاب = mod_نقاب_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ساکن خالی
UpdateActionMods(ساخت xkb_keymap *keymap, اتحاد xkb_action *act,
                 xkb_mod_نقاب_t modmap)
{
    انتخاب (act->type) {
    حالت ACTION_TYPE_MOD_سیٹ:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_تالا:
        اگر (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_نقاب(keymap, &act->mods.mods);
        توڑو;
    طےشدہ:
        توڑو;
    }
}

ساکن ثابت ساخت xkb_sym_interpret default_interpret = {
    .sym = XKB_کلید_No_علامت /* unused */,
    .مطابقت = مطابقت_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_کلید_REPEAT,
    .virtual_mod = (xkb_mod_اشاریہ_t) DEFAULT_INTERPRET_VMOD,
    .عدد_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

قسمتعریف darray(ثابت ساخت xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ساکن bool
ڈھونڈو_Interp_For_کلید(ساخت xkb_keymap *keymap, ثابت ساخت xkb_کلید *کلید,
                 xkb_layout_اشاریہ_t گروہ, xkb_level_اشاریہ_t level,
                 xkb_sym_interprets *interprets)
{
    ثابت xkb_keysym_t *syms;
    صحیح عدد_syms;

    عدد_syms = xkb_keymap_کلید_get_syms_by_level(keymap, کلید->keycode, گروہ,
                                                level, &syms);
    اگر (عدد_syms <= 0)
        واپس false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    برائے (صحیح s = 0; s < عدد_syms; s++) {
        bool ملا = false;
        برائے (darray_سائز_t i = 0; i < keymap->عدد_sym_interprets; i++) {
            ساخت xkb_sym_interpret * ثابت interp = &keymap->sym_interprets[i];
            xkb_mod_نقاب_t mods;

            ملا = false;

            اگر (interp->sym != syms[s] && interp->sym != XKB_کلید_No_علامت)
                جاری;

            اگر (interp->level_one_only && level != 0)
                mods = 0;
            ورنہ
                mods = کلید->modmap;

            انتخاب (interp->مطابقت) {
            حالت مطابقت_NONE:
                ملا = !(interp->mods & mods);
                توڑو;
            حالت مطابقت_ANY_OR_NONE:
                ملا = (!mods || (interp->mods & mods));
                توڑو;
            حالت مطابقت_ANY:
                ملا = (interp->mods & mods);
                توڑو;
            حالت مطابقت_ALL:
                ملا = ((interp->mods & mods) == interp->mods);
                توڑو;
            حالت مطابقت_EXACTLY:
                ملا = (interp->mods == mods);
                توڑو;
            }

            اگر (ملا && i > 0 && interp->sym == XKB_کلید_No_علامت) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ساخت xkb_sym_interpret ثابت **پچھلا_interp;
                darray_foreach(پچھلا_interp, *interprets) {
                    اگر (*پچھلا_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        ملا = false;
                        log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_متن(keymap->ctx, syms[s]),
                                 level + 1, گروہ + 1,
                                 کلید_Name_متن(keymap->ctx, کلید->name));
                        جاؤ not_ملا;
                    }
                }
            }
            اگر (ملا) {
                darray_append(*interprets, interp);
                interp->required = true;
                توڑو;
            }
        }
        اگر (!ملا)
not_ملا:
            darray_append(*interprets, &default_interpret);
    }
    واپس true;
}

ساکن bool
Apply_Interps_مقصد_کلید(ساخت xkb_keymap *keymap, ساخت xkb_کلید *کلید)
{
    xkb_mod_نقاب_t vmodmap = 0;
    xkb_level_اشاریہ_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(اتحاد xkb_action) actions = darray_new();

    برائے (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
        /* Skip any interpretation for this group if it has explicit actions */
        اگر (کلید->groups[گروہ].explicit_actions)
            جاری;

        برائے (level = 0; level < Xkb_کلید_عدد_سطحیں(کلید, گروہ); level++) {
            أكد(کلید->groups[گروہ].سطحیں[level].عدد_actions == 0);

            ثابت ساخت xkb_sym_interpret **interp_iter;
            ثابت ساخت xkb_sym_interpret *interp;
            سائز_t k;
            darray_resize(interprets, 0);

            ثابت bool ملا = ڈھونڈو_Interp_For_کلید(keymap, کلید, گروہ, level, &interprets);
            اگر (!ملا)
                جاری;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                اگر (گروہ == 0 && level == 0 &&
                    !(کلید->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    کلید->repeats = true;
                }

                اگر ((گروہ == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    اگر (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                انتخاب (interp->عدد_actions) {
                حالت 0:
                    توڑو;
                حالت 1:
                    darray_append(actions, interp->a.action);
                    توڑو;
                طےشدہ:
                    darray_append_اشیاء(actions, interp->a.actions,
                                        interp->عدد_actions);
                }
            }

            /* Copy the actions */
            اگر (unlikely(darray_سائز(actions)) > MAX_ACTIONS_فی_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         کلید_Name_متن(keymap->ctx, کلید->name),
                         MAX_ACTIONS_فی_LEVEL, darray_سائز(actions));
                کلید->groups[گروہ].سطحیں[level].عدد_actions =
                    MAX_ACTIONS_فی_LEVEL;
            } ورنہ {
                کلید->groups[گروہ].سطحیں[level].عدد_actions =
                    (xkb_action_گنتی_t) darray_سائز(actions);
            }
            انتخاب (darray_سائز(actions)) {
                حالت 0:
                    کلید->groups[گروہ].سطحیں[level].a.action =
                        (اتحاد xkb_action) { .type = ACTION_TYPE_NONE };
                    توڑو;
                حالت 1:
                    کلید->groups[گروہ].سطحیں[level].a.action =
                        darray_شے(actions, 0);
                    توڑو;
                طےشدہ:
                    کلید->groups[گروہ].سطحیں[level].a.actions =
                        memdup(darray_اشیاء(actions),
                               کلید->groups[گروہ].سطحیں[level].عدد_actions,
                               حجم(*darray_اشیاء(actions)));
                    اگر (!کلید->groups[گروہ].سطحیں[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        واپس false;
                    }
            }

            اگر (!darray_خالی_خانے(actions))
                کلید->groups[گروہ].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        اگر (کلید->groups[گروہ].implicit_actions)
            کلید->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    اگر (!(کلید->explicit & EXPLICIT_VMODMAP))
        کلید->vmodmap = vmodmap;

    واپس true;
}

ساکن اندرونی bool
is_mod_action(اتحاد xkb_action *action)
{
    واپس action->type == ACTION_TYPE_MOD_سیٹ ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_تالا;
}

ساکن اندرونی bool
is_گروہ_action(اتحاد xkb_action *action)
{
    واپس action->type == ACTION_TYPE_گروہ_سیٹ ||
           action->type == ACTION_TYPE_گروہ_LATCH ||
           action->type == ACTION_TYPE_گروہ_تالا;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ساکن خالی
Check_مضرب_Actions_Categories(ساخت xkb_keymap *keymap, ساخت xkb_کلید *کلید)
{
    برائے (xkb_layout_اشاریہ_t g = 0; g < کلید->عدد_groups; g++) {
        برائے (xkb_level_اشاریہ_t l = 0; l < Xkb_کلید_عدد_سطحیں(کلید, g); l++) {
            ساخت xkb_level *level = &کلید->groups[g].سطحیں[l];
            اگر (level->عدد_actions <= 1)
                جاری;
            برائے (xkb_action_گنتی_t i = 0; i < level->عدد_actions; i++) {
                اتحاد xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool گروہ_action = is_گروہ_action(action1);
                اگر (!(mod_action || گروہ_action ||
                      action1->type == ACTION_TYPE_REDIRECT_کلید))
                    جاری;
                برائے (xkb_action_گنتی_t j = i + 1; j < level->عدد_actions; j++) {
                    اتحاد xkb_action *action2 = &level->a.actions[j];
                    اگر (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (گروہ_action && is_گروہ_action(action2)))
                    {
                        ثابت حرف * ثابت type = (mod_action)
                            ? "modifiers"
                            : (گروہ_action
                                ? "group"
                                : Action_Type_متن(action1->type));
                        log_err(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, کلید_Name_متن(keymap->ctx, کلید->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ساکن خالی
جوڑو_کلید_aliases(ساخت xkb_keymap *keymap, darray_سائز_t min, darray_سائز_t max,
                ساخت xkb_کلید_alias *aliases)
{
    برائے (darray_سائز_t alias = min; alias <= max; alias++) {
        ثابت Keycode_مطابقت اندراج_2 = keymap->کلید_نام[alias];
        اگر (اندراج_2.is_alias && اندراج_2.ملا) {
            *aliases = (ساخت xkb_کلید_alias) {
                .alias = alias,
                .real = اندراج_2.alias.real
            };
            aliases++;
        }
    }
}

ساکن bool
update_pending_کلید_fields(ساخت xkb_keymap_info *info, ساخت xkb_کلید *کلید)
{
    اگر (کلید->برآمد_کا_range_pending_گروہ) {
        ساخت pending_computation * ثابت pc = &darray_شے(
            *info->pending_computations, کلید->برآمد_کا_range_گروہ_نمبر
        );
        اگر (!pc->computed) {
            xkb_layout_اشاریہ_t گروہ = 0;
            انتخاب (اظہاریہ_Resolve_گروہ(info, pc->اظہاریہ, true, &گروہ, تہی)) {
            حالت PARSER_SUCCESS:
                pc->computed = true;
                pc->قدر = گروہ - 1;
                توڑو;
            حالت PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Invalid key redirect group index\n");
                واپس (info->strict & PARSER_NO_میدان_TYPE_MISMATCH);
            طےشدہ:
                /* Recover from error: ignore */;
            }
        }
        کلید->برآمد_کا_range_pending_گروہ = false;
        کلید->برآمد_کا_range_گروہ_نمبر = pc->قدر;
    }
    واپس true;
}

ساکن bool
update_pending_action_fields(ساخت xkb_keymap_info *info,
                             xkb_keycode_t keycode, اتحاد xkb_action *act)
{
    انتخاب (act->type) {
    حالت ACTION_TYPE_گروہ_سیٹ:
    حالت ACTION_TYPE_گروہ_LATCH:
    حالت ACTION_TYPE_گروہ_تالا:
        اگر (act->گروہ.flags & ACTION_PENDING_COMPUTATION) {
            ساخت pending_computation * ثابت pc =
                &darray_شے(*info->pending_computations, act->گروہ.گروہ);
            اگر (!pc->computed) {
                xkb_layout_اشاریہ_t گروہ = 0;
                ثابت bool absolute =
                    (act->گروہ.flags & ACTION_ABSOLUTE_SWITCH);
                انتخاب (اظہاریہ_Resolve_گروہ(info, pc->اظہاریہ, absolute, &گروہ, تہی)) {
                حالت PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                            "Invalid action group index\n");
                    واپس false;
                حالت PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    توڑو;
                طےشدہ:
                    pc->computed = true;
                    اگر (absolute) {
                        pc->قدر = گروہ - 1;
                    } ورنہ {
                        pc->قدر = گروہ;
                        اگر (pc->اظہاریہ->common.type == STMT_اظہاریہ_NEGATE)
                            pc->قدر = (uint32_t) (-(int32_t) pc->قدر);
                    }
                }
            }
            act->گروہ.گروہ = (int32_t) pc->قدر;
            act->گروہ.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        واپس true;
    حالت ACTION_TYPE_REDIRECT_کلید: {
        اگر (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_کلید_auto) {
            /* No auto value to set */
            واپس true;
        } ورنہ {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        واپس true;
    }
    طےشدہ:
        واپس true;
    }
}

ساکن bool
update_pending_led_fields(ساخت xkb_keymap_info *info, ساخت xkb_led *led)
{
    اگر (led->pending_groups) {
        ساخت pending_computation * ثابت pc =
            &darray_شے(*info->pending_computations, led->groups);
        اگر (!pc->computed) {
            xkb_layout_نقاب_t نقاب = 0;
            اگر (!اظہاریہ_Resolve_گروہ_نقاب(info, pc->اظہاریہ, &نقاب, تہی)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Invalid LED group mask\n");
                واپس false;
            }
            pc->computed = true;
            pc->قدر = نقاب;
        }
        led->pending_groups = false;
        led->groups = pc->قدر;
    }
    واپس true;
}

/** Indices for the array "groupIndexNames" */
شمار {
    گروہ_اشاریہ_NAME_آخری = 1,
};

/** Indices for the array "groupMaskNames" */
شمار {
    گروہ_نقاب_NAME_آخری = 3,
};

ساکن خالی
update_auto_گروہ_تالا_on_modifier_release(ساخت xkb_level * محدود level0,
                                           xkb_level_اشاریہ_t l,
                                           اتحاد xkb_action * محدود action)
{
    اگر (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].عدد_actions == 1 &&
        action->type == ACTION_TYPE_گروہ_تالا &&
        /* lockOnRelease not set explicitly */
        !(action->گروہ.flags & ACTION_EXPLICIT_تالا_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->عدد_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_سیٹ))
    {
        واپس;
    }

    action->گروہ.flags |= ACTION_AUTO_گروہ_تالا_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ساکن bool
UpdateDerivedKeymapFields(ساخت xkb_keymap_info *info)
{
    ساخت xkb_keymap * محدود ثابت keymap = &info->keymap;

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
#اگر_تعریف_شدہ _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(حجم(ساخت xkb_کلید_alias) == حجم(Keycode_مطابقت), "");
#ورنہ
    static_assert(حجم(ساخت xkb_کلید_alias) == 2 * حجم(Keycode_مطابقت), "");
#اگر_ختم
    darray_سائز_t عدد_کلید_aliases = 0;
    darray_سائز_t min_alias = 0;
    darray_سائز_t max_alias = 0;
    برائے (xkb_atom_t alias = 0; alias < keymap->عدد_کلید_نام; alias++) {
        ثابت Keycode_مطابقت اندراج_2 = keymap->کلید_نام[alias];
        اگر (اندراج_2.is_alias && اندراج_2.ملا) {
            اگر (!عدد_کلید_aliases)
                min_alias = alias;
            max_alias = alias;
            عدد_کلید_aliases++;
        }
    }
    اگر (عدد_کلید_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ثابت darray_سائز_t required_space = حجم(ساخت xkb_کلید_alias)
                                           / حجم(Keycode_مطابقت)
                                           * عدد_کلید_aliases;
        أكد(عدد_کلید_aliases <= keymap->عدد_کلید_نام);
        اگر (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            جوڑو_کلید_aliases(keymap, min_alias, max_alias, keymap->کلید_aliases);
            /* Shrink */
            ساخت xkb_کلید_alias * ثابت r =
                realloc(keymap->کلید_aliases,
                        عدد_کلید_aliases * حجم(*keymap->کلید_aliases));
            اگر (!r)
                واپس false;
            keymap->کلید_aliases = r;
        } ورنہ اگر (keymap->عدد_کلید_نام - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ساخت xkb_کلید_alias * ثابت aliases = (ساخت xkb_کلید_alias *) (
                keymap->کلید_نام + max_alias + 1 +
                !is_aligned(keymap->کلید_نام + max_alias + 1, حجم(*aliases))
            );
            جوڑو_کلید_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            حافظہ_نقل(keymap->کلید_aliases, aliases,
                   عدد_کلید_aliases * حجم(*keymap->کلید_aliases));
            /* Shrink */
            ساخت xkb_کلید_alias * ثابت r =
                realloc(keymap->کلید_aliases,
                        عدد_کلید_aliases * حجم(*keymap->کلید_aliases));
            اگر (!r)
                واپس false;
            keymap->کلید_aliases = r;
        } ورنہ {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ساخت xkb_کلید_alias * ثابت aliases = صفر_حافظہ_دو(عدد_کلید_aliases,
                                                          حجم(*aliases));
            اگر (!aliases)
                واپس false;
            جوڑو_کلید_aliases(keymap, min_alias, max_alias, aliases);
            آزاد(keymap->کلید_نام);
            keymap->کلید_aliases = aliases;
        }
    }
    keymap->عدد_کلید_aliases = عدد_کلید_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ساخت xkb_کلید *کلید;
    xkb_کلیدیں_foreach(کلید, keymap)
        keymap->عدد_groups = MAX(keymap->عدد_groups, کلید->عدد_groups);

    ثابت bool pending_computations = !darray_خالی_خانے(*info->pending_computations);

    اگر (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ثابت xkb_layout_اشاریہ_t عدد_groups
            = (keymap->عدد_groups)
            ? keymap->عدد_groups
            : 1; /* default to the “first” layout */
        info->lookup.گروہ_اشاریہ_نام[گروہ_اشاریہ_NAME_آخری] =
            (Lookup_اندراج) {
                .name = گروہ_آخری_اشاریہ_NAME,
                .قدر = عدد_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.گروہ_نقاب_نام[گروہ_نقاب_NAME_آخری] =
            (Lookup_اندراج) {
                .name = گروہ_آخری_اشاریہ_NAME,
                .قدر = UINT32_C(1) << (عدد_groups - 1)
            };

        /* Update interpret fields with pending computations */
        برائے (darray_سائز_t i = 0; i < keymap->عدد_sym_interprets; i++) {
            ساخت xkb_sym_interpret * محدود ثابت interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                اگر (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    واپس false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_کلیدیں_foreach(کلید, keymap) {
        اگر (!Apply_Interps_مقصد_کلید(keymap, کلید))
            واپس false;
        Check_مضرب_Actions_Categories(keymap, کلید);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_اشاریہ_t idx;
    ساخت xkb_mod *mod;
    xkb_کلیدیں_foreach(کلید, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            اگر (کلید->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= کلید->modmap;

    اگر (keymap->format >= XKB_KEYMAP_FORMAT_متن_V2) {
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
            اگر (mod->mapping == 0 && !(keymap->mods.explicit_vmods & نقاب)) {
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
    برائے (darray_سائز_t i = 0; i < keymap->عدد_types; i++) {
        Compute_Effective_نقاب(keymap, &keymap->types[i].mods);

        برائے (darray_سائز_t j = 0; j < keymap->types[i].عدد_entries; j++) {
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
                keymap->types[i].entries[j].mods.نقاب = 0;
                جاری;
            }
            Compute_Effective_نقاب(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_نقاب(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_کلیدیں_foreach(کلید, keymap) {
        اگر (!update_pending_کلید_fields(info, کلید))
            واپس false;
        برائے (xkb_layout_اشاریہ_t i = 0; i < کلید->عدد_groups; i++) {
            برائے (xkb_level_اشاریہ_t j = 0; j < Xkb_کلید_عدد_سطحیں(کلید, i); j++) {
                ساخت xkb_level * ثابت level = &کلید->groups[i].سطحیں[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, کلید->modmap);
                    اگر ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_کلید) &&
                        !update_pending_action_fields(info, کلید->keycode, act))
                    {
                        واپس false;
                    }
                    update_auto_گروہ_تالا_on_modifier_release(
                        کلید->groups[i].سطحیں, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ساخت xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_نقاب(keymap, &led->mods); /* vmod -> led maps */
        اگر (pending_computations &&
            !update_pending_led_fields(info, led))
            واپس false;
    }

    واپس true;
}

قسمتعریف bool (*compile_file_fn)(XkbFile *file, ساخت xkb_keymap_info *info);

ساکن ثابت compile_file_fn compile_file_fns[آخری_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_کلید_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ساکن خالی
pending_computations_array_free(pending_computation_array *p)
{
    ساخت pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->اظہاریہ);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ساخت xkb_keymap *keymap)
{
    XkbFile *files[آخری_KEYMAP_FILE_TYPE + 1] = { تہی };
    شمار xkb_file_type type;
    ساخت xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    برائے (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.اگلا) {
        اگر (file->file_type < پہلا_KEYMAP_FILE_TYPE ||
            file->file_type > آخری_KEYMAP_FILE_TYPE) {
            اگر (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } ورنہ {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_مقصد_string(file->file_type));
            }
            جاری;
        }

        اگر (files[file->file_type]) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_مقصد_string(file->file_type));
            جاری;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ساخت xkb_keymap_info info = {
        /* Copy the keymap */
        .keymap = *keymap,
        .strict = parser_strict_flags_from_keymap(keymap),
        .features = {
            .max_groups = format_max_groups(keymap->format),
            .max_overlays = format_max_overlays(keymap->format),
            .controls_name_انحراف = format_control_نام_انحراف(keymap->format),
            .گروہ_تالا_on_release =
                is_گروہ_تالا_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_تالا_On_Press_Supported(keymap->format),
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
            .گروہ_اشاریہ_نام = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [گروہ_اشاریہ_NAME_آخری] = {
                    keymap->عدد_groups ? گروہ_آخری_اشاریہ_NAME : تہی,
                    keymap->عدد_groups /* 1-indexed */
                },
                { تہی, 0 }
            },
            .گروہ_نقاب_نام = {
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
                [گروہ_نقاب_NAME_آخری] = {
                    keymap->عدد_groups ? گروہ_آخری_اشاریہ_NAME : تہی,
                    /* Be extra cautious */
                    keymap->عدد_groups && keymap->عدد_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->عدد_groups - 1))
                        : 0
                },
                { تہی, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    برائے (type = پہلا_KEYMAP_FILE_TYPE;
         type <= آخری_KEYMAP_FILE_TYPE;
         type++) {
        اگر (files[type] == تہی) {
            log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_مقصد_string(type));
        } ورنہ {
            log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_مقصد_string(type),
                    محفوظ_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ثابت bool ok = compile_file_fns[type](files[type], &info);
        اگر (!ok) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_مقصد_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            واپس false;
        }
    }

    ثابت bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    واپس ok;
}
