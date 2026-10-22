/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

#అన్తర్భావ "utils.h"

XKB_EXPORT_వ్యక్తిగత uint8_t
utf32_గమ్యం_utf8(uint32_t unichar, అక్షరం *బఫరు);

XKB_EXPORT_వ్యక్తిగత bool
is_valid_utf8(స్థిరరాశి అక్షరం *ss, కొలత_t len);
