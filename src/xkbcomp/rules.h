/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(ध्रुव संरचना xkb_rmlvo_builder *rmlvo,
                                  संरचना xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(संरचना xkb_context *ctx,
                                ध्रुव संरचना xkb_rule_names *rmlvo,
                                संरचना xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((परिमाण(xkb_layout_index_t) * अक्षर_बिट * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == परिमाण("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
