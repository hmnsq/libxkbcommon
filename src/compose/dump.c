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
print_compose_table_entry(FILE *file, কাঠামো xkb_compose_table_entry *entry)
{
    আকার_t nsyms;
    ধ্রুবক xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    অক্ষর buf[XKB_KEYSYM_NAME_MAX_আকার];
    জন্য (আকার_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, আকার(buf));
        fprintf(file, "<%s>", buf);
        যদি (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ধ্রুবক অক্ষর *utf8 = xkb_compose_table_entry_utf8(entry);
    যদি (*utf8 != '\0') {
        অক্ষর *escaped = escape_utf8_string_literal(utf8);
        যদি (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ফেরত false;
        } অন্যথা {
            fprintf(file, " \"%s\"", escaped);
            মুক্ত(escaped);
        }
    }
    ধ্রুবক xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    যদি (keysym != XKB_চাবি_No_প্রতীক) {
        xkb_keysym_get_name(keysym, buf, আকার(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ফেরত true;
}

bool
xkb_compose_table_dump(FILE *file, কাঠামো xkb_compose_table *table)
{
    কাঠামো xkb_compose_table_entry *entry;
    কাঠামো xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    যদি (!iter)
        ফেরত false;

    bool ok = true;
    যখন ((entry = xkb_compose_table_iterator_পরবর্তী(iter))) {
        যদি (!print_compose_table_entry(file, entry)) {
            ok = false;
            বিরতি;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ফেরত ok;
}
