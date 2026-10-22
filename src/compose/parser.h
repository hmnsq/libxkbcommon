/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-compose.h"

#ಅನ್ತರ್ಭಾವ "src/utils.h"

XKB_EXPORT_ಖಾಸಗಿ ಅಕ್ಷರ *
parse_string_literal(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string);

bool
parse_string(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name);

bool
parse_file(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
           FILE *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name);
