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
print_compose_table_entry(FILE *file, sa_nracanaa xkb_compose_table_entry *entry)
{
    size_t nsyms;
    const xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    var_na buf[XKB_KEYSYM_NAME_MAX_SIZE];
    for (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, parimaa_na(buf));
        fprintf(file, "<%s>", buf);
        ᯀᯮᯝ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    const var_na *utf8 = xkb_compose_table_entry_utf8(entry);
    ᯀᯮᯝ (*utf8 != '\0') {
        var_na *escaped = escape_utf8_string_literal(utf8);
        ᯀᯮᯝ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ᯀᯮᯞ false;
        } ᯀᯮᯝᯂᯪ {
            fprintf(file, " \"%s\"", escaped);
            free(escaped);
        }
    }
    const xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    ᯀᯮᯝ (keysym != XKB_KEY_NoSymbol) {
        xkb_keysym_get_name(keysym, buf, parimaa_na(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ᯀᯮᯞ true;
}

bool
xkb_compose_table_dump(FILE *file, sa_nracanaa xkb_compose_table *table)
{
    sa_nracanaa xkb_compose_table_entry *entry;
    sa_nracanaa xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    ᯀᯮᯝ (!iter)
        ᯀᯮᯞ false;

    bool ok = true;
    ᯀᯬᯝ ((entry = xkb_compose_table_iterator_next(iter))) {
        ᯀᯮᯝ (!print_compose_table_entry(file, entry)) {
            ok = false;
            bha_nga;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ᯀᯮᯞ ok;
}
