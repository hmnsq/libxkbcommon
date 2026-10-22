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
keymap_compare_mods(ರಚನೆ xkb_context *ctx,
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1,
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t mod_max = MIN(keymap1->mods.ಅಂಕೆ_mods,
                                        keymap2->mods.ಅಂಕೆ_mods);
    ಗಾಗಿ (xkb_mod_ಸೂಚ್ಯಂಕ_t mod = 0; mod < mod_max; mod++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod * ಸ್ಥಿರಾಂಕ mod1 = &keymap1->mods.mods[mod];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod * ಸ್ಥಿರಾಂಕ mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, mod1->name);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, mod2->name);
        ಆದರೆ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        ಆದರೆ (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        ಆದರೆ (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    ಆದರೆ (keymap1->mods.ಅಂಕೆ_mods != keymap2->mods.ಅಂಕೆ_mods) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.ಅಂಕೆ_mods, keymap2->mods.ಅಂಕೆ_mods);
        identical = false;
    }

    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
keymap_compare_keycodes(ರಚನೆ xkb_context *ctx,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    ಆದರೆ (keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು != keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
        identical = false;
    }
    ಆದರೆ (keymap1->min_ಕೀಲಿ_ಸಂಕೇತ != keymap2->min_ಕೀಲಿ_ಸಂಕೇತ) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ಕೀಲಿ_ಸಂಕೇತ, keymap2->min_ಕೀಲಿ_ಸಂಕೇತ);
        identical = false;
    }
    ಆದರೆ (keymap1->max_ಕೀಲಿ_ಸಂಕೇತ != keymap2->max_ಕೀಲಿ_ಸಂಕೇತ) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ);
        identical = false;
    }
    ಆದರೆ (keymap1->max_ಕೀಲಿ_ಸಂಕೇತ != keymap2->max_ಕೀಲಿ_ಸಂಕೇತ) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_ಕೀಲಿ_ಸಂಕೇತ, keymap2->min_ಕೀಲಿ_ಸಂಕೇತ);
        identical = false;
    }

    /* Check common keys */
    ಸ್ಥಿರಾಂಕ xkb_keycode_t k_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
    ಗಾಗಿ (xkb_keycode_t k = 0; k < k_max; k++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ key1 = &keymap1->ಕೀಲಿಗಳು[k];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ key2 = &keymap1->ಕೀಲಿಗಳು[k];
        ಆದರೆ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ಮುಂದುವರಿ;
        }

        ಸ್ಥಿರಾಂಕ xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, key1->name);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, key2->name);
        ಆದರೆ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t a_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases,
                                    keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases);
    ಗಾಗಿ (darray_ಗಾತ್ರ_t a = 0; a < a_max; a++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ entry1 = &keymap1->ಕೀಲಿ_aliases[a];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ entry2 = &keymap2->ಕೀಲಿ_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ alias1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, entry1->alias);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ alias2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, entry2->alias);
        ಆದರೆ (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ real1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, entry1->real);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ real2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, entry2->real);
        ಆದರೆ (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    ಆದರೆ (keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases != keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->ಅಂಕೆ_ಕೀಲಿ_aliases, keymap2->ಅಂಕೆ_ಕೀಲಿ_aliases);
        identical = false;
    }

    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
keymap_compare_leds(ರಚನೆ xkb_context *ctx,
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1,
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_ಸೂಚ್ಯಂಕ_t led_max = MIN(keymap1->ಅಂಕೆ_leds, keymap2->ಅಂಕೆ_leds);
    ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t led = 0; led < led_max; led++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led * ಸ್ಥಿರಾಂಕ led1 = &keymap1->leds[led];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led * ಸ್ಥಿರಾಂಕ led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, led1->name);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, led2->name);
        ಆದರೆ (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        ಆದರೆ (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        ಆದರೆ (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        ಆದರೆ (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        ಆದರೆ (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        ಆದರೆ (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    ಆದರೆ (keymap1->ಅಂಕೆ_leds != keymap2->ಅಂಕೆ_leds) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_leds, keymap2->ಅಂಕೆ_leds);
        identical = false;
    }

    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
compare_types(ರಚನೆ xkb_context *ctx,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *type1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx, type1->name);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx, type2->name);
    ಆದರೆ (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    ಆದರೆ (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (type1->ಅಂಕೆ_ಮಟ್ಟಗಳು != type2->ಅಂಕೆ_ಮಟ್ಟಗಳು) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ಅಂಕೆ_ಮಟ್ಟಗಳು, type2->ಅಂಕೆ_ಮಟ್ಟಗಳು);
        /* No point to check further properties */
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (type1->ಅಂಕೆ_level_ಹೆಸರುಗಳು != type2->ಅಂಕೆ_level_ಹೆಸರುಗಳು) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->ಅಂಕೆ_level_ಹೆಸರುಗಳು, type2->ಅಂಕೆ_level_ಹೆಸರುಗಳು);
        identical = false;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t l = 0; l < type1->ಅಂಕೆ_level_ಹೆಸರುಗಳು; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ lname1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx,
                                                      type1->level_ಹೆಸರುಗಳು[l]);
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ lname2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx,
                                                      type2->level_ಹೆಸರುಗಳು[l]);
            ಆದರೆ (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    ಆದರೆ (type1->ಅಂಕೆ_entries != type2->ಅಂಕೆ_entries) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->ಅಂಕೆ_entries, type2->ಅಂಕೆ_entries);
        identical = false;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (darray_ಗಾತ್ರ_t e = 0; e < type1->ಅಂಕೆ_entries; e++) {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು * ಸ್ಥಿರಾಂಕ entry1 = &type1->entries[e];
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು * ಸ್ಥಿರಾಂಕ entry2 = &type2->entries[e];
            ಆದರೆ (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            ಆದರೆ (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            ಆದರೆ (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
keymap_compare_types(ರಚನೆ xkb_context *ctx,
                     ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t t_max = MIN(keymap1->ಅಂಕೆ_types, keymap2->ಅಂಕೆ_types);
    ಗಾಗಿ (darray_ಗಾತ್ರ_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    ಆದರೆ (keymap1->ಅಂಕೆ_types != keymap2->ಅಂಕೆ_types) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->ಅಂಕೆ_types, keymap2->ಅಂಕೆ_types);
        identical = false;
    }

    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
compare_groups(ರಚನೆ xkb_context *ctx,
               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_ಸೂಚ್ಯಂಕ_t g,
               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಗುಂಪು *group1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಗುಂಪು *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    ಆದರೆ (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx,
                                                 group1->type->name);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        ಹಿಂತಿರುಗಿ false;
    }
    ಪ್ರತಿಜ್ಞಾ(group1->type->ಅಂಕೆ_ಮಟ್ಟಗಳು == group2->type->ಅಂಕೆ_ಮಟ್ಟಗಳು);

    bool identical = true;
    ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t l = 0; l < group1->type->ಅಂಕೆ_ಮಟ್ಟಗಳು; l++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level * ಸ್ಥಿರಾಂಕ level1 = &group1->ಮಟ್ಟಗಳು[l];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level * ಸ್ಥಿರಾಂಕ level2 = &group2->ಮಟ್ಟಗಳು[l];

        /* Keysyms */
        ಆದರೆ (level1->ಅಂಕೆ_syms != level2->ಅಂಕೆ_syms) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->ಅಂಕೆ_syms, level2->ಅಂಕೆ_syms);
            identical = false;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (level1->ಅಂಕೆ_syms > 1) {
                ಗಾಗಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < level1->ಅಂಕೆ_syms; k++) {
                    ಆದರೆ (level1->s.syms[k] == level2->s.syms[k])
                        ಮುಂದುವರಿ;
                    log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        ಆದರೆ (level1->ಅಂಕೆ_actions != level2->ಅಂಕೆ_actions) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->ಅಂಕೆ_actions, level2->ಅಂಕೆ_actions);
            identical = false;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (level1->ಅಂಕೆ_actions > 1) {
                ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t a = 0; a < level1->ಅಂಕೆ_actions; a++) {
                    ಆದರೆ (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        ಮುಂದುವರಿ;
                    log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (level1->ಅಂಕೆ_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    ಹಿಂತಿರುಗಿ identical;
}

ಸ್ಥಿರ bool
keymap_compare_symbols(ರಚನೆ xkb_context *ctx,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    ಆದರೆ (keymap1->ಅಂಕೆ_groups != keymap2->ಅಂಕೆ_groups) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_groups, keymap2->ಅಂಕೆ_groups);
        identical = false;
    }

    ಆದರೆ (keymap1->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು != keymap2->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು, keymap2->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು);
        identical = false;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t g = 0; g < keymap1->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name1 = xkb_atom_ಪಠ್ಯ(keymap1->ctx,
                                                     keymap1->ಗುಂಪು_ಹೆಸರುಗಳು[g]);
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name2 = xkb_atom_ಪಠ್ಯ(keymap2->ctx,
                                                     keymap2->ಗುಂಪು_ಹೆಸರುಗಳು[g]);
            ಆದರೆ (!streq_null(name1, name2)) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Group #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n", g, name1, name2);
                identical = false;
            }
        }
    }

    /*
     * Check common keycodes
     */
    ಸ್ಥಿರಾಂಕ xkb_keycode_t k_max = MIN(keymap1->ಅಂಕೆ_ಕೀಲಿಗಳು, keymap2->ಅಂಕೆ_ಕೀಲಿಗಳು);
    ಗಾಗಿ (xkb_keycode_t k = 0; k < k_max; k++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ key1 = &keymap1->ಕೀಲಿಗಳು[k];
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ key2 = &keymap1->ಕೀಲಿಗಳು[k];
        ಆದರೆ (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            ಮುಂದುವರಿ;
        }

        ಸ್ಥಿರಾಂಕ xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        ಆದರೆ (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        ಆದರೆ (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        ಆದರೆ (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        ಆದರೆ (key1->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy != key2->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy ||
            key1->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ != key2->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy,
                    key2->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy,
                    key1->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ,
                    key2->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ);
            identical = false;
        }
        ಆದರೆ (key1->ಅಂಕೆ_groups != key2->ಅಂಕೆ_groups) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->ಅಂಕೆ_groups, key2->ಅಂಕೆ_groups);
            identical = false;
        }
        ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t g_max = MIN(key1->ಅಂಕೆ_groups, key2->ಅಂಕೆ_groups);
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    ಹಿಂತಿರುಗಿ identical;
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
xkb_keymap_compare(ರಚನೆ xkb_context *ctx,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap1, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap2,
                   ಎಣಿಕೆ xkb_keymap_compare_property properties)
{
    bool identical = true;

    ಆದರೆ (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    ಆದರೆ (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    ಆದರೆ (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    ಆದರೆ (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    ಆದರೆ (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    ಹಿಂತಿರುಗಿ identical;
}
