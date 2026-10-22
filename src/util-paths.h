/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "utils.h"

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#define ALT_PATH_SEPARATOR '/'
#define is_path_separator(s) \
    ((s) == PATH_SEPARATOR || (s) == ALT_PATH_SEPARATOR)
#નહીંતર
#define PATH_SEPARATOR '/'
#define is_path_separator(s) ((s) == PATH_SEPARATOR)
#endif

XKB_EXPORT_PRIVATE bool
is_absolute_path(અચલ અક્ષર *path);
