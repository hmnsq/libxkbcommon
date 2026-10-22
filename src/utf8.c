/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, ଅକ୍ଷର *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ଯଦି (unichar <= 0x007f) {
        buffer[0] = (ଅକ୍ଷର) unichar;
        buffer[1] = '\0';
        ଫେରନ୍ତୁ 2;
    }
    ଅନ୍ୟଥା ଯଦି (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    ଅନ୍ୟଥା ଯଦି (is_surrogate(unichar)) {
        ଯାଅ ill_formed_code_unit_subsequence;
    }
    ଅନ୍ୟଥା ଯଦି (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    ଅନ୍ୟଥା ଯଦି (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    ଅନ୍ୟଥା {
        ଯାଅ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = length - 1; ଗଣନା_2 > 0; ଗଣନା_2--, shift += 6)
        buffer[ଗଣନା_2] = (ଅକ୍ଷର)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (ଅକ୍ଷର)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    ଫେରନ୍ତୁ length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    ଫେରନ୍ତୁ 0;
}

bool
is_valid_utf8(ଅଚଳ ଅକ୍ଷର *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ଅଚଳ uint8_t *s = (ଅଚଳ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ଯେତେବେଳେ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ଯଦି (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] == 0xE0) {
            i++;
            ଯଦି (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ଫେରନ୍ତୁ false;
            tail_bytes = 1;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] == 0xED) {
            i++;
            ଯଦି (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ଫେରନ୍ତୁ false;
            tail_bytes = 1;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] == 0xF0) {
            i++;
            ଯଦି (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ଫେରନ୍ତୁ false;
            tail_bytes = 2;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        ଅନ୍ୟଥା ଯଦି (s[i] == 0xF4) {
            i++;
            ଯଦି (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ଫେରନ୍ତୁ false;
            tail_bytes = 2;
        }
        ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ଯେତେବେଳେ (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        ଯଦି (tail_bytes != 0)
            ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}
