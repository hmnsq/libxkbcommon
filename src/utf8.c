/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_ಗಮ್ಯ_utf8(uint32_t unichar, ಅಕ್ಷರ *ಬಫರ್)
{
    uint8_t ಉದ್ದಳತೆ, ತಲೆ;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ಆದರೆ (unichar <= 0x007f) {
        ಬಫರ್[0] = (ಅಕ್ಷರ) unichar;
        ಬಫರ್[1] = '\0';
        ಹಿಂತಿರುಗಿ 2;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unichar <= 0x07FF) {
        ಉದ್ದಳತೆ = 2;
        ತಲೆ = 0xc0;
    }
    /* Handle surrogates */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_surrogate(unichar)) {
        ಹೋಗು ill_formed_ಸಂಕೇತ_unit_subsequence;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unichar <= 0xffff) {
        ಉದ್ದಳತೆ = 3;
        ತಲೆ = 0xe0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unichar <= 0x10ffff) {
        ಉದ್ದಳತೆ = 4;
        ತಲೆ = 0xf0;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಹೋಗು ill_formed_ಸಂಕೇತ_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t ಸರಿಸು = 0;
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = ಉದ್ದಳತೆ - 1; ಎಣಿಕೆ_2 > 0; ಎಣಿಕೆ_2--, ಸರಿಸು += 6)
        ಬಫರ್[ಎಣಿಕೆ_2] = (ಅಕ್ಷರ)(0x80 | ((unichar >> ಸರಿಸು) & 0x3f));

    ಬಫರ್[0] = (ಅಕ್ಷರ)(ತಲೆ | ((unichar >> ಸರಿಸು) & 0x3f));
    ಬಫರ್[ಉದ್ದಳತೆ] = '\0';

    ಹಿಂತಿರುಗಿ ಉದ್ದಳತೆ + 1;

ill_formed_ಸಂಕೇತ_unit_subsequence:
    ಬಫರ್[0] = '\0';
    ಹಿಂತಿರುಗಿ 0;
}

bool
is_valid_utf8(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ss, ಗಾತ್ರ_t len)
{
    ಗಾತ್ರ_t i = 0;
    ಗಾತ್ರ_t ಬಾಲ_ಬೈಟುಗಳು = 0;
    ಸ್ಥಿರಾಂಕ uint8_t *s = (ಸ್ಥಿರಾಂಕ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ಆಗ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ಆದರೆ (s[i] <= 0x7F) {
            ಬಾಲ_ಬೈಟುಗಳು = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            ಬಾಲ_ಬೈಟುಗಳು = 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] == 0xE0) {
            i++;
            ಆದರೆ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ಹಿಂತಿರುಗಿ false;
            ಬಾಲ_ಬೈಟುಗಳು = 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            ಬಾಲ_ಬೈಟುಗಳು = 2;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] == 0xED) {
            i++;
            ಆದರೆ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ಹಿಂತಿರುಗಿ false;
            ಬಾಲ_ಬೈಟುಗಳು = 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] >= 0xEE && s[i] <= 0xEF) {
            ಬಾಲ_ಬೈಟುಗಳು = 2;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] == 0xF0) {
            i++;
            ಆದರೆ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ಹಿಂತಿರುಗಿ false;
            ಬಾಲ_ಬೈಟುಗಳು = 2;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            ಬಾಲ_ಬೈಟುಗಳು = 3;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s[i] == 0xF4) {
            i++;
            ಆದರೆ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ಹಿಂತಿರುಗಿ false;
            ಬಾಲ_ಬೈಟುಗಳು = 2;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ಆಗ (i < len && ಬಾಲ_ಬೈಟುಗಳು > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            ಬಾಲ_ಬೈಟುಗಳು--;
        }

        ಆದರೆ (ಬಾಲ_ಬೈಟುಗಳು != 0)
            ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}
