/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

నిర్మాణం parser_param;
నిర్మాణం scanner;

#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "parser.h"

పూర్ణాంకం
_xkbcommon_lex(YYSTYPE *yylval, నిర్మాణం scanner *scanner);

XkbFile *
parse(నిర్మాణం xkb_context *ctx, స్థిరరాశి నిర్మాణం parser_keymap_config *config,
      నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *map);

bool
parse_తదుపరి(నిర్మాణం xkb_context *ctx,
           స్థిరరాశి నిర్మాణం parser_keymap_config *config,
           నిర్మాణం scanner *scanner, XkbFile **xkb_file);

పూర్ణాంకం
keyword_గమ్యం_token(స్థిరరాశి అక్షరం *string, కొలత_t len);
