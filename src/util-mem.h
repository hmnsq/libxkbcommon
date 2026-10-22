/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdalign.h>
#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>

#ܐܢ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
تعريفنوع بنية {
    long long __max_align_ll;
    long مزدوج __max_align_ld;
} max_align_t;
#endif

ساكن مضمن فراغ*
_steal(فراغ *ptr) {
    فراغ **original = (فراغ**)ptr;
    فراغ *swapped = *original;
    *original = عدم;
    ܦܢܐ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#ܐܚܪܢܐ
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساكن مضمن ثابت فراغ *
xkb_pointer_tag(ثابت فراغ *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    أكد((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ܦܢܐ (ثابت فراغ *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
ساكن مضمن ثابت فراغ *
xkb_pointer_untag(ثابت فراغ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ܦܢܐ (ثابت فراغ *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
ساكن مضمن uintptr_t
xkb_pointer_get_tag(ثابت فراغ *ptr)
{
    ܦܢܐ ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
