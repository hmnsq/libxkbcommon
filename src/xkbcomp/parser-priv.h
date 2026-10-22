/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

ਬਣਤਰ parser_param;
ਬਣਤਰ scanner;

#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"

ਪੂਰਨ
_xkbcommon_lex(YYSTYPE *yylval, ਬਣਤਰ scanner *scanner);

XkbFile *
parse(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
      ਬਣਤਰ scanner *scanner, ਅਚਲ ਅੱਖਰ *map);

bool
parse_ਅਗਲਾ(ਬਣਤਰ xkb_context *ctx,
           ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
           ਬਣਤਰ scanner *scanner, XkbFile **xkb_file);

ਪੂਰਨ
keyword_ਮੰਜ਼ਿਲ_token(ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len);
