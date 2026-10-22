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

#సూచన once

#అన్తర్భావ "config.h"

#నిర్వచితమైతే __has_include
#నిర్వచించు __ckd_has_include(x) __has_include(x)
#లేకపోతే
#నిర్వచించు __ckd_has_include(x) 0
#అయితే_ముగింపు

#ఒకవేళ __ckd_has_include(<stdckdint.h>)
#అన్తర్భావ <stdckdint.h>
#లేకపోతే

#నిర్వచించు __STDC_VERSION_STDCKDINT_H__ 202311L

#ఒకవేళ (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#నిర్వచించు __ckd_have_int128
#నిర్వచించు __ckd_intmax __int128
#లేదావేళ (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#నిర్వచించు __ckd_intmax దీర్ఘ దీర్ఘ
#లేకపోతే
#నిర్వచించు __ckd_intmax దీర్ఘ
#అయితే_ముగింపు

రకనిర్వచనం గుర్తుగల __ckd_intmax __ckd_intmax_t;
రకనిర్వచనం గుర్తులేని __ckd_intmax __ckd_uintmax_t;

#నిర్వచితమైతే __has_builtin
#నిర్వచించు __ckd_has_builtin(x) __has_builtin(x)
#లేకపోతే
#నిర్వచించు __ckd_has_builtin(x) 0
#అయితే_ముగింపు

#ఒకవేళ (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#నిర్వచించు ckd_కూడు(res, x, y) __builtin_add_overflow((x), (y), (res))
#నిర్వచించు ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#నిర్వచించు ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#లేదావేళ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#నిర్వచించు ckd_కూడు(res, a, b) __ckd_expr(కూడు, (res), (a), (b))
#నిర్వచించు ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#నిర్వచించు ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#ఒకవేళ defined(__GNUC__) || defined(__llvm__)
#నిర్వచించు __ckd_inline                                    \
  బాహ్య __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#లేకపోతే
#నిర్వచించు __ckd_inline స్థిర అంతఃవరుస
#అయితే_ముగింపు

#నిర్వచితమైతే __ckd_have_int128
#నిర్వచించు __ckd_generic_int128(x, y) , గుర్తుగల __int128: x, గుర్తులేని __int128: y
#లేకపోతే
#నిర్వచించు __ckd_generic_int128(x, y)
#అయితే_ముగింపు

#నిర్వచించు __ckd_sign(T)                           \
  ((T)1 << (పరిమాణం(T) * 8 - 1))

#నిర్వచించు __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           గుర్తుగల అక్షరం: 1,                      \
           గుర్తులేని అక్షరం: 0,                    \
           గుర్తుగల పొట్టి: 1,                     \
           గుర్తులేని పొట్టి: 0,                   \
           గుర్తుగల పూర్ణాంకం: 1,                       \
           గుర్తులేని పూర్ణాంకం: 0,                     \
           గుర్తుగల దీర్ఘ: 1,                      \
           గుర్తులేని దీర్ఘ: 0,                    \
           గుర్తుగల దీర్ఘ దీర్ఘ: 1,                 \
           గుర్తులేని దీర్ఘ దీర్ఘ: 0                \
           __ckd_generic_int128(1, 0))

#నిర్వచించు __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            గుర్తుగల అక్షరం: __ckd_##op##_schar,            \
            గుర్తులేని అక్షరం: __ckd_##op##_uchar,          \
            గుర్తుగల పొట్టి: __ckd_##op##_sshort,          \
            గుర్తులేని పొట్టి: __ckd_##op##_ushort,        \
            గుర్తుగల పూర్ణాంకం: __ckd_##op##_sint,              \
            గుర్తులేని పూర్ణాంకం: __ckd_##op##_uint,            \
            గుర్తుగల దీర్ఘ: __ckd_##op##_slong,            \
            గుర్తులేని దీర్ఘ: __ckd_##op##_ulong,          \
            గుర్తుగల దీర్ఘ దీర్ఘ: __ckd_##op##_slonger,     \
            గుర్తులేని దీర్ఘ దీర్ఘ: __ckd_##op##_ulonger    \
            __ckd_generic_int128(                       \
                __ckd_##op##_sint128,                   \
                __ckd_##op##_uint128))(                 \
                    res, a, b,                          \
                    __ckd_is_signed(a),                 \
                    __ckd_is_signed(b)))

#నిర్వచించు __ckd_declare_add(S, T)                                         \
  __ckd_inline అక్షరం S(శూన్య *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      అక్షరం __a_signed,                                  \
                      అక్షరం __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    అక్షరం __truncated = 0;                                               \
    ఒకవేళ (పరిమాణం(T) < పరిమాణం(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ఎంపిక (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      స్థితి 0:  /* u = u + u */                                          \
        తిరిగి __truncated | (__z < __x);                               \
      స్థితి 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      స్థితి 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      స్థితి 3:  /* u = s + s */                                          \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      స్థితి 4:  /* s = u + u */                                          \
        తిరిగి __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      స్థితి 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated | (__x + __y < __y);                         \
      స్థితి 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated | (__x + __y < __x);                         \
      స్థితి 7:  /* s = s + s */                                          \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      అప్రమేయ:                                                          \
        కొరకు (;;) (శూన్య)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, గుర్తుగల అక్షరం)
__ckd_declare_add(__ckd_add_uchar, గుర్తులేని అక్షరం)
__ckd_declare_add(__ckd_add_sshort, గుర్తుగల పొట్టి)
__ckd_declare_add(__ckd_add_ushort, గుర్తులేని పొట్టి)
__ckd_declare_add(__ckd_add_sint, గుర్తుగల పూర్ణాంకం)
__ckd_declare_add(__ckd_add_uint, గుర్తులేని పూర్ణాంకం)
__ckd_declare_add(__ckd_add_slong, గుర్తుగల దీర్ఘ)
__ckd_declare_add(__ckd_add_ulong, గుర్తులేని దీర్ఘ)
__ckd_declare_add(__ckd_add_slonger, గుర్తుగల దీర్ఘ దీర్ఘ)
__ckd_declare_add(__ckd_add_ulonger, గుర్తులేని దీర్ఘ దీర్ఘ)
#నిర్వచితమైతే __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, గుర్తుగల __int128)
__ckd_declare_add(__ckd_add_uint128, గుర్తులేని __int128)
#అయితే_ముగింపు

#నిర్వచించు __ckd_declare_sub(S, T)                                         \
  __ckd_inline అక్షరం S(శూన్య *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      అక్షరం __a_signed,                                  \
                      అక్షరం __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    అక్షరం __truncated = 0;                                               \
    ఒకవేళ (పరిమాణం(T) < పరిమాణం(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ఎంపిక (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      స్థితి 0:  /* u = u - u */                                          \
        తిరిగి __truncated | (__x < __y);                               \
      స్థితి 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      స్థితి 2:  /* u = s - u */                                          \
        తిరిగి __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      స్థితి 3:  /* u = s - s */                                          \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      స్థితి 4:  /* s = u - u */                                          \
        తిరిగి __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      స్థితి 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated | (__x >= __y);                              \
      స్థితి 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        తిరిగి __truncated | (__x < __y);                               \
      స్థితి 7:  /* s = s - s */                                          \
        తిరిగి __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      అప్రమేయ:                                                          \
        కొరకు (;;) (శూన్య)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, గుర్తుగల అక్షరం)
__ckd_declare_sub(__ckd_sub_uchar, గుర్తులేని అక్షరం)
__ckd_declare_sub(__ckd_sub_sshort, గుర్తుగల పొట్టి)
__ckd_declare_sub(__ckd_sub_ushort, గుర్తులేని పొట్టి)
__ckd_declare_sub(__ckd_sub_sint, గుర్తుగల పూర్ణాంకం)
__ckd_declare_sub(__ckd_sub_uint, గుర్తులేని పూర్ణాంకం)
__ckd_declare_sub(__ckd_sub_slong, గుర్తుగల దీర్ఘ)
__ckd_declare_sub(__ckd_sub_ulong, గుర్తులేని దీర్ఘ)
__ckd_declare_sub(__ckd_sub_slonger, గుర్తుగల దీర్ఘ దీర్ఘ)
__ckd_declare_sub(__ckd_sub_ulonger, గుర్తులేని దీర్ఘ దీర్ఘ)
#నిర్వచితమైతే __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, గుర్తుగల __int128)
__ckd_declare_sub(__ckd_sub_uint128, గుర్తులేని __int128)
#అయితే_ముగింపు

#నిర్వచించు __ckd_declare_mul(S, T)                                 \
  __ckd_inline అక్షరం S(శూన్య *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      అక్షరం __a_signed,                          \
                      అక్షరం __b_signed) {                        \
    ఎంపిక (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      స్థితి 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        పూర్ణాంకం __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (__o | (పరిమాణం(T) < పరిమాణం(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      స్థితి 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        పూర్ణాంకం __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      స్థితి 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        పూర్ణాంకం __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      స్థితి 3: {  /* u = s * s */                                \
        పూర్ణాంకం __o = 0;                                            \
        ఒకవేళ ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } లేకపోతే ఒకవేళ ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (__o | (పరిమాణం(T) < పరిమాణం(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      స్థితి 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        పూర్ణాంకం __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      స్థితి 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        పూర్ణాంకం __o = __t && __p / __t != __x;                      \
        పూర్ణాంకం __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        తిరిగి (అక్షరం)                                           \
               (__o | (__p > __m + __n) |                       \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      స్థితి 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        పూర్ణాంకం __o = __t && __p / __t != __y;                      \
        పూర్ణాంకం __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        తిరిగి (అక్షరం)                                           \
               (__o | (__p > __m + __n) |                       \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      స్థితి 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        తిరిగి (అక్షరం)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (పరిమాణం(T) < పరిమాణం(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      అప్రమేయ:                                                  \
        కొరకు (;;) (శూన్య)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, గుర్తుగల అక్షరం)
__ckd_declare_mul(__ckd_mul_uchar, గుర్తులేని అక్షరం)
__ckd_declare_mul(__ckd_mul_sshort, గుర్తుగల పొట్టి)
__ckd_declare_mul(__ckd_mul_ushort, గుర్తులేని పొట్టి)
__ckd_declare_mul(__ckd_mul_sint, గుర్తుగల పూర్ణాంకం)
__ckd_declare_mul(__ckd_mul_uint, గుర్తులేని పూర్ణాంకం)
__ckd_declare_mul(__ckd_mul_slong, గుర్తుగల దీర్ఘ)
__ckd_declare_mul(__ckd_mul_ulong, గుర్తులేని దీర్ఘ)
__ckd_declare_mul(__ckd_mul_slonger, గుర్తుగల దీర్ఘ దీర్ఘ)
__ckd_declare_mul(__ckd_mul_ulonger, గుర్తులేని దీర్ఘ దీర్ఘ)
#నిర్వచితమైతే __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, గుర్తుగల __int128)
__ckd_declare_mul(__ckd_mul_uint128, గుర్తులేని __int128)
#అయితే_ముగింపు

#లేకపోతే
#నిర్వచితమైతే _MSC_VER
#సూచన warning("checked integer arithmetic unsupported in this environment")
#లేకపోతే
#సూచన GCC warning "checked integer arithmetic unsupported in this environment"
#అయితే_ముగింపు

#నిర్వచించు ckd_కూడు(res, x, y) (*(res) = (x) + (y), 0)
#నిర్వచించు ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#నిర్వచించు ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#అయితే_ముగింపు /* GNU */
#అయితే_ముగింపు /* stdckdint.h */
