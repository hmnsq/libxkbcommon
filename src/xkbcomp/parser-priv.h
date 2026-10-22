/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

ರಚನೆ parser_param;
ರಚನೆ scanner;

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "parser.h"

ಪೂರ್ಣಾಂಕ
_xkbcommon_lex(YYSTYPE *yylval, ರಚನೆ scanner *scanner);

XkbFile *
parse(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
      ರಚನೆ scanner *scanner, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map);

bool
parse_ಮುಂದಿನ(ರಚನೆ xkb_context *ctx,
           ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
           ರಚನೆ scanner *scanner, XkbFile **xkb_file);

ಪೂರ್ಣಾಂಕ
keyword_ಗಮ್ಯ_token(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len);
