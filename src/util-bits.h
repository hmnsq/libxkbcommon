/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <stddef.h>

/*
 * Bit array using native pointer-width word for optimal performance
 */

#تعریف_کن بیت_ARRAY_UINTPTR_عرض (حجم(uintptr_t) * CHAR_BIT)

/* Compile-time sanity checks */
#اگر_تعریف_شده UINTPTR_عرض
static_assert(UINTPTR_عرض == بیت_ARRAY_UINTPTR_عرض,
              "uintptr_t contains padding bits!");
#پایان_اگر
static_assert((بیت_ARRAY_UINTPTR_عرض & (بیت_ARRAY_UINTPTR_عرض - 1)) == 0,
              "BIT_ARRAY_UINTPTR_WIDTH must be a power of 2!");

/**
 * Calculates the exact number of words required to hold num_bits.
 *
 * @param num_bits  Number of bits to store.
 * @return Number of `uintptr_t` words required.
 */
ساكن مضمن اندازه_t بیت_array_واژگان(اندازه_t رقم_بیتها) {
    أرجع رقم_بیتها / بیت_ARRAY_UINTPTR_عرض +
          (رقم_بیتها % بیت_ARRAY_UINTPTR_عرض != 0);
}

/**
 * Returns the boolean value (true/false) at the specified bit index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن bool بیت_array_get(ثابت uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بیت_ARRAY_UINTPTR_عرض;
    ثابت اندازه_t بیت_idx  = شاخص % بیت_ARRAY_UINTPTR_عرض;
    أرجع (arr[کلمه_idx] >> بیت_idx) & (uintptr_t)1;
}

/**
 * Sets the bit at index to 1 (true).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بیت_array_مجموعه(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بیت_ARRAY_UINTPTR_عرض;
    ثابت اندازه_t بیت_idx  = شاخص % بیت_ARRAY_UINTPTR_عرض;
    arr[کلمه_idx] |= ((uintptr_t)1 << بیت_idx);
}

/**
 * Clears the bit at index to 0 (false).
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بیت_array_clear(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بیت_ARRAY_UINTPTR_عرض;
    ثابت اندازه_t بیت_idx  = شاخص % بیت_ARRAY_UINTPTR_عرض;
    arr[کلمه_idx] &= ~((uintptr_t)1 << بیت_idx);
}

/**
 * Flips/toggles the bit at index.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بیت_array_toggle(uintptr_t *arr, اندازه_t شاخص) {
    ثابت اندازه_t کلمه_idx = شاخص / بیت_ARRAY_UINTPTR_عرض;
    ثابت اندازه_t بیت_idx  = شاخص % بیت_ARRAY_UINTPTR_عرض;
    arr[کلمه_idx] ^= ((uintptr_t)1 << بیت_idx);
}

/**
 * Sets or clears the bit at index based on a boolean value.
 *
 * @pre @p index must be less than the number of bits represented by arr.
 */
ساكن مضمن فراغ بیت_array_تخصیص_بده(uintptr_t *arr, اندازه_t شاخص, bool val) {
    ثابت اندازه_t کلمه_idx = شاخص / بیت_ARRAY_UINTPTR_عرض;
    ثابت اندازه_t بیت_idx  = شاخص % بیت_ARRAY_UINTPTR_عرض;
    uintptr_t ماسک = (uintptr_t)1 << بیت_idx;
    arr[کلمه_idx] = (arr[کلمه_idx] & ~ماسک)     /* clear bit */
                  | ((-(uintptr_t)val) & ماسک); /* set bit */
}

#لغو_تعریف بیت_ARRAY_UINTPTR_عرض
