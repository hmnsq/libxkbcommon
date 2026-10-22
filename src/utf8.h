/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, ವರ್ಣ *buffer);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(ಧ್ರುವ ವರ್ಣ *ss, size_t len);
