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

पूर्णांक
_xkbcommon_lex(YYSTYPE *yylval, संरचना scanner *scanner);

XkbFile *
parse(संरचना xkb_context *ctx, स्थिरांक संरचना parser_keymap_config *config,
      संरचना scanner *scanner, स्थिरांक अक्षर *map);

bool
parse_next(संरचना xkb_context *ctx,
           स्थिरांक संरचना parser_keymap_config *config,
           संरचना scanner *scanner, XkbFile **xkb_file);

पूर्णांक
keyword_to_token(स्थिरांक अक्षर *string, माप_प्रकार len);
