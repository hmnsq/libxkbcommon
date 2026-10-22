/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
స్థిర అంతఃవరుస అక్షరం*
escape_utf8_string_literal(స్థిరరాశి అక్షరం *from)
{
    స్థిరరాశి కొలత_t నిడివి = అక్షరశ్రేణి_పొడవు(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    అక్షరం* గమ్యం = సున్నా_జ్ఞాపకం_ఇవ్వు(4 * నిడివి + 1, పరిమాణం(*గమ్యం));
    ఒకవేళ (!గమ్యం)
        తిరిగి ఖాళీ;

    కొలత_t t = 0;
    bool మునుపటి_is_hex_escape = false;
    uint8_t nbytes = 0;
    కొరకు (కొలత_t f = 0; f < నిడివి;) {
        ఒకవేళ ((గుర్తులేని అక్షరం) from[f] < 0x80) {
            /* ASCII */
            ఒకవేళ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && మునుపటి_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_సురక్షిత(&గమ్యం[t], 5, "\\x%02x", from[f]);
                t += 4;
                మునుపటి_is_hex_escape = true;
            } లేకపోతే ఒకవేళ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_సురక్షిత(&గమ్యం[t], 3, "\\%c", from[f]);
                t += 2;
                మునుపటి_is_hex_escape = false;
            } లేకపోతే {
                /* Other characters */
                గమ్యం[t++] = from[f];
                మునుపటి_is_hex_escape = false;
            }
            f++;
            కొనసాగు;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        లేకపోతే ఒకవేళ ((గుర్తులేని అక్షరం) from[f] < 0xe0)
            nbytes = 2;
        లేకపోతే ఒకవేళ ((గుర్తులేని అక్షరం) from[f] < 0xf0)
            nbytes = 3;
        లేకపోతే
            nbytes = 4;
        జ్ఞాపకం_నకలు(&గమ్యం[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        మునుపటి_is_hex_escape = false;
    }
    గమ్యం[t++] = '\0';
    అక్షరం* ret = realloc(గమ్యం, t);
    ఒకవేళ (!ret)
        విడుదల(గమ్యం);
    తిరిగి ret;
}
