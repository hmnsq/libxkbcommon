/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdio.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"

#ضمن "src/utils.h"

XKB_EXPORT_نجی حرف *
parse_string_literal(بنية xkb_context *ctx, ثابت حرف *string);

bool
parse_string(بنية xkb_compose_جدول *جدول,
             ثابت حرف *string, ناپ_t len,
             ثابت حرف *file_name);

bool
parse_file(بنية xkb_compose_جدول *جدول,
           FILE *file, ثابت حرف *file_name);
