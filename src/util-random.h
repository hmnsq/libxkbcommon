/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଯଦି !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ଅନ୍ତର୍ଭାଵ <stdint.h>

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
srandom(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seed)
{
    srand(seed);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ long
random(ଶୂନ୍ୟ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ଅଚଳ long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    ଫେରନ୍ତୁ r & 0x7fffffffL;
}
#endif
