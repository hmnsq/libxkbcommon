/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

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
    اوږد اوږد __max_align_ll;
    اوږد مزدوج __max_align_ld;
} max_align_t;
#د_که_پای

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
#که_تعریف_شوی _WIN32
#تعریف_کړه steal(ptr_) \
    _steal(ptr_)
#وإلا
#تعریف_کړه steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#د_که_پای

#تعریف_کړه XKB_اشاره_ګر_نښان_بټونه  2
#تعریف_کړه XKB_اشاره_ګر_نښان_ماسک  ((uintptr_t)((1u << XKB_اشاره_ګر_نښان_بټونه) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساكن مضمن ثابت فراغ *
xkb_اشاره_ګر_نښان(ثابت فراغ *ptr, uintptr_t نښان)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(نښان <= XKB_اشاره_ګر_نښان_ماسک &&
           "tag exceeds available bits");
    أكد((addr & XKB_اشاره_ګر_نښان_ماسک) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)(addr | نښان);
}

/** Recover the original pointer from a tagged pointer. */
ساكن مضمن ثابت فراغ *
xkb_اشاره_ګر_untag(ثابت فراغ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)((uintptr_t)ptr & ~XKB_اشاره_ګر_نښان_ماسک);
}

/* Recover just the tag bits from a tagged pointer. */
ساكن مضمن uintptr_t
xkb_اشاره_ګر_get_نښان(ثابت فراغ *ptr)
{
    أرجع ((uintptr_t)ptr & XKB_اشاره_ګر_نښان_ماسک);
}
