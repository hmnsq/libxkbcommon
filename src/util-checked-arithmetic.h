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

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __has_include
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_has_include(x) __has_include(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_has_include(x) 0
#ਜੇ_ਅੰਤ

#ਜੇ __ckd_has_include(<stdckdint.h>)
#ਅਨ੍ਤਰ੍ਭਾਵ <stdckdint.h>
#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __STDC_VERSION_STDCKDINT_H__ 202311L

#ਜੇ (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_have_int128
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_intmax __int128
#ਜਾਂਜੇ (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_intmax ਲੰਮਾ ਲੰਮਾ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_intmax ਲੰਮਾ
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਨਿਸ਼ਾਨੀਵਾਲਾ __ckd_intmax __ckd_intmax_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ __ckd_intmax __ckd_uintmax_t;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __has_builtin
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_has_builtin(x) __has_builtin(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_has_builtin(x) 0
#ਜੇ_ਅੰਤ

#ਜੇ (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_ਜੋੜੋ(res, x, y) __builtin_add_overflow((x), (y), (res))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#ਜਾਂਜੇ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_ਜੋੜੋ(res, a, b) __ckd_expr(ਜੋੜੋ, (res), (a), (b))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#ਜੇ defined(__GNUC__) || defined(__llvm__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_inline                                    \
  ਬਾਹਰੀ __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_inline ਸਥਿਰ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ckd_have_int128
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_generic_int128(x, y) , ਨਿਸ਼ਾਨੀਵਾਲਾ __int128: x, ਬਿਨਾਨਿਸ਼ਾਨੀ __int128: y
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_generic_int128(x, y)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_sign(T)                           \
  ((T)1 << (ਆਕਾਰ(T) * 8 - 1))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ: 1,                      \
           ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ: 0,                    \
           ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ: 1,                     \
           ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ: 0,                   \
           ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ: 1,                       \
           ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ: 0,                     \
           ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ: 1,                      \
           ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ: 0,                    \
           ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ: 1,                 \
           ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ: 0                \
           __ckd_generic_int128(1, 0))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ: __ckd_##op##_schar,            \
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ: __ckd_##op##_uchar,          \
            ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ: __ckd_##op##_sshort,          \
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ: __ckd_##op##_ushort,        \
            ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ: __ckd_##op##_sint,              \
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ: __ckd_##op##_uint,            \
            ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ: __ckd_##op##_slong,            \
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ: __ckd_##op##_ulong,          \
            ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ: __ckd_##op##_slonger,     \
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ: __ckd_##op##_ulonger    \
            __ckd_generic_int128(                       \
                __ckd_##op##_sint128,                   \
                __ckd_##op##_uint128))(                 \
                    res, a, b,                          \
                    __ckd_is_signed(a),                 \
                    __ckd_is_signed(b)))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_declare_add(S, T)                                         \
  __ckd_inline ਅੱਖਰ S(ਖਾਲੀ *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      ਅੱਖਰ __a_signed,                                  \
                      ਅੱਖਰ __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    ਅੱਖਰ __truncated = 0;                                               \
    ਜੇ (ਆਕਾਰ(T) < ਆਕਾਰ(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ਚੋਣ (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      ਹਾਲਤ 0:  /* u = u + u */                                          \
        ਵਾਪਸ __truncated | (__z < __x);                               \
      ਹਾਲਤ 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ਹਾਲਤ 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ਹਾਲਤ 3:  /* u = s + s */                                          \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      ਹਾਲਤ 4:  /* s = u + u */                                          \
        ਵਾਪਸ __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      ਹਾਲਤ 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated | (__x + __y < __y);                         \
      ਹਾਲਤ 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated | (__x + __y < __x);                         \
      ਹਾਲਤ 7:  /* s = s + s */                                          \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      ਮੂਲ:                                                          \
        ਲਈ (;;) (ਖਾਲੀ)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)
__ckd_declare_add(__ckd_add_uchar, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)
__ckd_declare_add(__ckd_add_sshort, ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ)
__ckd_declare_add(__ckd_add_ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ)
__ckd_declare_add(__ckd_add_sint, ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ)
__ckd_declare_add(__ckd_add_uint, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
__ckd_declare_add(__ckd_add_slong, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ)
__ckd_declare_add(__ckd_add_ulong, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)
__ckd_declare_add(__ckd_add_slonger, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ)
__ckd_declare_add(__ckd_add_ulonger, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, ਨਿਸ਼ਾਨੀਵਾਲਾ __int128)
__ckd_declare_add(__ckd_add_uint128, ਬਿਨਾਨਿਸ਼ਾਨੀ __int128)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_declare_sub(S, T)                                         \
  __ckd_inline ਅੱਖਰ S(ਖਾਲੀ *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      ਅੱਖਰ __a_signed,                                  \
                      ਅੱਖਰ __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    ਅੱਖਰ __truncated = 0;                                               \
    ਜੇ (ਆਕਾਰ(T) < ਆਕਾਰ(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    ਚੋਣ (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      ਹਾਲਤ 0:  /* u = u - u */                                          \
        ਵਾਪਸ __truncated | (__x < __y);                               \
      ਹਾਲਤ 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      ਹਾਲਤ 2:  /* u = s - u */                                          \
        ਵਾਪਸ __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      ਹਾਲਤ 3:  /* u = s - s */                                          \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      ਹਾਲਤ 4:  /* s = u - u */                                          \
        ਵਾਪਸ __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      ਹਾਲਤ 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated | (__x >= __y);                              \
      ਹਾਲਤ 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        ਵਾਪਸ __truncated | (__x < __y);                               \
      ਹਾਲਤ 7:  /* s = s - s */                                          \
        ਵਾਪਸ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      ਮੂਲ:                                                          \
        ਲਈ (;;) (ਖਾਲੀ)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)
__ckd_declare_sub(__ckd_sub_uchar, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)
__ckd_declare_sub(__ckd_sub_sshort, ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ)
__ckd_declare_sub(__ckd_sub_ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ)
__ckd_declare_sub(__ckd_sub_sint, ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ)
__ckd_declare_sub(__ckd_sub_uint, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
__ckd_declare_sub(__ckd_sub_slong, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ)
__ckd_declare_sub(__ckd_sub_ulong, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)
__ckd_declare_sub(__ckd_sub_slonger, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ)
__ckd_declare_sub(__ckd_sub_ulonger, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, ਨਿਸ਼ਾਨੀਵਾਲਾ __int128)
__ckd_declare_sub(__ckd_sub_uint128, ਬਿਨਾਨਿਸ਼ਾਨੀ __int128)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ckd_declare_mul(S, T)                                 \
  __ckd_inline ਅੱਖਰ S(ਖਾਲੀ *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      ਅੱਖਰ __a_signed,                          \
                      ਅੱਖਰ __b_signed) {                        \
    ਚੋਣ (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      ਹਾਲਤ 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ਪੂਰਨ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      ਹਾਲਤ 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ਪੂਰਨ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਹਾਲਤ 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ਪੂਰਨ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਹਾਲਤ 3: {  /* u = s * s */                                \
        ਪੂਰਨ __o = 0;                                            \
        ਜੇ ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } ਨਹੀਂਤਾਂ ਜੇ ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      ਹਾਲਤ 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        ਪੂਰਨ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਹਾਲਤ 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        ਪੂਰਨ __o = __t && __p / __t != __x;                      \
        ਪੂਰਨ __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (__p > __m + __n) |                       \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਹਾਲਤ 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        ਪੂਰਨ __o = __t && __p / __t != __y;                      \
        ਪੂਰਨ __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (__o | (__p > __m + __n) |                       \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਹਾਲਤ 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        ਵਾਪਸ (ਅੱਖਰ)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (ਆਕਾਰ(T) < ਆਕਾਰ(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      ਮੂਲ:                                                  \
        ਲਈ (;;) (ਖਾਲੀ)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)
__ckd_declare_mul(__ckd_mul_uchar, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)
__ckd_declare_mul(__ckd_mul_sshort, ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ)
__ckd_declare_mul(__ckd_mul_ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ)
__ckd_declare_mul(__ckd_mul_sint, ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ)
__ckd_declare_mul(__ckd_mul_uint, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
__ckd_declare_mul(__ckd_mul_slong, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ)
__ckd_declare_mul(__ckd_mul_ulong, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)
__ckd_declare_mul(__ckd_mul_slonger, ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ)
__ckd_declare_mul(__ckd_mul_ulonger, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, ਨਿਸ਼ਾਨੀਵਾਲਾ __int128)
__ckd_declare_mul(__ckd_mul_uint128, ਬਿਨਾਨਿਸ਼ਾਨੀ __int128)
#ਜੇ_ਅੰਤ

#ਨਹੀਂਤਾਂ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning("checked integer arithmetic unsupported in this environment")
#ਨਹੀਂਤਾਂ
#ਹਦਾਇਤ GCC warning "checked integer arithmetic unsupported in this environment"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_ਜੋੜੋ(res, x, y) (*(res) = (x) + (y), 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#ਜੇ_ਅੰਤ /* GNU */
#ਜੇ_ਅੰਤ /* stdckdint.h */
