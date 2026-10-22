/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-compose.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "escape.h"
#அந்தர்பாவ "dump.h"
#அந்தர்பாவ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, கட்டமைப்பு xkb_compose_table_entry *entry)
{
    size_t nsyms;
    மாறிலி xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    எழுத்து buf[XKB_KEYSYM_NAME_MAX_SIZE];
    க்கு (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, அளவு(buf));
        fprintf(file, "<%s>", buf);
        எனில் (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    மாறிலி எழுத்து *utf8 = xkb_compose_table_entry_utf8(entry);
    எனில் (*utf8 != '\0') {
        எழுத்து *escaped = escape_utf8_string_literal(utf8);
        எனில் (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            திரும்பு false;
        } இல்லையெனில் {
            fprintf(file, " \"%s\"", escaped);
            விடுவி(escaped);
        }
    }
    மாறிலி xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    எனில் (keysym != XKB_சாவி_No_குறியீடு) {
        xkb_keysym_get_name(keysym, buf, அளவு(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    திரும்பு true;
}

bool
xkb_compose_table_dump(FILE *file, கட்டமைப்பு xkb_compose_table *table)
{
    கட்டமைப்பு xkb_compose_table_entry *entry;
    கட்டமைப்பு xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    எனில் (!iter)
        திரும்பு false;

    bool ok = true;
    போது ((entry = xkb_compose_table_iterator_next(iter))) {
        எனில் (!print_compose_table_entry(file, entry)) {
            ok = false;
            நிறுத்து;
        }
    }

    xkb_compose_table_iterator_free(iter);
    திரும்பு ok;
}
