/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#නම් defined(_MSC_VER)
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
static anta_hstha පූර්ණ_සංඛ්‍යා                                            \
parse_dec_to_##type(const var_na *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    සඳහා (i = 0;                                              \
         i < len && (unsigned var_na)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned var_na) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ආපසු (i >= len || (unsigned var_na)(s[i] - '0') >= 10U)  \
           ? (පූර්ණ_සංඛ්‍යා) i                                         \
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
static anta_hstha පූර්ණ_සංඛ්‍යා                                              \
parse_hex_to_##type(const var_na *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    සඳහා (; i < len && digits__[(unsigned var_na) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned var_na) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ආපසු (i >= len || !is_xdigit(s[i])) ? (පූර්ණ_සංඛ්‍යා) i : -1;      \
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

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
popcount32(uint32_t x)
{
#නම් defined(__GNUC__) || defined(__clang__)
    ආපසු (unsigned පූර්ණ_සංඛ්‍යා)__builtin_popcountl(x);
#elif defined(_MSC_VER)
    ආපසු __popcnt(x);
#නැතහොත්
    /* Fallback (Brian Kernighan’s method) */
    unsigned පූර්ණ_සංඛ්‍යා count = 0;
    අතරතුර (x) {
        x &= x - 1;
        count++;
    }
    ආපසු count;
#endif
}

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
ctz32(uint32_t x)
{
    නම් (x == 0)
        ආපසු 32;

#නම් defined(__GNUC__) || defined(__clang__)
    ආපසු _Generic(
        x,
        unsigned පූර්ණ_සංඛ්‍යා: (unsigned පූර්ණ_සංඛ්‍යා)__builtin_ctz(x),
        unsigned long: (unsigned පූර්ණ_සංඛ්‍යා)__builtin_ctzl(x),
        unsigned long long: (unsigned පූර්ණ_සංඛ්‍යා)__builtin_ctzll(x)
    );
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ආපසු (unsigned පූර්ණ_සංඛ්‍යා)index;
#නැතහොත්
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    static const unsigned පූර්ණ_සංඛ්‍යා debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ආපසු debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
clz32(uint32_t x)
{
    නම් (x == 0)
        ආපසු 32;

#නම් defined(__GNUC__) || defined(__clang__)
    ආපසු _Generic(
        x,
        unsigned පූර්ණ_සංඛ්‍යා:
            (unsigned පූර්ණ_සංඛ්‍යා)__builtin_clz(x) -
            (unsigned පූර්ණ_සංඛ්‍යා)((parimaa_na(unsigned පූර්ණ_සංඛ්‍යා) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned පූර්ණ_සංඛ්‍යා)__builtin_clzl(x) -
            (unsigned පූර්ණ_සංඛ්‍යා)((parimaa_na(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned පූර්ණ_සංඛ්‍යා)__builtin_clzll(x) -
            (unsigned පූර්ණ_සංඛ්‍යා)((parimaa_na(unsigned long long) * CHAR_BIT) - 32)
    );
#නැතහොත්
    // Efficient binary search fallback for 32-bit integers
    unsigned පූර්ණ_සංඛ්‍යා count = 0;
    නම් ((x & 0xffff0000) == 0) { count += 16; x <<= 16; }
    නම් ((x & 0xff000000) == 0) { count += 8;  x <<= 8;  }
    නම් ((x & 0xf0000000) == 0) { count += 4;  x <<= 4;  }
    නම් ((x & 0xc0000000) == 0) { count += 2;  x <<= 2;  }
    නම් ((x & 0x80000000) == 0) { count += 1; }
    ආපසු count;
#endif
}

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
next_pow2(unsigned පූර්ණ_සංඛ්‍යා x)
{
#නම් defined(__GNUC__) || defined(__clang__)
    නම් (x <= 1) ආපසු 1u;
    ආපසු 1u << ((පූර්ණ_සංඛ්‍යා)parimaa_na(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#නැතහොත්
    නම් (x <= 1u) ආපසු 1u;
    x--;
    සඳහා (unsigned s = 1; s < (unsigned)parimaa_na(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ආපසු x + 1u;
#endif
}

static anta_hstha uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned පූර්ණ_සංඛ්‍යා n) {
    const uint32_t original = mask;

    /* Clear the lowest set bit n times */
    අතරතුර (n-- && mask)
        mask &= (mask - 1);

    ආපසු original ^ mask;
}
