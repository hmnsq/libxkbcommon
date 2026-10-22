/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-compose.h"

#അന്തര്ഭാവ "src/utils.h"

XKB_EXPORT_PRIVATE അക്ഷരം *
parse_string_literal(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string);

bool
parse_string(ഘടന xkb_compose_table *table,
             സ്ഥിരം അക്ഷരം *string, size_t len,
             സ്ഥിരം അക്ഷരം *file_name);

bool
parse_file(ഘടന xkb_compose_table *table,
           FILE *file, സ്ഥിരം അക്ഷരം *file_name);
