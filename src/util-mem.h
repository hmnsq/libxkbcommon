/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdalign.h>
#समावेश <stdbool.h>
#समावेश <stddef.h>
#समावेश <stdint.h>
#समावेश <stdlib.h>

#अगर !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
प्रकार काष्ठा {
    दीर्घ दीर्घ __max_align_ll;
    दीर्घ द्विगुन __max_align_ld;
} max_align_t;
#पूर्ण_अगर

अटल अंतरभूत व्योम*
_steal(व्योम *ptr) {
    व्योम **original = (व्योम**)ptr;
    व्योम *swapped = *original;
    *original = शून्य;
    वापस swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#अगर_घोषित _WIN32
#घोषणा steal(ptr_) \
    _steal(ptr_)
#अन्यथा
#घोषणा steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#पूर्ण_अगर

#घोषणा XKB_POINTER_TAG_BITS  2
#घोषणा XKB_POINTER_TAG_MASK  ((uintptr_t)((1u << XKB_POINTER_TAG_BITS) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
अटल अंतरभूत स्थिर व्योम *
xkb_pointer_tag(स्थिर व्योम *ptr, uintptr_t tag)
{
    uintptr_t addr = (uintptr_t)ptr;
    निश्चित(tag <= XKB_POINTER_TAG_MASK &&
           "tag exceeds available bits");
    निश्चित((addr & XKB_POINTER_TAG_MASK) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    वापस (स्थिर व्योम *)(addr | tag);
}

/** Recover the original pointer from a tagged pointer. */
अटल अंतरभूत स्थिर व्योम *
xkb_pointer_untag(स्थिर व्योम *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    वापस (स्थिर व्योम *)((uintptr_t)ptr & ~XKB_POINTER_TAG_MASK);
}

/* Recover just the tag bits from a tagged pointer. */
अटल अंतरभूत uintptr_t
xkb_pointer_get_tag(स्थिर व्योम *ptr)
{
    वापस ((uintptr_t)ptr & XKB_POINTER_TAG_MASK);
}
