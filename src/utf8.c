/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_ਮੰਜ਼ਿਲ_utf8(uint32_t unichar, ਅੱਖਰ *ਬਫਰ)
{
    uint8_t ਲੰਮਾਈ, ਸਿਰ;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ਜੇ (unichar <= 0x007f) {
        ਬਫਰ[0] = (ਅੱਖਰ) unichar;
        ਬਫਰ[1] = '\0';
        ਵਾਪਸ 2;
    }
    ਨਹੀਂਤਾਂ ਜੇ (unichar <= 0x07FF) {
        ਲੰਮਾਈ = 2;
        ਸਿਰ = 0xc0;
    }
    /* Handle surrogates */
    ਨਹੀਂਤਾਂ ਜੇ (is_surrogate(unichar)) {
        ਜਾਓ ill_formed_ਕੋਡ_unit_subsequence;
    }
    ਨਹੀਂਤਾਂ ਜੇ (unichar <= 0xffff) {
        ਲੰਮਾਈ = 3;
        ਸਿਰ = 0xe0;
    }
    ਨਹੀਂਤਾਂ ਜੇ (unichar <= 0x10ffff) {
        ਲੰਮਾਈ = 4;
        ਸਿਰ = 0xf0;
    }
    ਨਹੀਂਤਾਂ {
        ਜਾਓ ill_formed_ਕੋਡ_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t ਖਿਸਕਾਓ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਲੰਮਾਈ - 1; ਗਿਣਤੀ_2 > 0; ਗਿਣਤੀ_2--, ਖਿਸਕਾਓ += 6)
        ਬਫਰ[ਗਿਣਤੀ_2] = (ਅੱਖਰ)(0x80 | ((unichar >> ਖਿਸਕਾਓ) & 0x3f));

    ਬਫਰ[0] = (ਅੱਖਰ)(ਸਿਰ | ((unichar >> ਖਿਸਕਾਓ) & 0x3f));
    ਬਫਰ[ਲੰਮਾਈ] = '\0';

    ਵਾਪਸ ਲੰਮਾਈ + 1;

ill_formed_ਕੋਡ_unit_subsequence:
    ਬਫਰ[0] = '\0';
    ਵਾਪਸ 0;
}

bool
is_valid_utf8(ਅਚਲ ਅੱਖਰ *ss, ਮਾਪ_t len)
{
    ਮਾਪ_t i = 0;
    ਮਾਪ_t ਪੂਛ_ਬਾਈਟਾਂ = 0;
    ਅਚਲ uint8_t *s = (ਅਚਲ uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    ਜਦੋਂ (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ਜੇ (s[i] <= 0x7F) {
            ਪੂਛ_ਬਾਈਟਾਂ = 0;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            ਪੂਛ_ਬਾਈਟਾਂ = 1;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] == 0xE0) {
            i++;
            ਜੇ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ਵਾਪਸ false;
            ਪੂਛ_ਬਾਈਟਾਂ = 1;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            ਪੂਛ_ਬਾਈਟਾਂ = 2;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] == 0xED) {
            i++;
            ਜੇ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ਵਾਪਸ false;
            ਪੂਛ_ਬਾਈਟਾਂ = 1;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] >= 0xEE && s[i] <= 0xEF) {
            ਪੂਛ_ਬਾਈਟਾਂ = 2;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] == 0xF0) {
            i++;
            ਜੇ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ਵਾਪਸ false;
            ਪੂਛ_ਬਾਈਟਾਂ = 2;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            ਪੂਛ_ਬਾਈਟਾਂ = 3;
        }
        ਨਹੀਂਤਾਂ ਜੇ (s[i] == 0xF4) {
            i++;
            ਜੇ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ਵਾਪਸ false;
            ਪੂਛ_ਬਾਈਟਾਂ = 2;
        }
        ਨਹੀਂਤਾਂ {
            ਵਾਪਸ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        ਜਦੋਂ (i < len && ਪੂਛ_ਬਾਈਟਾਂ > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            ਪੂਛ_ਬਾਈਟਾਂ--;
        }

        ਜੇ (ਪੂਛ_ਬਾਈਟਾਂ != 0)
            ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}
