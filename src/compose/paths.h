/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

అక్షరం *
resolve_locale(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *locale);

స్థిరరాశి అక్షరం *
get_xlocaledir_మార్గం(నిర్మాణం xkb_context *ctx);

అక్షరం *
get_xcomposefile_మార్గం(నిర్మాణం xkb_context *ctx);

అక్షరం *
get_xdg_xcompose_file_మార్గం(నిర్మాణం xkb_context *ctx);

అక్షరం *
get_home_xcompose_file_మార్గం(నిర్మాణం xkb_context *ctx);

అక్షరం *
get_locale_compose_file_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *locale);
