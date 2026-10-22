/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "src/messages-codes.h"
#அந்தர்பாவ "src/utils.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


நிலையான bool
keymap_compare_mods(கட்டமைப்பு xkb_context *ctx,
                    மாறிலி கட்டமைப்பு xkb_keymap *keymap1,
                    மாறிலி கட்டமைப்பு xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    மாறிலி xkb_mod_index_t mod_max = MIN(keymap1->mods.இலக்கம்_mods,
                                        keymap2->mods.இலக்கம்_mods);
    க்கு (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        மாறிலி கட்டமைப்பு xkb_mod * மாறிலி mod1 = &keymap1->mods.mods[mod];
        மாறிலி கட்டமைப்பு xkb_mod * மாறிலி mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        எனில் (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        எனில் (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        எனில் (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    எனில் (keymap1->mods.இலக்கம்_mods != keymap2->mods.இலக்கம்_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.இலக்கம்_mods, keymap2->mods.இலக்கம்_mods);
        identical = false;
    }

    திரும்பு identical;
}

நிலையான bool
keymap_compare_keycodes(கட்டமைப்பு xkb_context *ctx,
                        மாறிலி கட்டமைப்பு xkb_keymap *keymap1,
                        மாறிலி கட்டமைப்பு xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    எனில் (keymap1->இலக்கம்_சாவிகள் != keymap2->இலக்கம்_சாவிகள்) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->இலக்கம்_சாவிகள், keymap2->இலக்கம்_சாவிகள்);
        identical = false;
    }
    எனில் (keymap1->min_சாவி_code != keymap2->min_சாவி_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_சாவி_code, keymap2->min_சாவி_code);
        identical = false;
    }
    எனில் (keymap1->max_சாவி_code != keymap2->max_சாவி_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->இலக்கம்_சாவிகள்_கீழ், keymap2->இலக்கம்_சாவிகள்_கீழ்);
        identical = false;
    }
    எனில் (keymap1->max_சாவி_code != keymap2->max_சாவி_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_சாவி_code, keymap2->min_சாவி_code);
        identical = false;
    }

    /* Check common keys */
    மாறிலி xkb_keycode_t k_max = MIN(keymap1->இலக்கம்_சாவிகள், keymap2->இலக்கம்_சாவிகள்);
    க்கு (xkb_keycode_t k = 0; k < k_max; k++) {
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி key1 = &keymap1->சாவிகள்[k];
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி key2 = &keymap1->சாவிகள்[k];
        எனில் (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            தொடர்;
        }

        மாறிலி xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx, key1->name);
        மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx, key2->name);
        எனில் (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    மாறிலி darray_size_t a_max = MIN(keymap1->இலக்கம்_சாவி_aliases,
                                    keymap2->இலக்கம்_சாவி_aliases);
    க்கு (darray_size_t a = 0; a < a_max; a++) {
        மாறிலி கட்டமைப்பு xkb_சாவி_alias * மாறிலி entry1 = &keymap1->சாவி_aliases[a];
        மாறிலி கட்டமைப்பு xkb_சாவி_alias * மாறிலி entry2 = &keymap2->சாவி_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        மாறிலி எழுத்து * மாறிலி alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        மாறிலி எழுத்து * மாறிலி alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        எனில் (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        மாறிலி எழுத்து * மாறிலி real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        மாறிலி எழுத்து * மாறிலி real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        எனில் (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    எனில் (keymap1->இலக்கம்_சாவி_aliases != keymap2->இலக்கம்_சாவி_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->இலக்கம்_சாவி_aliases, keymap2->இலக்கம்_சாவி_aliases);
        identical = false;
    }

    திரும்பு identical;
}

நிலையான bool
keymap_compare_leds(கட்டமைப்பு xkb_context *ctx,
                    மாறிலி கட்டமைப்பு xkb_keymap *keymap1,
                    மாறிலி கட்டமைப்பு xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->இலக்கம்_leds, keymap2->இலக்கம்_leds);
    க்கு (xkb_led_index_t led = 0; led < led_max; led++) {
        மாறிலி கட்டமைப்பு xkb_led * மாறிலி led1 = &keymap1->leds[led];
        மாறிலி கட்டமைப்பு xkb_led * மாறிலி led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx, led1->name);
        மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx, led2->name);
        எனில் (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        எனில் (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        எனில் (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        எனில் (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        எனில் (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        எனில் (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    எனில் (keymap1->இலக்கம்_leds != keymap2->இலக்கம்_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->இலக்கம்_leds, keymap2->இலக்கம்_leds);
        identical = false;
    }

    திரும்பு identical;
}

நிலையான bool
compare_types(கட்டமைப்பு xkb_context *ctx,
              மாறிலி கட்டமைப்பு xkb_keymap *keymap1, மாறிலி கட்டமைப்பு xkb_keymap *keymap2,
              மாறிலி கட்டமைப்பு xkb_சாவி_type *type1, மாறிலி கட்டமைப்பு xkb_சாவி_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx, type1->name);
    மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx, type2->name);
    எனில் (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    எனில் (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        திரும்பு false;
    }

    எனில் (type1->இலக்கம்_levels != type2->இலக்கம்_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->இலக்கம்_levels, type2->இலக்கம்_levels);
        /* No point to check further properties */
        திரும்பு false;
    }

    எனில் (type1->இலக்கம்_level_names != type2->இலக்கம்_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->இலக்கம்_level_names, type2->இலக்கம்_level_names);
        identical = false;
    } இல்லையெனில் {
        க்கு (xkb_level_index_t l = 0; l < type1->இலக்கம்_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            மாறிலி எழுத்து * மாறிலி lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            மாறிலி எழுத்து * மாறிலி lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            எனில் (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    எனில் (type1->இலக்கம்_entries != type2->இலக்கம்_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->இலக்கம்_entries, type2->இலக்கம்_entries);
        identical = false;
    } இல்லையெனில் {
        க்கு (darray_size_t e = 0; e < type1->இலக்கம்_entries; e++) {
            மாறிலி கட்டமைப்பு xkb_சாவி_type_entry * மாறிலி entry1 = &type1->entries[e];
            மாறிலி கட்டமைப்பு xkb_சாவி_type_entry * மாறிலி entry2 = &type2->entries[e];
            எனில் (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            எனில் (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            எனில் (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    திரும்பு identical;
}

நிலையான bool
keymap_compare_types(கட்டமைப்பு xkb_context *ctx,
                     மாறிலி கட்டமைப்பு xkb_keymap *keymap1, மாறிலி கட்டமைப்பு xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    மாறிலி darray_size_t t_max = MIN(keymap1->இலக்கம்_types, keymap2->இலக்கம்_types);
    க்கு (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    எனில் (keymap1->இலக்கம்_types != keymap2->இலக்கம்_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->இலக்கம்_types, keymap2->இலக்கம்_types);
        identical = false;
    }

    திரும்பு identical;
}

நிலையான bool
compare_groups(கட்டமைப்பு xkb_context *ctx,
               மாறிலி கட்டமைப்பு xkb_keymap *keymap1, மாறிலி கட்டமைப்பு xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               மாறிலி கட்டமைப்பு xkb_group *group1, மாறிலி கட்டமைப்பு xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    எனில் (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        திரும்பு false;
    }
    ப்ரதிஜ்ஞா(group1->type->இலக்கம்_levels == group2->type->இலக்கம்_levels);

    bool identical = true;
    க்கு (xkb_level_index_t l = 0; l < group1->type->இலக்கம்_levels; l++) {
        மாறிலி கட்டமைப்பு xkb_level * மாறிலி level1 = &group1->levels[l];
        மாறிலி கட்டமைப்பு xkb_level * மாறிலி level2 = &group2->levels[l];

        /* Keysyms */
        எனில் (level1->இலக்கம்_syms != level2->இலக்கம்_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->இலக்கம்_syms, level2->இலக்கம்_syms);
            identical = false;
        } இல்லையெனில் {
            எனில் (level1->இலக்கம்_syms > 1) {
                க்கு (xkb_keysym_மொத்தம்_t k = 0; k < level1->இலக்கம்_syms; k++) {
                    எனில் (level1->s.syms[k] == level2->s.syms[k])
                        தொடர்;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } இல்லையெனில் எனில் (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        எனில் (level1->இலக்கம்_actions != level2->இலக்கம்_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->இலக்கம்_actions, level2->இலக்கம்_actions);
            identical = false;
        } இல்லையெனில் {
            எனில் (level1->இலக்கம்_actions > 1) {
                க்கு (xkb_action_மொத்தம்_t a = 0; a < level1->இலக்கம்_actions; a++) {
                    எனில் (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        தொடர்;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } இல்லையெனில் எனில் (level1->இலக்கம்_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    திரும்பு identical;
}

நிலையான bool
keymap_compare_symbols(கட்டமைப்பு xkb_context *ctx,
                       மாறிலி கட்டமைப்பு xkb_keymap *keymap1,
                       மாறிலி கட்டமைப்பு xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    எனில் (keymap1->இலக்கம்_groups != keymap2->இலக்கம்_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->இலக்கம்_groups, keymap2->இலக்கம்_groups);
        identical = false;
    }

    எனில் (keymap1->இலக்கம்_group_names != keymap2->இலக்கம்_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->இலக்கம்_group_names, keymap2->இலக்கம்_group_names);
        identical = false;
    } இல்லையெனில் {
        க்கு (xkb_layout_index_t g = 0; g < keymap1->இலக்கம்_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            மாறிலி எழுத்து * மாறிலி name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            மாறிலி எழுத்து * மாறிலி name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            எனில் (!streq_null(name1, name2)) {
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
    மாறிலி xkb_keycode_t k_max = MIN(keymap1->இலக்கம்_சாவிகள், keymap2->இலக்கம்_சாவிகள்);
    க்கு (xkb_keycode_t k = 0; k < k_max; k++) {
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி key1 = &keymap1->சாவிகள்[k];
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி key2 = &keymap1->சாவிகள்[k];
        எனில் (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            தொடர்;
        }

        மாறிலி xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        எனில் (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        எனில் (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        எனில் (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        எனில் (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_எண் != key2->out_of_range_group_எண்) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_எண்,
                    key2->out_of_range_group_எண்);
            identical = false;
        }
        எனில் (key1->இலக்கம்_groups != key2->இலக்கம்_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->இலக்கம்_groups, key2->இலக்கம்_groups);
            identical = false;
        }
        மாறிலி xkb_layout_index_t g_max = MIN(key1->இலக்கம்_groups, key2->இலக்கம்_groups);
        க்கு (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    திரும்பு identical;
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
xkb_keymap_compare(கட்டமைப்பு xkb_context *ctx,
                   மாறிலி கட்டமைப்பு xkb_keymap *keymap1, மாறிலி கட்டமைப்பு xkb_keymap *keymap2,
                   எண்ணிக்கை xkb_keymap_compare_property properties)
{
    bool identical = true;

    எனில் (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    எனில் (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    எனில் (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    எனில் (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    எனில் (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    திரும்பு identical;
}
