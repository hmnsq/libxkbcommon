/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

সংরচনা parser_param;
সংরচনা scanner;

#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "parser.h"

পূর্ণাঙ্ক
_xkbcommon_lex(YYSTYPE *yylval, সংরচনা scanner *scanner);

XkbFile *
parse(সংরচনা xkb_context *ctx, ধ্রুব সংরচনা parser_keymap_config *config,
      সংরচনা scanner *scanner, ধ্রুব বর্ণ *map);

bool
parse_next(সংরচনা xkb_context *ctx,
           ধ্রুব সংরচনা parser_keymap_config *config,
           সংরচনা scanner *scanner, XkbFile **xkb_file);

পূর্ণাঙ্ক
keyword_to_token(ধ্রুব বর্ণ *string, size_t len);
