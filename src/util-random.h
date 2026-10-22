/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಯದಿ !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ಅನ್ತರ್ಭಾವ <stdint.h>

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
srandom(unsigned ಪೂರ್ಣಾಙ್ಕ seed)
{
    srand(seed);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long
random(ಶೂನ್ಯ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ಧ್ರುವ long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    ಪ್ರತ್ಯಾಗಮ r & 0x7fffffffL;
}
#endif
