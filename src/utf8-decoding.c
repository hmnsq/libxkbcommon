/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "utils.h"
#అన్తర్భావ "utf8-decoding.h"

/* Array mapping the leading byte to the length of a UTF-8 sequence.
 * A value of zero indicates that the byte can not begin a UTF-8 sequence. */
స్థిర స్థిరరాశి uint8_t utf8_sequence_నిడివి_by_leading_byte[256] = {
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
utf8_sequence_నిడివి(స్థిరరాశి అక్షరం *s)
{
    తిరిగి utf8_sequence_నిడివి_by_leading_byte[(గుర్తులేని అక్షరం)s[0]];
}

/* Reads the next UTF-8 sequence in a string */
uint32_t
utf8_తదుపరి_సంకేతం_బిందువు(స్థిరరాశి అక్షరం *s, కొలత_t max_కొలత, కొలత_t *కొలత_బహిర్గతం)
{
    uint32_t cp = 0;
    uint8_t len = utf8_sequence_నిడివి(s);
    *కొలత_బహిర్గతం = 0;

    ఒకవేళ (!max_కొలత || len > max_కొలత)
        తిరిగి INVALID_UTF8_సంకేతం_బిందువు;

    /* Handle leading byte */
    ఎంపిక (len) {
    స్థితి 1:
        *కొలత_బహిర్గతం = 1;
        తిరిగి (uint32_t)s[0];
    స్థితి 2:
        cp = (uint32_t)s[0] & 0x1f;
        ఆపు;
    స్థితి 3:
        cp = (uint32_t)s[0] & 0x0f;
        ఆపు;
    స్థితి 4:
        cp = (uint32_t)s[0] & 0x07;
        ఆపు;
    అప్రమేయ:
        తిరిగి INVALID_UTF8_సంకేతం_బిందువు;
    }

    /* Process remaining bytes of the UTF-8 sequence */
    కొరకు (కొలత_t k = 1; k < len; k++) {
        ఒకవేళ (((uint32_t)s[k] & 0xc0) != 0x80)
            తిరిగి INVALID_UTF8_సంకేతం_బిందువు;
        cp <<= 6;
        cp |= (uint32_t)s[k] & 0x3f;
    }

    /* Check surrogates */
    ఒకవేళ (is_surrogate(cp))
        తిరిగి INVALID_UTF8_సంకేతం_బిందువు;

    *కొలత_బహిర్గతం = len;
    తిరిగి cp;
}
