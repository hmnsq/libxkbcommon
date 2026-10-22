/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"

ଗଣନା RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ଗଣନା {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

ସଂରଚନା xkb_rmlvo_builder_layout {
    ଅକ୍ଷର *layout;
    ଅକ୍ଷର *variant;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ସଂରଚନା xkb_rmlvo_builder_option {
    ଅକ୍ଷର *option;
    xkb_layout_mask_t layouts;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ସଂରଚନା xkb_rmlvo_builder {
    ଅକ୍ଷର *rules;
    ଅକ୍ଷର *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt;
    ସଂରଚନା xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *builder,
                                 ସଂରଚନା xkb_rule_names *rmlvo,
                                 ଅକ୍ଷର *buf, size_t buf_size);
