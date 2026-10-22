/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

ಅಕ್ಷರ *
resolve_locale(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_xlocaledir_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಅಕ್ಷರ *
get_xcomposefile_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಅಕ್ಷರ *
get_xdg_xcompose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಅಕ್ಷರ *
get_home_xcompose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಅಕ್ಷರ *
get_locale_compose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale);
