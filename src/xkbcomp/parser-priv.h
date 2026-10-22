/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

ഘടന parser_param;
ഘടന scanner;

#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "parser.h"

പൂർണ്ണസംഖ്യ
_xkbcommon_lex(YYSTYPE *yylval, ഘടന scanner *scanner);

XkbFile *
parse(ഘടന xkb_context *ctx, സ്ഥിരം ഘടന parser_keymap_config *config,
      ഘടന scanner *scanner, സ്ഥിരം അക്ഷരം *map);

bool
parse_next(ഘടന xkb_context *ctx,
           സ്ഥിരം ഘടന parser_keymap_config *config,
           ഘടന scanner *scanner, XkbFile **xkb_file);

പൂർണ്ണസംഖ്യ
keyword_to_token(സ്ഥിരം അക്ഷരം *string, size_t len);
