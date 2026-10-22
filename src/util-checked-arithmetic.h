/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright 2023 Justine Alexandra Roberts Tunney
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @fileoverview C23 Checked Arithmetic
 *
 * This header defines three type generic functions:
 *
 *   - `bool ckd_add(res, a, b)`
 *   - `bool ckd_sub(res, a, b)`
 *   - `bool ckd_mul(res, a, b)`
 *
 * Which allow integer arithmetic errors to be detected. There are many
 * kinds of integer errors, e.g. overflow, truncation, etc. These funcs
 * catch them all. Here's an example of how it works:
 *
 *     uint32_t c;
 *     int32_t a = 0x7fffffff;
 *     int32_t b = 2;
 *     assert(!ckd_add(&c, a, b));
 *     assert(c == 0x80000001u);
 *
 * Experienced C / C++ users should find this example counter-intuitive
 * because the expression `0x7fffffff + 2` not only overflows it's also
 * undefined behavior. However here we see it's specified, and does not
 * result in an error. That's because C23 checked arithmetic is not the
 * arithmetic you're used to. The new standard changes the mathematics.
 *
 * C23 checked arithmetic is defined as performing the arithmetic using
 * infinite precision and then checking if the resulting value will fit
 * in the output type. Our example above did not result in an error due
 * to `0x80000001` being a legal value for `uint32_t`.
 *
 * This implementation will use the GNU compiler builtins, when they're
 * available, only if you don't use build flags like `-std=c11` because
 * they define `__STRICT_ANSI__` and GCC extensions aren't really ANSI.
 * Instead, you'll get a pretty good pure C11 and C++11 implementation.
 *
 * @sa https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
 * @version 1.0 (2024-12-07)
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __has_include
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_has_include(x) __has_include(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_has_include(x) 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ __ckd_has_include(<stdckdint.h>)
#ಅನ್ತರ್ಭಾವ <stdckdint.h>
#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿಸು __STDC_VERSION_STDCKDINT_H__ 202311L

#ಆದರೆ (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_have_int128
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_intmax __int128
#ಅಥವಾಆದರೆ (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_intmax ದೀರ್ಘ ದೀರ್ಘ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_intmax ದೀರ್ಘ
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ __ckd_intmax __ckd_intmax_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ __ckd_intmax __ckd_uintmax_t;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __has_builtin
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_has_builtin(x) __has_builtin(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_has_builtin(x) 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_ಕೂಡಿಸು(res, x, y) __builtin_add_overflow((x), (y), (res))
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#ಅಥವಾಆದರೆ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#ವ್ಯಾಖ್ಯಾನಿಸು ckd_ಕೂಡಿಸು(res, a, b) __ckd_expr(ಕೂಡಿಸು, (res), (a), (b))
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#ಆದರೆ defined(__GNUC__) || defined(__llvm__)
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_inline                                    \
  ಬಾಹ್ಯ __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_inline ಸ್ಥಿರ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ckd_have_int128
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_generic_int128(x, y) , ಚಿಹ್ನೆಯುಳ್ಳ __int128: x, ಚಿಹ್ನೆರಹಿತ __int128: y
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_generic_int128(x, y)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_sign(T)                           \
  ((T)1 << (ಗಾತ್ರ(T) * 8 - 1))

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ: 1,                      \
           ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ: 0,                    \
           ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ: 1,                     \
           ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ: 0,                   \
           ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ: 1,                       \
           ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ: 0,                     \
           ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ: 1,                      \
           ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ: 0,                    \
           ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ ದೀರ್ಘ: 1,                 \
           ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ: 0                \
           __ckd_generic_int128(1, 0))

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ: __ckd_##op##_schar,            \
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ: __ckd_##op##_uchar,          \
            ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ: __ckd_##op##_sshort,          \
            ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ: __ckd_##op##_ushort,        \
            ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ: __ckd_##op##_sint,              \
            ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ: __ckd_##op##_uint,            \
            ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ: __ckd_##op##_slong,            \
            ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ: __ckd_##op##_ulong,          \
            ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ ದೀರ್ಘ: __ckd_##op##_slonger,     \
            ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ: __ckd_##op##_ulonger    \
            __ckd_generic_int128(                       \
                __ckd_##op##_sint128,                   \
                __ckd_##op##_uint128))(                 \
                    res, a, b,                          \
                    __ckd_is_signed(a),                 \
                    __ckd_is_signed(b)))

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_declare_add(S, T)                                         \
  __ckd_inline ಅಕ್ಷರ S(ಶೂನ್ಯ *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      ಅಕ್ಷರ __a_signed,                                  \
                      ಅಕ್ಷರ __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    ಅಕ್ಷರ __truncated = 0;                                               \
    ಆದರೆ (ಗಾತ್ರ(T) < ಗಾತ್ರ(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ಆಯ್ಕೆ (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      ಸ್ಥಿತಿ 0:  /* u = u + u */                                          \
        ಹಿಂತಿರುಗಿ __truncated | (__z < __x);                               \
      ಸ್ಥಿತಿ 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ಸ್ಥಿತಿ 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ಸ್ಥಿತಿ 3:  /* u = s + s */                                          \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      ಸ್ಥಿತಿ 4:  /* s = u + u */                                          \
        ಹಿಂತಿರುಗಿ __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      ಸ್ಥಿತಿ 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated | (__x + __y < __y);                         \
      ಸ್ಥಿತಿ 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated | (__x + __y < __x);                         \
      ಸ್ಥಿತಿ 7:  /* s = s + s */                                          \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ಪೂರ್ವನಿಯೋಜಿತ:                                                          \
        ಗಾಗಿ (;;) (ಶೂನ್ಯ)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)
__ckd_declare_add(__ckd_add_uchar, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)
__ckd_declare_add(__ckd_add_sshort, ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ)
__ckd_declare_add(__ckd_add_ushort, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ)
__ckd_declare_add(__ckd_add_sint, ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ)
__ckd_declare_add(__ckd_add_uint, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)
__ckd_declare_add(__ckd_add_slong, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ)
__ckd_declare_add(__ckd_add_ulong, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)
__ckd_declare_add(__ckd_add_slonger, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ ದೀರ್ಘ)
__ckd_declare_add(__ckd_add_ulonger, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, ಚಿಹ್ನೆಯುಳ್ಳ __int128)
__ckd_declare_add(__ckd_add_uint128, ಚಿಹ್ನೆರಹಿತ __int128)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_declare_sub(S, T)                                         \
  __ckd_inline ಅಕ್ಷರ S(ಶೂನ್ಯ *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      ಅಕ್ಷರ __a_signed,                                  \
                      ಅಕ್ಷರ __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    ಅಕ್ಷರ __truncated = 0;                                               \
    ಆದರೆ (ಗಾತ್ರ(T) < ಗಾತ್ರ(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ಆಯ್ಕೆ (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      ಸ್ಥಿತಿ 0:  /* u = u - u */                                          \
        ಹಿಂತಿರುಗಿ __truncated | (__x < __y);                               \
      ಸ್ಥಿತಿ 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      ಸ್ಥಿತಿ 2:  /* u = s - u */                                          \
        ಹಿಂತಿರುಗಿ __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      ಸ್ಥಿತಿ 3:  /* u = s - s */                                          \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      ಸ್ಥಿತಿ 4:  /* s = u - u */                                          \
        ಹಿಂತಿರುಗಿ __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      ಸ್ಥಿತಿ 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated | (__x >= __y);                              \
      ಸ್ಥಿತಿ 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ಹಿಂತಿರುಗಿ __truncated | (__x < __y);                               \
      ಸ್ಥಿತಿ 7:  /* s = s - s */                                          \
        ಹಿಂತಿರುಗಿ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      ಪೂರ್ವನಿಯೋಜಿತ:                                                          \
        ಗಾಗಿ (;;) (ಶೂನ್ಯ)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)
__ckd_declare_sub(__ckd_sub_uchar, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)
__ckd_declare_sub(__ckd_sub_sshort, ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ)
__ckd_declare_sub(__ckd_sub_ushort, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ)
__ckd_declare_sub(__ckd_sub_sint, ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ)
__ckd_declare_sub(__ckd_sub_uint, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)
__ckd_declare_sub(__ckd_sub_slong, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ)
__ckd_declare_sub(__ckd_sub_ulong, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)
__ckd_declare_sub(__ckd_sub_slonger, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ ದೀರ್ಘ)
__ckd_declare_sub(__ckd_sub_ulonger, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, ಚಿಹ್ನೆಯುಳ್ಳ __int128)
__ckd_declare_sub(__ckd_sub_uint128, ಚಿಹ್ನೆರಹಿತ __int128)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು __ckd_declare_mul(S, T)                                 \
  __ckd_inline ಅಕ್ಷರ S(ಶೂನ್ಯ *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      ಅಕ್ಷರ __a_signed,                          \
                      ಅಕ್ಷರ __b_signed) {                        \
    ಆಯ್ಕೆ (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      ಸ್ಥಿತಿ 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ಪೂರ್ಣಾಂಕ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      ಸ್ಥಿತಿ 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ಪೂರ್ಣಾಂಕ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಸ್ಥಿತಿ 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ಪೂರ್ಣಾಂಕ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಸ್ಥಿತಿ 3: {  /* u = s * s */                                \
        ಪೂರ್ಣಾಂಕ __o = 0;                                            \
        ಆದರೆ ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      ಸ್ಥಿತಿ 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ಪೂರ್ಣಾಂಕ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಸ್ಥಿತಿ 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        ಪೂರ್ಣಾಂಕ __o = __t && __p / __t != __x;                      \
        ಪೂರ್ಣಾಂಕ __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (__p > __m + __n) |                       \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಸ್ಥಿತಿ 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        ಪೂರ್ಣಾಂಕ __o = __t && __p / __t != __y;                      \
        ಪೂರ್ಣಾಂಕ __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (__o | (__p > __m + __n) |                       \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಸ್ಥಿತಿ 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (ಗಾತ್ರ(T) < ಗಾತ್ರ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ಪೂರ್ವನಿಯೋಜಿತ:                                                  \
        ಗಾಗಿ (;;) (ಶೂನ್ಯ)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)
__ckd_declare_mul(__ckd_mul_uchar, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)
__ckd_declare_mul(__ckd_mul_sshort, ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ)
__ckd_declare_mul(__ckd_mul_ushort, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ)
__ckd_declare_mul(__ckd_mul_sint, ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ)
__ckd_declare_mul(__ckd_mul_uint, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)
__ckd_declare_mul(__ckd_mul_slong, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ)
__ckd_declare_mul(__ckd_mul_ulong, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)
__ckd_declare_mul(__ckd_mul_slonger, ಚಿಹ್ನೆಯುಳ್ಳ ದೀರ್ಘ ದೀರ್ಘ)
__ckd_declare_mul(__ckd_mul_ulonger, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, ಚಿಹ್ನೆಯುಳ್ಳ __int128)
__ckd_declare_mul(__ckd_mul_uint128, ಚಿಹ್ನೆರಹಿತ __int128)
#ಆದರೆ_ಅಂತ್ಯ

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning("checked integer arithmetic unsupported in this environment")
#ಇಲ್ಲದಿದ್ದರೆ
#ಸೂಚನೆ GCC warning "checked integer arithmetic unsupported in this environment"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ckd_ಕೂಡಿಸು(res, x, y) (*(res) = (x) + (y), 0)
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#ವ್ಯಾಖ್ಯಾನಿಸು ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#ಆದರೆ_ಅಂತ್ಯ /* GNU */
#ಆದರೆ_ಅಂತ್ಯ /* stdckdint.h */
