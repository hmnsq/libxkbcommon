/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "keymap-formats.h"
#অন্তর্ভাব "util-numbers.h"

/* [WARNING] Must be in ascending order */
স্থির ধ্রুবক গণনা xkb_keymap_format keymap_formats[] = {
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
কাঠামো format_label {
    ধ্রুবক অক্ষর* label;
    গণনা xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
স্থির ধ্রুবক কাঠামো format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

আকার_t
xkb_keymap_supported_formats(ধ্রুবক গণনা xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ফেরত ARRAY_আকার(keymap_formats);
}

bool
xkb_keymap_is_supported_format(গণনা xkb_keymap_format format)
{
    যদি (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ফেরত false;
    }

    জন্য (আকার_t k = 0; k < ARRAY_আকার(keymap_formats); k++) {
        যদি (keymap_formats[k] == format)
            ফেরত true;
        /* Short-circuit because array is sorted */
        যদি (keymap_formats[k] > format)
            ফেরত false;
    }

    ফেরত false;
}

গণনা xkb_keymap_format
xkb_keymap_parse_format(ধ্রুবক অক্ষর *raw)
{
    যদি (!raw)
        ফেরত 0;

    uint32_t format = 0;
    যদি (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ফেরত (xkb_keymap_is_supported_format(format)) ? format : 0;
    } অন্যথা {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        জন্য (আকার_t k = 0; k < ARRAY_আকার(keymap_formats_labels); k++) {
            যদি (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ফেরত keymap_formats_labels[k].format;
        }
        ফেরত 0;
    }
}

ধ্রুবক অক্ষর *
xkb_keymap_get_format_label(গণনা xkb_keymap_format format)
{
    যদি (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ফেরত ফাঁকা;
    }

    জন্য (আকার_t k = 0; k < ARRAY_আকার(keymap_formats_labels); k++) {
        যদি (keymap_formats_labels[k].format == format)
            ফেরত keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        যদি (keymap_formats_labels[k].format > format)
            ফেরত ফাঁকা;
    }

    ফেরত ফাঁকা;
}
