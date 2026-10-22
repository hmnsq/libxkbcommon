/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "utils.h"

/**
 * Keymap properties to compare in `xkb_keymap_compare()`.
 *
 * The level of detail should be adjusted to the need.
 */
ગણના xkb_keymap_compare_property {
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

XKB_EXPORT_PRIVATE bool
xkb_keymap_compare(સંરચના xkb_context *ctx,
                   અચલ સંરચના xkb_keymap *keymap1,
                   અચલ સંરચના xkb_keymap *keymap2,
                   ગણના xkb_keymap_compare_property properties);
