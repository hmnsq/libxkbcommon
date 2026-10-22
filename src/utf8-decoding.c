/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ਸਥਿਰ ਅਚਲ uint8_t utf8_sequence_ਲੰਮਾਈ_by_leading_byte[256] = {
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
utf8_sequence_ਲੰਮਾਈ(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ utf8_sequence_ਲੰਮਾਈ_by_leading_byte[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_ਅਗਲਾ_ਕੋਡ_ਬਿੰਦੂ(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t max_ਮਾਪ, ਮਾਪ_t *ਮਾਪ_ਬਾਹਰਲਾ)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_ਲੰਮਾਈ(s);
    *ਮਾਪ_ਬਾਹਰਲਾ = 0;

    ਜੇ (!max_ਮਾਪ || len > max_ਮਾਪ)
        ਵਾਪਸ INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ;

    /* Handle leading byte */
    ਚੋਣ (len) {
    ਹਾਲਤ 1:
        *ਮਾਪ_ਬਾਹਰਲਾ = 1;
        ਵਾਪਸ (uint32_t)s[0];
    ਹਾਲਤ 2:
        cp = (uint32_t)s[0] & 0x1f;
        ਰੋਕੋ;
    ਹਾਲਤ 3:
        cp = (uint32_t)s[0] & 0x0f;
        ਰੋਕੋ;
    ਹਾਲਤ 4:
        cp = (uint32_t)s[0] & 0x07;
        ਰੋਕੋ;
    ਮੂਲ:
        ਵਾਪਸ INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    ਲਈ (ਮਾਪ_t k = 1; k < len; k++) {
        ਜੇ (((uint32_t)s[k] & 0xc0) != 0x80)
            ਵਾਪਸ INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    ਜੇ (is_surrogate(cp))
        ਵਾਪਸ INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ;

    *ਮਾਪ_ਬਾਹਰਲਾ = len;
    ਵਾਪਸ cp;
}
