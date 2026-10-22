/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "abi-check.h"
#അന്തര്ഭാവ "context.h"

ശൂന്യം
xkb_log_abi_error(ഘടന xkb_context * restrict ctx,
                  സ്ഥിരം അക്ഷരം * restrict func, എണ്ണൽ xkb_status status)
{
    തിരഞ്ഞെടുപ്പ് (status) {
    അവസ്ഥ XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        നിർത്തുക;
    അവസ്ഥ XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        നിർത്തുക;
    അവസ്ഥ XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        /* unreachable */
        പ്രതിജ്ഞാ(false);
    }
}
