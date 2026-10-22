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

#pragma once

#अन्तर्भाव "config.h"

#ifdef __has_include
#define __ckd_has_include(x) __has_include(x)
#अण्णहा
#define __ckd_has_include(x) 0
#endif

#जइ __ckd_has_include(<stdckdint.h>)
#अन्तर्भाव <stdckdint.h>
#अण्णहा

#define __STDC_VERSION_STDCKDINT_H__ 202311L

#जइ (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#define __ckd_have_int128
#define __ckd_intmax __int128
#नोचेद्यदि (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __ckd_intmax long long
#अण्णहा
#define __ckd_intmax long
#endif

प्रकारनिर्देश signed __ckd_intmax __ckd_intmax_t;
प्रकारनिर्देश unsigned __ckd_intmax __ckd_uintmax_t;

#ifdef __has_builtin
#define __ckd_has_builtin(x) __has_builtin(x)
#अण्णहा
#define __ckd_has_builtin(x) 0
#endif

#जइ (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#define ckd_add(res, x, y) __builtin_add_overflow((x), (y), (res))
#define ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#define ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#नोचेद्यदि defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define ckd_add(res, a, b) __ckd_expr(add, (res), (a), (b))
#define ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#define ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#जइ defined(__GNUC__) || defined(__llvm__)
#define __ckd_inline                                    \
  बाह्य __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#अण्णहा
#define __ckd_inline स्थिर अन्तःस्थ
#endif

#ifdef __ckd_have_int128
#define __ckd_generic_int128(x, y) , signed __int128: x, unsigned __int128: y
#अण्णहा
#define __ckd_generic_int128(x, y)
#endif

#define __ckd_sign(T)                           \
  ((T)1 << (परिमाण(T) * 8 - 1))

#define __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           signed वर्ण: 1,                      \
           unsigned वर्ण: 0,                    \
           signed short: 1,                     \
           unsigned short: 0,                   \
           signed पूर्णाङ्क: 1,                       \
           unsigned पूर्णाङ्क: 0,                     \
           signed long: 1,                      \
           unsigned long: 0,                    \
           signed long long: 1,                 \
           unsigned long long: 0                \
           __ckd_generic_int128(1, 0))

#define __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            signed वर्ण: __ckd_##op##_schar,            \
            unsigned वर्ण: __ckd_##op##_uchar,          \
            signed short: __ckd_##op##_sshort,          \
            unsigned short: __ckd_##op##_ushort,        \
            signed पूर्णाङ्क: __ckd_##op##_sint,              \
            unsigned पूर्णाङ्क: __ckd_##op##_uint,            \
            signed long: __ckd_##op##_slong,            \
            unsigned long: __ckd_##op##_ulong,          \
            signed long long: __ckd_##op##_slonger,     \
            unsigned long long: __ckd_##op##_ulonger    \
            __ckd_generic_int128(                       \
                __ckd_##op##_sint128,                   \
                __ckd_##op##_uint128))(                 \
                    res, a, b,                          \
                    __ckd_is_signed(a),                 \
                    __ckd_is_signed(b)))

#define __ckd_declare_add(S, T)                                         \
  __ckd_inline वर्ण S(शून्य *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      वर्ण __a_signed,                                  \
                      वर्ण __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    वर्ण __truncated = 0;                                               \
    जइ (परिमाण(T) < परिमाण(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    विकल्प (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      पक्ष 0:  /* u = u + u */                                          \
        प्रत्यागम __truncated | (__z < __x);                               \
      पक्ष 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      पक्ष 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      पक्ष 3:  /* u = s + s */                                          \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      पक्ष 4:  /* s = u + u */                                          \
        प्रत्यागम __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      पक्ष 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated | (__x + __y < __y);                         \
      पक्ष 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated | (__x + __y < __x);                         \
      पक्ष 7:  /* s = s + s */                                          \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      स्वभाव:                                                          \
        प्रति (;;) (शून्य)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, signed वर्ण)
__ckd_declare_add(__ckd_add_uchar, unsigned वर्ण)
__ckd_declare_add(__ckd_add_sshort, signed short)
__ckd_declare_add(__ckd_add_ushort, unsigned short)
__ckd_declare_add(__ckd_add_sint, signed पूर्णाङ्क)
__ckd_declare_add(__ckd_add_uint, unsigned पूर्णाङ्क)
__ckd_declare_add(__ckd_add_slong, signed long)
__ckd_declare_add(__ckd_add_ulong, unsigned long)
__ckd_declare_add(__ckd_add_slonger, signed long long)
__ckd_declare_add(__ckd_add_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, signed __int128)
__ckd_declare_add(__ckd_add_uint128, unsigned __int128)
#endif

#define __ckd_declare_sub(S, T)                                         \
  __ckd_inline वर्ण S(शून्य *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      वर्ण __a_signed,                                  \
                      वर्ण __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    वर्ण __truncated = 0;                                               \
    जइ (परिमाण(T) < परिमाण(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    विकल्प (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      पक्ष 0:  /* u = u - u */                                          \
        प्रत्यागम __truncated | (__x < __y);                               \
      पक्ष 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      पक्ष 2:  /* u = s - u */                                          \
        प्रत्यागम __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      पक्ष 3:  /* u = s - s */                                          \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      पक्ष 4:  /* s = u - u */                                          \
        प्रत्यागम __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      पक्ष 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated | (__x >= __y);                              \
      पक्ष 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        प्रत्यागम __truncated | (__x < __y);                               \
      पक्ष 7:  /* s = s - s */                                          \
        प्रत्यागम __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      स्वभाव:                                                          \
        प्रति (;;) (शून्य)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, signed वर्ण)
__ckd_declare_sub(__ckd_sub_uchar, unsigned वर्ण)
__ckd_declare_sub(__ckd_sub_sshort, signed short)
__ckd_declare_sub(__ckd_sub_ushort, unsigned short)
__ckd_declare_sub(__ckd_sub_sint, signed पूर्णाङ्क)
__ckd_declare_sub(__ckd_sub_uint, unsigned पूर्णाङ्क)
__ckd_declare_sub(__ckd_sub_slong, signed long)
__ckd_declare_sub(__ckd_sub_ulong, unsigned long)
__ckd_declare_sub(__ckd_sub_slonger, signed long long)
__ckd_declare_sub(__ckd_sub_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, signed __int128)
__ckd_declare_sub(__ckd_sub_uint128, unsigned __int128)
#endif

#define __ckd_declare_mul(S, T)                                 \
  __ckd_inline वर्ण S(शून्य *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      वर्ण __a_signed,                          \
                      वर्ण __b_signed) {                        \
    विकल्प (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      पक्ष 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णाङ्क __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (__o | (परिमाण(T) < परिमाण(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      पक्ष 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णाङ्क __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पक्ष 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णाङ्क __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पक्ष 3: {  /* u = s * s */                                \
        पूर्णाङ्क __o = 0;                                            \
        जइ ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } अण्णहा जइ ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (__o | (परिमाण(T) < परिमाण(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      पक्ष 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णाङ्क __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पक्ष 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        पूर्णाङ्क __o = __t && __p / __t != __x;                      \
        पूर्णाङ्क __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        प्रत्यागम (वर्ण)                                           \
               (__o | (__p > __m + __n) |                       \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पक्ष 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        पूर्णाङ्क __o = __t && __p / __t != __y;                      \
        पूर्णाङ्क __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        प्रत्यागम (वर्ण)                                           \
               (__o | (__p > __m + __n) |                       \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पक्ष 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        प्रत्यागम (वर्ण)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (परिमाण(T) < परिमाण(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्वभाव:                                                  \
        प्रति (;;) (शून्य)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, signed वर्ण)
__ckd_declare_mul(__ckd_mul_uchar, unsigned वर्ण)
__ckd_declare_mul(__ckd_mul_sshort, signed short)
__ckd_declare_mul(__ckd_mul_ushort, unsigned short)
__ckd_declare_mul(__ckd_mul_sint, signed पूर्णाङ्क)
__ckd_declare_mul(__ckd_mul_uint, unsigned पूर्णाङ्क)
__ckd_declare_mul(__ckd_mul_slong, signed long)
__ckd_declare_mul(__ckd_mul_ulong, unsigned long)
__ckd_declare_mul(__ckd_mul_slonger, signed long long)
__ckd_declare_mul(__ckd_mul_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, signed __int128)
__ckd_declare_mul(__ckd_mul_uint128, unsigned __int128)
#endif

#अण्णहा
#ifdef _MSC_VER
#pragma warning("checked integer arithmetic unsupported in this environment")
#अण्णहा
#pragma GCC warning "checked integer arithmetic unsupported in this environment"
#endif

#define ckd_add(res, x, y) (*(res) = (x) + (y), 0)
#define ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#define ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#endif /* GNU */
#endif /* stdckdint.h */
