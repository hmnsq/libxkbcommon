/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>
#समावेश <stddef.h>
#समावेश <stdio.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-compose.h"

#समावेश "src/utils.h"

XKB_EXPORT_PRIVATE अक्षर *
parse_string_literal(काष्ठा xkb_context *ctx, स्थिर अक्षर *string);

bool
parse_string(काष्ठा xkb_compose_table *table,
             स्थिर अक्षर *string, माप_प्रकार len,
             स्थिर अक्षर *file_name);

bool
parse_file(काष्ठा xkb_compose_table *table,
           खाता *file, स्थिर अक्षर *file_name);
