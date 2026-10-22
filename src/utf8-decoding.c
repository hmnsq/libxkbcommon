/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ସ୍ଥିର ଅଚଳ uint8_t utf8_sequence_length_by_leading_byte[256] = {
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
utf8_sequence_length(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ utf8_sequence_length_by_leading_byte[(unsigned ଅକ୍ଷର)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_next_code_point(ଅଚଳ ଅକ୍ଷର *s, size_t max_size, size_t *size_out)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_length(s);
    *size_out = 0;

    ଯଦି (!max_size || len > max_size)
        ଫେରନ୍ତୁ INVALID_UTF8_CODE_POINT;

    /* Handle leading byte */
    ବାଛ (len) {
    ସ୍ଥିତି 1:
        *size_out = 1;
        ଫେରନ୍ତୁ (uint32_t)s[0];
    ସ୍ଥିତି 2:
        cp = (uint32_t)s[0] & 0x1f;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 3:
        cp = (uint32_t)s[0] & 0x0f;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 4:
        cp = (uint32_t)s[0] & 0x07;
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ INVALID_UTF8_CODE_POINT;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    ପାଇଁ (size_t k = 1; k < len; k++) {
        ଯଦି (((uint32_t)s[k] & 0xc0) != 0x80)
            ଫେରନ୍ତୁ INVALID_UTF8_CODE_POINT;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    ଯଦି (is_surrogate(cp))
        ଫେରନ୍ତୁ INVALID_UTF8_CODE_POINT;

    *size_out = len;
    ଫେରନ୍ତୁ cp;
}
