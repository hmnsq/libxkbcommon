/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "utils.h"

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#define ALT_PATH_SEPARATOR '/'
#define is_path_separator(s) \
    ((s) == PATH_SEPARATOR || (s) == ALT_PATH_SEPARATOR)
#അല്ലെങ്കിൽ
#define PATH_SEPARATOR '/'
#define is_path_separator(s) ((s) == PATH_SEPARATOR)
#endif

XKB_EXPORT_PRIVATE bool
is_absolute_path(സ്ഥിരം അക്ഷരം *path);
