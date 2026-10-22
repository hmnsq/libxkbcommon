/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdlib.h>

#यदि !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#अन्तर्भाव <stdint.h>

स्थिर अन्तःस्थ शून्य
srandom(अचिह्नित पूर्णाङ्क बीजम्)
{
    बीजं_रोपय(बीजम्);
}

स्थिर अन्तःस्थ दीर्घ
random(शून्य)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ध्रुव दीर्घ r = ((दीर्घ)यादृच्छिकम्() & 0x7fffU)
                 | (((दीर्घ)यादृच्छिकम्() & 0x7fffU) << 15)
                 | (((दीर्घ)यादृच्छिकम्() & 0x7fffU) << 30);

    प्रत्यागम r & 0x7fffffffL;
}
#यदि_अन्तः
