/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdlib.h>

#અન્તર્ભાવ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
સ્થિર અંતઃરેખ અક્ષર*
escape_utf8_string_literal(અચલ અક્ષર *from)
{
    અચલ size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    અક્ષર* to = calloc(4 * length + 1, કદ(*to));
    જો (!to)
        પાછા ખાલી;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    માટે (size_t f = 0; f < length;) {
        જો ((unsigned અક્ષર) from[f] < 0x80) {
            /* ASCII */
            જો (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } નહીંતર જો (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } નહીંતર {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            ચાલુ;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        નહીંતર જો ((unsigned અક્ષર) from[f] < 0xe0)
            nbytes = 2;
        નહીંતર જો ((unsigned અક્ષર) from[f] < 0xf0)
            nbytes = 3;
        નહીંતર
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    અક્ષર* ret = realloc(to, t);
    જો (!ret)
        મુક્ત(to);
    પાછા ret;
}
