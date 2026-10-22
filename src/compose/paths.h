/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"

अक्षर *
resolve_locale(काष्ठा xkb_context *ctx, स्थिर अक्षर *locale);

स्थिर अक्षर *
get_xlocaledir_path(काष्ठा xkb_context *ctx);

अक्षर *
get_xcomposefile_path(काष्ठा xkb_context *ctx);

अक्षर *
get_xdg_xcompose_file_path(काष्ठा xkb_context *ctx);

अक्षर *
get_home_xcompose_file_path(काष्ठा xkb_context *ctx);

अक्षर *
get_locale_compose_file_path(काष्ठा xkb_context *ctx, स्थिर अक्षर *locale);
