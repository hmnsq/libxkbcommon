/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن <stdbool.h>

بنية parser_param;
بنية scanner;

#ضمن "scanner-utils.h"
#ضمن "parser.h"

صحيح
_xkbcommon_lex(YYSTYPE *yylval, بنية scanner *scanner);

XkbFile *
parse(بنية xkb_context *ctx, ثابت بنية parser_keymap_config *config,
      بنية scanner *scanner, ثابت حرف *map);

bool
parse_next(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file);

صحيح
keyword_to_token(ثابت حرف *string, size_t len);
