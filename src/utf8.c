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
utf32_to_utf8(uint32_t unichar, अक्षर *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    जर (unichar <= 0x007f) {
        buffer[0] = (अक्षर) unichar;
        buffer[1] = '\0';
        परत 2;
    }
    अन्यथा जर (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    अन्यथा जर (is_surrogate(unichar)) {
        जा ill_formed_code_unit_subsequence;
    }
    अन्यथा जर (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    अन्यथा जर (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    अन्यथा {
        जा ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    साठी (अचिन्हित पूर्णांक मोजणी = length - 1; मोजणी > 0; मोजणी--, shift += 6)
        buffer[मोजणी] = (अक्षर)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (अक्षर)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    परत length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    परत 0;
}

bool
is_valid_utf8(स्थिरांक अक्षर *ss, माप_प्रकार len)
{
    माप_प्रकार i = 0;
    माप_प्रकार tail_bytes = 0;
    स्थिरांक uint8_t *s = (स्थिरांक uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    पर्यंत (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        जर (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        अन्यथा जर (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        अन्यथा जर (s[i] == 0xE0) {
            i++;
            जर (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                परत false;
            tail_bytes = 1;
        }
        अन्यथा जर (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        अन्यथा जर (s[i] == 0xED) {
            i++;
            जर (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                परत false;
            tail_bytes = 1;
        }
        अन्यथा जर (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        अन्यथा जर (s[i] == 0xF0) {
            i++;
            जर (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                परत false;
            tail_bytes = 2;
        }
        अन्यथा जर (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        अन्यथा जर (s[i] == 0xF4) {
            i++;
            जर (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                परत false;
            tail_bytes = 2;
        }
        अन्यथा {
            परत false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        पर्यंत (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        जर (tail_bytes != 0)
            परत false;
    }

    परत true;
}
