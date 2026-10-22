/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "utils.h"
#अन्तर्भाव "keymap-formats.h"
#अन्तर्भाव "util-numbers.h"

/* [WARNING] Must be in ascending order */
स्थिर ध्रुव गणना xkb_keymap_format keymap_formats[] = {
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
संरचना format_label {
    ध्रुव वर्ण* label;
    गणना xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
स्थिर ध्रुव संरचना format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(ध्रुव गणना xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    प्रत्यागम ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(गणना xkb_keymap_format format)
{
    यदि (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        प्रत्यागम false;
    }

    प्रति (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        यदि (keymap_formats[k] == format)
            प्रत्यागम true;
        /* Short-circuit because array is sorted */
        यदि (keymap_formats[k] > format)
            प्रत्यागम false;
    }

    प्रत्यागम false;
}

गणना xkb_keymap_format
xkb_keymap_parse_format(ध्रुव वर्ण *raw)
{
    यदि (!raw)
        प्रत्यागम 0;

    uint32_t format = 0;
    यदि (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        प्रत्यागम (xkb_keymap_is_supported_format(format)) ? format : 0;
    } अन्यथा {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        प्रति (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            यदि (strcmp(raw, keymap_formats_labels[k].label) == 0)
                प्रत्यागम keymap_formats_labels[k].format;
        }
        प्रत्यागम 0;
    }
}

ध्रुव वर्ण *
xkb_keymap_get_format_label(गणना xkb_keymap_format format)
{
    यदि (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        प्रत्यागम रिक्त;
    }

    प्रति (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        यदि (keymap_formats_labels[k].format == format)
            प्रत्यागम keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        यदि (keymap_formats_labels[k].format > format)
            प्रत्यागम रिक्त;
    }

    प्रत्यागम रिक्त;
}
