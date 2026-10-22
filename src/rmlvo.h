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

কাঠামো xkb_rmlvo_builder_layout {
    অক্ষর *layout;
    অক্ষর *variant;
};

প্রকারলেখ darray(কাঠামো xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

কাঠামো xkb_rmlvo_builder_option {
    অক্ষর *option;
    xkb_layout_mask_t layouts;
};

প্রকারলেখ darray(কাঠামো xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

কাঠামো xkb_rmlvo_builder {
    অক্ষর *rules;
    অক্ষর *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    পূর্ণসংখ্যা refcnt;
    কাঠামো xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(ধ্রুবক কাঠামো xkb_rmlvo_builder *builder,
                                 কাঠামো xkb_rule_names *rmlvo,
                                 অক্ষর *buf, আকার_t buf_আকার);
