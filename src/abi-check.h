/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"

గణన {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_కొలత = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#నిర్వచించు assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + పరిమాణం(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) పరిమాణం(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#నిర్వచించు assert_same_క్షేత్రం(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    పరిమాణం(((T){0}).f) == పరిమాణం(((T##suffix){0}).f) &&                        \
    alignof(__typeof__(((T){0}).f)) == alignof(__typeof__(((T##suffix){0}).f)),\
    "Field " #f " is different in " #T " and " #T #suffix                      \
)

/**
 * Check the size field of a versioned struct for forward-compatibility.
 *
 * @param v1_size      Size of the first version of the struct. A caller_size
 *                     below this indicates a buggy or uninitialized size field.
 * @param min_size     Size of the oldest version still acceptable to this call.
 *                     A caller_size below this triggers backward-compat error.
 * @param callee_meaningful_bytes
 *                     Number of leading bytes, as known by the callee, that
 *                     hold meaningful/known data. Bytes from here up to
 *                     `caller_size` must be zero.
 *                     Use `offsetof(struct, reserved)` if there is `reserved`
 *                     field, otherwise `sizeof(struct)`.
 * @param callee_size  The size field provided by the callee.
 * @param caller_size  The size field provided by the caller.
 * @param caller_data  Pointer to the start of the struct.
 *
 * @returns `::XKB_SUCCESS` if the struct is valid and safe to use, otherwise
 * an error code indicating the kind of ABI violation.
 */
స్థిర అంతఃవరుస గణన xkb_స్థితి
xkb_check_versioned_struct_కొలత(కొలత_t v1_కొలత, కొలత_t min_కొలత,
                                 కొలత_t callee_meaningful_బైట్లు,
                                 కొలత_t callee_కొలత, కొలత_t caller_కొలత,
                                 స్థిరరాశి శూన్య *caller_దత్తాంశం)
{
    స్థిర స్థిరరాశి కొలత_t max_కొలత = (కొలత_t)XKB_ABI_MAX_కొలత;

    /* Check internal misuse */
    ప్రతిజ్ఞా(v1_కొలత <= min_కొలత);
    ప్రతిజ్ఞా(min_కొలత <= callee_కొలత);
    ప్రతిజ్ఞా(callee_meaningful_బైట్లు <= callee_కొలత);
    ప్రతిజ్ఞా(callee_కొలత <= max_కొలత);

    ఒకవేళ (caller_కొలత < v1_కొలత || caller_కొలత > max_కొలత || !caller_దత్తాంశం)
        తిరిగి XKB_ERROR_ABI_INVALID_STRUCT_కొలత;

    ఒకవేళ (caller_కొలత < min_కొలత)
        తిరిగి XKB_ERROR_ABI_BACKWARD_COMPAT;

    ఒకవేళ (caller_కొలత <= callee_meaningful_బైట్లు) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        తిరిగి XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    స్థిరరాశి గుర్తులేని అక్షరం *p =
        (స్థిరరాశి గుర్తులేని అక్షరం *)caller_దత్తాంశం + callee_meaningful_బైట్లు;
    స్థిరరాశి గుర్తులేని అక్షరం *చివర = (స్థిరరాశి గుర్తులేని అక్షరం *)caller_దత్తాంశం + caller_కొలత;

    అయితే (p < చివర) {
        ఒకవేళ (*(p++) != 0)
            తిరిగి XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    తిరిగి XKB_SUCCESS;
}

#నిర్వచించు xkb_check_versioned_struct_కొలత_2(v1_కొలత, min_కొలత, \
                                        callee_meaningful_బైట్లు, x) \
    xkb_check_versioned_struct_కొలత(v1_కొలత, min_కొలత, \
                                     callee_meaningful_బైట్లు, \
                                     పరిమాణం(*(x)), (x)->కొలత, x)

శూన్య
xkb_log_abi_error(నిర్మాణం xkb_context * పరిమిత ctx,
                  స్థిరరాశి అక్షరం * పరిమిత func, గణన xkb_స్థితి స్థితి_2);
