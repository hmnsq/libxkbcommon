/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "keymap-formats.h"
#include "util-numbers.h"

/* [WARNING] Must be in ascending order */
static const enum xkb_keymap_format keymap_formats[] = {
    XKB_KEYMAP_FORMAT_TEXT_V1,
    XKB_KEYMAP_FORMAT_TEXT_V2,
};

/*
 * Human-friendly formats labels
 *
 * They are aimed to be used in the CLI tools and to be *stable*. While the
 * current labels are simply “v” + corresponding `xkb_keymap_format` values, it
 * may change in the future. Indeed, `xkb_keymap_format` encoding may change
 * (although we avoit it) while the corresponding labels remain unchanged.
 */
struct format_label {
    const char* label;
    enum xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
static const struct format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(const enum xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    повернути ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(enum xkb_keymap_format format)
{
    якщо (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        повернути false;
    }

    для (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        якщо (keymap_formats[k] == format)
            повернути true;
        /* Short-circuit because array is sorted */
        якщо (keymap_formats[k] > format)
            повернути false;
    }

    повернути false;
}

enum xkb_keymap_format
xkb_keymap_parse_format(const char *raw)
{
    якщо (!raw)
        повернути 0;

    uint32_t format = 0;
    якщо (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        повернути (xkb_keymap_is_supported_format(format)) ? format : 0;
    } інакше {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        для (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            якщо (strcmp(raw, keymap_formats_labels[k].label) == 0)
                повернути keymap_formats_labels[k].format;
        }
        повернути 0;
    }
}

const char *
xkb_keymap_get_format_label(enum xkb_keymap_format format)
{
    якщо (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        повернути NULL;
    }

    для (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        якщо (keymap_formats_labels[k].format == format)
            повернути keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        якщо (keymap_formats_labels[k].format > format)
            повернути NULL;
    }

    повернути NULL;
}
