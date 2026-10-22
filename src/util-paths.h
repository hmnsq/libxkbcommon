/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "utils.h"

#که_تعریف_شوی _WIN32
#تعریف_کړه لار_SEPARATOR '\\'
#تعریف_کړه ALT_لار_SEPARATOR '/'
#تعریف_کړه is_لار_separator(s) \
    ((s) == لار_SEPARATOR || (s) == ALT_لار_SEPARATOR)
#وإلا
#تعریف_کړه لار_SEPARATOR '/'
#تعریف_کړه is_لار_separator(s) ((s) == لار_SEPARATOR)
#د_که_پای

XKB_EXPORT_شخصي bool
is_absolute_لار(ثابت حرف *لار);
