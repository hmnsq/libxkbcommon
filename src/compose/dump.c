/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <stdio.h>
#समावेश <stdbool.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon-compose.h"
#समावेश "darray.h"
#समावेश "parser.h"
#समावेश "escape.h"
#समावेश "dump.h"
#समावेश "src/keysym.h"

bool
print_compose_table_entry(खाता *file, काष्ठा xkb_compose_table_entry *entry)
{
    माप_प्रकार nsyms;
    स्थिर xkb_keysym_t *syms = xkb_compose_table_entry_sequence(entry, &nsyms);
    अक्षर buf[XKB_KEYSYM_NAME_MAX_आकार];
    क्रम (माप_प्रकार i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, माप(buf));
        ख_लिखो(file, "<%s>", buf);
        अगर (i + 1 < nsyms) {
            ख_लिखो(file, " ");
        }
    }
    ख_लिखो(file, " : ");
    स्थिर अक्षर *utf8 = xkb_compose_table_entry_utf8(entry);
    अगर (*utf8 != '\0') {
        अक्षर *escaped = escape_utf8_string_literal(utf8);
        अगर (!escaped) {
            ख_लिखो(मानक_त्रुटि, "ERROR: Cannot escape the string: allocation error\n");
            वापस false;
        } अन्यथा {
            ख_लिखो(file, " \"%s\"", escaped);
            मुक्त(escaped);
        }
    }
    स्थिर xkb_keysym_t keysym = xkb_compose_table_entry_keysym(entry);
    अगर (keysym != XKB_कुंजी_No_प्रतीक) {
        xkb_keysym_get_name(keysym, buf, माप(buf));
        ख_लिखो(file, " %s", buf);
    }
    ख_लिखो(file, "\n");
    वापस true;
}

bool
xkb_compose_table_dump(खाता *file, काष्ठा xkb_compose_table *table)
{
    काष्ठा xkb_compose_table_entry *entry;
    काष्ठा xkb_compose_table_iterator *iter = xkb_compose_table_iterator_new(table);

    अगर (!iter)
        वापस false;

    bool ok = true;
    जबतक ((entry = xkb_compose_table_iterator_अग्र(iter))) {
        अगर (!print_compose_table_entry(file, entry)) {
            ok = false;
            अवरोध;
        }
    }

    xkb_compose_table_iterator_free(iter);
    वापस ok;
}
