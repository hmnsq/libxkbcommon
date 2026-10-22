/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "darray.h"

गणना RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

गणना {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

संरचना xkb_rmlvo_builder_layout {
    वर्ण *layout;
    वर्ण *variant;
};

प्रकारनिर्देश darray(संरचना xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

संरचना xkb_rmlvo_builder_option {
    वर्ण *option;
    xkb_layout_mask_t layouts;
};

प्रकारनिर्देश darray(संरचना xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

संरचना xkb_rmlvo_builder {
    वर्ण *rules;
    वर्ण *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    पूर्णाङ्क refcnt;
    संरचना xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ध्रुव संरचना xkb_rmlvo_builder *builder,
                                 संरचना xkb_rule_names *rmlvo,
                                 वर्ण *buf, माप_प्रकार buf_size);
