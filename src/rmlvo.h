/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stdbool.h>

#include "xkbcommon/xkbcommon.h"
#include "darray.h"

ga_nanaa RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ga_nanaa {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

sa_nracanaa xkb_rmlvo_builder_layout {
    var_na *layout;
    var_na *variant;
};

typedef darray(sa_nracanaa xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

sa_nracanaa xkb_rmlvo_builder_option {
    var_na *option;
    xkb_layout_mask_t layouts;
};

typedef darray(sa_nracanaa xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

sa_nracanaa xkb_rmlvo_builder {
    var_na *rules;
    var_na *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    ចំនួនគត់ refcnt;
    sa_nracanaa xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(const sa_nracanaa xkb_rmlvo_builder *builder,
                                 sa_nracanaa xkb_rule_names *rmlvo,
                                 var_na *buf, size_t buf_size);
