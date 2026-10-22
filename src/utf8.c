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
    uint8_t لمائی, سر;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    إذا (unichar <= 0x007f) {
        بفر[0] = (حرف) unichar;
        بفر[1] = '\0';
        أرجع 2;
    }
    وإلا إذا (unichar <= 0x07FF) {
        لمائی = 2;
        سر = 0xc0;
    }
    /* Handle surrogates */
    وإلا إذا (is_surrogate(unichar)) {
        اذهب ill_formed_کوڈ_unit_subsequence;
    }
    وإلا إذا (unichar <= 0xffff) {
        لمائی = 3;
        سر = 0xe0;
    }
    وإلا إذا (unichar <= 0x10ffff) {
        لمائی = 4;
        سر = 0xf0;
    }
    وإلا {
        اذهب ill_formed_کوڈ_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t کھسکاؤ = 0;
    لكل (بنانشانی صحيح گنتی = لمائی - 1; گنتی > 0; گنتی--, کھسکاؤ += 6)
        بفر[گنتی] = (حرف)(0x80 | ((unichar >> کھسکاؤ) & 0x3f));

    بفر[0] = (حرف)(سر | ((unichar >> کھسکاؤ) & 0x3f));
    بفر[لمائی] = '\0';

    أرجع لمائی + 1;

ill_formed_کوڈ_unit_subsequence:
    بفر[0] = '\0';
    أرجع 0;
}

bool
is_valid_utf8(ثابت حرف *ss, ناپ_t len)
{
    ناپ_t i = 0;
    ناپ_t پوچھ_بائٹاں = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    طالما (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        إذا (s[i] <= 0x7F) {
            پوچھ_بائٹاں = 0;
        }
        وإلا إذا (s[i] >= 0xC2 && s[i] <= 0xDF) {
            پوچھ_بائٹاں = 1;
        }
        وإلا إذا (s[i] == 0xE0) {
            i++;
            إذا (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                أرجع false;
            پوچھ_بائٹاں = 1;
        }
        وإلا إذا (s[i] >= 0xE1 && s[i] <= 0xEC) {
            پوچھ_بائٹاں = 2;
        }
        وإلا إذا (s[i] == 0xED) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                أرجع false;
            پوچھ_بائٹاں = 1;
        }
        وإلا إذا (s[i] >= 0xEE && s[i] <= 0xEF) {
            پوچھ_بائٹاں = 2;
        }
        وإلا إذا (s[i] == 0xF0) {
            i++;
            إذا (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                أرجع false;
            پوچھ_بائٹاں = 2;
        }
        وإلا إذا (s[i] >= 0xF1 && s[i] <= 0xF3) {
            پوچھ_بائٹاں = 3;
        }
        وإلا إذا (s[i] == 0xF4) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                أرجع false;
            پوچھ_بائٹاں = 2;
        }
        وإلا {
            أرجع false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        طالما (i < len && پوچھ_بائٹاں > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            پوچھ_بائٹاں--;
        }

        إذا (پوچھ_بائٹاں != 0)
            أرجع false;
    }

    أرجع true;
}
