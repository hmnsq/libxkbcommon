/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "context.h"

शून्य
xkb_log_abi_error(संरचना xkb_context * सीमित ctx,
                  ध्रुव वर्ण * सीमित func, गणना xkb_स्थितिः स्थितिः)
{
    विकल्प (स्थितिः) {
    पक्ष XKB_ERROR_ABI_INVALID_STRUCT_आकार:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_आकार,
                "%s: ABI error: unsupported versioned struct\n", func);
        भङ्ग;
    पक्ष XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        भङ्ग;
    पक्ष XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        भङ्ग;
    स्वभाव:
        /* unreachable */
        प्रतिज्ञा(false);
    }
}
