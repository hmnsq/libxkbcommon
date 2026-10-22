/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "utils.h"

#اگر_تعریف_شده _WIN32
#تعریف_کن مسیر_SEPARATOR '\\'
#تعریف_کن ALT_مسیر_SEPARATOR '/'
#تعریف_کن is_مسیر_separator(s) \
    ((s) == مسیر_SEPARATOR || (s) == ALT_مسیر_SEPARATOR)
#وإلا
#تعریف_کن مسیر_SEPARATOR '/'
#تعریف_کن is_مسیر_separator(s) ((s) == مسیر_SEPARATOR)
#پایان_اگر

XKB_EXPORT_خصوصی bool
is_absolute_مسیر(ثابت حرف *مسیر);
