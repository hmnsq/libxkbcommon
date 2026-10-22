/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "darray.h"

எண்ணிக்கை RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

எண்ணிக்கை {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

கட்டமைப்பு xkb_rmlvo_builder_layout {
    எழுத்து *layout;
    எழுத்து *variant;
};

வகைவரை darray(கட்டமைப்பு xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

கட்டமைப்பு xkb_rmlvo_builder_option {
    எழுத்து *option;
    xkb_layout_mask_t layouts;
};

வகைவரை darray(கட்டமைப்பு xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

கட்டமைப்பு xkb_rmlvo_builder {
    எழுத்து *rules;
    எழுத்து *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    முழு refcnt;
    கட்டமைப்பு xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *builder,
                                 கட்டமைப்பு xkb_rule_names *rmlvo,
                                 எழுத்து *buf, size_t buf_size);
