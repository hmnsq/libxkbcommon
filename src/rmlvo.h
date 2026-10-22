/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"

تعداد RMLVO {
    RMLVO_قواعد = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

تعداد {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_ماسک_GLOBAL = 0
};

بنية xkb_rmlvo_builder_layout {
    حرف *layout;
    حرف *variant;
};

تعريفنوع darray(بنية xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

بنية xkb_rmlvo_builder_option {
    حرف *option;
    xkb_layout_ماسک_t layouts;
};

تعريفنوع darray(بنية xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

بنية xkb_rmlvo_builder {
    حرف *قواعد;
    حرف *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    صحيح refcnt;
    بنية xkb_context *ctx;
};

bool
xkb_rmlvo_builder_مقصد_قواعد_نامها(ثابت بنية xkb_rmlvo_builder *builder,
                                 بنية xkb_قاعده_نامها *rmlvo,
                                 حرف *buf, اندازه_t buf_اندازه);
