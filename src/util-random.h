/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdlib.h>

#எனில் !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#அந்தர்பாவ <stdint.h>

நிலையான உள்வரி வெறுமை
srandom(unsigned முழு seed)
{
    srand(seed);
}

நிலையான உள்வரி long
random(வெறுமை)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    மாறிலி long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    திரும்பு r & 0x7fffffffL;
}
#endif
