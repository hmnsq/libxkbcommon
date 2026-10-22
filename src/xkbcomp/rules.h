/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(ধ্রুব সংরচনা xkb_rmlvo_builder *rmlvo,
                                  সংরচনা xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(সংরচনা xkb_context *ctx,
                                ধ্রুব সংরচনা xkb_rule_names *rmlvo,
                                সংরচনা xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((পরিমাণ(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == পরিমাণ("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
