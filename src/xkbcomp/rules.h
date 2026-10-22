/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                                  கட்டமைப்பு xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(கட்டமைப்பு xkb_context *ctx,
                                மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo,
                                கட்டமைப்பு xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((அளவு(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == அளவு("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
