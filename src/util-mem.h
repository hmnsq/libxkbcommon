/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdalign.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stdlib.h>

#જો !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
પ્રકારવ્યાખ્યા સંરચના {
    long long __max_align_ll;
    long દ્વિગુણ __max_align_ld;
} max_align_t;
#endif

સ્થિર અંતઃરેખ શૂન્ય*
_steal(શૂન્ય *ptr) {
    શૂન્ય **original = (શૂન્ય**)ptr;
    શૂન્ય *swapped = *original;
    *original = ખાલી;
    પાછા swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#નહીંતર
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
સ્થિર અંતઃરેખ અચલ શૂન્ય *
xkb_pointer_tag(અચલ શૂન્ય *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    પ્રતિજ્ઞા(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    પ્રતિજ્ઞા((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    પાછા (અચલ શૂન્ય *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
સ્થિર અંતઃરેખ અચલ શૂન્ય *
xkb_pointer_untag(અચલ શૂન્ય *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    પાછા (અચલ શૂન્ય *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
સ્થિર અંતઃરેખ uintptr_t
xkb_pointer_get_tag(અચલ શૂન્ય *ptr)
{
    પાછા ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
