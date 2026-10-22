/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "utils.h"

#جے_تعریف_شدہ _WIN32
#تعریف_کرو رستہ_SEPARATOR '\\'
#تعریف_کرو ALT_رستہ_SEPARATOR '/'
#تعریف_کرو is_رستہ_separator(s) \
    ((s) == رستہ_SEPARATOR || (s) == ALT_رستہ_SEPARATOR)
#وإلا
#تعریف_کرو رستہ_SEPARATOR '/'
#تعریف_کرو is_رستہ_separator(s) ((s) == رستہ_SEPARATOR)
#جے_انت

XKB_EXPORT_نجی bool
is_absolute_رستہ(ثابت حرف *رستہ);
