/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "utils.h"

XKB_EXPORT_PRIVATE uint8_t
utf32_to_utf8(uint32_t unichar, ଅକ୍ଷର *buffer);

XKB_EXPORT_PRIVATE bool
is_valid_utf8(ଅଚଳ ଅକ୍ଷର *ss, size_t len);
