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
स्थिर अचर गणना xkb_keymap_format keymap_formats[] = {
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
    अचर वर्ण* label;
    गणना xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
स्थिर अचर संरचना format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

माप_प्रकार
xkb_keymap_supported_formats(अचर गणना xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    फर्काऊ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(गणना xkb_keymap_format format)
{
    यदि (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        फर्काऊ false;
    }

    लागि (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        यदि (keymap_formats[k] == format)
            फर्काऊ true;
        /* Short-circuit because array is sorted */
        यदि (keymap_formats[k] > format)
            फर्काऊ false;
    }

    फर्काऊ false;
}

गणना xkb_keymap_format
xkb_keymap_parse_format(अचर वर्ण *raw)
{
    यदि (!raw)
        फर्काऊ 0;

    uint32_t format = 0;
    यदि (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        फर्काऊ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } अन्यथा {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        लागि (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            यदि (म_भेद(raw, keymap_formats_labels[k].label) == 0)
                फर्काऊ keymap_formats_labels[k].format;
        }
        फर्काऊ 0;
    }
}

अचर वर्ण *
xkb_keymap_get_format_label(गणना xkb_keymap_format format)
{
    यदि (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        फर्काऊ रिक्त;
    }

    लागि (माप_प्रकार k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        यदि (keymap_formats_labels[k].format == format)
            फर्काऊ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        यदि (keymap_formats_labels[k].format > format)
            फर्काऊ रिक्त;
    }

    फर्काऊ रिक्त;
}
