/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "utils.h"
#ضمن "keymap-formats.h"
#ضمن "util-numbers.h"

/* [WARNING] Must be in ascending order */
ساكن ثابت تعداد xkb_keymap_format keymap_formats[] = {
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
بنية format_label {
    ثابت حرف* label;
    تعداد xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ساكن ثابت بنية format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(ثابت تعداد xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ܦܢܐ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(تعداد xkb_keymap_format format)
{
    ܐܢ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ܦܢܐ false;
    }

    لكل (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        ܐܢ (keymap_formats[k] == format)
            ܦܢܐ true;
        /* Short-circuit because array is sorted */
        ܐܢ (keymap_formats[k] > format)
            ܦܢܐ false;
    }

    ܦܢܐ false;
}

تعداد xkb_keymap_format
xkb_keymap_parse_format(ثابت حرف *raw)
{
    ܐܢ (!raw)
        ܦܢܐ 0;

    uint32_t format = 0;
    ܐܢ (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ܦܢܐ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ܐܚܪܢܐ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        لكل (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            ܐܢ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ܦܢܐ keymap_formats_labels[k].format;
        }
        ܦܢܐ 0;
    }
}

ثابت حرف *
xkb_keymap_get_format_label(تعداد xkb_keymap_format format)
{
    ܐܢ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ܦܢܐ عدم;
    }

    لكل (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        ܐܢ (keymap_formats_labels[k].format == format)
            ܦܢܐ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ܐܢ (keymap_formats_labels[k].format > format)
            ܦܢܐ عدم;
    }

    ܦܢܐ عدم;
}
