/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "keymap.h"
#అన్తర్భావ "src/messages-codes.h"
#అన్తర్భావ "src/utils.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


స్థిర bool
keymap_compare_mods(నిర్మాణం xkb_context *ctx,
                    స్థిరరాశి నిర్మాణం xkb_keymap *keymap1,
                    స్థిరరాశి నిర్మాణం xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    స్థిరరాశి xkb_mod_సూచీ_t mod_max = MIN(keymap1->mods.అంకె_mods,
                                        keymap2->mods.అంకె_mods);
    కొరకు (xkb_mod_సూచీ_t mod = 0; mod < mod_max; mod++) {
        స్థిరరాశి నిర్మాణం xkb_mod * స్థిరరాశి mod1 = &keymap1->mods.mods[mod];
        స్థిరరాశి నిర్మాణం xkb_mod * స్థిరరాశి mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx, mod1->name);
        స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx, mod2->name);
        ఒకవేళ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ఒకవేళ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ఒకవేళ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ఒకవేళ (keymap1->mods.అంకె_mods != keymap2->mods.అంకె_mods) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.అంకె_mods, keymap2->mods.అంకె_mods);
        identical = false;
    }

    తిరిగి identical;
}

స్థిర bool
keymap_compare_keycodes(నిర్మాణం xkb_context *ctx,
                        స్థిరరాశి నిర్మాణం xkb_keymap *keymap1,
                        స్థిరరాశి నిర్మాణం xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ఒకవేళ (keymap1->అంకె_కీలకాలు != keymap2->అంకె_కీలకాలు) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->అంకె_కీలకాలు, keymap2->అంకె_కీలకాలు);
        identical = false;
    }
    ఒకవేళ (keymap1->min_కీలకం_సంకేతం != keymap2->min_కీలకం_సంకేతం) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_కీలకం_సంకేతం, keymap2->min_కీలకం_సంకేతం);
        identical = false;
    }
    ఒకవేళ (keymap1->max_కీలకం_సంకేతం != keymap2->max_కీలకం_సంకేతం) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->అంకె_కీలకాలు_దిగువ, keymap2->అంకె_కీలకాలు_దిగువ);
        identical = false;
    }
    ఒకవేళ (keymap1->max_కీలకం_సంకేతం != keymap2->max_కీలకం_సంకేతం) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_కీలకం_సంకేతం, keymap2->min_కీలకం_సంకేతం);
        identical = false;
    }

    /* Check common keys */
    స్థిరరాశి xkb_keycode_t k_max = MIN(keymap1->అంకె_కీలకాలు, keymap2->అంకె_కీలకాలు);
    కొరకు (xkb_keycode_t k = 0; k < k_max; k++) {
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి key1 = &keymap1->కీలకాలు[k];
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి key2 = &keymap1->కీలకాలు[k];
        ఒకవేళ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            కొనసాగు;
        }

        స్థిరరాశి xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx, key1->name);
        స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx, key2->name);
        ఒకవేళ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    స్థిరరాశి darray_కొలత_t a_max = MIN(keymap1->అంకె_కీలకం_aliases,
                                    keymap2->అంకె_కీలకం_aliases);
    కొరకు (darray_కొలత_t a = 0; a < a_max; a++) {
        స్థిరరాశి నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి entry1 = &keymap1->కీలకం_aliases[a];
        స్థిరరాశి నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి entry2 = &keymap2->కీలకం_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        స్థిరరాశి అక్షరం * స్థిరరాశి alias1 = xkb_atom_పాఠ్యం(keymap1->ctx, entry1->alias);
        స్థిరరాశి అక్షరం * స్థిరరాశి alias2 = xkb_atom_పాఠ్యం(keymap2->ctx, entry2->alias);
        ఒకవేళ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        స్థిరరాశి అక్షరం * స్థిరరాశి real1 = xkb_atom_పాఠ్యం(keymap1->ctx, entry1->real);
        స్థిరరాశి అక్షరం * స్థిరరాశి real2 = xkb_atom_పాఠ్యం(keymap2->ctx, entry2->real);
        ఒకవేళ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ఒకవేళ (keymap1->అంకె_కీలకం_aliases != keymap2->అంకె_కీలకం_aliases) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->అంకె_కీలకం_aliases, keymap2->అంకె_కీలకం_aliases);
        identical = false;
    }

    తిరిగి identical;
}

స్థిర bool
keymap_compare_leds(నిర్మాణం xkb_context *ctx,
                    స్థిరరాశి నిర్మాణం xkb_keymap *keymap1,
                    స్థిరరాశి నిర్మాణం xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_సూచీ_t led_max = MIN(keymap1->అంకె_leds, keymap2->అంకె_leds);
    కొరకు (xkb_led_సూచీ_t led = 0; led < led_max; led++) {
        స్థిరరాశి నిర్మాణం xkb_led * స్థిరరాశి led1 = &keymap1->leds[led];
        స్థిరరాశి నిర్మాణం xkb_led * స్థిరరాశి led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx, led1->name);
        స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx, led2->name);
        ఒకవేళ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ఒకవేళ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ఒకవేళ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ఒకవేళ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ఒకవేళ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ఒకవేళ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ఒకవేళ (keymap1->అంకె_leds != keymap2->అంకె_leds) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->అంకె_leds, keymap2->అంకె_leds);
        identical = false;
    }

    తిరిగి identical;
}

స్థిర bool
compare_types(నిర్మాణం xkb_context *ctx,
              స్థిరరాశి నిర్మాణం xkb_keymap *keymap1, స్థిరరాశి నిర్మాణం xkb_keymap *keymap2,
              స్థిరరాశి నిర్మాణం xkb_కీలకం_type *type1, స్థిరరాశి నిర్మాణం xkb_కీలకం_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx, type1->name);
    స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx, type2->name);
    ఒకవేళ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ఒకవేళ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        తిరిగి false;
    }

    ఒకవేళ (type1->అంకె_స్థాయిలు != type2->అంకె_స్థాయిలు) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->అంకె_స్థాయిలు, type2->అంకె_స్థాయిలు);
        /* No point to check further properties */
        తిరిగి false;
    }

    ఒకవేళ (type1->అంకె_level_పేర్లు != type2->అంకె_level_పేర్లు) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->అంకె_level_పేర్లు, type2->అంకె_level_పేర్లు);
        identical = false;
    } లేకపోతే {
        కొరకు (xkb_level_సూచీ_t l = 0; l < type1->అంకె_level_పేర్లు; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            స్థిరరాశి అక్షరం * స్థిరరాశి lname1 = xkb_atom_పాఠ్యం(keymap1->ctx,
                                                      type1->level_పేర్లు[l]);
            స్థిరరాశి అక్షరం * స్థిరరాశి lname2 = xkb_atom_పాఠ్యం(keymap2->ctx,
                                                      type2->level_పేర్లు[l]);
            ఒకవేళ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ఒకవేళ (type1->అంకె_entries != type2->అంకె_entries) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->అంకె_entries, type2->అంకె_entries);
        identical = false;
    } లేకపోతే {
        కొరకు (darray_కొలత_t e = 0; e < type1->అంకె_entries; e++) {
            స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు * స్థిరరాశి entry1 = &type1->entries[e];
            స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు * స్థిరరాశి entry2 = &type2->entries[e];
            ఒకవేళ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ఒకవేళ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ఒకవేళ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    తిరిగి identical;
}

స్థిర bool
keymap_compare_types(నిర్మాణం xkb_context *ctx,
                     స్థిరరాశి నిర్మాణం xkb_keymap *keymap1, స్థిరరాశి నిర్మాణం xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    స్థిరరాశి darray_కొలత_t t_max = MIN(keymap1->అంకె_types, keymap2->అంకె_types);
    కొరకు (darray_కొలత_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ఒకవేళ (keymap1->అంకె_types != keymap2->అంకె_types) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->అంకె_types, keymap2->అంకె_types);
        identical = false;
    }

    తిరిగి identical;
}

స్థిర bool
compare_groups(నిర్మాణం xkb_context *ctx,
               స్థిరరాశి నిర్మాణం xkb_keymap *keymap1, స్థిరరాశి నిర్మాణం xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_సూచీ_t g,
               స్థిరరాశి నిర్మాణం xkb_సమూహం *group1, స్థిరరాశి నిర్మాణం xkb_సమూహం *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ఒకవేళ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx,
                                                 group1->type->name);
        స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        తిరిగి false;
    }
    ప్రతిజ్ఞా(group1->type->అంకె_స్థాయిలు == group2->type->అంకె_స్థాయిలు);

    bool identical = true;
    కొరకు (xkb_level_సూచీ_t l = 0; l < group1->type->అంకె_స్థాయిలు; l++) {
        స్థిరరాశి నిర్మాణం xkb_level * స్థిరరాశి level1 = &group1->స్థాయిలు[l];
        స్థిరరాశి నిర్మాణం xkb_level * స్థిరరాశి level2 = &group2->స్థాయిలు[l];

        /* Keysyms */
        ఒకవేళ (level1->అంకె_syms != level2->అంకె_syms) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->అంకె_syms, level2->అంకె_syms);
            identical = false;
        } లేకపోతే {
            ఒకవేళ (level1->అంకె_syms > 1) {
                కొరకు (xkb_keysym_లెక్క_t k = 0; k < level1->అంకె_syms; k++) {
                    ఒకవేళ (level1->s.syms[k] == level2->s.syms[k])
                        కొనసాగు;
                    log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } లేకపోతే ఒకవేళ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ఒకవేళ (level1->అంకె_actions != level2->అంకె_actions) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->అంకె_actions, level2->అంకె_actions);
            identical = false;
        } లేకపోతే {
            ఒకవేళ (level1->అంకె_actions > 1) {
                కొరకు (xkb_action_లెక్క_t a = 0; a < level1->అంకె_actions; a++) {
                    ఒకవేళ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        కొనసాగు;
                    log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } లేకపోతే ఒకవేళ (level1->అంకె_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    తిరిగి identical;
}

స్థిర bool
keymap_compare_symbols(నిర్మాణం xkb_context *ctx,
                       స్థిరరాశి నిర్మాణం xkb_keymap *keymap1,
                       స్థిరరాశి నిర్మాణం xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ఒకవేళ (keymap1->అంకె_groups != keymap2->అంకె_groups) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->అంకె_groups, keymap2->అంకె_groups);
        identical = false;
    }

    ఒకవేళ (keymap1->అంకె_సమూహం_పేర్లు != keymap2->అంకె_సమూహం_పేర్లు) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->అంకె_సమూహం_పేర్లు, keymap2->అంకె_సమూహం_పేర్లు);
        identical = false;
    } లేకపోతే {
        కొరకు (xkb_layout_సూచీ_t g = 0; g < keymap1->అంకె_సమూహం_పేర్లు; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            స్థిరరాశి అక్షరం * స్థిరరాశి name1 = xkb_atom_పాఠ్యం(keymap1->ctx,
                                                     keymap1->సమూహం_పేర్లు[g]);
            స్థిరరాశి అక్షరం * స్థిరరాశి name2 = xkb_atom_పాఠ్యం(keymap2->ctx,
                                                     keymap2->సమూహం_పేర్లు[g]);
            ఒకవేళ (!streq_null(name1, name2)) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Group #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n", g, name1, name2);
                identical = false;
            }
        }
    }

    /*
     * Check common keycodes
     */
    స్థిరరాశి xkb_keycode_t k_max = MIN(keymap1->అంకె_కీలకాలు, keymap2->అంకె_కీలకాలు);
    కొరకు (xkb_keycode_t k = 0; k < k_max; k++) {
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి key1 = &keymap1->కీలకాలు[k];
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి key2 = &keymap1->కీలకాలు[k];
        ఒకవేళ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            కొనసాగు;
        }

        స్థిరరాశి xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ఒకవేళ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ఒకవేళ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ఒకవేళ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ఒకవేళ (key1->బహిర్గతం_యొక్క_range_సమూహం_policy != key2->బహిర్గతం_యొక్క_range_సమూహం_policy ||
            key1->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య != key2->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->బహిర్గతం_యొక్క_range_సమూహం_policy,
                    key2->బహిర్గతం_యొక్క_range_సమూహం_policy,
                    key1->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య,
                    key2->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య);
            identical = false;
        }
        ఒకవేళ (key1->అంకె_groups != key2->అంకె_groups) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->అంకె_groups, key2->అంకె_groups);
            identical = false;
        }
        స్థిరరాశి xkb_layout_సూచీ_t g_max = MIN(key1->అంకె_groups, key2->అంకె_groups);
        కొరకు (xkb_layout_సూచీ_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    తిరిగి identical;
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
xkb_keymap_compare(నిర్మాణం xkb_context *ctx,
                   స్థిరరాశి నిర్మాణం xkb_keymap *keymap1, స్థిరరాశి నిర్మాణం xkb_keymap *keymap2,
                   గణన xkb_keymap_compare_property properties)
{
    bool identical = true;

    ఒకవేళ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ఒకవేళ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ఒకవేళ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ఒకవేళ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ఒకవేళ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    తిరిగి identical;
}
