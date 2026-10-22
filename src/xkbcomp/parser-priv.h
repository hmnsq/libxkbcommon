/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>

ساختار parser_param;
ساختار scanner;

#شامل "scanner-utils.h"
#شامل "parser.h"

صحیح
_xkbcommon_lex(YYSTYPE *yylval, ساختار scanner *scanner);

XkbFile *
parse(ساختار xkb_context *ctx, ثابت ساختار parser_keymap_config *config,
      ساختار scanner *scanner, ثابت نویسه *map);

bool
parse_next(ساختار xkb_context *ctx,
           ثابت ساختار parser_keymap_config *config,
           ساختار scanner *scanner, XkbFile **xkb_file);

صحیح
keyword_to_token(ثابت نویسه *string, size_t len);
