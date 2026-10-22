/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdlib.h>

#অন্তর্ভাৱ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
স্থির অন্তঃস্থ ৱর্ণ*
escape_utf8_string_literal(ধ্রুৱ ৱর্ণ *from)
{
    ধ্রুৱ size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    ৱর্ণ* to = calloc(4 * length + 1, পরিমাণ(*to));
    যদি (!to)
        ঘূৰাই রিক্ত;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    বাবে (size_t f = 0; f < length;) {
        যদি ((unsigned ৱর্ণ) from[f] < 0x80) {
            /* ASCII */
            যদি (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } অন্যথা যদি (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } অন্যথা {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            অনুৱর্ত;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        অন্যথা যদি ((unsigned ৱর্ণ) from[f] < 0xe0)
            nbytes = 2;
        অন্যথা যদি ((unsigned ৱর্ণ) from[f] < 0xf0)
            nbytes = 3;
        অন্যথা
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    ৱর্ণ* ret = realloc(to, t);
    যদি (!ret)
        মুক্ত(to);
    ঘূৰাই ret;
}
