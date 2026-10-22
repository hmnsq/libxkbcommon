/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#define is_utf8_start(c) (((c) & 0xc0) != 0x80)
#define INVALID_UTF8_CODE_POINT UINT32_MAX

uint8_t
utf8_sequence_দৈর্ঘ্য(ধ্রুবক অক্ষর *s);

uint32_t
utf8_পরবর্তী_code_point(ধ্রুবক অক্ষর *s, আকার_t max_আকার, আকার_t *আকার_out);
