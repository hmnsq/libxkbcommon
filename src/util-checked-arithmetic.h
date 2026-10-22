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
#अन्यथा
#define __ckd_has_include(x) 0
#endif

#जर __ckd_has_include(<stdckdint.h>)
#अन्तर्भाव <stdckdint.h>
#अन्यथा

#define __STDC_VERSION_STDCKDINT_H__ 202311L

#जर (!defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__))
#define __ckd_have_int128
#define __ckd_intmax __int128
#किंवाजर (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __ckd_intmax दीर्घ दीर्घ
#अन्यथा
#define __ckd_intmax दीर्घ
#endif

प्रकारव्याख्या चिन्हित __ckd_intmax __ckd_intmax_t;
प्रकारव्याख्या अचिन्हित __ckd_intmax __ckd_uintmax_t;

#ifdef __has_builtin
#define __ckd_has_builtin(x) __has_builtin(x)
#अन्यथा
#define __ckd_has_builtin(x) 0
#endif

#जर (!defined(__STRICT_ANSI__) &&                                       \
     ((defined(__GNUC__) && __GNUC__ >= 5 && !defined(__ICC)) ||        \
      (__ckd_has_builtin(__builtin_add_overflow) &&                     \
       __ckd_has_builtin(__builtin_sub_overflow) &&                     \
       __ckd_has_builtin(__builtin_mul_overflow))))
#define ckd_add(res, x, y) __builtin_add_overflow((x), (y), (res))
#define ckd_sub(res, x, y) __builtin_sub_overflow((x), (y), (res))
#define ckd_mul(res, x, y) __builtin_mul_overflow((x), (y), (res))

#किंवाजर defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define ckd_add(res, a, b) __ckd_expr(add, (res), (a), (b))
#define ckd_sub(res, a, b) __ckd_expr(sub, (res), (a), (b))
#define ckd_mul(res, a, b) __ckd_expr(mul, (res), (a), (b))

#जर defined(__GNUC__) || defined(__llvm__)
#define __ckd_inline                                    \
  बाह्य __inline __attribute__((__gnu_inline__,        \
                                 __always_inline__,     \
                                 __artificial__))
#अन्यथा
#define __ckd_inline स्थिर ओळीत
#endif

#ifdef __ckd_have_int128
#define __ckd_generic_int128(x, y) , चिन्हित __int128: x, अचिन्हित __int128: y
#अन्यथा
#define __ckd_generic_int128(x, y)
#endif

#define __ckd_sign(T)                           \
  ((T)1 << (आकार(T) * 8 - 1))

#define __ckd_is_signed(x)                      \
  _Generic(x,                                   \
           चिन्हित अक्षर: 1,                      \
           अचिन्हित अक्षर: 0,                    \
           चिन्हित लघु: 1,                     \
           अचिन्हित लघु: 0,                   \
           चिन्हित पूर्णांक: 1,                       \
           अचिन्हित पूर्णांक: 0,                     \
           चिन्हित दीर्घ: 1,                      \
           अचिन्हित दीर्घ: 0,                    \
           चिन्हित दीर्घ दीर्घ: 1,                 \
           अचिन्हित दीर्घ दीर्घ: 0                \
           __ckd_generic_int128(1, 0))

#define __ckd_expr(op, res, a, b)                       \
  (_Generic(*res,                                       \
            चिन्हित अक्षर: __ckd_##op##_schar,            \
            अचिन्हित अक्षर: __ckd_##op##_uchar,          \
            चिन्हित लघु: __ckd_##op##_sshort,          \
            अचिन्हित लघु: __ckd_##op##_ushort,        \
            चिन्हित पूर्णांक: __ckd_##op##_sint,              \
            अचिन्हित पूर्णांक: __ckd_##op##_uint,            \
            चिन्हित दीर्घ: __ckd_##op##_slong,            \
            अचिन्हित दीर्घ: __ckd_##op##_ulong,          \
            चिन्हित दीर्घ दीर्घ: __ckd_##op##_slonger,     \
            अचिन्हित दीर्घ दीर्घ: __ckd_##op##_ulonger    \
            __ckd_generic_int128(                       \
                __ckd_##op##_sint128,                   \
                __ckd_##op##_uint128))(                 \
                    res, a, b,                          \
                    __ckd_is_signed(a),                 \
                    __ckd_is_signed(b)))

#define __ckd_declare_add(S, T)                                         \
  __ckd_inline अक्षर S(शून्य *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      अक्षर __a_signed,                                  \
                      अक्षर __b_signed) {                                \
    __ckd_uintmax_t __z = __x + __y;                                    \
    *(T *)__res = (T)__z;                                               \
    अक्षर __truncated = 0;                                               \
    जर (आकार(T) < आकार(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    निवड (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      स्थिती 0:  /* u = u + u */                                          \
        परत __truncated | (__z < __x);                               \
      स्थिती 1:  /* u = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      स्थिती 2:  /* u = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      स्थिती 3:  /* u = s + s */                                          \
        परत __truncated |                                            \
            ((__ckd_intmax_t)(((__z | __x) &  __y) |                    \
                              ((__z & __x) & ~__y)) < 0);               \
      स्थिती 4:  /* s = u + u */                                          \
        परत __truncated | (__z < __x) | ((__ckd_intmax_t)__z < 0);   \
      स्थिती 5:  /* s = u + s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated | (__x + __y < __y);                         \
      स्थिती 6:  /* s = s + u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated | (__x + __y < __x);                         \
      स्थिती 7:  /* s = s + s */                                          \
        परत __truncated |                                            \
            ((__ckd_intmax_t)((__z ^ __x) &                             \
                              (__z ^ __y)) < 0);                        \
      पूर्वनिर्धारित:                                                          \
        साठी (;;) (शून्य)0;                                               \
    }                                                                   \
  }

__ckd_declare_add(__ckd_add_schar, चिन्हित अक्षर)
__ckd_declare_add(__ckd_add_uchar, अचिन्हित अक्षर)
__ckd_declare_add(__ckd_add_sshort, चिन्हित लघु)
__ckd_declare_add(__ckd_add_ushort, अचिन्हित लघु)
__ckd_declare_add(__ckd_add_sint, चिन्हित पूर्णांक)
__ckd_declare_add(__ckd_add_uint, अचिन्हित पूर्णांक)
__ckd_declare_add(__ckd_add_slong, चिन्हित दीर्घ)
__ckd_declare_add(__ckd_add_ulong, अचिन्हित दीर्घ)
__ckd_declare_add(__ckd_add_slonger, चिन्हित दीर्घ दीर्घ)
__ckd_declare_add(__ckd_add_ulonger, अचिन्हित दीर्घ दीर्घ)
#ifdef __ckd_have_int128
__ckd_declare_add(__ckd_add_sint128, चिन्हित __int128)
__ckd_declare_add(__ckd_add_uint128, अचिन्हित __int128)
#endif

#define __ckd_declare_sub(S, T)                                         \
  __ckd_inline अक्षर S(शून्य *__res,                                      \
                      __ckd_uintmax_t __x,                              \
                      __ckd_uintmax_t __y,                              \
                      अक्षर __a_signed,                                  \
                      अक्षर __b_signed) {                                \
    __ckd_uintmax_t __z = __x - __y;                                    \
    *(T *)__res = (T)__z;                                               \
    अक्षर __truncated = 0;                                               \
    जर (आकार(T) < आकार(__ckd_intmax_t)) {                           \
      __truncated = __z != (__ckd_uintmax_t)(T)__z;                     \
    }                                                                   \
    निवड (__ckd_is_signed((T)0) << 2 |                                \
            __a_signed << 1 | __b_signed) {                             \
      स्थिती 0:  /* u = u - u */                                          \
        परत __truncated | (__x < __y);                               \
      स्थिती 1:  /* u = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      स्थिती 2:  /* u = s - u */                                          \
        परत __truncated | (__y > __x) | ((__ckd_intmax_t)__x < 0);   \
      स्थिती 3:  /* u = s - s */                                          \
        परत __truncated |                                            \
            ((__ckd_intmax_t)(((__z & __x) &  __y) |                    \
                              ((__z | __x) & ~__y)) < 0);               \
      स्थिती 4:  /* s = u - u */                                          \
        परत __truncated | ((__x < __y) ^ ((__ckd_intmax_t)__z < 0)); \
      स्थिती 5:  /* s = u - s */                                          \
        __y ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated | (__x >= __y);                              \
      स्थिती 6:  /* s = s - u */                                          \
        __x ^= __ckd_sign(__ckd_uintmax_t);                             \
        परत __truncated | (__x < __y);                               \
      स्थिती 7:  /* s = s - s */                                          \
        परत __truncated |                                            \
            ((__ckd_intmax_t)((__x ^ __y) &                             \
                              (__z ^ __x)) < 0);                        \
      पूर्वनिर्धारित:                                                          \
        साठी (;;) (शून्य)0;                                               \
    }                                                                   \
  }

__ckd_declare_sub(__ckd_sub_schar, चिन्हित अक्षर)
__ckd_declare_sub(__ckd_sub_uchar, अचिन्हित अक्षर)
__ckd_declare_sub(__ckd_sub_sshort, चिन्हित लघु)
__ckd_declare_sub(__ckd_sub_ushort, अचिन्हित लघु)
__ckd_declare_sub(__ckd_sub_sint, चिन्हित पूर्णांक)
__ckd_declare_sub(__ckd_sub_uint, अचिन्हित पूर्णांक)
__ckd_declare_sub(__ckd_sub_slong, चिन्हित दीर्घ)
__ckd_declare_sub(__ckd_sub_ulong, अचिन्हित दीर्घ)
__ckd_declare_sub(__ckd_sub_slonger, चिन्हित दीर्घ दीर्घ)
__ckd_declare_sub(__ckd_sub_ulonger, अचिन्हित दीर्घ दीर्घ)
#ifdef __ckd_have_int128
__ckd_declare_sub(__ckd_sub_sint128, चिन्हित __int128)
__ckd_declare_sub(__ckd_sub_uint128, अचिन्हित __int128)
#endif

#define __ckd_declare_mul(S, T)                                 \
  __ckd_inline अक्षर S(शून्य *__res,                              \
                      __ckd_uintmax_t __x,                      \
                      __ckd_uintmax_t __y,                      \
                      अक्षर __a_signed,                          \
                      अक्षर __b_signed) {                        \
    निवड (__ckd_is_signed((T)0) << 2 |                        \
            __a_signed << 1 | __b_signed) {                     \
      स्थिती 0: {  /* u = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णांक __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (__o | (आकार(T) < आकार(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      स्थिती 1: {  /* u = u * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णांक __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (__o | (((__ckd_intmax_t)__y < 0) & !!__x) |     \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्थिती 2: {  /* u = s * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णांक __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (__o | (((__ckd_intmax_t)__x < 0) & !!__y) |     \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्थिती 3: {  /* u = s * s */                                \
        पूर्णांक __o = 0;                                            \
        जर ((__ckd_intmax_t)(__x & __y) < 0) {                  \
          __x = 0 - __x;                                        \
          __y = 0 - __y;                                        \
        } अन्यथा जर ((__ckd_intmax_t)(__x ^ __y) < 0) {           \
          __o = __x && __y;                                     \
        }                                                       \
        __ckd_uintmax_t __z = __x * __y;                        \
        __o |= __x && __z / __x != __y;                         \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (__o | (आकार(T) < आकार(__z) &&               \
                       __z != (__ckd_uintmax_t)*(T *)__res));   \
      }                                                         \
      स्थिती 4: {  /* s = u * u */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        पूर्णांक __o = __x && __z / __x != __y;                      \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (__o | ((__ckd_intmax_t)(__z) < 0) |             \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्थिती 5: {  /* s = u * s */                                \
        __ckd_uintmax_t __t = 0 - __y;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __y : __t;            \
        __ckd_uintmax_t __p = __t * __x;                        \
        पूर्णांक __o = __t && __p / __t != __x;                      \
        पूर्णांक __n = (__ckd_intmax_t)__y < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        परत (अक्षर)                                           \
               (__o | (__p > __m + __n) |                       \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्थिती 6: {  /* s = s * u */                                \
        __ckd_uintmax_t __t = 0 - __x;                          \
        __t = (__ckd_intmax_t)(__t) < 0 ? __x : __t;            \
        __ckd_uintmax_t __p = __t * __y;                        \
        पूर्णांक __o = __t && __p / __t != __y;                      \
        पूर्णांक __n = (__ckd_intmax_t)__x < 0;                      \
        __ckd_uintmax_t __z = __n ? 0 - __p : __p;              \
        *(T *)__res = (T)__z;                                   \
        __ckd_uintmax_t __m = __ckd_sign(__ckd_uintmax_t) - 1;  \
        परत (अक्षर)                                           \
               (__o | (__p > __m + __n) |                       \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      स्थिती 7: {  /* s = s * s */                                \
        __ckd_uintmax_t __z = __x * __y;                        \
        *(T *)__res = (T)__z;                                   \
        परत (अक्षर)                                           \
               (((((__ckd_intmax_t)__y < 0) &&                  \
                  (__x == __ckd_sign(__ckd_uintmax_t))) ||      \
                 (__y && (((__ckd_intmax_t)__z /                \
                           (__ckd_intmax_t)__y) !=              \
                          (__ckd_intmax_t)__x))) |              \
                (आकार(T) < आकार(__z) &&                     \
                 __z != (__ckd_uintmax_t)*(T *)__res));         \
      }                                                         \
      पूर्वनिर्धारित:                                                  \
        साठी (;;) (शून्य)0;                                       \
    }                                                           \
  }

__ckd_declare_mul(__ckd_mul_schar, चिन्हित अक्षर)
__ckd_declare_mul(__ckd_mul_uchar, अचिन्हित अक्षर)
__ckd_declare_mul(__ckd_mul_sshort, चिन्हित लघु)
__ckd_declare_mul(__ckd_mul_ushort, अचिन्हित लघु)
__ckd_declare_mul(__ckd_mul_sint, चिन्हित पूर्णांक)
__ckd_declare_mul(__ckd_mul_uint, अचिन्हित पूर्णांक)
__ckd_declare_mul(__ckd_mul_slong, चिन्हित दीर्घ)
__ckd_declare_mul(__ckd_mul_ulong, अचिन्हित दीर्घ)
__ckd_declare_mul(__ckd_mul_slonger, चिन्हित दीर्घ दीर्घ)
__ckd_declare_mul(__ckd_mul_ulonger, अचिन्हित दीर्घ दीर्घ)
#ifdef __ckd_have_int128
__ckd_declare_mul(__ckd_mul_sint128, चिन्हित __int128)
__ckd_declare_mul(__ckd_mul_uint128, अचिन्हित __int128)
#endif

#अन्यथा
#ifdef _MSC_VER
#pragma warning("checked integer arithmetic unsupported in this environment")
#अन्यथा
#pragma GCC warning "checked integer arithmetic unsupported in this environment"
#endif

#define ckd_add(res, x, y) (*(res) = (x) + (y), 0)
#define ckd_sub(res, x, y) (*(res) = (x) - (y), 0)
#define ckd_mul(res, x, y) (*(res) = (x) * (y), 0)

#endif /* GNU */
#endif /* stdckdint.h */
