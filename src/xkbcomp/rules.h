/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "xkbcomp-priv.h"
#समावेश "rmlvo.h"

XKB_EXPORT_PRIVATE bool
xkb_components_from_rmlvo_builder(स्थिर काष्ठा xkb_rmlvo_builder *rmlvo,
                                  काष्ठा xkb_component_names *out,
                                  xkb_layout_सूचकांक_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(काष्ठा xkb_context *ctx,
                                स्थिर काष्ठा xkb_rule_names *rmlvo,
                                काष्ठा xkb_component_names *out,
                                xkb_layout_सूचकांक_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#घोषणा MAX_LAYOUT_सूचकांक_STR_लंबाई \
        (1 + ((माप(xkb_layout_सूचकांक_t) * अक्षर_बिट * 5) >> 4))
static_assert(MAX_LAYOUT_सूचकांक_STR_लंबाई == माप("4294967295"), "");

#घोषणा OPTIONS_GROUP_SPECIFIER_PREFIX '!'
