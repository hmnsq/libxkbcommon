/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "keymap-formats.h"
#include "util-numbers.h"

/* [WARNING] Must be in ascending order */
static const ga_nanaa xkb_keymap_format keymap_formats[] = {
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
sa_nracanaa format_label {
    const var_na* label;
    ga_nanaa xkb_keymap_format format;
};
/* [WARNING] Must be in ascending order of the keymap format. */
static const sa_nracanaa format_label keymap_formats_labels [] = {
    { "xkb_v1", XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "v1",     XKB_KEYMAP_FORMAT_TEXT_V1 },
    { "xkb_v2", XKB_KEYMAP_FORMAT_TEXT_V2 },
    { "v2",     XKB_KEYMAP_FORMAT_TEXT_V2 },
};

size_t
xkb_keymap_supported_formats(const ga_nanaa xkb_keymap_format **formats)
{
    *formats = keymap_formats;
    ត្រឡប់ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ga_nanaa xkb_keymap_format format)
{
    ប្រសិនបើ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ត្រឡប់ false;
    }

    សម្រាប់ (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        ប្រសិនបើ (keymap_formats[k] == format)
            ត្រឡប់ true;
        /* Short-circuit because array is sorted */
        ប្រសិនបើ (keymap_formats[k] > format)
            ត្រឡប់ false;
    }

    ត្រឡប់ false;
}

ga_nanaa xkb_keymap_format
xkb_keymap_parse_format(const var_na *raw)
{
    ប្រសិនបើ (!raw)
        ត្រឡប់ 0;

    uint32_t format = 0;
    ប្រសិនបើ (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ត្រឡប់ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } បើមិនដូច្នេះ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        សម្រាប់ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            ប្រសិនបើ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ត្រឡប់ keymap_formats_labels[k].format;
        }
        ត្រឡប់ 0;
    }
}

const var_na *
xkb_keymap_get_format_label(ga_nanaa xkb_keymap_format format)
{
    ប្រសិនបើ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ត្រឡប់ NULL;
    }

    សម្រាប់ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        ប្រសិនបើ (keymap_formats_labels[k].format == format)
            ត្រឡប់ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ប្រសិនបើ (keymap_formats_labels[k].format > format)
            ត្រឡប់ NULL;
    }

    ត្រឡប់ NULL;
}
