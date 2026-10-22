/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#ವ್ಯಾಖ್ಯಾನಿಸು ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ (ಗಾತ್ರ(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ UINTPTR_ಅಗಲ
static_assert(UINTPTR_ಅಗಲ == ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ,
              "uintptr_t contains padding bits!");
#ಆದರೆ_ಅಂತ್ಯ
static_assert((ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ & (ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t ಬಿಟ್_array_ಪದಗಳು(ಗಾತ್ರ_t ಅಂಕೆ_ಬಿಟ್ಟುಗಳು) {
    ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಬಿಟ್ಟುಗಳು / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ +
          (ಅಂಕೆ_ಬಿಟ್ಟುಗಳು % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಬಿಟ್_array_get(ಸ್ಥಿರಾಂಕ uintptr_t *arr, ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ) {
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಶಬ್ದ_idx = ಸೂಚ್ಯಂಕ / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಬಿಟ್_idx  = ಸೂಚ್ಯಂಕ % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಹಿಂತಿರುಗಿ (arr[ಶಬ್ದ_idx] >> ಬಿಟ್_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಬಿಟ್_array_ಗಣ(uintptr_t *arr, ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ) {
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಶಬ್ದ_idx = ಸೂಚ್ಯಂಕ / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಬಿಟ್_idx  = ಸೂಚ್ಯಂಕ % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    arr[ಶಬ್ದ_idx] |= ((uintptr_t)1 << ಬಿಟ್_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಬಿಟ್_array_clear(uintptr_t *arr, ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ) {
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಶಬ್ದ_idx = ಸೂಚ್ಯಂಕ / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಬಿಟ್_idx  = ಸೂಚ್ಯಂಕ % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    arr[ಶಬ್ದ_idx] &= ~((uintptr_t)1 << ಬಿಟ್_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಬಿಟ್_array_toggle(uintptr_t *arr, ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ) {
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಶಬ್ದ_idx = ಸೂಚ್ಯಂಕ / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಬಿಟ್_idx  = ಸೂಚ್ಯಂಕ % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    arr[ಶಬ್ದ_idx] ^= ((uintptr_t)1 << ಬಿಟ್_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಬಿಟ್_array_ನಿಯೋಜಿಸು(uintptr_t *arr, ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ, bool val) {
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಶಬ್ದ_idx = ಸೂಚ್ಯಂಕ / ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಬಿಟ್_idx  = ಸೂಚ್ಯಂಕ % ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ;
    uintptr_t ಮುಖವಾಡ = (uintptr_t)1 << ಬಿಟ್_idx;
    arr[ಶಬ್ದ_idx] = (arr[ಶಬ್ದ_idx] & ~ಮುಖವಾಡ)     /* clear bit */
                  | ((-(uintptr_t)val) & ಮುಖವಾಡ); /* set bit */
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಬಿಟ್_ARRAY_UINTPTR_ಅಗಲ
