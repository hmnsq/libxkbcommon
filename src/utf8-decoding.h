/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stddef.h>
#شامل <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#تعریف_کرو is_utf8_آغاز(c) (((c) & 0xc0) != 0x80)
#تعریف_کرو INVALID_UTF8_رمز_نقطہ UINT32_MAX

uint8_t
utf8_sequence_لمبائی(ثابت حرف *s);

uint32_t
utf8_اگلا_رمز_نقطہ(ثابت حرف *s, سائز_t max_سائز, سائز_t *سائز_برآمد);
