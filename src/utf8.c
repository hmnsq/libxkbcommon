/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "utils.h"
#अन्तर्भाव "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, वर्ण *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    जइ (unichar <= 0x007f) {
        buffer[0] = (वर्ण) unichar;
        buffer[1] = '\0';
        प्रत्यागम 2;
    }
    अण्णहा जइ (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    अण्णहा जइ (is_surrogate(unichar)) {
        गच्छ ill_formed_code_unit_subsequence;
    }
    अण्णहा जइ (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    अण्णहा जइ (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    अण्णहा {
        गच्छ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    प्रति (unsigned पूर्णाङ्क गणनम् = length - 1; गणनम् > 0; गणनम्--, shift += 6)
        buffer[गणनम्] = (वर्ण)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (वर्ण)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    प्रत्यागम length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    प्रत्यागम 0;
}

bool
is_valid_utf8(ध्रुव वर्ण *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ध्रुव uint8_t *s = (ध्रुव uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    जाव (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        जइ (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        अण्णहा जइ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        अण्णहा जइ (s[i] == 0xE0) {
            i++;
            जइ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                प्रत्यागम false;
            tail_bytes = 1;
        }
        अण्णहा जइ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        अण्णहा जइ (s[i] == 0xED) {
            i++;
            जइ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                प्रत्यागम false;
            tail_bytes = 1;
        }
        अण्णहा जइ (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        अण्णहा जइ (s[i] == 0xF0) {
            i++;
            जइ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                प्रत्यागम false;
            tail_bytes = 2;
        }
        अण्णहा जइ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        अण्णहा जइ (s[i] == 0xF4) {
            i++;
            जइ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                प्रत्यागम false;
            tail_bytes = 2;
        }
        अण्णहा {
            प्रत्यागम false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        जाव (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        जइ (tail_bytes != 0)
            प्रत्यागम false;
    }

    प्रत्यागम true;
}
