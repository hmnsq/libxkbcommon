/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ (ਆਕਾਰ(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ UINTPTR_ਚੌੜਾਈ
static_assert(UINTPTR_ਚੌੜਾਈ == ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ,
              "uintptr_t contains padding bits!");
#ਜੇ_ਅੰਤ
static_assert((ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ & (ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸ਼ਬਦ(ਮਾਪ_t ਅੰਕ_ਬਿੱਟ) {
    ਵਾਪਸ ਅੰਕ_ਬਿੱਟ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ +
          (ਅੰਕ_ਬਿੱਟ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਦੋਆਧਾਰੀ_ਅੰਕ_array_get(ਅਚਲ uintptr_t *arr, ਮਾਪ_t ਸੂਚਕਾਂਕ) {
    ਅਚਲ ਮਾਪ_t ਸ਼ਬਦ_idx = ਸੂਚਕਾਂਕ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਅਚਲ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_idx  = ਸੂਚਕਾਂਕ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਵਾਪਸ (arr[ਸ਼ਬਦ_idx] >> ਦੋਆਧਾਰੀ_ਅੰਕ_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੈੱਟ(uintptr_t *arr, ਮਾਪ_t ਸੂਚਕਾਂਕ) {
    ਅਚਲ ਮਾਪ_t ਸ਼ਬਦ_idx = ਸੂਚਕਾਂਕ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਅਚਲ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_idx  = ਸੂਚਕਾਂਕ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    arr[ਸ਼ਬਦ_idx] |= ((uintptr_t)1 << ਦੋਆਧਾਰੀ_ਅੰਕ_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਦੋਆਧਾਰੀ_ਅੰਕ_array_clear(uintptr_t *arr, ਮਾਪ_t ਸੂਚਕਾਂਕ) {
    ਅਚਲ ਮਾਪ_t ਸ਼ਬਦ_idx = ਸੂਚਕਾਂਕ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਅਚਲ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_idx  = ਸੂਚਕਾਂਕ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    arr[ਸ਼ਬਦ_idx] &= ~((uintptr_t)1 << ਦੋਆਧਾਰੀ_ਅੰਕ_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਦੋਆਧਾਰੀ_ਅੰਕ_array_toggle(uintptr_t *arr, ਮਾਪ_t ਸੂਚਕਾਂਕ) {
    ਅਚਲ ਮਾਪ_t ਸ਼ਬਦ_idx = ਸੂਚਕਾਂਕ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਅਚਲ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_idx  = ਸੂਚਕਾਂਕ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    arr[ਸ਼ਬਦ_idx] ^= ((uintptr_t)1 << ਦੋਆਧਾਰੀ_ਅੰਕ_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੌਂਪੋ(uintptr_t *arr, ਮਾਪ_t ਸੂਚਕਾਂਕ, bool val) {
    ਅਚਲ ਮਾਪ_t ਸ਼ਬਦ_idx = ਸੂਚਕਾਂਕ / ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    ਅਚਲ ਮਾਪ_t ਦੋਆਧਾਰੀ_ਅੰਕ_idx  = ਸੂਚਕਾਂਕ % ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ;
    uintptr_t ਨਕਾਬ = (uintptr_t)1 << ਦੋਆਧਾਰੀ_ਅੰਕ_idx;
    arr[ਸ਼ਬਦ_idx] = (arr[ਸ਼ਬਦ_idx] & ~ਨਕਾਬ)     /* clear bit */
                  | ((-(uintptr_t)val) & ਨਕਾਬ); /* set bit */
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_UINTPTR_ਚੌੜਾਈ
