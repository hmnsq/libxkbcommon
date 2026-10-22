/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

কাঠামো parser_param;
কাঠামো scanner;

#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "parser.h"

পূর্ণসংখ্যা
_xkbcommon_lex(YYSTYPE *yylval, কাঠামো scanner *scanner);

XkbFile *
parse(কাঠামো xkb_context *ctx, ধ্রুবক কাঠামো parser_keymap_config *config,
      কাঠামো scanner *scanner, ধ্রুবক অক্ষর *map);

bool
parse_পরবর্তী(কাঠামো xkb_context *ctx,
           ধ্রুবক কাঠামো parser_keymap_config *config,
           কাঠামো scanner *scanner, XkbFile **xkb_file);

পূর্ণসংখ্যা
keyword_to_token(ধ্রুবক অক্ষর *string, আকার_t len);
