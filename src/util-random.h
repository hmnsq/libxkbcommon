/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdlib.h>

#जर !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#अन्तर्भाव <stdint.h>

स्थिर ओळीत शून्य
srandom(अचिन्हित पूर्णांक seed)
{
    बेक्रम(seed);
}

स्थिर ओळीत दीर्घ
random(शून्य)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    स्थिरांक दीर्घ r = ((दीर्घ)अक्रम() & 0x7fffU)
                 | (((दीर्घ)अक्रम() & 0x7fffU) << 15)
                 | (((दीर्घ)अक्रम() & 0x7fffU) << 30);

    परत r & 0x7fffffffL;
}
#endif
