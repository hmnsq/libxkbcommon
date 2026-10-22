/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdlib.h>

#अगर !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#समावेश <stdint.h>

अटल अंतरभूत व्योम
srandom(अचिन्हित पूर्णांक seed)
{
    बेक्रम(seed);
}

अटल अंतरभूत दीर्घ
random(व्योम)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    स्थिर दीर्घ r = ((दीर्घ)अक्रम() & 0x7fffU)
                 | (((दीर्घ)अक्रम() & 0x7fffU) << 15)
                 | (((दीर्घ)अक्रम() & 0x7fffU) << 30);

    वापस r & 0x7fffffffL;
}
#पूर्ण_अगर
