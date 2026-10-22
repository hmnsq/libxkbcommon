/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#ವ್ಯಾಖ್ಯಾನಿಸು is_utf8_ಆರಂಭ(c) (((c) & 0xc0) != 0x80)
#ವ್ಯಾಖ್ಯಾನಿಸು INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು UINT32_MAX

uint8_t
utf8_sequence_ಉದ್ದಳತೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s);

uint32_t
utf8_ಮುಂದಿನ_ಸಂಕೇತ_ಬಿಂದು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t max_ಗಾತ್ರ, ಗಾತ್ರ_t *ಗಾತ್ರ_ಹೊರಹೋಗು);
