/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>

#إذا defined(_MSC_VER)
# include <intrin.h>
#پایان_اگر

#ضمن "utils.h"

/*
 * Define various parsers to avoid the use of strto* -- it’s slower and accepts
 * a bunch of stuff we don’t want to allow, like signs, spaces, even locale stuff.
 *
 * But the real feature is that it does not require a NULL-terminated string, so
 * it works safely *also* on any buffer, assuming the correct corresponding size
 * is provided. For NULL-terminated strings, just pass `SIZE_MAX` as the length:
 * the parsers will *always* stop on a NULL character.
 */

#تعریف_کن MAKE_PARSE_DEC_مقصد(type, max)                         \
ساكن مضمن صحيح                                            \
parse_dec_مقصد##type(ثابت حرف *s, اندازه_t len, type (*بیرون))  \
{                                                            \
    type نتیجه = 0;                                         \
    اندازه_t i;                                                \
    لكل (i = 0;                                              \
         i < len && (بیعلامت حرف)(s[i] - '0') < 10U &&     \
         نتیجه <= (max) / 10 &&                             \
         نتیجه * 10 <= (max) - (بیعلامت حرف) (s[i] - '0');\
         i++) {                                              \
        نتیجه = نتیجه * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *بیرون = نتیجه;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    أرجع (i >= len || (بیعلامت حرف)(s[i] - '0') >= 10U)  \
           ? (صحيح) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_مقصد(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_مقصد(uint64_t, UINT64_MAX)

#لغو_تعریف MAKE_PARSE_DEC_مقصد

ساكن ثابت بیعلامت حرف digits__[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 10,   11,   12,   13,   14,   15,   0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 10,   11,   12,   13,   14,   15,   0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};

#تعریف_کن MAKE_PARSE_HEX_مقصد(type, max)                           \
ساكن مضمن صحيح                                              \
parse_hex_مقصد##type(ثابت حرف *s, اندازه_t len, type (*بیرون))    \
{                                                              \
    type نتیجه = 0;                                           \
    اندازه_t i = 0;                                              \
    لكل (; i < len && digits__[(بیعلامت حرف) s[i]] < 16u &&  \
         نتیجه <= (max) >> 4;                                 \
         i++) {                                                \
        نتیجه = نتیجه * 16 + digits__[(بیعلامت حرف) s[i]]; \
    }                                                          \
    *بیرون = نتیجه;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    أرجع (i >= len || !is_xdigit(s[i])) ? (صحيح) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_مقصد(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_مقصد(uint64_t, UINT64_MAX)

#لغو_تعریف MAKE_PARSE_HEX_مقصد

ساكن مضمن بیعلامت صحيح
popcount32(uint32_t x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (بیعلامت صحيح)__builtin_popcountl(x);
#وإلاإذا defined(_MSC_VER)
    أرجع __popcnt(x);
#وإلا
    /* Fallback (Brian Kernighan’s method) */
    بیعلامت صحيح شمار = 0;
    طالما (x) {
        x &= x - 1;
        شمار++;
    }
    أرجع شمار;
#پایان_اگر
}

ساكن مضمن بیعلامت صحيح
ctz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بیعلامت صحيح: (بیعلامت صحيح)__builtin_ctz(x),
        بیعلامت دراز: (بیعلامت صحيح)__builtin_ctzl(x),
        بیعلامت دراز دراز: (بیعلامت صحيح)__builtin_ctzll(x)
    );
#وإلاإذا defined(_MSC_VER)
    بیعلامت دراز شاخص;
    بیت_پیمایش_Forward(&شاخص, x);
    أرجع (بیعلامت صحيح)شاخص;
#وإلا
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ساكن ثابت بیعلامت صحيح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    أرجع debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#پایان_اگر
}

ساكن مضمن بیعلامت صحيح
clz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بیعلامت صحيح:
            (بیعلامت صحيح)__builtin_clz(x) -
            (بیعلامت صحيح)((حجم(بیعلامت صحيح) * CHAR_BIT) - 32),
        بیعلامت دراز:
            (بیعلامت صحيح)__builtin_clzl(x) -
            (بیعلامت صحيح)((حجم(بیعلامت دراز) * CHAR_BIT) - 32),
        بیعلامت دراز دراز:
            (بیعلامت صحيح)__builtin_clzll(x) -
            (بیعلامت صحيح)((حجم(بیعلامت دراز دراز) * CHAR_BIT) - 32)
    );
#وإلا
    // Efficient binary search fallback for 32-bit integers
    بیعلامت صحيح شمار = 0;
    إذا ((x & 0xffff0000) == 0) { شمار += 16; x <<= 16; }
    إذا ((x & 0xff000000) == 0) { شمار += 8;  x <<= 8;  }
    إذا ((x & 0xf0000000) == 0) { شمار += 4;  x <<= 4;  }
    إذا ((x & 0xc0000000) == 0) { شمار += 2;  x <<= 2;  }
    إذا ((x & 0x80000000) == 0) { شمار += 1; }
    أرجع شمار;
#پایان_اگر
}

ساكن مضمن بیعلامت صحيح
بعدی_pow2(بیعلامت صحيح x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    إذا (x <= 1) أرجع 1u;
    أرجع 1u << ((صحيح)حجم(بیعلامت) * CHAR_BIT - __builtin_clz(x - 1));
#وإلا
    إذا (x <= 1u) أرجع 1u;
    x--;
    لكل (بیعلامت s = 1; s < (بیعلامت)حجم(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    أرجع x + 1u;
#پایان_اگر
}

ساكن مضمن uint32_t
keep_پایینترین_n_مجموعه_بیتها(uint32_t ماسک, بیعلامت صحيح n) {
    ثابت uint32_t original = ماسک;

    /* Clear the lowest set bit n times */
    طالما (n-- && ماسک)
        ماسک &= (ماسک - 1);

    أرجع original ^ ماسک;
}
