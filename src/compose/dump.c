/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <stdio.h>
#ضمن <stdbool.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "darray.h"
#ضمن "parser.h"
#ضمن "escape.h"
#ضمن "dump.h"
#ضمن "src/keysym.h"

bool
print_compose_جدول_داخلا(FILE *file, بنية xkb_compose_جدول_داخلا *داخلا)
{
    سائيز_t nsyms;
    ثابت xkb_keysym_t *syms = xkb_compose_جدول_داخلا_sequence(داخلا, &nsyms);
    حرف buf[XKB_KEYSYM_NAME_MAX_سائيز];
    لكل (سائيز_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, حجم(buf));
        fprintf(file, "<%s>", buf);
        إذا (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ثابت حرف *utf8 = xkb_compose_جدول_داخلا_utf8(داخلا);
    إذا (*utf8 != '\0') {
        حرف *escaped = escape_utf8_string_literal(utf8);
        إذا (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            أرجع false;
        } وإلا {
            fprintf(file, " \"%s\"", escaped);
            حرر(escaped);
        }
    }
    ثابت xkb_keysym_t keysym = xkb_compose_جدول_داخلا_keysym(داخلا);
    إذا (keysym != XKB_ڪنجي_No_نشان) {
        xkb_keysym_get_name(keysym, buf, حجم(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    أرجع true;
}

bool
xkb_compose_جدول_dump(FILE *file, بنية xkb_compose_جدول *جدول)
{
    بنية xkb_compose_جدول_داخلا *داخلا;
    بنية xkb_compose_جدول_iterator *iter = xkb_compose_جدول_iterator_new(جدول);

    إذا (!iter)
        أرجع false;

    bool ok = true;
    طالما ((داخلا = xkb_compose_جدول_iterator_ايندڙ(iter))) {
        إذا (!print_compose_جدول_داخلا(file, داخلا)) {
            ok = false;
            اكسر;
        }
    }

    xkb_compose_جدول_iterator_free(iter);
    أرجع ok;
}
