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
xkb_log_abi_error(ರಚನೆ xkb_context * ಸೀಮಿತ ctx,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸೀಮಿತ func, ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2)
{
    ಆಯ್ಕೆ (ಸ್ಥಿತಿ_2) {
    ಸ್ಥಿತಿ XKB_ERROR_ABI_INVALID_STRUCT_ಗಾತ್ರ:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_ಗಾತ್ರ,
                "%s: ABI error: unsupported versioned struct\n", func);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        /* unreachable */
        ಪ್ರತಿಜ್ಞಾ(false);
    }
}
