/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ*
escape_utf8_string_literal(ಧ್ರುವ ವರ್ಣ *from)
{
    ಧ್ರುವ size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    ವರ್ಣ* to = calloc(4 * length + 1, ಪರಿಮಾಣ(*to));
    ಯದಿ (!to)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    ಪ್ರತಿ (size_t f = 0; f < length;) {
        ಯದಿ ((unsigned ವರ್ಣ) from[f] < 0x80) {
            /* ASCII */
            ಯದಿ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } ಅನ್ಯಥಾ ಯದಿ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } ಅನ್ಯಥಾ {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            ಅನುವರ್ತ;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        ಅನ್ಯಥಾ ಯದಿ ((unsigned ವರ್ಣ) from[f] < 0xe0)
            nbytes = 2;
        ಅನ್ಯಥಾ ಯದಿ ((unsigned ವರ್ಣ) from[f] < 0xf0)
            nbytes = 3;
        ಅನ್ಯಥಾ
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    ವರ್ಣ* ret = realloc(to, t);
    ಯದಿ (!ret)
        ಮುಕ್ತ(to);
    ಪ್ರತ್ಯಾಗಮ ret;
}
