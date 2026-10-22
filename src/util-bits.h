/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#define BIT_ARRAY_UINTPTR_WIDTH (કદ(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#ifdef UINTPTR_WIDTH
static_assert(UINTPTR_WIDTH == BIT_ARRAY_UINTPTR_WIDTH,
              "uintptr_t contains padding bits!");
#endif
static_assert((BIT_ARRAY_UINTPTR_WIDTH & (BIT_ARRAY_UINTPTR_WIDTH - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
સ્થિર અંતઃરેખ size_t bit_array_શબ્દો(size_t અંક_bits) {
    પાછા અંક_bits / BIT_ARRAY_UINTPTR_WIDTH +
          (અંક_bits % BIT_ARRAY_UINTPTR_WIDTH != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
સ્થિર અંતઃરેખ bool bit_array_get(અચલ uintptr_t *arr, size_t index) {
    અચલ size_t word_idx = index / BIT_ARRAY_UINTPTR_WIDTH;
    અચલ size_t bit_idx  = index % BIT_ARRAY_UINTPTR_WIDTH;
    પાછા (arr[word_idx] >> bit_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
સ્થિર અંતઃરેખ શૂન્ય bit_array_set(uintptr_t *arr, size_t index) {
    અચલ size_t word_idx = index / BIT_ARRAY_UINTPTR_WIDTH;
    અચલ size_t bit_idx  = index % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] |= ((uintptr_t)1 << bit_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
સ્થિર અંતઃરેખ શૂન્ય bit_array_clear(uintptr_t *arr, size_t index) {
    અચલ size_t word_idx = index / BIT_ARRAY_UINTPTR_WIDTH;
    અચલ size_t bit_idx  = index % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] &= ~((uintptr_t)1 << bit_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
સ્થિર અંતઃરેખ શૂન્ય bit_array_toggle(uintptr_t *arr, size_t index) {
    અચલ size_t word_idx = index / BIT_ARRAY_UINTPTR_WIDTH;
    અચલ size_t bit_idx  = index % BIT_ARRAY_UINTPTR_WIDTH;
    arr[word_idx] ^= ((uintptr_t)1 << bit_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
સ્થિર અંતઃરેખ શૂન્ય bit_array_assign(uintptr_t *arr, size_t index, bool val) {
    અચલ size_t word_idx = index / BIT_ARRAY_UINTPTR_WIDTH;
    અચલ size_t bit_idx  = index % BIT_ARRAY_UINTPTR_WIDTH;
    uintptr_t mask = (uintptr_t)1 << bit_idx;
    arr[word_idx] = (arr[word_idx] & ~mask)     /* clear bit */
                  | ((-(uintptr_t)val) & mask); /* set bit */
}

#undef BIT_ARRAY_UINTPTR_WIDTH
