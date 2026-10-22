/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo,
                                  ഘടന xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(ഘടന xkb_context *ctx,
                                സ്ഥിരം ഘടന xkb_rule_names *rmlvo,
                                ഘടന xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((വലുപ്പം(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == വലുപ്പം("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
