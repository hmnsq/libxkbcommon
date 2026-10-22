/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdlib.h>

#إذا !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#ضمن <stdint.h>

ساكن مضمن فراغ
srandom(بیعلامت صحيح دانه)
{
    srand(دانه);
}

ساكن مضمن دراز
random(فراغ)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    ثابت دراز r = ((دراز)rand() & 0x7fffU)
                 | (((دراز)rand() & 0x7fffU) << 15)
                 | (((دراز)rand() & 0x7fffU) << 30);

    أرجع r & 0x7fffffffL;
}
#پایان_اگر
