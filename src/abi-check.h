/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"

गणना {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_आकार = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#परिभाषय assert_no_padding(T, f, ...) static_assert(      \
    offsetof(T, f) + परिमाण(((T){0}).f) ==               \
    __VA_OPT__(offsetof(T, __VA_ARGS__) + 0 *) परिमाण(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#परिभाषय assert_same_क्षेत्रम्(T, suffix, f) static_assert(                         \
    offsetof(T, f) == offsetof(T##suffix, f) &&                                \
    परिमाण(((T){0}).f) == परिमाण(((T##suffix){0}).f) &&                        \
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
स्थिर अन्तःस्थ गणना xkb_स्थितिः
xkb_check_versioned_struct_आकार(आकार_t v1_आकार, आकार_t min_आकार,
                                 आकार_t callee_meaningful_अष्टकानि,
                                 आकार_t callee_आकार, आकार_t caller_आकार,
                                 ध्रुव शून्य *caller_दत्त)
{
    स्थिर ध्रुव आकार_t max_आकार = (आकार_t)XKB_ABI_MAX_आकार;

    /* Check internal misuse */
    प्रतिज्ञा(v1_आकार <= min_आकार);
    प्रतिज्ञा(min_आकार <= callee_आकार);
    प्रतिज्ञा(callee_meaningful_अष्टकानि <= callee_आकार);
    प्रतिज्ञा(callee_आकार <= max_आकार);

    यदि (caller_आकार < v1_आकार || caller_आकार > max_आकार || !caller_दत्त)
        प्रत्यागम XKB_ERROR_ABI_INVALID_STRUCT_आकार;

    यदि (caller_आकार < min_आकार)
        प्रत्यागम XKB_ERROR_ABI_BACKWARD_COMPAT;

    यदि (caller_आकार <= callee_meaningful_अष्टकानि) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        प्रत्यागम XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    ध्रुव अचिह्नित वर्ण *p =
        (ध्रुव अचिह्नित वर्ण *)caller_दत्त + callee_meaningful_अष्टकानि;
    ध्रुव अचिह्नित वर्ण *अन्तः = (ध्रुव अचिह्नित वर्ण *)caller_दत्त + caller_आकार;

    यावत् (p < अन्तः) {
        यदि (*(p++) != 0)
            प्रत्यागम XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    प्रत्यागम XKB_SUCCESS;
}

#परिभाषय xkb_check_versioned_struct_आकार_2(v1_आकार, min_आकार, \
                                        callee_meaningful_अष्टकानि, x) \
    xkb_check_versioned_struct_आकार(v1_आकार, min_आकार, \
                                     callee_meaningful_अष्टकानि, \
                                     परिमाण(*(x)), (x)->आकार, x)

शून्य
xkb_log_abi_error(संरचना xkb_context * सीमित ctx,
                  ध्रुव वर्ण * सीमित func, गणना xkb_स्थितिः स्थितिः);
