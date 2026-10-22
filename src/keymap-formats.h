/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#నిర్వచించు DEFAULT_ఆదానం_KEYMAP_FORMAT  XKB_KEYMAP_FORMAT_పాఠ్యం_V2
#నిర్వచించు DEFAULT_OUTPUT_KEYMAP_FORMAT XKB_KEYMAP_USE_ORIGINAL_FORMAT

/**
 * Provide an array of the supported keymap formats, sorted in ascending order
 * (newest last).
 *
 * @param[out] formats An array of the supported keymap formats.
 *
 * @returns The size of the array.
 *
 * @memberof xkb_keymap
 */
కొలత_t
xkb_keymap_supported_formats(స్థిరరాశి గణన xkb_keymap_format **formats);

/**
 * Check if the given keymap format is supported.
 *
 * @param[in] format A keymap format to test.
 *
 * @memberof xkb_keymap
 */
bool
xkb_keymap_is_supported_format(గణన xkb_keymap_format format);

/**
 * Parse a keymap version.
 *
 * @param[in] raw A raw keymap format string. May be the numeric value or the
 * label of the version, starting with `v`.
 *
 * @return The corresponding format or 0 on error.
 */
గణన xkb_keymap_format
xkb_keymap_parse_format(స్థిరరాశి అక్షరం *raw);

/**
 * Return a human-friendly label for the keymap format.
 *
 * @param[in] format A keymap format to look for.
 *
 * @return The label corresponding to the format, or `NULL` if not supported.
 */
స్థిరరాశి అక్షరం *
xkb_keymap_get_format_label(గణన xkb_keymap_format format);
