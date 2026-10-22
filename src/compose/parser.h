/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-compose.h"

#ଅନ୍ତର୍ଭାଵ "src/utils.h"

XKB_EXPORT_PRIVATE ଅକ୍ଷର *
parse_string_literal(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *string);

bool
parse_string(ସଂରଚନା xkb_compose_table *table,
             ଅଚଳ ଅକ୍ଷର *string, size_t len,
             ଅଚଳ ଅକ୍ଷର *file_name);

bool
parse_file(ସଂରଚନା xkb_compose_table *table,
           FILE *file, ଅଚଳ ଅକ୍ଷର *file_name);
