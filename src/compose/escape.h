/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
स्थिर अन्तःस्थ वर्ण*
escape_utf8_string_literal(ध्रुव वर्ण *from)
{
    ध्रुव आकार_t दीर्घता = वाक्यदैर्घ्यम्(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    वर्ण* गन्तव्यम् = शून्यस्मृतिं_देहि(4 * दीर्घता + 1, परिमाण(*गन्तव्यम्));
    यदि (!गन्तव्यम्)
        प्रत्यागम रिक्त;

    आकार_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    प्रति (आकार_t f = 0; f < दीर्घता;) {
        यदि ((अचिह्नित वर्ण) from[f] < 0x80) {
            /* ASCII */
            यदि (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&गन्तव्यम्[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } अन्यथा यदि (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&गन्तव्यम्[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } अन्यथा {
                /* Other characters */
                गन्तव्यम्[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            अनुवर्त;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        अन्यथा यदि ((अचिह्नित वर्ण) from[f] < 0xe0)
            nbytes = 2;
        अन्यथा यदि ((अचिह्नित वर्ण) from[f] < 0xf0)
            nbytes = 3;
        अन्यथा
            nbytes = 4;
        स्मृतिं_प्रतिलिख(&गन्तव्यम्[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    गन्तव्यम्[t++] = '\0';
    वर्ण* ret = realloc(गन्तव्यम्, t);
    यदि (!ret)
        मुक्त(गन्तव्यम्);
    प्रत्यागम ret;
}
