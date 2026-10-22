/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stdint.h>

#যদি defined(_MSC_VER)
# include <intrin.h>
#endif

#অন্তর্ভাৱ "utils.h"

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
স্থির অন্তঃস্থ পূর্ণাঙ্ক                                            \
parse_dec_to_##type(ধ্রুৱ ৱর্ণ *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    বাবে (i = 0;                                              \
         i < len && (unsigned ৱর্ণ)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned ৱর্ণ) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ঘূৰাই (i >= len || (unsigned ৱর্ণ)(s[i] - '0') >= 10U)  \
           ? (পূর্ণাঙ্ক) i                                         \
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

স্থির ধ্রুৱ unsigned ৱর্ণ digits__[] = {
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
স্থির অন্তঃস্থ পূর্ণাঙ্ক                                              \
parse_hex_to_##type(ধ্রুৱ ৱর্ণ *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    বাবে (; i < len && digits__[(unsigned ৱর্ণ) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned ৱর্ণ) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ঘূৰাই (i >= len || !is_xdigit(s[i])) ? (পূর্ণাঙ্ক) i : -1;      \
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

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক
popcount32(uint32_t x)
{
#যদি defined(__GNUC__) || defined(__clang__)
    ঘূৰাই (unsigned পূর্ণাঙ্ক)__builtin_popcountl(x);
#নোচেদ্যদি defined(_MSC_VER)
    ঘূৰাই __popcnt(x);
#অন্যথা
    /* Fallback (Brian Kernighan’s method) */
    unsigned পূর্ণাঙ্ক গুনতি = 0;
    যেতিয়ালৈ (x) {
        x &= x - 1;
        গুনতি++;
    }
    ঘূৰাই গুনতি;
#endif
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক
ctz32(uint32_t x)
{
    যদি (x == 0)
        ঘূৰাই 32;

#যদি defined(__GNUC__) || defined(__clang__)
    ঘূৰাই _Generic(
        x,
        unsigned পূর্ণাঙ্ক: (unsigned পূর্ণাঙ্ক)__builtin_ctz(x),
        unsigned long: (unsigned পূর্ণাঙ্ক)__builtin_ctzl(x),
        unsigned long long: (unsigned পূর্ণাঙ্ক)__builtin_ctzll(x)
    );
#নোচেদ্যদি defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ঘূৰাই (unsigned পূর্ণাঙ্ক)index;
#অন্যথা
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    স্থির ধ্রুৱ unsigned পূর্ণাঙ্ক debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ঘূৰাই debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক
clz32(uint32_t x)
{
    যদি (x == 0)
        ঘূৰাই 32;

#যদি defined(__GNUC__) || defined(__clang__)
    ঘূৰাই _Generic(
        x,
        unsigned পূর্ণাঙ্ক:
            (unsigned পূর্ণাঙ্ক)__builtin_clz(x) -
            (unsigned পূর্ণাঙ্ক)((পরিমাণ(unsigned পূর্ণাঙ্ক) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned পূর্ণাঙ্ক)__builtin_clzl(x) -
            (unsigned পূর্ণাঙ্ক)((পরিমাণ(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned পূর্ণাঙ্ক)__builtin_clzll(x) -
            (unsigned পূর্ণাঙ্ক)((পরিমাণ(unsigned long long) * CHAR_BIT) - 32)
    );
#অন্যথা
    // Efficient binary search fallback for 32-bit integers
    unsigned পূর্ণাঙ্ক গুনতি = 0;
    যদি ((x & 0xffff0000) == 0) { গুনতি += 16; x <<= 16; }
    যদি ((x & 0xff000000) == 0) { গুনতি += 8;  x <<= 8;  }
    যদি ((x & 0xf0000000) == 0) { গুনতি += 4;  x <<= 4;  }
    যদি ((x & 0xc0000000) == 0) { গুনতি += 2;  x <<= 2;  }
    যদি ((x & 0x80000000) == 0) { গুনতি += 1; }
    ঘূৰাই গুনতি;
#endif
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক
next_pow2(unsigned পূর্ণাঙ্ক x)
{
#যদি defined(__GNUC__) || defined(__clang__)
    যদি (x <= 1) ঘূৰাই 1u;
    ঘূৰাই 1u << ((পূর্ণাঙ্ক)পরিমাণ(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#অন্যথা
    যদি (x <= 1u) ঘূৰাই 1u;
    x--;
    বাবে (unsigned s = 1; s < (unsigned)পরিমাণ(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ঘূৰাই x + 1u;
#endif
}

স্থির অন্তঃস্থ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned পূর্ণাঙ্ক n) {
    ধ্রুৱ uint32_t original = mask;

    /* Clear the lowest set bit n times */
    যেতিয়ালৈ (n-- && mask)
        mask &= (mask - 1);

    ঘূৰাই original ^ mask;
}
