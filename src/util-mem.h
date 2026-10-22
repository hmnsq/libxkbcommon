/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل <assert.h>
#شامل <stdalign.h>
#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>
#شامل <stdlib.h>

#اگر !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
قسمتعریف ساخت {
    طویل طویل __max_align_ll;
    طویل دوہرا __max_align_ld;
} max_align_t;
#اگر_ختم

ساکن اندرونی خالی*
_steal(خالی *ptr) {
    خالی **original = (خالی**)ptr;
    خالی *swapped = *original;
    *original = تہی;
    واپس swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#اگر_تعریف_شدہ _WIN32
#تعریف_کرو steal(ptr_) \
    _steal(ptr_)
#ورنہ
#تعریف_کرو steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#اگر_ختم

#تعریف_کرو XKB_اشارہ_گر_لیبل_بٹ  2
#تعریف_کرو XKB_اشارہ_گر_لیبل_نقاب  ((uintptr_t)((1u << XKB_اشارہ_گر_لیبل_بٹ) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ساکن اندرونی ثابت خالی *
xkb_اشارہ_گر_لیبل(ثابت خالی *ptr, uintptr_t لیبل)
{
    uintptr_t addr = (uintptr_t)ptr;
    أكد(لیبل <= XKB_اشارہ_گر_لیبل_نقاب &&
           "tag exceeds available bits");
    أكد((addr & XKB_اشارہ_گر_لیبل_نقاب) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    واپس (ثابت خالی *)(addr | لیبل);
}

/** Recover the original pointer from a tagged pointer. */
ساکن اندرونی ثابت خالی *
xkb_اشارہ_گر_untag(ثابت خالی *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    واپس (ثابت خالی *)((uintptr_t)ptr & ~XKB_اشارہ_گر_لیبل_نقاب);
}

/* Recover just the tag bits from a tagged pointer. */
ساکن اندرونی uintptr_t
xkb_اشارہ_گر_get_لیبل(ثابت خالی *ptr)
{
    واپس ((uintptr_t)ptr & XKB_اشارہ_گر_لیبل_نقاب);
}
