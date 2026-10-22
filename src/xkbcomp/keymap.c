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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "ast-build.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

ಸ್ಥಿರ bool
has_unbound_vmods(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ)
{
    xkb_mod_ಸೂಚ್ಯಂಕ_t k;
    ರಚನೆ xkb_mod *mod;
    xkb_vmods_enumerate(k, mod, &keymap->mods)
        ಆದರೆ ((ಮುಖವಾಡ & (UINT32_C(1) << k)) && mod->mapping == 0)
            ಹಿಂತಿರುಗಿ true;
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
Compute_Effective_ಮುಖವಾಡ(ರಚನೆ xkb_keymap *keymap, ರಚನೆ xkb_mods *mods)
{
    /*
     * Since we accept numeric values for the vmod mask in the keymap, we may
     * have extra bits set that encode no real/virtual modifier. Keep them
     * unchanged for consistency.
     */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t unknown_mods = ~(xkb_mod_ಮುಖವಾಡ_t)
        ((UINT64_C(1) << keymap->mods.ಅಂಕೆ_mods) - UINT64_C(1));
    mods->ಮುಖವಾಡ = mod_ಮುಖವಾಡ_get_effective(keymap, mods->mods)
               | (mods->mods & unknown_mods);
}

ಸ್ಥಿರ ಶೂನ್ಯ
UpdateActionMods(ರಚನೆ xkb_keymap *keymap, ಒಕ್ಕೂಟ xkb_action *act,
                 xkb_mod_ಮುಖವಾಡ_t modmap)
{
    ಆಯ್ಕೆ (act->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಬೀಗ:
        ಆದರೆ (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        Compute_Effective_ಮುಖವಾಡ(keymap, &act->mods.mods);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret default_interpret = {
    .sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ /* unused */,
    .ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_ANY_OR_NONE,
    .mods = 0,
    .repeat = DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT,
    .virtual_mod = (xkb_mod_ಸೂಚ್ಯಂಕ_t) DEFAULT_INTERPRET_VMOD,
    .ಅಂಕೆ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret*) xkb_sym_interprets;

/**
 * Find an interpretation which applies to this particular level, either by
 * finding an exact match for the symbol and modifier combination, or a
 * generic XKB_KEY_NoSymbol match.
 */
ಸ್ಥಿರ bool
ಕಂಡುಹಿಡಿ_Interp_For_ಕೀಲಿ(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು, xkb_level_ಸೂಚ್ಯಂಕ_t level,
                 xkb_sym_interprets *interprets)
{
    ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms;
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_syms;

    ಅಂಕೆ_syms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(keymap, ಕೀಲಿ->keycode, ಗುಂಪು,
                                                level, &syms);
    ಆದರೆ (ಅಂಕೆ_syms <= 0)
        ಹಿಂತಿರುಗಿ false;

    /*
     * There may be multiple matchings interprets; we should always return
     * the most specific. Here we rely on compat.c to set up the
     * sym_interprets array from the most specific to the least specific,
     * such that when we find a match we return immediately.
     */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < ಅಂಕೆ_syms; s++) {
        bool ಸಿಕ್ಕಿತು = false;
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_sym_interprets; i++) {
            ರಚನೆ xkb_sym_interpret * ಸ್ಥಿರಾಂಕ interp = &keymap->sym_interprets[i];
            xkb_mod_ಮುಖವಾಡ_t mods;

            ಸಿಕ್ಕಿತು = false;

            ಆದರೆ (interp->sym != syms[s] && interp->sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                ಮುಂದುವರಿ;

            ಆದರೆ (interp->level_one_only && level != 0)
                mods = 0;
            ಇಲ್ಲದಿದ್ದರೆ
                mods = ಕೀಲಿ->modmap;

            ಆಯ್ಕೆ (interp->ಹೊಂದಿಕೆ) {
            ಸ್ಥಿತಿ ಹೊಂದಿಕೆ_NONE:
                ಸಿಕ್ಕಿತು = !(interp->mods & mods);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ಹೊಂದಿಕೆ_ANY_OR_NONE:
                ಸಿಕ್ಕಿತು = (!mods || (interp->mods & mods));
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ಹೊಂದಿಕೆ_ANY:
                ಸಿಕ್ಕಿತು = (interp->mods & mods);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ಹೊಂದಿಕೆ_ALL:
                ಸಿಕ್ಕಿತು = ((interp->mods & mods) == interp->mods);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ಹೊಂದಿಕೆ_EXACTLY:
                ಸಿಕ್ಕಿತು = (interp->mods == mods);
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ಸಿಕ್ಕಿತು && i > 0 && interp->sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                /*
                 * For an interpretation matching Any keysym, we may get the
                 * same interpretation for multiple keysyms. This may result in
                 * unwanted duplicate actions. So set this interpretation only
                 * if no previous keysym was matched with this interpret at this
                 * level, else set the default interpretation.
                 */
                ರಚನೆ xkb_sym_interpret ಸ್ಥಿರಾಂಕ **ಹಿಂದಿನ_interp;
                darray_foreach(ಹಿಂದಿನ_interp, *interprets) {
                    ಆದರೆ (*ಹಿಂದಿನ_interp == interp) {
                        /* Keep as a safeguard against refactoring
                         * NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores) */
                        ಸಿಕ್ಕಿತು = false;
                        log_warn(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                 "Repeated interpretation ignored for keysym "
                                 "#%d \"%s\" at level %"PRIu32"/group %"PRIu32" "
                                 "on key %s.\n",
                                 s + 1, Keysym_ಪಠ್ಯ(keymap->ctx, syms[s]),
                                 level + 1, ಗುಂಪು + 1,
                                 ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name));
                        ಹೋಗು not_ಸಿಕ್ಕಿತು;
                    }
                }
            }
            ಆದರೆ (ಸಿಕ್ಕಿತು) {
                darray_append(*interprets, interp);
                interp->required = true;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (!ಸಿಕ್ಕಿತು)
not_ಸಿಕ್ಕಿತು:
            darray_append(*interprets, &default_interpret);
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Apply_Interps_ಗಮ್ಯ_ಕೀಲಿ(ರಚನೆ xkb_keymap *keymap, ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    xkb_mod_ಮುಖವಾಡ_t vmodmap = 0;
    xkb_level_ಸೂಚ್ಯಂಕ_t level;
    // FIXME: do not use darray, add actions directly in FindInterpForKey
    xkb_sym_interprets interprets = darray_new();
    darray(ಒಕ್ಕೂಟ xkb_action) actions = darray_new();

    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
        /* Skip any interpretation for this group if it has explicit actions */
        ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].explicit_actions)
            ಮುಂದುವರಿ;

        ಗಾಗಿ (level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, ಗುಂಪು); level++) {
            ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions == 0);

            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret **interp_iter;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret *interp;
            ಗಾತ್ರ_t k;
            darray_resize(interprets, 0);

            ಸ್ಥಿರಾಂಕ bool ಸಿಕ್ಕಿತು = ಕಂಡುಹಿಡಿ_Interp_For_ಕೀಲಿ(keymap, ಕೀಲಿ, ಗುಂಪು, level, &interprets);
            ಆದರೆ (!ಸಿಕ್ಕಿತು)
                ಮುಂದುವರಿ;

            darray_enumerate(k, interp_iter, interprets) {
                interp = *interp_iter;
                /* Infer default key behaviours from the base level. */
                ಆದರೆ (ಗುಂಪು == 0 && level == 0 &&
                    !(ಕೀಲಿ->explicit & EXPLICIT_REPEAT) && interp->repeat)
                {
                    ಕೀಲಿ->repeats = true;
                }

                ಆದರೆ ((ಗುಂಪು == 0 && level == 0) || !interp->level_one_only) {
                    static_assert((uint32_t)DEFAULT_INTERPRET_VMOD ==
                                  (uint32_t)XKB_MOD_INVALID, "");
                    static_assert(!DEFAULT_INTERPRET_VMODMAP, "");
                    ಆದರೆ (interp->virtual_mod != XKB_MOD_INVALID)
                        vmodmap |= (UINT32_C(1) << interp->virtual_mod);
                }

                ಆಯ್ಕೆ (interp->ಅಂಕೆ_actions) {
                ಸ್ಥಿತಿ 0:
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 1:
                    darray_append(actions, interp->a.action);
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    darray_append_ವಸ್ತುಗಳು(actions, interp->a.actions,
                                        interp->ಅಂಕೆ_actions);
                }
            }

            /* Copy the actions */
            ಆದರೆ (unlikely(darray_ಗಾತ್ರ(actions)) > MAX_ACTIONS_ಪ್ರತಿ_LEVEL) {
                log_warn(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "Could not append interpret actions to key %s: "
                         "maximum is %u, got: %u. Dropping excessive actions\n",
                         ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name),
                         MAX_ACTIONS_ಪ್ರತಿ_LEVEL, darray_ಗಾತ್ರ(actions));
                ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions =
                    MAX_ACTIONS_ಪ್ರತಿ_LEVEL;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions =
                    (xkb_action_ಎಣಿಕೆ_t) darray_ಗಾತ್ರ(actions);
            }
            ಆಯ್ಕೆ (darray_ಗಾತ್ರ(actions)) {
                ಸ್ಥಿತಿ 0:
                    ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].a.action =
                        (ಒಕ್ಕೂಟ xkb_action) { .type = ACTION_TYPE_NONE };
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 1:
                    ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].a.action =
                        darray_ಅಂಶ(actions, 0);
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].a.actions =
                        memdup(darray_ವಸ್ತುಗಳು(actions),
                               ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions,
                               ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(actions)));
                    ಆದರೆ (!ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].a.actions) {
                        log_err(keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                                "Could not allocate interpret actions\n");
                        darray_free(actions);
                        darray_free(interprets);
                        ಹಿಂತಿರುಗಿ false;
                    }
            }

            ಆದರೆ (!darray_ಖಾಲಿಗಳು(actions))
                ಕೀಲಿ->groups[ಗುಂಪು].implicit_actions = true;

            /* Do not free here */
            darray_resize(actions, 0);
        }

        ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].implicit_actions)
            ಕೀಲಿ->implicit_actions = true;
    }
    darray_free(actions);
    darray_free(interprets);

    ಆದರೆ (!(ಕೀಲಿ->explicit & EXPLICIT_VMODMAP))
        ಕೀಲಿ->vmodmap = vmodmap;

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_mod_action(ಒಕ್ಕೂಟ xkb_action *action)
{
    ಹಿಂತಿರುಗಿ action->type == ACTION_TYPE_MOD_ಗಣ ||
           action->type == ACTION_TYPE_MOD_LATCH ||
           action->type == ACTION_TYPE_MOD_ಬೀಗ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ಗುಂಪು_action(ಒಕ್ಕೂಟ xkb_action *action)
{
    ಹಿಂತಿರುಗಿ action->type == ACTION_TYPE_ಗುಂಪು_ಗಣ ||
           action->type == ACTION_TYPE_ಗುಂಪು_LATCH ||
           action->type == ACTION_TYPE_ಗುಂಪು_ಬೀಗ;
}

/* Check for mixing actions of the same category.
 * We do not support that yet, because it needs a careful refactor of the state
 * handling. See: `xkb_filter_apply_all`. */
ಸ್ಥಿರ ಶೂನ್ಯ
Check_ಅಪವರ್ತ್ಯ_Actions_Categories(ರಚನೆ xkb_keymap *keymap, ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t g = 0; g < ಕೀಲಿ->ಅಂಕೆ_groups; g++) {
        ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t l = 0; l < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, g); l++) {
            ರಚನೆ xkb_level *level = &ಕೀಲಿ->groups[g].ಮಟ್ಟಗಳು[l];
            ಆದರೆ (level->ಅಂಕೆ_actions <= 1)
                ಮುಂದುವರಿ;
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t i = 0; i < level->ಅಂಕೆ_actions; i++) {
                ಒಕ್ಕೂಟ xkb_action *action1 = &level->a.actions[i];
                bool mod_action = is_mod_action(action1);
                bool ಗುಂಪು_action = is_ಗುಂಪು_action(action1);
                ಆದರೆ (!(mod_action || ಗುಂಪು_action ||
                      action1->type == ACTION_TYPE_REDIRECT_ಕೀಲಿ))
                    ಮುಂದುವರಿ;
                ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t j = i + 1; j < level->ಅಂಕೆ_actions; j++) {
                    ಒಕ್ಕೂಟ xkb_action *action2 = &level->a.actions[j];
                    ಆದರೆ (action1->type == action2->type ||
                        (mod_action && is_mod_action(action2)) ||
                        (ಗುಂಪು_action && is_ಗುಂಪು_action(action2)))
                    {
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ type = (mod_action)
                            ? "modifiers"
                            : (ಗುಂಪು_action
                                ? "group"
                                : Action_Type_ಪಠ್ಯ(action1->type));
                        log_err(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "Cannot use multiple %s actions "
                                "in the same level. Action #%u "
                                "for key %s in group %"PRIu32"/level %"PRIu32" "
                                "ignored.\n", type,
                                j + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name),
                                g + 1, l + 1);
                        action2->type = ACTION_TYPE_NONE;
                    }
                }
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ಕೀಲಿ_aliases(ರಚನೆ xkb_keymap *keymap, darray_ಗಾತ್ರ_t min, darray_ಗಾತ್ರ_t max,
                ರಚನೆ xkb_ಕೀಲಿ_alias *aliases)
{
    ಗಾಗಿ (darray_ಗಾತ್ರ_t alias = min; alias <= max; alias++) {
        ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ನಮೂದು = keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[alias];
        ಆದರೆ (ನಮೂದು.is_alias && ನಮೂದು.ಸಿಕ್ಕಿತು) {
            *aliases = (ರಚನೆ xkb_ಕೀಲಿ_alias) {
                .alias = alias,
                .real = ನಮೂದು.alias.real
            };
            aliases++;
        }
    }
}

ಸ್ಥಿರ bool
update_pending_ಕೀಲಿ_fields(ರಚನೆ xkb_keymap_info *info, ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಆದರೆ (ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು) {
        ರಚನೆ pending_computation * ಸ್ಥಿರಾಂಕ pc = &darray_ಅಂಶ(
            *info->pending_computations, ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ
        );
        ಆದರೆ (!pc->computed) {
            xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0;
            ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info, pc->ಅಭಿವ್ಯಕ್ತಿ, true, &ಗುಂಪು, ಖಾಲಿ)) {
            ಸ್ಥಿತಿ PARSER_SUCCESS:
                pc->computed = true;
                pc->ಮೌಲ್ಯ = ಗುಂಪು - 1;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ PARSER_FATAL_ERROR:
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                        "Invalid key redirect group index\n");
                ಹಿಂತಿರುಗಿ (info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH);
            ಪೂರ್ವನಿಯೋಜಿತ:
                /* Recover from error: ignore */;
            }
        }
        ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು = false;
        ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = pc->ಮೌಲ್ಯ;
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
update_pending_action_fields(ರಚನೆ xkb_keymap_info *info,
                             xkb_keycode_t keycode, ಒಕ್ಕೂಟ xkb_action *act)
{
    ಆಯ್ಕೆ (act->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಬೀಗ:
        ಆದರೆ (act->ಗುಂಪು.flags & ACTION_PENDING_COMPUTATION) {
            ರಚನೆ pending_computation * ಸ್ಥಿರಾಂಕ pc =
                &darray_ಅಂಶ(*info->pending_computations, act->ಗುಂಪು.ಗುಂಪು);
            ಆದರೆ (!pc->computed) {
                xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0;
                ಸ್ಥಿರಾಂಕ bool absolute =
                    (act->ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH);
                ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info, pc->ಅಭಿವ್ಯಕ್ತಿ, absolute, &ಗುಂಪು, ಖಾಲಿ)) {
                ಸ್ಥಿತಿ PARSER_FATAL_ERROR:
                    log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                            "Invalid action group index\n");
                    ಹಿಂತಿರುಗಿ false;
                ಸ್ಥಿತಿ PARSER_RECOVERABLE_ERROR:
                    /* ignore */
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    pc->computed = true;
                    ಆದರೆ (absolute) {
                        pc->ಮೌಲ್ಯ = ಗುಂಪು - 1;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        pc->ಮೌಲ್ಯ = ಗುಂಪು;
                        ಆದರೆ (pc->ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE)
                            pc->ಮೌಲ್ಯ = (uint32_t) (-(int32_t) pc->ಮೌಲ್ಯ);
                    }
                }
            }
            act->ಗುಂಪು.ಗುಂಪು = (int32_t) pc->ಮೌಲ್ಯ;
            act->ಗುಂಪು.flags &= ~ACTION_PENDING_COMPUTATION;
        }
        ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ: {
        ಆದರೆ (keycode == XKB_KEYCODE_INVALID ||
            act->redirect.keycode != info->keymap.redirect_ಕೀಲಿ_auto) {
            /* No auto value to set */
            ಹಿಂತಿರುಗಿ true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * Auto value: use the provided keycode to redirect to the key
             * where the action is defined
             */
            act->redirect.keycode = keycode;
        }
        ಹಿಂತಿರುಗಿ true;
    }
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ true;
    }
}

ಸ್ಥಿರ bool
update_pending_led_fields(ರಚನೆ xkb_keymap_info *info, ರಚನೆ xkb_led *led)
{
    ಆದರೆ (led->pending_groups) {
        ರಚನೆ pending_computation * ಸ್ಥಿರಾಂಕ pc =
            &darray_ಅಂಶ(*info->pending_computations, led->groups);
        ಆದರೆ (!pc->computed) {
            xkb_layout_ಮುಖವಾಡ_t ಮುಖವಾಡ = 0;
            ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು_ಮುಖವಾಡ(info, pc->ಅಭಿವ್ಯಕ್ತಿ, &ಮುಖವಾಡ, ಖಾಲಿ)) {
                log_err(info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                        "Invalid LED group mask\n");
                ಹಿಂತಿರುಗಿ false;
            }
            pc->computed = true;
            pc->ಮೌಲ್ಯ = ಮುಖವಾಡ;
        }
        led->pending_groups = false;
        led->groups = pc->ಮೌಲ್ಯ;
    }
    ಹಿಂತಿರುಗಿ true;
}

/** Indices for the array "groupIndexNames" */
ಎಣಿಕೆ {
    ಗುಂಪು_ಸೂಚ್ಯಂಕ_NAME_ಕೊನೆಯ = 1,
};

/** Indices for the array "groupMaskNames" */
ಎಣಿಕೆ {
    ಗುಂಪು_ಮುಖವಾಡ_NAME_ಕೊನೆಯ = 3,
};

ಸ್ಥಿರ ಶೂನ್ಯ
update_auto_ಗುಂಪು_ಬೀಗ_on_modifier_release(ರಚನೆ xkb_level * ಸೀಮಿತ level0,
                                           xkb_level_ಸೂಚ್ಯಂಕ_t l,
                                           ಒಕ್ಕೂಟ xkb_action * ಸೀಮಿತ action)
{
    ಆದರೆ (!(/* Must be level index 1+ */
        l > 0 &&
        /* level has a single action: LockGroup() */
        level0[l].ಅಂಕೆ_actions == 1 &&
        action->type == ACTION_TYPE_ಗುಂಪು_ಬೀಗ &&
        /* lockOnRelease not set explicitly */
        !(action->ಗುಂಪು.flags & ACTION_EXPLICIT_ಬೀಗ_ON_RELEASE) &&
        /* level 0 has a single action: SetMods() */
        level0->ಅಂಕೆ_actions == 1 &&
        level0->a.action.type ==  ACTION_TYPE_MOD_ಗಣ))
    {
        ಹಿಂತಿರುಗಿ;
    }

    action->ಗುಂಪು.flags |= ACTION_AUTO_ಗುಂಪು_ಬೀಗ_ON_MODIFIER_RELEASE;
}

/**
 * This collects a bunch of disparate functions which was done in the server
 * at various points that really should've been done within xkbcomp.  Turns out
 * your actions and types are a lot more useful when any of your modifiers
 * other than Shift actually do something ...
 */
ಸ್ಥಿರ bool
UpdateDerivedKeymapFields(ರಚನೆ xkb_keymap_info *info)
{
    ರಚನೆ xkb_keymap * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ keymap = &info->keymap;

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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    /* Bit fields are not packed on Windows as efficiently as other platforms */
    static_assert(ಗಾತ್ರ(ರಚನೆ xkb_ಕೀಲಿ_alias) == ಗಾತ್ರ(Keycode_ಹೊಂದಿಕೆ), "");
#ಇಲ್ಲದಿದ್ದರೆ
    static_assert(ಗಾತ್ರ(ರಚನೆ xkb_ಕೀಲಿ_alias) == 2 * ಗಾತ್ರ(Keycode_ಹೊಂದಿಕೆ), "");
#ಆದರೆ_ಅಂತ್ಯ
    darray_ಗಾತ್ರ_t ಅಂಕೆ_ಕೀಲಿ_aliases = 0;
    darray_ಗಾತ್ರ_t min_alias = 0;
    darray_ಗಾತ್ರ_t max_alias = 0;
    ಗಾಗಿ (xkb_atom_t alias = 0; alias < keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು; alias++) {
        ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ನಮೂದು = keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[alias];
        ಆದರೆ (ನಮೂದು.is_alias && ನಮೂದು.ಸಿಕ್ಕಿತು) {
            ಆದರೆ (!ಅಂಕೆ_ಕೀಲಿ_aliases)
                min_alias = alias;
            max_alias = alias;
            ಅಂಕೆ_ಕೀಲಿ_aliases++;
        }
    }
    ಆದರೆ (ಅಂಕೆ_ಕೀಲಿ_aliases) {
        /*
         * No fancy algorithm used here: either we can trivially write the whole
         * range of `xkb_key_aliases` without overlapping with the `KeycodeMatch`
         * alias entries or we allocate a new array.
         * In practice no new allocation is needed and it performs better than
         * using a buffer to handle overlaps.
         */
        ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t required_space = ಗಾತ್ರ(ರಚನೆ xkb_ಕೀಲಿ_alias)
                                           / ಗಾತ್ರ(Keycode_ಹೊಂದಿಕೆ)
                                           * ಅಂಕೆ_ಕೀಲಿ_aliases;
        ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_ಕೀಲಿ_aliases <= keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು);
        ಆದರೆ (min_alias >= required_space) {
            /* Overwrite before the *first* alias entry */
            ಕೂಡಿಸು_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, keymap->ಕೀಲಿ_aliases);
            /* Shrink */
            ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ r =
                realloc(keymap->ಕೀಲಿ_aliases,
                        ಅಂಕೆ_ಕೀಲಿ_aliases * ಗಾತ್ರ(*keymap->ಕೀಲಿ_aliases));
            ಆದರೆ (!r)
                ಹಿಂತಿರುಗಿ false;
            keymap->ಕೀಲಿ_aliases = r;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು - max_alias - 1 > required_space) {
            /* Overwrite after the *last* alias entry */
            ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ aliases = (ರಚನೆ xkb_ಕೀಲಿ_alias *) (
                keymap->ಕೀಲಿ_ಹೆಸರುಗಳು + max_alias + 1 +
                !is_aligned(keymap->ಕೀಲಿ_ಹೆಸರುಗಳು + max_alias + 1, ಗಾತ್ರ(*aliases))
            );
            ಕೂಡಿಸು_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, aliases);
            /* Move to the start */
            ಸ್ಮರಣೆ_ನಕಲು(keymap->ಕೀಲಿ_aliases, aliases,
                   ಅಂಕೆ_ಕೀಲಿ_aliases * ಗಾತ್ರ(*keymap->ಕೀಲಿ_aliases));
            /* Shrink */
            ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ r =
                realloc(keymap->ಕೀಲಿ_aliases,
                        ಅಂಕೆ_ಕೀಲಿ_aliases * ಗಾತ್ರ(*keymap->ಕೀಲಿ_aliases));
            ಆದರೆ (!r)
                ಹಿಂತಿರುಗಿ false;
            keymap->ಕೀಲಿ_aliases = r;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * No space trivially available: instead of processing the LUT with
             * a buffer to handle overlaps, we simply allocate a dedicated array
             */
            ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ aliases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ_ಕೀಲಿ_aliases,
                                                          ಗಾತ್ರ(*aliases));
            ಆದರೆ (!aliases)
                ಹಿಂತಿರುಗಿ false;
            ಕೂಡಿಸು_ಕೀಲಿ_aliases(keymap, min_alias, max_alias, aliases);
            ಬಿಡುಗಡೆ(keymap->ಕೀಲಿ_ಹೆಸರುಗಳು);
            keymap->ಕೀಲಿ_aliases = aliases;
        }
    }
    keymap->ಅಂಕೆ_ಕೀಲಿ_aliases = ಅಂಕೆ_ಕೀಲಿ_aliases;

    /* Find maximum number of groups out of all keys in the keymap. */
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        keymap->ಅಂಕೆ_groups = MAX(keymap->ಅಂಕೆ_groups, ಕೀಲಿ->ಅಂಕೆ_groups);

    ಸ್ಥಿರಾಂಕ bool pending_computations = !darray_ಖಾಲಿಗಳು(*info->pending_computations);

    ಆದರೆ (pending_computations) {
        /*
         * Update non-static LUTs for pending computations:
         * - enable entry by setting a non-null name
         * - update the corresponding value
         */
        ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups
            = (keymap->ಅಂಕೆ_groups)
            ? keymap->ಅಂಕೆ_groups
            : 1; /* default to the “first” layout */
        info->lookup.ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು[ಗುಂಪು_ಸೂಚ್ಯಂಕ_NAME_ಕೊನೆಯ] =
            (Lookup_ನಮೂದು) {
                .name = ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME,
                .ಮೌಲ್ಯ = ಅಂಕೆ_groups
            };
        static_assert(XKB_MAX_GROUPS == 32, "Guard left shift");
        info->lookup.ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು[ಗುಂಪು_ಮುಖವಾಡ_NAME_ಕೊನೆಯ] =
            (Lookup_ನಮೂದು) {
                .name = ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME,
                .ಮೌಲ್ಯ = UINT32_C(1) << (ಅಂಕೆ_groups - 1)
            };

        /* Update interpret fields with pending computations */
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_sym_interprets; i++) {
            ರಚನೆ xkb_sym_interpret * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ interp =
                &keymap->sym_interprets[i];
            xkb_level_foreach_action(act, interp) {
                ಆದರೆ (!update_pending_action_fields(info, XKB_KEYCODE_INVALID, act))
                    ಹಿಂತಿರುಗಿ false;
            }
        }
    }

    /* Find all the interprets for the key and bind them to actions,
     * which will also update the vmodmap. */
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಆದರೆ (!Apply_Interps_ಗಮ್ಯ_ಕೀಲಿ(keymap, ಕೀಲಿ))
            ಹಿಂತಿರುಗಿ false;
        Check_ಅಪವರ್ತ್ಯ_Actions_Categories(keymap, ಕೀಲಿ);
    }

    /* Update keymap->mods, the virtual -> real mod mapping. */
    xkb_mod_ಸೂಚ್ಯಂಕ_t idx;
    ರಚನೆ xkb_mod *mod;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        xkb_vmods_enumerate(idx, mod, &keymap->mods)
            ಆದರೆ (ಕೀಲಿ->vmodmap & (UINT32_C(1) << idx))
                mod->mapping |= ಕೀಲಿ->modmap;

    ಆದರೆ (keymap->format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2) {
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
            ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = UINT32_C(1) << idx;
            ಆದರೆ (mod->mapping == 0 && !(keymap->mods.explicit_vmods & ಮುಖವಾಡ)) {
                mod->mapping = ಮುಖವಾಡ;
                /*
                 * Make the mapping explicit, so that it is interoperable
                 * without relying on the implementation-specific virtual
                 * modifiers indices.
                 */
                keymap->mods.explicit_vmods |= ಮುಖವಾಡ;
            }
        }
    }

    /* Update the canonical modifiers state mask */
    ಪ್ರತಿಜ್ಞಾ(keymap->canonical_state_ಮುಖವಾಡ == MOD_REAL_ಮುಖವಾಡ_ALL);
    xkb_mod_ಮುಖವಾಡ_t extra_canonical_mods = 0;
    xkb_vmods_enumerate(idx, mod, &keymap->mods) {
        extra_canonical_mods |= mod->mapping;
    }
    keymap->canonical_state_ಮುಖವಾಡ |= extra_canonical_mods;

    /* Now update the level masks for all the types to reflect the vmods. */
    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
        Compute_Effective_ಮುಖವಾಡ(keymap, &keymap->types[i].mods);

        ಗಾಗಿ (darray_ಗಾತ್ರ_t j = 0; j < keymap->types[i].ಅಂಕೆ_entries; j++) {
            ಆದರೆ (has_unbound_vmods(keymap,
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
                keymap->types[i].entries[j].mods.ಮುಖವಾಡ = 0;
                ಮುಂದುವರಿ;
            }
            Compute_Effective_ಮುಖವಾಡ(keymap, &keymap->types[i].entries[j].mods);
            Compute_Effective_ಮುಖವಾಡ(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

    /* Update action modifiers and fields with pending computations. */
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಆದರೆ (!update_pending_ಕೀಲಿ_fields(info, ಕೀಲಿ))
            ಹಿಂತಿರುಗಿ false;
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0; i < ಕೀಲಿ->ಅಂಕೆ_groups; i++) {
            ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t j = 0; j < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, i); j++) {
                ರಚನೆ xkb_level * ಸ್ಥಿರಾಂಕ level = &ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು[j];
                xkb_level_foreach_action(act, level) {
                    UpdateActionMods(keymap, act, ಕೀಲಿ->modmap);
                    ಆದರೆ ((pending_computations ||
                         act->type == ACTION_TYPE_REDIRECT_ಕೀಲಿ) &&
                        !update_pending_action_fields(info, ಕೀಲಿ->keycode, act))
                    {
                        ಹಿಂತಿರುಗಿ false;
                    }
                    update_auto_ಗುಂಪು_ಬೀಗ_on_modifier_release(
                        ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು, j, act
                    );
                }
            }
        }
    }

    /* Update LEDs */
    ರಚನೆ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        Compute_Effective_ಮುಖವಾಡ(keymap, &led->mods); /* vmod -> led maps */
        ಆದರೆ (pending_computations &&
            !update_pending_led_fields(info, led))
            ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ bool (*compile_file_fn)(XkbFile *file, ರಚನೆ xkb_keymap_info *info);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ compile_file_fn compile_file_fns[ಕೊನೆಯ_KEYMAP_FILE_TYPE + 1] = {
    [FILE_TYPE_KEYCODES] = CompileKeycodes,
    [FILE_TYPE_TYPES] = Compile_ಕೀಲಿ_Types,
    [FILE_TYPE_COMPAT] = CompileCompatMap,
    [FILE_TYPE_SYMBOLS] = CompileSymbols,
};

ಸ್ಥಿರ ಶೂನ್ಯ
pending_computations_array_free(pending_computation_array *p)
{
    ರಚನೆ pending_computation *pc;
    darray_foreach(pc, *p)
        FreeStmt((ParseCommon*) pc->ಅಭಿವ್ಯಕ್ತಿ);
    darray_free(*p);
}

bool
CompileKeymap(XkbFile *file, ರಚನೆ xkb_keymap *keymap)
{
    XkbFile *files[ಕೊನೆಯ_KEYMAP_FILE_TYPE + 1] = { ಖಾಲಿ };
    ಎಣಿಕೆ xkb_file_type type;
    ರಚನೆ xkb_context *ctx = keymap->ctx;

    /* Collect section files and check for duplicates. */
    ಗಾಗಿ (file = (XkbFile *) file->defs; file;
         file = (XkbFile *) file->common.ಮುಂದಿನ) {
        ಆದರೆ (file->file_type < ಮೊದಲ_KEYMAP_FILE_TYPE ||
            file->file_type > ಕೊನೆಯ_KEYMAP_FILE_TYPE) {
            ಆದರೆ (file->file_type == FILE_TYPE_GEOMETRY) {
                log_vrb(ctx, XKB_LOG_VERBOSITY_BRIEF,
                        XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION,
                        "Geometry sections are not supported; ignoring\n");
            } ಇಲ್ಲದಿದ್ದರೆ {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Cannot define %s in a keymap file\n",
                        xkb_file_type_ಗಮ್ಯ_string(file->file_type));
            }
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (files[file->file_type]) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "More than one %s section in keymap file; "
                    "All sections after the first ignored\n",
                    xkb_file_type_ಗಮ್ಯ_string(file->file_type));
            ಮುಂದುವರಿ;
        }

        files[file->file_type] = file;
    }

    /*
     * Keymap augmented with compilation-specific data
     */
    pending_computation_array pending_computations = darray_new();
    ರಚನೆ xkb_keymap_info info = {
        /* Copy the keymap */
        .keymap = *keymap,
        .strict = parser_strict_flags_from_keymap(keymap),
        .features = {
            .max_groups = format_max_groups(keymap->format),
            .max_overlays = format_max_overlays(keymap->format),
            .controls_name_ಸ್ಥಾನಾಂತರ = format_control_ಹೆಸರುಗಳು_ಸ್ಥಾನಾಂತರ(keymap->format),
            .ಗುಂಪು_ಬೀಗ_on_release =
                is_ಗುಂಪು_ಬೀಗ_On_Release_Supported(keymap->format),
            .mods_unlock_on_press =
                is_Mods_Un_ಬೀಗ_On_Press_Supported(keymap->format),
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
            .ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು = {
                { "first", 1 },
                /*
                 * The entry “last” is initially active only if using the RMLVO
                 * API, otherwise it will be activated after parsing the symbols
                 * and used for pending computations.
                 *
                 * NOTE: The RMLVO case is valid as long as multiple groups per
                 * key is disallowed in an xkb_symbols section.
                 */
                [ಗುಂಪು_ಸೂಚ್ಯಂಕ_NAME_ಕೊನೆಯ] = {
                    keymap->ಅಂಕೆ_groups ? ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME : ಖಾಲಿ,
                    keymap->ಅಂಕೆ_groups /* 1-indexed */
                },
                { ಖಾಲಿ, 0 }
            },
            .ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು = {
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
                [ಗುಂಪು_ಮುಖವಾಡ_NAME_ಕೊನೆಯ] = {
                    keymap->ಅಂಕೆ_groups ? ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME : ಖಾಲಿ,
                    /* Be extra cautious */
                    keymap->ಅಂಕೆ_groups && keymap->ಅಂಕೆ_groups <= XKB_MAX_GROUPS
                        ? (UINT32_C(1) << (keymap->ಅಂಕೆ_groups - 1))
                        : 0
                },
                { ಖಾಲಿ, 0 }
            },
        },
        .pending_computations = &pending_computations,
    };

    /*
     * Compile sections
     *
     * NOTE: Any component is optional.
     */
    ಗಾಗಿ (type = ಮೊದಲ_KEYMAP_FILE_TYPE;
         type <= ಕೊನೆಯ_KEYMAP_FILE_TYPE;
         type++) {
        ಆದರೆ (files[type] == ಖಾಲಿ) {
            log_dbg(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Component %s not provided in keymap\n",
                    xkb_file_type_ಗಮ್ಯ_string(type));
        } ಇಲ್ಲದಿದ್ದರೆ {
            log_dbg(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Compiling %s \"%s\"\n",
                    xkb_file_type_ಗಮ್ಯ_string(type),
                    ಸುರಕ್ಷಿತ_map_name(files[type]->name));
        }

        /* Missing components are initialized with defaults */
        ಸ್ಥಿರಾಂಕ bool ok = compile_file_fns[type](files[type], &info);
        ಆದರೆ (!ok) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Failed to compile %s\n",
                    xkb_file_type_ಗಮ್ಯ_string(type));
            /* Copy back to the keymap, so that all can be properly freed */
            *keymap = info.keymap;
            pending_computations_array_free(&pending_computations);
            ಹಿಂತಿರುಗಿ false;
        }
    }

    ಸ್ಥಿರಾಂಕ bool ok = UpdateDerivedKeymapFields(&info);
    /* Copy back the keymap */
    *keymap = info.keymap;
    pending_computations_array_free(&pending_computations);
    ಹಿಂತಿರುಗಿ ok;
}
