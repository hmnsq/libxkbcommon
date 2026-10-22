/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#གལ་ཏེ defined(_MSC_VER)
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
static anta_hstha ཧྲིལ                                            \
parse_dec_to_##type(const var_na *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    ཆེད་དུ (i = 0;                                              \
         i < len && (unsigned var_na)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned var_na) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ཕྱིར (i >= len || (unsigned var_na)(s[i] - '0') >= 10U)  \
           ? (ཧྲིལ) i                                         \
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
static anta_hstha ཧྲིལ                                              \
parse_hex_to_##type(const var_na *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    ཆེད་དུ (; i < len && digits__[(unsigned var_na) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned var_na) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ཕྱིར (i >= len || !is_xdigit(s[i])) ? (ཧྲིལ) i : -1;      \
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

static anta_hstha unsigned ཧྲིལ
popcount32(uint32_t x)
{
#གལ་ཏེ defined(__GNUC__) || defined(__clang__)
    ཕྱིར (unsigned ཧྲིལ)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    ཕྱིར __popcnt(x);
#དེ་མིན
    /* Fallback (Brian Kernighan’s method) */
    unsigned ཧྲིལ count = 0;
    སྐབས (x) {
        x &= x - 1;
        count++;
    }
    ཕྱིར count;
#endif
}

static anta_hstha unsigned ཧྲིལ
ctz32(uint32_t x)
{
    གལ་ཏེ (x == 0)
        ཕྱིར 32;

#གལ་ཏེ defined(__GNUC__) || defined(__clang__)
    ཕྱིར _Generic(
        x,
        unsigned ཧྲིལ: (unsigned ཧྲིལ)__builtin_ctz(x),
        unsigned long: (unsigned ཧྲིལ)__builtin_ctzl(x),
        unsigned long long: (unsigned ཧྲིལ)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ཕྱིར (unsigned ཧྲིལ)index;
#དེ་མིན
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned ཧྲིལ debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ཕྱིར debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static anta_hstha unsigned ཧྲིལ
clz32(uint32_t x)
{
    གལ་ཏེ (x == 0)
        ཕྱིར 32;

#གལ་ཏེ defined(__GNUC__) || defined(__clang__)
    ཕྱིར _Generic(
        x,
        unsigned ཧྲིལ:
            (unsigned ཧྲིལ)__builtin_clz(x) -
            (unsigned ཧྲིལ)((parimaa_na(unsigned ཧྲིལ) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned ཧྲིལ)__builtin_clzl(x) -
            (unsigned ཧྲིལ)((parimaa_na(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned ཧྲིལ)__builtin_clzll(x) -
            (unsigned ཧྲིལ)((parimaa_na(unsigned long long) * CHAR_BIT) - 32)
    );
#དེ་མིན
    // Efficient binary search fallback for 32-bit integers
    unsigned ཧྲིལ count = 0;
    གལ་ཏེ ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    གལ་ཏེ ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    གལ་ཏེ ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    གལ་ཏེ ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    གལ་ཏེ ((x & 0x80000000) == 0) { count += 1; }
    ཕྱིར count;
#endif
}

static anta_hstha unsigned ཧྲིལ
next_pow2(unsigned ཧྲིལ x)
{
#གལ་ཏེ defined(__GNUC__) || defined(__clang__)
    གལ་ཏེ (x <= 1) ཕྱིར 1u;
    ཕྱིར 1u << ((ཧྲིལ)parimaa_na(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#དེ་མིན
    གལ་ཏེ (x <= 1u) ཕྱིར 1u;
    x--;
    ཆེད་དུ (unsigned s = 1; s < (unsigned)parimaa_na(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ཕྱིར x + 1u;
#endif
}

static anta_hstha uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned ཧྲིལ n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    སྐབས (n-- && mask)
        mask &= (mask - 1);

    ཕྱིར original ^ mask;
}
