/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "abi-check.h"
#شامل "context.h"

خالی
xkb_log_abi_error(ساخت xkb_context * محدود ctx,
                  ثابت حرف * محدود func, شمار xkb_حالت حالت_2)
{
    انتخاب (حالت_2) {
    حالت XKB_ERROR_ABI_INVALID_STRUCT_سائز:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_سائز,
                "%s: ABI error: unsupported versioned struct\n", func);
        توڑو;
    حالت XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        توڑو;
    حالت XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        توڑو;
    طےشدہ:
        /* unreachable */
        أكد(false);
    }
}
