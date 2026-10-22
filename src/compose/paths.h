/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

वर्ण *
resolve_locale(संरचना xkb_context *ctx, ध्रुव वर्ण *locale);

ध्रुव वर्ण *
get_xlocaledir_मार्गः(संरचना xkb_context *ctx);

वर्ण *
get_xcomposefile_मार्गः(संरचना xkb_context *ctx);

वर्ण *
get_xdg_xcompose_file_मार्गः(संरचना xkb_context *ctx);

वर्ण *
get_home_xcompose_file_मार्गः(संरचना xkb_context *ctx);

वर्ण *
get_locale_compose_file_मार्गः(संरचना xkb_context *ctx, ध्रुव वर्ण *locale);
