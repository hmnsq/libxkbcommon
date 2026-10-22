/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#تعریف_کرو ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی (حجم(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#جے_تعریف_شدہ UINTPTR_چوڑائی
static_assert(UINTPTR_چوڑائی == ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی,
              "uintptr_t contains padding bits!");
#جے_انت
static_assert((ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی & (ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ساكن مضمن ناپ_t ثنائی_ہندسہ_array_الفاظ(ناپ_t عدد_بٹ) {
    أرجع عدد_بٹ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی +
          (عدد_بٹ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن bool ثنائی_ہندسہ_array_get(ثابت uintptr_t *arr, ناپ_t اشاریہ) {
    ثابت ناپ_t لفظ_idx = اشاریہ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    ثابت ناپ_t ثنائی_ہندسہ_idx  = اشاریہ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    أرجع (arr[لفظ_idx] >> ثنائی_ہندسہ_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ثنائی_ہندسہ_array_سیٹ(uintptr_t *arr, ناپ_t اشاریہ) {
    ثابت ناپ_t لفظ_idx = اشاریہ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    ثابت ناپ_t ثنائی_ہندسہ_idx  = اشاریہ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    arr[لفظ_idx] |= ((uintptr_t)1 << ثنائی_ہندسہ_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ثنائی_ہندسہ_array_clear(uintptr_t *arr, ناپ_t اشاریہ) {
    ثابت ناپ_t لفظ_idx = اشاریہ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    ثابت ناپ_t ثنائی_ہندسہ_idx  = اشاریہ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    arr[لفظ_idx] &= ~((uintptr_t)1 << ثنائی_ہندسہ_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ثنائی_ہندسہ_array_toggle(uintptr_t *arr, ناپ_t اشاریہ) {
    ثابت ناپ_t لفظ_idx = اشاریہ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    ثابت ناپ_t ثنائی_ہندسہ_idx  = اشاریہ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    arr[لفظ_idx] ^= ((uintptr_t)1 << ثنائی_ہندسہ_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ ثنائی_ہندسہ_array_سونپو(uintptr_t *arr, ناپ_t اشاریہ, bool val) {
    ثابت ناپ_t لفظ_idx = اشاریہ / ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    ثابت ناپ_t ثنائی_ہندسہ_idx  = اشاریہ % ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی;
    uintptr_t نقاب = (uintptr_t)1 << ثنائی_ہندسہ_idx;
    arr[لفظ_idx] = (arr[لفظ_idx] & ~نقاب)     /* clear bit */
                  | ((-(uintptr_t)val) & نقاب); /* set bit */
}

#تعریف_رد ثنائی_ہندسہ_ARRAY_UINTPTR_چوڑائی
