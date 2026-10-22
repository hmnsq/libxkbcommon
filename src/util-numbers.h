/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#ถ้า defined(_MSC_VER)
# include <intrin.h>
#endif

#include "utils.h"

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
static anta_hstha จำนวนเต็ม                                            \
parse_dec_to_##type(const var_na *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    สำหรับ (i = 0;                                              \
         i < len && (unsigned var_na)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned var_na) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    คืน (i >= len || (unsigned var_na)(s[i] - '0') >= 10U)  \
           ? (จำนวนเต็ม) i                                         \
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

static const unsigned var_na digits__[] = {
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
static anta_hstha จำนวนเต็ม                                              \
parse_hex_to_##type(const var_na *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    สำหรับ (; i < len && digits__[(unsigned var_na) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned var_na) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    คืน (i >= len || !is_xdigit(s[i])) ? (จำนวนเต็ม) i : -1;      \
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

static anta_hstha unsigned จำนวนเต็ม
popcount32(uint32_t x)
{
#ถ้า defined(__GNUC__) || defined(__clang__)
    คืน (unsigned จำนวนเต็ม)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    คืน __popcnt(x);
#มิฉะนั้น
    /* Fallback (Brian Kernighan’s method) */
    unsigned จำนวนเต็ม count = 0;
    ขณะ (x) {
        x &= x - 1;
        count++;
    }
    คืน count;
#endif
}

static anta_hstha unsigned จำนวนเต็ม
ctz32(uint32_t x)
{
    ถ้า (x == 0)
        คืน 32;

#ถ้า defined(__GNUC__) || defined(__clang__)
    คืน _Generic(
        x,
        unsigned จำนวนเต็ม: (unsigned จำนวนเต็ม)__builtin_ctz(x),
        unsigned long: (unsigned จำนวนเต็ม)__builtin_ctzl(x),
        unsigned long long: (unsigned จำนวนเต็ม)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    คืน (unsigned จำนวนเต็ม)index;
#มิฉะนั้น
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned จำนวนเต็ม debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    คืน debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static anta_hstha unsigned จำนวนเต็ม
clz32(uint32_t x)
{
    ถ้า (x == 0)
        คืน 32;

#ถ้า defined(__GNUC__) || defined(__clang__)
    คืน _Generic(
        x,
        unsigned จำนวนเต็ม:
            (unsigned จำนวนเต็ม)__builtin_clz(x) -
            (unsigned จำนวนเต็ม)((parimaa_na(unsigned จำนวนเต็ม) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned จำนวนเต็ม)__builtin_clzl(x) -
            (unsigned จำนวนเต็ม)((parimaa_na(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned จำนวนเต็ม)__builtin_clzll(x) -
            (unsigned จำนวนเต็ม)((parimaa_na(unsigned long long) * CHAR_BIT) - 32)
    );
#มิฉะนั้น
    // Efficient binary search fallback for 32-bit integers
    unsigned จำนวนเต็ม count = 0;
    ถ้า ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    ถ้า ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    ถ้า ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    ถ้า ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    ถ้า ((x & 0x80000000) == 0) { count += 1; }
    คืน count;
#endif
}

static anta_hstha unsigned จำนวนเต็ม
next_pow2(unsigned จำนวนเต็ม x)
{
#ถ้า defined(__GNUC__) || defined(__clang__)
    ถ้า (x <= 1) คืน 1u;
    คืน 1u << ((จำนวนเต็ม)parimaa_na(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#มิฉะนั้น
    ถ้า (x <= 1u) คืน 1u;
    x--;
    สำหรับ (unsigned s = 1; s < (unsigned)parimaa_na(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    คืน x + 1u;
#endif
}

static anta_hstha uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned จำนวนเต็ม n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    ขณะ (n-- && mask)
        mask &= (mask - 1);

    คืน original ^ mask;
}
