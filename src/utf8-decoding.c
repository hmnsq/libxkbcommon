/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "utils.h"
#شامل "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ساکن ثابت uint8_t utf8_sequence_لمبائی_by_leading_byte[256] = {
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
utf8_sequence_لمبائی(ثابت حرف *s)
{
    واپس utf8_sequence_لمبائی_by_leading_byte[(غیرعلامتی حرف)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_اگلا_رمز_نقطہ(ثابت حرف *s, سائز_t max_سائز, سائز_t *سائز_برآمد)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_لمبائی(s);
    *سائز_برآمد = 0;

    اگر (!max_سائز || len > max_سائز)
        واپس INVALID_UTF8_رمز_نقطہ;

    /* Handle leading byte */
    انتخاب (len) {
    حالت 1:
        *سائز_برآمد = 1;
        واپس (uint32_t)s[0];
    حالت 2:
        cp = (uint32_t)s[0] & 0x1f;
        توڑو;
    حالت 3:
        cp = (uint32_t)s[0] & 0x0f;
        توڑو;
    حالت 4:
        cp = (uint32_t)s[0] & 0x07;
        توڑو;
    طےشدہ:
        واپس INVALID_UTF8_رمز_نقطہ;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    برائے (سائز_t k = 1; k < len; k++) {
        اگر (((uint32_t)s[k] & 0xc0) != 0x80)
            واپس INVALID_UTF8_رمز_نقطہ;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    اگر (is_surrogate(cp))
        واپس INVALID_UTF8_رمز_نقطہ;

    *سائز_برآمد = len;
    واپس cp;
}
