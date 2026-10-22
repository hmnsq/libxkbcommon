/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "xkbcommon/xkbcommon-compose.h"
#include "darray.h"
#include "parser.h"
#include "escape.h"
#include "dump.h"
#include "src/keysym.h"

bool
print_compose_table_entry(FILE *file, struct xkb_compose_table_entry *entry)
{
    size_t nsyms;
    const xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    char buf[XKB_KEYSYM_NAME_MAX_SIZE];
    для (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, sizeof(buf));
        fprintf(file, "<%s>", buf);
        если (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    const char *utf8 = xkb_compose_table_entry_utf8(entry);
    если (*utf8 != '\0') {
        char *escaped = escape_utf8_string_literal(utf8);
        если (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            вернуть false;
        } иначе {
            fprintf(file, " \"%s\"", escaped);
            free(escaped);
        }
    }
    const xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    если (keysym != XKB_KEY_NoSymbol) {
        xkb_keysym_get_name(keysym, buf, sizeof(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    вернуть true;
}

bool
xkb_compose_table_dump(FILE *file, struct xkb_compose_table *table)
{
    struct xkb_compose_table_entry *entry;
    struct xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    если (!iter)
        вернуть false;

    bool ok = true;
    пока ((entry = xkb_compose_table_iterator_next(iter))) {
        если (!print_compose_table_entry(file, entry)) {
            ok = false;
            break;
        }
    }

    xkb_compose_table_iterator_free(iter);
    вернуть ok;
}
