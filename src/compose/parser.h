/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-compose.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "src/utils.h"

XKB_EXPORT_ਨਿੱਜੀ ਅੱਖਰ *
parse_string_literal(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string);

bool
parse_string(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ,
             ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len,
             ਅਚਲ ਅੱਖਰ *file_name);

bool
parse_file(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ,
           FILE *file, ਅਚਲ ਅੱਖਰ *file_name);
