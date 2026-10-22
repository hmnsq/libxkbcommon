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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_format keymap_formats[] = {
    XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1,
    XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2,
};

/*
 * Human-friendly formats labels
 *
 * They are aimed to be used in the CLI tools and to be *stable*. While the
 * current labels are simply “v” + corresponding `xkb_keymap_format` values, it
 * may change in the future. Indeed, `xkb_keymap_format` encoding may change
 * (although we avoit it) while the corresponding labels remain unchanged.
 */
ರಚನೆ format_label {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* label;
    ಎಣಿಕೆ xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2 },
};

ಗಾತ್ರ_t
xkb_keymap_supported_formats(ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ಹಿಂತಿರುಗಿ ARRAY_ಗಾತ್ರ(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಆದರೆ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ಹಿಂತಿರುಗಿ false;
    }

    ಗಾಗಿ (ಗಾತ್ರ_t k = 0; k < ARRAY_ಗಾತ್ರ(keymap_formats); k++) {
        ಆದರೆ (keymap_formats[k] == format)
            ಹಿಂತಿರುಗಿ true;
        /* Short-circuit because array is sorted */
        ಆದರೆ (keymap_formats[k] > format)
            ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಎಣಿಕೆ xkb_keymap_format
xkb_keymap_parse_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *raw)
{
    ಆದರೆ (!raw)
        ಹಿಂತಿರುಗಿ 0;

    uint32_t format = 0;
    ಆದರೆ (parse_hex_ಗಮ್ಯ_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ಹಿಂತಿರುಗಿ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        ಗಾಗಿ (ಗಾತ್ರ_t k = 0; k < ARRAY_ಗಾತ್ರ(keymap_formats_labels); k++) {
            ಆದರೆ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ಹಿಂತಿರುಗಿ keymap_formats_labels[k].format;
        }
        ಹಿಂತಿರುಗಿ 0;
    }
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_get_format_label(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಆದರೆ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಗಾಗಿ (ಗಾತ್ರ_t k = 0; k < ARRAY_ಗಾತ್ರ(keymap_formats_labels); k++) {
        ಆದರೆ (keymap_formats_labels[k].format == format)
            ಹಿಂತಿರುಗಿ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ಆದರೆ (keymap_formats_labels[k].format > format)
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
