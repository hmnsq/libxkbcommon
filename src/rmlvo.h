/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"

ਗਿਣਤੀ RMLVO {
    RMLVO_ਨਿਯਮਾਂ = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ਗਿਣਤੀ {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL = 0
};

ਬਣਤਰ xkb_rmlvo_builder_layout {
    ਅੱਖਰ *layout;
    ਅੱਖਰ *variant;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ਬਣਤਰ xkb_rmlvo_builder_option {
    ਅੱਖਰ *option;
    xkb_layout_ਨਕਾਬ_t layouts;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ਬਣਤਰ xkb_rmlvo_builder {
    ਅੱਖਰ *ਨਿਯਮਾਂ;
    ਅੱਖਰ *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    ਪੂਰਨ refcnt;
    ਬਣਤਰ xkb_context *ctx;
};

bool
xkb_rmlvo_builder_ਮੰਜ਼ਿਲ_ਨਿਯਮਾਂ_ਨਾਂ(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *builder,
                                 ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo,
                                 ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ);
