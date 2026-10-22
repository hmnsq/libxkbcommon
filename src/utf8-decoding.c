/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश "utils.h"
#समावेश "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
अटल स्थिर uint8_t utf8_sequence_लंबाई_by_leading_byte[256] = {
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
utf8_sequence_लंबाई(स्थिर अक्षर *s)
{
    वापस utf8_sequence_लंबाई_by_leading_byte[(अचिन्हित अक्षर)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_अग्र_code_point(स्थिर अक्षर *s, माप_प्रकार max_आकार, माप_प्रकार *आकार_out)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_लंबाई(s);
    *आकार_out = 0;

    अगर (!max_आकार || len > max_आकार)
        वापस INVALID_UTF8_CODE_POINT;

    /* Handle leading byte */
    चयन (len) {
    हाल 1:
        *आकार_out = 1;
        वापस (uint32_t)s[0];
    हाल 2:
        cp = (uint32_t)s[0] & 0x1f;
        अवरोध;
    हाल 3:
        cp = (uint32_t)s[0] & 0x0f;
        अवरोध;
    हाल 4:
        cp = (uint32_t)s[0] & 0x07;
        अवरोध;
    शेष:
        वापस INVALID_UTF8_CODE_POINT;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    क्रम (माप_प्रकार k = 1; k < len; k++) {
        अगर (((uint32_t)s[k] & 0xc0) != 0x80)
            वापस INVALID_UTF8_CODE_POINT;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    अगर (is_surrogate(cp))
        वापस INVALID_UTF8_CODE_POINT;

    *आकार_out = len;
    वापस cp;
}
