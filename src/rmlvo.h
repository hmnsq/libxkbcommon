/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "darray.h"

ಎಣಿಕೆ RMLVO {
    RMLVO_ನಿಯಮಗಳು = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

ಎಣಿಕೆ {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL = 0
};

ರಚನೆ xkb_rmlvo_builder_layout {
    ಅಕ್ಷರ *layout;
    ಅಕ್ಷರ *variant;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

ರಚನೆ xkb_rmlvo_builder_option {
    ಅಕ್ಷರ *option;
    xkb_layout_ಮುಖವಾಡ_t layouts;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

ರಚನೆ xkb_rmlvo_builder {
    ಅಕ್ಷರ *ನಿಯಮಗಳು;
    ಅಕ್ಷರ *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    ಪೂರ್ಣಾಂಕ refcnt;
    ರಚನೆ xkb_context *ctx;
};

bool
xkb_rmlvo_builder_ಗಮ್ಯ_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *builder,
                                 ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo,
                                 ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ);
