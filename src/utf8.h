/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, अक्षर *buffer);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(स्थिरांक अक्षर *ss, माप_प्रकार len);
