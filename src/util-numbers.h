/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stdint.h>

#எனில் defined(_MSC_VER)
# include <intrin.h>
#endif

#அந்தர்பாவ "utils.h"

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
நிலையான உள்வரி முழு                                            \
parse_dec_to_##type(மாறிலி எழுத்து *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    க்கு (i = 0;                                              \
         i < len && (unsigned எழுத்து)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned எழுத்து) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    திரும்பு (i >= len || (unsigned எழுத்து)(s[i] - '0') >= 10U)  \
           ? (முழு) i                                         \
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

நிலையான மாறிலி unsigned எழுத்து digits__[] = {
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
நிலையான உள்வரி முழு                                              \
parse_hex_to_##type(மாறிலி எழுத்து *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    க்கு (; i < len && digits__[(unsigned எழுத்து) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned எழுத்து) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    திரும்பு (i >= len || !is_xdigit(s[i])) ? (முழு) i : -1;      \
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

நிலையான உள்வரி unsigned முழு
popcount32(uint32_t x)
{
#எனில் defined(__GNUC__) || defined(__clang__)
    திரும்பு (unsigned முழு)__builtin_popcountl(x);
#அல்லதெனில் defined(_MSC_VER)
    திரும்பு __popcnt(x);
#இல்லையெனில்
    /* Fallback (Brian Kernighan’s method) */
    unsigned முழு மொத்தம் = 0;
    போது (x) {
        x &= x - 1;
        மொத்தம்++;
    }
    திரும்பு மொத்தம்;
#endif
}

நிலையான உள்வரி unsigned முழு
ctz32(uint32_t x)
{
    எனில் (x == 0)
        திரும்பு 32;

#எனில் defined(__GNUC__) || defined(__clang__)
    திரும்பு _Generic(
        x,
        unsigned முழு: (unsigned முழு)__builtin_ctz(x),
        unsigned long: (unsigned முழு)__builtin_ctzl(x),
        unsigned long long: (unsigned முழு)__builtin_ctzll(x)
    );
#அல்லதெனில் defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    திரும்பு (unsigned முழு)index;
#இல்லையெனில்
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    நிலையான மாறிலி unsigned முழு debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    திரும்பு debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

நிலையான உள்வரி unsigned முழு
clz32(uint32_t x)
{
    எனில் (x == 0)
        திரும்பு 32;

#எனில் defined(__GNUC__) || defined(__clang__)
    திரும்பு _Generic(
        x,
        unsigned முழு:
            (unsigned முழு)__builtin_clz(x) -
            (unsigned முழு)((அளவு(unsigned முழு) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned முழு)__builtin_clzl(x) -
            (unsigned முழு)((அளவு(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned முழு)__builtin_clzll(x) -
            (unsigned முழு)((அளவு(unsigned long long) * CHAR_BIT) - 32)
    );
#இல்லையெனில்
    // Efficient binary search fallback for 32-bit integers
    unsigned முழு மொத்தம் = 0;
    எனில் ((x & 0xffff0000) == 0) { மொத்தம் += 16; x <<= 16; }
    எனில் ((x & 0xff000000) == 0) { மொத்தம் += 8;  x <<= 8;  }
    எனில் ((x & 0xf0000000) == 0) { மொத்தம் += 4;  x <<= 4;  }
    எனில் ((x & 0xc0000000) == 0) { மொத்தம் += 2;  x <<= 2;  }
    எனில் ((x & 0x80000000) == 0) { மொத்தம் += 1; }
    திரும்பு மொத்தம்;
#endif
}

நிலையான உள்வரி unsigned முழு
next_pow2(unsigned முழு x)
{
#எனில் defined(__GNUC__) || defined(__clang__)
    எனில் (x <= 1) திரும்பு 1u;
    திரும்பு 1u << ((முழு)அளவு(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#இல்லையெனில்
    எனில் (x <= 1u) திரும்பு 1u;
    x--;
    க்கு (unsigned s = 1; s < (unsigned)அளவு(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    திரும்பு x + 1u;
#endif
}

நிலையான உள்வரி uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned முழு n) {
    மாறிலி uint32_t original = mask;

    /* Clear the lowest set bit n times */
    போது (n-- && mask)
        mask &= (mask - 1);

    திரும்பு original ^ mask;
}
