/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-compose.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "escape.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dump.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "src/keysym.h"

bool
print_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ(FILE *file, ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਮਾਪ_t nsyms;
    ਅਚਲ xkb_keysym_t *syms = xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_sequence(ਇੰਦਰਾਜ, &nsyms);
    ਅੱਖਰ buf[XKB_KEYSYM_NAME_MAX_ਮਾਪ];
    ਲਈ (ਮਾਪ_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, ਆਕਾਰ(buf));
        fprintf(file, "<%s>", buf);
        ਜੇ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ਅਚਲ ਅੱਖਰ *utf8 = xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_utf8(ਇੰਦਰਾਜ);
    ਜੇ (*utf8 != '\0') {
        ਅੱਖਰ *escaped = escape_utf8_string_literal(utf8);
        ਜੇ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ਵਾਪਸ false;
        } ਨਹੀਂਤਾਂ {
            fprintf(file, " \"%s\"", escaped);
            ਮੁਕਤ(escaped);
        }
    }
    ਅਚਲ xkb_keysym_t keysym = xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_keysym(ਇੰਦਰਾਜ);
    ਜੇ (keysym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        xkb_keysym_get_name(keysym, buf, ਆਕਾਰ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ਵਾਪਸ true;
}

bool
xkb_compose_ਸਾਰਣੀ_dump(FILE *file, ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator *iter = xkb_compose_ਸਾਰਣੀ_iterator_new(ਸਾਰਣੀ);

    ਜੇ (!iter)
        ਵਾਪਸ false;

    bool ok = true;
    ਜਦੋਂ ((ਇੰਦਰਾਜ = xkb_compose_ਸਾਰਣੀ_iterator_ਅਗਲਾ(iter))) {
        ਜੇ (!print_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ(file, ਇੰਦਰਾਜ)) {
            ok = false;
            ਰੋਕੋ;
        }
    }

    xkb_compose_ਸਾਰਣੀ_iterator_free(iter);
    ਵਾਪਸ ok;
}
