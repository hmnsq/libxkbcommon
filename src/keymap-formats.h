/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"

#घोषणा DEFAULT_निवेश_KEYMAP_FORMAT  XKB_KEYMAP_FORMAT_TEXT_V2
#घोषणा DEFAULT_OUTPUT_KEYMAP_FORMAT XKB_KEYMAP_USE_ORIGINAL_FORMAT

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
माप_प्रकार
xkb_keymap_supported_formats(स्थिर क्रमागत xkb_keymap_format **formats);

/**
 * Check if the given keymap format is supported.
 *
 * @param[in] format A keymap format to test.
 *
 * @memberof xkb_keymap
 */
bool
xkb_keymap_is_supported_format(क्रमागत xkb_keymap_format format);

/**
 * Parse a keymap version.
 *
 * @param[in] raw A raw keymap format string. May be the numeric value or the
 * label of the version, starting with `v`.
 *
 * @return The corresponding format or 0 on error.
 */
क्रमागत xkb_keymap_format
xkb_keymap_parse_format(स्थिर अक्षर *raw);

/**
 * Return a human-friendly label for the keymap format.
 *
 * @param[in] format A keymap format to look for.
 *
 * @return The label corresponding to the format, or `NULL` if not supported.
 */
स्थिर अक्षर *
xkb_keymap_get_format_label(क्रमागत xkb_keymap_format format);
