/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap-formats.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"

/* [WARNING] Must be in ascending order */
ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ xkb_keymap_format keymap_formats[] = {
    XKB_KEYMAP_FORMAT_ਪਾਠ_V1,
    XKB_KEYMAP_FORMAT_ਪਾਠ_V2,
};

/*
 * Human-friendly formats labels
 *
 * They are aimed to be used in the CLI tools and to be *stable*. While the
 * current labels are simply “v” + corresponding `xkb_keymap_format` values, it
 * may change in the future. Indeed, `xkb_keymap_format` encoding may change
 * (although we avoit it) while the corresponding labels remain unchanged.
 */
ਬਣਤਰ format_label {
    ਅਚਲ ਅੱਖਰ* label;
    ਗਿਣਤੀ xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ਸਥਿਰ ਅਚਲ ਬਣਤਰ format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_ਪਾਠ_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_ਪਾਠ_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_ਪਾਠ_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_ਪਾਠ_V2 },
};

ਮਾਪ_t
xkb_keymap_supported_formats(ਅਚਲ ਗਿਣਤੀ xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ਵਾਪਸ ARRAY_ਮਾਪ(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਜੇ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ਵਾਪਸ false;
    }

    ਲਈ (ਮਾਪ_t k = 0; k < ARRAY_ਮਾਪ(keymap_formats); k++) {
        ਜੇ (keymap_formats[k] == format)
            ਵਾਪਸ true;
        /* Short-circuit because array is sorted */
        ਜੇ (keymap_formats[k] > format)
            ਵਾਪਸ false;
    }

    ਵਾਪਸ false;
}

ਗਿਣਤੀ xkb_keymap_format
xkb_keymap_parse_format(ਅਚਲ ਅੱਖਰ *raw)
{
    ਜੇ (!raw)
        ਵਾਪਸ 0;

    uint32_t format = 0;
    ਜੇ (parse_hex_ਮੰਜ਼ਿਲ_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ਵਾਪਸ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ਨਹੀਂਤਾਂ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        ਲਈ (ਮਾਪ_t k = 0; k < ARRAY_ਮਾਪ(keymap_formats_labels); k++) {
            ਜੇ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ਵਾਪਸ keymap_formats_labels[k].format;
        }
        ਵਾਪਸ 0;
    }
}

ਅਚਲ ਅੱਖਰ *
xkb_keymap_get_format_label(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਜੇ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਲਈ (ਮਾਪ_t k = 0; k < ARRAY_ਮਾਪ(keymap_formats_labels); k++) {
        ਜੇ (keymap_formats_labels[k].format == format)
            ਵਾਪਸ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ਜੇ (keymap_formats_labels[k].format > format)
            ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ਨਿਰਾ;
}
