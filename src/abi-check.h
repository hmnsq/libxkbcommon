/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"

تعداد {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_اندازه = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#تعریف_کن assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + حجم(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) حجم(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#تعریف_کن assert_same_فیلد(T, suffix, f) static_assert(                         \
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
ساكن مضمن تعداد xkb_وضعیت
xkb_check_versioned_struct_اندازه(اندازه_t v1_اندازه, اندازه_t min_اندازه,
                                 اندازه_t callee_meaningful_بایتها,
                                 اندازه_t callee_اندازه, اندازه_t caller_اندازه,
                                 ثابت فراغ *caller_داده)
{
    ساكن ثابت اندازه_t max_اندازه = (اندازه_t)XKB_ABI_MAX_اندازه;

    /* Check internal misuse */
    أكد(v1_اندازه <= min_اندازه);
    أكد(min_اندازه <= callee_اندازه);
    أكد(callee_meaningful_بایتها <= callee_اندازه);
    أكد(callee_اندازه <= max_اندازه);

    إذا (caller_اندازه < v1_اندازه || caller_اندازه > max_اندازه || !caller_داده)
        أرجع XKB_ERROR_ABI_INVALID_STRUCT_اندازه;

    إذا (caller_اندازه < min_اندازه)
        أرجع XKB_ERROR_ABI_BACKWARD_COMPAT;

    إذا (caller_اندازه <= callee_meaningful_بایتها) {
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
    ثابت بیعلامت حرف *p =
        (ثابت بیعلامت حرف *)caller_داده + callee_meaningful_بایتها;
    ثابت بیعلامت حرف *انتها = (ثابت بیعلامت حرف *)caller_داده + caller_اندازه;

    طالما (p < انتها) {
        إذا (*(p++) != 0)
            أرجع XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    أرجع XKB_SUCCESS;
}

#تعریف_کن xkb_check_versioned_struct_اندازه_2(v1_اندازه, min_اندازه, \
                                        callee_meaningful_بایتها, x) \
    xkb_check_versioned_struct_اندازه(v1_اندازه, min_اندازه, \
                                     callee_meaningful_بایتها, \
                                     حجم(*(x)), (x)->اندازه, x)

فراغ
xkb_log_abi_error(بنية xkb_context * محدود ctx,
                  ثابت حرف * محدود func, تعداد xkb_وضعیت وضعیت);
