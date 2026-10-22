/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

حرف *
resolve_locale(بنية xkb_context *ctx, ثابت حرف *locale);

ثابت حرف *
get_xlocaledir_مسیر(بنية xkb_context *ctx);

حرف *
get_xcomposefile_مسیر(بنية xkb_context *ctx);

حرف *
get_xdg_xcompose_file_مسیر(بنية xkb_context *ctx);

حرف *
get_home_xcompose_file_مسیر(بنية xkb_context *ctx);

حرف *
get_locale_compose_file_مسیر(بنية xkb_context *ctx, ثابت حرف *locale);
