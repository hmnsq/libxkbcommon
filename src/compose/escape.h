/*
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "src/utils.h"

/* Ad-hoc escaping for UTF-8 string
 *
 * Note that it only escapes the strict minimum to get a valid Compose file.
 * It also escapes hexadecimal digits after an hexadecimal escape. This is not
 * strictly needed by the current implementation: "\x0abcg" parses as "␊bcg",
 * but better be cautious than sorry and produce "\x0a\x62\x63g" instead.
 * In the latter string there is no ambiguity and no need to know the maximum
 * number of digits supported by the escape sequence.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ*
escape_utf8_string_literal(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from)
{
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(from);
    /* Longest escape is converting ASCII character to "\xNN" */
    ಅಕ್ಷರ* ಗಮ್ಯ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(4 * ಉದ್ದಳತೆ + 1, ಗಾತ್ರ(*ಗಮ್ಯ));
    ಆದರೆ (!ಗಮ್ಯ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಗಾತ್ರ_t t = 0;
    bool ಹಿಂದಿನ_is_hex_escape = false;
    uint8_t nbytes = 0;
    ಗಾಗಿ (ಗಾತ್ರ_t f = 0; f < ಉದ್ದಳತೆ;) {
        ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) from[f] < 0x80) {
            /* ASCII */
            ಆದರೆ (from[f] <= 0x10 || from[f] == 0x7f ||
                (is_xdigit(from[f]) && ಹಿಂದಿನ_is_hex_escape))
            {
                /* Control character or
                   hexadecimal digit following an hexadecimal escape */
                snprintf_ಸುರಕ್ಷಿತ(&ಗಮ್ಯ[t], 5, "\\x%02x", from[f]);
                t += 4;
                ಹಿಂದಿನ_is_hex_escape = true;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (from[f] == '"' || from[f] == '\\') {
                /* Quote and backslash */
                snprintf_ಸುರಕ್ಷಿತ(&ಗಮ್ಯ[t], 3, "\\%c", from[f]);
                t += 2;
                ಹಿಂದಿನ_is_hex_escape = false;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Other characters */
                ಗಮ್ಯ[t++] = from[f];
                ಹಿಂದಿನ_is_hex_escape = false;
            }
            f++;
            ಮುಂದುವರಿ;
        }
        /* Test next byte for the next Unicode codepoint’s bytes count */
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) from[f] < 0xe0)
            nbytes = 2;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) from[f] < 0xf0)
            nbytes = 3;
        ಇಲ್ಲದಿದ್ದರೆ
            nbytes = 4;
        ಸ್ಮರಣೆ_ನಕಲು(&ಗಮ್ಯ[t], &from[f], nbytes);
        t += nbytes;
        f += nbytes;
        ಹಿಂದಿನ_is_hex_escape = false;
    }
    ಗಮ್ಯ[t++] = '\0';
    ಅಕ್ಷರ* ret = realloc(ಗಮ್ಯ, t);
    ಆದರೆ (!ret)
        ಬಿಡುಗಡೆ(ಗಮ್ಯ);
    ಹಿಂತಿರುಗಿ ret;
}
