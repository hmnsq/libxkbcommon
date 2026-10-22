/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "utils.h"

#अगर_घोषित _WIN32
#घोषणा PATH_SEPARATOR '\\'
#घोषणा ALT_PATH_SEPARATOR '/'
#घोषणा is_path_separator(s) \
    ((s) == PATH_SEPARATOR || (s) == ALT_PATH_SEPARATOR)
#अन्यथा
#घोषणा PATH_SEPARATOR '/'
#घोषणा is_path_separator(s) ((s) == PATH_SEPARATOR)
#पूर्ण_अगर

XKB_EXPORT_PRIVATE bool
is_absolute_path(स्थिर अक्षर *path);
