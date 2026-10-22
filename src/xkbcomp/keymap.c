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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast-build.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

ਸਥਿਰ bool
has_unbound_vmods(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਨਕਾਬ_t ਨਕਾਬ)
{
    xkb_mod_ਸੂਚਕਾਂਕ_t k;
    ਬਣਤਰ xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        ਜੇ ((ਨਕਾਬ & (UINT32_C(1) << k)) && mod->mapping == 0)
            ਵਾਪਸ true;
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
Compute_Effective_ਨਕਾਬ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t unknown_mods = ~(xkb_mod_ਨਕਾਬ_t)
        ((UINT64_C(1) << keymap->mods.ਅੰਕ_mods) - UINT64_C(1));
    mods->ਨਕਾਬ = mod_ਨਕਾਬ_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ਸਥਿਰ ਖਾਲੀ
UpdateActionMods(ਬਣਤਰ xkb_keymap *keymap, ਸੰਘ xkb_action *act,
                 xkb_mod_ਨਕਾਬ_t modmap)
{
    ਚੋਣ (act->type) {
    ਹਾਲਤ ACTION_TYPE_MOD_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_MOD_LATCH:
    ਹਾਲਤ ACTION_TYPE_MOD_ਤਾਲਾ:
        ਜੇ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_ਨਕਾਬ(keymap, &act->mods.mods);
        ਰੋਕੋ;
    ਮੂਲ:
        ਰੋਕੋ;
    }
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_sym_interpret default_interpret = {
    .sym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ /* unused */,
    .ਮੇਲ = ਮੇਲ_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ਕੁੰਜੀ_REPEAT,
    .virtual_mod = (xkb_mod_ਸੂਚਕਾਂਕ_t) DEFAULT_INTERPRET_VMOD,
    .ਅੰਕ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਅਚਲ ਬਣਤਰ xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ਸਥਿਰ bool
ਲੱਭੋ_Interp_For_ਕੁੰਜੀ(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                 xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ, xkb_level_ਸੂਚਕਾਂਕ_t level,
                 xkb_sym_interprets *interprets)
{
    ਅਚਲ xkb_keysym_t *syms;
    ਪੂਰਨ ਅੰਕ_syms;

    ਅੰਕ_syms = xkb_keymap_ਕੁੰਜੀ_get_syms_by_level(keymap, ਕੁੰਜੀ->keycode, ਸਮੂਹ,
                                                level, &syms);
    ਜੇ (ਅੰਕ_syms <= 0)
        ਵਾਪਸ false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    ਲਈ (ਪੂਰਨ s = 0; s < ਅੰਕ_syms; s++) {
        bool ਮਿਲਿਆ = false;
        ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_sym_interprets; i++) {
            ਬਣਤਰ xkb_sym_interpret * ਅਚਲ interp = &keymap->sym_interprets[i];
            xkb_mod_ਨਕਾਬ_t mods;

            ਮਿਲਿਆ = false;

            ਜੇ (interp->sym != syms[s] && interp->sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                ਜਾਰੀ;

            ਜੇ (interp->level_one_only && level != 0)
                mods = 0;
            ਨਹੀਂਤਾਂ
                mods = ਕੁੰਜੀ->modmap;

            ਚੋਣ (interp->ਮੇਲ) {
            ਹਾਲਤ ਮੇਲ_NONE:
                ਮਿਲਿਆ = !(interp->mods & mods);
                ਰੋਕੋ;
            ਹਾਲਤ ਮੇਲ_ANY_OR_NONE:
                ਮਿਲਿਆ = (!mods || (interp->mods & mods));
                ਰੋਕੋ;
            ਹਾਲਤ ਮੇਲ_ANY:
                ਮਿਲਿਆ = (interp->mods & mods);
                ਰੋਕੋ;
            ਹਾਲਤ ਮੇਲ_ALL:
                ਮਿਲਿਆ = ((interp->mods & mods) == interp->mods);
                ਰੋਕੋ;
            ਹਾਲਤ ਮੇਲ_EXACTLY:
                ਮਿਲਿਆ = (interp->mods == mods);
                ਰੋਕੋ;
            }

            ਜੇ (ਮਿਲਿਆ && i > 0 && interp->sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ਬਣਤਰ xkb_sym_interpret ਅਚਲ **ਪਿਛਲਾ_interp;
                darray_foreach(ਪਿਛਲਾ_interp, *interprets) {
                    ਜੇ (*ਪਿਛਲਾ_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        ਮਿਲਿਆ = false;
                        log_warn(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_ਪਾਠ(keymap->ctx, syms[s]),
                                 level + 1, ਸਮੂਹ + 1,
                                 ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name));
                        ਜਾਓ not_ਮਿਲਿਆ;
                    }
                }
            }
            ਜੇ (ਮਿਲਿਆ) {
                darray_append(*interprets, interp);
                interp->required = true;
                ਰੋਕੋ;
            }
        }
        ਜੇ (!ਮਿਲਿਆ)
not_ਮਿਲਿਆ:
            darray_append(*interprets, &default_interpret);
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Apply_Interps_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    xkb_mod_ਨਕਾਬ_t vmodmap = 0;
    xkb_level_ਸੂਚਕਾਂਕ_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(ਸੰਘ xkb_action) actions = darray_new();

    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
        /* Skip any interpretation for this group if it has explicit actions */
        ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].explicit_actions)
            ਜਾਰੀ;

        ਲਈ (level = 0; level < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, ਸਮੂਹ); level++) {
            ਪ੍ਰਤਿਜ੍ਞਾ(ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions == 0);

            ਅਚਲ ਬਣਤਰ xkb_sym_interpret **interp_iter;
            ਅਚਲ ਬਣਤਰ xkb_sym_interpret *interp;
            ਮਾਪ_t k;
            darray_resize(interprets, 0);

            ਅਚਲ bool ਮਿਲਿਆ = ਲੱਭੋ_Interp_For_ਕੁੰਜੀ(keymap, ਕੁੰਜੀ, ਸਮੂਹ, level, &interprets);
            ਜੇ (!ਮਿਲਿਆ)
                ਜਾਰੀ;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                ਜੇ (ਸਮੂਹ == 0 && level == 0 &&
                    !(ਕੁੰਜੀ->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ਕੁੰਜੀ->repeats = true;
                }

                ਜੇ ((ਸਮੂਹ == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    ਜੇ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                ਚੋਣ (interp->ਅੰਕ_actions) {
                ਹਾਲਤ 0:
                    ਰੋਕੋ;
                ਹਾਲਤ 1:
                    darray_append(actions, interp->a.action);
                    ਰੋਕੋ;
                ਮੂਲ:
                    darray_append_ਚੀਜ਼ਾਂ(actions, interp->a.actions,
                                        interp->ਅੰਕ_actions);
                }
            }

            /* Copy the actions */
            ਜੇ (unlikely(darray_ਮਾਪ(actions)) > MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name),
                         MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL, darray_ਮਾਪ(actions));
                ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions =
                    MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL;
            } ਨਹੀਂਤਾਂ {
                ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions =
                    (xkb_action_ਗਿਣਤੀ_t) darray_ਮਾਪ(actions);
            }
            ਚੋਣ (darray_ਮਾਪ(actions)) {
                ਹਾਲਤ 0:
                    ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].a.action =
                        (ਸੰਘ xkb_action) { .type = ACTION_TYPE_NONE };
                    ਰੋਕੋ;
                ਹਾਲਤ 1:
                    ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].a.action =
                        darray_ਵਸਤੂ(actions, 0);
                    ਰੋਕੋ;
                ਮੂਲ:
                    ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].a.actions =
                        memdup(darray_ਚੀਜ਼ਾਂ(actions),
                               ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions,
                               ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(actions)));
                    ਜੇ (!ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ਵਾਪਸ false;
                    }
            }

            ਜੇ (!darray_ਖਾਲੀ_ਥਾਵਾਂ(actions))
                ਕੁੰਜੀ->groups[ਸਮੂਹ].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].implicit_actions)
            ਕੁੰਜੀ->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    ਜੇ (!(ਕੁੰਜੀ->explicit & EXPLICIT_VMODMAP))
        ਕੁੰਜੀ->vmodmap = vmodmap;

    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_mod_action(ਸੰਘ xkb_action *action)
{
    ਵਾਪਸ action->type == ACTION_TYPE_MOD_ਸੈੱਟ ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_ਤਾਲਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ਸਮੂਹ_action(ਸੰਘ xkb_action *action)
{
    ਵਾਪਸ action->type == ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ ||
           action->type == ACTION_TYPE_ਸਮੂਹ_LATCH ||
           action->type == ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ਸਥਿਰ ਖਾਲੀ
Check_ਗੁਣਜ_Actions_Categories(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t g = 0; g < ਕੁੰਜੀ->ਅੰਕ_groups; g++) {
        ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t l = 0; l < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, g); l++) {
            ਬਣਤਰ xkb_level *level = &ਕੁੰਜੀ->groups[g].ਪੱਧਰ[l];
            ਜੇ (level->ਅੰਕ_actions <= 1)
                ਜਾਰੀ;
            ਲਈ (xkb_action_ਗਿਣਤੀ_t i = 0; i < level->ਅੰਕ_actions; i++) {
                ਸੰਘ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool ਸਮੂਹ_action = is_ਸਮੂਹ_action(action1);
                ਜੇ (!(mod_action || ਸਮੂਹ_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ਕੁੰਜੀ))
                    ਜਾਰੀ;
                ਲਈ (xkb_action_ਗਿਣਤੀ_t j = i + 1; j < level->ਅੰਕ_actions; j++) {
                    ਸੰਘ xkb_action *action2 = &level->a.actions[j];
                    ਜੇ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (ਸਮੂਹ_action && is_ਸਮੂਹ_action(action2)))
                    {
                        ਅਚਲ ਅੱਖਰ * ਅਚਲ type = (mod_action)
                            ? "modifiers"
                            : (ਸਮੂਹ_action
                                ? "group"
                                : Action_Type_ਪਾਠ(action1->type));
                        log_err(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਕੁੰਜੀ_aliases(ਬਣਤਰ xkb_keymap *keymap, darray_ਮਾਪ_t min, darray_ਮਾਪ_t max,
                ਬਣਤਰ xkb_ਕੁੰਜੀ_alias *aliases)
{
    ਲਈ (darray_ਮਾਪ_t alias = min; alias <= max; alias++) {
        ਅਚਲ Keycode_ਮੇਲ ਇੰਦਰਾਜ = keymap->ਕੁੰਜੀ_ਨਾਂ[alias];
        ਜੇ (ਇੰਦਰਾਜ.is_alias && ਇੰਦਰਾਜ.ਮਿਲਿਆ) {
            *aliases = (ਬਣਤਰ xkb_ਕੁੰਜੀ_alias) {
                .alias = alias,
                .real = ਇੰਦਰਾਜ.alias.real
            };
            aliases++;
        }
    }
}

ਸਥਿਰ bool
update_pending_ਕੁੰਜੀ_fields(ਬਣਤਰ xkb_keymap_info *info, ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    ਜੇ (ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ) {
        ਬਣਤਰ pending_computation * ਅਚਲ pc = &darray_ਵਸਤੂ(
            *info->pending_computations, ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ
        );
        ਜੇ (!pc->computed) {
            xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0;
            ਚੋਣ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info, pc->ਵਿਅੰਜਕ, true, &ਸਮੂਹ, ਨਿਰਾ)) {
            ਹਾਲਤ PARSER_SUCCESS:
                pc->computed = true;
                pc->ਮੁੱਲ = ਸਮੂਹ - 1;
                ਰੋਕੋ;
            ਹਾਲਤ PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                        "Invalid key redirect group index\n");
                ਵਾਪਸ (info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH);
            ਮੂਲ:
                /* Recover from error: ignore */;
            }
        }
        ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ = false;
        ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = pc->ਮੁੱਲ;
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
update_pending_action_fields(ਬਣਤਰ xkb_keymap_info *info,
                             xkb_keycode_t keycode, ਸੰਘ xkb_action *act)
{
    ਚੋਣ (act->type) {
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_LATCH:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ:
        ਜੇ (act->ਸਮੂਹ.flags & ACTION_PENDING_COMPUTATION) {
            ਬਣਤਰ pending_computation * ਅਚਲ pc =
                &darray_ਵਸਤੂ(*info->pending_computations, act->ਸਮੂਹ.ਸਮੂਹ);
            ਜੇ (!pc->computed) {
                xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0;
                ਅਚਲ bool absolute =
                    (act->ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH);
                ਚੋਣ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info, pc->ਵਿਅੰਜਕ, absolute, &ਸਮੂਹ, ਨਿਰਾ)) {
                ਹਾਲਤ PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                            "Invalid action group index\n");
                    ਵਾਪਸ false;
                ਹਾਲਤ PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    ਰੋਕੋ;
                ਮੂਲ:
                    pc->computed = true;
                    ਜੇ (absolute) {
                        pc->ਮੁੱਲ = ਸਮੂਹ - 1;
                    } ਨਹੀਂਤਾਂ {
                        pc->ਮੁੱਲ = ਸਮੂਹ;
                        ਜੇ (pc->ਵਿਅੰਜਕ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE)
                            pc->ਮੁੱਲ = (uint32_t) (-(int32_t) pc->ਮੁੱਲ);
                    }
                }
            }
            act->ਸਮੂਹ.ਸਮੂਹ = (int32_t) pc->ਮੁੱਲ;
            act->ਸਮੂਹ.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ਵਾਪਸ true;
    ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ: {
        ਜੇ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ਕੁੰਜੀ_auto) {
            /* No auto value to set */
            ਵਾਪਸ true;
        } ਨਹੀਂਤਾਂ {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ਵਾਪਸ true;
    }
    ਮੂਲ:
        ਵਾਪਸ true;
    }
}

ਸਥਿਰ bool
update_pending_led_fields(ਬਣਤਰ xkb_keymap_info *info, ਬਣਤਰ xkb_led *led)
{
    ਜੇ (led->pending_groups) {
        ਬਣਤਰ pending_computation * ਅਚਲ pc =
            &darray_ਵਸਤੂ(*info->pending_computations, led->groups);
        ਜੇ (!pc->computed) {
            xkb_layout_ਨਕਾਬ_t ਨਕਾਬ = 0;
            ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ_ਨਕਾਬ(info, pc->ਵਿਅੰਜਕ, &ਨਕਾਬ, ਨਿਰਾ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                        "Invalid LED group mask\n");
                ਵਾਪਸ false;
            }
            pc->computed = true;
            pc->ਮੁੱਲ = ਨਕਾਬ;
        }
        led->pending_groups = false;
        led->groups = pc->ਮੁੱਲ;
    }
    ਵਾਪਸ true;
}

/** Indices for the array "groupIndexNames" */
ਗਿਣਤੀ {
    ਸਮੂਹ_ਸੂਚਕਾਂਕ_NAME_ਆਖਰੀ = 1,
};

/** Indices for the array "groupMaskNames" */
ਗਿਣਤੀ {
    ਸਮੂਹ_ਨਕਾਬ_NAME_ਆਖਰੀ = 3,
};

ਸਥਿਰ ਖਾਲੀ
update_auto_ਸਮੂਹ_ਤਾਲਾ_on_modifier_release(ਬਣਤਰ xkb_level * ਸੀਮਿਤ level0,
                                           xkb_level_ਸੂਚਕਾਂਕ_t l,
                                           ਸੰਘ xkb_action * ਸੀਮਿਤ action)
{
    ਜੇ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].ਅੰਕ_actions == 1 &&
        action->type == ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ &&
        /* lockOnRelease not set explicitly */
        !(action->ਸਮੂਹ.flags & ACTION_EXPLICIT_ਤਾਲਾ_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->ਅੰਕ_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_ਸੈੱਟ))
    {
        ਵਾਪਸ;
    }

    action->ਸਮੂਹ.flags |= ACTION_AUTO_ਸਮੂਹ_ਤਾਲਾ_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ਸਥਿਰ bool
UpdateDerivedKeymapFields(ਬਣਤਰ xkb_keymap_info *info)
{
    ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ ਅਚਲ keymap = &info->keymap;

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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_ਕੁੰਜੀ_alias) == ਆਕਾਰ(Keycode_ਮੇਲ), "");
#ਨਹੀਂਤਾਂ
    static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_ਕੁੰਜੀ_alias) == 2 * ਆਕਾਰ(Keycode_ਮੇਲ), "");
#ਜੇ_ਅੰਤ
    darray_ਮਾਪ_t ਅੰਕ_ਕੁੰਜੀ_aliases = 0;
    darray_ਮਾਪ_t min_alias = 0;
    darray_ਮਾਪ_t max_alias = 0;
    ਲਈ (xkb_atom_t alias = 0; alias < keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ; alias++) {
        ਅਚਲ Keycode_ਮੇਲ ਇੰਦਰਾਜ = keymap->ਕੁੰਜੀ_ਨਾਂ[alias];
        ਜੇ (ਇੰਦਰਾਜ.is_alias && ਇੰਦਰਾਜ.ਮਿਲਿਆ) {
            ਜੇ (!ਅੰਕ_ਕੁੰਜੀ_aliases)
                min_alias = alias;
            max_alias = alias;
            ਅੰਕ_ਕੁੰਜੀ_aliases++;
        }
    }
    ਜੇ (ਅੰਕ_ਕੁੰਜੀ_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ਅਚਲ darray_ਮਾਪ_t required_space = ਆਕਾਰ(ਬਣਤਰ xkb_ਕੁੰਜੀ_alias)
                                           / ਆਕਾਰ(Keycode_ਮੇਲ)
                                           * ਅੰਕ_ਕੁੰਜੀ_aliases;
        ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕ_ਕੁੰਜੀ_aliases <= keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ);
        ਜੇ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            ਜੋੜੋ_ਕੁੰਜੀ_aliases(keymap, min_alias, max_alias, keymap->ਕੁੰਜੀ_aliases);
            /* Shrink */
            ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ r =
                realloc(keymap->ਕੁੰਜੀ_aliases,
                        ਅੰਕ_ਕੁੰਜੀ_aliases * ਆਕਾਰ(*keymap->ਕੁੰਜੀ_aliases));
            ਜੇ (!r)
                ਵਾਪਸ false;
            keymap->ਕੁੰਜੀ_aliases = r;
        } ਨਹੀਂਤਾਂ ਜੇ (keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ aliases = (ਬਣਤਰ xkb_ਕੁੰਜੀ_alias *) (
                keymap->ਕੁੰਜੀ_ਨਾਂ + max_alias + 1 +
                !is_aligned(keymap->ਕੁੰਜੀ_ਨਾਂ + max_alias + 1, ਆਕਾਰ(*aliases))
            );
            ਜੋੜੋ_ਕੁੰਜੀ_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(keymap->ਕੁੰਜੀ_aliases, aliases,
                   ਅੰਕ_ਕੁੰਜੀ_aliases * ਆਕਾਰ(*keymap->ਕੁੰਜੀ_aliases));
            /* Shrink */
            ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ r =
                realloc(keymap->ਕੁੰਜੀ_aliases,
                        ਅੰਕ_ਕੁੰਜੀ_aliases * ਆਕਾਰ(*keymap->ਕੁੰਜੀ_aliases));
            ਜੇ (!r)
                ਵਾਪਸ false;
            keymap->ਕੁੰਜੀ_aliases = r;
        } ਨਹੀਂਤਾਂ {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ aliases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ_ਕੁੰਜੀ_aliases,
                                                          ਆਕਾਰ(*aliases));
            ਜੇ (!aliases)
                ਵਾਪਸ false;
            ਜੋੜੋ_ਕੁੰਜੀ_aliases(keymap, min_alias, max_alias, aliases);
            ਮੁਕਤ(keymap->ਕੁੰਜੀ_ਨਾਂ);
            keymap->ਕੁੰਜੀ_aliases = aliases;
        }
    }
    keymap->ਅੰਕ_ਕੁੰਜੀ_aliases = ਅੰਕ_ਕੁੰਜੀ_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap)
        keymap->ਅੰਕ_groups = MAX(keymap->ਅੰਕ_groups, ਕੁੰਜੀ->ਅੰਕ_groups);

    ਅਚਲ bool pending_computations = !darray_ਖਾਲੀ_ਥਾਵਾਂ(*info->pending_computations);

    ਜੇ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups
            = (keymap->ਅੰਕ_groups)
            ? keymap->ਅੰਕ_groups
            : 1; /* default to the “first” layout */
        info->lookup.ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ[ਸਮੂਹ_ਸੂਚਕਾਂਕ_NAME_ਆਖਰੀ] =
            (Lookup_ਇੰਦਰਾਜ) {
                .name = ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME,
                .ਮੁੱਲ = ਅੰਕ_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ[ਸਮੂਹ_ਨਕਾਬ_NAME_ਆਖਰੀ] =
            (Lookup_ਇੰਦਰਾਜ) {
                .name = ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME,
                .ਮੁੱਲ = UINT32_C(1) << (ਅੰਕ_groups - 1)
            };

        /* Update interpret fields with pending computations */
        ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_sym_interprets; i++) {
            ਬਣਤਰ xkb_sym_interpret * ਸੀਮਿਤ ਅਚਲ interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                ਜੇ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ਵਾਪਸ false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        ਜੇ (!Apply_Interps_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(keymap, ਕੁੰਜੀ))
            ਵਾਪਸ false;
        Check_ਗੁਣਜ_Actions_Categories(keymap, ਕੁੰਜੀ);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_ਸੂਚਕਾਂਕ_t idx;
    ਬਣਤਰ xkb_mod *mod;
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            ਜੇ (ਕੁੰਜੀ->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ਕੁੰਜੀ->modmap;

    ਜੇ (keymap->format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2) {
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
            ਅਚਲ xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = UINT32_C(1) << idx;
            ਜੇ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & ਨਕਾਬ)) {
                mod->mapping = ਨਕਾਬ;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= ਨਕਾਬ;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    ਪ੍ਰਤਿਜ੍ਞਾ(keymap->canonical_state_ਨਕਾਬ == MOD_REAL_ਨਕਾਬ_ALL);
    xkb_mod_ਨਕਾਬ_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_ਨਕਾਬ |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_types; i++) {
        Compute_Effective_ਨਕਾਬ(keymap, &keymap->types[i].mods);

        ਲਈ (darray_ਮਾਪ_t j = 0; j < keymap->types[i].ਅੰਕ_entries; j++) {
            ਜੇ (has_unbound_vmods(keymap,
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
                keymap->types[i].entries[j].mods.ਨਕਾਬ = 0;
                ਜਾਰੀ;
            }
            Compute_Effective_ਨਕਾਬ(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_ਨਕਾਬ(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        ਜੇ (!update_pending_ਕੁੰਜੀ_fields(info, ਕੁੰਜੀ))
            ਵਾਪਸ false;
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t i = 0; i < ਕੁੰਜੀ->ਅੰਕ_groups; i++) {
            ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t j = 0; j < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, i); j++) {
                ਬਣਤਰ xkb_level * ਅਚਲ level = &ਕੁੰਜੀ->groups[i].ਪੱਧਰ[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ਕੁੰਜੀ->modmap);
                    ਜੇ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ਕੁੰਜੀ) &&
                        !update_pending_action_fields(info, ਕੁੰਜੀ->keycode, act))
                    {
                        ਵਾਪਸ false;
                    }
                    update_auto_ਸਮੂਹ_ਤਾਲਾ_on_modifier_release(
                        ਕੁੰਜੀ->groups[i].ਪੱਧਰ, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ਬਣਤਰ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_ਨਕਾਬ(keymap, &led->mods); /* vmod -> led maps */
        ਜੇ (pending_computations &&
            !update_pending_led_fields(info, led))
            ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ bool (*compile_file_fn)(XkbFile *file, ਬਣਤਰ xkb_keymap_info *info);

ਸਥਿਰ ਅਚਲ compile_file_fn compile_file_fns[ਆਖਰੀ_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ਕੁੰਜੀ_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ਸਥਿਰ ਖਾਲੀ
pending_computations_array_free(pending_computation_array *p)
{
    ਬਣਤਰ pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->ਵਿਅੰਜਕ);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ਬਣਤਰ xkb_keymap *keymap)
{
    XkbFile *files[ਆਖਰੀ_KEYMAP_FILE_TYPE + 1] = { ਨਿਰਾ };
    ਗਿਣਤੀ xkb_file_type type;
    ਬਣਤਰ xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    ਲਈ (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.ਅਗਲਾ) {
        ਜੇ (file->file_type < ਪਹਿਲਾ_KEYMAP_FILE_TYPE ||
            file->file_type > ਆਖਰੀ_KEYMAP_FILE_TYPE) {
            ਜੇ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } ਨਹੀਂਤਾਂ {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_ਮੰਜ਼ਿਲ_string(file->file_type));
            }
            ਜਾਰੀ;
        }

        ਜੇ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_ਮੰਜ਼ਿਲ_string(file->file_type));
            ਜਾਰੀ;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ਬਣਤਰ xkb_keymap_info info = {
        /* Copy the keymap */
        .keymap = *keymap,
        .strict = parser_strict_flags_from_keymap(keymap),
        .features = {
            .max_groups = format_max_groups(keymap->format),
            .max_overlays = format_max_overlays(keymap->format),
            .controls_name_ਵਿੱਥ = format_control_ਨਾਂ_ਵਿੱਥ(keymap->format),
            .ਸਮੂਹ_ਤਾਲਾ_on_release =
                is_ਸਮੂਹ_ਤਾਲਾ_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_ਤਾਲਾ_On_Press_Supported(keymap->format),
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
            .ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [ਸਮੂਹ_ਸੂਚਕਾਂਕ_NAME_ਆਖਰੀ] = {
                    keymap->ਅੰਕ_groups ? ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME : ਨਿਰਾ,
                    keymap->ਅੰਕ_groups /* 1-indexed */
                },
                { ਨਿਰਾ, 0 }
            },
            .ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ = {
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
                [ਸਮੂਹ_ਨਕਾਬ_NAME_ਆਖਰੀ] = {
                    keymap->ਅੰਕ_groups ? ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME : ਨਿਰਾ,
                    /* Be extra cautious */
                    keymap->ਅੰਕ_groups && keymap->ਅੰਕ_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->ਅੰਕ_groups - 1))
                        : 0
                },
                { ਨਿਰਾ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    ਲਈ (type = ਪਹਿਲਾ_KEYMAP_FILE_TYPE;
         type <= ਆਖਰੀ_KEYMAP_FILE_TYPE;
         type++) {
        ਜੇ (files[type] == ਨਿਰਾ) {
            log_dbg(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_ਮੰਜ਼ਿਲ_string(type));
        } ਨਹੀਂਤਾਂ {
            log_dbg(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_ਮੰਜ਼ਿਲ_string(type),
                    ਸੁਰੱਖਿਅਤ_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ਅਚਲ bool ok = compile_file_fns[type](files[type], &info);
        ਜੇ (!ok) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_ਮੰਜ਼ਿਲ_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ਵਾਪਸ false;
        }
    }

    ਅਚਲ bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ਵਾਪਸ ok;
}
