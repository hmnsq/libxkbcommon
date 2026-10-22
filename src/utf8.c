/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, അക്ഷരം *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    എങ്കിൽ (unichar <= 0x007f) {
        buffer[0] = (അക്ഷരം) unichar;
        buffer[1] = '\0';
        തിരികെ 2;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    അല്ലെങ്കിൽ എങ്കിൽ (is_surrogate(unichar)) {
        പോകുക ill_formed_code_unit_subsequence;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    അല്ലെങ്കിൽ {
        പോകുക ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ എണ്ണം = length - 1; എണ്ണം > 0; എണ്ണം--, shift += 6)
        buffer[എണ്ണം] = (അക്ഷരം)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (അക്ഷരം)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    തിരികെ length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    തിരികെ 0;
}

bool
is_valid_utf8(സ്ഥിരം അക്ഷരം *ss, size_t len)
{
    size_t i = 0;
    size_t tail_bytes = 0;
    സ്ഥിരം uint8_t *s = (സ്ഥിരം uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ആകുമ്പോൾ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        എങ്കിൽ (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] == 0xE0) {
            i++;
            എങ്കിൽ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                തിരികെ false;
            tail_bytes = 1;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] == 0xED) {
            i++;
            എങ്കിൽ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                തിരികെ false;
            tail_bytes = 1;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] == 0xF0) {
            i++;
            എങ്കിൽ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                തിരികെ false;
            tail_bytes = 2;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s[i] == 0xF4) {
            i++;
            എങ്കിൽ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                തിരികെ false;
            tail_bytes = 2;
        }
        അല്ലെങ്കിൽ {
            തിരികെ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ആകുമ്പോൾ (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        എങ്കിൽ (tail_bytes != 0)
            തിരികെ false;
    }

    തിരികെ true;
}
