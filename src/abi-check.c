/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "context.h"

शून्य
xkb_log_abi_error(संरचना xkb_context * restrict ctx,
                  स्थिरांक अक्षर * restrict func, गणना xkb_status status)
{
    निवड (status) {
    स्थिती XKB_ERROR_ABI_INVALID_STRUCT_SIZE:
        log_err(ctx, XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
                "%s: ABI error: unsupported versioned struct\n", func);
        थांबा;
    स्थिती XKB_ERROR_ABI_BACKWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_BACKWARD_COMPAT,
                "%s: ABI version mismatch: missing newer required fields\n",
                func);
        थांबा;
    स्थिती XKB_ERROR_ABI_FORWARD_COMPAT:
        log_err(ctx, XKB_ERROR_ABI_FORWARD_COMPAT,
                "%s: ABI version mismatch: cannot use newer fields\n", func);
        थांबा;
    पूर्वनिर्धारित:
        /* unreachable */
        प्रतिज्ञा(false);
    }
}
