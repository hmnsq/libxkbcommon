/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "abi-check.h"
#అన్తర్భావ "context.h"

శూన్య
xkb_log_abi_error(నిర్మాణం xkb_context * పరిమిత ctx,
                  స్థిరరాశి అక్షరం * పరిమిత func, గణన xkb_స్థితి స్థితి_2)
{
    ఎంపిక (స్థితి_2) {
    స్థితి XKB_ERROR_ABI_INVALID_STRUCT_కొలత:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_కొలత,
                "%s: ABI error: unsupported versioned struct\n", func);
        ఆపు;
    స్థితి XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ఆపు;
    స్థితి XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ఆపు;
    అప్రమేయ:
        /* unreachable */
        ప్రతిజ్ఞా(false);
    }
}
