/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdlib.h>

#ఒకవేళ !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#అన్తర్భావ <stdint.h>

స్థిర అంతఃవరుస శూన్య
srandom(గుర్తులేని పూర్ణాంకం విత్తనం)
{
    srand(విత్తనం);
}

స్థిర అంతఃవరుస దీర్ఘ
random(శూన్య)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    స్థిరరాశి దీర్ఘ r = ((దీర్ఘ)rand() & 0x7fffU)
                 | (((దీర్ఘ)rand() & 0x7fffU) << 15)
                 | (((దీర్ఘ)rand() & 0x7fffU) << 30);

    తిరిగి r & 0x7fffffffL;
}
#అయితే_ముగింపు
