/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "keymap.h"
#अन्तर्भाव "src/messages-codes.h"
#अन्तर्भाव "src/utils.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


स्थिर bool
keymap_compare_mods(संरचना xkb_context *ctx,
                    स्थिरांक संरचना xkb_keymap *keymap1,
                    स्थिरांक संरचना xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    स्थिरांक xkb_mod_index_t mod_max = MIN(keymap1->mods.अंक_mods,
                                        keymap2->mods.अंक_mods);
    साठी (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        स्थिरांक संरचना xkb_mod * स्थिरांक mod1 = &keymap1->mods.mods[mod];
        स्थिरांक संरचना xkb_mod * स्थिरांक mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        जर (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        जर (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        जर (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    जर (keymap1->mods.अंक_mods != keymap2->mods.अंक_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.अंक_mods, keymap2->mods.अंक_mods);
        identical = false;
    }

    परत identical;
}

स्थिर bool
keymap_compare_keycodes(संरचना xkb_context *ctx,
                        स्थिरांक संरचना xkb_keymap *keymap1,
                        स्थिरांक संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    जर (keymap1->अंक_किल्ल्या != keymap2->अंक_किल्ल्या) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अंक_किल्ल्या, keymap2->अंक_किल्ल्या);
        identical = false;
    }
    जर (keymap1->min_किल्ली_code != keymap2->min_किल्ली_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_किल्ली_code, keymap2->min_किल्ली_code);
        identical = false;
    }
    जर (keymap1->max_किल्ली_code != keymap2->max_किल्ली_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अंक_किल्ल्या_खालचा, keymap2->अंक_किल्ल्या_खालचा);
        identical = false;
    }
    जर (keymap1->max_किल्ली_code != keymap2->max_किल्ली_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_किल्ली_code, keymap2->min_किल्ली_code);
        identical = false;
    }

    /* Check common keys */
    स्थिरांक xkb_keycode_t k_max = MIN(keymap1->अंक_किल्ल्या, keymap2->अंक_किल्ल्या);
    साठी (xkb_keycode_t k = 0; k < k_max; k++) {
        स्थिरांक संरचना xkb_किल्ली * स्थिरांक key1 = &keymap1->किल्ल्या[k];
        स्थिरांक संरचना xkb_किल्ली * स्थिरांक key2 = &keymap1->किल्ल्या[k];
        जर (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            सुरूठेवा;
        }

        स्थिरांक xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx, key1->name);
        स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx, key2->name);
        जर (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    स्थिरांक darray_size_t a_max = MIN(keymap1->अंक_किल्ली_aliases,
                                    keymap2->अंक_किल्ली_aliases);
    साठी (darray_size_t a = 0; a < a_max; a++) {
        स्थिरांक संरचना xkb_किल्ली_alias * स्थिरांक entry1 = &keymap1->किल्ली_aliases[a];
        स्थिरांक संरचना xkb_किल्ली_alias * स्थिरांक entry2 = &keymap2->किल्ली_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        स्थिरांक अक्षर * स्थिरांक alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        स्थिरांक अक्षर * स्थिरांक alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        जर (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        स्थिरांक अक्षर * स्थिरांक real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        स्थिरांक अक्षर * स्थिरांक real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        जर (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    जर (keymap1->अंक_किल्ली_aliases != keymap2->अंक_किल्ली_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->अंक_किल्ली_aliases, keymap2->अंक_किल्ली_aliases);
        identical = false;
    }

    परत identical;
}

स्थिर bool
keymap_compare_leds(संरचना xkb_context *ctx,
                    स्थिरांक संरचना xkb_keymap *keymap1,
                    स्थिरांक संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->अंक_leds, keymap2->अंक_leds);
    साठी (xkb_led_index_t led = 0; led < led_max; led++) {
        स्थिरांक संरचना xkb_led * स्थिरांक led1 = &keymap1->leds[led];
        स्थिरांक संरचना xkb_led * स्थिरांक led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx, led1->name);
        स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx, led2->name);
        जर (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        जर (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        जर (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        जर (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        जर (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        जर (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    जर (keymap1->अंक_leds != keymap2->अंक_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अंक_leds, keymap2->अंक_leds);
        identical = false;
    }

    परत identical;
}

स्थिर bool
compare_types(संरचना xkb_context *ctx,
              स्थिरांक संरचना xkb_keymap *keymap1, स्थिरांक संरचना xkb_keymap *keymap2,
              स्थिरांक संरचना xkb_किल्ली_type *type1, स्थिरांक संरचना xkb_किल्ली_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx, type1->name);
    स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx, type2->name);
    जर (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    जर (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        परत false;
    }

    जर (type1->अंक_levels != type2->अंक_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->अंक_levels, type2->अंक_levels);
        /* No point to check further properties */
        परत false;
    }

    जर (type1->अंक_level_names != type2->अंक_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->अंक_level_names, type2->अंक_level_names);
        identical = false;
    } अन्यथा {
        साठी (xkb_level_index_t l = 0; l < type1->अंक_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            स्थिरांक अक्षर * स्थिरांक lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            स्थिरांक अक्षर * स्थिरांक lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            जर (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    जर (type1->अंक_entries != type2->अंक_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->अंक_entries, type2->अंक_entries);
        identical = false;
    } अन्यथा {
        साठी (darray_size_t e = 0; e < type1->अंक_entries; e++) {
            स्थिरांक संरचना xkb_किल्ली_type_entry * स्थिरांक entry1 = &type1->entries[e];
            स्थिरांक संरचना xkb_किल्ली_type_entry * स्थिरांक entry2 = &type2->entries[e];
            जर (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            जर (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            जर (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    परत identical;
}

स्थिर bool
keymap_compare_types(संरचना xkb_context *ctx,
                     स्थिरांक संरचना xkb_keymap *keymap1, स्थिरांक संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    स्थिरांक darray_size_t t_max = MIN(keymap1->अंक_types, keymap2->अंक_types);
    साठी (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    जर (keymap1->अंक_types != keymap2->अंक_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->अंक_types, keymap2->अंक_types);
        identical = false;
    }

    परत identical;
}

स्थिर bool
compare_groups(संरचना xkb_context *ctx,
               स्थिरांक संरचना xkb_keymap *keymap1, स्थिरांक संरचना xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               स्थिरांक संरचना xkb_group *group1, स्थिरांक संरचना xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    जर (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        परत false;
    }
    प्रतिज्ञा(group1->type->अंक_levels == group2->type->अंक_levels);

    bool identical = true;
    साठी (xkb_level_index_t l = 0; l < group1->type->अंक_levels; l++) {
        स्थिरांक संरचना xkb_level * स्थिरांक level1 = &group1->levels[l];
        स्थिरांक संरचना xkb_level * स्थिरांक level2 = &group2->levels[l];

        /* Keysyms */
        जर (level1->अंक_syms != level2->अंक_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->अंक_syms, level2->अंक_syms);
            identical = false;
        } अन्यथा {
            जर (level1->अंक_syms > 1) {
                साठी (xkb_keysym_मोजणी_t k = 0; k < level1->अंक_syms; k++) {
                    जर (level1->s.syms[k] == level2->s.syms[k])
                        सुरूठेवा;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } अन्यथा जर (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        जर (level1->अंक_actions != level2->अंक_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->अंक_actions, level2->अंक_actions);
            identical = false;
        } अन्यथा {
            जर (level1->अंक_actions > 1) {
                साठी (xkb_action_मोजणी_t a = 0; a < level1->अंक_actions; a++) {
                    जर (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        सुरूठेवा;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } अन्यथा जर (level1->अंक_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    परत identical;
}

स्थिर bool
keymap_compare_symbols(संरचना xkb_context *ctx,
                       स्थिरांक संरचना xkb_keymap *keymap1,
                       स्थिरांक संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    जर (keymap1->अंक_groups != keymap2->अंक_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अंक_groups, keymap2->अंक_groups);
        identical = false;
    }

    जर (keymap1->अंक_group_names != keymap2->अंक_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अंक_group_names, keymap2->अंक_group_names);
        identical = false;
    } अन्यथा {
        साठी (xkb_layout_index_t g = 0; g < keymap1->अंक_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            स्थिरांक अक्षर * स्थिरांक name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            स्थिरांक अक्षर * स्थिरांक name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            जर (!streq_null(name1, name2)) {
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
    स्थिरांक xkb_keycode_t k_max = MIN(keymap1->अंक_किल्ल्या, keymap2->अंक_किल्ल्या);
    साठी (xkb_keycode_t k = 0; k < k_max; k++) {
        स्थिरांक संरचना xkb_किल्ली * स्थिरांक key1 = &keymap1->किल्ल्या[k];
        स्थिरांक संरचना xkb_किल्ली * स्थिरांक key2 = &keymap1->किल्ल्या[k];
        जर (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            सुरूठेवा;
        }

        स्थिरांक xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        जर (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        जर (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        जर (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        जर (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_संख्या != key2->out_of_range_group_संख्या) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_संख्या,
                    key2->out_of_range_group_संख्या);
            identical = false;
        }
        जर (key1->अंक_groups != key2->अंक_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->अंक_groups, key2->अंक_groups);
            identical = false;
        }
        स्थिरांक xkb_layout_index_t g_max = MIN(key1->अंक_groups, key2->अंक_groups);
        साठी (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    परत identical;
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
xkb_keymap_compare(संरचना xkb_context *ctx,
                   स्थिरांक संरचना xkb_keymap *keymap1, स्थिरांक संरचना xkb_keymap *keymap2,
                   गणना xkb_keymap_compare_property properties)
{
    bool identical = true;

    जर (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    जर (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    जर (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    जर (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    जर (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    परत identical;
}
