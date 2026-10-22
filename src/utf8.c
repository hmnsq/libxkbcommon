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
    uint8_t اوږدتیا, سر;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    إذا (unichar <= 0x007f) {
        بفر[0] = (حرف) unichar;
        بفر[1] = '\0';
        أرجع 2;
    }
    وإلا إذا (unichar <= 0x07FF) {
        اوږدتیا = 2;
        سر = 0xc0;
    }
    /* Handle surrogates */
    وإلا إذا (is_surrogate(unichar)) {
        اذهب ill_formed_کوډ_unit_subsequence;
    }
    وإلا إذا (unichar <= 0xffff) {
        اوږدتیا = 3;
        سر = 0xe0;
    }
    وإلا إذا (unichar <= 0x10ffff) {
        اوږدتیا = 4;
        سر = 0xf0;
    }
    وإلا {
        اذهب ill_formed_کوډ_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t ښويدنه = 0;
    لكل (بینښه صحيح شمېر = اوږدتیا - 1; شمېر > 0; شمېر--, ښويدنه += 6)
        بفر[شمېر] = (حرف)(0x80 | ((unichar >> ښويدنه) & 0x3f));

    بفر[0] = (حرف)(سر | ((unichar >> ښويدنه) & 0x3f));
    بفر[اوږدتیا] = '\0';

    أرجع اوږدتیا + 1;

ill_formed_کوډ_unit_subsequence:
    بفر[0] = '\0';
    أرجع 0;
}

bool
is_valid_utf8(ثابت حرف *ss, اندازه_t len)
{
    اندازه_t i = 0;
    اندازه_t لکۍ_بایټونه = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    طالما (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        إذا (s[i] <= 0x7F) {
            لکۍ_بایټونه = 0;
        }
        وإلا إذا (s[i] >= 0xC2 && s[i] <= 0xDF) {
            لکۍ_بایټونه = 1;
        }
        وإلا إذا (s[i] == 0xE0) {
            i++;
            إذا (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                أرجع false;
            لکۍ_بایټونه = 1;
        }
        وإلا إذا (s[i] >= 0xE1 && s[i] <= 0xEC) {
            لکۍ_بایټونه = 2;
        }
        وإلا إذا (s[i] == 0xED) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                أرجع false;
            لکۍ_بایټونه = 1;
        }
        وإلا إذا (s[i] >= 0xEE && s[i] <= 0xEF) {
            لکۍ_بایټونه = 2;
        }
        وإلا إذا (s[i] == 0xF0) {
            i++;
            إذا (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                أرجع false;
            لکۍ_بایټونه = 2;
        }
        وإلا إذا (s[i] >= 0xF1 && s[i] <= 0xF3) {
            لکۍ_بایټونه = 3;
        }
        وإلا إذا (s[i] == 0xF4) {
            i++;
            إذا (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                أرجع false;
            لکۍ_بایټونه = 2;
        }
        وإلا {
            أرجع false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        طالما (i < len && لکۍ_بایټونه > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            لکۍ_بایټونه--;
        }

        إذا (لکۍ_بایټونه != 0)
            أرجع false;
    }

    أرجع true;
}
