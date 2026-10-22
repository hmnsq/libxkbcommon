/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include <stdbool.h>

sa_nracanaa parser_param;
sa_nracanaa scanner;

#include "scanner-utils.h"
#include "parser.h"

ကိန်းပြည့်
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *scanner);

XkbFile *
parse(sa_nracanaa xkb_context *ctx, const sa_nracanaa parser_keymap_config *config,
      sa_nracanaa scanner *scanner, const var_na *map);

bool
parse_next(sa_nracanaa xkb_context *ctx,
           const sa_nracanaa parser_keymap_config *config,
           sa_nracanaa scanner *scanner, XkbFile **xkb_file);

ကိန်းပြည့်
keyword_to_token(const var_na *string, size_t len);
