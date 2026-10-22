/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#အကယ်၍ defined(_MSC_VER)
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
static anta_hstha ကိန်းပြည့်                                            \
parse_dec_to_##type(const var_na *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    အတွက် (i = 0;                                              \
         i < len && (unsigned var_na)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned var_na) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ပြန်ပေး (i >= len || (unsigned var_na)(s[i] - '0') >= 10U)  \
           ? (ကိန်းပြည့်) i                                         \
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
static anta_hstha ကိန်းပြည့်                                              \
parse_hex_to_##type(const var_na *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    အတွက် (; i < len && digits__[(unsigned var_na) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned var_na) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ပြန်ပေး (i >= len || !is_xdigit(s[i])) ? (ကိန်းပြည့်) i : -1;      \
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

static anta_hstha unsigned ကိန်းပြည့်
popcount32(uint32_t x)
{
#အကယ်၍ defined(__GNUC__) || defined(__clang__)
    ပြန်ပေး (unsigned ကိန်းပြည့်)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    ပြန်ပေး __popcnt(x);
#မဟုတ်ပါက
    /* Fallback (Brian Kernighan’s method) */
    unsigned ကိန်းပြည့် count = 0;
    နေစဉ် (x) {
        x &= x - 1;
        count++;
    }
    ပြန်ပေး count;
#endif
}

static anta_hstha unsigned ကိန်းပြည့်
ctz32(uint32_t x)
{
    အကယ်၍ (x == 0)
        ပြန်ပေး 32;

#အကယ်၍ defined(__GNUC__) || defined(__clang__)
    ပြန်ပေး _Generic(
        x,
        unsigned ကိန်းပြည့်: (unsigned ကိန်းပြည့်)__builtin_ctz(x),
        unsigned long: (unsigned ကိန်းပြည့်)__builtin_ctzl(x),
        unsigned long long: (unsigned ကိန်းပြည့်)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ပြန်ပေး (unsigned ကိန်းပြည့်)index;
#မဟုတ်ပါက
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned ကိန်းပြည့် debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ပြန်ပေး debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static anta_hstha unsigned ကိန်းပြည့်
clz32(uint32_t x)
{
    အကယ်၍ (x == 0)
        ပြန်ပေး 32;

#အကယ်၍ defined(__GNUC__) || defined(__clang__)
    ပြန်ပေး _Generic(
        x,
        unsigned ကိန်းပြည့်:
            (unsigned ကိန်းပြည့်)__builtin_clz(x) -
            (unsigned ကိန်းပြည့်)((parimaa_na(unsigned ကိန်းပြည့်) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned ကိန်းပြည့်)__builtin_clzl(x) -
            (unsigned ကိန်းပြည့်)((parimaa_na(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned ကိန်းပြည့်)__builtin_clzll(x) -
            (unsigned ကိန်းပြည့်)((parimaa_na(unsigned long long) * CHAR_BIT) - 32)
    );
#မဟုတ်ပါက
    // Efficient binary search fallback for 32-bit integers
    unsigned ကိန်းပြည့် count = 0;
    အကယ်၍ ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    အကယ်၍ ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    အကယ်၍ ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    အကယ်၍ ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    အကယ်၍ ((x & 0x80000000) == 0) { count += 1; }
    ပြန်ပေး count;
#endif
}

static anta_hstha unsigned ကိန်းပြည့်
next_pow2(unsigned ကိန်းပြည့် x)
{
#အကယ်၍ defined(__GNUC__) || defined(__clang__)
    အကယ်၍ (x <= 1) ပြန်ပေး 1u;
    ပြန်ပေး 1u << ((ကိန်းပြည့်)parimaa_na(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#မဟုတ်ပါက
    အကယ်၍ (x <= 1u) ပြန်ပေး 1u;
    x--;
    အတွက် (unsigned s = 1; s < (unsigned)parimaa_na(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ပြန်ပေး x + 1u;
#endif
}

static anta_hstha uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned ကိန်းပြည့် n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    နေစဉ် (n-- && mask)
        mask &= (mask - 1);

    ပြန်ပေး original ^ mask;
}
