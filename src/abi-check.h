/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

ਗਿਣਤੀ {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_ਮਾਪ = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + ਆਕਾਰ(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) ਆਕਾਰ(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ assert_same_ਖੇਤਰ(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    ਆਕਾਰ(((T){0}).f) == ਆਕਾਰ(((T##suffix){0}).f) &&                        \
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_check_versioned_struct_ਮਾਪ(ਮਾਪ_t v1_ਮਾਪ, ਮਾਪ_t min_ਮਾਪ,
                                 ਮਾਪ_t callee_meaningful_ਬਾਈਟਾਂ,
                                 ਮਾਪ_t callee_ਮਾਪ, ਮਾਪ_t caller_ਮਾਪ,
                                 ਅਚਲ ਖਾਲੀ *caller_ਅੰਕੜੇ)
{
    ਸਥਿਰ ਅਚਲ ਮਾਪ_t max_ਮਾਪ = (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ;

    /* Check internal misuse */
    ਪ੍ਰਤਿਜ੍ਞਾ(v1_ਮਾਪ <= min_ਮਾਪ);
    ਪ੍ਰਤਿਜ੍ਞਾ(min_ਮਾਪ <= callee_ਮਾਪ);
    ਪ੍ਰਤਿਜ੍ਞਾ(callee_meaningful_ਬਾਈਟਾਂ <= callee_ਮਾਪ);
    ਪ੍ਰਤਿਜ੍ਞਾ(callee_ਮਾਪ <= max_ਮਾਪ);

    ਜੇ (caller_ਮਾਪ < v1_ਮਾਪ || caller_ਮਾਪ > max_ਮਾਪ || !caller_ਅੰਕੜੇ)
        ਵਾਪਸ XKB_ERROR_ABI_INVALID_STRUCT_ਮਾਪ;

    ਜੇ (caller_ਮਾਪ < min_ਮਾਪ)
        ਵਾਪਸ XKB_ERROR_ABI_BACKWARD_COMPAT;

    ਜੇ (caller_ਮਾਪ <= callee_meaningful_ਬਾਈਟਾਂ) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        ਵਾਪਸ XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p =
        (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)caller_ਅੰਕੜੇ + callee_meaningful_ਬਾਈਟਾਂ;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਸਿਰਾ = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)caller_ਅੰਕੜੇ + caller_ਮਾਪ;

    ਜਦੋਂ (p < ਸਿਰਾ) {
        ਜੇ (*(p++) != 0)
            ਵਾਪਸ XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    ਵਾਪਸ XKB_SUCCESS;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_check_versioned_struct_ਮਾਪ_2(v1_ਮਾਪ, min_ਮਾਪ, \
                                        callee_meaningful_ਬਾਈਟਾਂ, x) \
    xkb_check_versioned_struct_ਮਾਪ(v1_ਮਾਪ, min_ਮਾਪ, \
                                     callee_meaningful_ਬਾਈਟਾਂ, \
                                     ਆਕਾਰ(*(x)), (x)->ਮਾਪ, x)

ਖਾਲੀ
xkb_log_abi_error(ਬਣਤਰ xkb_context * ਸੀਮਿਤ ctx,
                  ਅਚਲ ਅੱਖਰ * ਸੀਮਿਤ func, ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ);
