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
utf32_to_utf8(uint32_t unichar, वर्ण *buffer)
{
    uint8_t length, head;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    यदि (unichar <= 0x007f) {
        buffer[0] = (वर्ण) unichar;
        buffer[1] = '\0';
        फर्काऊ 2;
    }
    अन्यथा यदि (unichar <= 0x07FF) {
        length = 2;
        head = 0xc0;
    }
    /* Handle surrogates */
    अन्यथा यदि (is_surrogate(unichar)) {
        गच्छ ill_formed_code_unit_subsequence;
    }
    अन्यथा यदि (unichar <= 0xffff) {
        length = 3;
        head = 0xe0;
    }
    अन्यथा यदि (unichar <= 0x10ffff) {
        length = 4;
        head = 0xf0;
    }
    अन्यथा {
        गच्छ ill_formed_code_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t shift = 0;
    लागि (अचिन्हित पूर्णाङ्क गन्ती = length - 1; गन्ती > 0; गन्ती--, shift += 6)
        buffer[गन्ती] = (वर्ण)(0x80 | ((unichar >> shift) & 0x3f));

    buffer[0] = (वर्ण)(head | ((unichar >> shift) & 0x3f));
    buffer[length] = '\0';

    फर्काऊ length + 1;

ill_formed_code_unit_subsequence:
    buffer[0] = '\0';
    फर्काऊ 0;
}

bool
is_valid_utf8(अचर वर्ण *ss, माप_प्रकार len)
{
    माप_प्रकार i = 0;
    माप_प्रकार tail_bytes = 0;
    अचर uint8_t *s = (अचर uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    जबसम्म (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        यदि (s[i] <= 0x7F) {
            tail_bytes = 0;
        }
        अन्यथा यदि (s[i] >= 0xC2 && s[i] <= 0xDF) {
            tail_bytes = 1;
        }
        अन्यथा यदि (s[i] == 0xE0) {
            i++;
            यदि (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                फर्काऊ false;
            tail_bytes = 1;
        }
        अन्यथा यदि (s[i] >= 0xE1 && s[i] <= 0xEC) {
            tail_bytes = 2;
        }
        अन्यथा यदि (s[i] == 0xED) {
            i++;
            यदि (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                फर्काऊ false;
            tail_bytes = 1;
        }
        अन्यथा यदि (s[i] >= 0xEE && s[i] <= 0xEF) {
            tail_bytes = 2;
        }
        अन्यथा यदि (s[i] == 0xF0) {
            i++;
            यदि (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                फर्काऊ false;
            tail_bytes = 2;
        }
        अन्यथा यदि (s[i] >= 0xF1 && s[i] <= 0xF3) {
            tail_bytes = 3;
        }
        अन्यथा यदि (s[i] == 0xF4) {
            i++;
            यदि (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                फर्काऊ false;
            tail_bytes = 2;
        }
        अन्यथा {
            फर्काऊ false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        जबसम्म (i < len && tail_bytes > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            tail_bytes--;
        }

        यदि (tail_bytes != 0)
            फर्काऊ false;
    }

    फर्काऊ true;
}
