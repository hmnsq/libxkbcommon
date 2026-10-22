/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

বর্ণ *
resolve_locale(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *locale);

ধ্রুব বর্ণ *
get_xlocaledir_path(সংরচনা xkb_context *ctx);

বর্ণ *
get_xcomposefile_path(সংরচনা xkb_context *ctx);

বর্ণ *
get_xdg_xcompose_file_path(সংরচনা xkb_context *ctx);

বর্ণ *
get_home_xcompose_file_path(সংরচনা xkb_context *ctx);

বর্ণ *
get_locale_compose_file_path(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *locale);
