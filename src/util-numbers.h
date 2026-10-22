/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <limits.h>
#समावेश <stdbool.h>
#समावेश <stdint.h>

#अगर defined(_MSC_VER)
# include <intrin.h>
#पूर्ण_अगर

#समावेश "utils.h"

/*
 * Define various parsers to avoid the use of strto* -- it’s slower and accepts
 * a bunch of stuff we don’t want to allow, like signs, spaces, even locale stuff.
 *
 * But the real feature is that it does not require a NULL-terminated string, so
 * it works safely *also* on any buffer, assuming the correct corresponding size
 * is provided. For NULL-terminated strings, just pass `SIZE_MAX` as the length:
 * the parsers will *always* stop on a NULL character.
 */

#घोषणा MAKE_PARSE_DEC_TO(type, max)                         \
अटल अंतरभूत पूर्णांक                                            \
parse_dec_to_##type(स्थिर अक्षर *s, माप_प्रकार len, type (*out))  \
{                                                            \
    type परिणाम = 0;                                         \
    माप_प्रकार i;                                                \
    क्रम (i = 0;                                              \
         i < len && (अचिन्हित अक्षर)(s[i] - '0') < 10U &&     \
         परिणाम <= (max) / 10 &&                             \
         परिणाम * 10 <= (max) - (अचिन्हित अक्षर) (s[i] - '0');\
         i++) {                                              \
        परिणाम = परिणाम * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = परिणाम;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    वापस (i >= len || (अचिन्हित अक्षर)(s[i] - '0') >= 10U)  \
           ? (पूर्णांक) i                                         \
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

#अघोषित MAKE_PARSE_DEC_TO

अटल स्थिर अचिन्हित अक्षर digits__[] = {
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

#घोषणा MAKE_PARSE_HEX_TO(type, max)                           \
अटल अंतरभूत पूर्णांक                                              \
parse_hex_to_##type(स्थिर अक्षर *s, माप_प्रकार len, type (*out))    \
{                                                              \
    type परिणाम = 0;                                           \
    माप_प्रकार i = 0;                                              \
    क्रम (; i < len && digits__[(अचिन्हित अक्षर) s[i]] < 16u &&  \
         परिणाम <= (max) >> 4;                                 \
         i++) {                                                \
        परिणाम = परिणाम * 16 + digits__[(अचिन्हित अक्षर) s[i]]; \
    }                                                          \
    *out = परिणाम;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    वापस (i >= len || !is_xdigit(s[i])) ? (पूर्णांक) i : -1;      \
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

#अघोषित MAKE_PARSE_HEX_TO

अटल अंतरभूत अचिन्हित पूर्णांक
popcount32(uint32_t x)
{
#अगर defined(__GNUC__) || defined(__clang__)
    वापस (अचिन्हित पूर्णांक)__builtin_popcountl(x);
#या_अगर defined(_MSC_VER)
    वापस __popcnt(x);
#अन्यथा
    /* Fallback (Brian Kernighan’s method) */
    अचिन्हित पूर्णांक गिनती = 0;
    जबतक (x) {
        x &= x - 1;
        गिनती++;
    }
    वापस गिनती;
#पूर्ण_अगर
}

अटल अंतरभूत अचिन्हित पूर्णांक
ctz32(uint32_t x)
{
    अगर (x == 0)
        वापस 32;

#अगर defined(__GNUC__) || defined(__clang__)
    वापस _Generic(
        x,
        अचिन्हित पूर्णांक: (अचिन्हित पूर्णांक)__builtin_ctz(x),
        अचिन्हित दीर्घ: (अचिन्हित पूर्णांक)__builtin_ctzl(x),
        अचिन्हित दीर्घ दीर्घ: (अचिन्हित पूर्णांक)__builtin_ctzll(x)
    );
#या_अगर defined(_MSC_VER)
    अचिन्हित दीर्घ सूचकांक;
    _BitScanForward(&सूचकांक, x);
    वापस (अचिन्हित पूर्णांक)सूचकांक;
#अन्यथा
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    अटल स्थिर अचिन्हित पूर्णांक debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    वापस debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#पूर्ण_अगर
}

अटल अंतरभूत अचिन्हित पूर्णांक
clz32(uint32_t x)
{
    अगर (x == 0)
        वापस 32;

#अगर defined(__GNUC__) || defined(__clang__)
    वापस _Generic(
        x,
        अचिन्हित पूर्णांक:
            (अचिन्हित पूर्णांक)__builtin_clz(x) -
            (अचिन्हित पूर्णांक)((माप(अचिन्हित पूर्णांक) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ:
            (अचिन्हित पूर्णांक)__builtin_clzl(x) -
            (अचिन्हित पूर्णांक)((माप(अचिन्हित दीर्घ) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ दीर्घ:
            (अचिन्हित पूर्णांक)__builtin_clzll(x) -
            (अचिन्हित पूर्णांक)((माप(अचिन्हित दीर्घ दीर्घ) * अक्षर_बिट) - 32)
    );
#अन्यथा
    // Efficient binary search fallback for 32-bit integers
    अचिन्हित पूर्णांक गिनती = 0;
    अगर ((x & 0xffff0000) == 0) { गिनती += 16; x <<= 16; }
    अगर ((x & 0xff000000) == 0) { गिनती += 8;  x <<= 8;  }
    अगर ((x & 0xf0000000) == 0) { गिनती += 4;  x <<= 4;  }
    अगर ((x & 0xc0000000) == 0) { गिनती += 2;  x <<= 2;  }
    अगर ((x & 0x80000000) == 0) { गिनती += 1; }
    वापस गिनती;
#पूर्ण_अगर
}

अटल अंतरभूत अचिन्हित पूर्णांक
अग्र_pow2(अचिन्हित पूर्णांक x)
{
#अगर defined(__GNUC__) || defined(__clang__)
    अगर (x <= 1) वापस 1u;
    वापस 1u << ((पूर्णांक)माप(अचिन्हित) * अक्षर_बिट - __builtin_clz(x - 1));
#अन्यथा
    अगर (x <= 1u) वापस 1u;
    x--;
    क्रम (अचिन्हित s = 1; s < (अचिन्हित)माप(x)*अक्षर_बिट; s <<= 1)
        x |= x >> s;
    वापस x + 1u;
#पूर्ण_अगर
}

अटल अंतरभूत uint32_t
keep_lowest_n_set_bits(uint32_t mask, अचिन्हित पूर्णांक n) {
    स्थिर uint32_t original = mask;

    /* Clear the lowest set bit n times */
    जबतक (n-- && mask)
        mask &= (mask - 1);

    वापस original ^ mask;
}
