/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>

#إذا defined(_MSC_VER)
# include <intrin.h>
#د_که_پای

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

#تعریف_کړه MAKE_PARSE_DEC_منزل(type, max)                         \
ساكن مضمن صحيح                                            \
parse_dec_منزل##type(ثابت حرف *s, اندازه_t len, type (*بهر))  \
{                                                            \
    type پایله = 0;                                         \
    اندازه_t i;                                                \
    لكل (i = 0;                                              \
         i < len && (بینښه حرف)(s[i] - '0') < 10U &&     \
         پایله <= (max) / 10 &&                             \
         پایله * 10 <= (max) - (بینښه حرف) (s[i] - '0');\
         i++) {                                              \
        پایله = پایله * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *بهر = پایله;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    أرجع (i >= len || (بینښه حرف)(s[i] - '0') >= 10U)  \
           ? (صحيح) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_منزل(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_منزل(uint64_t, UINT64_MAX)

#تعریف_لغوه MAKE_PARSE_DEC_منزل

ساكن ثابت بینښه حرف digits__[] = {
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

#تعریف_کړه MAKE_PARSE_HEX_منزل(type, max)                           \
ساكن مضمن صحيح                                              \
parse_hex_منزل##type(ثابت حرف *s, اندازه_t len, type (*بهر))    \
{                                                              \
    type پایله = 0;                                           \
    اندازه_t i = 0;                                              \
    لكل (; i < len && digits__[(بینښه حرف) s[i]] < 16u &&  \
         پایله <= (max) >> 4;                                 \
         i++) {                                                \
        پایله = پایله * 16 + digits__[(بینښه حرف) s[i]]; \
    }                                                          \
    *بهر = پایله;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    أرجع (i >= len || !is_xdigit(s[i])) ? (صحيح) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_منزل(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_منزل(uint64_t, UINT64_MAX)

#تعریف_لغوه MAKE_PARSE_HEX_منزل

ساكن مضمن بینښه صحيح
popcount32(uint32_t x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (بینښه صحيح)__builtin_popcountl(x);
#وإلاإذا defined(_MSC_VER)
    أرجع __popcnt(x);
#وإلا
    /* Fallback (Brian Kernighan’s method) */
    بینښه صحيح شمېر = 0;
    طالما (x) {
        x &= x - 1;
        شمېر++;
    }
    أرجع شمېر;
#د_که_پای
}

ساكن مضمن بینښه صحيح
ctz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بینښه صحيح: (بینښه صحيح)__builtin_ctz(x),
        بینښه اوږد: (بینښه صحيح)__builtin_ctzl(x),
        بینښه اوږد اوږد: (بینښه صحيح)__builtin_ctzll(x)
    );
#وإلاإذا defined(_MSC_VER)
    بینښه اوږد شاخص;
    بټ_لټون_Forward(&شاخص, x);
    أرجع (بینښه صحيح)شاخص;
#وإلا
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ساكن ثابت بینښه صحيح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    أرجع debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#د_که_پای
}

ساكن مضمن بینښه صحيح
clz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بینښه صحيح:
            (بینښه صحيح)__builtin_clz(x) -
            (بینښه صحيح)((حجم(بینښه صحيح) * CHAR_BIT) - 32),
        بینښه اوږد:
            (بینښه صحيح)__builtin_clzl(x) -
            (بینښه صحيح)((حجم(بینښه اوږد) * CHAR_BIT) - 32),
        بینښه اوږد اوږد:
            (بینښه صحيح)__builtin_clzll(x) -
            (بینښه صحيح)((حجم(بینښه اوږد اوږد) * CHAR_BIT) - 32)
    );
#وإلا
    // Efficient binary search fallback for 32-bit integers
    بینښه صحيح شمېر = 0;
    إذا ((x & 0xffff0000) == 0) { شمېر += 16; x <<= 16; }
    إذا ((x & 0xff000000) == 0) { شمېر += 8;  x <<= 8;  }
    إذا ((x & 0xf0000000) == 0) { شمېر += 4;  x <<= 4;  }
    إذا ((x & 0xc0000000) == 0) { شمېر += 2;  x <<= 2;  }
    إذا ((x & 0x80000000) == 0) { شمېر += 1; }
    أرجع شمېر;
#د_که_پای
}

ساكن مضمن بینښه صحيح
بل_pow2(بینښه صحيح x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    إذا (x <= 1) أرجع 1u;
    أرجع 1u << ((صحيح)حجم(بینښه) * CHAR_BIT - __builtin_clz(x - 1));
#وإلا
    إذا (x <= 1u) أرجع 1u;
    x--;
    لكل (بینښه s = 1; s < (بینښه)حجم(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    أرجع x + 1u;
#د_که_پای
}

ساكن مضمن uint32_t
keep_ټیټ_ترین_n_سېټ_بټونه(uint32_t ماسک, بینښه صحيح n) {
    ثابت uint32_t original = ماسک;

    /* Clear the lowest set bit n times */
    طالما (n-- && ماسک)
        ماسک &= (ماسک - 1);

    أرجع original ^ ماسک;
}
