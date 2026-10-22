/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>

#إذا defined(_MSC_VER)
# include <intrin.h>
#جيڪڏهن_پورو

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

#وصف_ڪريو MAKE_PARSE_DEC_منزل(type, max)                         \
ساكن مضمن صحيح                                            \
parse_dec_منزل##type(ثابت حرف *s, سائيز_t len, type (*ٻاهريون))  \
{                                                            \
    type نتيجو = 0;                                         \
    سائيز_t i;                                                \
    لكل (i = 0;                                              \
         i < len && (بنانشان حرف)(s[i] - '0') < 10U &&     \
         نتيجو <= (max) / 10 &&                             \
         نتيجو * 10 <= (max) - (بنانشان حرف) (s[i] - '0');\
         i++) {                                              \
        نتيجو = نتيجو * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *ٻاهريون = نتيجو;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    أرجع (i >= len || (بنانشان حرف)(s[i] - '0') >= 10U)  \
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

#وصف_رد MAKE_PARSE_DEC_منزل

ساكن ثابت بنانشان حرف digits__[] = {
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

#وصف_ڪريو MAKE_PARSE_HEX_منزل(type, max)                           \
ساكن مضمن صحيح                                              \
parse_hex_منزل##type(ثابت حرف *s, سائيز_t len, type (*ٻاهريون))    \
{                                                              \
    type نتيجو = 0;                                           \
    سائيز_t i = 0;                                              \
    لكل (; i < len && digits__[(بنانشان حرف) s[i]] < 16u &&  \
         نتيجو <= (max) >> 4;                                 \
         i++) {                                                \
        نتيجو = نتيجو * 16 + digits__[(بنانشان حرف) s[i]]; \
    }                                                          \
    *ٻاهريون = نتيجو;                                             \
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

#وصف_رد MAKE_PARSE_HEX_منزل

ساكن مضمن بنانشان صحيح
popcount32(uint32_t x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (بنانشان صحيح)__builtin_popcountl(x);
#وإلاإذا defined(_MSC_VER)
    أرجع __popcnt(x);
#وإلا
    /* Fallback (Brian Kernighan’s method) */
    بنانشان صحيح ڳڻپ = 0;
    طالما (x) {
        x &= x - 1;
        ڳڻپ++;
    }
    أرجع ڳڻپ;
#جيڪڏهن_پورو
}

ساكن مضمن بنانشان صحيح
ctz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بنانشان صحيح: (بنانشان صحيح)__builtin_ctz(x),
        بنانشان ڊگهو: (بنانشان صحيح)__builtin_ctzl(x),
        بنانشان ڊگهو ڊگهو: (بنانشان صحيح)__builtin_ctzll(x)
    );
#وإلاإذا defined(_MSC_VER)
    بنانشان ڊگهو اشاريو;
    ٻٽو_انگ_جاچ_Forward(&اشاريو, x);
    أرجع (بنانشان صحيح)اشاريو;
#وإلا
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    ساكن ثابت بنانشان صحيح debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    أرجع debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#جيڪڏهن_پورو
}

ساكن مضمن بنانشان صحيح
clz32(uint32_t x)
{
    إذا (x == 0)
        أرجع 32;

#إذا defined(__GNUC__) || defined(__clang__)
    أرجع _Generic(
        x,
        بنانشان صحيح:
            (بنانشان صحيح)__builtin_clz(x) -
            (بنانشان صحيح)((حجم(بنانشان صحيح) * CHAR_BIT) - 32),
        بنانشان ڊگهو:
            (بنانشان صحيح)__builtin_clzl(x) -
            (بنانشان صحيح)((حجم(بنانشان ڊگهو) * CHAR_BIT) - 32),
        بنانشان ڊگهو ڊگهو:
            (بنانشان صحيح)__builtin_clzll(x) -
            (بنانشان صحيح)((حجم(بنانشان ڊگهو ڊگهو) * CHAR_BIT) - 32)
    );
#وإلا
    // Efficient binary search fallback for 32-bit integers
    بنانشان صحيح ڳڻپ = 0;
    إذا ((x & 0xffff0000) == 0) { ڳڻپ += 16; x <<= 16; }
    إذا ((x & 0xff000000) == 0) { ڳڻپ += 8;  x <<= 8;  }
    إذا ((x & 0xf0000000) == 0) { ڳڻپ += 4;  x <<= 4;  }
    إذا ((x & 0xc0000000) == 0) { ڳڻپ += 2;  x <<= 2;  }
    إذا ((x & 0x80000000) == 0) { ڳڻپ += 1; }
    أرجع ڳڻپ;
#جيڪڏهن_پورو
}

ساكن مضمن بنانشان صحيح
ايندڙ_pow2(بنانشان صحيح x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    إذا (x <= 1) أرجع 1u;
    أرجع 1u << ((صحيح)حجم(بنانشان) * CHAR_BIT - __builtin_clz(x - 1));
#وإلا
    إذا (x <= 1u) أرجع 1u;
    x--;
    لكل (بنانشان s = 1; s < (بنانشان)حجم(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    أرجع x + 1u;
#جيڪڏهن_پورو
}

ساكن مضمن uint32_t
keep_سڀ_کان_هيٺ_n_سيٽ_بٽ(uint32_t نقاب, بنانشان صحيح n) {
    ثابت uint32_t original = نقاب;

    /* Clear the lowest set bit n times */
    طالما (n-- && نقاب)
        نقاب &= (نقاب - 1);

    أرجع original ^ نقاب;
}
