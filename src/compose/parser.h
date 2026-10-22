/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdio.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"

#ضمن "src/utils.h"

XKB_EXPORT_PRIVATE حرف *
parse_string_literal(بنية xkb_context *ctx, ثابت حرف *string);

bool
parse_string(بنية xkb_compose_table *table,
             ثابت حرف *string, size_t len,
             ثابت حرف *file_name);

bool
parse_file(بنية xkb_compose_table *table,
           FILE *file, ثابت حرف *file_name);
