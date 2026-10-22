/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਜੇ !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
srandom(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬੀਜ)
{
    srand(ਬੀਜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ
random(ਖਾਲੀ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ਅਚਲ ਲੰਮਾ r = ((ਲੰਮਾ)rand() & 0x7fffU)
                 | (((ਲੰਮਾ)rand() & 0x7fffU) << 15)
                 | (((ਲੰਮਾ)rand() & 0x7fffU) << 30);

    ਵਾਪਸ r & 0x7fffffffL;
}
#ਜੇ_ਅੰਤ
