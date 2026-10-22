/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#నిర్వచించు బిట్టు_ARRAY_UINTPTR_వెడల్పు (పరిమాణం(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#నిర్వచితమైతే UINTPTR_వెడల్పు
static_assert(UINTPTR_వెడల్పు == బిట్టు_ARRAY_UINTPTR_వెడల్పు,
              "uintptr_t contains padding bits!");
#అయితే_ముగింపు
static_assert((బిట్టు_ARRAY_UINTPTR_వెడల్పు & (బిట్టు_ARRAY_UINTPTR_వెడల్పు - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
స్థిర అంతఃవరుస కొలత_t బిట్టు_array_పదాలు(కొలత_t అంకె_బిట్లు) {
    తిరిగి అంకె_బిట్లు / బిట్టు_ARRAY_UINTPTR_వెడల్పు +
          (అంకె_బిట్లు % బిట్టు_ARRAY_UINTPTR_వెడల్పు != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
స్థిర అంతఃవరుస bool బిట్టు_array_get(స్థిరరాశి uintptr_t *arr, కొలత_t సూచీ) {
    స్థిరరాశి కొలత_t మాట_idx = సూచీ / బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    స్థిరరాశి కొలత_t బిట్టు_idx  = సూచీ % బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    తిరిగి (arr[మాట_idx] >> బిట్టు_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
స్థిర అంతఃవరుస శూన్య బిట్టు_array_సమితి(uintptr_t *arr, కొలత_t సూచీ) {
    స్థిరరాశి కొలత_t మాట_idx = సూచీ / బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    స్థిరరాశి కొలత_t బిట్టు_idx  = సూచీ % బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    arr[మాట_idx] |= ((uintptr_t)1 << బిట్టు_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
స్థిర అంతఃవరుస శూన్య బిట్టు_array_clear(uintptr_t *arr, కొలత_t సూచీ) {
    స్థిరరాశి కొలత_t మాట_idx = సూచీ / బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    స్థిరరాశి కొలత_t బిట్టు_idx  = సూచీ % బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    arr[మాట_idx] &= ~((uintptr_t)1 << బిట్టు_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
స్థిర అంతఃవరుస శూన్య బిట్టు_array_toggle(uintptr_t *arr, కొలత_t సూచీ) {
    స్థిరరాశి కొలత_t మాట_idx = సూచీ / బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    స్థిరరాశి కొలత_t బిట్టు_idx  = సూచీ % బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    arr[మాట_idx] ^= ((uintptr_t)1 << బిట్టు_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
స్థిర అంతఃవరుస శూన్య బిట్టు_array_కేటాయించు(uintptr_t *arr, కొలత_t సూచీ, bool val) {
    స్థిరరాశి కొలత_t మాట_idx = సూచీ / బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    స్థిరరాశి కొలత_t బిట్టు_idx  = సూచీ % బిట్టు_ARRAY_UINTPTR_వెడల్పు;
    uintptr_t ముసుగు = (uintptr_t)1 << బిట్టు_idx;
    arr[మాట_idx] = (arr[మాట_idx] & ~ముసుగు)     /* clear bit */
                  | ((-(uintptr_t)val) & ముసుగు); /* set bit */
}

#నిర్వచనం_రద్దు బిట్టు_ARRAY_UINTPTR_వెడల్పు
