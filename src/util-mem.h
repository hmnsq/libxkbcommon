/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdalign.h>
#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>

#إذا !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
تعريفنوع بنية {
    ڊگهو ڊگهو __max_align_ll;
    ڊگهو مزدوج __max_align_ld;
} max_align_t;
#جيڪڏهن_پورو

ساكن مضمن فراغ*
_steal(فراغ *ptr) {
    فراغ **original = (فراغ**)ptr;
    فراغ *swapped = *original;
    *original = عدم;
    أرجع swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#جيڪڏهن_وصف_ٿيل _WIN32
#وصف_ڪريو steal(ptr_) \
    _steal(ptr_)
#وإلا
#وصف_ڪريو steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#جيڪڏهن_پورو

#وصف_ڪريو XKB_اشارو_ٽيگ_بٽ  2
#وصف_ڪريو XKB_اشارو_ٽيگ_نقاب  ((uintptr_t)((1u << XKB_اشارو_ٽيگ_بٽ) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساكن مضمن ثابت فراغ *
xkb_اشارو_ٽيگ(ثابت فراغ *ptr, uintptr_t ٽيگ)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(ٽيگ <= XKB_اشارو_ٽيگ_نقاب &&
           "tag exceeds available bits");
    أكد((addr & XKB_اشارو_ٽيگ_نقاب) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)(addr | ٽيگ);
}

/** Recover the original pointer from a tagged pointer. */
ساكن مضمن ثابت فراغ *
xkb_اشارو_untag(ثابت فراغ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)((uintptr_t)ptr & ~XKB_اشارو_ٽيگ_نقاب);
}

/* Recover just the tag bits from a tagged pointer. */
ساكن مضمن uintptr_t
xkb_اشارو_get_ٽيگ(ثابت فراغ *ptr)
{
    أرجع ((uintptr_t)ptr & XKB_اشارو_ٽيگ_نقاب);
}
