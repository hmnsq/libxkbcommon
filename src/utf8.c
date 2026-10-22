/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#شامل "config.h"

#شامل "utils.h"
#شامل "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_مقصد_utf8(uint32_t unichar, حرف *بفر)
{
    uint8_t لمبائی, سر;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    اگر (unichar <= 0x007f) {
        بفر[0] = (حرف) unichar;
        بفر[1] = '\0';
        واپس 2;
    }
    ورنہ اگر (unichar <= 0x07FF) {
        لمبائی = 2;
        سر = 0xc0;
    }
    /* Handle surrogates */
    ورنہ اگر (is_surrogate(unichar)) {
        جاؤ ill_formed_رمز_unit_subsequence;
    }
    ورنہ اگر (unichar <= 0xffff) {
        لمبائی = 3;
        سر = 0xe0;
    }
    ورنہ اگر (unichar <= 0x10ffff) {
        لمبائی = 4;
        سر = 0xf0;
    }
    ورنہ {
        جاؤ ill_formed_رمز_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t سرکاؤ = 0;
    برائے (غیرعلامتی صحیح گنتی = لمبائی - 1; گنتی > 0; گنتی--, سرکاؤ += 6)
        بفر[گنتی] = (حرف)(0x80 | ((unichar >> سرکاؤ) & 0x3f));

    بفر[0] = (حرف)(سر | ((unichar >> سرکاؤ) & 0x3f));
    بفر[لمبائی] = '\0';

    واپس لمبائی + 1;

ill_formed_رمز_unit_subsequence:
    بفر[0] = '\0';
    واپس 0;
}

bool
is_valid_utf8(ثابت حرف *ss, سائز_t len)
{
    سائز_t i = 0;
    سائز_t دم_بائٹیں = 0;
    ثابت uint8_t *s = (ثابت uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    جب (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        اگر (s[i] <= 0x7F) {
            دم_بائٹیں = 0;
        }
        ورنہ اگر (s[i] >= 0xC2 && s[i] <= 0xDF) {
            دم_بائٹیں = 1;
        }
        ورنہ اگر (s[i] == 0xE0) {
            i++;
            اگر (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                واپس false;
            دم_بائٹیں = 1;
        }
        ورنہ اگر (s[i] >= 0xE1 && s[i] <= 0xEC) {
            دم_بائٹیں = 2;
        }
        ورنہ اگر (s[i] == 0xED) {
            i++;
            اگر (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                واپس false;
            دم_بائٹیں = 1;
        }
        ورنہ اگر (s[i] >= 0xEE && s[i] <= 0xEF) {
            دم_بائٹیں = 2;
        }
        ورنہ اگر (s[i] == 0xF0) {
            i++;
            اگر (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                واپس false;
            دم_بائٹیں = 2;
        }
        ورنہ اگر (s[i] >= 0xF1 && s[i] <= 0xF3) {
            دم_بائٹیں = 3;
        }
        ورنہ اگر (s[i] == 0xF4) {
            i++;
            اگر (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                واپس false;
            دم_بائٹیں = 2;
        }
        ورنہ {
            واپس false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        جب (i < len && دم_بائٹیں > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            دم_بائٹیں--;
        }

        اگر (دم_بائٹیں != 0)
            واپس false;
    }

    واپس true;
}
