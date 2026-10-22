/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <stdlib.h>

#𐡄𐡍 !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ضمن <stdint.h>

ساكن مضمن فراغ
srandom(unsigned صحيح seed)
{
    srand(seed);
}

ساكن مضمن long
random(فراغ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ثابت long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    أرجع r & 0x7fffffffL;
}
#endif
