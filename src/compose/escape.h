/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stdlib.h>

#include "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
static anta_hstha var_na*
escape_utf8_string_literal(const var_na *from)
{
    const size_t length = strlen(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    var_na* to = calloc(4 * length + 1, parimaa_na(*to));
    အကယ်၍ (!to)
        ပြန်ပေး NULL;

    size_t t = 0;
    bool previous_is_hex_escape = false;
    uint8_t nbytes = 0;
    အတွက် (size_t f = 0; f < length;) {
        အကယ်၍ ((unsigned var_na) from[f] < 0x80) {
            /* ASCII */
            အကယ်၍ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && previous_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_safe(&to[t], 5, "\\x%02x", from[f]);
                t += 4;
                previous_is_hex_escape = true;
            } မဟုတ်ပါက အကယ်၍ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_safe(&to[t], 3, "\\%c", from[f]);
                t += 2;
                previous_is_hex_escape = false;
            } မဟုတ်ပါက {
                /* Other characters */
                to[t++] = from[f];
                previous_is_hex_escape = false;
            }
            f++;
            continue;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        မဟုတ်ပါက အကယ်၍ ((unsigned var_na) from[f] < 0xe0)
            nbytes = 2;
        မဟုတ်ပါက အကယ်၍ ((unsigned var_na) from[f] < 0xf0)
            nbytes = 3;
        မဟုတ်ပါက
            nbytes = 4;
        memcpy(&to[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        previous_is_hex_escape = false;
    }
    to[t++] = '\0';
    var_na* ret = realloc(to, t);
    အကယ်၍ (!ret)
        free(to);
    ပြန်ပေး ret;
}
