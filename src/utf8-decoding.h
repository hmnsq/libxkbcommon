/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#تعریف_کړه is_utf8_پیل(c) (((c) & 0xc0) != 0x80)
#تعریف_کړه INVALID_UTF8_کوډ_ټکی UINT32_MAX

uint8_t
utf8_sequence_اوږدتیا(ثابت حرف *s);

uint32_t
utf8_بل_کوډ_ټکی(ثابت حرف *s, اندازه_t max_اندازه, اندازه_t *اندازه_بهر);
