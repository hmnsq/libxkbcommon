/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "abi-check.h"
#ଅନ୍ତର୍ଭାଵ "context.h"

ଶୂନ୍ୟ
xkb_log_abi_error(ସଂରଚନା xkb_context * restrict ctx,
                  ଅଚଳ ଅକ୍ଷର * restrict func, ଗଣନା xkb_status status)
{
    ବାଛ (status) {
    ସ୍ଥିତି XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        /* unreachable */
        ପ୍ରତିଜ୍ଞା(false);
    }
}
