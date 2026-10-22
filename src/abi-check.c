/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "abi-check.h"
#ضمن "context.h"

فراغ
xkb_log_abi_error(بنية xkb_context * محدود ctx,
                  ثابت حرف * محدود func, تعداد xkb_حالت حالت)
{
    بدل (حالت) {
    حالة XKB_ERROR_ABI_INVALID_STRUCT_اندازه:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_اندازه,
                "%s: ABI error: unsupported versioned struct\n", func);
        اكسر;
    حالة XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        اكسر;
    حالة XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        اكسر;
    افتراضي:
        /* unreachable */
        أكد(false);
    }
}
