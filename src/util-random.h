/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdlib.h>

#যদি !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#অন্তর্ভাব <stdint.h>

স্থির অন্তঃরেখ শূন্য
srandom(unsigned পূর্ণসংখ্যা seed)
{
    srand(seed);
}

স্থির অন্তঃরেখ long
random(শূন্য)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ধ্রুবক long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    ফেরত r & 0x7fffffffL;
}
#endif
