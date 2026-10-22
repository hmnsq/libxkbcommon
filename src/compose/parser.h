/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stddef.h>
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon-compose.h"

#অন্তর্ভাৱ "src/utils.h"

XKB_EXPORT_PRIVATE ৱর্ণ *
parse_string_literal(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *string);

bool
parse_string(সংরচনা xkb_compose_table *table,
             ধ্রুৱ ৱর্ণ *string, size_t len,
             ধ্রুৱ ৱর্ণ *file_name);

bool
parse_file(সংরচনা xkb_compose_table *table,
           FILE *file, ধ্রুৱ ৱর্ণ *file_name);
