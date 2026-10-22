/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo,
                                  ಸಂರಚನಾ xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(ಸಂರಚನಾ xkb_context *ctx,
                                ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *rmlvo,
                                ಸಂರಚನಾ xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_INDEX_STR_LENGTH \
        (1 + ((ಪರಿಮಾಣ(xkb_layout_index_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_INDEX_STR_LENGTH == ಪರಿಮಾಣ("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
