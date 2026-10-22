/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <stdio.h>
#شامل <stdbool.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon-compose.h"
#شامل "darray.h"
#شامل "parser.h"
#شامل "escape.h"
#شامل "dump.h"
#شامل "src/keysym.h"

bool
print_compose_table_entry(FILE *file, ساختار xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ثابت xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    نویسه buf[XKB_KEYSYM_NAME_MAX_SIZE];
    برای (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, اندازه(buf));
        fprintf(file, "<%s>", buf);
        اگر (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ثابت نویسه *utf8 = xkb_compose_table_entry_utf8(entry);
    اگر (*utf8 != '\0') {
        نویسه *escaped = escape_utf8_string_literal(utf8);
        اگر (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            بازگشت false;
        } وگرنه {
            fprintf(file, " \"%s\"", escaped);
            آزاد(escaped);
        }
    }
    ثابت xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    اگر (keysym != XKB_کلید_No_نماد) {
        xkb_keysym_get_name(keysym, buf, اندازه(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    بازگشت true;
}

bool
xkb_compose_table_dump(FILE *file, ساختار xkb_compose_table *table)
{
    ساختار xkb_compose_table_entry *entry;
    ساختار xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    اگر (!iter)
        بازگشت false;

    bool ok = true;
    وقتی ((entry = xkb_compose_table_iterator_next(iter))) {
        اگر (!print_compose_table_entry(file, entry)) {
            ok = false;
            بشکن;
        }
    }

    xkb_compose_table_iterator_free(iter);
    بازگشت ok;
}
