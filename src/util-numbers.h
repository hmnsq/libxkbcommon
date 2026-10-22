/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>

#यदि defined(_MSC_VER)
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
parse_dec_to_##type(ध्रुव वर्ण *s, माप_प्रकार len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    माप_प्रकार i;                                                \
    प्रति (i = 0;                                              \
         i < len && (अचिन्हित वर्ण)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (अचिन्हित वर्ण) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    प्रत्यागम (i >= len || (अचिन्हित वर्ण)(s[i] - '0') >= 10U)  \
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

स्थिर ध्रुव अचिन्हित वर्ण digits__[] = {
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
parse_hex_to_##type(ध्रुव वर्ण *s, माप_प्रकार len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    माप_प्रकार i = 0;                                              \
    प्रति (; i < len && digits__[(अचिन्हित वर्ण) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(अचिन्हित वर्ण) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    प्रत्यागम (i >= len || !is_xdigit(s[i])) ? (पूर्णाङ्क) i : -1;      \
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

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
popcount32(uint32_t x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम (अचिन्हित पूर्णाङ्क)__builtin_popcountl(x);
#नोचेद्यदि defined(_MSC_VER)
    प्रत्यागम __popcnt(x);
#अन्यथा
    /* Fallback (Brian Kernighan’s method) */
    अचिन्हित पूर्णाङ्क गिनती = 0;
    यावत् (x) {
        x &= x - 1;
        गिनती++;
    }
    प्रत्यागम गिनती;
#endif
}

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
ctz32(uint32_t x)
{
    यदि (x == 0)
        प्रत्यागम 32;

#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम _Generic(
        x,
        अचिन्हित पूर्णाङ्क: (अचिन्हित पूर्णाङ्क)__builtin_ctz(x),
        अचिन्हित दीर्घ: (अचिन्हित पूर्णाङ्क)__builtin_ctzl(x),
        अचिन्हित दीर्घ दीर्घ: (अचिन्हित पूर्णाङ्क)__builtin_ctzll(x)
    );
#नोचेद्यदि defined(_MSC_VER)
    अचिन्हित दीर्घ index;
    _BitScanForward(&index, x);
    प्रत्यागम (अचिन्हित पूर्णाङ्क)index;
#अन्यथा
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    स्थिर ध्रुव अचिन्हित पूर्णाङ्क debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    प्रत्यागम debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
clz32(uint32_t x)
{
    यदि (x == 0)
        प्रत्यागम 32;

#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम _Generic(
        x,
        अचिन्हित पूर्णाङ्क:
            (अचिन्हित पूर्णाङ्क)__builtin_clz(x) -
            (अचिन्हित पूर्णाङ्क)((परिमाण(अचिन्हित पूर्णाङ्क) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ:
            (अचिन्हित पूर्णाङ्क)__builtin_clzl(x) -
            (अचिन्हित पूर्णाङ्क)((परिमाण(अचिन्हित दीर्घ) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ दीर्घ:
            (अचिन्हित पूर्णाङ्क)__builtin_clzll(x) -
            (अचिन्हित पूर्णाङ्क)((परिमाण(अचिन्हित दीर्घ दीर्घ) * अक्षर_बिट) - 32)
    );
#अन्यथा
    // Efficient binary search fallback for 32-bit integers
    अचिन्हित पूर्णाङ्क गिनती = 0;
    यदि ((x & 0xffff0000) == 0) { गिनती += 16; x <<= 16; }
    यदि ((x & 0xff000000) == 0) { गिनती += 8;  x <<= 8;  }
    यदि ((x & 0xf0000000) == 0) { गिनती += 4;  x <<= 4;  }
    यदि ((x & 0xc0000000) == 0) { गिनती += 2;  x <<= 2;  }
    यदि ((x & 0x80000000) == 0) { गिनती += 1; }
    प्रत्यागम गिनती;
#endif
}

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
next_pow2(अचिन्हित पूर्णाङ्क x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    यदि (x <= 1) प्रत्यागम 1u;
    प्रत्यागम 1u << ((पूर्णाङ्क)परिमाण(अचिन्हित) * अक्षर_बिट - __builtin_clz(x - 1));
#अन्यथा
    यदि (x <= 1u) प्रत्यागम 1u;
    x--;
    प्रति (अचिन्हित s = 1; s < (अचिन्हित)परिमाण(x)*अक्षर_बिट; s <<= 1)
        x |= x >> s;
    प्रत्यागम x + 1u;
#endif
}

स्थिर अन्तःस्थ uint32_t
keep_lowest_n_set_bits(uint32_t mask, अचिन्हित पूर्णाङ्क n) {
    ध्रुव uint32_t original = mask;

    /* Clear the lowest set bit n times */
    यावत् (n-- && mask)
        mask &= (mask - 1);

    प्रत्यागम original ^ mask;
}
