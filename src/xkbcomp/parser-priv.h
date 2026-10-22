/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

काष्ठा parser_param;
काष्ठा scanner;

#समावेश "scanner-utils.h"
#समावेश "parser.h"

पूर्णांक
_xkbcommon_lex(YYSTYPE *yylval, काष्ठा scanner *scanner);

XkbFile *
parse(काष्ठा xkb_context *ctx, स्थिर काष्ठा parser_keymap_config *config,
      काष्ठा scanner *scanner, स्थिर अक्षर *map);

bool
parse_अग्र(काष्ठा xkb_context *ctx,
           स्थिर काष्ठा parser_keymap_config *config,
           काष्ठा scanner *scanner, XkbFile **xkb_file);

पूर्णांक
keyword_to_token(स्थिर अक्षर *string, माप_प्रकार len);
