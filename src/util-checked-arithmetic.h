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

#അന്തര്ഭാവ "config.h"

#ifdef __has_include
#define __ckd_has_include(x) __has_include(x)
#അല്ലെങ്കിൽ
#define __ckd_has_include(x) 0
#endif

#എങ്കിൽ __ckd_has_include(<stdckdint.h>)
#അന്തര്ഭാവ <stdckdint.h>
#അല്ലെങ്കിൽ

#define __STDC_VERSION_STDCKDINT_H__ 202311L

#എങ്കിൽ (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#define __ckd_have_int128
#define __ckd_intmax __int128
#അല്ലെങ്കിലെങ്കിൽ (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __ckd_intmax long long
#അല്ലെങ്കിൽ
#define __ckd_intmax long
#endif

തരനിർവചനം signed __ckd_intmax __ckd_intmax_t;
തരനിർവചനം unsigned __ckd_intmax __ckd_uintmax_t;

#ifdef __has_builtin
#define __ckd_has_builtin(x) __has_builtin(x)
#അല്ലെങ്കിൽ
#define __ckd_has_builtin(x) 0
#endif

#എങ്കിൽ (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#define ckd_add(res, x, y) __builtin_add_overflow((x), (y), (res))
#define ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#define ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#അല്ലെങ്കിലെങ്കിൽ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define ckd_add(res, a, b) __ckd_expr(add, (res), (a), (b))
#define ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#define ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#എങ്കിൽ defined(__GNUC__) || defined(__llvm__)
#define __ckd_inline                                    \
  ബാഹ്യ __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#അല്ലെങ്കിൽ
#define __ckd_inline സ്ഥിതം വരിയിൽ
#endif

#ifdef __ckd_have_int128
#define __ckd_generic_int128(x, y) , signed __int128: x, unsigned __int128: y
#അല്ലെങ്കിൽ
#define __ckd_generic_int128(x, y)
#endif

#define __ckd_sign(T)                           \
  ((T)1 << (വലുപ്പം(T) * 8 - 1))

#define __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           signed അക്ഷരം: 1,                      \
           unsigned അക്ഷരം: 0,                    \
           signed short: 1,                     \
           unsigned short: 0,                   \
           signed പൂർണ്ണസംഖ്യ: 1,                       \
           unsigned പൂർണ്ണസംഖ്യ: 0,                     \
           signed long: 1,                      \
           unsigned long: 0,                    \
           signed long long: 1,                 \
           unsigned long long: 0                \
           __ckd_generic_int128(1, 0))

#define __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            signed അക്ഷരം: __ckd_##op##_schar,            \
            unsigned അക്ഷരം: __ckd_##op##_uchar,          \
            signed short: __ckd_##op##_sshort,          \
            unsigned short: __ckd_##op##_ushort,        \
            signed പൂർണ്ണസംഖ്യ: __ckd_##op##_sint,              \
            unsigned പൂർണ്ണസംഖ്യ: __ckd_##op##_uint,            \
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
  __ckd_inline അക്ഷരം S(ശൂന്യം *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      അക്ഷരം __a_signed,                                  \
                      അക്ഷരം __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    അക്ഷരം __truncated = 0;                                               \
    എങ്കിൽ (വലുപ്പം(T) < വലുപ്പം(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    തിരഞ്ഞെടുപ്പ് (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      അവസ്ഥ 0:  /* u = u + u */                                          \
        തിരികെ __truncated | (__z < __x);                               \
      അവസ്ഥ 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      അവസ്ഥ 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      അവസ്ഥ 3:  /* u = s + s */                                          \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      അവസ്ഥ 4:  /* s = u + u */                                          \
        തിരികെ __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      അവസ്ഥ 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated | (__x + __y < __y);                         \
      അവസ്ഥ 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated | (__x + __y < __x);                         \
      അവസ്ഥ 7:  /* s = s + s */                                          \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      സ്ഥിരസ്ഥിതി:                                                          \
        വേണ്ടി (;;) (ശൂന്യം)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, signed അക്ഷരം)
__ckd_declare_add(__ckd_add_uchar, unsigned അക്ഷരം)
__ckd_declare_add(__ckd_add_sshort, signed short)
__ckd_declare_add(__ckd_add_ushort, unsigned short)
__ckd_declare_add(__ckd_add_sint, signed പൂർണ്ണസംഖ്യ)
__ckd_declare_add(__ckd_add_uint, unsigned പൂർണ്ണസംഖ്യ)
__ckd_declare_add(__ckd_add_slong, signed long)
__ckd_declare_add(__ckd_add_ulong, unsigned long)
__ckd_declare_add(__ckd_add_slonger, signed long long)
__ckd_declare_add(__ckd_add_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, signed __int128)
__ckd_declare_add(__ckd_add_uint128, unsigned __int128)
#endif

#define __ckd_declare_sub(S, T)                                         \
  __ckd_inline അക്ഷരം S(ശൂന്യം *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      അക്ഷരം __a_signed,                                  \
                      അക്ഷരം __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    അക്ഷരം __truncated = 0;                                               \
    എങ്കിൽ (വലുപ്പം(T) < വലുപ്പം(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    തിരഞ്ഞെടുപ്പ് (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      അവസ്ഥ 0:  /* u = u - u */                                          \
        തിരികെ __truncated | (__x < __y);                               \
      അവസ്ഥ 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      അവസ്ഥ 2:  /* u = s - u */                                          \
        തിരികെ __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      അവസ്ഥ 3:  /* u = s - s */                                          \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      അവസ്ഥ 4:  /* s = u - u */                                          \
        തിരികെ __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      അവസ്ഥ 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated | (__x >= __y);                              \
      അവസ്ഥ 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        തിരികെ __truncated | (__x < __y);                               \
      അവസ്ഥ 7:  /* s = s - s */                                          \
        തിരികെ __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      സ്ഥിരസ്ഥിതി:                                                          \
        വേണ്ടി (;;) (ശൂന്യം)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, signed അക്ഷരം)
__ckd_declare_sub(__ckd_sub_uchar, unsigned അക്ഷരം)
__ckd_declare_sub(__ckd_sub_sshort, signed short)
__ckd_declare_sub(__ckd_sub_ushort, unsigned short)
__ckd_declare_sub(__ckd_sub_sint, signed പൂർണ്ണസംഖ്യ)
__ckd_declare_sub(__ckd_sub_uint, unsigned പൂർണ്ണസംഖ്യ)
__ckd_declare_sub(__ckd_sub_slong, signed long)
__ckd_declare_sub(__ckd_sub_ulong, unsigned long)
__ckd_declare_sub(__ckd_sub_slonger, signed long long)
__ckd_declare_sub(__ckd_sub_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, signed __int128)
__ckd_declare_sub(__ckd_sub_uint128, unsigned __int128)
#endif

#define __ckd_declare_mul(S, T)                                 \
  __ckd_inline അക്ഷരം S(ശൂന്യം *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      അക്ഷരം __a_signed,                          \
                      അക്ഷരം __b_signed) {                        \
    തിരഞ്ഞെടുപ്പ് (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      അവസ്ഥ 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        പൂർണ്ണസംഖ്യ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (വലുപ്പം(T) < വലുപ്പം(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      അവസ്ഥ 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        പൂർണ്ണസംഖ്യ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      അവസ്ഥ 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        പൂർണ്ണസംഖ്യ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      അവസ്ഥ 3: {  /* u = s * s */                                \
        പൂർണ്ണസംഖ്യ __o = 0;                                            \
        എങ്കിൽ ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } അല്ലെങ്കിൽ എങ്കിൽ ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (വലുപ്പം(T) < വലുപ്പം(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      അവസ്ഥ 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        പൂർണ്ണസംഖ്യ __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      അവസ്ഥ 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        പൂർണ്ണസംഖ്യ __o = __t && __p / __t != __x;                      \
        പൂർണ്ണസംഖ്യ __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (__p > __m + __n) |                       \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      അവസ്ഥ 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        പൂർണ്ണസംഖ്യ __o = __t && __p / __t != __y;                      \
        പൂർണ്ണസംഖ്യ __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        തിരികെ (അക്ഷരം)                                           \
               (__o | (__p > __m + __n) |                       \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      അവസ്ഥ 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        തിരികെ (അക്ഷരം)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (വലുപ്പം(T) < വലുപ്പം(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      സ്ഥിരസ്ഥിതി:                                                  \
        വേണ്ടി (;;) (ശൂന്യം)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, signed അക്ഷരം)
__ckd_declare_mul(__ckd_mul_uchar, unsigned അക്ഷരം)
__ckd_declare_mul(__ckd_mul_sshort, signed short)
__ckd_declare_mul(__ckd_mul_ushort, unsigned short)
__ckd_declare_mul(__ckd_mul_sint, signed പൂർണ്ണസംഖ്യ)
__ckd_declare_mul(__ckd_mul_uint, unsigned പൂർണ്ണസംഖ്യ)
__ckd_declare_mul(__ckd_mul_slong, signed long)
__ckd_declare_mul(__ckd_mul_ulong, unsigned long)
__ckd_declare_mul(__ckd_mul_slonger, signed long long)
__ckd_declare_mul(__ckd_mul_ulonger, unsigned long long)
#ifdef __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, signed __int128)
__ckd_declare_mul(__ckd_mul_uint128, unsigned __int128)
#endif

#അല്ലെങ്കിൽ
#ifdef _MSC_VER
#pragma warning("checked integer arithmetic unsupported in this environment")
#അല്ലെങ്കിൽ
#pragma GCC warning "checked integer arithmetic unsupported in this environment"
#endif

#define ckd_add(res, x, y) (*(res) = (x) + (y), 0)
#define ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#define ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#endif /* GNU */
#endif /* stdckdint.h */
