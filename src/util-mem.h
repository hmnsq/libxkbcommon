/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdalign.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdlib.h>

#எனில் !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
வகைவரை கட்டமைப்பு {
    long long __max_align_ll;
    long இரட்டை __max_align_ld;
} max_align_t;
#endif

நிலையான உள்வரி வெறுமை*
_steal(வெறுமை *ptr) {
    வெறுமை **original = (வெறுமை**)ptr;
    வெறுமை *swapped = *original;
    *original = வெற்று;
    திரும்பு swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#இல்லையெனில்
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
நிலையான உள்வரி மாறிலி வெறுமை *
xkb_pointer_tag(மாறிலி வெறுமை *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    ப்ரதிஜ்ஞா(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    ப்ரதிஜ்ஞா((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    திரும்பு (மாறிலி வெறுமை *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
நிலையான உள்வரி மாறிலி வெறுமை *
xkb_pointer_untag(மாறிலி வெறுமை *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    திரும்பு (மாறிலி வெறுமை *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
நிலையான உள்வரி uintptr_t
xkb_pointer_get_tag(மாறிலி வெறுமை *ptr)
{
    திரும்பு ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
