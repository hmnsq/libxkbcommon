/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <stdio.h>
#شامل <stdbool.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon-compose.h"
#شامل "darray.h"
#شامل "parser.h"
#شامل "escape.h"
#شامل "dump.h"
#شامل "src/keysym.h"

bool
print_compose_جدول_اندراج(FILE *file, ساخت xkb_compose_جدول_اندراج *اندراج_2)
{
    سائز_t nsyms;
    ثابت xkb_keysym_t *syms = xkb_compose_جدول_اندراج_sequence(اندراج_2, &nsyms);
    حرف buf[XKB_KEYSYM_NAME_MAX_سائز];
    برائے (سائز_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, حجم(buf));
        fprintf(file, "<%s>", buf);
        اگر (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ثابت حرف *utf8 = xkb_compose_جدول_اندراج_utf8(اندراج_2);
    اگر (*utf8 != '\0') {
        حرف *escaped = escape_utf8_string_literal(utf8);
        اگر (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            واپس false;
        } ورنہ {
            fprintf(file, " \"%s\"", escaped);
            آزاد(escaped);
        }
    }
    ثابت xkb_keysym_t keysym = xkb_compose_جدول_اندراج_keysym(اندراج_2);
    اگر (keysym != XKB_کلید_No_علامت) {
        xkb_keysym_get_name(keysym, buf, حجم(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    واپس true;
}

bool
xkb_compose_جدول_dump(FILE *file, ساخت xkb_compose_جدول *جدول)
{
    ساخت xkb_compose_جدول_اندراج *اندراج_2;
    ساخت xkb_compose_جدول_iterator *iter = xkb_compose_جدول_iterator_new(جدول);

    اگر (!iter)
        واپس false;

    bool ok = true;
    جب ((اندراج_2 = xkb_compose_جدول_iterator_اگلا(iter))) {
        اگر (!print_compose_جدول_اندراج(file, اندراج_2)) {
            ok = false;
            توڑو;
        }
    }

    xkb_compose_جدول_iterator_free(iter);
    واپس ok;
}
