/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>
#समावेश <stdbool.h>
#समावेश <stdint.h>
#समावेश <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#घोषणा BIT_ARRAY_UINTPTR_WIDTH (माप(uintptr_t) * अक्षर_बिट)

/* Compile-time sanity checks */
#अगर_घोषित UINTPTR_WIDTH
static_assert(UINTPTR_WIDTH == BIT_ARRAY_UINTPTR_WIDTH,
              "uintptr_t contains padding bits!");
#पूर्ण_अगर
static_assert((BIT_ARRAY_UINTPTR_WIDTH & (BIT_ARRAY_UINTPTR_WIDTH - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
अटल अंतरभूत माप_प्रकार bit_array_शब्दगण(माप_प्रकार अंक_bits) {
    वापस अंक_bits / BIT_ARRAY_UINTPTR_WIDTH +
          (अंक_bits % BIT_ARRAY_UINTPTR_WIDTH != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
अटल अंतरभूत bool bit_array_get(स्थिर uintptr_t *arr, माप_प्रकार सूचकांक) {
    स्थिर माप_प्रकार word_idx = सूचकांक / BIT_ARRAY_UINTPTR_WIDTH;
    स्थिर माप_प्रकार bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_WIDTH;
    वापस (arr[word_idx] >> bit_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
अटल अंतरभूत व्योम bit_array_set(uintptr_t *arr, माप_प्रकार सूचकांक) {
    स्थिर माप_प्रकार word_idx = सूचकांक / BIT_ARRAY_UINTPTR_WIDTH;
    स्थिर माप_प्रकार bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] |= ((uintptr_t)1 << bit_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
अटल अंतरभूत व्योम bit_array_clear(uintptr_t *arr, माप_प्रकार सूचकांक) {
    स्थिर माप_प्रकार word_idx = सूचकांक / BIT_ARRAY_UINTPTR_WIDTH;
    स्थिर माप_प्रकार bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] &= ~((uintptr_t)1 << bit_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
अटल अंतरभूत व्योम bit_array_toggle(uintptr_t *arr, माप_प्रकार सूचकांक) {
    स्थिर माप_प्रकार word_idx = सूचकांक / BIT_ARRAY_UINTPTR_WIDTH;
    स्थिर माप_प्रकार bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] ^= ((uintptr_t)1 << bit_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
अटल अंतरभूत व्योम bit_array_assign(uintptr_t *arr, माप_प्रकार सूचकांक, bool val) {
    स्थिर माप_प्रकार word_idx = सूचकांक / BIT_ARRAY_UINTPTR_WIDTH;
    स्थिर माप_प्रकार bit_idx  = सूचकांक % BIT_ARRAY_UINTPTR_WIDTH;
    uintptr_t mask = (uintptr_t)1 << bit_idx;
    arr[word_idx] = (arr[word_idx] & ~mask)     /* clear bit */
                  | ((-(uintptr_t)val) & mask); /* set bit */
}

#अघोषित BIT_ARRAY_UINTPTR_WIDTH
