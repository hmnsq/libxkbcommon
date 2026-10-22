/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "keymap-formats.h"
#அந்தர்பாவ "util-numbers.h"

/* [WARNING] Must be in ascending order */
நிலையான மாறிலி எண்ணிக்கை xkb_keymap_format keymap_formats[] = {
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
கட்டமைப்பு format_label {
    மாறிலி எழுத்து* label;
    எண்ணிக்கை xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
நிலையான மாறிலி கட்டமைப்பு format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(மாறிலி எண்ணிக்கை xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    திரும்பு ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(எண்ணிக்கை xkb_keymap_format format)
{
    எனில் (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        திரும்பு false;
    }

    க்கு (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        எனில் (keymap_formats[k] == format)
            திரும்பு true;
        /* Short-circuit because array is sorted */
        எனில் (keymap_formats[k] > format)
            திரும்பு false;
    }

    திரும்பு false;
}

எண்ணிக்கை xkb_keymap_format
xkb_keymap_parse_format(மாறிலி எழுத்து *raw)
{
    எனில் (!raw)
        திரும்பு 0;

    uint32_t format = 0;
    எனில் (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        திரும்பு (xkb_keymap_is_supported_format(format)) ? format : 0;
    } இல்லையெனில் {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        க்கு (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            எனில் (strcmp(raw, keymap_formats_labels[k].label) == 0)
                திரும்பு keymap_formats_labels[k].format;
        }
        திரும்பு 0;
    }
}

மாறிலி எழுத்து *
xkb_keymap_get_format_label(எண்ணிக்கை xkb_keymap_format format)
{
    எனில் (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        திரும்பு வெற்று;
    }

    க்கு (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        எனில் (keymap_formats_labels[k].format == format)
            திரும்பு keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        எனில் (keymap_formats_labels[k].format > format)
            திரும்பு வெற்று;
    }

    திரும்பு வெற்று;
}
