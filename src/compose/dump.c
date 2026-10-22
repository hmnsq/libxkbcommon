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
    स्थिरांक xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    अक्षर buf[XKB_KEYSYM_NAME_MAX_SIZE];
    साठी (माप_प्रकार i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, आकार(buf));
        ख_लिखो(file, "<%s>", buf);
        जर (i + 1 < nsyms) {
            ख_लिखो(file, " ");
        }
    }
    ख_लिखो(file, " : ");
    स्थिरांक अक्षर *utf8 = xkb_compose_table_entry_utf8(entry);
    जर (*utf8 != '\0') {
        अक्षर *escaped = escape_utf8_string_literal(utf8);
        जर (!escaped) {
            ख_लिखो(मानक_त्रुटि, "ERROR: Cannot escape the string: allocation error\n");
            परत false;
        } अन्यथा {
            ख_लिखो(file, " \"%s\"", escaped);
            मुक्त(escaped);
        }
    }
    स्थिरांक xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    जर (keysym != XKB_किल्ली_No_चिन्ह) {
        xkb_keysym_get_name(keysym, buf, आकार(buf));
        ख_लिखो(file, " %s", buf);
    }
    ख_लिखो(file, "\n");
    परत true;
}

bool
xkb_compose_table_dump(खाता *file, संरचना xkb_compose_table *table)
{
    संरचना xkb_compose_table_entry *entry;
    संरचना xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    जर (!iter)
        परत false;

    bool ok = true;
    पर्यंत ((entry = xkb_compose_table_iterator_next(iter))) {
        जर (!print_compose_table_entry(file, entry)) {
            ok = false;
            थांबा;
        }
    }

    xkb_compose_table_iterator_free(iter);
    परत ok;
}
