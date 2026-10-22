/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>

#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "src/messages-codes.h"
#অন্তর্ভাৱ "src/utils.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


স্থির bool
keymap_compare_mods(সংরচনা xkb_context *ctx,
                    ধ্রুৱ সংরচনা xkb_keymap *keymap1,
                    ধ্রুৱ সংরচনা xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ধ্রুৱ xkb_mod_index_t mod_max = MIN(keymap1->mods.অঙ্ক_mods,
                                        keymap2->mods.অঙ্ক_mods);
    বাবে (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        ধ্রুৱ সংরচনা xkb_mod * ধ্রুৱ mod1 = &keymap1->mods.mods[mod];
        ধ্রুৱ সংরচনা xkb_mod * ধ্রুৱ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        যদি (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        যদি (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        যদি (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    যদি (keymap1->mods.অঙ্ক_mods != keymap2->mods.অঙ্ক_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.অঙ্ক_mods, keymap2->mods.অঙ্ক_mods);
        identical = false;
    }

    ঘূৰাই identical;
}

স্থির bool
keymap_compare_keycodes(সংরচনা xkb_context *ctx,
                        ধ্রুৱ সংরচনা xkb_keymap *keymap1,
                        ধ্রুৱ সংরচনা xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    যদি (keymap1->অঙ্ক_চাবিগুলি != keymap2->অঙ্ক_চাবিগুলি) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->অঙ্ক_চাবিগুলি, keymap2->অঙ্ক_চাবিগুলি);
        identical = false;
    }
    যদি (keymap1->min_চাবি_code != keymap2->min_চাবি_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_চাবি_code, keymap2->min_চাবি_code);
        identical = false;
    }
    যদি (keymap1->max_চাবি_code != keymap2->max_চাবি_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->অঙ্ক_চাবিগুলি_নিচেৰ, keymap2->অঙ্ক_চাবিগুলি_নিচেৰ);
        identical = false;
    }
    যদি (keymap1->max_চাবি_code != keymap2->max_চাবি_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_চাবি_code, keymap2->min_চাবি_code);
        identical = false;
    }

    /* Check common keys */
    ধ্রুৱ xkb_keycode_t k_max = MIN(keymap1->অঙ্ক_চাবিগুলি, keymap2->অঙ্ক_চাবিগুলি);
    বাবে (xkb_keycode_t k = 0; k < k_max; k++) {
        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ key1 = &keymap1->চাবিগুলি[k];
        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ key2 = &keymap1->চাবিগুলি[k];
        যদি (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            অনুৱর্ত;
        }

        ধ্রুৱ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx, key1->name);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx, key2->name);
        যদি (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ধ্রুৱ darray_size_t a_max = MIN(keymap1->অঙ্ক_চাবি_aliases,
                                    keymap2->অঙ্ক_চাবি_aliases);
    বাবে (darray_size_t a = 0; a < a_max; a++) {
        ধ্রুৱ সংরচনা xkb_চাবি_alias * ধ্রুৱ entry1 = &keymap1->চাবি_aliases[a];
        ধ্রুৱ সংরচনা xkb_চাবি_alias * ধ্রুৱ entry2 = &keymap2->চাবি_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ধ্রুৱ ৱর্ণ * ধ্রুৱ alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        যদি (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ধ্রুৱ ৱর্ণ * ধ্রুৱ real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        যদি (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    যদি (keymap1->অঙ্ক_চাবি_aliases != keymap2->অঙ্ক_চাবি_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->অঙ্ক_চাবি_aliases, keymap2->অঙ্ক_চাবি_aliases);
        identical = false;
    }

    ঘূৰাই identical;
}

স্থির bool
keymap_compare_leds(সংরচনা xkb_context *ctx,
                    ধ্রুৱ সংরচনা xkb_keymap *keymap1,
                    ধ্রুৱ সংরচনা xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->অঙ্ক_leds, keymap2->অঙ্ক_leds);
    বাবে (xkb_led_index_t led = 0; led < led_max; led++) {
        ধ্রুৱ সংরচনা xkb_led * ধ্রুৱ led1 = &keymap1->leds[led];
        ধ্রুৱ সংরচনা xkb_led * ধ্রুৱ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx, led1->name);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx, led2->name);
        যদি (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        যদি (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        যদি (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        যদি (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        যদি (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        যদি (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    যদি (keymap1->অঙ্ক_leds != keymap2->অঙ্ক_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->অঙ্ক_leds, keymap2->অঙ্ক_leds);
        identical = false;
    }

    ঘূৰাই identical;
}

স্থির bool
compare_types(সংরচনা xkb_context *ctx,
              ধ্রুৱ সংরচনা xkb_keymap *keymap1, ধ্রুৱ সংরচনা xkb_keymap *keymap2,
              ধ্রুৱ সংরচনা xkb_চাবি_type *type1, ধ্রুৱ সংরচনা xkb_চাবি_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx, type1->name);
    ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx, type2->name);
    যদি (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    যদি (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ঘূৰাই false;
    }

    যদি (type1->অঙ্ক_levels != type2->অঙ্ক_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->অঙ্ক_levels, type2->অঙ্ক_levels);
        /* No point to check further properties */
        ঘূৰাই false;
    }

    যদি (type1->অঙ্ক_level_names != type2->অঙ্ক_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->অঙ্ক_level_names, type2->অঙ্ক_level_names);
        identical = false;
    } অন্যথা {
        বাবে (xkb_level_index_t l = 0; l < type1->অঙ্ক_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ধ্রুৱ ৱর্ণ * ধ্রুৱ lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            ধ্রুৱ ৱর্ণ * ধ্রুৱ lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            যদি (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    যদি (type1->অঙ্ক_entries != type2->অঙ্ক_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->অঙ্ক_entries, type2->অঙ্ক_entries);
        identical = false;
    } অন্যথা {
        বাবে (darray_size_t e = 0; e < type1->অঙ্ক_entries; e++) {
            ধ্রুৱ সংরচনা xkb_চাবি_type_entry * ধ্রুৱ entry1 = &type1->entries[e];
            ধ্রুৱ সংরচনা xkb_চাবি_type_entry * ধ্রুৱ entry2 = &type2->entries[e];
            যদি (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            যদি (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            যদি (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ঘূৰাই identical;
}

স্থির bool
keymap_compare_types(সংরচনা xkb_context *ctx,
                     ধ্রুৱ সংরচনা xkb_keymap *keymap1, ধ্রুৱ সংরচনা xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ধ্রুৱ darray_size_t t_max = MIN(keymap1->অঙ্ক_types, keymap2->অঙ্ক_types);
    বাবে (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    যদি (keymap1->অঙ্ক_types != keymap2->অঙ্ক_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->অঙ্ক_types, keymap2->অঙ্ক_types);
        identical = false;
    }

    ঘূৰাই identical;
}

স্থির bool
compare_groups(সংরচনা xkb_context *ctx,
               ধ্রুৱ সংরচনা xkb_keymap *keymap1, ধ্রুৱ সংরচনা xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               ধ্রুৱ সংরচনা xkb_group *group1, ধ্রুৱ সংরচনা xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    যদি (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ঘূৰাই false;
    }
    প্রতিজ্ঞা(group1->type->অঙ্ক_levels == group2->type->অঙ্ক_levels);

    bool identical = true;
    বাবে (xkb_level_index_t l = 0; l < group1->type->অঙ্ক_levels; l++) {
        ধ্রুৱ সংরচনা xkb_level * ধ্রুৱ level1 = &group1->levels[l];
        ধ্রুৱ সংরচনা xkb_level * ধ্রুৱ level2 = &group2->levels[l];

        /* Keysyms */
        যদি (level1->অঙ্ক_syms != level2->অঙ্ক_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->অঙ্ক_syms, level2->অঙ্ক_syms);
            identical = false;
        } অন্যথা {
            যদি (level1->অঙ্ক_syms > 1) {
                বাবে (xkb_keysym_গুনতি_t k = 0; k < level1->অঙ্ক_syms; k++) {
                    যদি (level1->s.syms[k] == level2->s.syms[k])
                        অনুৱর্ত;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } অন্যথা যদি (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        যদি (level1->অঙ্ক_actions != level2->অঙ্ক_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->অঙ্ক_actions, level2->অঙ্ক_actions);
            identical = false;
        } অন্যথা {
            যদি (level1->অঙ্ক_actions > 1) {
                বাবে (xkb_action_গুনতি_t a = 0; a < level1->অঙ্ক_actions; a++) {
                    যদি (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        অনুৱর্ত;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } অন্যথা যদি (level1->অঙ্ক_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ঘূৰাই identical;
}

স্থির bool
keymap_compare_symbols(সংরচনা xkb_context *ctx,
                       ধ্রুৱ সংরচনা xkb_keymap *keymap1,
                       ধ্রুৱ সংরচনা xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    যদি (keymap1->অঙ্ক_groups != keymap2->অঙ্ক_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->অঙ্ক_groups, keymap2->অঙ্ক_groups);
        identical = false;
    }

    যদি (keymap1->অঙ্ক_group_names != keymap2->অঙ্ক_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->অঙ্ক_group_names, keymap2->অঙ্ক_group_names);
        identical = false;
    } অন্যথা {
        বাবে (xkb_layout_index_t g = 0; g < keymap1->অঙ্ক_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ধ্রুৱ ৱর্ণ * ধ্রুৱ name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            ধ্রুৱ ৱর্ণ * ধ্রুৱ name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            যদি (!streq_null(name1, name2)) {
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
    ধ্রুৱ xkb_keycode_t k_max = MIN(keymap1->অঙ্ক_চাবিগুলি, keymap2->অঙ্ক_চাবিগুলি);
    বাবে (xkb_keycode_t k = 0; k < k_max; k++) {
        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ key1 = &keymap1->চাবিগুলি[k];
        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ key2 = &keymap1->চাবিগুলি[k];
        যদি (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            অনুৱর্ত;
        }

        ধ্রুৱ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        যদি (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        যদি (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        যদি (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        যদি (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_সংখ্যা != key2->out_of_range_group_সংখ্যা) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_সংখ্যা,
                    key2->out_of_range_group_সংখ্যা);
            identical = false;
        }
        যদি (key1->অঙ্ক_groups != key2->অঙ্ক_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->অঙ্ক_groups, key2->অঙ্ক_groups);
            identical = false;
        }
        ধ্রুৱ xkb_layout_index_t g_max = MIN(key1->অঙ্ক_groups, key2->অঙ্ক_groups);
        বাবে (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ঘূৰাই identical;
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
xkb_keymap_compare(সংরচনা xkb_context *ctx,
                   ধ্রুৱ সংরচনা xkb_keymap *keymap1, ধ্রুৱ সংরচনা xkb_keymap *keymap2,
                   গণনা xkb_keymap_compare_property properties)
{
    bool identical = true;

    যদি (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    যদি (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    যদি (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    যদি (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    যদি (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ঘূৰাই identical;
}
