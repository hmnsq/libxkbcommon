/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಆದರೆ defined(_MSC_VER)
# include <intrin.h>
#ಆದರೆ_ಅಂತ್ಯ

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

#ವ್ಯಾಖ್ಯಾನಿಸು MAKE_PARSE_DEC_ಗಮ್ಯ(type, max)                         \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ                                            \
parse_dec_ಗಮ್ಯ##type(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t len, type (*ಹೊರಹೋಗು))  \
{                                                            \
    type ಫಲಿತಾಂಶ = 0;                                         \
    ಗಾತ್ರ_t i;                                                \
    ಗಾಗಿ (i = 0;                                              \
         i < len && (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)(s[i] - '0') < 10U &&     \
         ಫಲಿತಾಂಶ <= (max) / 10 &&                             \
         ಫಲಿತಾಂಶ * 10 <= (max) - (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (s[i] - '0');\
         i++) {                                              \
        ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *ಹೊರಹೋಗು = ಫಲಿತಾಂಶ;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ಹಿಂತಿರುಗಿ (i >= len || (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)(s[i] - '0') >= 10U)  \
           ? (ಪೂರ್ಣಾಂಕ) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_ಗಮ್ಯ(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_ಗಮ್ಯ(uint64_t, UINT64_MAX)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MAKE_PARSE_DEC_ಗಮ್ಯ

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ digits__[] = {
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

#ವ್ಯಾಖ್ಯಾನಿಸು MAKE_PARSE_HEX_ಗಮ್ಯ(type, max)                           \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ                                              \
parse_hex_ಗಮ್ಯ##type(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t len, type (*ಹೊರಹೋಗು))    \
{                                                              \
    type ಫಲಿತಾಂಶ = 0;                                           \
    ಗಾತ್ರ_t i = 0;                                              \
    ಗಾಗಿ (; i < len && digits__[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) s[i]] < 16u &&  \
         ಫಲಿತಾಂಶ <= (max) >> 4;                                 \
         i++) {                                                \
        ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ * 16 + digits__[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) s[i]]; \
    }                                                          \
    *ಹೊರಹೋಗು = ಫಲಿತಾಂಶ;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ಹಿಂತಿರುಗಿ (i >= len || !is_xdigit(s[i])) ? (ಪೂರ್ಣಾಂಕ) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_ಗಮ್ಯ(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_ಗಮ್ಯ(uint64_t, UINT64_MAX)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MAKE_PARSE_HEX_ಗಮ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
popcount32(uint32_t x)
{
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_popcountl(x);
#ಅಥವಾಆದರೆ defined(_MSC_VER)
    ಹಿಂತಿರುಗಿ __popcnt(x);
#ಇಲ್ಲದಿದ್ದರೆ
    /* Fallback (Brian Kernighan’s method) */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಆಗ (x) {
        x &= x - 1;
        ಎಣಿಕೆ_2++;
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
ctz32(uint32_t x)
{
    ಆದರೆ (x == 0)
        ಹಿಂತಿರುಗಿ 32;

#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಹಿಂತಿರುಗಿ _Generic(
        x,
        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ: (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_ctz(x),
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ: (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_ctzl(x),
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ: (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_ctzll(x)
    );
#ಅಥವಾಆದರೆ defined(_MSC_VER)
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸೂಚ್ಯಂಕ;
    ಬಿಟ್_ಪರಿಶೀಲನೆ_Forward(&ಸೂಚ್ಯಂಕ, x);
    ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)ಸೂಚ್ಯಂಕ;
#ಇಲ್ಲದಿದ್ದರೆ
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ಹಿಂತಿರುಗಿ debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
clz32(uint32_t x)
{
    ಆದರೆ (x == 0)
        ಹಿಂತಿರುಗಿ 32;

#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಹಿಂತಿರುಗಿ _Generic(
        x,
        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ:
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_clz(x) -
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)((ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) * CHAR_BIT) - 32),
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ:
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_clzl(x) -
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)((ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) * CHAR_BIT) - 32),
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ:
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_clzll(x) -
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)((ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) * CHAR_BIT) - 32)
    );
#ಇಲ್ಲದಿದ್ದರೆ
    // Efficient binary search fallback for 32-bit integers
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಆದರೆ ((x & 0xffff0000) == 0) { ಎಣಿಕೆ_2 += 16; x <<= 16; }
    ಆದರೆ ((x & 0xff000000) == 0) { ಎಣಿಕೆ_2 += 8;  x <<= 8;  }
    ಆದರೆ ((x & 0xf0000000) == 0) { ಎಣಿಕೆ_2 += 4;  x <<= 4;  }
    ಆದರೆ ((x & 0xc0000000) == 0) { ಎಣಿಕೆ_2 += 2;  x <<= 2;  }
    ಆದರೆ ((x & 0x80000000) == 0) { ಎಣಿಕೆ_2 += 1; }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
ಮುಂದಿನ_pow2(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ x)
{
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಆದರೆ (x <= 1) ಹಿಂತಿರುಗಿ 1u;
    ಹಿಂತಿರುಗಿ 1u << ((ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ) * CHAR_BIT - __builtin_clz(x - 1));
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (x <= 1u) ಹಿಂತಿರುಗಿ 1u;
    x--;
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ s = 1; s < (ಚಿಹ್ನೆರಹಿತ)ಗಾತ್ರ(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ಹಿಂತಿರುಗಿ x + 1u;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t
keep_ಅತಿಕಡಿಮೆ_n_ಗಣ_ಬಿಟ್ಟುಗಳು(uint32_t ಮುಖವಾಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n) {
    ಸ್ಥಿರಾಂಕ uint32_t original = ಮುಖವಾಡ;

    /* Clear the lowest set bit n times */
    ಆಗ (n-- && ಮುಖವಾಡ)
        ಮುಖವಾಡ &= (ಮುಖವಾಡ - 1);

    ಹಿಂತಿರುಗಿ original ^ ಮುಖವಾಡ;
}
