/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "xkbcommon/xkbcommon.h"

क्रमागत {
    /** Use a reasonable upper size limit to avoid malicious caller values */
    XKB_ABI_MAX_आकार = 4096,
};

/**
 * Check that field `f` in compound type `T` must be contiguous with either the
 * next field (3rd argument) or the struct end (when using only 2 arguments).
 */
#घोषणा assert_no_padding(T, f, ...) static_assert(      \
    दुरत्व(T, f) + माप(((T){0}).f) ==               \
    __VA_OPT__(दुरत्व(T, __VA_ARGS__) + 0 *) माप(T),\
    #T " has implicit padding after " #f                 \
)

/**
 * Check that the offset, size and alignment of a field are identical
 * in 2 structs.
 */
#घोषणा assert_same_field(T, suffix, f) static_assert(                         \
    दुरत्व(T, f) == दुरत्व(T##suffix, f) &&                                \
    माप(((T){0}).f) == माप(((T##suffix){0}).f) &&                        \
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
अटल अंतरभूत क्रमागत xkb_status
xkb_check_versioned_struct_आकार(माप_प्रकार v1_आकार, माप_प्रकार min_आकार,
                                 माप_प्रकार callee_meaningful_bytes,
                                 माप_प्रकार callee_आकार, माप_प्रकार caller_आकार,
                                 स्थिर व्योम *caller_तथ्य)
{
    अटल स्थिर माप_प्रकार max_आकार = (माप_प्रकार)XKB_ABI_MAX_आकार;

    /* Check internal misuse */
    निश्चित(v1_आकार <= min_आकार);
    निश्चित(min_आकार <= callee_आकार);
    निश्चित(callee_meaningful_bytes <= callee_आकार);
    निश्चित(callee_आकार <= max_आकार);

    अगर (caller_आकार < v1_आकार || caller_आकार > max_आकार || !caller_तथ्य)
        वापस XKB_ERROR_ABI_INVALID_STRUCT_आकार;

    अगर (caller_आकार < min_आकार)
        वापस XKB_ERROR_ABI_BACKWARD_COMPAT;

    अगर (caller_आकार <= callee_meaningful_bytes) {
        /*
         * Caller's data does not reach beyond the callee meaningful region:
         * nothing to check, missing bytes default to 0.
         *
         * E.g.: caller has older or same callee struct, without explicit
         * trailing padding.
         */
        वापस XKB_SUCCESS;
    }

    /*
     * Reserved and/or unknown trailing bytes must be zero, whether they
     * fall inside the callee struct size or beyond it.
     *
     * E.g.: callee struct with explicit trailing padding, new struct using
     * callee reserved field, caller newer struct with greater size.
     */
    स्थिर अचिन्हित अक्षर *p =
        (स्थिर अचिन्हित अक्षर *)caller_तथ्य + callee_meaningful_bytes;
    स्थिर अचिन्हित अक्षर *end = (स्थिर अचिन्हित अक्षर *)caller_तथ्य + caller_आकार;

    जबतक (p < end) {
        अगर (*(p++) != 0)
            वापस XKB_ERROR_ABI_FORWARD_COMPAT;
    }

    वापस XKB_SUCCESS;
}

#घोषणा xkb_check_versioned_struct_आकार_2(v1_आकार, min_आकार, \
                                        callee_meaningful_bytes, x) \
    xkb_check_versioned_struct_आकार(v1_आकार, min_आकार, \
                                     callee_meaningful_bytes, \
                                     माप(*(x)), (x)->आकार, x)

व्योम
xkb_log_abi_error(काष्ठा xkb_context * restrict ctx,
                  स्थिर अक्षर * restrict func, क्रमागत xkb_status status);
