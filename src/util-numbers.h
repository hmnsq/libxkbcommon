/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଯଦି defined(_MSC_VER)
# include <intrin.h>
#endif

#ଅନ୍ତର୍ଭାଵ "utils.h"

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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା                                            \
parse_dec_to_##type(ଅଚଳ ଅକ୍ଷର *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    ପାଇଁ (i = 0;                                              \
         i < len && (unsigned ଅକ୍ଷର)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned ଅକ୍ଷର) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ଫେରନ୍ତୁ (i >= len || (unsigned ଅକ୍ଷର)(s[i] - '0') >= 10U)  \
           ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i                                         \
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

ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର digits__[] = {
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା                                              \
parse_hex_to_##type(ଅଚଳ ଅକ୍ଷର *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    ପାଇଁ (; i < len && digits__[(unsigned ଅକ୍ଷର) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned ଅକ୍ଷର) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ଫେରନ୍ତୁ (i >= len || !is_xdigit(s[i])) ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i : -1;      \
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
popcount32(uint32_t x)
{
#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଫେରନ୍ତୁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_popcountl(x);
#ନଚେତଯଦି defined(_MSC_VER)
    ଫେରନ୍ତୁ __popcnt(x);
#ଅନ୍ୟଥା
    /* Fallback (Brian Kernighan’s method) */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ଯେତେବେଳେ (x) {
        x &= x - 1;
        ଗଣନା_2++;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ctz32(uint32_t x)
{
    ଯଦି (x == 0)
        ଫେରନ୍ତୁ 32;

#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଫେରନ୍ତୁ _Generic(
        x,
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା: (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_ctz(x),
        unsigned long: (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_ctzl(x),
        unsigned long long: (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_ctzll(x)
    );
#ନଚେତଯଦି defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ଫେରନ୍ତୁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)index;
#ଅନ୍ୟଥା
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ସ୍ଥିର ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ଫେରନ୍ତୁ debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
clz32(uint32_t x)
{
    ଯଦି (x == 0)
        ଫେରନ୍ତୁ 32;

#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଫେରନ୍ତୁ _Generic(
        x,
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା:
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_clz(x) -
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_clzl(x) -
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((ଆକାର(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_clzll(x) -
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((ଆକାର(unsigned long long) * CHAR_BIT) - 32)
    );
#ଅନ୍ୟଥା
    // Efficient binary search fallback for 32-bit integers
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ଯଦି ((x & 0xffff0000) == 0) { ଗଣନା_2 += 16; x <<= 16; }
    ଯଦି ((x & 0xff000000) == 0) { ଗଣନା_2 += 8;  x <<= 8;  }
    ଯଦି ((x & 0xf0000000) == 0) { ଗଣନା_2 += 4;  x <<= 4;  }
    ଯଦି ((x & 0xc0000000) == 0) { ଗଣନା_2 += 2;  x <<= 2;  }
    ଯଦି ((x & 0x80000000) == 0) { ଗଣନା_2 += 1; }
    ଫେରନ୍ତୁ ଗଣନା_2;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
next_pow2(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା x)
{
#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଯଦି (x <= 1) ଫେରନ୍ତୁ 1u;
    ଫେରନ୍ତୁ 1u << ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#ଅନ୍ୟଥା
    ଯଦି (x <= 1u) ଫେରନ୍ତୁ 1u;
    x--;
    ପାଇଁ (unsigned s = 1; s < (unsigned)ଆକାର(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ଫେରନ୍ତୁ x + 1u;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) {
    ଅଚଳ uint32_t original = mask;

    /* Clear the lowest set bit n times */
    ଯେତେବେଳେ (n-- && mask)
        mask &= (mask - 1);

    ଫେରନ୍ତୁ original ^ mask;
}
