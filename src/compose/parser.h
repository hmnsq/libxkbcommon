/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdio.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-compose.h"

#অন্তর্ভাব "src/utils.h"

XKB_EXPORT_PRIVATE বর্ণ *
parse_string_literal(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *string);

bool
parse_string(সংরচনা xkb_compose_table *table,
             ধ্রুব বর্ণ *string, size_t len,
             ধ্রুব বর্ণ *file_name);

bool
parse_file(সংরচনা xkb_compose_table *table,
           FILE *file, ধ্রুব বর্ণ *file_name);
