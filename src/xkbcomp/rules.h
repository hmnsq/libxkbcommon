/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_components_from_rmlvo_builder(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                                  ਬਣਤਰ xkb_component_ਨਾਂ *ਬਾਹਰਲਾ,
                                  xkb_layout_ਸੂਚਕਾਂਕ_t *explicit_layouts);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_components_from_ਨਿਯਮਾਂ_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
                                ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo,
                                ਬਣਤਰ xkb_component_ਨਾਂ *ਬਾਹਰਲਾ,
                                xkb_layout_ਸੂਚਕਾਂਕ_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_LAYOUT_ਸੂਚਕਾਂਕ_STR_ਲੰਮਾਈ \
        (1 + ((ਆਕਾਰ(xkb_layout_ਸੂਚਕਾਂਕ_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_ਸੂਚਕਾਂਕ_STR_ਲੰਮਾਈ == ਆਕਾਰ("4294967295"), "");

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPTIONS_ਸਮੂਹ_SPECIFIER_ਅਗੇਤਰ '!'
