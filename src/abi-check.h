/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"

شمار {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_سائز = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#تعریف_کرو assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + حجم(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) حجم(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#تعریف_کرو assert_same_میدان(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    حجم(((T){0}).f) == حجم(((T##suffix){0}).f) &&                        \
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
ساکن اندرونی شمار xkb_حالت
xkb_check_versioned_struct_سائز(سائز_t v1_سائز, سائز_t min_سائز,
                                 سائز_t callee_meaningful_بائٹیں,
                                 سائز_t callee_سائز, سائز_t caller_سائز,
                                 ثابت خالی *caller_ڈیٹا)
{
    ساکن ثابت سائز_t max_سائز = (سائز_t)XKB_ABI_MAX_سائز;

    /* Check internal misuse */
    أكد(v1_سائز <= min_سائز);
    أكد(min_سائز <= callee_سائز);
    أكد(callee_meaningful_بائٹیں <= callee_سائز);
    أكد(callee_سائز <= max_سائز);

    اگر (caller_سائز < v1_سائز || caller_سائز > max_سائز || !caller_ڈیٹا)
        واپس XKB_ERROR_ABI_INVALID_STRUCT_سائز;

    اگر (caller_سائز < min_سائز)
        واپس XKB_ERROR_ABI_BACKWARD_COMPAT;

    اگر (caller_سائز <= callee_meaningful_بائٹیں) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        واپس XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ثابت غیرعلامتی حرف *p =
        (ثابت غیرعلامتی حرف *)caller_ڈیٹا + callee_meaningful_بائٹیں;
    ثابت غیرعلامتی حرف *انتہا = (ثابت غیرعلامتی حرف *)caller_ڈیٹا + caller_سائز;

    جب (p < انتہا) {
        اگر (*(p++) != 0)
            واپس XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    واپس XKB_SUCCESS;
}

#تعریف_کرو xkb_check_versioned_struct_سائز_2(v1_سائز, min_سائز, \
                                        callee_meaningful_بائٹیں, x) \
    xkb_check_versioned_struct_سائز(v1_سائز, min_سائز, \
                                     callee_meaningful_بائٹیں, \
                                     حجم(*(x)), (x)->سائز, x)

خالی
xkb_log_abi_error(ساخت xkb_context * محدود ctx,
                  ثابت حرف * محدود func, شمار xkb_حالت حالت_2);
