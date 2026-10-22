/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, અક્ષર *buffer);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(અચલ અક્ષર *ss, size_t len);
