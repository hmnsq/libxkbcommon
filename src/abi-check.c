/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "abi-check.h"
#அந்தர்பாவ "context.h"

வெறுமை
xkb_log_abi_error(கட்டமைப்பு xkb_context * restrict ctx,
                  மாறிலி எழுத்து * restrict func, எண்ணிக்கை xkb_status status)
{
    தேர்வு (status) {
    நிலை XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        நிறுத்து;
    நிலை XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        நிறுத்து;
    நிலை XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        நிறுத்து;
    இயல்பு:
        /* unreachable */
        ப்ரதிஜ்ஞா(false);
    }
}
