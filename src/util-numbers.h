/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಯದಿ defined(_MSC_VER)
# include <intrin.h>
#endif

#ಅನ್ತರ್ಭಾವ "utils.h"

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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ                                            \
parse_dec_to_##type(ಧ್ರುವ ವರ್ಣ *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    ಪ್ರತಿ (i = 0;                                              \
         i < len && (unsigned ವರ್ಣ)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned ವರ್ಣ) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ಪ್ರತ್ಯಾಗಮ (i >= len || (unsigned ವರ್ಣ)(s[i] - '0') >= 10U)  \
           ? (ಪೂರ್ಣಾಙ್ಕ) i                                         \
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

ಸ್ಥಿರ ಧ್ರುವ unsigned ವರ್ಣ digits__[] = {
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ                                              \
parse_hex_to_##type(ಧ್ರುವ ವರ್ಣ *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    ಪ್ರತಿ (; i < len && digits__[(unsigned ವರ್ಣ) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned ವರ್ಣ) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ಪ್ರತ್ಯಾಗಮ (i >= len || !is_xdigit(s[i])) ? (ಪೂರ್ಣಾಙ್ಕ) i : -1;      \
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
popcount32(uint32_t x)
{
#ಯದಿ defined(__GNUC__) || defined(__clang__)
    ಪ್ರತ್ಯಾಗಮ (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_popcountl(x);
#ನೋಚೇದ್ಯದಿ defined(_MSC_VER)
    ಪ್ರತ್ಯಾಗಮ __popcnt(x);
#ಅನ್ಯಥಾ
    /* Fallback (Brian Kernighan’s method) */
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಯಾವತ್ (x) {
        x &= x - 1;
        ಎಣಿಕೆ++;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
ctz32(uint32_t x)
{
    ಯದಿ (x == 0)
        ಪ್ರತ್ಯಾಗಮ 32;

#ಯದಿ defined(__GNUC__) || defined(__clang__)
    ಪ್ರತ್ಯಾಗಮ _Generic(
        x,
        unsigned ಪೂರ್ಣಾಙ್ಕ: (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_ctz(x),
        unsigned long: (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_ctzl(x),
        unsigned long long: (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_ctzll(x)
    );
#ನೋಚೇದ್ಯದಿ defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    ಪ್ರತ್ಯಾಗಮ (unsigned ಪೂರ್ಣಾಙ್ಕ)index;
#ಅನ್ಯಥಾ
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ಸ್ಥಿರ ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ಪ್ರತ್ಯಾಗಮ debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
clz32(uint32_t x)
{
    ಯದಿ (x == 0)
        ಪ್ರತ್ಯಾಗಮ 32;

#ಯದಿ defined(__GNUC__) || defined(__clang__)
    ಪ್ರತ್ಯಾಗಮ _Generic(
        x,
        unsigned ಪೂರ್ಣಾಙ್ಕ:
            (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_clz(x) -
            (unsigned ಪೂರ್ಣಾಙ್ಕ)((ಪರಿಮಾಣ(unsigned ಪೂರ್ಣಾಙ್ಕ) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_clzl(x) -
            (unsigned ಪೂರ್ಣಾಙ್ಕ)((ಪರಿಮಾಣ(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned ಪೂರ್ಣಾಙ್ಕ)__builtin_clzll(x) -
            (unsigned ಪೂರ್ಣಾಙ್ಕ)((ಪರಿಮಾಣ(unsigned long long) * CHAR_BIT) - 32)
    );
#ಅನ್ಯಥಾ
    // Efficient binary search fallback for 32-bit integers
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಯದಿ ((x & 0xffff0000) == 0) { ಎಣಿಕೆ += 16; x <<= 16; }
    ಯದಿ ((x & 0xff000000) == 0) { ಎಣಿಕೆ += 8;  x <<= 8;  }
    ಯದಿ ((x & 0xf0000000) == 0) { ಎಣಿಕೆ += 4;  x <<= 4;  }
    ಯದಿ ((x & 0xc0000000) == 0) { ಎಣಿಕೆ += 2;  x <<= 2;  }
    ಯದಿ ((x & 0x80000000) == 0) { ಎಣಿಕೆ += 1; }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
next_pow2(unsigned ಪೂರ್ಣಾಙ್ಕ x)
{
#ಯದಿ defined(__GNUC__) || defined(__clang__)
    ಯದಿ (x <= 1) ಪ್ರತ್ಯಾಗಮ 1u;
    ಪ್ರತ್ಯಾಗಮ 1u << ((ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#ಅನ್ಯಥಾ
    ಯದಿ (x <= 1u) ಪ್ರತ್ಯಾಗಮ 1u;
    x--;
    ಪ್ರತಿ (unsigned s = 1; s < (unsigned)ಪರಿಮಾಣ(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ಪ್ರತ್ಯಾಗಮ x + 1u;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned ಪೂರ್ಣಾಙ್ಕ n) {
    ಧ್ರುವ uint32_t original = mask;

    /* Clear the lowest set bit n times */
    ಯಾವತ್ (n-- && mask)
        mask &= (mask - 1);

    ಪ್ರತ್ಯಾಗಮ original ^ mask;
}
