/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>

#شامل "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, نویسه *buffer);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(ثابت نویسه *ss, size_t len);
