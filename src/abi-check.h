/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

গণনা {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_আকার = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#define assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + আকার(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) আকার(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#define assert_same_field(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    আকার(((T){0}).f) == আকার(((T##suffix){0}).f) &&                        \
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
স্থির অন্তঃরেখ গণনা xkb_status
xkb_check_versioned_struct_আকার(আকার_t v1_আকার, আকার_t min_আকার,
                                 আকার_t callee_meaningful_bytes,
                                 আকার_t callee_আকার, আকার_t caller_আকার,
                                 ধ্রুবক শূন্য *caller_তথ্য)
{
    স্থির ধ্রুবক আকার_t max_আকার = (আকার_t)XKB_ABI_MAX_আকার;

    /* Check internal misuse */
    প্রতিজ্ঞা(v1_আকার <= min_আকার);
    প্রতিজ্ঞা(min_আকার <= callee_আকার);
    প্রতিজ্ঞা(callee_meaningful_bytes <= callee_আকার);
    প্রতিজ্ঞা(callee_আকার <= max_আকার);

    যদি (caller_আকার < v1_আকার || caller_আকার > max_আকার || !caller_তথ্য)
        ফেরত XKB_ERROR_ABI_INVALID_STRUCT_আকার;

    যদি (caller_আকার < min_আকার)
        ফেরত XKB_ERROR_ABI_BACKWARD_COMPAT;

    যদি (caller_আকার <= callee_meaningful_bytes) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        ফেরত XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ধ্রুবক unsigned অক্ষর *p =
        (ধ্রুবক unsigned অক্ষর *)caller_তথ্য + callee_meaningful_bytes;
    ধ্রুবক unsigned অক্ষর *end = (ধ্রুবক unsigned অক্ষর *)caller_তথ্য + caller_আকার;

    যখন (p < end) {
        যদি (*(p++) != 0)
            ফেরত XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    ফেরত XKB_SUCCESS;
}

#define xkb_check_versioned_struct_আকার_2(v1_আকার, min_আকার, \
                                        callee_meaningful_bytes, x) \
    xkb_check_versioned_struct_আকার(v1_আকার, min_আকার, \
                                     callee_meaningful_bytes, \
                                     আকার(*(x)), (x)->আকার_2, x)

শূন্য
xkb_log_abi_error(কাঠামো xkb_context * restrict ctx,
                  ধ্রুবক অক্ষর * restrict func, গণনা xkb_status status);
