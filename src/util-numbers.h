/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdint.h>

#ఒకవేళ defined(_MSC_VER)
# include <intrin.h>
#అయితే_ముగింపు

#అన్తర్భావ "utils.h"

/*
 * Define various parsers to avoid the use of strto* -- it’s slower and accepts
 * a bunch of stuff we don’t want to allow, like signs, spaces, even locale stuff.
 *
 * But the real feature is that it does not require a NULL-terminated string, so
 * it works safely *also* on any buffer, assuming the correct corresponding size
 * is provided. For NULL-terminated strings, just pass `SIZE_MAX` as the length:
 * the parsers will *always* stop on a NULL character.
 */

#నిర్వచించు MAKE_PARSE_DEC_గమ్యం(type, max)                         \
స్థిర అంతఃవరుస పూర్ణాంకం                                            \
parse_dec_గమ్యం##type(స్థిరరాశి అక్షరం *s, కొలత_t len, type (*బహిర్గతం))  \
{                                                            \
    type ఫలితం = 0;                                         \
    కొలత_t i;                                                \
    కొరకు (i = 0;                                              \
         i < len && (గుర్తులేని అక్షరం)(s[i] - '0') < 10U &&     \
         ఫలితం <= (max) / 10 &&                             \
         ఫలితం * 10 <= (max) - (గుర్తులేని అక్షరం) (s[i] - '0');\
         i++) {                                              \
        ఫలితం = ఫలితం * 10 + (type)(s[i] - '0');           \
    }                                                        \
    *బహిర్గతం = ఫలితం;                                           \
    /* Check if there is more to parse */                    \
    /* We can safely convert the length to int on success */ \
    తిరిగి (i >= len || (గుర్తులేని అక్షరం)(s[i] - '0') >= 10U)  \
           ? (పూర్ణాంకం) i                                         \
           : -1;                                             \
}

/**
 * Parse a `uint32_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_గమ్యం(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in decimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_DEC_గమ్యం(uint64_t, UINT64_MAX)

#నిర్వచనం_రద్దు MAKE_PARSE_DEC_గమ్యం

స్థిర స్థిరరాశి గుర్తులేని అక్షరం digits__[] = {
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

#నిర్వచించు MAKE_PARSE_HEX_గమ్యం(type, max)                           \
స్థిర అంతఃవరుస పూర్ణాంకం                                              \
parse_hex_గమ్యం##type(స్థిరరాశి అక్షరం *s, కొలత_t len, type (*బహిర్గతం))    \
{                                                              \
    type ఫలితం = 0;                                           \
    కొలత_t i = 0;                                              \
    కొరకు (; i < len && digits__[(గుర్తులేని అక్షరం) s[i]] < 16u &&  \
         ఫలితం <= (max) >> 4;                                 \
         i++) {                                                \
        ఫలితం = ఫలితం * 16 + digits__[(గుర్తులేని అక్షరం) s[i]]; \
    }                                                          \
    *బహిర్గతం = ఫలితం;                                             \
    /* Check if there is more to parse */                      \
    /* We can safely convert the length to int on success */   \
    తిరిగి (i >= len || !is_xdigit(s[i])) ? (పూర్ణాంకం) i : -1;      \
}

/**
 * Parse a `uint32_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_గమ్యం(uint32_t, UINT32_MAX)

/**
 * Parse a `uint64_t` in hexdecimal format.
 *
 * @returns -1 on error (overflow) or the count of characters parsed.
 */
MAKE_PARSE_HEX_గమ్యం(uint64_t, UINT64_MAX)

#నిర్వచనం_రద్దు MAKE_PARSE_HEX_గమ్యం

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
popcount32(uint32_t x)
{
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    తిరిగి (గుర్తులేని పూర్ణాంకం)__builtin_popcountl(x);
#లేదావేళ defined(_MSC_VER)
    తిరిగి __popcnt(x);
#లేకపోతే
    /* Fallback (Brian Kernighan’s method) */
    గుర్తులేని పూర్ణాంకం లెక్క = 0;
    అయితే (x) {
        x &= x - 1;
        లెక్క++;
    }
    తిరిగి లెక్క;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
ctz32(uint32_t x)
{
    ఒకవేళ (x == 0)
        తిరిగి 32;

#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    తిరిగి _Generic(
        x,
        గుర్తులేని పూర్ణాంకం: (గుర్తులేని పూర్ణాంకం)__builtin_ctz(x),
        గుర్తులేని దీర్ఘ: (గుర్తులేని పూర్ణాంకం)__builtin_ctzl(x),
        గుర్తులేని దీర్ఘ దీర్ఘ: (గుర్తులేని పూర్ణాంకం)__builtin_ctzll(x)
    );
#లేదావేళ defined(_MSC_VER)
    గుర్తులేని దీర్ఘ సూచీ;
    బిట్టు_పరిశీలన_Forward(&సూచీ, x);
    తిరిగి (గుర్తులేని పూర్ణాంకం)సూచీ;
#లేకపోతే
    /* Fallback: De Bruijn sequence algorithm for 32-bit integers */
    స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం debruijn32[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    తిరిగి debruijn32[((x & -x) * UINT32_C(0x077cb531)) >> 27];
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
clz32(uint32_t x)
{
    ఒకవేళ (x == 0)
        తిరిగి 32;

#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    తిరిగి _Generic(
        x,
        గుర్తులేని పూర్ణాంకం:
            (గుర్తులేని పూర్ణాంకం)__builtin_clz(x) -
            (గుర్తులేని పూర్ణాంకం)((పరిమాణం(గుర్తులేని పూర్ణాంకం) * CHAR_BIT) - 32),
        గుర్తులేని దీర్ఘ:
            (గుర్తులేని పూర్ణాంకం)__builtin_clzl(x) -
            (గుర్తులేని పూర్ణాంకం)((పరిమాణం(గుర్తులేని దీర్ఘ) * CHAR_BIT) - 32),
        గుర్తులేని దీర్ఘ దీర్ఘ:
            (గుర్తులేని పూర్ణాంకం)__builtin_clzll(x) -
            (గుర్తులేని పూర్ణాంకం)((పరిమాణం(గుర్తులేని దీర్ఘ దీర్ఘ) * CHAR_BIT) - 32)
    );
#లేకపోతే
    // Efficient binary search fallback for 32-bit integers
    గుర్తులేని పూర్ణాంకం లెక్క = 0;
    ఒకవేళ ((x & 0xffff0000) == 0) { లెక్క += 16; x <<= 16; }
    ఒకవేళ ((x & 0xff000000) == 0) { లెక్క += 8;  x <<= 8;  }
    ఒకవేళ ((x & 0xf0000000) == 0) { లెక్క += 4;  x <<= 4;  }
    ఒకవేళ ((x & 0xc0000000) == 0) { లెక్క += 2;  x <<= 2;  }
    ఒకవేళ ((x & 0x80000000) == 0) { లెక్క += 1; }
    తిరిగి లెక్క;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
తదుపరి_pow2(గుర్తులేని పూర్ణాంకం x)
{
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    ఒకవేళ (x <= 1) తిరిగి 1u;
    తిరిగి 1u << ((పూర్ణాంకం)పరిమాణం(గుర్తులేని) * CHAR_BIT - __builtin_clz(x - 1));
#లేకపోతే
    ఒకవేళ (x <= 1u) తిరిగి 1u;
    x--;
    కొరకు (గుర్తులేని s = 1; s < (గుర్తులేని)పరిమాణం(x)*CHAR_BIT; s <<= 1)
        x |= x >> s;
    తిరిగి x + 1u;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస uint32_t
keep_అత్యల్ప_n_సమితి_బిట్లు(uint32_t ముసుగు, గుర్తులేని పూర్ణాంకం n) {
    స్థిరరాశి uint32_t original = ముసుగు;

    /* Clear the lowest set bit n times */
    అయితే (n-- && ముసుగు)
        ముసుగు &= (ముసుగు - 1);

    తిరిగి original ^ ముసుగు;
}
