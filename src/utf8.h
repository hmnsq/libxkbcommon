/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "utils.h"

XKB_EXPORT_ಖಾಸಗಿ uint8_t
utf32_ಗಮ್ಯ_utf8(uint32_t unichar, ಅಕ್ಷರ *ಬಫರ್);

XKB_EXPORT_ಖಾಸಗಿ bool
is_valid_utf8(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ss, ಗಾತ್ರ_t len);
