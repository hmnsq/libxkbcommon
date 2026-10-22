/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdalign.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਜੇ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਲੰਮਾ ਲੰਮਾ __max_align_ll;
    ਲੰਮਾ ਦੁੱਗਣਾ __max_align_ld;
} max_align_t;
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ*
_steal(ਖਾਲੀ *ptr) {
    ਖਾਲੀ **original = (ਖਾਲੀ**)ptr;
    ਖਾਲੀ *swapped = *original;
    *original = ਨਿਰਾ;
    ਵਾਪਸ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ steal(ptr_) \
    _steal(ptr_)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_ਸੂਚਕ_ਟੈਗ_ਬਿੱਟ  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ  ((uintptr_t)((1u << XKB_ਸੂਚਕ_ਟੈਗ_ਬਿੱਟ) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਖਾਲੀ *
xkb_ਸੂਚਕ_ਟੈਗ(ਅਚਲ ਖਾਲੀ *ptr, uintptr_t ਟੈਗ)
{
    uintptr_t addr = (uintptr_t)ptr;
    ਪ੍ਰਤਿਜ੍ਞਾ(ਟੈਗ <= XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ &&
           "tag exceeds available bits");
    ਪ੍ਰਤਿਜ੍ਞਾ((addr & XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ਵਾਪਸ (ਅਚਲ ਖਾਲੀ *)(addr | ਟੈਗ);
}

/** Recover the original pointer from a tagged pointer. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਖਾਲੀ *
xkb_ਸੂਚਕ_untag(ਅਚਲ ਖਾਲੀ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ਵਾਪਸ (ਅਚਲ ਖਾਲੀ *)((uintptr_t)ptr & ~XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ);
}

/* Recover just the tag bits from a tagged pointer. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uintptr_t
xkb_ਸੂਚਕ_get_ਟੈਗ(ਅਚਲ ਖਾਲੀ *ptr)
{
    ਵਾਪਸ ((uintptr_t)ptr & XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ);
}
