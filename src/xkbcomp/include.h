/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_MAX_ಆಳ 15

#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ '+'
#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ  '|'
#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ  '^'
#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_DEFAULT_ಪೂರ್ವಪ್ರತ್ಯಯ  ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ
#ವ್ಯಾಖ್ಯಾನಿಸು is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(ch) \
    ((ch) == ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ || (ch) == ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ || \
     (ch) == ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ)
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ವಿಲೀನ_ಬಹುಲಕ_PREFIXES[] =
    { ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ, ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ, ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ, 0 };


bool
ParseIncludeMap(ಅಕ್ಷರ **str_inout, ಅಕ್ಷರ **file_rtrn, ಅಕ್ಷರ **map_rtrn,
                ಅಕ್ಷರ *nextop_rtrn, ಅಕ್ಷರ **extra_ದತ್ತಾಂಶ);

ssize_t
expand_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೋಷಕ_file_name,
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t name_len, ಎಣಿಕೆ xkb_file_type type,
            ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ);

FILE *
ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೋಷಕ_file_name,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t name_len, ಎಣಿಕೆ xkb_file_type type,
                  ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಸ್ಥಾನಾಂತರ,
                  bool required);

bool
Exceeds_Include_Max_ಆಳ(ರಚನೆ xkb_context *ctx, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ);

XkbFile *
ProcessIncludeFile(ರಚನೆ xkb_context *ctx,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
                   ಸ್ಥಿರಾಂಕ IncludeStmt *stmt,
                   ಎಣಿಕೆ xkb_file_type file_type, ಅಕ್ಷರ *ಮಾರ್ಗ, ಗಾತ್ರ_t ಮಾರ್ಗ_ಗಾತ್ರ);
