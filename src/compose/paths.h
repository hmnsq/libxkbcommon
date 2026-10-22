/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

ଅକ୍ଷର *
resolve_locale(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *locale);

ଅଚଳ ଅକ୍ଷର *
get_xlocaledir_path(ସଂରଚନା xkb_context *ctx);

ଅକ୍ଷର *
get_xcomposefile_path(ସଂରଚନା xkb_context *ctx);

ଅକ୍ଷର *
get_xdg_xcompose_file_path(ସଂରଚନା xkb_context *ctx);

ଅକ୍ଷର *
get_home_xcompose_file_path(ସଂରଚନା xkb_context *ctx);

ଅକ୍ଷର *
get_locale_compose_file_path(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *locale);
