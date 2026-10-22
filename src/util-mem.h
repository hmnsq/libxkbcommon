/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

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
    لما لما __max_align_ll;
    لما مزدوج __max_align_ld;
} max_align_t;
#جے_انت

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
#جے_تعریف_شدہ _WIN32
#تعریف_کرو steal(ptr_) \
    _steal(ptr_)
#وإلا
#تعریف_کرو steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#جے_انت

#تعریف_کرو XKB_اشارہ_گر_ٹیگ_بٹ  2
#تعریف_کرو XKB_اشارہ_گر_ٹیگ_نقاب  ((uintptr_t)((1u << XKB_اشارہ_گر_ٹیگ_بٹ) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساكن مضمن ثابت فراغ *
xkb_اشارہ_گر_ٹیگ(ثابت فراغ *ptr, uintptr_t ٹیگ)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(ٹیگ <= XKB_اشارہ_گر_ٹیگ_نقاب &&
           "tag exceeds available bits");
    أكد((addr & XKB_اشارہ_گر_ٹیگ_نقاب) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)(addr | ٹیگ);
}

/** Recover the original pointer from a tagged pointer. */
ساكن مضمن ثابت فراغ *
xkb_اشارہ_گر_untag(ثابت فراغ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    أرجع (ثابت فراغ *)((uintptr_t)ptr & ~XKB_اشارہ_گر_ٹیگ_نقاب);
}

/* Recover just the tag bits from a tagged pointer. */
ساكن مضمن uintptr_t
xkb_اشارہ_گر_get_ٹیگ(ثابت فراغ *ptr)
{
    أرجع ((uintptr_t)ptr & XKB_اشارہ_گر_ٹیگ_نقاب);
}
