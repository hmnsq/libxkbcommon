/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

XKB_EXPORT_ਨਿੱਜੀ uint8_t
utf32_ਮੰਜ਼ਿਲ_utf8(uint32_t unichar, ਅੱਖਰ *ਬਫਰ);

XKB_EXPORT_ਨਿੱਜੀ bool
is_valid_utf8(ਅਚਲ ਅੱਖਰ *ss, ਮਾਪ_t len);
