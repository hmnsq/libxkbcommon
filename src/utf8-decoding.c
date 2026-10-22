/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "utils.h"
#अन्तर्भाव "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
स्थिर ध्रुव uint8_t utf8_sequence_दीर्घता_by_leading_byte[256] = {
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
utf8_sequence_दीर्घता(ध्रुव वर्ण *s)
{
    प्रत्यागम utf8_sequence_दीर्घता_by_leading_byte[(अचिह्नित वर्ण)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_अग्र_संहिता_point(ध्रुव वर्ण *s, आकार_t max_आकार, आकार_t *आकार_बहिर्गमनम्)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_दीर्घता(s);
    *आकार_बहिर्गमनम् = 0;

    यदि (!max_आकार || len > max_आकार)
        प्रत्यागम INVALID_UTF8_संहिता_POINT;

    /* Handle leading byte */
    विकल्प (len) {
    पक्ष 1:
        *आकार_बहिर्गमनम् = 1;
        प्रत्यागम (uint32_t)s[0];
    पक्ष 2:
        cp = (uint32_t)s[0] & 0x1f;
        भङ्ग;
    पक्ष 3:
        cp = (uint32_t)s[0] & 0x0f;
        भङ्ग;
    पक्ष 4:
        cp = (uint32_t)s[0] & 0x07;
        भङ्ग;
    स्वभाव:
        प्रत्यागम INVALID_UTF8_संहिता_POINT;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    प्रति (आकार_t k = 1; k < len; k++) {
        यदि (((uint32_t)s[k] & 0xc0) != 0x80)
            प्रत्यागम INVALID_UTF8_संहिता_POINT;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    यदि (is_surrogate(cp))
        प्रत्यागम INVALID_UTF8_संहिता_POINT;

    *आकार_बहिर्गमनम् = len;
    प्रत्यागम cp;
}
