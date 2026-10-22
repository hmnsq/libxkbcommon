/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ಸ್ಥಿರ ಧ್ರುವ uint8_t utf8_sequence_length_by_leading_byte[256] = {
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
utf8_sequence_length(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ utf8_sequence_length_by_leading_byte[(unsigned ವರ್ಣ)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_next_code_point(ಧ್ರುವ ವರ್ಣ *s, size_t max_size, size_t *size_out)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_length(s);
    *size_out = 0;

    ಯದಿ (!max_size || len > max_size)
        ಪ್ರತ್ಯಾಗಮ INVALID_UTF8_CODE_POINT;

    /* Handle leading byte */
    ವಿಕಲ್ಪ (len) {
    ಪಕ್ಷ 1:
        *size_out = 1;
        ಪ್ರತ್ಯಾಗಮ (uint32_t)s[0];
    ಪಕ್ಷ 2:
        cp = (uint32_t)s[0] & 0x1f;
        ಭಙ್ಗ;
    ಪಕ್ಷ 3:
        cp = (uint32_t)s[0] & 0x0f;
        ಭಙ್ಗ;
    ಪಕ್ಷ 4:
        cp = (uint32_t)s[0] & 0x07;
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ INVALID_UTF8_CODE_POINT;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    ಪ್ರತಿ (size_t k = 1; k < len; k++) {
        ಯದಿ (((uint32_t)s[k] & 0xc0) != 0x80)
            ಪ್ರತ್ಯಾಗಮ INVALID_UTF8_CODE_POINT;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    ಯದಿ (is_surrogate(cp))
        ಪ್ರತ್ಯಾಗಮ INVALID_UTF8_CODE_POINT;

    *size_out = len;
    ಪ್ರತ್ಯಾಗಮ cp;
}
