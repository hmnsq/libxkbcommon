/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

ସଂରଚନା parser_param;
ସଂରଚନା scanner;

#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_xkbcommon_lex(YYSTYPE *yylval, ସଂରଚନା scanner *scanner);

XkbFile *
parse(ସଂରଚନା xkb_context *ctx, ଅଚଳ ସଂରଚନା parser_keymap_config *config,
      ସଂରଚନା scanner *scanner, ଅଚଳ ଅକ୍ଷର *map);

bool
parse_next(ସଂରଚନା xkb_context *ctx,
           ଅଚଳ ସଂରଚନା parser_keymap_config *config,
           ସଂରଚନା scanner *scanner, XkbFile **xkb_file);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
keyword_to_token(ଅଚଳ ଅକ୍ଷର *string, size_t len);
