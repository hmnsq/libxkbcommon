/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "utils.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿಸು ಮಾರ್ಗ_SEPARATOR '\\'
#ವ್ಯಾಖ್ಯಾನಿಸು ALT_ಮಾರ್ಗ_SEPARATOR '/'
#ವ್ಯಾಖ್ಯಾನಿಸು is_ಮಾರ್ಗ_separator(s) \
    ((s) == ಮಾರ್ಗ_SEPARATOR || (s) == ALT_ಮಾರ್ಗ_SEPARATOR)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಮಾರ್ಗ_SEPARATOR '/'
#ವ್ಯಾಖ್ಯಾನಿಸು is_ಮಾರ್ಗ_separator(s) ((s) == ಮಾರ್ಗ_SEPARATOR)
#ಆದರೆ_ಅಂತ್ಯ

XKB_EXPORT_ಖಾಸಗಿ bool
is_absolute_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);
