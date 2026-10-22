/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon-compose.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "parser.h"
#અન્તર્ભાવ "escape.h"
#અન્તર્ભાવ "dump.h"
#અન્તર્ભાવ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, સંરચના xkb_compose_table_entry *entry)
{
    size_t nsyms;
    અચલ xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    અક્ષર buf[XKB_KEYSYM_NAME_MAX_SIZE];
    માટે (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, કદ(buf));
        fprintf(file, "<%s>", buf);
        જો (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    અચલ અક્ષર *utf8 = xkb_compose_table_entry_utf8(entry);
    જો (*utf8 != '\0') {
        અક્ષર *escaped = escape_utf8_string_literal(utf8);
        જો (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            પાછા false;
        } નહીંતર {
            fprintf(file, " \"%s\"", escaped);
            મુક્ત(escaped);
        }
    }
    અચલ xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    જો (keysym != XKB_ચાવી_No_પ્રતીક) {
        xkb_keysym_get_name(keysym, buf, કદ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    પાછા true;
}

bool
xkb_compose_table_dump(FILE *file, સંરચના xkb_compose_table *table)
{
    સંરચના xkb_compose_table_entry *entry;
    સંરચના xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    જો (!iter)
        પાછા false;

    bool ok = true;
    જ્યારે ((entry = xkb_compose_table_iterator_next(iter))) {
        જો (!print_compose_table_entry(file, entry)) {
            ok = false;
            તોડો;
        }
    }

    xkb_compose_table_iterator_free(iter);
    પાછા ok;
}
