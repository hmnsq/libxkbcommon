/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "utils.h"

#اگر_تعریف_شدہ _WIN32
#تعریف_کرو راستہ_SEPARATOR '\\'
#تعریف_کرو ALT_راستہ_SEPARATOR '/'
#تعریف_کرو is_راستہ_separator(s) \
    ((s) == راستہ_SEPARATOR || (s) == ALT_راستہ_SEPARATOR)
#ورنہ
#تعریف_کرو راستہ_SEPARATOR '/'
#تعریف_کرو is_راستہ_separator(s) ((s) == راستہ_SEPARATOR)
#اگر_ختم

XKB_EXPORT_نجی bool
is_absolute_راستہ(ثابت حرف *راستہ);
