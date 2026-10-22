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
utf32_to_utf8(uint32_t unichar, char *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    якщо (unichar <= 0x007f) {
        buffer[0] = (char) unichar;
        buffer[1] = '\0';
        повернути 2;
    }
    інакше якщо (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    інакше якщо (is_surrogate(unichar)) {
        goto ill_formed_code_unit_subsequence;
    }
    інакше якщо (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    інакше якщо (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    інакше {
        goto ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    для (unsigned ціле count = length - 1; count > 0; count--, shift += 6)
        buffer[count] = (char)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (char)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    повернути length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    повернути 0;
}

bool
is_valid_utf8(const char *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    const uint8_t *s = (const uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    поки (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        якщо (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        інакше якщо (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        інакше якщо (s[i] == 0xE0) {
            i++;
            якщо (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                повернути false;
            tail_bytes = 1;
        }
        інакше якщо (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        інакше якщо (s[i] == 0xED) {
            i++;
            якщо (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                повернути false;
            tail_bytes = 1;
        }
        інакше якщо (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        інакше якщо (s[i] == 0xF0) {
            i++;
            якщо (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                повернути false;
            tail_bytes = 2;
        }
        інакше якщо (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        інакше якщо (s[i] == 0xF4) {
            i++;
            якщо (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                повернути false;
            tail_bytes = 2;
        }
        інакше {
            повернути false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        поки (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        якщо (tail_bytes != 0)
            повернути false;
    }

    повернути true;
}
