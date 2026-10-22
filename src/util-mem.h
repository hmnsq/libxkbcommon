/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdalign.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಆದರೆ !HAVE_MAX_ALIGN_T
/* Fallback for legacy compilers or older C modes on MSVC */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ದೀರ್ಘ ದೀರ್ಘ __max_align_ll;
    ದೀರ್ಘ ದ್ವಿಗುಣ __max_align_ld;
} max_align_t;
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ*
_steal(ಶೂನ್ಯ *ptr) {
    ಶೂನ್ಯ **original = (ಶೂನ್ಯ**)ptr;
    ಶೂನ್ಯ *swapped = *original;
    *original = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ swapped;
}

/**
 * Resets the pointer content and resets the data to NULL.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿಸು steal(ptr_) \
    _steal(ptr_)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು steal(ptr_) \
    (__typeof__(*(ptr_)))_steal(ptr_)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಬಿಟ್ಟುಗಳು  2
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ  ((uintptr_t)((1u << XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಬಿಟ್ಟುಗಳು) - 1u))

static_assert(alignof(max_align_t) >= 4,
              "Pointer alignment insufficient for tagging");

/** Pack @p ptr and a 0-3 @p tag. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *
xkb_ಸೂಚಕ_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr, uintptr_t ಟ್ಯಾಗ್)
{
    uintptr_t addr = (uintptr_t)ptr;
    ಪ್ರತಿಜ್ಞಾ(ಟ್ಯಾಗ್ <= XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ &&
           "tag exceeds available bits");
    ಪ್ರತಿಜ್ಞಾ((addr & XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ) == 0 &&
           "pointer not sufficiently aligned for tagging");
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ಹಿಂತಿರುಗಿ (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)(addr | ಟ್ಯಾಗ್);
}

/** Recover the original pointer from a tagged pointer. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *
xkb_ಸೂಚಕ_untag(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    ಹಿಂತಿರುಗಿ (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)((uintptr_t)ptr & ~XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ);
}

/* Recover just the tag bits from a tagged pointer. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ uintptr_t
xkb_ಸೂಚಕ_get_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
    ಹಿಂತಿರುಗಿ ((uintptr_t)ptr & XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ);
}
