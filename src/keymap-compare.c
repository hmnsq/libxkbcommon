/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "src/messages-codes.h"
#അന്തര്ഭാവ "src/utils.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


സ്ഥിതം bool
keymap_compare_mods(ഘടന xkb_context *ctx,
                    സ്ഥിരം ഘടന xkb_keymap *keymap1,
                    സ്ഥിരം ഘടന xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    സ്ഥിരം xkb_mod_index_t mod_max = MIN(keymap1->mods.അക്കം_mods,
                                        keymap2->mods.അക്കം_mods);
    വേണ്ടി (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        സ്ഥിരം ഘടന xkb_mod * സ്ഥിരം mod1 = &keymap1->mods.mods[mod];
        സ്ഥിരം ഘടന xkb_mod * സ്ഥിരം mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        എങ്കിൽ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        എങ്കിൽ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        എങ്കിൽ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    എങ്കിൽ (keymap1->mods.അക്കം_mods != keymap2->mods.അക്കം_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.അക്കം_mods, keymap2->mods.അക്കം_mods);
        identical = false;
    }

    തിരികെ identical;
}

സ്ഥിതം bool
keymap_compare_keycodes(ഘടന xkb_context *ctx,
                        സ്ഥിരം ഘടന xkb_keymap *keymap1,
                        സ്ഥിരം ഘടന xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    എങ്കിൽ (keymap1->അക്കം_താക്കോലുകൾ != keymap2->അക്കം_താക്കോലുകൾ) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->അക്കം_താക്കോലുകൾ, keymap2->അക്കം_താക്കോലുകൾ);
        identical = false;
    }
    എങ്കിൽ (keymap1->min_താക്കോൽ_code != keymap2->min_താക്കോൽ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_താക്കോൽ_code, keymap2->min_താക്കോൽ_code);
        identical = false;
    }
    എങ്കിൽ (keymap1->max_താക്കോൽ_code != keymap2->max_താക്കോൽ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->അക്കം_താക്കോലുകൾ_താഴെയുള്ള, keymap2->അക്കം_താക്കോലുകൾ_താഴെയുള്ള);
        identical = false;
    }
    എങ്കിൽ (keymap1->max_താക്കോൽ_code != keymap2->max_താക്കോൽ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_താക്കോൽ_code, keymap2->min_താക്കോൽ_code);
        identical = false;
    }

    /* Check common keys */
    സ്ഥിരം xkb_keycode_t k_max = MIN(keymap1->അക്കം_താക്കോലുകൾ, keymap2->അക്കം_താക്കോലുകൾ);
    വേണ്ടി (xkb_keycode_t k = 0; k < k_max; k++) {
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം key1 = &keymap1->താക്കോലുകൾ[k];
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം key2 = &keymap1->താക്കോലുകൾ[k];
        എങ്കിൽ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            തുടരുക;
        }

        സ്ഥിരം xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx, key1->name);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx, key2->name);
        എങ്കിൽ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    സ്ഥിരം darray_size_t a_max = MIN(keymap1->അക്കം_താക്കോൽ_aliases,
                                    keymap2->അക്കം_താക്കോൽ_aliases);
    വേണ്ടി (darray_size_t a = 0; a < a_max; a++) {
        സ്ഥിരം ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം entry1 = &keymap1->താക്കോൽ_aliases[a];
        സ്ഥിരം ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം entry2 = &keymap2->താക്കോൽ_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        സ്ഥിരം അക്ഷരം * സ്ഥിരം alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        എങ്കിൽ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        സ്ഥിരം അക്ഷരം * സ്ഥിരം real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        എങ്കിൽ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    എങ്കിൽ (keymap1->അക്കം_താക്കോൽ_aliases != keymap2->അക്കം_താക്കോൽ_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->അക്കം_താക്കോൽ_aliases, keymap2->അക്കം_താക്കോൽ_aliases);
        identical = false;
    }

    തിരികെ identical;
}

സ്ഥിതം bool
keymap_compare_leds(ഘടന xkb_context *ctx,
                    സ്ഥിരം ഘടന xkb_keymap *keymap1,
                    സ്ഥിരം ഘടന xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->അക്കം_leds, keymap2->അക്കം_leds);
    വേണ്ടി (xkb_led_index_t led = 0; led < led_max; led++) {
        സ്ഥിരം ഘടന xkb_led * സ്ഥിരം led1 = &keymap1->leds[led];
        സ്ഥിരം ഘടന xkb_led * സ്ഥിരം led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx, led1->name);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx, led2->name);
        എങ്കിൽ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        എങ്കിൽ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        എങ്കിൽ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        എങ്കിൽ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        എങ്കിൽ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        എങ്കിൽ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    എങ്കിൽ (keymap1->അക്കം_leds != keymap2->അക്കം_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->അക്കം_leds, keymap2->അക്കം_leds);
        identical = false;
    }

    തിരികെ identical;
}

സ്ഥിതം bool
compare_types(ഘടന xkb_context *ctx,
              സ്ഥിരം ഘടന xkb_keymap *keymap1, സ്ഥിരം ഘടന xkb_keymap *keymap2,
              സ്ഥിരം ഘടന xkb_താക്കോൽ_type *type1, സ്ഥിരം ഘടന xkb_താക്കോൽ_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx, type1->name);
    സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx, type2->name);
    എങ്കിൽ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    എങ്കിൽ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        തിരികെ false;
    }

    എങ്കിൽ (type1->അക്കം_levels != type2->അക്കം_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->അക്കം_levels, type2->അക്കം_levels);
        /* No point to check further properties */
        തിരികെ false;
    }

    എങ്കിൽ (type1->അക്കം_level_names != type2->അക്കം_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->അക്കം_level_names, type2->അക്കം_level_names);
        identical = false;
    } അല്ലെങ്കിൽ {
        വേണ്ടി (xkb_level_index_t l = 0; l < type1->അക്കം_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            സ്ഥിരം അക്ഷരം * സ്ഥിരം lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            സ്ഥിരം അക്ഷരം * സ്ഥിരം lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            എങ്കിൽ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    എങ്കിൽ (type1->അക്കം_entries != type2->അക്കം_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->അക്കം_entries, type2->അക്കം_entries);
        identical = false;
    } അല്ലെങ്കിൽ {
        വേണ്ടി (darray_size_t e = 0; e < type1->അക്കം_entries; e++) {
            സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry * സ്ഥിരം entry1 = &type1->entries[e];
            സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry * സ്ഥിരം entry2 = &type2->entries[e];
            എങ്കിൽ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            എങ്കിൽ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            എങ്കിൽ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    തിരികെ identical;
}

സ്ഥിതം bool
keymap_compare_types(ഘടന xkb_context *ctx,
                     സ്ഥിരം ഘടന xkb_keymap *keymap1, സ്ഥിരം ഘടന xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    സ്ഥിരം darray_size_t t_max = MIN(keymap1->അക്കം_types, keymap2->അക്കം_types);
    വേണ്ടി (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    എങ്കിൽ (keymap1->അക്കം_types != keymap2->അക്കം_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->അക്കം_types, keymap2->അക്കം_types);
        identical = false;
    }

    തിരികെ identical;
}

സ്ഥിതം bool
compare_groups(ഘടന xkb_context *ctx,
               സ്ഥിരം ഘടന xkb_keymap *keymap1, സ്ഥിരം ഘടന xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               സ്ഥിരം ഘടന xkb_group *group1, സ്ഥിരം ഘടന xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    എങ്കിൽ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        തിരികെ false;
    }
    പ്രതിജ്ഞാ(group1->type->അക്കം_levels == group2->type->അക്കം_levels);

    bool identical = true;
    വേണ്ടി (xkb_level_index_t l = 0; l < group1->type->അക്കം_levels; l++) {
        സ്ഥിരം ഘടന xkb_level * സ്ഥിരം level1 = &group1->levels[l];
        സ്ഥിരം ഘടന xkb_level * സ്ഥിരം level2 = &group2->levels[l];

        /* Keysyms */
        എങ്കിൽ (level1->അക്കം_syms != level2->അക്കം_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->അക്കം_syms, level2->അക്കം_syms);
            identical = false;
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (level1->അക്കം_syms > 1) {
                വേണ്ടി (xkb_keysym_എണ്ണം_t k = 0; k < level1->അക്കം_syms; k++) {
                    എങ്കിൽ (level1->s.syms[k] == level2->s.syms[k])
                        തുടരുക;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        എങ്കിൽ (level1->അക്കം_actions != level2->അക്കം_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->അക്കം_actions, level2->അക്കം_actions);
            identical = false;
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (level1->അക്കം_actions > 1) {
                വേണ്ടി (xkb_action_എണ്ണം_t a = 0; a < level1->അക്കം_actions; a++) {
                    എങ്കിൽ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        തുടരുക;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (level1->അക്കം_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    തിരികെ identical;
}

സ്ഥിതം bool
keymap_compare_symbols(ഘടന xkb_context *ctx,
                       സ്ഥിരം ഘടന xkb_keymap *keymap1,
                       സ്ഥിരം ഘടന xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    എങ്കിൽ (keymap1->അക്കം_groups != keymap2->അക്കം_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->അക്കം_groups, keymap2->അക്കം_groups);
        identical = false;
    }

    എങ്കിൽ (keymap1->അക്കം_group_names != keymap2->അക്കം_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->അക്കം_group_names, keymap2->അക്കം_group_names);
        identical = false;
    } അല്ലെങ്കിൽ {
        വേണ്ടി (xkb_layout_index_t g = 0; g < keymap1->അക്കം_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            സ്ഥിരം അക്ഷരം * സ്ഥിരം name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            സ്ഥിരം അക്ഷരം * സ്ഥിരം name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            എങ്കിൽ (!streq_null(name1, name2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Group #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n", g, name1, name2);
                identical = false;
            }
        }
    }

    /*
     * Check common keycodes
     */
    സ്ഥിരം xkb_keycode_t k_max = MIN(keymap1->അക്കം_താക്കോലുകൾ, keymap2->അക്കം_താക്കോലുകൾ);
    വേണ്ടി (xkb_keycode_t k = 0; k < k_max; k++) {
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം key1 = &keymap1->താക്കോലുകൾ[k];
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം key2 = &keymap1->താക്കോലുകൾ[k];
        എങ്കിൽ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            തുടരുക;
        }

        സ്ഥിരം xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        എങ്കിൽ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        എങ്കിൽ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        എങ്കിൽ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        എങ്കിൽ (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_സംഖ്യ != key2->out_of_range_group_സംഖ്യ) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_സംഖ്യ,
                    key2->out_of_range_group_സംഖ്യ);
            identical = false;
        }
        എങ്കിൽ (key1->അക്കം_groups != key2->അക്കം_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->അക്കം_groups, key2->അക്കം_groups);
            identical = false;
        }
        സ്ഥിരം xkb_layout_index_t g_max = MIN(key1->അക്കം_groups, key2->അക്കം_groups);
        വേണ്ടി (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    തിരികെ identical;
}

/**
 * Compare two keymaps.
 *
 * This is primarily aimed for testing if two keymaps compiled from different
 * sources have the same properties.
 *
 * Return `true` if the properties to compare are identical, `false` otherwise.
 */
bool
xkb_keymap_compare(ഘടന xkb_context *ctx,
                   സ്ഥിരം ഘടന xkb_keymap *keymap1, സ്ഥിരം ഘടന xkb_keymap *keymap2,
                   എണ്ണൽ xkb_keymap_compare_property properties)
{
    bool identical = true;

    എങ്കിൽ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    എങ്കിൽ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    എങ്കിൽ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    എങ്കിൽ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    എങ്കിൽ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    തിരികെ identical;
}
