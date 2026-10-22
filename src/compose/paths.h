/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "config.h"

#include "xkbcommon/xkbcommon.h"

var_na *
resolve_locale(sa_nracanaa xkb_context *ctx, const var_na *locale);

const var_na *
get_xlocaledir_path(sa_nracanaa xkb_context *ctx);

var_na *
get_xcomposefile_path(sa_nracanaa xkb_context *ctx);

var_na *
get_xdg_xcompose_file_path(sa_nracanaa xkb_context *ctx);

var_na *
get_home_xcompose_file_path(sa_nracanaa xkb_context *ctx);

var_na *
get_locale_compose_file_path(sa_nracanaa xkb_context *ctx, const var_na *locale);
