/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "keymap.h"
#شامل "src/messages-codes.h"
#شامل "src/utils.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "keymap-compare.h"

/*
 * NOTE: these keymap comparison utilities are pretty basic and require that
 *       items have the *same order* in the compared keymaps.
 */

/* TODO: enable relaxing the order on some items, such as: aliases, etc. */


ساکن bool
keymap_compare_mods(ساخت xkb_context *ctx,
                    ثابت ساخت xkb_keymap *keymap1,
                    ثابت ساخت xkb_keymap *keymap2)
{
    bool identical = true;
    /* Check that the keymaps have the same virtual modifiers */

    /* Check common modifiers*/
    ثابت xkb_mod_اشاریہ_t mod_max = MIN(keymap1->mods.عدد_mods,
                                        keymap2->mods.عدد_mods);
    برائے (xkb_mod_اشاریہ_t mod = 0; mod < mod_max; mod++) {
        ثابت ساخت xkb_mod * ثابت mod1 = &keymap1->mods.mods[mod];
        ثابت ساخت xkb_mod * ثابت mod2 = &keymap2->mods.mods[mod];
        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx, mod1->name);
        ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx, mod2->name);
        اگر (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Modifier #%"PRIu32" names do not match: "
                    "\"%s\" != \"%s\"\n", mod, name1, name2);
            identical = false;
        }
        اگر (mod1->type != mod2->type) {
            /* Unlikely, only for completeness */
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Modifier #%"PRIu32" types do not match: "
                    "%d != %d\n", mod, mod1->type, mod2->type);
            identical = false;
        }
        اگر (mod1->mapping != mod2->mapping) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Modifier #%"PRIu32" mappings do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    mod, mod1->mapping, mod2->mapping);
            identical = false;
        }
    }

    /* Check non-common modifiers */
    اگر (keymap1->mods.عدد_mods != keymap2->mods.عدد_mods) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Modifiers counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->mods.عدد_mods, keymap2->mods.عدد_mods);
        identical = false;
    }

    واپس identical;
}

ساکن bool
keymap_compare_keycodes(ساخت xkb_context *ctx,
                        ثابت ساخت xkb_keymap *keymap1,
                        ثابت ساخت xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check keycode range
     */
    اگر (keymap1->عدد_کلیدیں != keymap2->عدد_کلیدیں) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->عدد_کلیدیں, keymap2->عدد_کلیدیں);
        identical = false;
    }
    اگر (keymap1->min_کلید_رمز != keymap2->min_کلید_رمز) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Min keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_کلید_رمز, keymap2->min_کلید_رمز);
        identical = false;
    }
    اگر (keymap1->max_کلید_رمز != keymap2->max_کلید_رمز) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Low keycodes counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->عدد_کلیدیں_کم, keymap2->عدد_کلیدیں_کم);
        identical = false;
    }
    اگر (keymap1->max_کلید_رمز != keymap2->max_کلید_رمز) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Max keycodes do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->min_کلید_رمز, keymap2->min_کلید_رمز);
        identical = false;
    }

    /* Check common keys */
    ثابت xkb_keycode_t k_max = MIN(keymap1->عدد_کلیدیں, keymap2->عدد_کلیدیں);
    برائے (xkb_keycode_t k = 0; k < k_max; k++) {
        ثابت ساخت xkb_کلید * ثابت key1 = &keymap1->کلیدیں[k];
        ثابت ساخت xkb_کلید * ثابت key2 = &keymap1->کلیدیں[k];
        اگر (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            جاری;
        }

        ثابت xkb_keycode_t kc = key1->keycode;

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx, key1->name);
        ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx, key2->name);
        اگر (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" names do not match: "
                    "\"%s\" != \"%s\"\n", kc, name1, name2);
            identical = false;
        }
    }

    /* Check common aliases */
    ثابت darray_سائز_t a_max = MIN(keymap1->عدد_کلید_aliases,
                                    keymap2->عدد_کلید_aliases);
    برائے (darray_سائز_t a = 0; a < a_max; a++) {
        ثابت ساخت xkb_کلید_alias * ثابت entry1 = &keymap1->کلید_aliases[a];
        ثابت ساخت xkb_کلید_alias * ثابت entry2 = &keymap2->کلید_aliases[a];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ثابت حرف * ثابت alias1 = xkb_atom_متن(keymap1->ctx, entry1->alias);
        ثابت حرف * ثابت alias2 = xkb_atom_متن(keymap2->ctx, entry2->alias);
        اگر (!streq_null(alias1, alias2)) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Alias #%u names do not match: \"%s\" != \"%s\"\n",
                    a, alias1, alias2);
            identical = false;
        }

        ثابت حرف * ثابت real1 = xkb_atom_متن(keymap1->ctx, entry1->real);
        ثابت حرف * ثابت real2 = xkb_atom_متن(keymap2->ctx, entry2->real);
        اگر (!streq_null(real1, real2)) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Alias #%u \"%s\" target do not match: \"%s\" != \"%s\"\n",
                    a, alias1, real1, real2);
            identical = false;
        }
    }
    اگر (keymap1->عدد_کلید_aliases != keymap2->عدد_کلید_aliases) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Aliases count do not match: %u != %u\n",
                keymap1->عدد_کلید_aliases, keymap2->عدد_کلید_aliases);
        identical = false;
    }

    واپس identical;
}

ساکن bool
keymap_compare_leds(ساخت xkb_context *ctx,
                    ثابت ساخت xkb_keymap *keymap1,
                    ثابت ساخت xkb_keymap *keymap2)
{
    bool identical = true;

    /* Check common LEDs */
    xkb_led_اشاریہ_t led_max = MIN(keymap1->عدد_leds, keymap2->عدد_leds);
    برائے (xkb_led_اشاریہ_t led = 0; led < led_max; led++) {
        ثابت ساخت xkb_led * ثابت led1 = &keymap1->leds[led];
        ثابت ساخت xkb_led * ثابت led2 = &keymap2->leds[led];

        /* NOTE: cannot compare atoms: keymaps may use different contexts */
        ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx, led1->name);
        ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx, led2->name);
        اگر (!streq_null(name1, name2)) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" names do not match: \"%s\" != \"%s\"\n",
                    led, name1, name2);
            identical = false;
        }

        اگر (led1->which_groups != led2->which_groups) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_groups` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_groups, led2->which_groups);
            identical = false;
        }

        اگر (led1->groups != led2->groups) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" \"%s\" `groups` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->groups, led2->groups);
            identical = false;
        }

        اگر (led1->which_mods != led2->which_mods) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" \"%s\" `which_mods` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->which_mods, led2->which_mods);
            identical = false;
        }

        اگر (led1->mods.mods != led2->mods.mods) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" \"%s\" `mods` do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n",
                    led, name1, led1->mods.mods, led2->mods.mods);
            identical = false;
        }

        اگر (led1->ctrls != led2->ctrls) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "LED #%"PRIu32" \"%s\" `ctrls` do not match: "
                    "0x%x != 0x%x\n",
                    led, name1, led1->ctrls, led2->ctrls);
            identical = false;
        }
    }

    اگر (keymap1->عدد_leds != keymap2->عدد_leds) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "LEDs count do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->عدد_leds, keymap2->عدد_leds);
        identical = false;
    }

    واپس identical;
}

ساکن bool
compare_types(ساخت xkb_context *ctx,
              ثابت ساخت xkb_keymap *keymap1, ثابت ساخت xkb_keymap *keymap2,
              ثابت ساخت xkb_کلید_type *type1, ثابت ساخت xkb_کلید_type *type2)
{
    bool identical = true;

    /* NOTE: cannot compare atoms: keymaps may use different contexts */
    ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx, type1->name);
    ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx, type2->name);
    اگر (!streq_null(name1, name2)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key type names do not match: \"%s\" != \"%s\"\n",
                name1, name2);
        identical = false;
    }

    /* From here we will use the name of `type1` as the reference */

    اگر (type1->mods.mods != type2->mods.mods) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key type \"%s\" mods do not match: "
                "0x%"PRIx32" != 0x%"PRIx32"\n",
                name1, type1->mods.mods, type2->mods.mods);
        /* No point to check further properties */
        واپس false;
    }

    اگر (type1->عدد_سطحیں != type2->عدد_سطحیں) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key type \"%s\" levels count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->عدد_سطحیں, type2->عدد_سطحیں);
        /* No point to check further properties */
        واپس false;
    }

    اگر (type1->عدد_level_نام != type2->عدد_level_نام) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key type \"%s\" level names count do not match: "
                "%"PRIu32" != %"PRIu32"\n",
                name1, type1->عدد_level_نام, type2->عدد_level_نام);
        identical = false;
    } ورنہ {
        برائے (xkb_level_اشاریہ_t l = 0; l < type1->عدد_level_نام; l++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ثابت حرف * ثابت lname1 = xkb_atom_متن(keymap1->ctx,
                                                      type1->level_نام[l]);
            ثابت حرف * ثابت lname2 = xkb_atom_متن(keymap2->ctx,
                                                      type2->level_نام[l]);
            اگر (!streq_null(lname1, lname2)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key type \"%s\" level #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n",
                        name1, l, lname1, lname2);
                identical = false;
            }
        }
    }

    اگر (type1->عدد_entries != type2->عدد_entries) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key type \"%s\" entries count do not match: "
                "%u != %u\n",
                name1, type1->عدد_entries, type2->عدد_entries);
        identical = false;
    } ورنہ {
        برائے (darray_سائز_t e = 0; e < type1->عدد_entries; e++) {
            ثابت ساخت xkb_کلید_type_اندراج * ثابت entry1 = &type1->entries[e];
            ثابت ساخت xkb_کلید_type_اندراج * ثابت entry2 = &type2->entries[e];
            اگر (entry1->level != entry2->level) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key type \"%s\" entry #%u levels do not match: "
                        "%"PRIu32" != %"PRIu32"\n",
                        name1, e, entry1->level, entry2->level);
                identical = false;
            }
            اگر (entry1->mods.mods != entry2->mods.mods) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key type \"%s\" entry #%u mods do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->mods.mods, entry2->mods.mods);
                identical = false;
            }
            اگر (entry1->preserve.mods != entry2->preserve.mods) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key type \"%s\" entry #%u preserve do not match: "
                        "0x%"PRIx32" != 0x%"PRIx32"\n",
                        name1, e, entry1->preserve.mods, entry2->preserve.mods);
                identical = false;
            }
        }
    }

    واپس identical;
}

ساکن bool
keymap_compare_types(ساخت xkb_context *ctx,
                     ثابت ساخت xkb_keymap *keymap1, ثابت ساخت xkb_keymap *keymap2)
{
    bool identical = true;

    /* Compare common types */
    ثابت darray_سائز_t t_max = MIN(keymap1->عدد_types, keymap2->عدد_types);
    برائے (darray_سائز_t t = 0; t < t_max; t++) {
        identical = compare_types(ctx, keymap1, keymap2,
                                  &keymap1->types[t], &keymap2->types[t])
                  && identical;
    }

    اگر (keymap1->عدد_types != keymap2->عدد_types) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key types counts do not match: %u != %u\n",
                keymap1->عدد_types, keymap2->عدد_types);
        identical = false;
    }

    واپس identical;
}

ساکن bool
compare_groups(ساخت xkb_context *ctx,
               ثابت ساخت xkb_keymap *keymap1, ثابت ساخت xkb_keymap *keymap2,
               xkb_keycode_t kc, xkb_layout_اشاریہ_t g,
               ثابت ساخت xkb_گروہ *group1, ثابت ساخت xkb_گروہ *group2)
{
    /* TODO: repeatedly compare the same key types is inefficient!! */
    اگر (!compare_types(ctx, keymap1, keymap2, group1->type, group2->type)) {
        ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx,
                                                 group1->type->name);
        ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx,
                                                 group2->type->name);
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Key 0x%"PRIx32"/group %"PRIu32" types do not match: "
                "\"%s\" != \"%s\"\n", kc, g, name1, name2);
        /* No relevant to compare groups with different types */
        واپس false;
    }
    أكد(group1->type->عدد_سطحیں == group2->type->عدد_سطحیں);

    bool identical = true;
    برائے (xkb_level_اشاریہ_t l = 0; l < group1->type->عدد_سطحیں; l++) {
        ثابت ساخت xkb_level * ثابت level1 = &group1->سطحیں[l];
        ثابت ساخت xkb_level * ثابت level2 = &group2->سطحیں[l];

        /* Keysyms */
        اگر (level1->عدد_syms != level2->عدد_syms) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "keysyms count do not match: %u != %u\n",
                    kc, g, l, level1->عدد_syms, level2->عدد_syms);
            identical = false;
        } ورنہ {
            اگر (level1->عدد_syms > 1) {
                برائے (xkb_keysym_گنتی_t k = 0; k < level1->عدد_syms; k++) {
                    اگر (level1->s.syms[k] == level2->s.syms[k])
                        جاری;
                    log_err(ctx, XKB_LOG_پیغام_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "keysyms #%u do not match: "
                            "0x%"PRIx32" != 0x%"PRIx32"\n",
                            kc, g, l, k, level1->s.syms[k], level2->s.syms[k]);
                    identical = false;
                }
            } ورنہ اگر (level1->s.sym != level2->s.sym) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "keysyms do not match: 0x%"PRIx32" != 0x%"PRIx32"\n",
                        kc, g, l, level1->s.sym, level2->s.sym);
                identical = false;
            }
        }

        /* Actions */
        اگر (level1->عدد_actions != level2->عدد_actions) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                    "actions count do not match: %u != %u\n",
                    kc, g, l, level1->عدد_actions, level2->عدد_actions);
            identical = false;
        } ورنہ {
            اگر (level1->عدد_actions > 1) {
                برائے (xkb_action_گنتی_t a = 0; a < level1->عدد_actions; a++) {
                    اگر (action_equal(&level1->a.actions[a], &level2->a.actions[a]))
                        جاری;
                    log_err(ctx, XKB_LOG_پیغام_NO_ID,
                            "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                            "actions #%u do not match\n", kc, g, l, a);
                    identical = false;
                }
            } ورنہ اگر (level1->عدد_actions == 1 &&
                       !action_equal(&level1->a.action, &level2->a.action)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Key 0x%"PRIx32"/group %"PRIu32"/level %"PRIu32" "
                        "actions do not match\n", kc, g, l);
                identical = false;
            }
        }
    }
    واپس identical;
}

ساکن bool
keymap_compare_symbols(ساخت xkb_context *ctx,
                       ثابت ساخت xkb_keymap *keymap1,
                       ثابت ساخت xkb_keymap *keymap2)
{
    bool identical = true;

    /*
     * Check groups
     */
    اگر (keymap1->عدد_groups != keymap2->عدد_groups) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Group counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->عدد_groups, keymap2->عدد_groups);
        identical = false;
    }

    اگر (keymap1->عدد_گروہ_نام != keymap2->عدد_گروہ_نام) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Group name counts do not match: %"PRIu32" != %"PRIu32"\n",
                keymap1->عدد_گروہ_نام, keymap2->عدد_گروہ_نام);
        identical = false;
    } ورنہ {
        برائے (xkb_layout_اشاریہ_t g = 0; g < keymap1->عدد_گروہ_نام; g++) {
            /* NOTE: cannot compare atoms: keymaps may use different contexts */
            ثابت حرف * ثابت name1 = xkb_atom_متن(keymap1->ctx,
                                                     keymap1->گروہ_نام[g]);
            ثابت حرف * ثابت name2 = xkb_atom_متن(keymap2->ctx,
                                                     keymap2->گروہ_نام[g]);
            اگر (!streq_null(name1, name2)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Group #%"PRIu32" names do not match: "
                        "\"%s\" != \"%s\"\n", g, name1, name2);
                identical = false;
            }
        }
    }

    /*
     * Check common keycodes
     */
    ثابت xkb_keycode_t k_max = MIN(keymap1->عدد_کلیدیں, keymap2->عدد_کلیدیں);
    برائے (xkb_keycode_t k = 0; k < k_max; k++) {
        ثابت ساخت xkb_کلید * ثابت key1 = &keymap1->کلیدیں[k];
        ثابت ساخت xkb_کلید * ثابت key2 = &keymap1->کلیدیں[k];
        اگر (key1->keycode != key2->keycode) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key #%"PRIu32" keycodes do not match: "
                    "%"PRIx32" != %"PRIx32"\n", k, key1->keycode, key2->keycode);
            identical = false;
            /* It does not make sense to compare further properties */
            جاری;
        }

        ثابت xkb_keycode_t kc = key1->keycode;

        /* NOTE: key name checked in `compare_keycodes()` */

        اگر (key1->modmap != key2->modmap) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" modmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->modmap, key2->modmap);
            identical = false;
        }
        اگر (key1->vmodmap != key2->vmodmap) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" vmodmap do not match: "
                    "0x%"PRIx32" != 0x%"PRIx32"\n", kc, key1->vmodmap, key2->vmodmap);
            identical = false;
        }
        اگر (key1->repeats != key2->repeats) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" repeats do not match: "
                    "%d != %d\n", kc, key1->repeats, key2->repeats);
            identical = false;
        }
        اگر (key1->برآمد_کا_range_گروہ_policy != key2->برآمد_کا_range_گروہ_policy ||
            key1->برآمد_کا_range_گروہ_نمبر != key2->برآمد_کا_range_گروہ_نمبر) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" out-of-range do not match: "
                    "%d != %d or %"PRIu32" != %"PRIu32"\n", kc,
                    key1->برآمد_کا_range_گروہ_policy,
                    key2->برآمد_کا_range_گروہ_policy,
                    key1->برآمد_کا_range_گروہ_نمبر,
                    key2->برآمد_کا_range_گروہ_نمبر);
            identical = false;
        }
        اگر (key1->عدد_groups != key2->عدد_groups) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Key 0x%"PRIx32" groups counts do not match: "
                    "%"PRIu32" != %"PRIu32"\n", kc,
                    key1->عدد_groups, key2->عدد_groups);
            identical = false;
        }
        ثابت xkb_layout_اشاریہ_t g_max = MIN(key1->عدد_groups, key2->عدد_groups);
        برائے (xkb_layout_اشاریہ_t g = 0; g < g_max; g++) {
            identical = compare_groups(ctx, keymap1, keymap2, kc, g,
                                       &key1->groups[g], &key2->groups[g])
                      && identical;
        }
    }

    واپس identical;
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
xkb_keymap_compare(ساخت xkb_context *ctx,
                   ثابت ساخت xkb_keymap *keymap1, ثابت ساخت xkb_keymap *keymap2,
                   شمار xkb_keymap_compare_property properties)
{
    bool identical = true;

    اگر (properties & XKB_KEYMAP_CMP_MODS) {
        identical = keymap_compare_mods(ctx, keymap1, keymap2) && identical;
    }

    اگر (properties & XKB_KEYMAP_CMP_TYPES) {
        identical = keymap_compare_types(ctx, keymap1, keymap2) && identical;
    }

    اگر (properties & XKB_KEYMAP_CMP_LEDS) {
        identical = keymap_compare_leds(ctx, keymap1, keymap2) && identical;
    }

    اگر (properties & XKB_KEYMAP_CMP_KEYCODES) {
        identical = keymap_compare_keycodes(ctx, keymap1, keymap2) && identical;
    }

    اگر (properties & XKB_KEYMAP_CMP_SYMBOLS) {
        identical = keymap_compare_symbols(ctx, keymap1, keymap2) && identical;
    }

    واپس identical;
}
