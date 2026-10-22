/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "darray.h"

എണ്ണൽ RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

എണ്ണൽ {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

ഘടന xkb_rmlvo_builder_layout {
    അക്ഷരം *layout;
    അക്ഷരം *variant;
};

തരനിർവചനം darray(ഘടന xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ഘടന xkb_rmlvo_builder_option {
    അക്ഷരം *option;
    xkb_layout_mask_t layouts;
};

തരനിർവചനം darray(ഘടന xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ഘടന xkb_rmlvo_builder {
    അക്ഷരം *rules;
    അക്ഷരം *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    പൂർണ്ണസംഖ്യ refcnt;
    ഘടന xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(സ്ഥിരം ഘടന xkb_rmlvo_builder *builder,
                                 ഘടന xkb_rule_names *rmlvo,
                                 അക്ഷരം *buf, size_t buf_size);
