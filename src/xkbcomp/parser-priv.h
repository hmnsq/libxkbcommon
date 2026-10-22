/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>

கட்டமைப்பு parser_param;
கட்டமைப்பு scanner;

#அந்தர்பாவ "scanner-utils.h"
#அந்தர்பாவ "parser.h"

முழு
_xkbcommon_lex(YYSTYPE *yylval, கட்டமைப்பு scanner *scanner);

XkbFile *
parse(கட்டமைப்பு xkb_context *ctx, மாறிலி கட்டமைப்பு parser_keymap_config *config,
      கட்டமைப்பு scanner *scanner, மாறிலி எழுத்து *map);

bool
parse_next(கட்டமைப்பு xkb_context *ctx,
           மாறிலி கட்டமைப்பு parser_keymap_config *config,
           கட்டமைப்பு scanner *scanner, XkbFile **xkb_file);

முழு
keyword_to_token(மாறிலி எழுத்து *string, size_t len);
