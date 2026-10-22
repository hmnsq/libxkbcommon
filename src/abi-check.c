/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "abi-check.h"
#ಅನ್ತರ್ಭಾವ "context.h"

ಶೂನ್ಯ
xkb_log_abi_error(ಸಂರಚನಾ xkb_context * restrict ctx,
                  ಧ್ರುವ ವರ್ಣ * restrict func, ಗಣನಾ xkb_status status)
{
    ವಿಕಲ್ಪ (status) {
    ಪಕ್ಷ XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        ಭಙ್ಗ;
    ಪಕ್ಷ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ಭಙ್ಗ;
    ಪಕ್ಷ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        /* unreachable */
        ಪ್ರತಿಜ್ಞಾ(false);
    }
}
