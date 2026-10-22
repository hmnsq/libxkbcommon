/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, ವರ್ಣ *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ಯದಿ (unichar <= 0x007f) {
        buffer[0] = (ವರ್ಣ) unichar;
        buffer[1] = '\0';
        ಪ್ರತ್ಯಾಗಮ 2;
    }
    ಅನ್ಯಥಾ ಯದಿ (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    ಅನ್ಯಥಾ ಯದಿ (is_surrogate(unichar)) {
        ಗಚ್ಛ ill_formed_code_unit_subsequence;
    }
    ಅನ್ಯಥಾ ಯದಿ (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    ಅನ್ಯಥಾ ಯದಿ (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    ಅನ್ಯಥಾ {
        ಗಚ್ಛ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = length - 1; ಎಣಿಕೆ > 0; ಎಣಿಕೆ--, shift += 6)
        buffer[ಎಣಿಕೆ] = (ವರ್ಣ)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (ವರ್ಣ)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    ಪ್ರತ್ಯಾಗಮ length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    ಪ್ರತ್ಯಾಗಮ 0;
}

bool
is_valid_utf8(ಧ್ರುವ ವರ್ಣ *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ಧ್ರುವ uint8_t *s = (ಧ್ರುವ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ಯಾವತ್ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ಯದಿ (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] == 0xE0) {
            i++;
            ಯದಿ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ಪ್ರತ್ಯಾಗಮ false;
            tail_bytes = 1;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] == 0xED) {
            i++;
            ಯದಿ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ಪ್ರತ್ಯಾಗಮ false;
            tail_bytes = 1;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] == 0xF0) {
            i++;
            ಯದಿ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ಪ್ರತ್ಯಾಗಮ false;
            tail_bytes = 2;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        ಅನ್ಯಥಾ ಯದಿ (s[i] == 0xF4) {
            i++;
            ಯದಿ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ಪ್ರತ್ಯಾಗಮ false;
            tail_bytes = 2;
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ಯಾವತ್ (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        ಯದಿ (tail_bytes != 0)
            ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}
