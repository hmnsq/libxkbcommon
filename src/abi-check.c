/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "abi-check.h"
#समावेश "context.h"

व्योम
xkb_log_abi_error(काष्ठा xkb_context * restrict ctx,
                  स्थिर अक्षर * restrict func, क्रमागत xkb_status status)
{
    चयन (status) {
    हाल XKB_ERROR_ABI_INVALID_STRUCT_आकार:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_आकार,
                "%s: ABI error: unsupported versioned struct\n", func);
        अवरोध;
    हाल XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        अवरोध;
    हाल XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        अवरोध;
    शेष:
        /* unreachable */
        निश्चित(false);
    }
}
