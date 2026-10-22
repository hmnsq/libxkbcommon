/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "abi-check.h"
#অন্তর্ভাব "context.h"

শূন্য
xkb_log_abi_error(সংরচনা xkb_context * restrict ctx,
                  ধ্রুব বর্ণ * restrict func, গণনা xkb_status status)
{
    বিকল্প (status) {
    পক্ষ XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        ভঙ্গ;
    পক্ষ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ভঙ্গ;
    পক্ষ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ভঙ্গ;
    স্বভাব:
        /* unreachable */
        প্রতিজ্ঞা(false);
    }
}
