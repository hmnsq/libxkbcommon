/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdio.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-compose.h"

#شامل "src/utils.h"

XKB_EXPORT_PRIVATE نویسه *
parse_string_literal(ساختار xkb_context *ctx, ثابت نویسه *string);

bool
parse_string(ساختار xkb_compose_table *table,
             ثابت نویسه *string, size_t len,
             ثابت نویسه *file_name);

bool
parse_file(ساختار xkb_compose_table *table,
           FILE *file, ثابت نویسه *file_name);
