/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "keymap-formats.h"
#അന്തര്ഭാവ "util-numbers.h"

/* [WARNING] Must be in ascending order */
സ്ഥിതം സ്ഥിരം എണ്ണൽ xkb_keymap_format keymap_formats[] = {
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
ഘടന format_label {
    സ്ഥിരം അക്ഷരം* label;
    എണ്ണൽ xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
സ്ഥിതം സ്ഥിരം ഘടന format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(സ്ഥിരം എണ്ണൽ xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    തിരികെ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(എണ്ണൽ xkb_keymap_format format)
{
    എങ്കിൽ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        തിരികെ false;
    }

    വേണ്ടി (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        എങ്കിൽ (keymap_formats[k] == format)
            തിരികെ true;
        /* Short-circuit because array is sorted */
        എങ്കിൽ (keymap_formats[k] > format)
            തിരികെ false;
    }

    തിരികെ false;
}

എണ്ണൽ xkb_keymap_format
xkb_keymap_parse_format(സ്ഥിരം അക്ഷരം *raw)
{
    എങ്കിൽ (!raw)
        തിരികെ 0;

    uint32_t format = 0;
    എങ്കിൽ (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        തിരികെ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } അല്ലെങ്കിൽ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        വേണ്ടി (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            എങ്കിൽ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                തിരികെ keymap_formats_labels[k].format;
        }
        തിരികെ 0;
    }
}

സ്ഥിരം അക്ഷരം *
xkb_keymap_get_format_label(എണ്ണൽ xkb_keymap_format format)
{
    എങ്കിൽ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        തിരികെ ശൂന്യ;
    }

    വേണ്ടി (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        എങ്കിൽ (keymap_formats_labels[k].format == format)
            തിരികെ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        എങ്കിൽ (keymap_formats_labels[k].format > format)
            തിരികെ ശൂന്യ;
    }

    തിരികെ ശൂന്യ;
}
