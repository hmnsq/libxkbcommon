/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "keymap-formats.h"
#અન્તર્ભાવ "util-numbers.h"

/* [WARNING] Must be in ascending order */
સ્થિર અચલ ગણના xkb_keymap_format keymap_formats[] = {
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
સંરચના format_label {
    અચલ અક્ષર* label;
    ગણના xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
સ્થિર અચલ સંરચના format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(અચલ ગણના xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    પાછા ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ગણના xkb_keymap_format format)
{
    જો (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        પાછા false;
    }

    માટે (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        જો (keymap_formats[k] == format)
            પાછા true;
        /* Short-circuit because array is sorted */
        જો (keymap_formats[k] > format)
            પાછા false;
    }

    પાછા false;
}

ગણના xkb_keymap_format
xkb_keymap_parse_format(અચલ અક્ષર *raw)
{
    જો (!raw)
        પાછા 0;

    uint32_t format = 0;
    જો (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        પાછા (xkb_keymap_is_supported_format(format)) ? format : 0;
    } નહીંતર {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        માટે (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            જો (strcmp(raw, keymap_formats_labels[k].label) == 0)
                પાછા keymap_formats_labels[k].format;
        }
        પાછા 0;
    }
}

અચલ અક્ષર *
xkb_keymap_get_format_label(ગણના xkb_keymap_format format)
{
    જો (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        પાછા ખાલી;
    }

    માટે (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        જો (keymap_formats_labels[k].format == format)
            પાછા keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        જો (keymap_formats_labels[k].format > format)
            પાછા ખાલી;
    }

    પાછા ખાલી;
}
