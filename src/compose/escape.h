/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdlib.h>

#شامل "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ساکن اندرونی حرف*
escape_utf8_string_literal(ثابت حرف *from)
{
    ثابت سائز_t لمبائی = لڑی_لمبائی(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    حرف* مقصد = صفر_حافظہ_دو(4 * لمبائی + 1, حجم(*مقصد));
    اگر (!مقصد)
        واپس تہی;

    سائز_t t = 0;
    bool پچھلا_is_hex_escape = false;
    uint8_t nbytes = 0;
    برائے (سائز_t f = 0; f < لمبائی;) {
        اگر ((غیرعلامتی حرف) from[f] < 0x80) {
            /* ASCII */
            اگر (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && پچھلا_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_محفوظ(&مقصد[t], 5, "\\x%02x", from[f]);
                t += 4;
                پچھلا_is_hex_escape = true;
            } ورنہ اگر (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_محفوظ(&مقصد[t], 3, "\\%c", from[f]);
                t += 2;
                پچھلا_is_hex_escape = false;
            } ورنہ {
                /* Other characters */
                مقصد[t++] = from[f];
                پچھلا_is_hex_escape = false;
            }
            f++;
            جاری;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        ورنہ اگر ((غیرعلامتی حرف) from[f] < 0xe0)
            nbytes = 2;
        ورنہ اگر ((غیرعلامتی حرف) from[f] < 0xf0)
            nbytes = 3;
        ورنہ
            nbytes = 4;
        حافظہ_نقل(&مقصد[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        پچھلا_is_hex_escape = false;
    }
    مقصد[t++] = '\0';
    حرف* ret = realloc(مقصد, t);
    اگر (!ret)
        آزاد(مقصد);
    واپس ret;
}
