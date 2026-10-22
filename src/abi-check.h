/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"

تعداد {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_ناپ = 4096,
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
ساكن مضمن تعداد xkb_حالت
xkb_check_versioned_struct_ناپ(ناپ_t v1_ناپ, ناپ_t min_ناپ,
                                 ناپ_t callee_meaningful_بائٹاں,
                                 ناپ_t callee_ناپ, ناپ_t caller_ناپ,
                                 ثابت فراغ *caller_انکڑے)
{
    ساكن ثابت ناپ_t max_ناپ = (ناپ_t)XKB_ABI_MAX_ناپ;

    /* Check internal misuse */
    أكد(v1_ناپ <= min_ناپ);
    أكد(min_ناپ <= callee_ناپ);
    أكد(callee_meaningful_بائٹاں <= callee_ناپ);
    أكد(callee_ناپ <= max_ناپ);

    إذا (caller_ناپ < v1_ناپ || caller_ناپ > max_ناپ || !caller_انکڑے)
        أرجع XKB_ERROR_ABI_INVALID_STRUCT_ناپ;

    إذا (caller_ناپ < min_ناپ)
        أرجع XKB_ERROR_ABI_BACKWARD_COMPAT;

    إذا (caller_ناپ <= callee_meaningful_بائٹاں) {
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
    ثابت بنانشانی حرف *p =
        (ثابت بنانشانی حرف *)caller_انکڑے + callee_meaningful_بائٹاں;
    ثابت بنانشانی حرف *سرا = (ثابت بنانشانی حرف *)caller_انکڑے + caller_ناپ;

    طالما (p < سرا) {
        إذا (*(p++) != 0)
            أرجع XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    أرجع XKB_SUCCESS;
}

#تعریف_کرو xkb_check_versioned_struct_ناپ_2(v1_ناپ, min_ناپ, \
                                        callee_meaningful_بائٹاں, x) \
    xkb_check_versioned_struct_ناپ(v1_ناپ, min_ناپ, \
                                     callee_meaningful_بائٹاں, \
                                     حجم(*(x)), (x)->ناپ, x)

فراغ
xkb_log_abi_error(بنية xkb_context * محدود ctx,
                  ثابت حرف * محدود func, تعداد xkb_حالت حالت);
