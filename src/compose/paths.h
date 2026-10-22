/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

ৱর্ণ *
resolve_locale(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *locale);

ধ্রুৱ ৱর্ণ *
get_xlocaledir_path(সংরচনা xkb_context *ctx);

ৱর্ণ *
get_xcomposefile_path(সংরচনা xkb_context *ctx);

ৱর্ণ *
get_xdg_xcompose_file_path(সংরচনা xkb_context *ctx);

ৱর্ণ *
get_home_xcompose_file_path(সংরচনা xkb_context *ctx);

ৱর্ণ *
get_locale_compose_file_path(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *locale);
