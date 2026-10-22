/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-compose.h"

#అన్తర్భావ "src/utils.h"

XKB_EXPORT_వ్యక్తిగత అక్షరం *
parse_string_literal(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string);

bool
parse_string(నిర్మాణం xkb_compose_పట్టిక *పట్టిక,
             స్థిరరాశి అక్షరం *string, కొలత_t len,
             స్థిరరాశి అక్షరం *file_name);

bool
parse_file(నిర్మాణం xkb_compose_పట్టిక *పట్టిక,
           FILE *file, స్థిరరాశి అక్షరం *file_name);
