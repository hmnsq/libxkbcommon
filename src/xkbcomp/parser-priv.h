/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

ساخت parser_param;
ساخت scanner;

#شامل "scanner-utils.h"
#شامل "parser.h"

صحیح
_xkbcommon_lex(YYSTYPE *yylval, ساخت scanner *scanner);

XkbFile *
parse(ساخت xkb_context *ctx, ثابت ساخت parser_keymap_config *config,
      ساخت scanner *scanner, ثابت حرف *map);

bool
parse_اگلا(ساخت xkb_context *ctx,
           ثابت ساخت parser_keymap_config *config,
           ساخت scanner *scanner, XkbFile **xkb_file);

صحیح
keyword_مقصد_token(ثابت حرف *string, سائز_t len);
