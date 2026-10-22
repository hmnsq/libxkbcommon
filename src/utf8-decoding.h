/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#परिभाषय is_utf8_start(c) (((c) & 0xc0) != 0x80)
#परिभाषय INVALID_UTF8_संहिता_POINT UINT32_MAX

uint8_t
utf8_sequence_दीर्घता(ध्रुव वर्ण *s);

uint32_t
utf8_अग्र_संहिता_point(ध्रुव वर्ण *s, आकार_t max_आकार, आकार_t *आकार_बहिर्गमनम्);
