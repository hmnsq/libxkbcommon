/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

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
    دراز دراز __max_align_ll;
    دراز مزدوج __max_align_ld;
} max_align_t;
#پایان_اگر

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
#اگر_تعریف_شده _WIN32
#تعریف_کن steal(ptr_) \
    _steal(ptr_)
#وإلا
#تعریف_کن steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#پایان_اگر

#تعریف_کن XKB_اشاره_گر_برچسب_بیتها  2
#تعریف_کن XKB_اشاره_گر_برچسب_ماسک  ((uintptr_t)((1u << XKB_اشاره_گر_برچسب_بیتها) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساكن مضمن ثابت فراغ *
xkb_اشاره_گر_برچسب(ثابت فراغ *ptr, uintptr_t برچسب)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(برچسب <= XKB_اشاره_گر_برچسب_ماسک &&
           "tag exceeds available bits");
    أكد((addr & XKB_اشاره_گر_برچسب_ماسک) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)(addr | برچسب);
}

/** Recover the original pointer from a tagged pointer. */
ساكن مضمن ثابت فراغ *
xkb_اشاره_گر_untag(ثابت فراغ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)((uintptr_t)ptr & ~XKB_اشاره_گر_برچسب_ماسک);
}

/* Recover just the tag bits from a tagged pointer. */
ساكن مضمن uintptr_t
xkb_اشاره_گر_get_برچسب(ثابت فراغ *ptr)
{
    أرجع ((uintptr_t)ptr & XKB_اشاره_گر_برچسب_ماسک);
}
