/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdint.h>

#যদি defined(_MSC_VER)
# include <intrin.h>
#endif

#অন্তর্ভাব "utils.h"

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
স্থির অন্তঃরেখ পূর্ণসংখ্যা                                            \
parse_dec_to_##type(ধ্রুবক অক্ষর *s, আকার_t len, type (*out))  \
{                                                            \
    type ফলাফল = 0;                                         \
    আকার_t i;                                                \
    জন্য (i = 0;                                              \
         i < len && (unsigned অক্ষর)(s[i] - '0') < 10U &&     \
         ফলাফল <= (max) / 10 &&                             \
         ফলাফল * 10 <= (max) - (unsigned অক্ষর) (s[i] - '0');\
         i++) {                                              \
        ফলাফল = ফলাফল * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = ফলাফল;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ফেরত (i >= len || (unsigned অক্ষর)(s[i] - '0') >= 10U)  \
           ? (পূর্ণসংখ্যা) i                                         \
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

স্থির ধ্রুবক unsigned অক্ষর digits__[] = {
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
স্থির অন্তঃরেখ পূর্ণসংখ্যা                                              \
parse_hex_to_##type(ধ্রুবক অক্ষর *s, আকার_t len, type (*out))    \
{                                                              \
    type ফলাফল = 0;                                           \
    আকার_t i = 0;                                              \
    জন্য (; i < len && digits__[(unsigned অক্ষর) s[i]] < 16u &&  \
         ফলাফল <= (max) >> 4;                                 \
         i++) {                                                \
        ফলাফল = ফলাফল * 16 + digits__[(unsigned অক্ষর) s[i]]; \
    }                                                          \
    *out = ফলাফল;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ফেরত (i >= len || !is_xdigit(s[i])) ? (পূর্ণসংখ্যা) i : -1;      \
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

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
popcount32(uint32_t x)
{
#যদি defined(__GNUC__) || defined(__clang__)
    ফেরত (unsigned পূর্ণসংখ্যা)__builtin_popcountl(x);
#নতুবা_যদি defined(_MSC_VER)
    ফেরত __popcnt(x);
#অন্যথা
    /* Fallback (Brian Kernighan’s method) */
    unsigned পূর্ণসংখ্যা গুনতি = 0;
    যখন (x) {
        x &= x - 1;
        গুনতি++;
    }
    ফেরত গুনতি;
#endif
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
ctz32(uint32_t x)
{
    যদি (x == 0)
        ফেরত 32;

#যদি defined(__GNUC__) || defined(__clang__)
    ফেরত _Generic(
        x,
        unsigned পূর্ণসংখ্যা: (unsigned পূর্ণসংখ্যা)__builtin_ctz(x),
        unsigned long: (unsigned পূর্ণসংখ্যা)__builtin_ctzl(x),
        unsigned long long: (unsigned পূর্ণসংখ্যা)__builtin_ctzll(x)
    );
#নতুবা_যদি defined(_MSC_VER)
    unsigned long সূচক;
    _BitScanForward(&সূচক, x);
    ফেরত (unsigned পূর্ণসংখ্যা)সূচক;
#অন্যথা
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    স্থির ধ্রুবক unsigned পূর্ণসংখ্যা debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ফেরত debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
clz32(uint32_t x)
{
    যদি (x == 0)
        ফেরত 32;

#যদি defined(__GNUC__) || defined(__clang__)
    ফেরত _Generic(
        x,
        unsigned পূর্ণসংখ্যা:
            (unsigned পূর্ণসংখ্যা)__builtin_clz(x) -
            (unsigned পূর্ণসংখ্যা)((আকার(unsigned পূর্ণসংখ্যা) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned পূর্ণসংখ্যা)__builtin_clzl(x) -
            (unsigned পূর্ণসংখ্যা)((আকার(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned পূর্ণসংখ্যা)__builtin_clzll(x) -
            (unsigned পূর্ণসংখ্যা)((আকার(unsigned long long) * CHAR_BIT) - 32)
    );
#অন্যথা
    // Efficient binary search fallback for 32-bit integers
    unsigned পূর্ণসংখ্যা গুনতি = 0;
    যদি ((x & 0xffff0000) == 0) { গুনতি += 16; x <<= 16; }
    যদি ((x & 0xff000000) == 0) { গুনতি += 8;  x <<= 8;  }
    যদি ((x & 0xf0000000) == 0) { গুনতি += 4;  x <<= 4;  }
    যদি ((x & 0xc0000000) == 0) { গুনতি += 2;  x <<= 2;  }
    যদি ((x & 0x80000000) == 0) { গুনতি += 1; }
    ফেরত গুনতি;
#endif
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
পরবর্তী_pow2(unsigned পূর্ণসংখ্যা x)
{
#যদি defined(__GNUC__) || defined(__clang__)
    যদি (x <= 1) ফেরত 1u;
    ফেরত 1u << ((পূর্ণসংখ্যা)আকার(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#অন্যথা
    যদি (x <= 1u) ফেরত 1u;
    x--;
    জন্য (unsigned s = 1; s < (unsigned)আকার(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ফেরত x + 1u;
#endif
}

স্থির অন্তঃরেখ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned পূর্ণসংখ্যা n) {
    ধ্রুবক uint32_t original = mask;

    /* Clear the lowest set bit n times */
    যখন (n-- && mask)
        mask &= (mask - 1);

    ফেরত original ^ mask;
}
