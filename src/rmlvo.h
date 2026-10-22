/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "darray.h"

ગણના RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ગણના {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

સંરચના xkb_rmlvo_builder_layout {
    અક્ષર *layout;
    અક્ષર *variant;
};

પ્રકારવ્યાખ્યા darray(સંરચના xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

સંરચના xkb_rmlvo_builder_option {
    અક્ષર *option;
    xkb_layout_mask_t layouts;
};

પ્રકારવ્યાખ્યા darray(સંરચના xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

સંરચના xkb_rmlvo_builder {
    અક્ષર *rules;
    અક્ષર *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    પૂર્ણાંક refcnt;
    સંરચના xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(અચલ સંરચના xkb_rmlvo_builder *builder,
                                 સંરચના xkb_rule_names *rmlvo,
                                 અક્ષર *buf, size_t buf_size);
