/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "abi-check.h"
#شامل "context.h"

تهی
xkb_log_abi_error(ساختار xkb_context * restrict ctx,
                  ثابت نویسه * restrict func, شمارش xkb_status status)
{
    انتخاب (status) {
    حالت XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        بشکن;
    حالت XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        بشکن;
    حالت XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        بشکن;
    پیشفرض:
        /* unreachable */
        أكد(false);
    }
}
