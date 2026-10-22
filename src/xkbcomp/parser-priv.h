/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

संरचना parser_param;
संरचना scanner;

#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "parser.h"

पूर्णाङ्क
_xkbcommon_lex(YYSTYPE *yylval, संरचना scanner *scanner);

XkbFile *
parse(संरचना xkb_context *ctx, ध्रुव संरचना parser_keymap_config *config,
      संरचना scanner *scanner, ध्रुव वर्ण *map);

bool
parse_next(संरचना xkb_context *ctx,
           ध्रुव संरचना parser_keymap_config *config,
           संरचना scanner *scanner, XkbFile **xkb_file);

पूर्णाङ्क
keyword_to_token(ध्रुव वर्ण *string, size_t len);
