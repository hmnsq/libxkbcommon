/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_components_from_rmlvo_builder(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo,
                                  ರಚನೆ xkb_component_ಹೆಸರುಗಳು *ಹೊರಹೋಗು,
                                  xkb_layout_ಸೂಚ್ಯಂಕ_t *explicit_layouts);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_components_from_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
                                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo,
                                ರಚನೆ xkb_component_ಹೆಸರುಗಳು *ಹೊರಹೋಗು,
                                xkb_layout_ಸೂಚ್ಯಂಕ_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_LAYOUT_ಸೂಚ್ಯಂಕ_STR_ಉದ್ದಳತೆ \
        (1 + ((ಗಾತ್ರ(xkb_layout_ಸೂಚ್ಯಂಕ_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_ಸೂಚ್ಯಂಕ_STR_ಉದ್ದಳತೆ == ಗಾತ್ರ("4294967295"), "");

#ವ್ಯಾಖ್ಯಾನಿಸು OPTIONS_ಗುಂಪು_SPECIFIER_ಪೂರ್ವಪ್ರತ್ಯಯ '!'
