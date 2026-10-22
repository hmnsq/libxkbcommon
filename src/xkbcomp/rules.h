/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcomp-priv.h"
#شامل "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(ثابت ساختار xkb_rmlvo_builder *rmlvo,
                                  ساختار xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(ساختار xkb_context *ctx,
                                ثابت ساختار xkb_rule_names *rmlvo,
                                ساختار xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((اندازه(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == اندازه("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
