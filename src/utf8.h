/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "utils.h"

XKB_EXPORT_خصوصی uint8_t
utf32_مقصد_utf8(uint32_t unichar, حرف *میانگیر);

XKB_EXPORT_خصوصی bool
is_valid_utf8(ثابت حرف *ss, اندازه_t len);
