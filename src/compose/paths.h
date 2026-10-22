/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

எழுத்து *
resolve_locale(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *locale);

மாறிலி எழுத்து *
get_xlocaledir_path(கட்டமைப்பு xkb_context *ctx);

எழுத்து *
get_xcomposefile_path(கட்டமைப்பு xkb_context *ctx);

எழுத்து *
get_xdg_xcompose_file_path(கட்டமைப்பு xkb_context *ctx);

எழுத்து *
get_home_xcompose_file_path(கட்டமைப்பு xkb_context *ctx);

எழுத்து *
get_locale_compose_file_path(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *locale);
