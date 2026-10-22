/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#만약 defined(_MSC_VER)
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
static inline 정수                                            \
parse_dec_to_##type(const char *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    위해 (i = 0;                                              \
         i < len && (unsigned char)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned char) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    반환 (i >= len || (unsigned char)(s[i] - '0') >= 10U)  \
           ? (정수) i                                         \
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

static const unsigned char digits__[] = {
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
static inline 정수                                              \
parse_hex_to_##type(const char *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    위해 (; i < len && digits__[(unsigned char) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned char) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    반환 (i >= len || !is_xdigit(s[i])) ? (정수) i : -1;      \
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

static inline unsigned 정수
popcount32(uint32_t x)
{
#만약 defined(__GNUC__) || defined(__clang__)
    반환 (unsigned 정수)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    반환 __popcnt(x);
#아니면
    /* Fallback (Brian Kernighan’s method) */
    unsigned 정수 count = 0;
    동안 (x) {
        x &= x - 1;
        count++;
    }
    반환 count;
#endif
}

static inline unsigned 정수
ctz32(uint32_t x)
{
    만약 (x == 0)
        반환 32;

#만약 defined(__GNUC__) || defined(__clang__)
    반환 _Generic(
        x,
        unsigned 정수: (unsigned 정수)__builtin_ctz(x),
        unsigned long: (unsigned 정수)__builtin_ctzl(x),
        unsigned long long: (unsigned 정수)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    반환 (unsigned 정수)index;
#아니면
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned 정수 debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    반환 debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static inline unsigned 정수
clz32(uint32_t x)
{
    만약 (x == 0)
        반환 32;

#만약 defined(__GNUC__) || defined(__clang__)
    반환 _Generic(
        x,
        unsigned 정수:
            (unsigned 정수)__builtin_clz(x) -
            (unsigned 정수)((sizeof(unsigned 정수) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned 정수)__builtin_clzl(x) -
            (unsigned 정수)((sizeof(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned 정수)__builtin_clzll(x) -
            (unsigned 정수)((sizeof(unsigned long long) * CHAR_BIT) - 32)
    );
#아니면
    // Efficient binary search fallback for 32-bit integers
    unsigned 정수 count = 0;
    만약 ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    만약 ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    만약 ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    만약 ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    만약 ((x & 0x80000000) == 0) { count += 1; }
    반환 count;
#endif
}

static inline unsigned 정수
next_pow2(unsigned 정수 x)
{
#만약 defined(__GNUC__) || defined(__clang__)
    만약 (x <= 1) 반환 1u;
    반환 1u << ((정수)sizeof(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#아니면
    만약 (x <= 1u) 반환 1u;
    x--;
    위해 (unsigned s = 1; s < (unsigned)sizeof(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    반환 x + 1u;
#endif
}

static inline uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned 정수 n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    동안 (n-- && mask)
        mask &= (mask - 1);

    반환 original ^ mask;
}
