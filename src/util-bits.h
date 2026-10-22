/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#परिभाषय BIT_ARRAY_UINTPTR_विस्तारः (परिमाण(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#यदि_परिभाषितम् UINTPTR_विस्तारः
static_assert(UINTPTR_विस्तारः == BIT_ARRAY_UINTPTR_विस्तारः,
              "uintptr_t contains padding bits!");
#यदि_अन्तः
static_assert((BIT_ARRAY_UINTPTR_विस्तारः & (BIT_ARRAY_UINTPTR_विस्तारः - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
स्थिर अन्तःस्थ आकार_t bit_array_शब्दाः(आकार_t अङ्कः_द्वयङ्काः) {
    प्रत्यागम अङ्कः_द्वयङ्काः / BIT_ARRAY_UINTPTR_विस्तारः +
          (अङ्कः_द्वयङ्काः % BIT_ARRAY_UINTPTR_विस्तारः != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
स्थिर अन्तःस्थ bool bit_array_get(ध्रुव uintptr_t *arr, आकार_t सूचकांक) {
    ध्रुव आकार_t word_idx = सूचकांक / BIT_ARRAY_UINTPTR_विस्तारः;
    ध्रुव आकार_t bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_विस्तारः;
    प्रत्यागम (arr[word_idx] >> bit_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
स्थिर अन्तःस्थ शून्य bit_array_समुच्चयः(uintptr_t *arr, आकार_t सूचकांक) {
    ध्रुव आकार_t word_idx = सूचकांक / BIT_ARRAY_UINTPTR_विस्तारः;
    ध्रुव आकार_t bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_विस्तारः;
    arr[word_idx] |= ((uintptr_t)1 << bit_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
स्थिर अन्तःस्थ शून्य bit_array_clear(uintptr_t *arr, आकार_t सूचकांक) {
    ध्रुव आकार_t word_idx = सूचकांक / BIT_ARRAY_UINTPTR_विस्तारः;
    ध्रुव आकार_t bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_विस्तारः;
    arr[word_idx] &= ~((uintptr_t)1 << bit_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
स्थिर अन्तःस्थ शून्य bit_array_toggle(uintptr_t *arr, आकार_t सूचकांक) {
    ध्रुव आकार_t word_idx = सूचकांक / BIT_ARRAY_UINTPTR_विस्तारः;
    ध्रुव आकार_t bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_विस्तारः;
    arr[word_idx] ^= ((uintptr_t)1 << bit_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
स्थिर अन्तःस्थ शून्य bit_array_नियोजय(uintptr_t *arr, आकार_t सूचकांक, bool val) {
    ध्रुव आकार_t word_idx = सूचकांक / BIT_ARRAY_UINTPTR_विस्तारः;
    ध्रुव आकार_t bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_विस्तारः;
    uintptr_t आवरणम् = (uintptr_t)1 << bit_idx;
    arr[word_idx] = (arr[word_idx] & ~आवरणम्)     /* clear bit */
                  | ((-(uintptr_t)val) & आवरणम्); /* set bit */
}

#अपरिभाषय BIT_ARRAY_UINTPTR_विस्तारः
