/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#وصف_ڪريو ٻٽو_انگ_ARRAY_UINTPTR_ويڪر (حجم(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#جيڪڏهن_وصف_ٿيل UINTPTR_ويڪر
static_assert(UINTPTR_ويڪر == ٻٽو_انگ_ARRAY_UINTPTR_ويڪر,
              "uintptr_t contains padding bits!");
#جيڪڏهن_پورو
static_assert((ٻٽو_انگ_ARRAY_UINTPTR_ويڪر & (ٻٽو_انگ_ARRAY_UINTPTR_ويڪر - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ساكن مضمن سائيز_t ٻٽو_انگ_array_لفظ(سائيز_t انگ_بٽ) {
    أرجع انگ_بٽ / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر +
          (انگ_بٽ % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن bool ٻٽو_انگ_array_get(ثابت uintptr_t *arr, سائيز_t اشاريو) {
    ثابت سائيز_t لفظ_idx = اشاريو / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    ثابت سائيز_t ٻٽو_انگ_idx  = اشاريو % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    أرجع (arr[لفظ_idx] >> ٻٽو_انگ_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ٻٽو_انگ_array_سيٽ(uintptr_t *arr, سائيز_t اشاريو) {
    ثابت سائيز_t لفظ_idx = اشاريو / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    ثابت سائيز_t ٻٽو_انگ_idx  = اشاريو % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    arr[لفظ_idx] |= ((uintptr_t)1 << ٻٽو_انگ_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ٻٽو_انگ_array_clear(uintptr_t *arr, سائيز_t اشاريو) {
    ثابت سائيز_t لفظ_idx = اشاريو / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    ثابت سائيز_t ٻٽو_انگ_idx  = اشاريو % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    arr[لفظ_idx] &= ~((uintptr_t)1 << ٻٽو_انگ_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ٻٽو_انگ_array_toggle(uintptr_t *arr, سائيز_t اشاريو) {
    ثابت سائيز_t لفظ_idx = اشاريو / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    ثابت سائيز_t ٻٽو_انگ_idx  = اشاريو % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    arr[لفظ_idx] ^= ((uintptr_t)1 << ٻٽو_انگ_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ٻٽو_انگ_array_سونپيو(uintptr_t *arr, سائيز_t اشاريو, bool val) {
    ثابت سائيز_t لفظ_idx = اشاريو / ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    ثابت سائيز_t ٻٽو_انگ_idx  = اشاريو % ٻٽو_انگ_ARRAY_UINTPTR_ويڪر;
    uintptr_t نقاب = (uintptr_t)1 << ٻٽو_انگ_idx;
    arr[لفظ_idx] = (arr[لفظ_idx] & ~نقاب)     /* clear bit */
                  | ((-(uintptr_t)val) & نقاب); /* set bit */
}

#وصف_رد ٻٽو_انگ_ARRAY_UINTPTR_ويڪر
