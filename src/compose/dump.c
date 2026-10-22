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
    위해 (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, sizeof(buf));
        fprintf(file, "<%s>", buf);
        만약 (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    const char *utf8 = xkb_compose_table_entry_utf8(entry);
    만약 (*utf8 != '\0') {
        char *escaped = escape_utf8_string_literal(utf8);
        만약 (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            반환 false;
        } 아니면 {
            fprintf(file, " \"%s\"", escaped);
            free(escaped);
        }
    }
    const xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    만약 (keysym != XKB_KEY_NoSymbol) {
        xkb_keysym_get_name(keysym, buf, sizeof(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    반환 true;
}

bool
xkb_compose_table_dump(FILE *file, struct xkb_compose_table *table)
{
    struct xkb_compose_table_entry *entry;
    struct xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    만약 (!iter)
        반환 false;

    bool ok = true;
    동안 ((entry = xkb_compose_table_iterator_next(iter))) {
        만약 (!print_compose_table_entry(file, entry)) {
            ok = false;
            break;
        }
    }

    xkb_compose_table_iterator_free(iter);
    반환 ok;
}
