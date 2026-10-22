/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "src/messages-codes.h"
#ಅನ್ತರ್ಭಾವ "src/utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


ಸ್ಥಿರ bool
keymap_compare_mods(ಸಂರಚನಾ xkb_context *ctx,
                    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1,
                    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ಧ್ರುವ xkb_mod_index_t mod_max = MIN(keymap1->mods.ಅಂಕೆ_mods,
                                        keymap2->mods.ಅಂಕೆ_mods);
    ಪ್ರತಿ (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_mod * ಧ್ರುವ mod1 = &keymap1->mods.mods[mod];
        ಧ್ರುವ ಸಂರಚನಾ xkb_mod * ಧ್ರುವ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        ಯದಿ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ಯದಿ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ಯದಿ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ಯದಿ (keymap1->mods.ಅಂಕೆ_mods != keymap2->mods.ಅಂಕೆ_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.ಅಂಕೆ_mods, keymap2->mods.ಅಂಕೆ_mods);
        identical = false;
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
keymap_compare_keycodes(ಸಂರಚನಾ xkb_context *ctx,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ಯದಿ (keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು != keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
        identical = false;
    }
    ಯದಿ (keymap1->min_ಕೀಲಿ_code != keymap2->min_ಕೀಲಿ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ಕೀಲಿ_code, keymap2->min_ಕೀಲಿ_code);
        identical = false;
    }
    ಯದಿ (keymap1->max_ಕೀಲಿ_code != keymap2->max_ಕೀಲಿ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ);
        identical = false;
    }
    ಯದಿ (keymap1->max_ಕೀಲಿ_code != keymap2->max_ಕೀಲಿ_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ಕೀಲಿ_code, keymap2->min_ಕೀಲಿ_code);
        identical = false;
    }

    /* Check common keys */
    ಧ್ರುವ xkb_keycode_t k_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
    ಪ್ರತಿ (xkb_keycode_t k = 0; k < k_max; k++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ key1 = &keymap1->ಕೀಲಿಗಳು[k];
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ key2 = &keymap1->ಕೀಲಿಗಳು[k];
        ಯದಿ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ಅನುವರ್ತ;
        }

        ಧ್ರುವ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx, key1->name);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx, key2->name);
        ಯದಿ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ಧ್ರುವ darray_size_t a_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases,
                                    keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases);
    ಪ್ರತಿ (darray_size_t a = 0; a < a_max; a++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ entry1 = &keymap1->ಕೀಲಿ_aliases[a];
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ entry2 = &keymap2->ಕೀಲಿ_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        ಯದಿ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        ಯದಿ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ಯದಿ (keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases != keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases, keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases);
        identical = false;
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
keymap_compare_leds(ಸಂರಚನಾ xkb_context *ctx,
                    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1,
                    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->ಅಂಕೆ_leds, keymap2->ಅಂಕೆ_leds);
    ಪ್ರತಿ (xkb_led_index_t led = 0; led < led_max; led++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_led * ಧ್ರುವ led1 = &keymap1->leds[led];
        ಧ್ರುವ ಸಂರಚನಾ xkb_led * ಧ್ರುವ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx, led1->name);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx, led2->name);
        ಯದಿ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ಯದಿ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ಯದಿ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ಯದಿ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ಯದಿ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ಯದಿ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ಯದಿ (keymap1->ಅಂಕೆ_leds != keymap2->ಅಂಕೆ_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_leds, keymap2->ಅಂಕೆ_leds);
        identical = false;
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
compare_types(ಸಂರಚನಾ xkb_context *ctx,
              ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2,
              ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type1, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx, type1->name);
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx, type2->name);
    ಯದಿ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ಯದಿ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (type1->ಅಂಕೆ_levels != type2->ಅಂಕೆ_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ಅಂಕೆ_levels, type2->ಅಂಕೆ_levels);
        /* No point to check further properties */
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (type1->ಅಂಕೆ_level_names != type2->ಅಂಕೆ_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ಅಂಕೆ_level_names, type2->ಅಂಕೆ_level_names);
        identical = false;
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿ (xkb_level_index_t l = 0; l < type1->ಅಂಕೆ_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            ಯದಿ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ಯದಿ (type1->ಅಂಕೆ_entries != type2->ಅಂಕೆ_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->ಅಂಕೆ_entries, type2->ಅಂಕೆ_entries);
        identical = false;
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿ (darray_size_t e = 0; e < type1->ಅಂಕೆ_entries; e++) {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry * ಧ್ರುವ entry1 = &type1->entries[e];
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry * ಧ್ರುವ entry2 = &type2->entries[e];
            ಯದಿ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ಯದಿ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ಯದಿ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
keymap_compare_types(ಸಂರಚನಾ xkb_context *ctx,
                     ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ಧ್ರುವ darray_size_t t_max = MIN(keymap1->ಅಂಕೆ_types, keymap2->ಅಂಕೆ_types);
    ಪ್ರತಿ (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ಯದಿ (keymap1->ಅಂಕೆ_types != keymap2->ಅಂಕೆ_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->ಅಂಕೆ_types, keymap2->ಅಂಕೆ_types);
        identical = false;
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
compare_groups(ಸಂರಚನಾ xkb_context *ctx,
               ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               ಧ್ರುವ ಸಂರಚನಾ xkb_group *group1, ಧ್ರುವ ಸಂರಚನಾ xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ಯದಿ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಪ್ರತಿಜ್ಞಾ(group1->type->ಅಂಕೆ_levels == group2->type->ಅಂಕೆ_levels);

    bool identical = true;
    ಪ್ರತಿ (xkb_level_index_t l = 0; l < group1->type->ಅಂಕೆ_levels; l++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_level * ಧ್ರುವ level1 = &group1->levels[l];
        ಧ್ರುವ ಸಂರಚನಾ xkb_level * ಧ್ರುವ level2 = &group2->levels[l];

        /* Keysyms */
        ಯದಿ (level1->ಅಂಕೆ_syms != level2->ಅಂಕೆ_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->ಅಂಕೆ_syms, level2->ಅಂಕೆ_syms);
            identical = false;
        } ಅನ್ಯಥಾ {
            ಯದಿ (level1->ಅಂಕೆ_syms > 1) {
                ಪ್ರತಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < level1->ಅಂಕೆ_syms; k++) {
                    ಯದಿ (level1->s.syms[k] == level2->s.syms[k])
                        ಅನುವರ್ತ;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ಅನ್ಯಥಾ ಯದಿ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ಯದಿ (level1->ಅಂಕೆ_actions != level2->ಅಂಕೆ_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->ಅಂಕೆ_actions, level2->ಅಂಕೆ_actions);
            identical = false;
        } ಅನ್ಯಥಾ {
            ಯದಿ (level1->ಅಂಕೆ_actions > 1) {
                ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t a = 0; a < level1->ಅಂಕೆ_actions; a++) {
                    ಯದಿ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        ಅನುವರ್ತ;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ಅನ್ಯಥಾ ಯದಿ (level1->ಅಂಕೆ_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ identical;
}

ಸ್ಥಿರ bool
keymap_compare_symbols(ಸಂರಚನಾ xkb_context *ctx,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ಯದಿ (keymap1->ಅಂಕೆ_groups != keymap2->ಅಂಕೆ_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_groups, keymap2->ಅಂಕೆ_groups);
        identical = false;
    }

    ಯದಿ (keymap1->ಅಂಕೆ_group_names != keymap2->ಅಂಕೆ_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_group_names, keymap2->ಅಂಕೆ_group_names);
        identical = false;
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿ (xkb_layout_index_t g = 0; g < keymap1->ಅಂಕೆ_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            ಯದಿ (!streq_null(name1, name2)) {
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
    ಧ್ರುವ xkb_keycode_t k_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
    ಪ್ರತಿ (xkb_keycode_t k = 0; k < k_max; k++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ key1 = &keymap1->ಕೀಲಿಗಳು[k];
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ key2 = &keymap1->ಕೀಲಿಗಳು[k];
        ಯದಿ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ಅನುವರ್ತ;
        }

        ಧ್ರುವ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ಯದಿ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ಯದಿ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ಯದಿ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ಯದಿ (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_ಸಂಖ್ಯೆ != key2->out_of_range_group_ಸಂಖ್ಯೆ) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_ಸಂಖ್ಯೆ,
                    key2->out_of_range_group_ಸಂಖ್ಯೆ);
            identical = false;
        }
        ಯದಿ (key1->ಅಂಕೆ_groups != key2->ಅಂಕೆ_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->ಅಂಕೆ_groups, key2->ಅಂಕೆ_groups);
            identical = false;
        }
        ಧ್ರುವ xkb_layout_index_t g_max = MIN(key1->ಅಂಕೆ_groups, key2->ಅಂಕೆ_groups);
        ಪ್ರತಿ (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ಪ್ರತ್ಯಾಗಮ identical;
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
xkb_keymap_compare(ಸಂರಚನಾ xkb_context *ctx,
                   ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap1, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap2,
                   ಗಣನಾ xkb_keymap_compare_property properties)
{
    bool identical = true;

    ಯದಿ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ಯದಿ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ಯದಿ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ಯದಿ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ಯದಿ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ಪ್ರತ್ಯಾಗಮ identical;
}
