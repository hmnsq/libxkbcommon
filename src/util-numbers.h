/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>

#यदि defined(_MSC_VER)
# include <intrin.h>
#यदि_अन्तः

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

#परिभाषय MAKE_PARSE_DEC_गन्तव्यम्(type, max)                         \
स्थिर अन्तःस्थ पूर्णाङ्क                                            \
parse_dec_गन्तव्यम्##type(ध्रुव वर्ण *s, आकार_t len, type (*बहिर्गमनम्))  \
{                                                            \
    type परिणाम = 0;                                         \
    आकार_t i;                                                \
    प्रति (i = 0;                                              \
         i < len && (अचिह्नित वर्ण)(s[i] - '0') < 10U &&     \
         परिणाम <= (max) / 10 &&                             \
         परिणाम * 10 <= (max) - (अचिह्नित वर्ण) (s[i] - '0');\
         i++) {                                              \
        परिणाम = परिणाम * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *बहिर्गमनम् = परिणाम;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    प्रत्यागम (i >= len || (अचिह्नित वर्ण)(s[i] - '0') >= 10U)  \
           ? (पूर्णाङ्क) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_गन्तव्यम्(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_गन्तव्यम्(uint64_t, UINT64_MAX)

#अपरिभाषय MAKE_PARSE_DEC_गन्तव्यम्

स्थिर ध्रुव अचिह्नित वर्ण digits__[] = {
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

#परिभाषय MAKE_PARSE_HEX_गन्तव्यम्(type, max)                           \
स्थिर अन्तःस्थ पूर्णाङ्क                                              \
parse_hex_गन्तव्यम्##type(ध्रुव वर्ण *s, आकार_t len, type (*बहिर्गमनम्))    \
{                                                              \
    type परिणाम = 0;                                           \
    आकार_t i = 0;                                              \
    प्रति (; i < len && digits__[(अचिह्नित वर्ण) s[i]] < 16u &&  \
         परिणाम <= (max) >> 4;                                 \
         i++) {                                                \
        परिणाम = परिणाम * 16 + digits__[(अचिह्नित वर्ण) s[i]]; \
    }                                                          \
    *बहिर्गमनम् = परिणाम;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    प्रत्यागम (i >= len || !is_xdigit(s[i])) ? (पूर्णाङ्क) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_गन्तव्यम्(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_गन्तव्यम्(uint64_t, UINT64_MAX)

#अपरिभाषय MAKE_PARSE_HEX_गन्तव्यम्

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क
popcount32(uint32_t x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम (अचिह्नित पूर्णाङ्क)__builtin_popcountl(x);
#नोचेद्यदि defined(_MSC_VER)
    प्रत्यागम __popcnt(x);
#अन्यथा
    /* Fallback (Brian Kernighan’s method) */
    अचिह्नित पूर्णाङ्क गणनम् = 0;
    यावत् (x) {
        x &= x - 1;
        गणनम्++;
    }
    प्रत्यागम गणनम्;
#यदि_अन्तः
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क
ctz32(uint32_t x)
{
    यदि (x == 0)
        प्रत्यागम 32;

#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम _Generic(
        x,
        अचिह्नित पूर्णाङ्क: (अचिह्नित पूर्णाङ्क)__builtin_ctz(x),
        अचिह्नित दीर्घ: (अचिह्नित पूर्णाङ्क)__builtin_ctzl(x),
        अचिह्नित दीर्घ दीर्घ: (अचिह्नित पूर्णाङ्क)__builtin_ctzll(x)
    );
#नोचेद्यदि defined(_MSC_VER)
    अचिह्नित दीर्घ सूचकांक;
    Bit_अवलोकनम्_Forward(&सूचकांक, x);
    प्रत्यागम (अचिह्नित पूर्णाङ्क)सूचकांक;
#अन्यथा
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    स्थिर ध्रुव अचिह्नित पूर्णाङ्क debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    प्रत्यागम debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#यदि_अन्तः
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क
clz32(uint32_t x)
{
    यदि (x == 0)
        प्रत्यागम 32;

#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम _Generic(
        x,
        अचिह्नित पूर्णाङ्क:
            (अचिह्नित पूर्णाङ्क)__builtin_clz(x) -
            (अचिह्नित पूर्णाङ्क)((परिमाण(अचिह्नित पूर्णाङ्क) * CHAR_BIT) - 32),
        अचिह्नित दीर्घ:
            (अचिह्नित पूर्णाङ्क)__builtin_clzl(x) -
            (अचिह्नित पूर्णाङ्क)((परिमाण(अचिह्नित दीर्घ) * CHAR_BIT) - 32),
        अचिह्नित दीर्घ दीर्घ:
            (अचिह्नित पूर्णाङ्क)__builtin_clzll(x) -
            (अचिह्नित पूर्णाङ्क)((परिमाण(अचिह्नित दीर्घ दीर्घ) * CHAR_BIT) - 32)
    );
#अन्यथा
    // Efficient binary search fallback for 32-bit integers
    अचिह्नित पूर्णाङ्क गणनम् = 0;
    यदि ((x & 0xffff0000) == 0) { गणनम् += 16; x <<= 16; }
    यदि ((x & 0xff000000) == 0) { गणनम् += 8;  x <<= 8;  }
    यदि ((x & 0xf0000000) == 0) { गणनम् += 4;  x <<= 4;  }
    यदि ((x & 0xc0000000) == 0) { गणनम् += 2;  x <<= 2;  }
    यदि ((x & 0x80000000) == 0) { गणनम् += 1; }
    प्रत्यागम गणनम्;
#यदि_अन्तः
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क
अग्र_pow2(अचिह्नित पूर्णाङ्क x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    यदि (x <= 1) प्रत्यागम 1u;
    प्रत्यागम 1u << ((पूर्णाङ्क)परिमाण(अचिह्नित) * CHAR_BIT - __builtin_clz(x - 1));
#अन्यथा
    यदि (x <= 1u) प्रत्यागम 1u;
    x--;
    प्रति (अचिह्नित s = 1; s < (अचिह्नित)परिमाण(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    प्रत्यागम x + 1u;
#यदि_अन्तः
}

स्थिर अन्तःस्थ uint32_t
keep_निम्नतमम्_n_समुच्चयः_द्वयङ्काः(uint32_t आवरणम्, अचिह्नित पूर्णाङ्क n) {
    ध्रुव uint32_t original = आवरणम्;

    /* Clear the lowest set bit n times */
    यावत् (n-- && आवरणम्)
        आवरणम् &= (आवरणम् - 1);

    प्रत्यागम original ^ आवरणम्;
}
