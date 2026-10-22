/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon-compose.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "parser.h"
#অন্তর্ভাব "escape.h"
#অন্তর্ভাব "dump.h"
#অন্তর্ভাব "src/keysym.h"

bool
print_compose_table_entry(FILE *file, সংরচনা xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ধ্রুব xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    বর্ণ buf[XKB_KEYSYM_NAME_MAX_SIZE];
    প্রতি (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, পরিমাণ(buf));
        fprintf(file, "<%s>", buf);
        যদি (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ধ্রুব বর্ণ *utf8 = xkb_compose_table_entry_utf8(entry);
    যদি (*utf8 != '\0') {
        বর্ণ *escaped = escape_utf8_string_literal(utf8);
        যদি (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            প্রত্যাগম false;
        } অন্যথা {
            fprintf(file, " \"%s\"", escaped);
            মুক্ত(escaped);
        }
    }
    ধ্রুব xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    যদি (keysym != XKB_চাবি_No_প্রতীক) {
        xkb_keysym_get_name(keysym, buf, পরিমাণ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    প্রত্যাগম true;
}

bool
xkb_compose_table_dump(FILE *file, সংরচনা xkb_compose_table *table)
{
    সংরচনা xkb_compose_table_entry *entry;
    সংরচনা xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    যদি (!iter)
        প্রত্যাগম false;

    bool ok = true;
    যাবত্ ((entry = xkb_compose_table_iterator_next(iter))) {
        যদি (!print_compose_table_entry(file, entry)) {
            ok = false;
            ভঙ্গ;
        }
    }

    xkb_compose_table_iterator_free(iter);
    প্রত্যাগম ok;
}
