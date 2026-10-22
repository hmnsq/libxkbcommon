/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"

تعداد {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_سائيز = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#وصف_ڪريو assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + حجم(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) حجم(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#وصف_ڪريو assert_same_ميدان(T, suffix, f) static_assert(                         \
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
ساكن مضمن تعداد xkb_حالت
xkb_check_versioned_struct_سائيز(سائيز_t v1_سائيز, سائيز_t min_سائيز,
                                 سائيز_t callee_meaningful_بائيٽون,
                                 سائيز_t callee_سائيز, سائيز_t caller_سائيز,
                                 ثابت فراغ *caller_معلومات)
{
    ساكن ثابت سائيز_t max_سائيز = (سائيز_t)XKB_ABI_MAX_سائيز;

    /* Check internal misuse */
    أكد(v1_سائيز <= min_سائيز);
    أكد(min_سائيز <= callee_سائيز);
    أكد(callee_meaningful_بائيٽون <= callee_سائيز);
    أكد(callee_سائيز <= max_سائيز);

    إذا (caller_سائيز < v1_سائيز || caller_سائيز > max_سائيز || !caller_معلومات)
        أرجع XKB_ERROR_ABI_INVALID_STRUCT_سائيز;

    إذا (caller_سائيز < min_سائيز)
        أرجع XKB_ERROR_ABI_BACKWARD_COMPAT;

    إذا (caller_سائيز <= callee_meaningful_بائيٽون) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        أرجع XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ثابت بنانشان حرف *p =
        (ثابت بنانشان حرف *)caller_معلومات + callee_meaningful_بائيٽون;
    ثابت بنانشان حرف *پڇاڙي = (ثابت بنانشان حرف *)caller_معلومات + caller_سائيز;

    طالما (p < پڇاڙي) {
        إذا (*(p++) != 0)
            أرجع XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    أرجع XKB_SUCCESS;
}

#وصف_ڪريو xkb_check_versioned_struct_سائيز_2(v1_سائيز, min_سائيز, \
                                        callee_meaningful_بائيٽون, x) \
    xkb_check_versioned_struct_سائيز(v1_سائيز, min_سائيز, \
                                     callee_meaningful_بائيٽون, \
                                     حجم(*(x)), (x)->سائيز, x)

فراغ
xkb_log_abi_error(بنية xkb_context * محدود ctx,
                  ثابت حرف * محدود func, تعداد xkb_حالت حالت);
