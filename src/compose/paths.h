/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

ವರ್ಣ *
resolve_locale(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *locale);

ಧ್ರುವ ವರ್ಣ *
get_xlocaledir_path(ಸಂರಚನಾ xkb_context *ctx);

ವರ್ಣ *
get_xcomposefile_path(ಸಂರಚನಾ xkb_context *ctx);

ವರ್ಣ *
get_xdg_xcompose_file_path(ಸಂರಚನಾ xkb_context *ctx);

ವರ್ಣ *
get_home_xcompose_file_path(ಸಂರಚನಾ xkb_context *ctx);

ವರ್ಣ *
get_locale_compose_file_path(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *locale);
