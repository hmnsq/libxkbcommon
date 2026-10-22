/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

حرف *
resolve_locale(ساخت xkb_context *ctx, ثابت حرف *locale);

ثابت حرف *
get_xlocaledir_راستہ(ساخت xkb_context *ctx);

حرف *
get_xcomposefile_راستہ(ساخت xkb_context *ctx);

حرف *
get_xdg_xcompose_file_راستہ(ساخت xkb_context *ctx);

حرف *
get_home_xcompose_file_راستہ(ساخت xkb_context *ctx);

حرف *
get_locale_compose_file_راستہ(ساخت xkb_context *ctx, ثابت حرف *locale);
