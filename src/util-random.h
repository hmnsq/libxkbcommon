/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdlib.h>

#اگر !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#شامل <stdint.h>

ایستا درونخطی تهی
srandom(unsigned صحیح seed)
{
    srand(seed);
}

ایستا درونخطی long
random(تهی)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ثابت long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    بازگشت r & 0x7fffffffL;
}
#endif
