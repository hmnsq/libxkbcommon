/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/**
 * Keymap properties to compare in `xkb_keymap_compare()`.
 *
 * The level of detail should be adjusted to the need.
 */
ਗਿਣਤੀ xkb_keymap_compare_property {
    XKB_KEYMAP_CMP_MODS = (1u << 0),
    XKB_KEYMAP_CMP_LEDS = (1u << 1),
    XKB_KEYMAP_CMP_TYPES = (1u << 2),
    /* TODO: compat entries */
    XKB_KEYMAP_CMP_KEYCODES = (1u << 3),
    XKB_KEYMAP_CMP_SYMBOLS = (1u << 4),
    XKB_KEYMAP_CMP_ALL = XKB_KEYMAP_CMP_MODS
                       | XKB_KEYMAP_CMP_LEDS
                       | XKB_KEYMAP_CMP_TYPES
                       | XKB_KEYMAP_CMP_KEYCODES
                       | XKB_KEYMAP_CMP_SYMBOLS,
    /* TODO: add interprets to next */
    XKB_KEYMAP_CMP_POSSIBLY_DROPPED = XKB_KEYMAP_CMP_TYPES,
};

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keymap_compare(ਬਣਤਰ xkb_context *ctx,
                   ਅਚਲ ਬਣਤਰ xkb_keymap *keymap1,
                   ਅਚਲ ਬਣਤਰ xkb_keymap *keymap2,
                   ਗਿਣਤੀ xkb_keymap_compare_property properties);
