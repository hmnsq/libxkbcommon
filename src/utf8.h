/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>

#شامل "utils.h"

XKB_EXPORT_نجی uint8_t
utf32_مقصد_utf8(uint32_t unichar, حرف *بفر);

XKB_EXPORT_نجی bool
is_valid_utf8(ثابت حرف *ss, سائز_t len);
