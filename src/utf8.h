/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "utils.h"

XKB_EXPORT_ذاتي uint8_t
utf32_منزل_utf8(uint32_t unichar, حرف *بفر);

XKB_EXPORT_ذاتي bool
is_valid_utf8(ثابت حرف *ss, سائيز_t len);
