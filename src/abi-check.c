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
xkb_log_abi_error(কাঠামো xkb_context * restrict ctx,
                  ধ্রুবক অক্ষর * restrict func, গণনা xkb_status status)
{
    নির্বাচন (status) {
    ক্ষেত্র XKB_ERROR_ABI_INVALID_STRUCT_আকার:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_আকার,
                "%s: ABI error: unsupported versioned struct\n", func);
        বিরতি;
    ক্ষেত্র XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        বিরতি;
    ক্ষেত্র XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        বিরতি;
    পূর্বনির্ধারিত:
        /* unreachable */
        প্রতিজ্ঞা(false);
    }
}
