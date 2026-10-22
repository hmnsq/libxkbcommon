/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#شامل "config.h"

#شامل "utils.h"
#شامل "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, نویسه *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    اگر (unichar <= 0x007f) {
        buffer[0] = (نویسه) unichar;
        buffer[1] = '\0';
        بازگشت 2;
    }
    وگرنه اگر (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    وگرنه اگر (is_surrogate(unichar)) {
        برو ill_formed_code_unit_subsequence;
    }
    وگرنه اگر (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    وگرنه اگر (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    وگرنه {
        برو ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    برای (unsigned صحیح شمار = length - 1; شمار > 0; شمار--, shift += 6)
        buffer[شمار] = (نویسه)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (نویسه)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    بازگشت length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    بازگشت 0;
}

bool
is_valid_utf8(ثابت نویسه *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    وقتی (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        اگر (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        وگرنه اگر (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        وگرنه اگر (s[i] == 0xE0) {
            i++;
            اگر (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                بازگشت false;
            tail_bytes = 1;
        }
        وگرنه اگر (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        وگرنه اگر (s[i] == 0xED) {
            i++;
            اگر (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                بازگشت false;
            tail_bytes = 1;
        }
        وگرنه اگر (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        وگرنه اگر (s[i] == 0xF0) {
            i++;
            اگر (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                بازگشت false;
            tail_bytes = 2;
        }
        وگرنه اگر (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        وگرنه اگر (s[i] == 0xF4) {
            i++;
            اگر (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                بازگشت false;
            tail_bytes = 2;
        }
        وگرنه {
            بازگشت false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        وقتی (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        اگر (tail_bytes != 0)
            بازگشت false;
    }

    بازگشت true;
}
