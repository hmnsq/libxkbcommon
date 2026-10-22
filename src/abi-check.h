/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

গণনা {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_SIZE = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#define assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + পরিমাণ(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) পরিমাণ(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#define assert_same_field(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    পরিমাণ(((T){0}).f) == পরিমাণ(((T##suffix){0}).f) &&                        \
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
স্থির অন্তঃস্থ গণনা xkb_status
xkb_check_versioned_struct_size_(size_t v1_size, size_t min_size,
                                 size_t callee_meaningful_bytes,
                                 size_t callee_size, size_t caller_size,
                                 ধ্রুৱ শূন্য *caller_data)
{
    স্থির ধ্রুৱ size_t max_size = (size_t)XKB_ABI_MAX_SIZE;

    /* Check internal misuse */
    প্রতিজ্ঞা(v1_size <= min_size);
    প্রতিজ্ঞা(min_size <= callee_size);
    প্রতিজ্ঞা(callee_meaningful_bytes <= callee_size);
    প্রতিজ্ঞা(callee_size <= max_size);

    যদি (caller_size < v1_size || caller_size > max_size || !caller_data)
        ঘূৰাই XKB_ERROR_ABI_INVALID_STRUCT_SIZE;

    যদি (caller_size < min_size)
        ঘূৰাই XKB_ERROR_ABI_BACKWARD_COMPAT;

    যদি (caller_size <= callee_meaningful_bytes) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        ঘূৰাই XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ধ্রুৱ unsigned ৱর্ণ *p =
        (ধ্রুৱ unsigned ৱর্ণ *)caller_data + callee_meaningful_bytes;
    ধ্রুৱ unsigned ৱর্ণ *end = (ধ্রুৱ unsigned ৱর্ণ *)caller_data + caller_size;

    যেতিয়ালৈ (p < end) {
        যদি (*(p++) != 0)
            ঘূৰাই XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    ঘূৰাই XKB_SUCCESS;
}

#define xkb_check_versioned_struct_size(v1_size, min_size, \
                                        callee_meaningful_bytes, x) \
    xkb_check_versioned_struct_size_(v1_size, min_size, \
                                     callee_meaningful_bytes, \
                                     পরিমাণ(*(x)), (x)->size, x)

শূন্য
xkb_log_abi_error(সংরচনা xkb_context * restrict ctx,
                  ধ্রুৱ ৱর্ণ * restrict func, গণনা xkb_status status);
