/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>

#إذا defined(_MSC_VER)
# include <intrin.h>
#endif

#ضمن "utils.h"

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
ساكن مضمن صحيح                                            \
parse_dec_to_##type(ثابت حرف *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    لكل (i = 0;                                              \
         i < len && (unsigned حرف)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned حرف) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    أرجع (i >= len || (unsigned حرف)(s[i] - '0') >= 10U)  \
           ? (صحيح) i                                         \
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

ساكن ثابت unsigned حرف digits__[] = {
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
ساكن مضمن صحيح                                              \
parse_hex_to_##type(ثابت حرف *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    لكل (; i < len && digits__[(unsigned حرف) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned حرف) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    أرجع (i >= len || !is_xdigit(s[i])) ? (صحيح) i : -1;      \
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

ساكن مضمن unsigned صحيح
popcount32(uint32_t x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (unsigned صحيح)__builtin_popcountl(x);
#وإلاإذا defined(_MSC_VER)
    أرجع __popcnt(x);
#وإلا
    /* Fallback (Brian Kernighan’s method) */
    unsigned صحيح كدي = 0;
    طالما (x) {
        x &= x - 1;
        كدي++;
    }
    أرجع كدي;
#endif
}

ساكن مضمن unsigned صحيح
ctz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        unsigned صحيح: (unsigned صحيح)__builtin_ctz(x),
        unsigned long: (unsigned صحيح)__builtin_ctzl(x),
        unsigned long long: (unsigned صحيح)__builtin_ctzll(x)
    );
#وإلاإذا defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    أرجع (unsigned صحيح)index;
#وإلا
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ساكن ثابت unsigned صحيح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    أرجع debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

ساكن مضمن unsigned صحيح
clz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        unsigned صحيح:
            (unsigned صحيح)__builtin_clz(x) -
            (unsigned صحيح)((حجم(unsigned صحيح) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned صحيح)__builtin_clzl(x) -
            (unsigned صحيح)((حجم(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned صحيح)__builtin_clzll(x) -
            (unsigned صحيح)((حجم(unsigned long long) * CHAR_BIT) - 32)
    );
#وإلا
    // Efficient binary search fallback for 32-bit integers
    unsigned صحيح كدي = 0;
    إذا ((x & 0xffff0000) == 0) { كدي += 16; x <<= 16; }
    إذا ((x & 0xff000000) == 0) { كدي += 8;  x <<= 8;  }
    إذا ((x & 0xf0000000) == 0) { كدي += 4;  x <<= 4;  }
    إذا ((x & 0xc0000000) == 0) { كدي += 2;  x <<= 2;  }
    إذا ((x & 0x80000000) == 0) { كدي += 1; }
    أرجع كدي;
#endif
}

ساكن مضمن unsigned صحيح
next_pow2(unsigned صحيح x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    إذا (x <= 1) أرجع 1u;
    أرجع 1u << ((صحيح)حجم(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#وإلا
    إذا (x <= 1u) أرجع 1u;
    x--;
    لكل (unsigned s = 1; s < (unsigned)حجم(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    أرجع x + 1u;
#endif
}

ساكن مضمن uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned صحيح n) {
    ثابت uint32_t original = mask;

    /* Clear the lowest set bit n times */
    طالما (n-- && mask)
        mask &= (mask - 1);

    أرجع original ^ mask;
}
