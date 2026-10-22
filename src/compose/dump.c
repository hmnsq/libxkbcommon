/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-compose.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "escape.h"
#ಅನ್ತರ್ಭಾವ "dump.h"
#ಅನ್ತರ್ಭಾವ "src/keysym.h"

bool
print_compose_table_entry(FILE *file, ಸಂರಚನಾ xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ಧ್ರುವ xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    ವರ್ಣ buf[XKB_KEYSYM_NAME_MAX_SIZE];
    ಪ್ರತಿ (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, ಪರಿಮಾಣ(buf));
        fprintf(file, "<%s>", buf);
        ಯದಿ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ಧ್ರುವ ವರ್ಣ *utf8 = xkb_compose_table_entry_utf8(entry);
    ಯದಿ (*utf8 != '\0') {
        ವರ್ಣ *escaped = escape_utf8_string_literal(utf8);
        ಯದಿ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ಪ್ರತ್ಯಾಗಮ false;
        } ಅನ್ಯಥಾ {
            fprintf(file, " \"%s\"", escaped);
            ಮುಕ್ತ(escaped);
        }
    }
    ಧ್ರುವ xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    ಯದಿ (keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        xkb_keysym_get_name(keysym, buf, ಪರಿಮಾಣ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
xkb_compose_table_dump(FILE *file, ಸಂರಚನಾ xkb_compose_table *table)
{
    ಸಂರಚನಾ xkb_compose_table_entry *entry;
    ಸಂರಚನಾ xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    ಯದಿ (!iter)
        ಪ್ರತ್ಯಾಗಮ false;

    bool ok = true;
    ಯಾವತ್ ((entry = xkb_compose_table_iterator_next(iter))) {
        ಯದಿ (!print_compose_table_entry(file, entry)) {
            ok = false;
            ಭಙ್ಗ;
        }
    }

    xkb_compose_table_iterator_free(iter);
    ಪ್ರತ್ಯಾಗಮ ok;
}
