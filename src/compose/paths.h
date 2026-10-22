/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

અક્ષર *
resolve_locale(સંરચના xkb_context *ctx, અચલ અક્ષર *locale);

અચલ અક્ષર *
get_xlocaledir_path(સંરચના xkb_context *ctx);

અક્ષર *
get_xcomposefile_path(સંરચના xkb_context *ctx);

અક્ષર *
get_xdg_xcompose_file_path(સંરચના xkb_context *ctx);

અક્ષર *
get_home_xcompose_file_path(સંરચના xkb_context *ctx);

અક્ષર *
get_locale_compose_file_path(સંરચના xkb_context *ctx, અચલ અક્ષર *locale);
