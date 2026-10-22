/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର*
escape_utf8_string_literal(ଅଚଳ ଅକ୍ଷର *from)
{
    ଅଚଳ size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    ଅକ୍ଷର* to = calloc(4 * length + 1, ଆକାର(*to));
    ଯଦି (!to)
        ଫେରନ୍ତୁ ଖାଲି;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    ପାଇଁ (size_t f = 0; f < length;) {
        ଯଦି ((unsigned ଅକ୍ଷର) from[f] < 0x80) {
            /* ASCII */
            ଯଦି (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } ଅନ୍ୟଥା ଯଦି (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } ଅନ୍ୟଥା {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            ଜାରି;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        ଅନ୍ୟଥା ଯଦି ((unsigned ଅକ୍ଷର) from[f] < 0xe0)
            nbytes = 2;
        ଅନ୍ୟଥା ଯଦି ((unsigned ଅକ୍ଷର) from[f] < 0xf0)
            nbytes = 3;
        ଅନ୍ୟଥା
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    ଅକ୍ଷର* ret = realloc(to, t);
    ଯଦି (!ret)
        ମୁକ୍ତ(to);
    ଫେରନ୍ତୁ ret;
}
