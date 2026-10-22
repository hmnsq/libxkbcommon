/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "darray.h"

ಗಣನಾ RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ಗಣನಾ {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

ಸಂರಚನಾ xkb_rmlvo_builder_layout {
    ವರ್ಣ *layout;
    ವರ್ಣ *variant;
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ಸಂರಚನಾ xkb_rmlvo_builder_option {
    ವರ್ಣ *option;
    xkb_layout_mask_t layouts;
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ಸಂರಚನಾ xkb_rmlvo_builder {
    ವರ್ಣ *rules;
    ವರ್ಣ *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    ಪೂರ್ಣಾಙ್ಕ refcnt;
    ಸಂರಚನಾ xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *builder,
                                 ಸಂರಚನಾ xkb_rule_names *rmlvo,
                                 ವರ್ಣ *buf, size_t buf_size);
