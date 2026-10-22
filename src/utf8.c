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
utf32_مقصد_utf8(uint32_t unichar, حرف *میانگیر)
{
    uint8_t درازی, سر;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    إذا (unichar <= 0x007f) {
        میانگیر[0] = (حرف) unichar;
        میانگیر[1] = '\0';
        أرجع 2;
    }
    وإلا إذا (unichar <= 0x07FF) {
        درازی = 2;
        سر = 0xc0;
    }
    /* Handle surrogates */
    وإلا إذا (is_surrogate(unichar)) {
        اذهب ill_formed_کد_unit_subsequence;
    }
    وإلا إذا (unichar <= 0xffff) {
        درازی = 3;
        سر = 0xe0;
    }
    وإلا إذا (unichar <= 0x10ffff) {
        درازی = 4;
        سر = 0xf0;
    }
    وإلا {
        اذهب ill_formed_کد_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t لغزش = 0;
    لكل (بیعلامت صحيح شمار = درازی - 1; شمار > 0; شمار--, لغزش += 6)
        میانگیر[شمار] = (حرف)(0x80 | ((unichar >> لغزش) & 0x3f));

    میانگیر[0] = (حرف)(سر | ((unichar >> لغزش) & 0x3f));
    میانگیر[درازی] = '\0';

    أرجع درازی + 1;

ill_formed_کد_unit_subsequence:
    میانگیر[0] = '\0';
    أرجع 0;
}

bool
is_valid_utf8(ثابت حرف *ss, اندازه_t len)
{
    اندازه_t i = 0;
    اندازه_t دم_بایتها = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    طالما (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        إذا (s[i] <= 0x7F) {
            دم_بایتها = 0;
        }
        وإلا إذا (s[i] >= 0xC2 && s[i] <= 0xDF) {
            دم_بایتها = 1;
        }
        وإلا إذا (s[i] == 0xE0) {
            i++;
            إذا (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                أرجع false;
            دم_بایتها = 1;
        }
        وإلا إذا (s[i] >= 0xE1 && s[i] <= 0xEC) {
            دم_بایتها = 2;
        }
        وإلا إذا (s[i] == 0xED) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                أرجع false;
            دم_بایتها = 1;
        }
        وإلا إذا (s[i] >= 0xEE && s[i] <= 0xEF) {
            دم_بایتها = 2;
        }
        وإلا إذا (s[i] == 0xF0) {
            i++;
            إذا (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                أرجع false;
            دم_بایتها = 2;
        }
        وإلا إذا (s[i] >= 0xF1 && s[i] <= 0xF3) {
            دم_بایتها = 3;
        }
        وإلا إذا (s[i] == 0xF4) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                أرجع false;
            دم_بایتها = 2;
        }
        وإلا {
            أرجع false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        طالما (i < len && دم_بایتها > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            دم_بایتها--;
        }

        إذا (دم_بایتها != 0)
            أرجع false;
    }

    أرجع true;
}
