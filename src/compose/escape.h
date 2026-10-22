/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
நிலையான உள்வரி எழுத்து*
escape_utf8_string_literal(மாறிலி எழுத்து *from)
{
    மாறிலி size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    எழுத்து* to = calloc(4 * length + 1, அளவு(*to));
    எனில் (!to)
        திரும்பு வெற்று;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    க்கு (size_t f = 0; f < length;) {
        எனில் ((unsigned எழுத்து) from[f] < 0x80) {
            /* ASCII */
            எனில் (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } இல்லையெனில் எனில் (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } இல்லையெனில் {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            தொடர்;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        இல்லையெனில் எனில் ((unsigned எழுத்து) from[f] < 0xe0)
            nbytes = 2;
        இல்லையெனில் எனில் ((unsigned எழுத்து) from[f] < 0xf0)
            nbytes = 3;
        இல்லையெனில்
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    எழுத்து* ret = realloc(to, t);
    எனில் (!ret)
        விடுவி(to);
    திரும்பு ret;
}
