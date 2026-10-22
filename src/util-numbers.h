/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdint.h>

#എങ്കിൽ defined(_MSC_VER)
# include <intrin.h>
#endif

#അന്തര്ഭാവ "utils.h"

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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ                                            \
parse_dec_to_##type(സ്ഥിരം അക്ഷരം *s, size_t len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    size_t i;                                                \
    വേണ്ടി (i = 0;                                              \
         i < len && (unsigned അക്ഷരം)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (unsigned അക്ഷരം) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    തിരികെ (i >= len || (unsigned അക്ഷരം)(s[i] - '0') >= 10U)  \
           ? (പൂർണ്ണസംഖ്യ) i                                         \
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

സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം digits__[] = {
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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ                                              \
parse_hex_to_##type(സ്ഥിരം അക്ഷരം *s, size_t len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    size_t i = 0;                                              \
    വേണ്ടി (; i < len && digits__[(unsigned അക്ഷരം) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(unsigned അക്ഷരം) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    തിരികെ (i >= len || !is_xdigit(s[i])) ? (പൂർണ്ണസംഖ്യ) i : -1;      \
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

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
popcount32(uint32_t x)
{
#എങ്കിൽ defined(__GNUC__) || defined(__clang__)
    തിരികെ (unsigned പൂർണ്ണസംഖ്യ)__builtin_popcountl(x);
#അല്ലെങ്കിലെങ്കിൽ defined(_MSC_VER)
    തിരികെ __popcnt(x);
#അല്ലെങ്കിൽ
    /* Fallback (Brian Kernighan’s method) */
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    ആകുമ്പോൾ (x) {
        x &= x - 1;
        എണ്ണം++;
    }
    തിരികെ എണ്ണം;
#endif
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
ctz32(uint32_t x)
{
    എങ്കിൽ (x == 0)
        തിരികെ 32;

#എങ്കിൽ defined(__GNUC__) || defined(__clang__)
    തിരികെ _Generic(
        x,
        unsigned പൂർണ്ണസംഖ്യ: (unsigned പൂർണ്ണസംഖ്യ)__builtin_ctz(x),
        unsigned long: (unsigned പൂർണ്ണസംഖ്യ)__builtin_ctzl(x),
        unsigned long long: (unsigned പൂർണ്ണസംഖ്യ)__builtin_ctzll(x)
    );
#അല്ലെങ്കിലെങ്കിൽ defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, x);
    തിരികെ (unsigned പൂർണ്ണസംഖ്യ)index;
#അല്ലെങ്കിൽ
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    സ്ഥിതം സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    തിരികെ debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
clz32(uint32_t x)
{
    എങ്കിൽ (x == 0)
        തിരികെ 32;

#എങ്കിൽ defined(__GNUC__) || defined(__clang__)
    തിരികെ _Generic(
        x,
        unsigned പൂർണ്ണസംഖ്യ:
            (unsigned പൂർണ്ണസംഖ്യ)__builtin_clz(x) -
            (unsigned പൂർണ്ണസംഖ്യ)((വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ) * CHAR_BIT) - 32),
        unsigned long:
            (unsigned പൂർണ്ണസംഖ്യ)__builtin_clzl(x) -
            (unsigned പൂർണ്ണസംഖ്യ)((വലുപ്പം(unsigned long) * CHAR_BIT) - 32),
        unsigned long long:
            (unsigned പൂർണ്ണസംഖ്യ)__builtin_clzll(x) -
            (unsigned പൂർണ്ണസംഖ്യ)((വലുപ്പം(unsigned long long) * CHAR_BIT) - 32)
    );
#അല്ലെങ്കിൽ
    // Efficient binary search fallback for 32-bit integers
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    എങ്കിൽ ((x & 0xffff0000) == 0) { എണ്ണം += 16; x <<= 16; }
    എങ്കിൽ ((x & 0xff000000) == 0) { എണ്ണം += 8;  x <<= 8;  }
    എങ്കിൽ ((x & 0xf0000000) == 0) { എണ്ണം += 4;  x <<= 4;  }
    എങ്കിൽ ((x & 0xc0000000) == 0) { എണ്ണം += 2;  x <<= 2;  }
    എങ്കിൽ ((x & 0x80000000) == 0) { എണ്ണം += 1; }
    തിരികെ എണ്ണം;
#endif
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
next_pow2(unsigned പൂർണ്ണസംഖ്യ x)
{
#എങ്കിൽ defined(__GNUC__) || defined(__clang__)
    എങ്കിൽ (x <= 1) തിരികെ 1u;
    തിരികെ 1u << ((പൂർണ്ണസംഖ്യ)വലുപ്പം(unsigned) * CHAR_BIT - __builtin_clz(x - 1));
#അല്ലെങ്കിൽ
    എങ്കിൽ (x <= 1u) തിരികെ 1u;
    x--;
    വേണ്ടി (unsigned s = 1; s < (unsigned)വലുപ്പം(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    തിരികെ x + 1u;
#endif
}

സ്ഥിതം വരിയിൽ uint32_t
keep_lowest_n_set_bits(uint32_t mask, unsigned പൂർണ്ണസംഖ്യ n) {
    സ്ഥിരം uint32_t original = mask;

    /* Clear the lowest set bit n times */
    ആകുമ്പോൾ (n-- && mask)
        mask &= (mask - 1);

    തിരികെ original ^ mask;
}
