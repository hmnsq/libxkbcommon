/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcomp-priv.h"
#شامل "rmlvo.h"

XKB_EXPORT_نجی bool
xkb_components_from_rmlvo_builder(ثابت ساخت xkb_rmlvo_builder *rmlvo,
                                  ساخت xkb_component_نام *برآمد,
                                  xkb_layout_اشاریہ_t *explicit_layouts);

XKB_EXPORT_نجی bool
xkb_components_from_قواعد_نام(ساخت xkb_context *ctx,
                                ثابت ساخت xkb_قاعدہ_نام *rmlvo,
                                ساخت xkb_component_نام *برآمد,
                                xkb_layout_اشاریہ_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#تعریف_کرو MAX_LAYOUT_اشاریہ_STR_لمبائی \
        (1 + ((حجم(xkb_layout_اشاریہ_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_اشاریہ_STR_لمبائی == حجم("4294967295"), "");

#تعریف_کرو OPTIONS_گروہ_SPECIFIER_سابقہ '!'
