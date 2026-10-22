/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#include "config.h"

#include "utils.h"
#include "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, var_na *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ᬬᬾᬦ᭄ (unichar <= 0x007f) {
        buffer[0] = (var_na) unichar;
        buffer[1] = '\0';
        ᬯᬮᬶ 2;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    ᬮᬶᬬ ᬬᬾᬦ᭄ (is_surrogate(unichar)) {
        goto ill_formed_code_unit_subsequence;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    ᬮᬶᬬ {
        goto ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    for (unsigned puur_naa_nka count = length - 1; count > 0; count--, shift += 6)
        buffer[count] = (var_na)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (var_na)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    ᬯᬮᬶ length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    ᬯᬮᬶ 0;
}

bool
is_valid_utf8(const var_na *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    const uint8_t *s = (const uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ᬲᭂᬤᭂᬮ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ᬬᬾᬦ᭄ (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] == 0xE0) {
            i++;
            ᬬᬾᬦ᭄ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ᬯᬮᬶ false;
            tail_bytes = 1;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] == 0xED) {
            i++;
            ᬬᬾᬦ᭄ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ᬯᬮᬶ false;
            tail_bytes = 1;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] == 0xF0) {
            i++;
            ᬬᬾᬦ᭄ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ᬯᬮᬶ false;
            tail_bytes = 2;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (s[i] == 0xF4) {
            i++;
            ᬬᬾᬦ᭄ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ᬯᬮᬶ false;
            tail_bytes = 2;
        }
        ᬮᬶᬬ {
            ᬯᬮᬶ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ᬲᭂᬤᭂᬮ (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        ᬬᬾᬦ᭄ (tail_bytes != 0)
            ᬯᬮᬶ false;
    }

    ᬯᬮᬶ true;
}
