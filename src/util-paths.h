/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "utils.h"

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#define ALT_PATH_SEPARATOR '/'
#define is_path_separator(s) \
    ((s) == PATH_SEPARATOR || (s) == ALT_PATH_SEPARATOR)
#अण्णहा
#define PATH_SEPARATOR '/'
#define is_path_separator(s) ((s) == PATH_SEPARATOR)
#endif

XKB_EXPORT_PRIVATE bool
is_absolute_path(ध्रुव वर्ण *path);
