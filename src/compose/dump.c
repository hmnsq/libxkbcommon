/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon-compose.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "escape.h"
#अन्तर्भाव "dump.h"
#अन्तर्भाव "src/keysym.h"

bool
print_compose_table_entry(FILE *file, संरचना xkb_compose_table_entry *entry)
{
    size_t nsyms;
    ध्रुव xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    वर्ण buf[XKB_KEYSYM_NAME_MAX_SIZE];
    अत्थाय (size_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, परिमाण(buf));
        fprintf(file, "<%s>", buf);
        सचे (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ध्रुव वर्ण *utf8 = xkb_compose_table_entry_utf8(entry);
    सचे (*utf8 != '\0') {
        वर्ण *escaped = escape_utf8_string_literal(utf8);
        सचे (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            पटिनिवत्त false;
        } अञ्ञथा {
            fprintf(file, " \"%s\"", escaped);
            मुक्त(escaped);
        }
    }
    ध्रुव xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    सचे (keysym != XKB_कुञ्चिका_No_चिह्नम्) {
        xkb_keysym_get_name(keysym, buf, परिमाण(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    पटिनिवत्त true;
}

bool
xkb_compose_table_dump(FILE *file, संरचना xkb_compose_table *table)
{
    संरचना xkb_compose_table_entry *entry;
    संरचना xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    सचे (!iter)
        पटिनिवत्त false;

    bool ok = true;
    याव ((entry = xkb_compose_table_iterator_next(iter))) {
        सचे (!print_compose_table_entry(file, entry)) {
            ok = false;
            भङ्ग;
        }
    }

    xkb_compose_table_iterator_free(iter);
    पटिनिवत्त ok;
}
