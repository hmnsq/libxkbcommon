/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>

#सचे defined(_MSC_VER)
# include <intrin.h>
#endif

#अन्तर्भाव "utils.h"

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
स्थिर अन्तःस्थ पूर्णाङ्क                                            \
parse_dec_to_##type(ध्रुव वर्ण *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    अत्थाय (i = 0;                                              \
         i < len && (unsigned वर्ण)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned वर्ण) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    पटिनिवत्त (i >= len || (unsigned वर्ण)(s[i] - '0') >= 10U)  \
           ? (पूर्णाङ्क) i                                         \
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

स्थिर ध्रुव unsigned वर्ण digits__[] = {
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
स्थिर अन्तःस्थ पूर्णाङ्क                                              \
parse_hex_to_##type(ध्रुव वर्ण *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    अत्थाय (; i < len && digits__[(unsigned वर्ण) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned वर्ण) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    पटिनिवत्त (i >= len || !is_xdigit(s[i])) ? (पूर्णाङ्क) i : -1;      \
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

स्थिर अन्तःस्थ unsigned पूर्णाङ्क
popcount32(uint32_t x)
{
#सचे defined(__GNUC__) || defined(__clang__)
    पटिनिवत्त (unsigned पूर्णाङ्क)__builtin_popcountl(x);
#नोचेद्यदि defined(_MSC_VER)
    पटिनिवत्त __popcnt(x);
#अञ्ञथा
    /* Fallback (Brian Kernighan’s method) */
    unsigned पूर्णाङ्क गणनम् = 0;
    याव (x) {
        x &= x - 1;
        गणनम्++;
    }
    पटिनिवत्त गणनम्;
#endif
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क
ctz32(uint32_t x)
{
    सचे (x == 0)
        पटिनिवत्त 32;

#सचे defined(__GNUC__) || defined(__clang__)
    पटिनिवत्त _Generic(
        x,
        unsigned पूर्णाङ्क: (unsigned पूर्णाङ्क)__builtin_ctz(x),
        unsigned long: (unsigned पूर्णाङ्क)__builtin_ctzl(x),
        unsigned long long: (unsigned पूर्णाङ्क)__builtin_ctzll(x)
    );
#नोचेद्यदि defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    पटिनिवत्त (unsigned पूर्णाङ्क)index;
#अञ्ञथा
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    स्थिर ध्रुव unsigned पूर्णाङ्क debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    पटिनिवत्त debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क
clz32(uint32_t x)
{
    सचे (x == 0)
        पटिनिवत्त 32;

#सचे defined(__GNUC__) || defined(__clang__)
    पटिनिवत्त _Generic(
        x,
        unsigned पूर्णाङ्क:
            (unsigned पूर्णाङ्क)__builtin_clz(x) -
            (unsigned पूर्णाङ्क)((परिमाण(unsigned पूर्णाङ्क) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned पूर्णाङ्क)__builtin_clzl(x) -
            (unsigned पूर्णाङ्क)((परिमाण(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned पूर्णाङ्क)__builtin_clzll(x) -
            (unsigned पूर्णाङ्क)((परिमाण(unsigned long long) * CHAR_BIT) - 32)
    );
#अञ्ञथा
    // Efficient binary search fallback for 32-bit integers
    unsigned पूर्णाङ्क गणनम् = 0;
    सचे ((x & 0xffff0000) == 0) { गणनम् += 16; x <<= 16; }
    सचे ((x & 0xff000000) == 0) { गणनम् += 8;  x <<= 8;  }
    सचे ((x & 0xf0000000) == 0) { गणनम् += 4;  x <<= 4;  }
    सचे ((x & 0xc0000000) == 0) { गणनम् += 2;  x <<= 2;  }
    सचे ((x & 0x80000000) == 0) { गणनम् += 1; }
    पटिनिवत्त गणनम्;
#endif
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क
next_pow2(unsigned पूर्णाङ्क x)
{
#सचे defined(__GNUC__) || defined(__clang__)
    सचे (x <= 1) पटिनिवत्त 1u;
    पटिनिवत्त 1u << ((पूर्णाङ्क)परिमाण(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#अञ्ञथा
    सचे (x <= 1u) पटिनिवत्त 1u;
    x--;
    अत्थाय (unsigned s = 1; s < (unsigned)परिमाण(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    पटिनिवत्त x + 1u;
#endif
}

स्थिर अन्तःस्थ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned पूर्णाङ्क n) {
    ध्रुव uint32_t original = mask;

    /* Clear the lowest set bit n times */
    याव (n-- && mask)
        mask &= (mask - 1);

    पटिनिवत्त original ^ mask;
}
