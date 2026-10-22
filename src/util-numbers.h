/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>

#जर defined(_MSC_VER)
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
स्थिर ओळीत पूर्णांक                                            \
parse_dec_to_##type(स्थिरांक अक्षर *s, माप_प्रकार len, type (*out))  \
{                                                            \
    type result = 0;                                         \
    माप_प्रकार i;                                                \
    साठी (i = 0;                                              \
         i < len && (अचिन्हित अक्षर)(s[i] - '0') < 10U &&     \
         result <= (max) / 10 &&                             \
         result * 10 <= (max) - (अचिन्हित अक्षर) (s[i] - '0');\
         i++) {                                              \
        result = result * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *out = result;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    परत (i >= len || (अचिन्हित अक्षर)(s[i] - '0') >= 10U)  \
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

#undef MAKE_PARSE_DEC_TO

स्थिर स्थिरांक अचिन्हित अक्षर digits__[] = {
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
स्थिर ओळीत पूर्णांक                                              \
parse_hex_to_##type(स्थिरांक अक्षर *s, माप_प्रकार len, type (*out))    \
{                                                              \
    type result = 0;                                           \
    माप_प्रकार i = 0;                                              \
    साठी (; i < len && digits__[(अचिन्हित अक्षर) s[i]] < 16u &&  \
         result <= (max) >> 4;                                 \
         i++) {                                                \
        result = result * 16 + digits__[(अचिन्हित अक्षर) s[i]]; \
    }                                                          \
    *out = result;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    परत (i >= len || !is_xdigit(s[i])) ? (पूर्णांक) i : -1;      \
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

स्थिर ओळीत अचिन्हित पूर्णांक
popcount32(uint32_t x)
{
#जर defined(__GNUC__) || defined(__clang__)
    परत (अचिन्हित पूर्णांक)__builtin_popcountl(x);
#किंवाजर defined(_MSC_VER)
    परत __popcnt(x);
#अन्यथा
    /* Fallback (Brian Kernighan’s method) */
    अचिन्हित पूर्णांक मोजणी = 0;
    पर्यंत (x) {
        x &= x - 1;
        मोजणी++;
    }
    परत मोजणी;
#endif
}

स्थिर ओळीत अचिन्हित पूर्णांक
ctz32(uint32_t x)
{
    जर (x == 0)
        परत 32;

#जर defined(__GNUC__) || defined(__clang__)
    परत _Generic(
        x,
        अचिन्हित पूर्णांक: (अचिन्हित पूर्णांक)__builtin_ctz(x),
        अचिन्हित दीर्घ: (अचिन्हित पूर्णांक)__builtin_ctzl(x),
        अचिन्हित दीर्घ दीर्घ: (अचिन्हित पूर्णांक)__builtin_ctzll(x)
    );
#किंवाजर defined(_MSC_VER)
    अचिन्हित दीर्घ index;
    _BitScanForward(&index, x);
    परत (अचिन्हित पूर्णांक)index;
#अन्यथा
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    स्थिर स्थिरांक अचिन्हित पूर्णांक debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    परत debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#endif
}

स्थिर ओळीत अचिन्हित पूर्णांक
clz32(uint32_t x)
{
    जर (x == 0)
        परत 32;

#जर defined(__GNUC__) || defined(__clang__)
    परत _Generic(
        x,
        अचिन्हित पूर्णांक:
            (अचिन्हित पूर्णांक)__builtin_clz(x) -
            (अचिन्हित पूर्णांक)((आकार(अचिन्हित पूर्णांक) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ:
            (अचिन्हित पूर्णांक)__builtin_clzl(x) -
            (अचिन्हित पूर्णांक)((आकार(अचिन्हित दीर्घ) * अक्षर_बिट) - 32),
        अचिन्हित दीर्घ दीर्घ:
            (अचिन्हित पूर्णांक)__builtin_clzll(x) -
            (अचिन्हित पूर्णांक)((आकार(अचिन्हित दीर्घ दीर्घ) * अक्षर_बिट) - 32)
    );
#अन्यथा
    // Efficient binary search fallback for 32-bit integers
    अचिन्हित पूर्णांक मोजणी = 0;
    जर ((x & 0xffff0000) == 0) { मोजणी += 16; x <<= 16; }
    जर ((x & 0xff000000) == 0) { मोजणी += 8;  x <<= 8;  }
    जर ((x & 0xf0000000) == 0) { मोजणी += 4;  x <<= 4;  }
    जर ((x & 0xc0000000) == 0) { मोजणी += 2;  x <<= 2;  }
    जर ((x & 0x80000000) == 0) { मोजणी += 1; }
    परत मोजणी;
#endif
}

स्थिर ओळीत अचिन्हित पूर्णांक
next_pow2(अचिन्हित पूर्णांक x)
{
#जर defined(__GNUC__) || defined(__clang__)
    जर (x <= 1) परत 1u;
    परत 1u << ((पूर्णांक)आकार(अचिन्हित) * अक्षर_बिट - __builtin_clz(x - 1));
#अन्यथा
    जर (x <= 1u) परत 1u;
    x--;
    साठी (अचिन्हित s = 1; s < (अचिन्हित)आकार(x)*अक्षर_बिट; s <<= 1)
        x |= x >> s;
    परत x + 1u;
#endif
}

स्थिर ओळीत uint32_t
keep_lowest_n_set_bits(uint32_t mask, अचिन्हित पूर्णांक n) {
    स्थिरांक uint32_t original = mask;

    /* Clear the lowest set bit n times */
    पर्यंत (n-- && mask)
        mask &= (mask - 1);

    परत original ^ mask;
}
