/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stddef.h>
#অন্তর্ভাৱ <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#define is_utf8_start(c) (((c) & 0xc0) != 0x80)
#define INVALID_UTF8_CODE_POINT UINT32_MAX

uint8_t
utf8_sequence_length(ধ্রুৱ ৱর্ণ *s);

uint32_t
utf8_next_code_point(ধ্রুৱ ৱর্ণ *s, size_t max_size, size_t *size_out);
