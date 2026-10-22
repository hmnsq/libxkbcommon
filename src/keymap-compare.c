/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "src/messages-codes.h"
#અન્તર્ભાવ "src/utils.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


સ્થિર bool
keymap_compare_mods(સંરચના xkb_context *ctx,
                    અચલ સંરચના xkb_keymap *keymap1,
                    અચલ સંરચના xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    અચલ xkb_mod_index_t mod_max = MIN(keymap1->mods.અંક_mods,
                                        keymap2->mods.અંક_mods);
    માટે (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        અચલ સંરચના xkb_mod * અચલ mod1 = &keymap1->mods.mods[mod];
        અચલ સંરચના xkb_mod * અચલ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        જો (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        જો (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        જો (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    જો (keymap1->mods.અંક_mods != keymap2->mods.અંક_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.અંક_mods, keymap2->mods.અંક_mods);
        identical = false;
    }

    પાછા identical;
}

સ્થિર bool
keymap_compare_keycodes(સંરચના xkb_context *ctx,
                        અચલ સંરચના xkb_keymap *keymap1,
                        અચલ સંરચના xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    જો (keymap1->અંક_ચાવીઓ != keymap2->અંક_ચાવીઓ) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->અંક_ચાવીઓ, keymap2->અંક_ચાવીઓ);
        identical = false;
    }
    જો (keymap1->min_ચાવી_code != keymap2->min_ચાવી_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ચાવી_code, keymap2->min_ચાવી_code);
        identical = false;
    }
    જો (keymap1->max_ચાવી_code != keymap2->max_ચાવી_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->અંક_ચાવીઓ_નીચલું, keymap2->અંક_ચાવીઓ_નીચલું);
        identical = false;
    }
    જો (keymap1->max_ચાવી_code != keymap2->max_ચાવી_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ચાવી_code, keymap2->min_ચાવી_code);
        identical = false;
    }

    /* Check common keys */
    અચલ xkb_keycode_t k_max = MIN(keymap1->અંક_ચાવીઓ, keymap2->અંક_ચાવીઓ);
    માટે (xkb_keycode_t k = 0; k < k_max; k++) {
        અચલ સંરચના xkb_ચાવી * અચલ key1 = &keymap1->ચાવીઓ[k];
        અચલ સંરચના xkb_ચાવી * અચલ key2 = &keymap1->ચાવીઓ[k];
        જો (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ચાલુ;
        }

        અચલ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx, key1->name);
        અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx, key2->name);
        જો (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    અચલ darray_size_t a_max = MIN(keymap1->અંક_ચાવી_aliases,
                                    keymap2->અંક_ચાવી_aliases);
    માટે (darray_size_t a = 0; a < a_max; a++) {
        અચલ સંરચના xkb_ચાવી_alias * અચલ entry1 = &keymap1->ચાવી_aliases[a];
        અચલ સંરચના xkb_ચાવી_alias * અચલ entry2 = &keymap2->ચાવી_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        અચલ અક્ષર * અચલ alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        અચલ અક્ષર * અચલ alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        જો (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        અચલ અક્ષર * અચલ real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        અચલ અક્ષર * અચલ real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        જો (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    જો (keymap1->અંક_ચાવી_aliases != keymap2->અંક_ચાવી_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->અંક_ચાવી_aliases, keymap2->અંક_ચાવી_aliases);
        identical = false;
    }

    પાછા identical;
}

સ્થિર bool
keymap_compare_leds(સંરચના xkb_context *ctx,
                    અચલ સંરચના xkb_keymap *keymap1,
                    અચલ સંરચના xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->અંક_leds, keymap2->અંક_leds);
    માટે (xkb_led_index_t led = 0; led < led_max; led++) {
        અચલ સંરચના xkb_led * અચલ led1 = &keymap1->leds[led];
        અચલ સંરચના xkb_led * અચલ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx, led1->name);
        અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx, led2->name);
        જો (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        જો (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        જો (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        જો (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        જો (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        જો (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    જો (keymap1->અંક_leds != keymap2->અંક_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->અંક_leds, keymap2->અંક_leds);
        identical = false;
    }

    પાછા identical;
}

સ્થિર bool
compare_types(સંરચના xkb_context *ctx,
              અચલ સંરચના xkb_keymap *keymap1, અચલ સંરચના xkb_keymap *keymap2,
              અચલ સંરચના xkb_ચાવી_type *type1, અચલ સંરચના xkb_ચાવી_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx, type1->name);
    અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx, type2->name);
    જો (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    જો (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        પાછા false;
    }

    જો (type1->અંક_levels != type2->અંક_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->અંક_levels, type2->અંક_levels);
        /* No point to check further properties */
        પાછા false;
    }

    જો (type1->અંક_level_names != type2->અંક_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->અંક_level_names, type2->અંક_level_names);
        identical = false;
    } નહીંતર {
        માટે (xkb_level_index_t l = 0; l < type1->અંક_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            અચલ અક્ષર * અચલ lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            અચલ અક્ષર * અચલ lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            જો (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    જો (type1->અંક_entries != type2->અંક_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->અંક_entries, type2->અંક_entries);
        identical = false;
    } નહીંતર {
        માટે (darray_size_t e = 0; e < type1->અંક_entries; e++) {
            અચલ સંરચના xkb_ચાવી_type_entry * અચલ entry1 = &type1->entries[e];
            અચલ સંરચના xkb_ચાવી_type_entry * અચલ entry2 = &type2->entries[e];
            જો (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            જો (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            જો (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    પાછા identical;
}

સ્થિર bool
keymap_compare_types(સંરચના xkb_context *ctx,
                     અચલ સંરચના xkb_keymap *keymap1, અચલ સંરચના xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    અચલ darray_size_t t_max = MIN(keymap1->અંક_types, keymap2->અંક_types);
    માટે (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    જો (keymap1->અંક_types != keymap2->અંક_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->અંક_types, keymap2->અંક_types);
        identical = false;
    }

    પાછા identical;
}

સ્થિર bool
compare_groups(સંરચના xkb_context *ctx,
               અચલ સંરચના xkb_keymap *keymap1, અચલ સંરચના xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               અચલ સંરચના xkb_group *group1, અચલ સંરચના xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    જો (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        પાછા false;
    }
    પ્રતિજ્ઞા(group1->type->અંક_levels == group2->type->અંક_levels);

    bool identical = true;
    માટે (xkb_level_index_t l = 0; l < group1->type->અંક_levels; l++) {
        અચલ સંરચના xkb_level * અચલ level1 = &group1->levels[l];
        અચલ સંરચના xkb_level * અચલ level2 = &group2->levels[l];

        /* Keysyms */
        જો (level1->અંક_syms != level2->અંક_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->અંક_syms, level2->અંક_syms);
            identical = false;
        } નહીંતર {
            જો (level1->અંક_syms > 1) {
                માટે (xkb_keysym_ગણતરી_t k = 0; k < level1->અંક_syms; k++) {
                    જો (level1->s.syms[k] == level2->s.syms[k])
                        ચાલુ;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } નહીંતર જો (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        જો (level1->અંક_actions != level2->અંક_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->અંક_actions, level2->અંક_actions);
            identical = false;
        } નહીંતર {
            જો (level1->અંક_actions > 1) {
                માટે (xkb_action_ગણતરી_t a = 0; a < level1->અંક_actions; a++) {
                    જો (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        ચાલુ;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } નહીંતર જો (level1->અંક_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    પાછા identical;
}

સ્થિર bool
keymap_compare_symbols(સંરચના xkb_context *ctx,
                       અચલ સંરચના xkb_keymap *keymap1,
                       અચલ સંરચના xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    જો (keymap1->અંક_groups != keymap2->અંક_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->અંક_groups, keymap2->અંક_groups);
        identical = false;
    }

    જો (keymap1->અંક_group_names != keymap2->અંક_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->અંક_group_names, keymap2->અંક_group_names);
        identical = false;
    } નહીંતર {
        માટે (xkb_layout_index_t g = 0; g < keymap1->અંક_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            અચલ અક્ષર * અચલ name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            અચલ અક્ષર * અચલ name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            જો (!streq_null(name1, name2)) {
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
    અચલ xkb_keycode_t k_max = MIN(keymap1->અંક_ચાવીઓ, keymap2->અંક_ચાવીઓ);
    માટે (xkb_keycode_t k = 0; k < k_max; k++) {
        અચલ સંરચના xkb_ચાવી * અચલ key1 = &keymap1->ચાવીઓ[k];
        અચલ સંરચના xkb_ચાવી * અચલ key2 = &keymap1->ચાવીઓ[k];
        જો (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ચાલુ;
        }

        અચલ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        જો (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        જો (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        જો (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        જો (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_સંખ્યા != key2->out_of_range_group_સંખ્યા) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_સંખ્યા,
                    key2->out_of_range_group_સંખ્યા);
            identical = false;
        }
        જો (key1->અંક_groups != key2->અંક_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->અંક_groups, key2->અંક_groups);
            identical = false;
        }
        અચલ xkb_layout_index_t g_max = MIN(key1->અંક_groups, key2->અંક_groups);
        માટે (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    પાછા identical;
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
xkb_keymap_compare(સંરચના xkb_context *ctx,
                   અચલ સંરચના xkb_keymap *keymap1, અચલ સંરચના xkb_keymap *keymap2,
                   ગણના xkb_keymap_compare_property properties)
{
    bool identical = true;

    જો (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    જો (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    જો (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    જો (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    જો (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    પાછા identical;
}
