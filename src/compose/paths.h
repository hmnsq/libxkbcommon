/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

ਅੱਖਰ *
resolve_locale(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *locale);

ਅਚਲ ਅੱਖਰ *
get_xlocaledir_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅੱਖਰ *
get_xcomposefile_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅੱਖਰ *
get_xdg_xcompose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅੱਖਰ *
get_home_xcompose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅੱਖਰ *
get_locale_compose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *locale);
