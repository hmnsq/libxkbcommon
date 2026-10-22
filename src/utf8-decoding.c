/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "utils.h"
#ضمن "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ساكن ثابت uint8_t utf8_sequence_لمائی_by_leading_byte[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00-0x0F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x10-0x1F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x20-0x2F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x30-0x3F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x40-0x4F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x50-0x5F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x60-0x6F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x70-0x7F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x80-0x8F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0x90-0x9F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0xA0-0xAF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0xB0-0xBF */
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 0xC0-0xCF */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 0xD0-0xDF */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, /* 0xE0-0xEF */
    4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0xF0-0xFF */
};

/* Length of next utf-8 sequence */
uint8_t
utf8_sequence_لمائی(ثابت حرف *s)
{
    أرجع utf8_sequence_لمائی_by_leading_byte[(بنانشانی حرف)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_اگلا_کوڈ_نقطہ(ثابت حرف *s, ناپ_t max_ناپ, ناپ_t *ناپ_باہرلا)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_لمائی(s);
    *ناپ_باہرلا = 0;

    إذا (!max_ناپ || len > max_ناپ)
        أرجع INVALID_UTF8_کوڈ_نقطہ;

    /* Handle leading byte */
    بدل (len) {
    حالة 1:
        *ناپ_باہرلا = 1;
        أرجع (uint32_t)s[0];
    حالة 2:
        cp = (uint32_t)s[0] & 0x1f;
        اكسر;
    حالة 3:
        cp = (uint32_t)s[0] & 0x0f;
        اكسر;
    حالة 4:
        cp = (uint32_t)s[0] & 0x07;
        اكسر;
    افتراضي:
        أرجع INVALID_UTF8_کوڈ_نقطہ;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    لكل (ناپ_t k = 1; k < len; k++) {
        إذا (((uint32_t)s[k] & 0xc0) != 0x80)
            أرجع INVALID_UTF8_کوڈ_نقطہ;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    إذا (is_surrogate(cp))
        أرجع INVALID_UTF8_کوڈ_نقطہ;

    *ناپ_باہرلا = len;
    أرجع cp;
}
