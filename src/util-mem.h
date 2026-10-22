/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdalign.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>

#ఒకవేళ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
రకనిర్వచనం నిర్మాణం {
    దీర్ఘ దీర్ఘ __max_align_ll;
    దీర్ఘ ద్విగుణ __max_align_ld;
} max_align_t;
#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య*
_steal(శూన్య *ptr) {
    శూన్య **original = (శూన్య**)ptr;
    శూన్య *swapped = *original;
    *original = ఖాళీ;
    తిరిగి swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#నిర్వచితమైతే _WIN32
#నిర్వచించు steal(ptr_) \
    _steal(ptr_)
#లేకపోతే
#నిర్వచించు steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#అయితే_ముగింపు

#నిర్వచించు XKB_సూచిక_ట్యాగ్_బిట్లు  2
#నిర్వచించు XKB_సూచిక_ట్యాగ్_ముసుగు  ((uintptr_t)((1u << XKB_సూచిక_ట్యాగ్_బిట్లు) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
స్థిర అంతఃవరుస స్థిరరాశి శూన్య *
xkb_సూచిక_ట్యాగ్(స్థిరరాశి శూన్య *ptr, uintptr_t ట్యాగ్)
{
    uintptr_t addr = (uintptr_t)ptr;
    ప్రతిజ్ఞా(ట్యాగ్ <= XKB_సూచిక_ట్యాగ్_ముసుగు &&
           "tag exceeds available bits");
    ప్రతిజ్ఞా((addr & XKB_సూచిక_ట్యాగ్_ముసుగు) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    తిరిగి (స్థిరరాశి శూన్య *)(addr | ట్యాగ్);
}

/** Recover the original pointer from a tagged pointer. */
స్థిర అంతఃవరుస స్థిరరాశి శూన్య *
xkb_సూచిక_untag(స్థిరరాశి శూన్య *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    తిరిగి (స్థిరరాశి శూన్య *)((uintptr_t)ptr & ~XKB_సూచిక_ట్యాగ్_ముసుగు);
}

/* Recover just the tag bits from a tagged pointer. */
స్థిర అంతఃవరుస uintptr_t
xkb_సూచిక_get_ట్యాగ్(స్థిరరాశి శూన్య *ptr)
{
    తిరిగి ((uintptr_t)ptr & XKB_సూచిక_ట్యాగ్_ముసుగు);
}
