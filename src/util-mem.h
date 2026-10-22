/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdalign.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>

#यदि !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
प्रकारनिर्देश संरचना {
    दीर्घ दीर्घ __max_align_ll;
    दीर्घ द्विगुण __max_align_ld;
} max_align_t;
#यदि_अन्तः

स्थिर अन्तःस्थ शून्य*
_steal(शून्य *ptr) {
    शून्य **original = (शून्य**)ptr;
    शून्य *swapped = *original;
    *original = रिक्त;
    प्रत्यागम swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#यदि_परिभाषितम् _WIN32
#परिभाषय steal(ptr_) \
    _steal(ptr_)
#अन्यथा
#परिभाषय steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#यदि_अन्तः

#परिभाषय XKB_POINTER_चिह्नकम्_द्वयङ्काः  2
#परिभाषय XKB_POINTER_चिह्नकम्_आवरणम्  ((uintptr_t)((1u << XKB_POINTER_चिह्नकम्_द्वयङ्काः) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
स्थिर अन्तःस्थ ध्रुव शून्य *
xkb_pointer_चिह्नकम्(ध्रुव शून्य *ptr, uintptr_t चिह्नकम्)
{
    uintptr_t addr = (uintptr_t)ptr;
    प्रतिज्ञा(चिह्नकम् <= XKB_POINTER_चिह्नकम्_आवरणम् &&
           "tag exceeds available bits");
    प्रतिज्ञा((addr & XKB_POINTER_चिह्नकम्_आवरणम्) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    प्रत्यागम (ध्रुव शून्य *)(addr | चिह्नकम्);
}

/** Recover the original pointer from a tagged pointer. */
स्थिर अन्तःस्थ ध्रुव शून्य *
xkb_pointer_untag(ध्रुव शून्य *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    प्रत्यागम (ध्रुव शून्य *)((uintptr_t)ptr & ~XKB_POINTER_चिह्नकम्_आवरणम्);
}

/* Recover just the tag bits from a tagged pointer. */
स्थिर अन्तःस्थ uintptr_t
xkb_pointer_get_चिह्नकम्(ध्रुव शून्य *ptr)
{
    प्रत्यागम ((uintptr_t)ptr & XKB_POINTER_चिह्नकम्_आवरणम्);
}
