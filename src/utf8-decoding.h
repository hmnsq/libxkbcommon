/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#تعریف_کن is_utf8_شروع(c) (((c) & 0xc0) != 0x80)
#تعریف_کن INVALID_UTF8_کد_نقطه UINT32_MAX

uint8_t
utf8_sequence_درازی(ثابت حرف *s);

uint32_t
utf8_بعدی_کد_نقطه(ثابت حرف *s, اندازه_t max_اندازه, اندازه_t *اندازه_بیرون);
