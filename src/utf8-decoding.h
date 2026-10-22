/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stddef.h>
#समावेश <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#घोषणा is_utf8_start(c) (((c) & 0xc0) != 0x80)
#घोषणा INVALID_UTF8_CODE_POINT UINT32_MAX

uint8_t
utf8_sequence_लंबाई(स्थिर अक्षर *s);

uint32_t
utf8_अग्र_code_point(स्थिर अक्षर *s, माप_प्रकार max_आकार, माप_प्रकार *आकार_out);
