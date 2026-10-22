/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-compose.h"

#अन्तर्भाव "src/utils.h"

XKB_EXPORT_PRIVATE अक्षर *
parse_string_literal(संरचना xkb_context *ctx, स्थिरांक अक्षर *string);

bool
parse_string(संरचना xkb_compose_table *table,
             स्थिरांक अक्षर *string, माप_प्रकार len,
             स्थिरांक अक्षर *file_name);

bool
parse_file(संरचना xkb_compose_table *table,
           खाता *file, स्थिरांक अक्षर *file_name);
