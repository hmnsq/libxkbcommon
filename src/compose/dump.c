/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-compose.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "escape.h"
#ଅନ୍ତର୍ଭାଵ "dump.h"
#ଅନ୍ତର୍ଭାଵ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, ସଂରଚନା xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ଅଚଳ xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    ଅକ୍ଷର buf[XKB_KEYSYM_NAME_MAX_SIZE];
    ପାଇଁ (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, ଆକାର(buf));
        fprintf(file, "<%s>", buf);
        ଯଦି (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ଅଚଳ ଅକ୍ଷର *utf8 = xkb_compose_table_entry_utf8(entry);
    ଯଦି (*utf8 != '\0') {
        ଅକ୍ଷର *escaped = escape_utf8_string_literal(utf8);
        ଯଦି (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ଫେରନ୍ତୁ false;
        } ଅନ୍ୟଥା {
            fprintf(file, " \"%s\"", escaped);
            ମୁକ୍ତ(escaped);
        }
    }
    ଅଚଳ xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    ଯଦି (keysym != XKB_ଚାବି_No_ପ୍ରତୀକ) {
        xkb_keysym_get_name(keysym, buf, ଆକାର(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ଫେରନ୍ତୁ true;
}

bool
xkb_compose_table_dump(FILE *file, ସଂରଚନା xkb_compose_table *table)
{
    ସଂରଚନା xkb_compose_table_entry *entry;
    ସଂରଚନା xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    ଯଦି (!iter)
        ଫେରନ୍ତୁ false;

    bool ok = true;
    ଯେତେବେଳେ ((entry = xkb_compose_table_iterator_next(iter))) {
        ଯଦି (!print_compose_table_entry(file, entry)) {
            ok = false;
            ଭାଙ୍ଗ;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ଫେରନ୍ତୁ ok;
}
