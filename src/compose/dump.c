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
print_compose_table_entry(खाता *file, संरचना xkb_compose_table_entry *entry)
{
    माप_प्रकार nsyms;
    ध्रुव xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    वर्ण buf[XKB_KEYSYM_NAME_MAX_SIZE];
    प्रति (माप_प्रकार i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, परिमाण(buf));
        ख_लिखो(file, "<%s>", buf);
        यदि (i + 1 < nsyms) {
            ख_लिखो(file, " ");
        }
    }
    ख_लिखो(file, " : ");
    ध्रुव वर्ण *utf8 = xkb_compose_table_entry_utf8(entry);
    यदि (*utf8 != '\0') {
        वर्ण *escaped = escape_utf8_string_literal(utf8);
        यदि (!escaped) {
            ख_लिखो(मानक_त्रुटि, "ERROR: Cannot escape the string: allocation error\n");
            प्रत्यागम false;
        } अन्यथा {
            ख_लिखो(file, " \"%s\"", escaped);
            मुक्त(escaped);
        }
    }
    ध्रुव xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    यदि (keysym != XKB_कुंजी_No_प्रतीक) {
        xkb_keysym_get_name(keysym, buf, परिमाण(buf));
        ख_लिखो(file, " %s", buf);
    }
    ख_लिखो(file, "\n");
    प्रत्यागम true;
}

bool
xkb_compose_table_dump(खाता *file, संरचना xkb_compose_table *table)
{
    संरचना xkb_compose_table_entry *entry;
    संरचना xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    यदि (!iter)
        प्रत्यागम false;

    bool ok = true;
    यावत् ((entry = xkb_compose_table_iterator_next(iter))) {
        यदि (!print_compose_table_entry(file, entry)) {
            ok = false;
            भङ्ग;
        }
    }

    xkb_compose_table_iterator_free(iter);
    प्रत्यागम ok;
}
