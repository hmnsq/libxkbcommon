/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-compose.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "escape.h"
#അന്തര്ഭാവ "dump.h"
#അന്തര്ഭാവ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, ഘടന xkb_compose_table_entry *entry)
{
    size_t nsyms;
    സ്ഥിരം xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    അക്ഷരം buf[XKB_KEYSYM_NAME_MAX_SIZE];
    വേണ്ടി (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, വലുപ്പം(buf));
        fprintf(file, "<%s>", buf);
        എങ്കിൽ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    സ്ഥിരം അക്ഷരം *utf8 = xkb_compose_table_entry_utf8(entry);
    എങ്കിൽ (*utf8 != '\0') {
        അക്ഷരം *escaped = escape_utf8_string_literal(utf8);
        എങ്കിൽ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            തിരികെ false;
        } അല്ലെങ്കിൽ {
            fprintf(file, " \"%s\"", escaped);
            സ്വതന്ത്രം(escaped);
        }
    }
    സ്ഥിരം xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    എങ്കിൽ (keysym != XKB_താക്കോൽ_No_ചിഹ്നം) {
        xkb_keysym_get_name(keysym, buf, വലുപ്പം(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    തിരികെ true;
}

bool
xkb_compose_table_dump(FILE *file, ഘടന xkb_compose_table *table)
{
    ഘടന xkb_compose_table_entry *entry;
    ഘടന xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    എങ്കിൽ (!iter)
        തിരികെ false;

    bool ok = true;
    ആകുമ്പോൾ ((entry = xkb_compose_table_iterator_next(iter))) {
        എങ്കിൽ (!print_compose_table_entry(file, entry)) {
            ok = false;
            നിർത്തുക;
        }
    }

    xkb_compose_table_iterator_free(iter);
    തിരികെ ok;
}
