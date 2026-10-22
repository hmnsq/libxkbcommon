/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "utils.h"
#अन्तर्भाव "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_गन्तव्यम्_utf8(uint32_t unichar, वर्ण *बफर)
{
    uint8_t दीर्घता, शीर्ष;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    यदि (unichar <= 0x007f) {
        बफर[0] = (वर्ण) unichar;
        बफर[1] = '\0';
        प्रत्यागम 2;
    }
    अन्यथा यदि (unichar <= 0x07FF) {
        दीर्घता = 2;
        शीर्ष = 0xc0;
    }
    /* Handle surrogates */
    अन्यथा यदि (is_surrogate(unichar)) {
        गच्छ ill_formed_संहिता_unit_subsequence;
    }
    अन्यथा यदि (unichar <= 0xffff) {
        दीर्घता = 3;
        शीर्ष = 0xe0;
    }
    अन्यथा यदि (unichar <= 0x10ffff) {
        दीर्घता = 4;
        शीर्ष = 0xf0;
    }
    अन्यथा {
        गच्छ ill_formed_संहिता_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t सरणम् = 0;
    प्रति (अचिह्नित पूर्णाङ्क गणनम् = दीर्घता - 1; गणनम् > 0; गणनम्--, सरणम् += 6)
        बफर[गणनम्] = (वर्ण)(0x80 | ((unichar >> सरणम्) & 0x3f));

    बफर[0] = (वर्ण)(शीर्ष | ((unichar >> सरणम्) & 0x3f));
    बफर[दीर्घता] = '\0';

    प्रत्यागम दीर्घता + 1;

ill_formed_संहिता_unit_subsequence:
    बफर[0] = '\0';
    प्रत्यागम 0;
}

bool
is_valid_utf8(ध्रुव वर्ण *ss, आकार_t len)
{
    आकार_t i = 0;
    आकार_t पुच्छ_अष्टकानि = 0;
    ध्रुव uint8_t *s = (ध्रुव uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    यावत् (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        यदि (s[i] <= 0x7F) {
            पुच्छ_अष्टकानि = 0;
        }
        अन्यथा यदि (s[i] >= 0xC2 && s[i] <= 0xDF) {
            पुच्छ_अष्टकानि = 1;
        }
        अन्यथा यदि (s[i] == 0xE0) {
            i++;
            यदि (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                प्रत्यागम false;
            पुच्छ_अष्टकानि = 1;
        }
        अन्यथा यदि (s[i] >= 0xE1 && s[i] <= 0xEC) {
            पुच्छ_अष्टकानि = 2;
        }
        अन्यथा यदि (s[i] == 0xED) {
            i++;
            यदि (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                प्रत्यागम false;
            पुच्छ_अष्टकानि = 1;
        }
        अन्यथा यदि (s[i] >= 0xEE && s[i] <= 0xEF) {
            पुच्छ_अष्टकानि = 2;
        }
        अन्यथा यदि (s[i] == 0xF0) {
            i++;
            यदि (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                प्रत्यागम false;
            पुच्छ_अष्टकानि = 2;
        }
        अन्यथा यदि (s[i] >= 0xF1 && s[i] <= 0xF3) {
            पुच्छ_अष्टकानि = 3;
        }
        अन्यथा यदि (s[i] == 0xF4) {
            i++;
            यदि (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                प्रत्यागम false;
            पुच्छ_अष्टकानि = 2;
        }
        अन्यथा {
            प्रत्यागम false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        यावत् (i < len && पुच्छ_अष्टकानि > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            पुच्छ_अष्टकानि--;
        }

        यदि (पुच्छ_अष्टकानि != 0)
            प्रत्यागम false;
    }

    प्रत्यागम true;
}
