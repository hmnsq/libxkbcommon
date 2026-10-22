/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <limits.h>
#شامل <stdbool.h>
#شامل <stdint.h>

#اگر defined(_MSC_VER)
# include <intrin.h>
#endif

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

#define MAKE_PARSE_DEC_TO(type, max)                         \
ایستا درونخطی صحیح                                            \
parse_dec_to_##type(ثابت نویسه *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    برای (i = 0;                                              \
         i < len && (unsigned نویسه)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned نویسه) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    بازگشت (i >= len || (unsigned نویسه)(s[i] - '0') >= 10U)  \
           ? (صحیح) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_TO(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_TO(uint64_t, UINT64_MAX)

#undef MAKE_PARSE_DEC_TO

ایستا ثابت unsigned نویسه digits__[] = {
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

#define MAKE_PARSE_HEX_TO(type, max)                           \
ایستا درونخطی صحیح                                              \
parse_hex_to_##type(ثابت نویسه *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    برای (; i < len && digits__[(unsigned نویسه) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned نویسه) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    بازگشت (i >= len || !is_xdigit(s[i])) ? (صحیح) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_TO(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_TO(uint64_t, UINT64_MAX)

#undef MAKE_PARSE_HEX_TO

ایستا درونخطی unsigned صحیح
popcount32(uint32_t x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    بازگشت (unsigned صحیح)__builtin_popcountl(x);
#وگرنهاگر defined(_MSC_VER)
    بازگشت __popcnt(x);
#وگرنه
    /* Fallback (Brian Kernighan’s method) */
    unsigned صحیح شمار = 0;
    وقتی (x) {
        x &= x - 1;
        شمار++;
    }
    بازگشت شمار;
#endif
}

ایستا درونخطی unsigned صحیح
ctz32(uint32_t x)
{
    اگر (x == 0)
        بازگشت 32;

#اگر defined(__GNUC__) || defined(__clang__)
    بازگشت _Generic(
        x,
        unsigned صحیح: (unsigned صحیح)__builtin_ctz(x),
        unsigned long: (unsigned صحیح)__builtin_ctzl(x),
        unsigned long long: (unsigned صحیح)__builtin_ctzll(x)
    );
#وگرنهاگر defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    بازگشت (unsigned صحیح)index;
#وگرنه
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ایستا ثابت unsigned صحیح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    بازگشت debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

ایستا درونخطی unsigned صحیح
clz32(uint32_t x)
{
    اگر (x == 0)
        بازگشت 32;

#اگر defined(__GNUC__) || defined(__clang__)
    بازگشت _Generic(
        x,
        unsigned صحیح:
            (unsigned صحیح)__builtin_clz(x) -
            (unsigned صحیح)((اندازه(unsigned صحیح) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned صحیح)__builtin_clzl(x) -
            (unsigned صحیح)((اندازه(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned صحیح)__builtin_clzll(x) -
            (unsigned صحیح)((اندازه(unsigned long long) * CHAR_BIT) - 32)
    );
#وگرنه
    // Efficient binary search fallback for 32-bit integers
    unsigned صحیح شمار = 0;
    اگر ((x & 0xffff0000) == 0) { شمار += 16; x <<= 16; }
    اگر ((x & 0xff000000) == 0) { شمار += 8;  x <<= 8;  }
    اگر ((x & 0xf0000000) == 0) { شمار += 4;  x <<= 4;  }
    اگر ((x & 0xc0000000) == 0) { شمار += 2;  x <<= 2;  }
    اگر ((x & 0x80000000) == 0) { شمار += 1; }
    بازگشت شمار;
#endif
}

ایستا درونخطی unsigned صحیح
next_pow2(unsigned صحیح x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    اگر (x <= 1) بازگشت 1u;
    بازگشت 1u << ((صحیح)اندازه(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#وگرنه
    اگر (x <= 1u) بازگشت 1u;
    x--;
    برای (unsigned s = 1; s < (unsigned)اندازه(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    بازگشت x + 1u;
#endif
}

ایستا درونخطی uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned صحیح n) {
    ثابت uint32_t original = mask;

    /* Clear the lowest set bit n times */
    وقتی (n-- && mask)
        mask &= (mask - 1);

    بازگشت original ^ mask;
}
