/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#define is_utf8_start(c) (((c) & 0xc0) != 0x80)
#define INVALID_UTF8_CODE_POINT UINT32_MAX

uint8_t
utf8_sequence_length(અચલ અક્ષર *s);

uint32_t
utf8_next_code_point(અચલ અક્ષર *s, size_t max_size, size_t *size_out);
