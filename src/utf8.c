/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "utils.h"
#అన్తర్భావ "utf8.h"

/* Conformant encoding form conversion from UTF-32 to UTF-8.
 *
 * See https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf#G28875
 * for further details.
*/
uint8_t
utf32_గమ్యం_utf8(uint32_t unichar, అక్షరం *బఫరు)
{
    uint8_t నిడివి, తల;

    /* NOLINTBEGIN(bugprone-branch-clone) */
    ఒకవేళ (unichar <= 0x007f) {
        బఫరు[0] = (అక్షరం) unichar;
        బఫరు[1] = '\0';
        తిరిగి 2;
    }
    లేకపోతే ఒకవేళ (unichar <= 0x07FF) {
        నిడివి = 2;
        తల = 0xc0;
    }
    /* Handle surrogates */
    లేకపోతే ఒకవేళ (is_surrogate(unichar)) {
        వెళ్ళు ill_formed_సంకేతం_unit_subsequence;
    }
    లేకపోతే ఒకవేళ (unichar <= 0xffff) {
        నిడివి = 3;
        తల = 0xe0;
    }
    లేకపోతే ఒకవేళ (unichar <= 0x10ffff) {
        నిడివి = 4;
        తల = 0xf0;
    }
    లేకపోతే {
        వెళ్ళు ill_formed_సంకేతం_unit_subsequence;
    }
    /* NOLINTEND(bugprone-branch-clone) */

    uint8_t జరుపు = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం లెక్క = నిడివి - 1; లెక్క > 0; లెక్క--, జరుపు += 6)
        బఫరు[లెక్క] = (అక్షరం)(0x80 | ((unichar >> జరుపు) & 0x3f));

    బఫరు[0] = (అక్షరం)(తల | ((unichar >> జరుపు) & 0x3f));
    బఫరు[నిడివి] = '\0';

    తిరిగి నిడివి + 1;

ill_formed_సంకేతం_unit_subsequence:
    బఫరు[0] = '\0';
    తిరిగి 0;
}

bool
is_valid_utf8(స్థిరరాశి అక్షరం *ss, కొలత_t len)
{
    కొలత_t i = 0;
    కొలత_t తోక_బైట్లు = 0;
    స్థిరరాశి uint8_t *s = (స్థిరరాశి uint8_t *) ss;

    /* This beauty is from:
     *  The Unicode Standard Version 6.2 - Core Specification, Table 3.7
     *  https://www.unicode.org/versions/Unicode6.2.0/ch03.pdf#G7404
     * We can optimize if needed. */
    అయితే (i < len)
    {
        /* NOLINTBEGIN(bugprone-branch-clone) */
        ఒకవేళ (s[i] <= 0x7F) {
            తోక_బైట్లు = 0;
        }
        లేకపోతే ఒకవేళ (s[i] >= 0xC2 && s[i] <= 0xDF) {
            తోక_బైట్లు = 1;
        }
        లేకపోతే ఒకవేళ (s[i] == 0xE0) {
            i++;
            ఒకవేళ (i >= len || !(s[i] >= 0xA0 && s[i] <= 0xBF))
                తిరిగి false;
            తోక_బైట్లు = 1;
        }
        లేకపోతే ఒకవేళ (s[i] >= 0xE1 && s[i] <= 0xEC) {
            తోక_బైట్లు = 2;
        }
        లేకపోతే ఒకవేళ (s[i] == 0xED) {
            i++;
            ఒకవేళ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x9F))
                తిరిగి false;
            తోక_బైట్లు = 1;
        }
        లేకపోతే ఒకవేళ (s[i] >= 0xEE && s[i] <= 0xEF) {
            తోక_బైట్లు = 2;
        }
        లేకపోతే ఒకవేళ (s[i] == 0xF0) {
            i++;
            ఒకవేళ (i >= len || !(s[i] >= 0x90 && s[i] <= 0xBF))
                తిరిగి false;
            తోక_బైట్లు = 2;
        }
        లేకపోతే ఒకవేళ (s[i] >= 0xF1 && s[i] <= 0xF3) {
            తోక_బైట్లు = 3;
        }
        లేకపోతే ఒకవేళ (s[i] == 0xF4) {
            i++;
            ఒకవేళ (i >= len || !(s[i] >= 0x80 && s[i] <= 0x8F))
                తిరిగి false;
            తోక_బైట్లు = 2;
        }
        లేకపోతే {
            తిరిగి false;
        }
        /* NOLINTEND(bugprone-branch-clone) */

        i++;

        అయితే (i < len && తోక_బైట్లు > 0 && s[i] >= 0x80 && s[i] <= 0xBF) {
            i++;
            తోక_బైట్లు--;
        }

        ఒకవేళ (తోక_బైట్లు != 0)
            తిరిగి false;
    }

    తిరిగి true;
}
