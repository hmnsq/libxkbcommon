/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "darray.h"

क्रमागत RMLVO {
    RMLVO_RULES = (1 << 0),
    RMLVO_MODEL = (1 << 1),
    RMLVO_LAYOUT = (1 << 2),
    RMLVO_VARIANT = (1 << 3),
    RMLVO_OPTIONS = (1 << 4)
};

क्रमागत {
    /** Value of the layout mask for layout-independent options */
    XKB_OPTION_LAYOUT_MASK_GLOBAL = 0
};

काष्ठा xkb_rmlvo_builder_layout {
    अक्षर *layout;
    अक्षर *variant;
};

प्रकार darray(काष्ठा xkb_rmlvo_builder_layout) xkb_rmlvo_builder_layouts;

काष्ठा xkb_rmlvo_builder_option {
    अक्षर *option;
    xkb_layout_mask_t layouts;
};

प्रकार darray(काष्ठा xkb_rmlvo_builder_option) xkb_rmlvo_builder_options;

काष्ठा xkb_rmlvo_builder {
    अक्षर *rules;
    अक्षर *model;
    xkb_rmlvo_builder_layouts layouts;
    xkb_rmlvo_builder_options options;

    पूर्णांक refcnt;
    काष्ठा xkb_context *ctx;
};

bool
xkb_rmlvo_builder_to_rules_names(स्थिर काष्ठा xkb_rmlvo_builder *builder,
                                 काष्ठा xkb_rule_names *rmlvo,
                                 अक्षर *buf, माप_प्रकार buf_आकार);
