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
print_compose_ಕೋಷ್ಟಕ_ನಮೂದು(FILE *file, ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು)
{
    ಗಾತ್ರ_t nsyms;
    ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms = xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_sequence(ನಮೂದು, &nsyms);
    ಅಕ್ಷರ buf[XKB_KEYSYM_NAME_MAX_ಗಾತ್ರ];
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < nsyms; i++) {
        xkb_keysym_get_name(syms[i], buf, ಗಾತ್ರ(buf));
        fprintf(file, "<%s>", buf);
        ಆದರೆ (i + 1 < nsyms) {
            fprintf(file, " ");
        }
    }
    fprintf(file, " : ");
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *utf8 = xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_utf8(ನಮೂದು);
    ಆದರೆ (*utf8 != '\0') {
        ಅಕ್ಷರ *escaped = escape_utf8_string_literal(utf8);
        ಆದರೆ (!escaped) {
            fprintf(stderr, "ERROR: Cannot escape the string: allocation error\n");
            ಹಿಂತಿರುಗಿ false;
        } ಇಲ್ಲದಿದ್ದರೆ {
            fprintf(file, " \"%s\"", escaped);
            ಬಿಡುಗಡೆ(escaped);
        }
    }
    ಸ್ಥಿರಾಂಕ xkb_keysym_t keysym = xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_keysym(ನಮೂದು);
    ಆದರೆ (keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        xkb_keysym_get_name(keysym, buf, ಗಾತ್ರ(buf));
        fprintf(file, " %s", buf);
    }
    fprintf(file, "\n");
    ಹಿಂತಿರುಗಿ true;
}

bool
xkb_compose_ಕೋಷ್ಟಕ_dump(FILE *file, ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator *iter = xkb_compose_ಕೋಷ್ಟಕ_iterator_new(ಕೋಷ್ಟಕ);

    ಆದರೆ (!iter)
        ಹಿಂತಿರುಗಿ false;

    bool ok = true;
    ಆಗ ((ನಮೂದು = xkb_compose_ಕೋಷ್ಟಕ_iterator_ಮುಂದಿನ(iter))) {
        ಆದರೆ (!print_compose_ಕೋಷ್ಟಕ_ನಮೂದು(file, ನಮೂದು)) {
            ok = false;
            ನಿಲ್ಲಿಸು;
        }
    }

    xkb_compose_ಕೋಷ್ಟಕ_iterator_free(iter);
    ಹಿಂತಿರುಗಿ ok;
}
