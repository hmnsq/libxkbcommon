/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdlib.h>

#જો !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#અન્તર્ભાવ <stdint.h>

સ્થિર અંતઃરેખ શૂન્ય
srandom(unsigned પૂર્ણાંક seed)
{
    srand(seed);
}

સ્થિર અંતઃરેખ long
random(શૂન્ય)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    અચલ long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    પાછા r & 0x7fffffffL;
}
#endif
