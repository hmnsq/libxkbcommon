/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <stdio.h>
#ضمن <stdbool.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "darray.h"
#ضمن "parser.h"
#ضمن "escape.h"
#ضمن "dump.h"
#ضمن "src/keysym.h"

bool
print_compose_table_entry(FILE *file, بنية xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ثابت xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    حرف buf[XKB_KEYSYM_NAME_MAX_SIZE];
    עבור (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, حجم(buf));
        fprintf(file, "<%s>", buf);
        אם (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ثابت حرف *utf8 = xkb_compose_table_entry_utf8(entry);
    אם (*utf8 != '\0') {
        حرف *escaped = escape_utf8_string_literal(utf8);
        אם (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            החזר false;
        } אחרת {
            fprintf(file, " \"%s\"", escaped);
            حرر(escaped);
        }
    }
    ثابت xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    אם (keysym != XKB_KEY_NoSymbol) {
        xkb_keysym_get_name(keysym, buf, حجم(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    החזר true;
}

bool
xkb_compose_table_dump(FILE *file, بنية xkb_compose_table *table)
{
    بنية xkb_compose_table_entry *entry;
    بنية xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    אם (!iter)
        החזר false;

    bool ok = true;
    כלעוד ((entry = xkb_compose_table_iterator_next(iter))) {
        אם (!print_compose_table_entry(file, entry)) {
            ok = false;
            اكسر;
        }
    }

    xkb_compose_table_iterator_free(iter);
    החזר ok;
}
