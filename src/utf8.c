/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, ৱর্ণ *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    যদি (unichar <= 0x007f) {
        buffer[0] = (ৱর্ণ) unichar;
        buffer[1] = '\0';
        ঘূৰাই 2;
    }
    অন্যথা যদি (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    অন্যথা যদি (is_surrogate(unichar)) {
        গচ্ছ ill_formed_code_unit_subsequence;
    }
    অন্যথা যদি (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    অন্যথা যদি (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    অন্যথা {
        গচ্ছ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    বাবে (unsigned পূর্ণাঙ্ক গুনতি = length - 1; গুনতি > 0; গুনতি--, shift += 6)
        buffer[গুনতি] = (ৱর্ণ)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (ৱর্ণ)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    ঘূৰাই length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    ঘূৰাই 0;
}

bool
is_valid_utf8(ধ্রুৱ ৱর্ণ *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ধ্রুৱ uint8_t *s = (ধ্রুৱ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    যেতিয়ালৈ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        যদি (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        অন্যথা যদি (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        অন্যথা যদি (s[i] == 0xE0) {
            i++;
            যদি (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ঘূৰাই false;
            tail_bytes = 1;
        }
        অন্যথা যদি (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        অন্যথা যদি (s[i] == 0xED) {
            i++;
            যদি (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ঘূৰাই false;
            tail_bytes = 1;
        }
        অন্যথা যদি (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        অন্যথা যদি (s[i] == 0xF0) {
            i++;
            যদি (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ঘূৰাই false;
            tail_bytes = 2;
        }
        অন্যথা যদি (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        অন্যথা যদি (s[i] == 0xF4) {
            i++;
            যদি (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ঘূৰাই false;
            tail_bytes = 2;
        }
        অন্যথা {
            ঘূৰাই false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        যেতিয়ালৈ (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        যদি (tail_bytes != 0)
            ঘূৰাই false;
    }

    ঘূৰাই true;
}
