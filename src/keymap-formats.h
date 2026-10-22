/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಒಳಹರಿವು_KEYMAP_FORMAT  XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_OUTPUT_KEYMAP_FORMAT XKB_KEYMAP_USE_ORIGINAL_FORMAT

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
ಗಾತ್ರ_t
xkb_keymap_supported_formats(ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_format **formats);

/**
 * Check if the given keymap format is supported.
 *
 * @param[in] format A keymap format to test.
 *
 * @memberof xkb_keymap
 */
bool
xkb_keymap_is_supported_format(ಎಣಿಕೆ xkb_keymap_format format);

/**
 * Parse a keymap version.
 *
 * @param[in] raw A raw keymap format string. May be the numeric value or the
 * label of the version, starting with `v`.
 *
 * @return The corresponding format or 0 on error.
 */
ಎಣಿಕೆ xkb_keymap_format
xkb_keymap_parse_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *raw);

/**
 * Return a human-friendly label for the keymap format.
 *
 * @param[in] format A keymap format to look for.
 *
 * @return The label corresponding to the format, or `NULL` if not supported.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_get_format_label(ಎಣಿಕೆ xkb_keymap_format format);
