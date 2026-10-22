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
    ສຳລັບ (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, parimaa_na(buf));
        fprintf(file, "<%s>", buf);
        ຖ້າ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    const var_na *utf8 = xkb_compose_table_entry_utf8(entry);
    ຖ້າ (*utf8 != '\0') {
        var_na *escaped = escape_utf8_string_literal(utf8);
        ຖ້າ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ສົ່ງຄືນ false;
        } ບໍ່ດັ່ງນັ້ນ {
            fprintf(file, " \"%s\"", escaped);
            free(escaped);
        }
    }
    const xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    ຖ້າ (keysym != XKB_KEY_NoSymbol) {
        xkb_keysym_get_name(keysym, buf, parimaa_na(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ສົ່ງຄືນ true;
}

bool
xkb_compose_table_dump(FILE *file, sa_nracanaa xkb_compose_table *table)
{
    sa_nracanaa xkb_compose_table_entry *entry;
    sa_nracanaa xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    ຖ້າ (!iter)
        ສົ່ງຄືນ false;

    bool ok = true;
    ໃນຂະນະທີ່ ((entry = xkb_compose_table_iterator_next(iter))) {
        ຖ້າ (!print_compose_table_entry(file, entry)) {
            ok = false;
            bha_nga;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ສົ່ງຄືນ ok;
}
