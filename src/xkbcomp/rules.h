/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "rmlvo.h"

XKB_EXPORT_वैयक्तिकम् bool
xkb_components_from_rmlvo_builder(ध्रुव संरचना xkb_rmlvo_builder *rmlvo,
                                  संरचना xkb_component_नामानि *बहिर्गमनम्,
                                  xkb_layout_सूचकांक_t *explicit_layouts);

XKB_EXPORT_वैयक्तिकम् bool
xkb_components_from_नियमाः_नामानि(संरचना xkb_context *ctx,
                                ध्रुव संरचना xkb_नियमः_नामानि *rmlvo,
                                संरचना xkb_component_नामानि *बहिर्गमनम्,
                                xkb_layout_सूचकांक_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#परिभाषय MAX_LAYOUT_सूचकांक_STR_दीर्घता \
        (1 + ((परिमाण(xkb_layout_सूचकांक_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_सूचकांक_STR_दीर्घता == परिमाण("4294967295"), "");

#परिभाषय OPTIONS_गणः_SPECIFIER_उपसर्गः '!'
