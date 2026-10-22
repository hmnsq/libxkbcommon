/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

अक्षर *
resolve_locale(संरचना xkb_context *ctx, स्थिरांक अक्षर *locale);

स्थिरांक अक्षर *
get_xlocaledir_path(संरचना xkb_context *ctx);

अक्षर *
get_xcomposefile_path(संरचना xkb_context *ctx);

अक्षर *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx);

अक्षर *
get_home_xcompose_file_path(संरचना xkb_context *ctx);

अक्षर *
get_locale_compose_file_path(संरचना xkb_context *ctx, स्थिरांक अक्षर *locale);
