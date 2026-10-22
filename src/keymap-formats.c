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
ساکن ثابت شمار xkb_keymap_format keymap_formats[] = {
    XKB_KEYMAP_FORMAT_متن_V1,
    XKB_KEYMAP_FORMAT_متن_V2,
};

/*
 * Human-friendly formats labels
 *
 * They are aimed to be used in the CLI tools and to be *stable*. While the
 * current labels are simply “v” + corresponding `xkb_keymap_format` values, it
 * may change in the future. Indeed, `xkb_keymap_format` encoding may change
 * (although we avoit it) while the corresponding labels remain unchanged.
 */
ساخت format_label {
    ثابت حرف* label;
    شمار xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ساکن ثابت ساخت format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_متن_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_متن_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_متن_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_متن_V2 },
};

سائز_t
xkb_keymap_supported_formats(ثابت شمار xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    واپس ARRAY_سائز(keymap_formats);
}

bool
xkb_keymap_is_supported_format(شمار xkb_keymap_format format)
{
    اگر (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        واپس false;
    }

    برائے (سائز_t k = 0; k < ARRAY_سائز(keymap_formats); k++) {
        اگر (keymap_formats[k] == format)
            واپس true;
        /* Short-circuit because array is sorted */
        اگر (keymap_formats[k] > format)
            واپس false;
    }

    واپس false;
}

شمار xkb_keymap_format
xkb_keymap_parse_format(ثابت حرف *raw)
{
    اگر (!raw)
        واپس 0;

    uint32_t format = 0;
    اگر (parse_hex_مقصد_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        واپس (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ورنہ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        برائے (سائز_t k = 0; k < ARRAY_سائز(keymap_formats_labels); k++) {
            اگر (strcmp(raw, keymap_formats_labels[k].label) == 0)
                واپس keymap_formats_labels[k].format;
        }
        واپس 0;
    }
}

ثابت حرف *
xkb_keymap_get_format_label(شمار xkb_keymap_format format)
{
    اگر (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        واپس تہی;
    }

    برائے (سائز_t k = 0; k < ARRAY_سائز(keymap_formats_labels); k++) {
        اگر (keymap_formats_labels[k].format == format)
            واپس keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        اگر (keymap_formats_labels[k].format > format)
            واپس تہی;
    }

    واپس تہی;
}
