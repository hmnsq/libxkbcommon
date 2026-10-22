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

XKB_EXPORT_PRIVATE वर्ण *
parse_string_literal(संरचना xkb_context *ctx, ध्रुव वर्ण *string);

bool
parse_string(संरचना xkb_compose_table *table,
             ध्रुव वर्ण *string, size_t len,
             ध्रुव वर्ण *file_name);

bool
parse_file(संरचना xkb_compose_table *table,
           FILE *file, ध्रुव वर्ण *file_name);
