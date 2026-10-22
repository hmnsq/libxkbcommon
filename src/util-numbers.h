/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#если defined(_MSC_VER)
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
static inline целое                                            \
parse_dec_to_##type(const char *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    для (i = 0;                                              \
         i < len && (unsigned char)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned char) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    вернуть (i >= len || (unsigned char)(s[i] - '0') >= 10U)  \
           ? (целое) i                                         \
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
static inline целое                                              \
parse_hex_to_##type(const char *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    для (; i < len && digits__[(unsigned char) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned char) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    вернуть (i >= len || !is_xdigit(s[i])) ? (целое) i : -1;      \
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

static inline unsigned целое
popcount32(uint32_t x)
{
#если defined(__GNUC__) || defined(__clang__)
    вернуть (unsigned целое)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    вернуть __popcnt(x);
#иначе
    /* Fallback (Brian Kernighan’s method) */
    unsigned целое count = 0;
    пока (x) {
        x &= x - 1;
        count++;
    }
    вернуть count;
#endif
}

static inline unsigned целое
ctz32(uint32_t x)
{
    если (x == 0)
        вернуть 32;

#если defined(__GNUC__) || defined(__clang__)
    вернуть _Generic(
        x,
        unsigned целое: (unsigned целое)__builtin_ctz(x),
        unsigned long: (unsigned целое)__builtin_ctzl(x),
        unsigned long long: (unsigned целое)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    вернуть (unsigned целое)index;
#иначе
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned целое debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    вернуть debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static inline unsigned целое
clz32(uint32_t x)
{
    если (x == 0)
        вернуть 32;

#если defined(__GNUC__) || defined(__clang__)
    вернуть _Generic(
        x,
        unsigned целое:
            (unsigned целое)__builtin_clz(x) -
            (unsigned целое)((sizeof(unsigned целое) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned целое)__builtin_clzl(x) -
            (unsigned целое)((sizeof(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned целое)__builtin_clzll(x) -
            (unsigned целое)((sizeof(unsigned long long) * CHAR_BIT) - 32)
    );
#иначе
    // Efficient binary search fallback for 32-bit integers
    unsigned целое count = 0;
    если ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    если ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    если ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    если ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    если ((x & 0x80000000) == 0) { count += 1; }
    вернуть count;
#endif
}

static inline unsigned целое
next_pow2(unsigned целое x)
{
#если defined(__GNUC__) || defined(__clang__)
    если (x <= 1) вернуть 1u;
    вернуть 1u << ((целое)sizeof(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#иначе
    если (x <= 1u) вернуть 1u;
    x--;
    для (unsigned s = 1; s < (unsigned)sizeof(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    вернуть x + 1u;
#endif
}

static inline uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned целое n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    пока (n-- && mask)
        mask &= (mask - 1);

    вернуть original ^ mask;
}
