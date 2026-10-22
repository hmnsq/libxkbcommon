/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#समावेश "config.h"

#समावेश "utils.h"
#समावेश "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_to_utf8(uint32_t unichar, अक्षर *बफर)
{
    uint8_t लंबाई, शीर्ष;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    अगर (unichar <= 0x007f) {
        बफर[0] = (अक्षर) unichar;
        बफर[1] = '\0';
        वापस 2;
    }
    अन्यथा अगर (unichar <= 0x07FF) {
        लंबाई = 2;
        शीर्ष = 0xc0;
    }
    /* Handle surrogates */
    अन्यथा अगर (is_surrogate(unichar)) {
        जाओ ill_formed_code_unit_subsequence;
    }
    अन्यथा अगर (unichar <= 0xffff) {
        लंबाई = 3;
        शीर्ष = 0xe0;
    }
    अन्यथा अगर (unichar <= 0x10ffff) {
        लंबाई = 4;
        शीर्ष = 0xf0;
    }
    अन्यथा {
        जाओ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    क्रम (अचिन्हित पूर्णांक गिनती = लंबाई - 1; गिनती > 0; गिनती--, shift += 6)
        बफर[गिनती] = (अक्षर)(0x80 | ((unichar >> shift) & 0x3f));

    बफर[0] = (अक्षर)(शीर्ष | ((unichar >> shift) & 0x3f));
    बफर[लंबाई] = '\0';

    वापस लंबाई + 1;

ill_formed_code_unit_subsequence:
    बफर[0] = '\0';
    वापस 0;
}

bool
is_valid_utf8(स्थिर अक्षर *ss, माप_प्रकार len)
{
    माप_प्रकार i = 0;
    माप_प्रकार पुच्छ_bytes = 0;
    स्थिर uint8_t *s = (स्थिर uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    जबतक (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        अगर (s[i] <= 0x7F) {
            पुच्छ_bytes = 0;
        }
        अन्यथा अगर (s[i] >= 0xC2 && s[i] <= 0xDF) {
            पुच्छ_bytes = 1;
        }
        अन्यथा अगर (s[i] == 0xE0) {
            i++;
            अगर (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                वापस false;
            पुच्छ_bytes = 1;
        }
        अन्यथा अगर (s[i] >= 0xE1 && s[i] <= 0xEC) {
            पुच्छ_bytes = 2;
        }
        अन्यथा अगर (s[i] == 0xED) {
            i++;
            अगर (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                वापस false;
            पुच्छ_bytes = 1;
        }
        अन्यथा अगर (s[i] >= 0xEE && s[i] <= 0xEF) {
            पुच्छ_bytes = 2;
        }
        अन्यथा अगर (s[i] == 0xF0) {
            i++;
            अगर (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                वापस false;
            पुच्छ_bytes = 2;
        }
        अन्यथा अगर (s[i] >= 0xF1 && s[i] <= 0xF3) {
            पुच्छ_bytes = 3;
        }
        अन्यथा अगर (s[i] == 0xF4) {
            i++;
            अगर (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                वापस false;
            पुच्छ_bytes = 2;
        }
        अन्यथा {
            वापस false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        जबतक (i < len && पुच्छ_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            पुच्छ_bytes--;
        }

        अगर (पुच्छ_bytes != 0)
            वापस false;
    }

    वापस true;
}
