/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-compose.h"

#include "src/utils.h"

XKB_EXPORT_PRIVATE var_na *
parse_string_literal(sa_nracanaa xkb_context *ctx, const var_na *string);

bool
parse_string(sa_nracanaa xkb_compose_table *table,
             const var_na *string, size_t len,
             const var_na *file_name);

bool
parse_file(sa_nracanaa xkb_compose_table *table,
           FILE *file, const var_na *file_name);
