/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "darray.h"

గణన RMLVO {
    RMLVO_నియమాలు = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

గణన {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_ముసుగు_GLOBAL = 0
};

నిర్మాణం xkb_rmlvo_builder_layout {
    అక్షరం *layout;
    అక్షరం *variant;
};

రకనిర్వచనం darray(నిర్మాణం xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

నిర్మాణం xkb_rmlvo_builder_option {
    అక్షరం *option;
    xkb_layout_ముసుగు_t layouts;
};

రకనిర్వచనం darray(నిర్మాణం xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

నిర్మాణం xkb_rmlvo_builder {
    అక్షరం *నియమాలు;
    అక్షరం *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    పూర్ణాంకం refcnt;
    నిర్మాణం xkb_context *ctx;
};

bool
xkb_rmlvo_builder_గమ్యం_నియమాలు_పేర్లు(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *builder,
                                 నిర్మాణం xkb_నియమం_పేర్లు *rmlvo,
                                 అక్షరం *buf, కొలత_t buf_కొలత);
