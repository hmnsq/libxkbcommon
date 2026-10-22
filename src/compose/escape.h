/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ*
escape_utf8_string_literal(ਅਚਲ ਅੱਖਰ *from)
{
    ਅਚਲ ਮਾਪ_t ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    ਅੱਖਰ* ਮੰਜ਼ਿਲ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(4 * ਲੰਮਾਈ + 1, ਆਕਾਰ(*ਮੰਜ਼ਿਲ));
    ਜੇ (!ਮੰਜ਼ਿਲ)
        ਵਾਪਸ ਨਿਰਾ;

    ਮਾਪ_t t = 0;
    bool ਪਿਛਲਾ_is_hex_escape = false;
    uint8_t nbytes = 0;
    ਲਈ (ਮਾਪ_t f = 0; f < ਲੰਮਾਈ;) {
        ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) from[f] < 0x80) {
            /* ASCII */
            ਜੇ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && ਪਿਛਲਾ_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_ਸੁਰੱਖਿਅਤ(&ਮੰਜ਼ਿਲ[t], 5, "\\x%02x", from[f]);
                t += 4;
                ਪਿਛਲਾ_is_hex_escape = true;
            } ਨਹੀਂਤਾਂ ਜੇ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_ਸੁਰੱਖਿਅਤ(&ਮੰਜ਼ਿਲ[t], 3, "\\%c", from[f]);
                t += 2;
                ਪਿਛਲਾ_is_hex_escape = false;
            } ਨਹੀਂਤਾਂ {
                /* Other characters */
                ਮੰਜ਼ਿਲ[t++] = from[f];
                ਪਿਛਲਾ_is_hex_escape = false;
            }
            f++;
            ਜਾਰੀ;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        ਨਹੀਂਤਾਂ ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) from[f] < 0xe0)
            nbytes = 2;
        ਨਹੀਂਤਾਂ ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) from[f] < 0xf0)
            nbytes = 3;
        ਨਹੀਂਤਾਂ
            nbytes = 4;
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ਮੰਜ਼ਿਲ[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        ਪਿਛਲਾ_is_hex_escape = false;
    }
    ਮੰਜ਼ਿਲ[t++] = '\0';
    ਅੱਖਰ* ret = realloc(ਮੰਜ਼ਿਲ, t);
    ਜੇ (!ret)
        ਮੁਕਤ(ਮੰਜ਼ਿਲ);
    ਵਾਪਸ ret;
}
