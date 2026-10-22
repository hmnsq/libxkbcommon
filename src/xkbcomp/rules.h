/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcomp-priv.h"
#ضمن "rmlvo.h"

XKB_EXPORT_ذاتي bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_نالا *ٻاهريون,
                                  xkb_layout_اشاريو_t *explicit_layouts);

XKB_EXPORT_ذاتي bool
xkb_components_from_قاعدا_نالا(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعدو_نالا *rmlvo,
                                بنية xkb_component_نالا *ٻاهريون,
                                xkb_layout_اشاريو_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#وصف_ڪريو MAX_LAYOUT_اشاريو_STR_ڊيگهائي \
        (1 + ((حجم(xkb_layout_اشاريو_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_اشاريو_STR_ڊيگهائي == حجم("4294967295"), "");

#وصف_ڪريو OPTIONS_گروهه_SPECIFIER_اڳياڙي '!'
