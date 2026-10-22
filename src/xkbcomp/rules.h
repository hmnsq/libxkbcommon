/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcomp-priv.h"
#ضمن "rmlvo.h"

XKB_EXPORT_خصوصی bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_نامها *بیرون,
                                  xkb_layout_شاخص_t *explicit_layouts);

XKB_EXPORT_خصوصی bool
xkb_components_from_قواعد_نامها(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعده_نامها *rmlvo,
                                بنية xkb_component_نامها *بیرون,
                                xkb_layout_شاخص_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#تعریف_کن MAX_LAYOUT_شاخص_STR_درازی \
        (1 + ((حجم(xkb_layout_شاخص_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_شاخص_STR_درازی == حجم("4294967295"), "");

#تعریف_کن OPTIONS_گروه_SPECIFIER_پیشوند '!'
