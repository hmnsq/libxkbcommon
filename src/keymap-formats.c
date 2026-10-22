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
स्थिर स्थिरांक गणना xkb_keymap_format keymap_formats[] = {
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
    स्थिरांक अक्षर* label;
    गणना xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
स्थिर स्थिरांक संरचना format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

माप_प्रकार
xkb_keymap_supported_formats(स्थिरांक गणना xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    परत ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(गणना xkb_keymap_format format)
{
    जर (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        परत false;
    }

    साठी (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        जर (keymap_formats[k] == format)
            परत true;
        /* Short-circuit because array is sorted */
        जर (keymap_formats[k] > format)
            परत false;
    }

    परत false;
}

गणना xkb_keymap_format
xkb_keymap_parse_format(स्थिरांक अक्षर *raw)
{
    जर (!raw)
        परत 0;

    uint32_t format = 0;
    जर (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        परत (xkb_keymap_is_supported_format(format)) ? format : 0;
    } अन्यथा {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        साठी (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            जर (म_भेद(raw, keymap_formats_labels[k].label) == 0)
                परत keymap_formats_labels[k].format;
        }
        परत 0;
    }
}

स्थिरांक अक्षर *
xkb_keymap_get_format_label(गणना xkb_keymap_format format)
{
    जर (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        परत रिक्त;
    }

    साठी (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        जर (keymap_formats_labels[k].format == format)
            परत keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        जर (keymap_formats_labels[k].format > format)
            परत रिक्त;
    }

    परत रिक्त;
}
