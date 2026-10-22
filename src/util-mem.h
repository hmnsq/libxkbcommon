/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdalign.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stddef.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <stdlib.h>

#যদি !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
প্রকারনির্দেশ সংরচনা {
    long long __max_align_ll;
    long দ্ৱিগুণ __max_align_ld;
} max_align_t;
#endif

স্থির অন্তঃস্থ শূন্য*
_steal(শূন্য *ptr) {
    শূন্য **original = (শূন্য**)ptr;
    শূন্য *swapped = *original;
    *original = রিক্ত;
    ঘূৰাই swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#অন্যথা
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
স্থির অন্তঃস্থ ধ্রুৱ শূন্য *
xkb_pointer_tag(ধ্রুৱ শূন্য *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    প্রতিজ্ঞা(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    প্রতিজ্ঞা((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ঘূৰাই (ধ্রুৱ শূন্য *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
স্থির অন্তঃস্থ ধ্রুৱ শূন্য *
xkb_pointer_untag(ধ্রুৱ শূন্য *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ঘূৰাই (ধ্রুৱ শূন্য *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
স্থির অন্তঃস্থ uintptr_t
xkb_pointer_get_tag(ধ্রুৱ শূন্য *ptr)
{
    ঘূৰাই ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
