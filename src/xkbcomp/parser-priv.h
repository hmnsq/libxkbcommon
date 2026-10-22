/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

સંરચના parser_param;
સંરચના scanner;

#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "parser.h"

પૂર્ણાંક
_xkbcommon_lex(YYSTYPE *yylval, સંરચના scanner *scanner);

XkbFile *
parse(સંરચના xkb_context *ctx, અચલ સંરચના parser_keymap_config *config,
      સંરચના scanner *scanner, અચલ અક્ષર *map);

bool
parse_next(સંરચના xkb_context *ctx,
           અચલ સંરચના parser_keymap_config *config,
           સંરચના scanner *scanner, XkbFile **xkb_file);

પૂર્ણાંક
keyword_to_token(અચલ અક્ષર *string, size_t len);
