/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#تعریف_کړه بټ_ARRAY_UINTPTR_پلنوالی (حجم(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#که_تعریف_شوی UINTPTR_پلنوالی
static_assert(UINTPTR_پلنوالی == بټ_ARRAY_UINTPTR_پلنوالی,
              "uintptr_t contains padding bits!");
#د_که_پای
static_assert((بټ_ARRAY_UINTPTR_پلنوالی & (بټ_ARRAY_UINTPTR_پلنوالی - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ساكن مضمن اندازه_t بټ_array_ټکي(اندازه_t عدد_بټونه) {
    أرجع عدد_بټونه / بټ_ARRAY_UINTPTR_پلنوالی +
          (عدد_بټونه % بټ_ARRAY_UINTPTR_پلنوالی != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن bool بټ_array_get(ثابت uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بټ_ARRAY_UINTPTR_پلنوالی;
    ثابت اندازه_t بټ_idx  = شاخص % بټ_ARRAY_UINTPTR_پلنوالی;
    أرجع (arr[کلمه_idx] >> بټ_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بټ_array_سېټ(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بټ_ARRAY_UINTPTR_پلنوالی;
    ثابت اندازه_t بټ_idx  = شاخص % بټ_ARRAY_UINTPTR_پلنوالی;
    arr[کلمه_idx] |= ((uintptr_t)1 << بټ_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بټ_array_clear(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بټ_ARRAY_UINTPTR_پلنوالی;
    ثابت اندازه_t بټ_idx  = شاخص % بټ_ARRAY_UINTPTR_پلنوالی;
    arr[کلمه_idx] &= ~((uintptr_t)1 << بټ_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بټ_array_toggle(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بټ_ARRAY_UINTPTR_پلنوالی;
    ثابت اندازه_t بټ_idx  = شاخص % بټ_ARRAY_UINTPTR_پلنوالی;
    arr[کلمه_idx] ^= ((uintptr_t)1 << بټ_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بټ_array_ټاکل(uintptr_t *arr, اندازه_t شاخص, bool val) {
    ثابت اندازه_t کلمه_idx = شاخص / بټ_ARRAY_UINTPTR_پلنوالی;
    ثابت اندازه_t بټ_idx  = شاخص % بټ_ARRAY_UINTPTR_پلنوالی;
    uintptr_t ماسک = (uintptr_t)1 << بټ_idx;
    arr[کلمه_idx] = (arr[کلمه_idx] & ~ماسک)     /* clear bit */
                  | ((-(uintptr_t)val) & ماسک); /* set bit */
}

#تعریف_لغوه بټ_ARRAY_UINTPTR_پلنوالی
