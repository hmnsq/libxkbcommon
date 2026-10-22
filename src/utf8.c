/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ضمن "config.h"

#ضمن "utils.h"
#ضمن "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, حرف *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    𐤀𐤌 (unichar <= 0x007f) {
        buffer[0] = (حرف) unichar;
        buffer[1] = '\0';
        أرجع 2;
    }
    وإلا 𐤀𐤌 (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    وإلا 𐤀𐤌 (is_surrogate(unichar)) {
        اذهب ill_formed_code_unit_subsequence;
    }
    وإلا 𐤀𐤌 (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    وإلا 𐤀𐤌 (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    وإلا {
        اذهب ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    لكل (unsigned صحيح count = length - 1; count > 0; count--, shift += 6)
        buffer[count] = (حرف)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (حرف)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    أرجع length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    أرجع 0;
}

bool
is_valid_utf8(ثابت حرف *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    طالما (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        𐤀𐤌 (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        وإلا 𐤀𐤌 (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        وإلا 𐤀𐤌 (s[i] == 0xE0) {
            i++;
            𐤀𐤌 (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                أرجع false;
            tail_bytes = 1;
        }
        وإلا 𐤀𐤌 (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        وإلا 𐤀𐤌 (s[i] == 0xED) {
            i++;
            𐤀𐤌 (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                أرجع false;
            tail_bytes = 1;
        }
        وإلا 𐤀𐤌 (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        وإلا 𐤀𐤌 (s[i] == 0xF0) {
            i++;
            𐤀𐤌 (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                أرجع false;
            tail_bytes = 2;
        }
        وإلا 𐤀𐤌 (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        وإلا 𐤀𐤌 (s[i] == 0xF4) {
            i++;
            𐤀𐤌 (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                أرجع false;
            tail_bytes = 2;
        }
        وإلا {
            أرجع false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        طالما (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        𐤀𐤌 (tail_bytes != 0)
            أرجع false;
    }

    أرجع true;
}
