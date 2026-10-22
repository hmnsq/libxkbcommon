/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdalign.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h>

#എങ്കിൽ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
തരനിർവചനം ഘടന {
    long long __max_align_ll;
    long ഇരട്ട __max_align_ld;
} max_align_t;
#endif

സ്ഥിതം വരിയിൽ ശൂന്യം*
_steal(ശൂന്യം *ptr) {
    ശൂന്യം **original = (ശൂന്യം**)ptr;
    ശൂന്യം *swapped = *original;
    *original = ശൂന്യ;
    തിരികെ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ifdef _WIN32
#define steal(ptr_) \
    _steal(ptr_)
#അല്ലെങ്കിൽ
#define steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#endif

#define XKB_POINTER_TAG_BITS  2
#define XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
സ്ഥിതം വരിയിൽ സ്ഥിരം ശൂന്യം *
xkb_pointer_tag(സ്ഥിരം ശൂന്യം *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    പ്രതിജ്ഞാ(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    പ്രതിജ്ഞാ((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    തിരികെ (സ്ഥിരം ശൂന്യം *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
സ്ഥിതം വരിയിൽ സ്ഥിരം ശൂന്യം *
xkb_pointer_untag(സ്ഥിരം ശൂന്യം *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    തിരികെ (സ്ഥിരം ശൂന്യം *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
സ്ഥിതം വരിയിൽ uintptr_t
xkb_pointer_get_tag(സ്ഥിരം ശൂന്യം *ptr)
{
    തിരികെ ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
