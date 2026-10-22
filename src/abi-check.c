/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abi-check.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"

ਖਾਲੀ
xkb_log_abi_error(ਬਣਤਰ xkb_context * ਸੀਮਿਤ ctx,
                  ਅਚਲ ਅੱਖਰ * ਸੀਮਿਤ func, ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ)
{
    ਚੋਣ (ਸਥਿਤੀ) {
    ਹਾਲਤ XKB_ERROR_ABI_INVALID_STRUCT_ਮਾਪ:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_ਮਾਪ,
                "%s: ABI error: unsupported versioned struct\n", func);
        ਰੋਕੋ;
    ਹਾਲਤ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        ਰੋਕੋ;
    ਹਾਲਤ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        ਰੋਕੋ;
    ਮੂਲ:
        /* unreachable */
        ਪ੍ਰਤਿਜ੍ਞਾ(false);
    }
}
