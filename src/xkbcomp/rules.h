/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "rmlvo.h"

XKB_EXPORT_వ్యక్తిగత bool
xkb_components_from_rmlvo_builder(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo,
                                  నిర్మాణం xkb_component_పేర్లు *బహిర్గతం,
                                  xkb_layout_సూచీ_t *explicit_layouts);

XKB_EXPORT_వ్యక్తిగత bool
xkb_components_from_నియమాలు_పేర్లు(నిర్మాణం xkb_context *ctx,
                                స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo,
                                నిర్మాణం xkb_component_పేర్లు *బహిర్గతం,
                                xkb_layout_సూచీ_t *explicit_layouts);

/* Maximum length of a layout index string:
 *
 * length = ceiling (bitsize(xkb_layout_index_t) * logBase 10 2)
 *        < ceiling (bitsize(xkb_layout_index_t) * 5 / 16)
 *        < 1 + floor (bitsize(xkb_layout_index_t) * 5 / 16)
 */
#నిర్వచించు MAX_LAYOUT_సూచీ_STR_నిడివి \
        (1 + ((పరిమాణం(xkb_layout_సూచీ_t) * CHAR_BIT * 5) >> 4))
static_assert(MAX_LAYOUT_సూచీ_STR_నిడివి == పరిమాణం("4294967295"), "");

#నిర్వచించు OPTIONS_సమూహం_SPECIFIER_ఉపసర్గ '!'
