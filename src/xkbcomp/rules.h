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
xkb_components_from_rmlvo_builder(ধ্রুবক কাঠামো xkb_rmlvo_builder *rmlvo,
                                  কাঠামো xkb_component_names *out,
                                  xkb_layout_সূচক_t *explicit_layouts);

XKB_EXPORT_PRIVATE bool
xkb_components_from_rules_names(কাঠামো xkb_context *ctx,
                                ধ্রুবক কাঠামো xkb_rule_names *rmlvo,
                                কাঠামো xkb_component_names *out,
                                xkb_layout_সূচক_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#define MAX_LAYOUT_সূচক_STR_দৈর্ঘ্য \
        (1 + ((আকার(xkb_layout_সূচক_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_সূচক_STR_দৈর্ঘ্য == আকার("4294967295"), "");

#define OPTIONS_GROUP_SPECIFIER_PREFIX '!'
