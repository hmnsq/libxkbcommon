/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdlib.h>

#ضمن "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ساكن مضمن حرف*
escape_utf8_string_literal(ثابت حرف *from)
{
    ثابت ناپ_t لمائی = لڑی_لمبائی(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    حرف* منزل = صفر_حافظہ_دیو(4 * لمائی + 1, حجم(*منزل));
    إذا (!منزل)
        أرجع عدم;

    ناپ_t t = 0;
    bool پچھلا_is_hex_escape = false;
    uint8_t nbytes = 0;
    لكل (ناپ_t f = 0; f < لمائی;) {
        إذا ((بنانشانی حرف) from[f] < 0x80) {
            /* ASCII */
            إذا (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && پچھلا_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_محفوظ(&منزل[t], 5, "\\x%02x", from[f]);
                t += 4;
                پچھلا_is_hex_escape = true;
            } وإلا إذا (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_محفوظ(&منزل[t], 3, "\\%c", from[f]);
                t += 2;
                پچھلا_is_hex_escape = false;
            } وإلا {
                /* Other characters */
                منزل[t++] = from[f];
                پچھلا_is_hex_escape = false;
            }
            f++;
            استمر;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        وإلا إذا ((بنانشانی حرف) from[f] < 0xe0)
            nbytes = 2;
        وإلا إذا ((بنانشانی حرف) from[f] < 0xf0)
            nbytes = 3;
        وإلا
            nbytes = 4;
        حافظہ_نقل(&منزل[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        پچھلا_is_hex_escape = false;
    }
    منزل[t++] = '\0';
    حرف* ret = realloc(منزل, t);
    إذا (!ret)
        حرر(منزل);
    أرجع ret;
}
