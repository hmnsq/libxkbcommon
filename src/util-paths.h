/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "utils.h"

#جيڪڏهن_وصف_ٿيل _WIN32
#وصف_ڪريو رستو_SEPARATOR '\\'
#وصف_ڪريو ALT_رستو_SEPARATOR '/'
#وصف_ڪريو is_رستو_separator(s) \
    ((s) == رستو_SEPARATOR || (s) == ALT_رستو_SEPARATOR)
#وإلا
#وصف_ڪريو رستو_SEPARATOR '/'
#وصف_ڪريو is_رستو_separator(s) ((s) == رستو_SEPARATOR)
#جيڪڏهن_پورو

XKB_EXPORT_ذاتي bool
is_absolute_رستو(ثابت حرف *رستو);
