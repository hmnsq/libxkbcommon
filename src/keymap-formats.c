/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "utils.h"
#समावेश "keymap-formats.h"
#समावेश "util-numbers.h"

/* [WARNING] Must be in ascending order */
अटल स्थिर क्रमागत xkb_keymap_format keymap_formats[] = {
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
काष्ठा format_label {
    स्थिर अक्षर* label;
    क्रमागत xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
अटल स्थिर काष्ठा format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

माप_प्रकार
xkb_keymap_supported_formats(स्थिर क्रमागत xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    वापस ARRAY_आकार(keymap_formats);
}

bool
xkb_keymap_is_supported_format(क्रमागत xkb_keymap_format format)
{
    अगर (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        वापस false;
    }

    क्रम (माप_प्रकार k = 0; k < ARRAY_आकार(keymap_formats); k++) {
        अगर (keymap_formats[k] == format)
            वापस true;
        /* Short-circuit because array is sorted */
        अगर (keymap_formats[k] > format)
            वापस false;
    }

    वापस false;
}

क्रमागत xkb_keymap_format
xkb_keymap_parse_format(स्थिर अक्षर *raw)
{
    अगर (!raw)
        वापस 0;

    uint32_t format = 0;
    अगर (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        वापस (xkb_keymap_is_supported_format(format)) ? format : 0;
    } अन्यथा {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        क्रम (माप_प्रकार k = 0; k < ARRAY_आकार(keymap_formats_labels); k++) {
            अगर (म_भेद(raw, keymap_formats_labels[k].label) == 0)
                वापस keymap_formats_labels[k].format;
        }
        वापस 0;
    }
}

स्थिर अक्षर *
xkb_keymap_get_format_label(क्रमागत xkb_keymap_format format)
{
    अगर (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        वापस शून्य;
    }

    क्रम (माप_प्रकार k = 0; k < ARRAY_आकार(keymap_formats_labels); k++) {
        अगर (keymap_formats_labels[k].format == format)
            वापस keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        अगर (keymap_formats_labels[k].format > format)
            वापस शून्य;
    }

    वापस शून्य;
}
