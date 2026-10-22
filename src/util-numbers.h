/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <limits.h>
#شامل <stdbool.h>
#شامل <stdint.h>

#اگر defined(_MSC_VER)
# include <intrin.h>
#اگر_ختم

#شامل "utils.h"

/*
 * Define various parsers to avoid the use of strto* -- it’s slower and accepts
 * a bunch of stuff we don’t want to allow, like signs, spaces, even locale stuff.
 *
 * But the real feature is that it does not require a NULL-terminated string, so
 * it works safely *also* on any buffer, assuming the correct corresponding size
 * is provided. For NULL-terminated strings, just pass `SIZE_MAX` as the length:
 * the parsers will *always* stop on a NULL character.
 */

#تعریف_کرو MAKE_PARSE_DEC_مقصد(type, max)                         \
ساکن اندرونی صحیح                                            \
parse_dec_مقصد##type(ثابت حرف *s, سائز_t len, type (*برآمد))  \
{                                                            \
    type نتیجہ = 0;                                         \
    سائز_t i;                                                \
    برائے (i = 0;                                              \
         i < len && (غیرعلامتی حرف)(s[i] - '0') < 10U &&     \
         نتیجہ <= (max) / 10 &&                             \
         نتیجہ * 10 <= (max) - (غیرعلامتی حرف) (s[i] - '0');\
         i++) {                                              \
        نتیجہ = نتیجہ * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *برآمد = نتیجہ;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    واپس (i >= len || (غیرعلامتی حرف)(s[i] - '0') >= 10U)  \
           ? (صحیح) i                                         \
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

#تعریف_منسوخ MAKE_PARSE_DEC_مقصد

ساکن ثابت غیرعلامتی حرف digits__[] = {
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

#تعریف_کرو MAKE_PARSE_HEX_مقصد(type, max)                           \
ساکن اندرونی صحیح                                              \
parse_hex_مقصد##type(ثابت حرف *s, سائز_t len, type (*برآمد))    \
{                                                              \
    type نتیجہ = 0;                                           \
    سائز_t i = 0;                                              \
    برائے (; i < len && digits__[(غیرعلامتی حرف) s[i]] < 16u &&  \
         نتیجہ <= (max) >> 4;                                 \
         i++) {                                                \
        نتیجہ = نتیجہ * 16 + digits__[(غیرعلامتی حرف) s[i]]; \
    }                                                          \
    *برآمد = نتیجہ;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    واپس (i >= len || !is_xdigit(s[i])) ? (صحیح) i : -1;      \
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

#تعریف_منسوخ MAKE_PARSE_HEX_مقصد

ساکن اندرونی غیرعلامتی صحیح
popcount32(uint32_t x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    واپس (غیرعلامتی صحیح)__builtin_popcountl(x);
#وگرنہ_اگر defined(_MSC_VER)
    واپس __popcnt(x);
#ورنہ
    /* Fallback (Brian Kernighan’s method) */
    غیرعلامتی صحیح گنتی = 0;
    جب (x) {
        x &= x - 1;
        گنتی++;
    }
    واپس گنتی;
#اگر_ختم
}

ساکن اندرونی غیرعلامتی صحیح
ctz32(uint32_t x)
{
    اگر (x == 0)
        واپس 32;

#اگر defined(__GNUC__) || defined(__clang__)
    واپس _Generic(
        x,
        غیرعلامتی صحیح: (غیرعلامتی صحیح)__builtin_ctz(x),
        غیرعلامتی طویل: (غیرعلامتی صحیح)__builtin_ctzl(x),
        غیرعلامتی طویل طویل: (غیرعلامتی صحیح)__builtin_ctzll(x)
    );
#وگرنہ_اگر defined(_MSC_VER)
    غیرعلامتی طویل اشاریہ;
    ثنائی_ہندسہ_جائزہ_Forward(&اشاریہ, x);
    واپس (غیرعلامتی صحیح)اشاریہ;
#ورنہ
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ساکن ثابت غیرعلامتی صحیح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    واپس debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#اگر_ختم
}

ساکن اندرونی غیرعلامتی صحیح
clz32(uint32_t x)
{
    اگر (x == 0)
        واپس 32;

#اگر defined(__GNUC__) || defined(__clang__)
    واپس _Generic(
        x,
        غیرعلامتی صحیح:
            (غیرعلامتی صحیح)__builtin_clz(x) -
            (غیرعلامتی صحیح)((حجم(غیرعلامتی صحیح) * CHAR_BIT) - 32),
        غیرعلامتی طویل:
            (غیرعلامتی صحیح)__builtin_clzl(x) -
            (غیرعلامتی صحیح)((حجم(غیرعلامتی طویل) * CHAR_BIT) - 32),
        غیرعلامتی طویل طویل:
            (غیرعلامتی صحیح)__builtin_clzll(x) -
            (غیرعلامتی صحیح)((حجم(غیرعلامتی طویل طویل) * CHAR_BIT) - 32)
    );
#ورنہ
    // Efficient binary search fallback for 32-bit integers
    غیرعلامتی صحیح گنتی = 0;
    اگر ((x & 0xffff0000) == 0) { گنتی += 16; x <<= 16; }
    اگر ((x & 0xff000000) == 0) { گنتی += 8;  x <<= 8;  }
    اگر ((x & 0xf0000000) == 0) { گنتی += 4;  x <<= 4;  }
    اگر ((x & 0xc0000000) == 0) { گنتی += 2;  x <<= 2;  }
    اگر ((x & 0x80000000) == 0) { گنتی += 1; }
    واپس گنتی;
#اگر_ختم
}

ساکن اندرونی غیرعلامتی صحیح
اگلا_pow2(غیرعلامتی صحیح x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    اگر (x <= 1) واپس 1u;
    واپس 1u << ((صحیح)حجم(غیرعلامتی) * CHAR_BIT - __builtin_clz(x - 1));
#ورنہ
    اگر (x <= 1u) واپس 1u;
    x--;
    برائے (غیرعلامتی s = 1; s < (غیرعلامتی)حجم(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    واپس x + 1u;
#اگر_ختم
}

ساکن اندرونی uint32_t
keep_سب_سے_نیچا_n_سیٹ_بٹ(uint32_t نقاب, غیرعلامتی صحیح n) {
    ثابت uint32_t original = نقاب;

    /* Clear the lowest set bit n times */
    جب (n-- && نقاب)
        نقاب &= (نقاب - 1);

    واپس original ^ نقاب;
}
