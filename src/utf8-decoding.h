/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#وصف_ڪريو is_utf8_شروعات(c) (((c) & 0xc0) != 0x80)
#وصف_ڪريو INVALID_UTF8_ڪوڊ_نقطو UINT32_MAX

uint8_t
utf8_sequence_ڊيگهائي(ثابت حرف *s);

uint32_t
utf8_ايندڙ_ڪوڊ_نقطو(ثابت حرف *s, سائيز_t max_سائيز, سائيز_t *سائيز_ٻاهريون);
