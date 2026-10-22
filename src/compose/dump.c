/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon-compose.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "parser.h"
#অন্তর্ভাৱ "escape.h"
#অন্তর্ভাৱ "dump.h"
#অন্তর্ভাৱ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, সংরচনা xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ধ্রুৱ xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    ৱর্ণ buf[XKB_KEYSYM_NAME_MAX_SIZE];
    বাবে (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, পরিমাণ(buf));
        fprintf(file, "<%s>", buf);
        যদি (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ধ্রুৱ ৱর্ণ *utf8 = xkb_compose_table_entry_utf8(entry);
    যদি (*utf8 != '\0') {
        ৱর্ণ *escaped = escape_utf8_string_literal(utf8);
        যদি (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ঘূৰাই false;
        } অন্যথা {
            fprintf(file, " \"%s\"", escaped);
            মুক্ত(escaped);
        }
    }
    ধ্রুৱ xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    যদি (keysym != XKB_চাবি_No_প্ৰতীক) {
        xkb_keysym_get_name(keysym, buf, পরিমাণ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ঘূৰাই true;
}

bool
xkb_compose_table_dump(FILE *file, সংরচনা xkb_compose_table *table)
{
    সংরচনা xkb_compose_table_entry *entry;
    সংরচনা xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    যদি (!iter)
        ঘূৰাই false;

    bool ok = true;
    যেতিয়ালৈ ((entry = xkb_compose_table_iterator_next(iter))) {
        যদি (!print_compose_table_entry(file, entry)) {
            ok = false;
            ভঙ্গ;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ঘূৰাই ok;
}
