/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

ಸಂರಚನಾ parser_param;
ಸಂರಚನಾ scanner;

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "parser.h"

ಪೂರ್ಣಾಙ್ಕ
_xkbcommon_lex(YYSTYPE *yylval, ಸಂರಚನಾ scanner *scanner);

XkbFile *
parse(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
      ಸಂರಚನಾ scanner *scanner, ಧ್ರುವ ವರ್ಣ *map);

bool
parse_next(ಸಂರಚನಾ xkb_context *ctx,
           ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
           ಸಂರಚನಾ scanner *scanner, XkbFile **xkb_file);

ಪೂರ್ಣಾಙ್ಕ
keyword_to_token(ಧ್ರುವ ವರ್ಣ *string, size_t len);
