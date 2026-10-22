/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#تعریف_کرو is_utf8_شروع(c) (((c) & 0xc0) != 0x80)
#تعریف_کرو INVALID_UTF8_کوڈ_نقطہ UINT32_MAX

uint8_t
utf8_sequence_لمائی(ثابت حرف *s);

uint32_t
utf8_اگلا_کوڈ_نقطہ(ثابت حرف *s, ناپ_t max_ناپ, ناپ_t *ناپ_باہرلا);
