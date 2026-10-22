/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਰਸਤਾ_SEPARATOR '\\'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALT_ਰਸਤਾ_SEPARATOR '/'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_ਰਸਤਾ_separator(s) \
    ((s) == ਰਸਤਾ_SEPARATOR || (s) == ALT_ਰਸਤਾ_SEPARATOR)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਰਸਤਾ_SEPARATOR '/'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_ਰਸਤਾ_separator(s) ((s) == ਰਸਤਾ_SEPARATOR)
#ਜੇ_ਅੰਤ

XKB_EXPORT_ਨਿੱਜੀ bool
is_absolute_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);
