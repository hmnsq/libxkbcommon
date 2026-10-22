/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stdint.h>

#જો defined(_MSC_VER)
# include <intrin.h>
#endif

#અન્તર્ભાવ "utils.h"

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
સ્થિર અંતઃરેખ પૂર્ણાંક                                            \
parse_dec_to_##type(અચલ અક્ષર *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    માટે (i = 0;                                              \
         i < len && (unsigned અક્ષર)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned અક્ષર) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    પાછા (i >= len || (unsigned અક્ષર)(s[i] - '0') >= 10U)  \
           ? (પૂર્ણાંક) i                                         \
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

સ્થિર અચલ unsigned અક્ષર digits__[] = {
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
સ્થિર અંતઃરેખ પૂર્ણાંક                                              \
parse_hex_to_##type(અચલ અક્ષર *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    માટે (; i < len && digits__[(unsigned અક્ષર) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned અક્ષર) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    પાછા (i >= len || !is_xdigit(s[i])) ? (પૂર્ણાંક) i : -1;      \
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

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
popcount32(uint32_t x)
{
#જો defined(__GNUC__) || defined(__clang__)
    પાછા (unsigned પૂર્ણાંક)__builtin_popcountl(x);
#અથવાજો defined(_MSC_VER)
    પાછા __popcnt(x);
#નહીંતર
    /* Fallback (Brian Kernighan’s method) */
    unsigned પૂર્ણાંક ગણતરી = 0;
    જ્યારે (x) {
        x &= x - 1;
        ગણતરી++;
    }
    પાછા ગણતરી;
#endif
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
ctz32(uint32_t x)
{
    જો (x == 0)
        પાછા 32;

#જો defined(__GNUC__) || defined(__clang__)
    પાછા _Generic(
        x,
        unsigned પૂર્ણાંક: (unsigned પૂર્ણાંક)__builtin_ctz(x),
        unsigned long: (unsigned પૂર્ણાંક)__builtin_ctzl(x),
        unsigned long long: (unsigned પૂર્ણાંક)__builtin_ctzll(x)
    );
#અથવાજો defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    પાછા (unsigned પૂર્ણાંક)index;
#નહીંતર
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    સ્થિર અચલ unsigned પૂર્ણાંક debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    પાછા debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
clz32(uint32_t x)
{
    જો (x == 0)
        પાછા 32;

#જો defined(__GNUC__) || defined(__clang__)
    પાછા _Generic(
        x,
        unsigned પૂર્ણાંક:
            (unsigned પૂર્ણાંક)__builtin_clz(x) -
            (unsigned પૂર્ણાંક)((કદ(unsigned પૂર્ણાંક) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned પૂર્ણાંક)__builtin_clzl(x) -
            (unsigned પૂર્ણાંક)((કદ(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned પૂર્ણાંક)__builtin_clzll(x) -
            (unsigned પૂર્ણાંક)((કદ(unsigned long long) * CHAR_BIT) - 32)
    );
#નહીંતર
    // Efficient binary search fallback for 32-bit integers
    unsigned પૂર્ણાંક ગણતરી = 0;
    જો ((x & 0xffff0000) == 0) { ગણતરી += 16; x <<= 16; }
    જો ((x & 0xff000000) == 0) { ગણતરી += 8;  x <<= 8;  }
    જો ((x & 0xf0000000) == 0) { ગણતરી += 4;  x <<= 4;  }
    જો ((x & 0xc0000000) == 0) { ગણતરી += 2;  x <<= 2;  }
    જો ((x & 0x80000000) == 0) { ગણતરી += 1; }
    પાછા ગણતરી;
#endif
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
next_pow2(unsigned પૂર્ણાંક x)
{
#જો defined(__GNUC__) || defined(__clang__)
    જો (x <= 1) પાછા 1u;
    પાછા 1u << ((પૂર્ણાંક)કદ(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#નહીંતર
    જો (x <= 1u) પાછા 1u;
    x--;
    માટે (unsigned s = 1; s < (unsigned)કદ(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    પાછા x + 1u;
#endif
}

સ્થિર અંતઃરેખ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned પૂર્ણાંક n) {
    અચલ uint32_t original = mask;

    /* Clear the lowest set bit n times */
    જ્યારે (n-- && mask)
        mask &= (mask - 1);

    પાછા original ^ mask;
}
