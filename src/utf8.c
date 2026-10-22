/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, અક્ષર *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    જો (unichar <= 0x007f) {
        buffer[0] = (અક્ષર) unichar;
        buffer[1] = '\0';
        પાછા 2;
    }
    નહીંતર જો (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    નહીંતર જો (is_surrogate(unichar)) {
        જાઓ ill_formed_code_unit_subsequence;
    }
    નહીંતર જો (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    નહીંતર જો (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    નહીંતર {
        જાઓ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    માટે (unsigned પૂર્ણાંક ગણતરી = length - 1; ગણતરી > 0; ગણતરી--, shift += 6)
        buffer[ગણતરી] = (અક્ષર)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (અક્ષર)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    પાછા length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    પાછા 0;
}

bool
is_valid_utf8(અચલ અક્ષર *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    અચલ uint8_t *s = (અચલ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    જ્યારે (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        જો (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        નહીંતર જો (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        નહીંતર જો (s[i] == 0xE0) {
            i++;
            જો (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                પાછા false;
            tail_bytes = 1;
        }
        નહીંતર જો (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        નહીંતર જો (s[i] == 0xED) {
            i++;
            જો (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                પાછા false;
            tail_bytes = 1;
        }
        નહીંતર જો (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        નહીંતર જો (s[i] == 0xF0) {
            i++;
            જો (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                પાછા false;
            tail_bytes = 2;
        }
        નહીંતર જો (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        નહીંતર જો (s[i] == 0xF4) {
            i++;
            જો (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                પાછા false;
            tail_bytes = 2;
        }
        નહીંતર {
            પાછા false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        જ્યારે (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        જો (tail_bytes != 0)
            પાછા false;
    }

    પાછા true;
}
