/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdalign.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଯଦି !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    long long __max_align_ll;
    long ଦ୍ୱିଗୁଣ __max_align_ld;
} max_align_t;
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ*
_steal(ଶୂନ୍ୟ *ptr) {
    ଶୂନ୍ୟ **original = (ଶୂନ୍ୟ**)ptr;
    ଶୂନ୍ୟ *swapped = *original;
    *original = ଖାଲି;
    ଫେରନ୍ତୁ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#ଅନ୍ୟଥା
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଶୂନ୍ୟ *
xkb_pointer_tag(ଅଚଳ ଶୂନ୍ୟ *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    ପ୍ରତିଜ୍ଞା(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    ପ୍ରତିଜ୍ଞା((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ଫେରନ୍ତୁ (ଅଚଳ ଶୂନ୍ୟ *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଶୂନ୍ୟ *
xkb_pointer_untag(ଅଚଳ ଶୂନ୍ୟ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ଫେରନ୍ତୁ (ଅଚଳ ଶୂନ୍ୟ *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uintptr_t
xkb_pointer_get_tag(ଅଚଳ ଶୂନ୍ୟ *ptr)
{
    ଫେରନ୍ତୁ ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
