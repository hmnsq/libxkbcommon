/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "keymap-formats.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"

/* [WARNING] Must be in ascending order */
ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ xkb_keymap_format keymap_formats[] = {
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
ಸಂರಚನಾ format_label {
    ಧ್ರುವ ವರ್ಣ* label;
    ಗಣನಾ xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(ಧ್ರುವ ಗಣನಾ xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ಪ್ರತ್ಯಾಗಮ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ಗಣನಾ xkb_keymap_format format)
{
    ಯದಿ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತಿ (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        ಯದಿ (keymap_formats[k] == format)
            ಪ್ರತ್ಯಾಗಮ true;
        /* Short-circuit because array is sorted */
        ಯದಿ (keymap_formats[k] > format)
            ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಗಣನಾ xkb_keymap_format
xkb_keymap_parse_format(ಧ್ರುವ ವರ್ಣ *raw)
{
    ಯದಿ (!raw)
        ಪ್ರತ್ಯಾಗಮ 0;

    uint32_t format = 0;
    ಯದಿ (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ಪ್ರತ್ಯಾಗಮ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ಅನ್ಯಥಾ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        ಪ್ರತಿ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            ಯದಿ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ಪ್ರತ್ಯಾಗಮ keymap_formats_labels[k].format;
        }
        ಪ್ರತ್ಯಾಗಮ 0;
    }
}

ಧ್ರುವ ವರ್ಣ *
xkb_keymap_get_format_label(ಗಣನಾ xkb_keymap_format format)
{
    ಯದಿ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತಿ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        ಯದಿ (keymap_formats_labels[k].format == format)
            ಪ್ರತ್ಯಾಗಮ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ಯದಿ (keymap_formats_labels[k].format > format)
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}
