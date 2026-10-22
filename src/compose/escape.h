/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
സ്ഥിതം വരിയിൽ അക്ഷരം*
escape_utf8_string_literal(സ്ഥിരം അക്ഷരം *from)
{
    സ്ഥിരം size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    അക്ഷരം* to = calloc(4 * length + 1, വലുപ്പം(*to));
    എങ്കിൽ (!to)
        തിരികെ ശൂന്യ;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    വേണ്ടി (size_t f = 0; f < length;) {
        എങ്കിൽ ((unsigned അക്ഷരം) from[f] < 0x80) {
            /* ASCII */
            എങ്കിൽ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } അല്ലെങ്കിൽ എങ്കിൽ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } അല്ലെങ്കിൽ {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            തുടരുക;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        അല്ലെങ്കിൽ എങ്കിൽ ((unsigned അക്ഷരം) from[f] < 0xe0)
            nbytes = 2;
        അല്ലെങ്കിൽ എങ്കിൽ ((unsigned അക്ഷരം) from[f] < 0xf0)
            nbytes = 3;
        അല്ലെങ്കിൽ
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    അക്ഷരം* ret = realloc(to, t);
    എങ്കിൽ (!ret)
        സ്വതന്ത്രം(to);
    തിരികെ ret;
}
