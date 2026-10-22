/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ضمن "config.h"

#ضمن "utils.h"
#ضمن "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_منزل_utf8(uint32_t unichar, حرف *بفر)
{
    uint8_t ڊيگهائي, سر;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    إذا (unichar <= 0x007f) {
        بفر[0] = (حرف) unichar;
        بفر[1] = '\0';
        أرجع 2;
    }
    وإلا إذا (unichar <= 0x07FF) {
        ڊيگهائي = 2;
        سر = 0xc0;
    }
    /* Handle surrogates */
    وإلا إذا (is_surrogate(unichar)) {
        اذهب ill_formed_ڪوڊ_unit_subsequence;
    }
    وإلا إذا (unichar <= 0xffff) {
        ڊيگهائي = 3;
        سر = 0xe0;
    }
    وإلا إذا (unichar <= 0x10ffff) {
        ڊيگهائي = 4;
        سر = 0xf0;
    }
    وإلا {
        اذهب ill_formed_ڪوڊ_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t سرڪايو = 0;
    لكل (بنانشان صحيح ڳڻپ = ڊيگهائي - 1; ڳڻپ > 0; ڳڻپ--, سرڪايو += 6)
        بفر[ڳڻپ] = (حرف)(0x80 | ((unichar >> سرڪايو) & 0x3f));

    بفر[0] = (حرف)(سر | ((unichar >> سرڪايو) & 0x3f));
    بفر[ڊيگهائي] = '\0';

    أرجع ڊيگهائي + 1;

ill_formed_ڪوڊ_unit_subsequence:
    بفر[0] = '\0';
    أرجع 0;
}

bool
is_valid_utf8(ثابت حرف *ss, سائيز_t len)
{
    سائيز_t i = 0;
    سائيز_t پڇ_بائيٽون = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    طالما (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        إذا (s[i] <= 0x7F) {
            پڇ_بائيٽون = 0;
        }
        وإلا إذا (s[i] >= 0xC2 && s[i] <= 0xDF) {
            پڇ_بائيٽون = 1;
        }
        وإلا إذا (s[i] == 0xE0) {
            i++;
            إذا (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                أرجع false;
            پڇ_بائيٽون = 1;
        }
        وإلا إذا (s[i] >= 0xE1 && s[i] <= 0xEC) {
            پڇ_بائيٽون = 2;
        }
        وإلا إذا (s[i] == 0xED) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                أرجع false;
            پڇ_بائيٽون = 1;
        }
        وإلا إذا (s[i] >= 0xEE && s[i] <= 0xEF) {
            پڇ_بائيٽون = 2;
        }
        وإلا إذا (s[i] == 0xF0) {
            i++;
            إذا (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                أرجع false;
            پڇ_بائيٽون = 2;
        }
        وإلا إذا (s[i] >= 0xF1 && s[i] <= 0xF3) {
            پڇ_بائيٽون = 3;
        }
        وإلا إذا (s[i] == 0xF4) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                أرجع false;
            پڇ_بائيٽون = 2;
        }
        وإلا {
            أرجع false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        طالما (i < len && پڇ_بائيٽون > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            پڇ_بائيٽون--;
        }

        إذا (پڇ_بائيٽون != 0)
            أرجع false;
    }

    أرجع true;
}
