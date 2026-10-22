/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_utf8_ਸ਼ੁਰੂ(c) (((c) & 0xc0) != 0x80)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ UINT32_MAX

uint8_t
utf8_sequence_ਲੰਮਾਈ(ਅਚਲ ਅੱਖਰ *s);

uint32_t
utf8_ਅਗਲਾ_ਕੋਡ_ਬਿੰਦੂ(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t max_ਮਾਪ, ਮਾਪ_t *ਮਾਪ_ਬਾਹਰਲਾ);
