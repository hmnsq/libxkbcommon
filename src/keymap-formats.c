/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <stdint.h>
#شامل <string.h>

#شامل "utils.h"
#شامل "keymap-formats.h"
#شامل "util-numbers.h"

/* [WARNING] Must be in ascending order */
ایستا ثابت شمارش xkb_keymap_format keymap_formats[] = {
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
ساختار format_label {
    ثابت نویسه* label;
    شمارش xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ایستا ثابت ساختار format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(ثابت شمارش xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    بازگشت ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(شمارش xkb_keymap_format format)
{
    اگر (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        بازگشت false;
    }

    برای (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        اگر (keymap_formats[k] == format)
            بازگشت true;
        /* Short-circuit because array is sorted */
        اگر (keymap_formats[k] > format)
            بازگشت false;
    }

    بازگشت false;
}

شمارش xkb_keymap_format
xkb_keymap_parse_format(ثابت نویسه *raw)
{
    اگر (!raw)
        بازگشت 0;

    uint32_t format = 0;
    اگر (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        بازگشت (xkb_keymap_is_supported_format(format)) ? format : 0;
    } وگرنه {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        برای (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            اگر (strcmp(raw, keymap_formats_labels[k].label) == 0)
                بازگشت keymap_formats_labels[k].format;
        }
        بازگشت 0;
    }
}

ثابت نویسه *
xkb_keymap_get_format_label(شمارش xkb_keymap_format format)
{
    اگر (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        بازگشت پوچ;
    }

    برای (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        اگر (keymap_formats_labels[k].format == format)
            بازگشت keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        اگر (keymap_formats_labels[k].format > format)
            بازگشت پوچ;
    }

    بازگشت پوچ;
}
