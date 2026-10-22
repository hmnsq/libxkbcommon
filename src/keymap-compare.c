/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>

#include "keymap.h"
#include "src/messages-codes.h"
#include "src/utils.h"
#include "xkbcommon/xkbcommon.h"
#include "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


static bool
keymap_compare_mods(sa_nracanaa xkb_context *ctx,
                    const sa_nracanaa xkb_keymap *keymap1,
                    const sa_nracanaa xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    const xkb_mod_index_t mod_max = MIN(keymap1->mods.num_mods,
                                        keymap2->mods.num_mods);
    for (xkb_mod_index_t mod = 0; mod < mod_max; mod++) {
        const sa_nracanaa xkb_mod * const mod1 = &keymap1->mods.mods[mod];
        const sa_nracanaa xkb_mod * const mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        const var_na * const name1 = xkb_atom_text(keymap1->ctx, mod1->name);
        const var_na * const name2 = xkb_atom_text(keymap2->ctx, mod2->name);
        ᮚᮦᮔ᮪ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ᮚᮦᮔ᮪ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ᮚᮦᮔ᮪ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ᮚᮦᮔ᮪ (keymap1->mods.num_mods != keymap2->mods.num_mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.num_mods, keymap2->mods.num_mods);
        identical = false;
    }

    ᮝᮜᮤ identical;
}

static bool
keymap_compare_keycodes(sa_nracanaa xkb_context *ctx,
                        const sa_nracanaa xkb_keymap *keymap1,
                        const sa_nracanaa xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ᮚᮦᮔ᮪ (keymap1->num_keys != keymap2->num_keys) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->num_keys, keymap2->num_keys);
        identical = false;
    }
    ᮚᮦᮔ᮪ (keymap1->min_key_code != keymap2->min_key_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_key_code, keymap2->min_key_code);
        identical = false;
    }
    ᮚᮦᮔ᮪ (keymap1->max_key_code != keymap2->max_key_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->num_keys_low, keymap2->num_keys_low);
        identical = false;
    }
    ᮚᮦᮔ᮪ (keymap1->max_key_code != keymap2->max_key_code) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_key_code, keymap2->min_key_code);
        identical = false;
    }

    /* Check common keys */
    const xkb_keycode_t k_max = MIN(keymap1->num_keys, keymap2->num_keys);
    for (xkb_keycode_t k = 0; k < k_max; k++) {
        const sa_nracanaa xkb_key * const key1 = &keymap1->keys[k];
        const sa_nracanaa xkb_key * const key2 = &keymap1->keys[k];
        ᮚᮦᮔ᮪ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            continue;
        }

        const xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        const var_na * const name1 = xkb_atom_text(keymap1->ctx, key1->name);
        const var_na * const name2 = xkb_atom_text(keymap2->ctx, key2->name);
        ᮚᮦᮔ᮪ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    const darray_size_t a_max = MIN(keymap1->num_key_aliases,
                                    keymap2->num_key_aliases);
    for (darray_size_t a = 0; a < a_max; a++) {
        const sa_nracanaa xkb_key_alias * const entry1 = &keymap1->key_aliases[a];
        const sa_nracanaa xkb_key_alias * const entry2 = &keymap2->key_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        const var_na * const alias1 = xkb_atom_text(keymap1->ctx, entry1->alias);
        const var_na * const alias2 = xkb_atom_text(keymap2->ctx, entry2->alias);
        ᮚᮦᮔ᮪ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        const var_na * const real1 = xkb_atom_text(keymap1->ctx, entry1->real);
        const var_na * const real2 = xkb_atom_text(keymap2->ctx, entry2->real);
        ᮚᮦᮔ᮪ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ᮚᮦᮔ᮪ (keymap1->num_key_aliases != keymap2->num_key_aliases) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->num_key_aliases, keymap2->num_key_aliases);
        identical = false;
    }

    ᮝᮜᮤ identical;
}

static bool
keymap_compare_leds(sa_nracanaa xkb_context *ctx,
                    const sa_nracanaa xkb_keymap *keymap1,
                    const sa_nracanaa xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_index_t led_max = MIN(keymap1->num_leds, keymap2->num_leds);
    for (xkb_led_index_t led = 0; led < led_max; led++) {
        const sa_nracanaa xkb_led * const led1 = &keymap1->leds[led];
        const sa_nracanaa xkb_led * const led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        const var_na * const name1 = xkb_atom_text(keymap1->ctx, led1->name);
        const var_na * const name2 = xkb_atom_text(keymap2->ctx, led2->name);
        ᮚᮦᮔ᮪ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ᮚᮦᮔ᮪ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ᮚᮦᮔ᮪ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ᮚᮦᮔ᮪ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ᮚᮦᮔ᮪ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ᮚᮦᮔ᮪ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ᮚᮦᮔ᮪ (keymap1->num_leds != keymap2->num_leds) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->num_leds, keymap2->num_leds);
        identical = false;
    }

    ᮝᮜᮤ identical;
}

static bool
compare_types(sa_nracanaa xkb_context *ctx,
              const sa_nracanaa xkb_keymap *keymap1, const sa_nracanaa xkb_keymap *keymap2,
              const sa_nracanaa xkb_key_type *type1, const sa_nracanaa xkb_key_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    const var_na * const name1 = xkb_atom_text(keymap1->ctx, type1->name);
    const var_na * const name2 = xkb_atom_text(keymap2->ctx, type2->name);
    ᮚᮦᮔ᮪ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ᮚᮦᮔ᮪ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ᮝᮜᮤ false;
    }

    ᮚᮦᮔ᮪ (type1->num_levels != type2->num_levels) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->num_levels, type2->num_levels);
        /* No point to check further properties */
        ᮝᮜᮤ false;
    }

    ᮚᮦᮔ᮪ (type1->num_level_names != type2->num_level_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->num_level_names, type2->num_level_names);
        identical = false;
    } ᮜᮤᮚ {
        for (xkb_level_index_t l = 0; l < type1->num_level_names; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            const var_na * const lname1 = xkb_atom_text(keymap1->ctx,
                                                      type1->level_names[l]);
            const var_na * const lname2 = xkb_atom_text(keymap2->ctx,
                                                      type2->level_names[l]);
            ᮚᮦᮔ᮪ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ᮚᮦᮔ᮪ (type1->num_entries != type2->num_entries) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->num_entries, type2->num_entries);
        identical = false;
    } ᮜᮤᮚ {
        for (darray_size_t e = 0; e < type1->num_entries; e++) {
            const sa_nracanaa xkb_key_type_entry * const entry1 = &type1->entries[e];
            const sa_nracanaa xkb_key_type_entry * const entry2 = &type2->entries[e];
            ᮚᮦᮔ᮪ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ᮚᮦᮔ᮪ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ᮚᮦᮔ᮪ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ᮝᮜᮤ identical;
}

static bool
keymap_compare_types(sa_nracanaa xkb_context *ctx,
                     const sa_nracanaa xkb_keymap *keymap1, const sa_nracanaa xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    const darray_size_t t_max = MIN(keymap1->num_types, keymap2->num_types);
    for (darray_size_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ᮚᮦᮔ᮪ (keymap1->num_types != keymap2->num_types) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->num_types, keymap2->num_types);
        identical = false;
    }

    ᮝᮜᮤ identical;
}

static bool
compare_groups(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa xkb_keymap *keymap1, const sa_nracanaa xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_index_t g,
               const sa_nracanaa xkb_group *group1, const sa_nracanaa xkb_group *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ᮚᮦᮔ᮪ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        const var_na * const name1 = xkb_atom_text(keymap1->ctx,
                                                 group1->type->name);
        const var_na * const name2 = xkb_atom_text(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ᮝᮜᮤ false;
    }
    assert(group1->type->num_levels == group2->type->num_levels);

    bool identical = true;
    for (xkb_level_index_t l = 0; l < group1->type->num_levels; l++) {
        const sa_nracanaa xkb_level * const level1 = &group1->levels[l];
        const sa_nracanaa xkb_level * const level2 = &group2->levels[l];

        /* Keysyms */
        ᮚᮦᮔ᮪ (level1->num_syms != level2->num_syms) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->num_syms, level2->num_syms);
            identical = false;
        } ᮜᮤᮚ {
            ᮚᮦᮔ᮪ (level1->num_syms > 1) {
                for (xkb_keysym_count_t k = 0; k < level1->num_syms; k++) {
                    ᮚᮦᮔ᮪ (level1->s.syms[k] == level2->s.syms[k])
                        continue;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ᮜᮤᮚ ᮚᮦᮔ᮪ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ᮚᮦᮔ᮪ (level1->num_actions != level2->num_actions) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->num_actions, level2->num_actions);
            identical = false;
        } ᮜᮤᮚ {
            ᮚᮦᮔ᮪ (level1->num_actions > 1) {
                for (xkb_action_count_t a = 0; a < level1->num_actions; a++) {
                    ᮚᮦᮔ᮪ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        continue;
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ᮜᮤᮚ ᮚᮦᮔ᮪ (level1->num_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ᮝᮜᮤ identical;
}

static bool
keymap_compare_symbols(sa_nracanaa xkb_context *ctx,
                       const sa_nracanaa xkb_keymap *keymap1,
                       const sa_nracanaa xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ᮚᮦᮔ᮪ (keymap1->num_groups != keymap2->num_groups) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->num_groups, keymap2->num_groups);
        identical = false;
    }

    ᮚᮦᮔ᮪ (keymap1->num_group_names != keymap2->num_group_names) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->num_group_names, keymap2->num_group_names);
        identical = false;
    } ᮜᮤᮚ {
        for (xkb_layout_index_t g = 0; g < keymap1->num_group_names; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            const var_na * const name1 = xkb_atom_text(keymap1->ctx,
                                                     keymap1->group_names[g]);
            const var_na * const name2 = xkb_atom_text(keymap2->ctx,
                                                     keymap2->group_names[g]);
            ᮚᮦᮔ᮪ (!streq_null(name1, name2)) {
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
    const xkb_keycode_t k_max = MIN(keymap1->num_keys, keymap2->num_keys);
    for (xkb_keycode_t k = 0; k < k_max; k++) {
        const sa_nracanaa xkb_key * const key1 = &keymap1->keys[k];
        const sa_nracanaa xkb_key * const key2 = &keymap1->keys[k];
        ᮚᮦᮔ᮪ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            continue;
        }

        const xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ᮚᮦᮔ᮪ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ᮚᮦᮔ᮪ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ᮚᮦᮔ᮪ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ᮚᮦᮔ᮪ (key1->out_of_range_group_policy != key2->out_of_range_group_policy ||
            key1->out_of_range_group_number != key2->out_of_range_group_number) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->out_of_range_group_policy,
                    key2->out_of_range_group_policy,
                    key1->out_of_range_group_number,
                    key2->out_of_range_group_number);
            identical = false;
        }
        ᮚᮦᮔ᮪ (key1->num_groups != key2->num_groups) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->num_groups, key2->num_groups);
            identical = false;
        }
        const xkb_layout_index_t g_max = MIN(key1->num_groups, key2->num_groups);
        for (xkb_layout_index_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ᮝᮜᮤ identical;
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
xkb_keymap_compare(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa xkb_keymap *keymap1, const sa_nracanaa xkb_keymap *keymap2,
                   ga_nanaa xkb_keymap_compare_property properties)
{
    bool identical = true;

    ᮚᮦᮔ᮪ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ᮚᮦᮔ᮪ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ᮚᮦᮔ᮪ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ᮚᮦᮔ᮪ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ᮚᮦᮔ᮪ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ᮝᮜᮤ identical;
}
