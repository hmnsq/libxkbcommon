/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdio.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-compose.h"

#شامل "src/utils.h"

XKB_EXPORT_نجی حرف *
parse_string_literal(ساخت xkb_context *ctx, ثابت حرف *string);

bool
parse_string(ساخت xkb_compose_جدول *جدول,
             ثابت حرف *string, سائز_t len,
             ثابت حرف *file_name);

bool
parse_file(ساخت xkb_compose_جدول *جدول,
           FILE *file, ثابت حرف *file_name);
