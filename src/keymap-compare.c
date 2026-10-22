/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "src/messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "src/utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


ସ୍ଥିର bool
keymap_compare_mods(ସଂରଚନା xkb_context *ctx,
                    ଅଚଳ ସଂରଚନା xkb_keymap *keymap1,
                    ଅଚଳ ସଂରଚନା xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ଅଚଳ xkb_mod_index_t mod_max = MIN(keymap1->mods.ଅଙ୍କ_mods,
                                        keymap2->mods.ଅଙ୍କ_mods);
    ପାଇଁ (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        ଅଚଳ ସଂରଚନା xkb_mod * ଅଚଳ mod1 = &keymap1->mods.mods[mod];
        ଅଚଳ ସଂରଚନା xkb_mod * ଅଚଳ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        ଯଦି (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ଯଦି (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ଯଦି (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ଯଦି (keymap1->mods.ଅଙ୍କ_mods != keymap2->mods.ଅଙ୍କ_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.ଅଙ୍କ_mods, keymap2->mods.ଅଙ୍କ_mods);
        identical = false;
    }

    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
keymap_compare_keycodes(ସଂରଚନା xkb_context *ctx,
                        ଅଚଳ ସଂରଚନା xkb_keymap *keymap1,
                        ଅଚଳ ସଂରଚନା xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ଯଦି (keymap1->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ != keymap2->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, keymap2->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);
        identical = false;
    }
    ଯଦି (keymap1->min_ଚାବି_code != keymap2->min_ଚାବି_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ଚାବି_code, keymap2->min_ଚାବି_code);
        identical = false;
    }
    ଯଦି (keymap1->max_ଚାବି_code != keymap2->max_ଚାବି_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ, keymap2->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ);
        identical = false;
    }
    ଯଦି (keymap1->max_ଚାବି_code != keymap2->max_ଚାବି_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ଚାବି_code, keymap2->min_ଚାବି_code);
        identical = false;
    }

    /* Check common keys */
    ଅଚଳ xkb_keycode_t k_max = MIN(keymap1->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, keymap2->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);
    ପାଇଁ (xkb_keycode_t k = 0; k < k_max; k++) {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ key1 = &keymap1->ଚାବିଗୁଡ଼ିକ[k];
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ key2 = &keymap1->ଚାବିଗୁଡ଼ିକ[k];
        ଯଦି (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ଜାରି;
        }

        ଅଚଳ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx, key1->name);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx, key2->name);
        ଯଦି (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ଅଚଳ darray_size_t a_max = MIN(keymap1->ଅଙ୍କ_ଚାବି_aliases,
                                    keymap2->ଅଙ୍କ_ଚାବି_aliases);
    ପାଇଁ (darray_size_t a = 0; a < a_max; a++) {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ entry1 = &keymap1->ଚାବି_aliases[a];
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ entry2 = &keymap2->ଚାବି_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        ଯଦି (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ଅଚଳ ଅକ୍ଷର * ଅଚଳ real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        ଯଦି (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ଯଦି (keymap1->ଅଙ୍କ_ଚାବି_aliases != keymap2->ଅଙ୍କ_ଚାବି_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->ଅଙ୍କ_ଚାବି_aliases, keymap2->ଅଙ୍କ_ଚାବି_aliases);
        identical = false;
    }

    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
keymap_compare_leds(ସଂରଚନା xkb_context *ctx,
                    ଅଚଳ ସଂରଚନା xkb_keymap *keymap1,
                    ଅଚଳ ସଂରଚନା xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->ଅଙ୍କ_leds, keymap2->ଅଙ୍କ_leds);
    ପାଇଁ (xkb_led_index_t led = 0; led < led_max; led++) {
        ଅଚଳ ସଂରଚନା xkb_led * ଅଚଳ led1 = &keymap1->leds[led];
        ଅଚଳ ସଂରଚନା xkb_led * ଅଚଳ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx, led1->name);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx, led2->name);
        ଯଦି (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ଯଦି (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ଯଦି (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ଯଦି (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ଯଦି (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ଯଦି (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ଯଦି (keymap1->ଅଙ୍କ_leds != keymap2->ଅଙ୍କ_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ଅଙ୍କ_leds, keymap2->ଅଙ୍କ_leds);
        identical = false;
    }

    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
compare_types(ସଂରଚନା xkb_context *ctx,
              ଅଚଳ ସଂରଚନା xkb_keymap *keymap1, ଅଚଳ ସଂରଚନା xkb_keymap *keymap2,
              ଅଚଳ ସଂରଚନା xkb_ଚାବି_type *type1, ଅଚଳ ସଂରଚନା xkb_ଚାବି_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx, type1->name);
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx, type2->name);
    ଯଦି (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ଯଦି (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (type1->ଅଙ୍କ_levels != type2->ଅଙ୍କ_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ଅଙ୍କ_levels, type2->ଅଙ୍କ_levels);
        /* No point to check further properties */
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (type1->ଅଙ୍କ_level_names != type2->ଅଙ୍କ_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ଅଙ୍କ_level_names, type2->ଅଙ୍କ_level_names);
        identical = false;
    } ଅନ୍ୟଥା {
        ପାଇଁ (xkb_level_index_t l = 0; l < type1->ଅଙ୍କ_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            ଯଦି (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ଯଦି (type1->ଅଙ୍କ_entries != type2->ଅଙ୍କ_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->ଅଙ୍କ_entries, type2->ଅଙ୍କ_entries);
        identical = false;
    } ଅନ୍ୟଥା {
        ପାଇଁ (darray_size_t e = 0; e < type1->ଅଙ୍କ_entries; e++) {
            ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry * ଅଚଳ entry1 = &type1->entries[e];
            ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry * ଅଚଳ entry2 = &type2->entries[e];
            ଯଦି (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ଯଦି (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ଯଦି (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
keymap_compare_types(ସଂରଚନା xkb_context *ctx,
                     ଅଚଳ ସଂରଚନା xkb_keymap *keymap1, ଅଚଳ ସଂରଚନା xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ଅଚଳ darray_size_t t_max = MIN(keymap1->ଅଙ୍କ_types, keymap2->ଅଙ୍କ_types);
    ପାଇଁ (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ଯଦି (keymap1->ଅଙ୍କ_types != keymap2->ଅଙ୍କ_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->ଅଙ୍କ_types, keymap2->ଅଙ୍କ_types);
        identical = false;
    }

    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
compare_groups(ସଂରଚନା xkb_context *ctx,
               ଅଚଳ ସଂରଚନା xkb_keymap *keymap1, ଅଚଳ ସଂରଚନା xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               ଅଚଳ ସଂରଚନା xkb_group *group1, ଅଚଳ ସଂରଚନା xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ଯଦି (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ଫେରନ୍ତୁ false;
    }
    ପ୍ରତିଜ୍ଞା(group1->type->ଅଙ୍କ_levels == group2->type->ଅଙ୍କ_levels);

    bool identical = true;
    ପାଇଁ (xkb_level_index_t l = 0; l < group1->type->ଅଙ୍କ_levels; l++) {
        ଅଚଳ ସଂରଚନା xkb_level * ଅଚଳ level1 = &group1->levels[l];
        ଅଚଳ ସଂରଚନା xkb_level * ଅଚଳ level2 = &group2->levels[l];

        /* Keysyms */
        ଯଦି (level1->ଅଙ୍କ_syms != level2->ଅଙ୍କ_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->ଅଙ୍କ_syms, level2->ଅଙ୍କ_syms);
            identical = false;
        } ଅନ୍ୟଥା {
            ଯଦି (level1->ଅଙ୍କ_syms > 1) {
                ପାଇଁ (xkb_keysym_ଗଣନା_t k = 0; k < level1->ଅଙ୍କ_syms; k++) {
                    ଯଦି (level1->s.syms[k] == level2->s.syms[k])
                        ଜାରି;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ଅନ୍ୟଥା ଯଦି (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ଯଦି (level1->ଅଙ୍କ_actions != level2->ଅଙ୍କ_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->ଅଙ୍କ_actions, level2->ଅଙ୍କ_actions);
            identical = false;
        } ଅନ୍ୟଥା {
            ଯଦି (level1->ଅଙ୍କ_actions > 1) {
                ପାଇଁ (xkb_action_ଗଣନା_t a = 0; a < level1->ଅଙ୍କ_actions; a++) {
                    ଯଦି (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        ଜାରି;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ଅନ୍ୟଥା ଯଦି (level1->ଅଙ୍କ_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ଫେରନ୍ତୁ identical;
}

ସ୍ଥିର bool
keymap_compare_symbols(ସଂରଚନା xkb_context *ctx,
                       ଅଚଳ ସଂରଚନା xkb_keymap *keymap1,
                       ଅଚଳ ସଂରଚନା xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ଯଦି (keymap1->ଅଙ୍କ_groups != keymap2->ଅଙ୍କ_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ଅଙ୍କ_groups, keymap2->ଅଙ୍କ_groups);
        identical = false;
    }

    ଯଦି (keymap1->ଅଙ୍କ_group_names != keymap2->ଅଙ୍କ_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ଅଙ୍କ_group_names, keymap2->ଅଙ୍କ_group_names);
        identical = false;
    } ଅନ୍ୟଥା {
        ପାଇଁ (xkb_layout_index_t g = 0; g < keymap1->ଅଙ୍କ_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            ଯଦି (!streq_null(name1, name2)) {
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
    ଅଚଳ xkb_keycode_t k_max = MIN(keymap1->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, keymap2->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);
    ପାଇଁ (xkb_keycode_t k = 0; k < k_max; k++) {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ key1 = &keymap1->ଚାବିଗୁଡ଼ିକ[k];
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ key2 = &keymap1->ଚାବିଗୁଡ଼ିକ[k];
        ଯଦି (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ଜାରି;
        }

        ଅଚଳ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ଯଦି (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ଯଦି (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ଯଦି (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ଯଦି (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_ସଂଖ୍ୟା != key2->out_of_range_group_ସଂଖ୍ୟା) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_ସଂଖ୍ୟା,
                    key2->out_of_range_group_ସଂଖ୍ୟା);
            identical = false;
        }
        ଯଦି (key1->ଅଙ୍କ_groups != key2->ଅଙ୍କ_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->ଅଙ୍କ_groups, key2->ଅଙ୍କ_groups);
            identical = false;
        }
        ଅଚଳ xkb_layout_index_t g_max = MIN(key1->ଅଙ୍କ_groups, key2->ଅଙ୍କ_groups);
        ପାଇଁ (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ଫେରନ୍ତୁ identical;
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
xkb_keymap_compare(ସଂରଚନା xkb_context *ctx,
                   ଅଚଳ ସଂରଚନା xkb_keymap *keymap1, ଅଚଳ ସଂରଚନା xkb_keymap *keymap2,
                   ଗଣନା xkb_keymap_compare_property properties)
{
    bool identical = true;

    ଯଦି (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ଯଦି (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ଯଦି (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ଯଦି (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ଯଦି (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ଫେରନ୍ତୁ identical;
}
