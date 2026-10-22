/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                                  ସଂରଚନା xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(ସଂରଚନା xkb_context *ctx,
                                ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo,
                                ସଂରଚନା xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((ଆକାର(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == ଆକାର("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
