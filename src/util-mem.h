/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdalign.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಯದಿ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    long long __max_align_ll;
    long ದ್ವಿಗುಣ __max_align_ld;
} max_align_t;
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ*
_steal(ಶೂನ್ಯ *ptr) {
    ಶೂನ್ಯ **original = (ಶೂನ್ಯ**)ptr;
    ಶೂನ್ಯ *swapped = *original;
    *original = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#ಅನ್ಯಥಾ
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಶೂನ್ಯ *
xkb_pointer_tag(ಧ್ರುವ ಶೂನ್ಯ *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    ಪ್ರತಿಜ್ಞಾ(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    ಪ್ರತಿಜ್ಞಾ((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ಶೂನ್ಯ *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಶೂನ್ಯ *
xkb_pointer_untag(ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ಶೂನ್ಯ *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uintptr_t
xkb_pointer_get_tag(ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
    ಪ್ರತ್ಯಾಗಮ ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
