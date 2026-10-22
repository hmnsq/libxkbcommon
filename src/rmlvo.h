/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"

شمار RMLVO {
    RMLVO_قواعد = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

شمار {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_نقاب_GLOBAL = 0
};

ساخت xkb_rmlvo_builder_layout {
    حرف *layout;
    حرف *variant;
};

قسمتعریف darray(ساخت xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ساخت xkb_rmlvo_builder_option {
    حرف *option;
    xkb_layout_نقاب_t layouts;
};

قسمتعریف darray(ساخت xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ساخت xkb_rmlvo_builder {
    حرف *قواعد;
    حرف *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    صحیح refcnt;
    ساخت xkb_context *ctx;
};

bool
xkb_rmlvo_builder_مقصد_قواعد_نام(ثابت ساخت xkb_rmlvo_builder *builder,
                                 ساخت xkb_قاعدہ_نام *rmlvo,
                                 حرف *buf, سائز_t buf_سائز);
