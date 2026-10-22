/*
 * Copyright © 2026 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdlib.h>

#എങ്കിൽ !HAVE_RANDOM

/* Simple random() fallback based on rand() */

#അന്തര്ഭാവ <stdint.h>

സ്ഥിതം വരിയിൽ ശൂന്യം
srandom(unsigned പൂർണ്ണസംഖ്യ seed)
{
    srand(seed);
}

സ്ഥിതം വരിയിൽ long
random(ശൂന്യം)
{
    /* rand() is only guaranteed to cover [0, 32767] (15 bits), so
     * combine three calls to safely fill 31 bits regardless of the
     * platform's actual RAND_MAX. */
    സ്ഥിരം long r = ((long)rand() & 0x7fffU)
                 | (((long)rand() & 0x7fffU) << 15)
                 | (((long)rand() & 0x7fffU) << 30);

    തിരികെ r & 0x7fffffffL;
}
#endif
