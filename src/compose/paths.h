/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

نویسه *
resolve_locale(ساختار xkb_context *ctx, ثابت نویسه *locale);

ثابت نویسه *
get_xlocaledir_path(ساختار xkb_context *ctx);

نویسه *
get_xcomposefile_path(ساختار xkb_context *ctx);

نویسه *
get_xdg_xcompose_file_path(ساختار xkb_context *ctx);

نویسه *
get_home_xcompose_file_path(ساختار xkb_context *ctx);

نویسه *
get_locale_compose_file_path(ساختار xkb_context *ctx, ثابت نویسه *locale);
