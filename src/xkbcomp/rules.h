/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcomp-priv.h"
#ضمن "rmlvo.h"

XKB_EXPORT_نجی bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_ناں *باہرلا,
                                  xkb_layout_اشاریہ_t *explicit_layouts);

XKB_EXPORT_نجی bool
xkb_components_from_قاعدے_ناں(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعدہ_ناں *rmlvo,
                                بنية xkb_component_ناں *باہرلا,
                                xkb_layout_اشاریہ_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#تعریف_کرو MAX_LAYOUT_اشاریہ_STR_لمائی \
        (1 + ((حجم(xkb_layout_اشاریہ_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_اشاریہ_STR_لمائی == حجم("4294967295"), "");

#تعریف_کرو OPTIONS_گروہ_SPECIFIER_اگیتر '!'
