/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "utils.h"

XKB_EXPORT_वैयक्तिकम् uint8_t
utf32_गन्तव्यम्_utf8(uint32_t unichar, वर्ण *बफर);

XKB_EXPORT_वैयक्तिकम् bool
is_valid_utf8(ध्रुव वर्ण *ss, आकार_t len);
