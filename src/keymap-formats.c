/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "utils.h"
#అన్తర్భావ "keymap-formats.h"
#అన్తర్భావ "util-numbers.h"

/* [WARNING] Must be in ascending order */
స్థిర స్థిరరాశి గణన xkb_keymap_format keymap_formats[] = {
    XKB_KEYMAP_FORMAT_పాఠ్యం_V1,
    XKB_KEYMAP_FORMAT_పాఠ్యం_V2,
};

/*
 * Human-friendly formats labels
 *
 * They are aimed to be used in the CLI tools and to be *stable*. While the
 * current labels are simply “v” + corresponding `xkb_keymap_format` values, it
 * may change in the future. Indeed, `xkb_keymap_format` encoding may change
 * (although we avoit it) while the corresponding labels remain unchanged.
 */
నిర్మాణం format_label {
    స్థిరరాశి అక్షరం* label;
    గణన xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
స్థిర స్థిరరాశి నిర్మాణం format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_పాఠ్యం_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_పాఠ్యం_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_పాఠ్యం_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_పాఠ్యం_V2 },
};

కొలత_t
xkb_keymap_supported_formats(స్థిరరాశి గణన xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    తిరిగి ARRAY_కొలత(keymap_formats);
}

bool
xkb_keymap_is_supported_format(గణన xkb_keymap_format format)
{
    ఒకవేళ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        తిరిగి false;
    }

    కొరకు (కొలత_t k = 0; k < ARRAY_కొలత(keymap_formats); k++) {
        ఒకవేళ (keymap_formats[k] == format)
            తిరిగి true;
        /* Short-circuit because array is sorted */
        ఒకవేళ (keymap_formats[k] > format)
            తిరిగి false;
    }

    తిరిగి false;
}

గణన xkb_keymap_format
xkb_keymap_parse_format(స్థిరరాశి అక్షరం *raw)
{
    ఒకవేళ (!raw)
        తిరిగి 0;

    uint32_t format = 0;
    ఒకవేళ (parse_hex_గమ్యం_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        తిరిగి (xkb_keymap_is_supported_format(format)) ? format : 0;
    } లేకపోతే {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        కొరకు (కొలత_t k = 0; k < ARRAY_కొలత(keymap_formats_labels); k++) {
            ఒకవేళ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                తిరిగి keymap_formats_labels[k].format;
        }
        తిరిగి 0;
    }
}

స్థిరరాశి అక్షరం *
xkb_keymap_get_format_label(గణన xkb_keymap_format format)
{
    ఒకవేళ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        తిరిగి ఖాళీ;
    }

    కొరకు (కొలత_t k = 0; k < ARRAY_కొలత(keymap_formats_labels); k++) {
        ఒకవేళ (keymap_formats_labels[k].format == format)
            తిరిగి keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ఒకవేళ (keymap_formats_labels[k].format > format)
            తిరిగి ఖాళీ;
    }

    తిరిగి ఖాళీ;
}
