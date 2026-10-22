/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, অক্ষর *বাফার)
{
    uint8_t দৈর্ঘ্য, শীর্ষ;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    যদি (unichar <= 0x007f) {
        বাফার[0] = (অক্ষর) unichar;
        বাফার[1] = '\0';
        ফেরত 2;
    }
    অন্যথা যদি (unichar <= 0x07FF) {
        দৈর্ঘ্য = 2;
        শীর্ষ = 0xc0;
    }
    /* Handle surrogates */
    অন্যথা যদি (is_surrogate(unichar)) {
        যাও ill_formed_code_unit_subsequence;
    }
    অন্যথা যদি (unichar <= 0xffff) {
        দৈর্ঘ্য = 3;
        শীর্ষ = 0xe0;
    }
    অন্যথা যদি (unichar <= 0x10ffff) {
        দৈর্ঘ্য = 4;
        শীর্ষ = 0xf0;
    }
    অন্যথা {
        যাও ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    জন্য (unsigned পূর্ণসংখ্যা গুনতি = দৈর্ঘ্য - 1; গুনতি > 0; গুনতি--, shift += 6)
        বাফার[গুনতি] = (অক্ষর)(0x80 | ((unichar >> shift) & 0x3f));

    বাফার[0] = (অক্ষর)(শীর্ষ | ((unichar >> shift) & 0x3f));
    বাফার[দৈর্ঘ্য] = '\0';

    ফেরত দৈর্ঘ্য + 1;

ill_formed_code_unit_subsequence:
    বাফার[0] = '\0';
    ফেরত 0;
}

bool
is_valid_utf8(ধ্রুবক অক্ষর *ss, আকার_t len)
{
    আকার_t i = 0;
    আকার_t লেজ_bytes = 0;
    ধ্রুবক uint8_t *s = (ধ্রুবক uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    যখন (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        যদি (s[i] <= 0x7F) {
            লেজ_bytes = 0;
        }
        অন্যথা যদি (s[i] >= 0xC2 && s[i] <= 0xDF) {
            লেজ_bytes = 1;
        }
        অন্যথা যদি (s[i] == 0xE0) {
            i++;
            যদি (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                ফেরত false;
            লেজ_bytes = 1;
        }
        অন্যথা যদি (s[i] >= 0xE1 && s[i] <= 0xEC) {
            লেজ_bytes = 2;
        }
        অন্যথা যদি (s[i] == 0xED) {
            i++;
            যদি (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                ফেরত false;
            লেজ_bytes = 1;
        }
        অন্যথা যদি (s[i] >= 0xEE && s[i] <= 0xEF) {
            লেজ_bytes = 2;
        }
        অন্যথা যদি (s[i] == 0xF0) {
            i++;
            যদি (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                ফেরত false;
            লেজ_bytes = 2;
        }
        অন্যথা যদি (s[i] >= 0xF1 && s[i] <= 0xF3) {
            লেজ_bytes = 3;
        }
        অন্যথা যদি (s[i] == 0xF4) {
            i++;
            যদি (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                ফেরত false;
            লেজ_bytes = 2;
        }
        অন্যথা {
            ফেরত false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        যখন (i < len && লেজ_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            লেজ_bytes--;
        }

        যদি (লেজ_bytes != 0)
            ফেরত false;
    }

    ফেরত true;
}
