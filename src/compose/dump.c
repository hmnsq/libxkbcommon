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
print_compose_सारणी_प्रविष्टिः(FILE *file, संरचना xkb_compose_सारणी_प्रविष्टिः *प्रविष्टिः)
{
    आकार_t nsyms;
    ध्रुव xkb_keysym_t *syms = xkb_compose_सारणी_प्रविष्टिः_sequence(प्रविष्टिः, &nsyms);
    वर्ण buf[XKB_KEYSYM_NAME_MAX_आकार];
    प्रति (आकार_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, परिमाण(buf));
        fprintf(file, "<%s>", buf);
        यदि (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ध्रुव वर्ण *utf8 = xkb_compose_सारणी_प्रविष्टिः_utf8(प्रविष्टिः);
    यदि (*utf8 != '\0') {
        वर्ण *escaped = escape_utf8_string_literal(utf8);
        यदि (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            प्रत्यागम false;
        } अन्यथा {
            fprintf(file, " \"%s\"", escaped);
            मुक्त(escaped);
        }
    }
    ध्रुव xkb_keysym_t keysym = xkb_compose_सारणी_प्रविष्टिः_keysym(प्रविष्टिः);
    यदि (keysym != XKB_कुञ्चिका_No_चिह्नम्) {
        xkb_keysym_get_name(keysym, buf, परिमाण(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    प्रत्यागम true;
}

bool
xkb_compose_सारणी_dump(FILE *file, संरचना xkb_compose_सारणी *सारणी)
{
    संरचना xkb_compose_सारणी_प्रविष्टिः *प्रविष्टिः;
    संरचना xkb_compose_सारणी_iterator *iter = xkb_compose_सारणी_iterator_new(सारणी);

    यदि (!iter)
        प्रत्यागम false;

    bool ok = true;
    यावत् ((प्रविष्टिः = xkb_compose_सारणी_iterator_अग्र(iter))) {
        यदि (!print_compose_सारणी_प्रविष्टिः(file, प्रविष्टिः)) {
            ok = false;
            भङ्ग;
        }
    }

    xkb_compose_सारणी_iterator_free(iter);
    प्रत्यागम ok;
}
