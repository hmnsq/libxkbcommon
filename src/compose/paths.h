/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

അക്ഷരം *
resolve_locale(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *locale);

സ്ഥിരം അക്ഷരം *
get_xlocaledir_path(ഘടന xkb_context *ctx);

അക്ഷരം *
get_xcomposefile_path(ഘടന xkb_context *ctx);

അക്ഷരം *
get_xdg_xcompose_file_path(ഘടന xkb_context *ctx);

അക്ഷരം *
get_home_xcompose_file_path(ഘടന xkb_context *ctx);

അക്ഷരം *
get_locale_compose_file_path(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *locale);
