/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-compose.h"

#அந்தர்பாவ "src/utils.h"

XKB_EXPORT_PRIVATE எழுத்து *
parse_string_literal(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string);

bool
parse_string(கட்டமைப்பு xkb_compose_table *table,
             மாறிலி எழுத்து *string, size_t len,
             மாறிலி எழுத்து *file_name);

bool
parse_file(கட்டமைப்பு xkb_compose_table *table,
           FILE *file, மாறிலி எழுத்து *file_name);
