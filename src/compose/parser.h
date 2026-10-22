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

XKB_EXPORT_PRIVATE অক্ষর *
parse_string_literal(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *string);

bool
parse_string(কাঠামো xkb_compose_table *table,
             ধ্রুবক অক্ষর *string, আকার_t len,
             ধ্রুবক অক্ষর *file_name);

bool
parse_file(কাঠামো xkb_compose_table *table,
           FILE *file, ধ্রুবক অক্ষর *file_name);
