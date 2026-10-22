/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "keymap-formats.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"

/* [WARNING] Must be in ascending order */
ସ୍ଥିର ଅଚଳ ଗଣନା xkb_keymap_format keymap_formats[] = {
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
ସଂରଚନା format_label {
    ଅଚଳ ଅକ୍ଷର* label;
    ଗଣନା xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ସ୍ଥିର ଅଚଳ ସଂରଚନା format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(ଅଚଳ ଗଣନା xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ଫେରନ୍ତୁ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ଗଣନା xkb_keymap_format format)
{
    ଯଦି (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ଫେରନ୍ତୁ false;
    }

    ପାଇଁ (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        ଯଦି (keymap_formats[k] == format)
            ଫେରନ୍ତୁ true;
        /* Short-circuit because array is sorted */
        ଯଦି (keymap_formats[k] > format)
            ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ false;
}

ଗଣନା xkb_keymap_format
xkb_keymap_parse_format(ଅଚଳ ଅକ୍ଷର *raw)
{
    ଯଦି (!raw)
        ଫେରନ୍ତୁ 0;

    uint32_t format = 0;
    ଯଦି (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ଫେରନ୍ତୁ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ଅନ୍ୟଥା {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        ପାଇଁ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            ଯଦି (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ଫେରନ୍ତୁ keymap_formats_labels[k].format;
        }
        ଫେରନ୍ତୁ 0;
    }
}

ଅଚଳ ଅକ୍ଷର *
xkb_keymap_get_format_label(ଗଣନା xkb_keymap_format format)
{
    ଯଦି (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ପାଇଁ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        ଯଦି (keymap_formats_labels[k].format == format)
            ଫେରନ୍ତୁ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ଯଦି (keymap_formats_labels[k].format > format)
            ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}
