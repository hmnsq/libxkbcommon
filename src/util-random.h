/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdlib.h>

#اگر !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#شامل <stdint.h>

ساکن اندرونی خالی
srandom(غیرعلامتی صحیح بیج)
{
    srand(بیج);
}

ساکن اندرونی طویل
random(خالی)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ثابت طویل r = ((طویل)rand() & 0x7fffU)
                 | (((طویل)rand() & 0x7fffU) << 15)
                 | (((طویل)rand() & 0x7fffU) << 30);

    واپس r & 0x7fffffffL;
}
#اگر_ختم
