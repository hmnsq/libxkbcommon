/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

অক্ষর *
resolve_locale(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *locale);

ধ্রুবক অক্ষর *
get_xlocaledir_path(কাঠামো xkb_context *ctx);

অক্ষর *
get_xcomposefile_path(কাঠামো xkb_context *ctx);

অক্ষর *
get_xdg_xcompose_file_path(কাঠামো xkb_context *ctx);

অক্ষর *
get_home_xcompose_file_path(কাঠামো xkb_context *ctx);

অক্ষর *
get_locale_compose_file_path(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *locale);
