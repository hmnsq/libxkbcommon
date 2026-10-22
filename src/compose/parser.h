/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-compose.h"

#અન્તર્ભાવ "src/utils.h"

XKB_EXPORT_PRIVATE અક્ષર *
parse_string_literal(સંરચના xkb_context *ctx, અચલ અક્ષર *string);

bool
parse_string(સંરચના xkb_compose_table *table,
             અચલ અક્ષર *string, size_t len,
             અચલ અક્ષર *file_name);

bool
parse_file(સંરચના xkb_compose_table *table,
           FILE *file, અચલ અક્ષર *file_name);
