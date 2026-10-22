/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "abi-check.h"
#અન્તર્ભાવ "context.h"

શૂન્ય
xkb_log_abi_error(સંરચના xkb_context * restrict ctx,
                  અચલ અક્ષર * restrict func, ગણના xkb_status status)
{
    પસંદગી (status) {
    સ્થિતિ XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        તોડો;
    સ્થિતિ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        તોડો;
    સ્થિતિ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        તોડો;
    મૂળભૂત:
        /* unreachable */
        પ્રતિજ્ઞા(false);
    }
}
