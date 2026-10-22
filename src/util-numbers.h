/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਜੇ defined(_MSC_VER)
# include <intrin.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/*
 * Define various parsers to avoid the use of strto* -- it’s slower and accepts
 * a bunch of stuff we don’t want to allow, like signs, spaces, even locale stuff.
 *
 * But the real feature is that it does not require a NULL-terminated string, so
 * it works safely *also* on any buffer, assuming the correct corresponding size
 * is provided. For NULL-terminated strings, just pass `SIZE_MAX` as the length:
 * the parsers will *always* stop on a NULL character.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAKE_PARSE_DEC_ਮੰਜ਼ਿਲ(type, max)                         \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ                                            \
parse_dec_ਮੰਜ਼ਿਲ##type(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t len, type (*ਬਾਹਰਲਾ))  \
{                                                            \
    type ਨਤੀਜਾ = 0;                                         \
    ਮਾਪ_t i;                                                \
    ਲਈ (i = 0;                                              \
         i < len && (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)(s[i] - '0') < 10U &&     \
         ਨਤੀਜਾ <= (max) / 10 &&                             \
         ਨਤੀਜਾ * 10 <= (max) - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (s[i] - '0');\
         i++) {                                              \
        ਨਤੀਜਾ = ਨਤੀਜਾ * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *ਬਾਹਰਲਾ = ਨਤੀਜਾ;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    ਵਾਪਸ (i >= len || (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)(s[i] - '0') >= 10U)  \
           ? (ਪੂਰਨ) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_ਮੰਜ਼ਿਲ(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_ਮੰਜ਼ਿਲ(uint64_t, UINT64_MAX)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MAKE_PARSE_DEC_ਮੰਜ਼ਿਲ

ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ digits__[] = {
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAKE_PARSE_HEX_ਮੰਜ਼ਿਲ(type, max)                           \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ                                              \
parse_hex_ਮੰਜ਼ਿਲ##type(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t len, type (*ਬਾਹਰਲਾ))    \
{                                                              \
    type ਨਤੀਜਾ = 0;                                           \
    ਮਾਪ_t i = 0;                                              \
    ਲਈ (; i < len && digits__[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) s[i]] < 16u &&  \
         ਨਤੀਜਾ <= (max) >> 4;                                 \
         i++) {                                                \
        ਨਤੀਜਾ = ਨਤੀਜਾ * 16 + digits__[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) s[i]]; \
    }                                                          \
    *ਬਾਹਰਲਾ = ਨਤੀਜਾ;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    ਵਾਪਸ (i >= len || !is_xdigit(s[i])) ? (ਪੂਰਨ) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_ਮੰਜ਼ਿਲ(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_ਮੰਜ਼ਿਲ(uint64_t, UINT64_MAX)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MAKE_PARSE_HEX_ਮੰਜ਼ਿਲ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
popcount32(uint32_t x)
{
#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_popcountl(x);
#ਜਾਂਜੇ defined(_MSC_VER)
    ਵਾਪਸ __popcnt(x);
#ਨਹੀਂਤਾਂ
    /* Fallback (Brian Kernighan’s method) */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਜਦੋਂ (x) {
        x &= x - 1;
        ਗਿਣਤੀ_2++;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
ctz32(uint32_t x)
{
    ਜੇ (x == 0)
        ਵਾਪਸ 32;

#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਵਾਪਸ _Generic(
        x,
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ: (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_ctz(x),
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ: (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_ctzl(x),
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ: (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_ctzll(x)
    );
#ਜਾਂਜੇ defined(_MSC_VER)
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸੂਚਕਾਂਕ;
    ਦੋਆਧਾਰੀ_ਅੰਕ_ਜਾਂਚ_Forward(&ਸੂਚਕਾਂਕ, x);
    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)ਸੂਚਕਾਂਕ;
#ਨਹੀਂਤਾਂ
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    ਵਾਪਸ debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
clz32(uint32_t x)
{
    ਜੇ (x == 0)
        ਵਾਪਸ 32;

#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਵਾਪਸ _Generic(
        x,
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ:
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_clz(x) -
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)((ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) * CHAR_BIT) - 32),
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ:
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_clzl(x) -
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)((ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) * CHAR_BIT) - 32),
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ:
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)__builtin_clzll(x) -
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)((ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) * CHAR_BIT) - 32)
    );
#ਨਹੀਂਤਾਂ
    // Efficient binary search fallback for 32-bit integers
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਜੇ ((x & 0xffff0000) == 0) { ਗਿਣਤੀ_2 += 16; x <<= 16; }
    ਜੇ ((x & 0xff000000) == 0) { ਗਿਣਤੀ_2 += 8;  x <<= 8;  }
    ਜੇ ((x & 0xf0000000) == 0) { ਗਿਣਤੀ_2 += 4;  x <<= 4;  }
    ਜੇ ((x & 0xc0000000) == 0) { ਗਿਣਤੀ_2 += 2;  x <<= 2;  }
    ਜੇ ((x & 0x80000000) == 0) { ਗਿਣਤੀ_2 += 1; }
    ਵਾਪਸ ਗਿਣਤੀ_2;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
ਅਗਲਾ_pow2(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x)
{
#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਜੇ (x <= 1) ਵਾਪਸ 1u;
    ਵਾਪਸ 1u << ((ਪੂਰਨ)ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ) * CHAR_BIT - __builtin_clz(x - 1));
#ਨਹੀਂਤਾਂ
    ਜੇ (x <= 1u) ਵਾਪਸ 1u;
    x--;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ s = 1; s < (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਕਾਰ(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    ਵਾਪਸ x + 1u;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
keep_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_n_ਸੈੱਟ_ਬਿੱਟ(uint32_t ਨਕਾਬ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n) {
    ਅਚਲ uint32_t original = ਨਕਾਬ;

    /* Clear the lowest set bit n times */
    ਜਦੋਂ (n-- && ਨਕਾਬ)
        ਨਕਾਬ &= (ਨਕਾਬ - 1);

    ਵਾਪਸ original ^ ਨਕਾਬ;
}
