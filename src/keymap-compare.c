/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "src/messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "src/utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


ਸਥਿਰ bool
keymap_compare_mods(ਬਣਤਰ xkb_context *ctx,
                    ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1,
                    ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t mod_max = MIN(keymap1->mods.ਅੰਕ_mods,
                                        keymap2->mods.ਅੰਕ_mods);
    ਲਈ (xkb_mod_ਸੂਚਕਾਂਕ_t mod = 0; mod < mod_max; mod++) {
        ਅਚਲ ਬਣਤਰ xkb_mod * ਅਚਲ mod1 = &keymap1->mods.mods[mod];
        ਅਚਲ ਬਣਤਰ xkb_mod * ਅਚਲ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx, mod1->name);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx, mod2->name);
        ਜੇ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ਜੇ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ਜੇ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ਜੇ (keymap1->mods.ਅੰਕ_mods != keymap2->mods.ਅੰਕ_mods) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.ਅੰਕ_mods, keymap2->mods.ਅੰਕ_mods);
        identical = false;
    }

    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
keymap_compare_keycodes(ਬਣਤਰ xkb_context *ctx,
                        ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1,
                        ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ਜੇ (keymap1->ਅੰਕ_ਕੁੰਜੀਆਂ != keymap2->ਅੰਕ_ਕੁੰਜੀਆਂ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ਅੰਕ_ਕੁੰਜੀਆਂ, keymap2->ਅੰਕ_ਕੁੰਜੀਆਂ);
        identical = false;
    }
    ਜੇ (keymap1->min_ਕੁੰਜੀ_ਕੋਡ != keymap2->min_ਕੁੰਜੀ_ਕੋਡ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ਕੁੰਜੀ_ਕੋਡ, keymap2->min_ਕੁੰਜੀ_ਕੋਡ);
        identical = false;
    }
    ਜੇ (keymap1->max_ਕੁੰਜੀ_ਕੋਡ != keymap2->max_ਕੁੰਜੀ_ਕੋਡ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ, keymap2->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ);
        identical = false;
    }
    ਜੇ (keymap1->max_ਕੁੰਜੀ_ਕੋਡ != keymap2->max_ਕੁੰਜੀ_ਕੋਡ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ਕੁੰਜੀ_ਕੋਡ, keymap2->min_ਕੁੰਜੀ_ਕੋਡ);
        identical = false;
    }

    /* Check common keys */
    ਅਚਲ xkb_keycode_t k_max = MIN(keymap1->ਅੰਕ_ਕੁੰਜੀਆਂ, keymap2->ਅੰਕ_ਕੁੰਜੀਆਂ);
    ਲਈ (xkb_keycode_t k = 0; k < k_max; k++) {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ key1 = &keymap1->ਕੁੰਜੀਆਂ[k];
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ key2 = &keymap1->ਕੁੰਜੀਆਂ[k];
        ਜੇ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ਜਾਰੀ;
        }

        ਅਚਲ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx, key1->name);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx, key2->name);
        ਜੇ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ਅਚਲ darray_ਮਾਪ_t a_max = MIN(keymap1->ਅੰਕ_ਕੁੰਜੀ_aliases,
                                    keymap2->ਅੰਕ_ਕੁੰਜੀ_aliases);
    ਲਈ (darray_ਮਾਪ_t a = 0; a < a_max; a++) {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ entry1 = &keymap1->ਕੁੰਜੀ_aliases[a];
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ entry2 = &keymap2->ਕੁੰਜੀ_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ਅਚਲ ਅੱਖਰ * ਅਚਲ alias1 = xkb_atom_ਪਾਠ(keymap1->ctx, entry1->alias);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ alias2 = xkb_atom_ਪਾਠ(keymap2->ctx, entry2->alias);
        ਜੇ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ਅਚਲ ਅੱਖਰ * ਅਚਲ real1 = xkb_atom_ਪਾਠ(keymap1->ctx, entry1->real);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ real2 = xkb_atom_ਪਾਠ(keymap2->ctx, entry2->real);
        ਜੇ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ਜੇ (keymap1->ਅੰਕ_ਕੁੰਜੀ_aliases != keymap2->ਅੰਕ_ਕੁੰਜੀ_aliases) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->ਅੰਕ_ਕੁੰਜੀ_aliases, keymap2->ਅੰਕ_ਕੁੰਜੀ_aliases);
        identical = false;
    }

    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
keymap_compare_leds(ਬਣਤਰ xkb_context *ctx,
                    ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1,
                    ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_ਸੂਚਕਾਂਕ_t led_max = MIN(keymap1->ਅੰਕ_leds, keymap2->ਅੰਕ_leds);
    ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t led = 0; led < led_max; led++) {
        ਅਚਲ ਬਣਤਰ xkb_led * ਅਚਲ led1 = &keymap1->leds[led];
        ਅਚਲ ਬਣਤਰ xkb_led * ਅਚਲ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx, led1->name);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx, led2->name);
        ਜੇ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ਜੇ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ਜੇ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ਜੇ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ਜੇ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ਜੇ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ਜੇ (keymap1->ਅੰਕ_leds != keymap2->ਅੰਕ_leds) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ਅੰਕ_leds, keymap2->ਅੰਕ_leds);
        identical = false;
    }

    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
compare_types(ਬਣਤਰ xkb_context *ctx,
              ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1, ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2,
              ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type1, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx, type1->name);
    ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx, type2->name);
    ਜੇ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ਜੇ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ਵਾਪਸ false;
    }

    ਜੇ (type1->ਅੰਕ_ਪੱਧਰ != type2->ਅੰਕ_ਪੱਧਰ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ਅੰਕ_ਪੱਧਰ, type2->ਅੰਕ_ਪੱਧਰ);
        /* No point to check further properties */
        ਵਾਪਸ false;
    }

    ਜੇ (type1->ਅੰਕ_level_ਨਾਂ != type2->ਅੰਕ_level_ਨਾਂ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ਅੰਕ_level_ਨਾਂ, type2->ਅੰਕ_level_ਨਾਂ);
        identical = false;
    } ਨਹੀਂਤਾਂ {
        ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t l = 0; l < type1->ਅੰਕ_level_ਨਾਂ; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ਅਚਲ ਅੱਖਰ * ਅਚਲ lname1 = xkb_atom_ਪਾਠ(keymap1->ctx,
                                                      type1->level_ਨਾਂ[l]);
            ਅਚਲ ਅੱਖਰ * ਅਚਲ lname2 = xkb_atom_ਪਾਠ(keymap2->ctx,
                                                      type2->level_ਨਾਂ[l]);
            ਜੇ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ਜੇ (type1->ਅੰਕ_entries != type2->ਅੰਕ_entries) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->ਅੰਕ_entries, type2->ਅੰਕ_entries);
        identical = false;
    } ਨਹੀਂਤਾਂ {
        ਲਈ (darray_ਮਾਪ_t e = 0; e < type1->ਅੰਕ_entries; e++) {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ * ਅਚਲ entry1 = &type1->entries[e];
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ * ਅਚਲ entry2 = &type2->entries[e];
            ਜੇ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ਜੇ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ਜੇ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
keymap_compare_types(ਬਣਤਰ xkb_context *ctx,
                     ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1, ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ਅਚਲ darray_ਮਾਪ_t t_max = MIN(keymap1->ਅੰਕ_types, keymap2->ਅੰਕ_types);
    ਲਈ (darray_ਮਾਪ_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ਜੇ (keymap1->ਅੰਕ_types != keymap2->ਅੰਕ_types) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->ਅੰਕ_types, keymap2->ਅੰਕ_types);
        identical = false;
    }

    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
compare_groups(ਬਣਤਰ xkb_context *ctx,
               ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1, ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_ਸੂਚਕਾਂਕ_t g,
               ਅਚਲ ਬਣਤਰ xkb_ਸਮੂਹ *group1, ਅਚਲ ਬਣਤਰ xkb_ਸਮੂਹ *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ਜੇ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx,
                                                 group1->type->name);
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ਵਾਪਸ false;
    }
    ਪ੍ਰਤਿਜ੍ਞਾ(group1->type->ਅੰਕ_ਪੱਧਰ == group2->type->ਅੰਕ_ਪੱਧਰ);

    bool identical = true;
    ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t l = 0; l < group1->type->ਅੰਕ_ਪੱਧਰ; l++) {
        ਅਚਲ ਬਣਤਰ xkb_level * ਅਚਲ level1 = &group1->ਪੱਧਰ[l];
        ਅਚਲ ਬਣਤਰ xkb_level * ਅਚਲ level2 = &group2->ਪੱਧਰ[l];

        /* Keysyms */
        ਜੇ (level1->ਅੰਕ_syms != level2->ਅੰਕ_syms) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->ਅੰਕ_syms, level2->ਅੰਕ_syms);
            identical = false;
        } ਨਹੀਂਤਾਂ {
            ਜੇ (level1->ਅੰਕ_syms > 1) {
                ਲਈ (xkb_keysym_ਗਿਣਤੀ_t k = 0; k < level1->ਅੰਕ_syms; k++) {
                    ਜੇ (level1->s.syms[k] == level2->s.syms[k])
                        ਜਾਰੀ;
                    log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ਨਹੀਂਤਾਂ ਜੇ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ਜੇ (level1->ਅੰਕ_actions != level2->ਅੰਕ_actions) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->ਅੰਕ_actions, level2->ਅੰਕ_actions);
            identical = false;
        } ਨਹੀਂਤਾਂ {
            ਜੇ (level1->ਅੰਕ_actions > 1) {
                ਲਈ (xkb_action_ਗਿਣਤੀ_t a = 0; a < level1->ਅੰਕ_actions; a++) {
                    ਜੇ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        ਜਾਰੀ;
                    log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ਨਹੀਂਤਾਂ ਜੇ (level1->ਅੰਕ_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ਵਾਪਸ identical;
}

ਸਥਿਰ bool
keymap_compare_symbols(ਬਣਤਰ xkb_context *ctx,
                       ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1,
                       ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ਜੇ (keymap1->ਅੰਕ_groups != keymap2->ਅੰਕ_groups) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ਅੰਕ_groups, keymap2->ਅੰਕ_groups);
        identical = false;
    }

    ਜੇ (keymap1->ਅੰਕ_ਸਮੂਹ_ਨਾਂ != keymap2->ਅੰਕ_ਸਮੂਹ_ਨਾਂ) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ਅੰਕ_ਸਮੂਹ_ਨਾਂ, keymap2->ਅੰਕ_ਸਮੂਹ_ਨਾਂ);
        identical = false;
    } ਨਹੀਂਤਾਂ {
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t g = 0; g < keymap1->ਅੰਕ_ਸਮੂਹ_ਨਾਂ; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ਅਚਲ ਅੱਖਰ * ਅਚਲ name1 = xkb_atom_ਪਾਠ(keymap1->ctx,
                                                     keymap1->ਸਮੂਹ_ਨਾਂ[g]);
            ਅਚਲ ਅੱਖਰ * ਅਚਲ name2 = xkb_atom_ਪਾਠ(keymap2->ctx,
                                                     keymap2->ਸਮੂਹ_ਨਾਂ[g]);
            ਜੇ (!streq_null(name1, name2)) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Group #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n", g, name1, name2);
                identical = false;
            }
        }
    }

    /*
     * Check common keycodes
     */
    ਅਚਲ xkb_keycode_t k_max = MIN(keymap1->ਅੰਕ_ਕੁੰਜੀਆਂ, keymap2->ਅੰਕ_ਕੁੰਜੀਆਂ);
    ਲਈ (xkb_keycode_t k = 0; k < k_max; k++) {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ key1 = &keymap1->ਕੁੰਜੀਆਂ[k];
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ key2 = &keymap1->ਕੁੰਜੀਆਂ[k];
        ਜੇ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ਜਾਰੀ;
        }

        ਅਚਲ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ਜੇ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ਜੇ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ਜੇ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ਜੇ (key1->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy != key2->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy ||
            key1->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ != key2->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy,
                    key2->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy,
                    key1->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ,
                    key2->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ);
            identical = false;
        }
        ਜੇ (key1->ਅੰਕ_groups != key2->ਅੰਕ_groups) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->ਅੰਕ_groups, key2->ਅੰਕ_groups);
            identical = false;
        }
        ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t g_max = MIN(key1->ਅੰਕ_groups, key2->ਅੰਕ_groups);
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ਵਾਪਸ identical;
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
xkb_keymap_compare(ਬਣਤਰ xkb_context *ctx,
                   ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1, ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2,
                   ਗਿਣਤੀ xkb_keymap_compare_property properties)
{
    bool identical = true;

    ਜੇ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ਜੇ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ਜੇ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ਜੇ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ਜੇ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ਵਾਪਸ identical;
}
