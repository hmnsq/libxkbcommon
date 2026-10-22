/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

वर्ण *
resolve_locale(संरचना xkb_context *ctx, अचर वर्ण *locale);

अचर वर्ण *
get_xlocaledir_path(संरचना xkb_context *ctx);

वर्ण *
get_xcomposefile_path(संरचना xkb_context *ctx);

वर्ण *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx);

वर्ण *
get_home_xcompose_file_path(संरचना xkb_context *ctx);

वर्ण *
get_locale_compose_file_path(संरचना xkb_context *ctx, अचर वर्ण *locale);
