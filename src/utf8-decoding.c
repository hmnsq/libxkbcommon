/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
স্থির ধ্রুবক uint8_t utf8_sequence_দৈর্ঘ্য_by_leading_byte[256] = {
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
utf8_sequence_দৈর্ঘ্য(ধ্রুবক অক্ষর *s)
{
    ফেরত utf8_sequence_দৈর্ঘ্য_by_leading_byte[(unsigned অক্ষর)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_পরবর্তী_code_point(ধ্রুবক অক্ষর *s, আকার_t max_আকার, আকার_t *আকার_out)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_দৈর্ঘ্য(s);
    *আকার_out = 0;

    যদি (!max_আকার || len > max_আকার)
        ফেরত INVALID_UTF8_CODE_POINT;

    /* Handle leading byte */
    নির্বাচন (len) {
    ক্ষেত্র 1:
        *আকার_out = 1;
        ফেরত (uint32_t)s[0];
    ক্ষেত্র 2:
        cp = (uint32_t)s[0] & 0x1f;
        বিরতি;
    ক্ষেত্র 3:
        cp = (uint32_t)s[0] & 0x0f;
        বিরতি;
    ক্ষেত্র 4:
        cp = (uint32_t)s[0] & 0x07;
        বিরতি;
    পূর্বনির্ধারিত:
        ফেরত INVALID_UTF8_CODE_POINT;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    জন্য (আকার_t k = 1; k < len; k++) {
        যদি (((uint32_t)s[k] & 0xc0) != 0x80)
            ফেরত INVALID_UTF8_CODE_POINT;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    যদি (is_surrogate(cp))
        ফেরত INVALID_UTF8_CODE_POINT;

    *আকার_out = len;
    ফেরত cp;
}
