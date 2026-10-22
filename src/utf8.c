/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, எழுத்து *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    எனில் (unichar <= 0x007f) {
        buffer[0] = (எழுத்து) unichar;
        buffer[1] = '\0';
        திரும்பு 2;
    }
    இல்லையெனில் எனில் (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    இல்லையெனில் எனில் (is_surrogate(unichar)) {
        செல் ill_formed_code_unit_subsequence;
    }
    இல்லையெனில் எனில் (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    இல்லையெனில் எனில் (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    இல்லையெனில் {
        செல் ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    க்கு (unsigned முழு மொத்தம் = length - 1; மொத்தம் > 0; மொத்தம்--, shift += 6)
        buffer[மொத்தம்] = (எழுத்து)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (எழுத்து)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    திரும்பு length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    திரும்பு 0;
}

bool
is_valid_utf8(மாறிலி எழுத்து *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    மாறிலி uint8_t *s = (மாறிலி uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    போது (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        எனில் (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        இல்லையெனில் எனில் (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        இல்லையெனில் எனில் (s[i] == 0xE0) {
            i++;
            எனில் (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                திரும்பு false;
            tail_bytes = 1;
        }
        இல்லையெனில் எனில் (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        இல்லையெனில் எனில் (s[i] == 0xED) {
            i++;
            எனில் (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                திரும்பு false;
            tail_bytes = 1;
        }
        இல்லையெனில் எனில் (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        இல்லையெனில் எனில் (s[i] == 0xF0) {
            i++;
            எனில் (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                திரும்பு false;
            tail_bytes = 2;
        }
        இல்லையெனில் எனில் (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        இல்லையெனில் எனில் (s[i] == 0xF4) {
            i++;
            எனில் (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                திரும்பு false;
            tail_bytes = 2;
        }
        இல்லையெனில் {
            திரும்பு false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        போது (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        எனில் (tail_bytes != 0)
            திரும்பு false;
    }

    திரும்பு true;
}
