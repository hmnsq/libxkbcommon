/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>
#समावेश <stddef.h>
#समावेश <stdint.h>

#समावेश "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, अक्षर *बफर);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(स्थिर अक्षर *ss, माप_प्रकार len);
