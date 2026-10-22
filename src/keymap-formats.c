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
    ສົ່ງຄືນ ARRAY_SIZE(keymap_formats);
}

bool
xkb_keymap_is_supported_format(ga_nanaa xkb_keymap_format format)
{
    ຖ້າ (format < keymap_formats[0]) {
        /* Short-circuit because array is sorted */
        ສົ່ງຄືນ false;
    }

    ສຳລັບ (size_t k = 0; k < ARRAY_SIZE(keymap_formats); k++) {
        ຖ້າ (keymap_formats[k] == format)
            ສົ່ງຄືນ true;
        /* Short-circuit because array is sorted */
        ຖ້າ (keymap_formats[k] > format)
            ສົ່ງຄືນ false;
    }

    ສົ່ງຄືນ false;
}

ga_nanaa xkb_keymap_format
xkb_keymap_parse_format(const var_na *raw)
{
    ຖ້າ (!raw)
        ສົ່ງຄືນ 0;

    uint32_t format = 0;
    ຖ້າ (parse_hex_to_uint32_t(raw, SIZE_MAX, &format) > 0) {
        /* Numeric format */
        ສົ່ງຄືນ (xkb_keymap_is_supported_format(format)) ? format : 0;
    } ບໍ່ດັ່ງນັ້ນ {
        /* Parse label: e.g. “xkb_vXXXX” and “vXXX” */
        ສຳລັບ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
            ຖ້າ (strcmp(raw, keymap_formats_labels[k].label) == 0)
                ສົ່ງຄືນ keymap_formats_labels[k].format;
        }
        ສົ່ງຄືນ 0;
    }
}

const var_na *
xkb_keymap_get_format_label(ga_nanaa xkb_keymap_format format)
{
    ຖ້າ (format < keymap_formats_labels[0].format) {
        /* Short-circuit because array is sorted */
        ສົ່ງຄືນ NULL;
    }

    ສຳລັບ (size_t k = 0; k < ARRAY_SIZE(keymap_formats_labels); k++) {
        ຖ້າ (keymap_formats_labels[k].format == format)
            ສົ່ງຄືນ keymap_formats_labels[k].label;
        /* Short-circuit because array is sorted */
        ຖ້າ (keymap_formats_labels[k].format > format)
            ສົ່ງຄືນ NULL;
    }

    ສົ່ງຄືນ NULL;
}
