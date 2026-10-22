/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

ಎಣಿಕೆ {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_ಗಾತ್ರ = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#ವ್ಯಾಖ್ಯಾನಿಸು assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + ಗಾತ್ರ(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) ಗಾತ್ರ(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು assert_same_ಕ್ಷೇತ್ರ(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    ಗಾತ್ರ(((T){0}).f) == ಗಾತ್ರ(((T##suffix){0}).f) &&                        \
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_check_versioned_struct_ಗಾತ್ರ(ಗಾತ್ರ_t v1_ಗಾತ್ರ, ಗಾತ್ರ_t min_ಗಾತ್ರ,
                                 ಗಾತ್ರ_t callee_meaningful_ಬೈಟುಗಳು,
                                 ಗಾತ್ರ_t callee_ಗಾತ್ರ, ಗಾತ್ರ_t caller_ಗಾತ್ರ,
                                 ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *caller_ದತ್ತಾಂಶ)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t max_ಗಾತ್ರ = (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ;

    /* Check internal misuse */
    ಪ್ರತಿಜ್ಞಾ(v1_ಗಾತ್ರ <= min_ಗಾತ್ರ);
    ಪ್ರತಿಜ್ಞಾ(min_ಗಾತ್ರ <= callee_ಗಾತ್ರ);
    ಪ್ರತಿಜ್ಞಾ(callee_meaningful_ಬೈಟುಗಳು <= callee_ಗಾತ್ರ);
    ಪ್ರತಿಜ್ಞಾ(callee_ಗಾತ್ರ <= max_ಗಾತ್ರ);

    ಆದರೆ (caller_ಗಾತ್ರ < v1_ಗಾತ್ರ || caller_ಗಾತ್ರ > max_ಗಾತ್ರ || !caller_ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ XKB_ERROR_ABI_INVALID_STRUCT_ಗಾತ್ರ;

    ಆದರೆ (caller_ಗಾತ್ರ < min_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ XKB_ERROR_ABI_BACKWARD_COMPAT;

    ಆದರೆ (caller_ಗಾತ್ರ <= callee_meaningful_ಬೈಟುಗಳು) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *p =
        (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)caller_ದತ್ತಾಂಶ + callee_meaningful_ಬೈಟುಗಳು;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಕೊನೆ = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)caller_ದತ್ತಾಂಶ + caller_ಗಾತ್ರ;

    ಆಗ (p < ಕೊನೆ) {
        ಆದರೆ (*(p++) != 0)
            ಹಿಂತಿರುಗಿ XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_check_versioned_struct_ಗಾತ್ರ_2(v1_ಗಾತ್ರ, min_ಗಾತ್ರ, \
                                        callee_meaningful_ಬೈಟುಗಳು, x) \
    xkb_check_versioned_struct_ಗಾತ್ರ(v1_ಗಾತ್ರ, min_ಗಾತ್ರ, \
                                     callee_meaningful_ಬೈಟುಗಳು, \
                                     ಗಾತ್ರ(*(x)), (x)->ಗಾತ್ರ_2, x)

ಶೂನ್ಯ
xkb_log_abi_error(ರಚನೆ xkb_context * ಸೀಮಿತ ctx,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸೀಮಿತ func, ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2);
