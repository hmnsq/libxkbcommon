/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon-compose.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "escape.h"
#అన్తర్భావ "dump.h"
#అన్తర్భావ "src/keysym.h"

bool
print_compose_పట్టిక_నమోదు(FILE *file, నిర్మాణం xkb_compose_పట్టిక_నమోదు *నమోదు_2)
{
    కొలత_t nsyms;
    స్థిరరాశి xkb_keysym_t *syms = xkb_compose_పట్టిక_నమోదు_sequence(నమోదు_2, &nsyms);
    అక్షరం buf[XKB_KEYSYM_NAME_MAX_కొలత];
    కొరకు (కొలత_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, పరిమాణం(buf));
        fprintf(file, "<%s>", buf);
        ఒకవేళ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    స్థిరరాశి అక్షరం *utf8 = xkb_compose_పట్టిక_నమోదు_utf8(నమోదు_2);
    ఒకవేళ (*utf8 != '\0') {
        అక్షరం *escaped = escape_utf8_string_literal(utf8);
        ఒకవేళ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            తిరిగి false;
        } లేకపోతే {
            fprintf(file, " \"%s\"", escaped);
            విడుదల(escaped);
        }
    }
    స్థిరరాశి xkb_keysym_t keysym = xkb_compose_పట్టిక_నమోదు_keysym(నమోదు_2);
    ఒకవేళ (keysym != XKB_కీలకం_No_చిహ్నం) {
        xkb_keysym_get_name(keysym, buf, పరిమాణం(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    తిరిగి true;
}

bool
xkb_compose_పట్టిక_dump(FILE *file, నిర్మాణం xkb_compose_పట్టిక *పట్టిక)
{
    నిర్మాణం xkb_compose_పట్టిక_నమోదు *నమోదు_2;
    నిర్మాణం xkb_compose_పట్టిక_iterator *iter = xkb_compose_పట్టిక_iterator_new(పట్టిక);

    ఒకవేళ (!iter)
        తిరిగి false;

    bool ok = true;
    అయితే ((నమోదు_2 = xkb_compose_పట్టిక_iterator_తదుపరి(iter))) {
        ఒకవేళ (!print_compose_పట్టిక_నమోదు(file, నమోదు_2)) {
            ok = false;
            ఆపు;
        }
    }

    xkb_compose_పట్టిక_iterator_free(iter);
    తిరిగి ok;
}
