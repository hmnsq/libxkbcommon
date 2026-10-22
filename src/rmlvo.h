/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"

شمارش RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

شمارش {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

ساختار xkb_rmlvo_builder_layout {
    نویسه *layout;
    نویسه *variant;
};

تعریفنوع darray(ساختار xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ساختار xkb_rmlvo_builder_option {
    نویسه *option;
    xkb_layout_mask_t layouts;
};

تعریفنوع darray(ساختار xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ساختار xkb_rmlvo_builder {
    نویسه *rules;
    نویسه *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    صحیح refcnt;
    ساختار xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ثابت ساختار xkb_rmlvo_builder *builder,
                                 ساختار xkb_rule_names *rmlvo,
                                 نویسه *buf, size_t buf_size);
