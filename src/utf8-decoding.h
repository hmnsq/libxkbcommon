/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#define is_utf8_start(c) (((c) & 0xc0) != 0x80)
#define INVALID_UTF8_CODE_POINT UINT32_MAX

uint8_t
utf8_sequence_length(अचर वर्ण *s);

uint32_t
utf8_next_code_point(अचर वर्ण *s, माप_प्रकार max_size, माप_प्रकार *size_out);
