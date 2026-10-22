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
                    ध्रुव संरचना xkb_keymap *keymap1,
                    ध्रुव संरचना xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ध्रुव xkb_mod_index_t mod_max = MIN(keymap1->mods.अङ्कः_mods,
                                        keymap2->mods.अङ्कः_mods);
    अत्थाय (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        ध्रुव संरचना xkb_mod * ध्रुव mod1 = &keymap1->mods.mods[mod];
        ध्रुव संरचना xkb_mod * ध्रुव mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        सचे (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        सचे (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        सचे (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    सचे (keymap1->mods.अङ्कः_mods != keymap2->mods.अङ्कः_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.अङ्कः_mods, keymap2->mods.अङ्कः_mods);
        identical = false;
    }

    पटिनिवत्त identical;
}

स्थिर bool
keymap_compare_keycodes(संरचना xkb_context *ctx,
                        ध्रुव संरचना xkb_keymap *keymap1,
                        ध्रुव संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    सचे (keymap1->अङ्कः_कुञ्चिकाः != keymap2->अङ्कः_कुञ्चिकाः) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अङ्कः_कुञ्चिकाः, keymap2->अङ्कः_कुञ्चिकाः);
        identical = false;
    }
    सचे (keymap1->min_कुञ्चिका_code != keymap2->min_कुञ्चिका_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_कुञ्चिका_code, keymap2->min_कुञ्चिका_code);
        identical = false;
    }
    सचे (keymap1->max_कुञ्चिका_code != keymap2->max_कुञ्चिका_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अङ्कः_कुञ्चिकाः_नीचः, keymap2->अङ्कः_कुञ्चिकाः_नीचः);
        identical = false;
    }
    सचे (keymap1->max_कुञ्चिका_code != keymap2->max_कुञ्चिका_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_कुञ्चिका_code, keymap2->min_कुञ्चिका_code);
        identical = false;
    }

    /* Check common keys */
    ध्रुव xkb_keycode_t k_max = MIN(keymap1->अङ्कः_कुञ्चिकाः, keymap2->अङ्कः_कुञ्चिकाः);
    अत्थाय (xkb_keycode_t k = 0; k < k_max; k++) {
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव key1 = &keymap1->कुञ्चिकाः[k];
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव key2 = &keymap1->कुञ्चिकाः[k];
        सचे (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            अनुवर्त;
        }

        ध्रुव xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx, key1->name);
        ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx, key2->name);
        सचे (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ध्रुव darray_size_t a_max = MIN(keymap1->अङ्कः_कुञ्चिका_aliases,
                                    keymap2->अङ्कः_कुञ्चिका_aliases);
    अत्थाय (darray_size_t a = 0; a < a_max; a++) {
        ध्रुव संरचना xkb_कुञ्चिका_alias * ध्रुव entry1 = &keymap1->कुञ्चिका_aliases[a];
        ध्रुव संरचना xkb_कुञ्चिका_alias * ध्रुव entry2 = &keymap2->कुञ्चिका_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ध्रुव वर्ण * ध्रुव alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        ध्रुव वर्ण * ध्रुव alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        सचे (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ध्रुव वर्ण * ध्रुव real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        ध्रुव वर्ण * ध्रुव real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        सचे (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    सचे (keymap1->अङ्कः_कुञ्चिका_aliases != keymap2->अङ्कः_कुञ्चिका_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->अङ्कः_कुञ्चिका_aliases, keymap2->अङ्कः_कुञ्चिका_aliases);
        identical = false;
    }

    पटिनिवत्त identical;
}

स्थिर bool
keymap_compare_leds(संरचना xkb_context *ctx,
                    ध्रुव संरचना xkb_keymap *keymap1,
                    ध्रुव संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->अङ्कः_leds, keymap2->अङ्कः_leds);
    अत्थाय (xkb_led_index_t led = 0; led < led_max; led++) {
        ध्रुव संरचना xkb_led * ध्रुव led1 = &keymap1->leds[led];
        ध्रुव संरचना xkb_led * ध्रुव led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx, led1->name);
        ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx, led2->name);
        सचे (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        सचे (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        सचे (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        सचे (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        सचे (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        सचे (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    सचे (keymap1->अङ्कः_leds != keymap2->अङ्कः_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अङ्कः_leds, keymap2->अङ्कः_leds);
        identical = false;
    }

    पटिनिवत्त identical;
}

स्थिर bool
compare_types(संरचना xkb_context *ctx,
              ध्रुव संरचना xkb_keymap *keymap1, ध्रुव संरचना xkb_keymap *keymap2,
              ध्रुव संरचना xkb_कुञ्चिका_type *type1, ध्रुव संरचना xkb_कुञ्चिका_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx, type1->name);
    ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx, type2->name);
    सचे (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    सचे (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        पटिनिवत्त false;
    }

    सचे (type1->अङ्कः_levels != type2->अङ्कः_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->अङ्कः_levels, type2->अङ्कः_levels);
        /* No point to check further properties */
        पटिनिवत्त false;
    }

    सचे (type1->अङ्कः_level_names != type2->अङ्कः_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->अङ्कः_level_names, type2->अङ्कः_level_names);
        identical = false;
    } अञ्ञथा {
        अत्थाय (xkb_level_index_t l = 0; l < type1->अङ्कः_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ध्रुव वर्ण * ध्रुव lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            ध्रुव वर्ण * ध्रुव lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            सचे (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    सचे (type1->अङ्कः_entries != type2->अङ्कः_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->अङ्कः_entries, type2->अङ्कः_entries);
        identical = false;
    } अञ्ञथा {
        अत्थाय (darray_size_t e = 0; e < type1->अङ्कः_entries; e++) {
            ध्रुव संरचना xkb_कुञ्चिका_type_entry * ध्रुव entry1 = &type1->entries[e];
            ध्रुव संरचना xkb_कुञ्चिका_type_entry * ध्रुव entry2 = &type2->entries[e];
            सचे (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            सचे (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            सचे (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    पटिनिवत्त identical;
}

स्थिर bool
keymap_compare_types(संरचना xkb_context *ctx,
                     ध्रुव संरचना xkb_keymap *keymap1, ध्रुव संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ध्रुव darray_size_t t_max = MIN(keymap1->अङ्कः_types, keymap2->अङ्कः_types);
    अत्थाय (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    सचे (keymap1->अङ्कः_types != keymap2->अङ्कः_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->अङ्कः_types, keymap2->अङ्कः_types);
        identical = false;
    }

    पटिनिवत्त identical;
}

स्थिर bool
compare_groups(संरचना xkb_context *ctx,
               ध्रुव संरचना xkb_keymap *keymap1, ध्रुव संरचना xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               ध्रुव संरचना xkb_group *group1, ध्रुव संरचना xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    सचे (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        पटिनिवत्त false;
    }
    प्रतिज्ञा(group1->type->अङ्कः_levels == group2->type->अङ्कः_levels);

    bool identical = true;
    अत्थाय (xkb_level_index_t l = 0; l < group1->type->अङ्कः_levels; l++) {
        ध्रुव संरचना xkb_level * ध्रुव level1 = &group1->levels[l];
        ध्रुव संरचना xkb_level * ध्रुव level2 = &group2->levels[l];

        /* Keysyms */
        सचे (level1->अङ्कः_syms != level2->अङ्कः_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->अङ्कः_syms, level2->अङ्कः_syms);
            identical = false;
        } अञ्ञथा {
            सचे (level1->अङ्कः_syms > 1) {
                अत्थाय (xkb_keysym_गणनम्_t k = 0; k < level1->अङ्कः_syms; k++) {
                    सचे (level1->s.syms[k] == level2->s.syms[k])
                        अनुवर्त;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } अञ्ञथा सचे (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        सचे (level1->अङ्कः_actions != level2->अङ्कः_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->अङ्कः_actions, level2->अङ्कः_actions);
            identical = false;
        } अञ्ञथा {
            सचे (level1->अङ्कः_actions > 1) {
                अत्थाय (xkb_action_गणनम्_t a = 0; a < level1->अङ्कः_actions; a++) {
                    सचे (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        अनुवर्त;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } अञ्ञथा सचे (level1->अङ्कः_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    पटिनिवत्त identical;
}

स्थिर bool
keymap_compare_symbols(संरचना xkb_context *ctx,
                       ध्रुव संरचना xkb_keymap *keymap1,
                       ध्रुव संरचना xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    सचे (keymap1->अङ्कः_groups != keymap2->अङ्कः_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अङ्कः_groups, keymap2->अङ्कः_groups);
        identical = false;
    }

    सचे (keymap1->अङ्कः_group_names != keymap2->अङ्कः_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->अङ्कः_group_names, keymap2->अङ्कः_group_names);
        identical = false;
    } अञ्ञथा {
        अत्थाय (xkb_layout_index_t g = 0; g < keymap1->अङ्कः_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ध्रुव वर्ण * ध्रुव name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            ध्रुव वर्ण * ध्रुव name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            सचे (!streq_null(name1, name2)) {
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
    ध्रुव xkb_keycode_t k_max = MIN(keymap1->अङ्कः_कुञ्चिकाः, keymap2->अङ्कः_कुञ्चिकाः);
    अत्थाय (xkb_keycode_t k = 0; k < k_max; k++) {
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव key1 = &keymap1->कुञ्चिकाः[k];
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव key2 = &keymap1->कुञ्चिकाः[k];
        सचे (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            अनुवर्त;
        }

        ध्रुव xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        सचे (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        सचे (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        सचे (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        सचे (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_सङ्ख्या != key2->out_of_range_group_सङ्ख्या) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_सङ्ख्या,
                    key2->out_of_range_group_सङ्ख्या);
            identical = false;
        }
        सचे (key1->अङ्कः_groups != key2->अङ्कः_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->अङ्कः_groups, key2->अङ्कः_groups);
            identical = false;
        }
        ध्रुव xkb_layout_index_t g_max = MIN(key1->अङ्कः_groups, key2->अङ्कः_groups);
        अत्थाय (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    पटिनिवत्त identical;
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
                   ध्रुव संरचना xkb_keymap *keymap1, ध्रुव संरचना xkb_keymap *keymap2,
                   गणना xkb_keymap_compare_property properties)
{
    bool identical = true;

    सचे (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    सचे (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    सचे (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    सचे (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    सचे (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    पटिनिवत्त identical;
}
