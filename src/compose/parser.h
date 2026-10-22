/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-compose.h"

#ಅನ್ತರ್ಭಾವ "src/utils.h"

XKB_EXPORT_PRIVATE ವರ್ಣ *
parse_string_literal(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *string);

bool
parse_string(ಸಂರಚನಾ xkb_compose_table *table,
             ಧ್ರುವ ವರ್ಣ *string, size_t len,
             ಧ್ರುವ ವರ್ಣ *file_name);

bool
parse_file(ಸಂರಚನಾ xkb_compose_table *table,
           FILE *file, ಧ್ರುವ ವರ್ಣ *file_name);
