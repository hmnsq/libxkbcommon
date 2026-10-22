/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint8_t utf8_sequence_ಉದ್ದಳತೆ_by_leading_byte[256] = {
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
utf8_sequence_ಉದ್ದಳತೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ utf8_sequence_ಉದ್ದಳತೆ_by_leading_byte[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_ಮುಂದಿನ_ಸಂಕೇತ_ಬಿಂದು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t max_ಗಾತ್ರ, ಗಾತ್ರ_t *ಗಾತ್ರ_ಹೊರಹೋಗು)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_ಉದ್ದಳತೆ(s);
    *ಗಾತ್ರ_ಹೊರಹೋಗು = 0;

    ಆದರೆ (!max_ಗಾತ್ರ || len > max_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು;

    /* Handle leading byte */
    ಆಯ್ಕೆ (len) {
    ಸ್ಥಿತಿ 1:
        *ಗಾತ್ರ_ಹೊರಹೋಗು = 1;
        ಹಿಂತಿರುಗಿ (uint32_t)s[0];
    ಸ್ಥಿತಿ 2:
        cp = (uint32_t)s[0] & 0x1f;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 3:
        cp = (uint32_t)s[0] & 0x0f;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 4:
        cp = (uint32_t)s[0] & 0x07;
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    ಗಾಗಿ (ಗಾತ್ರ_t k = 1; k < len; k++) {
        ಆದರೆ (((uint32_t)s[k] & 0xc0) != 0x80)
            ಹಿಂತಿರುಗಿ INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    ಆದರೆ (is_surrogate(cp))
        ಹಿಂತಿರುಗಿ INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು;

    *ಗಾತ್ರ_ಹೊರಹೋಗು = len;
    ಹಿಂತಿರುಗಿ cp;
}
