/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#شامل "config.h"

#شامل <assert.h>
#شامل <stdalign.h>
#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>
#شامل <stdlib.h>

#اگر !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
تعریفنوع ساختار {
    long long __max_align_ll;
    long دوگانه __max_align_ld;
} max_align_t;
#endif

ایستا درونخطی تهی*
_steal(تهی *ptr) {
    تهی **original = (تهی**)ptr;
    تهی *swapped = *original;
    *original = پوچ;
    بازگشت swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#وگرنه
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ایستا درونخطی ثابت تهی *
xkb_pointer_tag(ثابت تهی *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    أكد((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    بازگشت (ثابت تهی *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
ایستا درونخطی ثابت تهی *
xkb_pointer_untag(ثابت تهی *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    بازگشت (ثابت تهی *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
ایستا درونخطی uintptr_t
xkb_pointer_get_tag(ثابت تهی *ptr)
{
    بازگشت ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
