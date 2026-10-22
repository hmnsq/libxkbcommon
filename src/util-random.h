/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಆದರೆ !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ಅನ್ತರ್ಭಾವ <stdint.h>

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
srandom(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬೀಜ)
{
    srand(ಬೀಜ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ
random(ಶೂನ್ಯ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ r = ((ದೀರ್ಘ)rand() & 0x7fffU)
                 | (((ದೀರ್ಘ)rand() & 0x7fffU) << 15)
                 | (((ದೀರ್ಘ)rand() & 0x7fffU) << 30);

    ಹಿಂತಿರುಗಿ r & 0x7fffffffL;
}
#ಆದರೆ_ಅಂತ್ಯ
