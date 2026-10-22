/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "darray.h"

গণনা RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

গণনা {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

সংরচনা xkb_rmlvo_builder_layout {
    বর্ণ *layout;
    বর্ণ *variant;
};

প্রকারনির্দেশ darray(সংরচনা xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

সংরচনা xkb_rmlvo_builder_option {
    বর্ণ *option;
    xkb_layout_mask_t layouts;
};

প্রকারনির্দেশ darray(সংরচনা xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

সংরচনা xkb_rmlvo_builder {
    বর্ণ *rules;
    বর্ণ *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    পূর্ণাঙ্ক refcnt;
    সংরচনা xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ধ্রুব সংরচনা xkb_rmlvo_builder *builder,
                                 সংরচনা xkb_rule_names *rmlvo,
                                 বর্ণ *buf, size_t buf_size);
