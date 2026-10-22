/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "ast.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#నిర్వచించు INCLUDE_MAX_లోతు 15

#నిర్వచించు విలీనం_OVERRIDE_ఉపసర్గ '+'
#నిర్వచించు విలీనం_AUGMENT_ఉపసర్గ  '|'
#నిర్వచించు విలీనం_REPLACE_ఉపసర్గ  '^'
#నిర్వచించు విలీనం_DEFAULT_ఉపసర్గ  విలీనం_OVERRIDE_ఉపసర్గ
#నిర్వచించు is_విలీనం_బహుళకం_ఉపసర్గ(ch) \
    ((ch) == విలీనం_OVERRIDE_ఉపసర్గ || (ch) == విలీనం_AUGMENT_ఉపసర్గ || \
     (ch) == విలీనం_REPLACE_ఉపసర్గ)
స్థిర స్థిరరాశి అక్షరం విలీనం_బహుళకం_PREFIXES[] =
    { విలీనం_OVERRIDE_ఉపసర్గ, విలీనం_AUGMENT_ఉపసర్గ, విలీనం_REPLACE_ఉపసర్గ, 0 };


bool
ParseIncludeMap(అక్షరం **str_inout, అక్షరం **file_rtrn, అక్షరం **map_rtrn,
                అక్షరం *nextop_rtrn, అక్షరం **extra_దత్తాంశం);

ssize_t
expand_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* మాతృ_file_name,
            స్థిరరాశి అక్షరం *name, కొలత_t name_len, గణన xkb_file_type type,
            అక్షరం *buf, కొలత_t buf_కొలత);

FILE *
కనుగొను_File_లోపల_Xkb_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* మాతృ_file_name,
                  స్థిరరాశి అక్షరం *name, కొలత_t name_len, గణన xkb_file_type type,
                  అక్షరం *buf, కొలత_t buf_కొలత, గుర్తులేని పూర్ణాంకం *స్థానభ్రంశం,
                  bool required);

bool
Exceeds_Include_Max_లోతు(నిర్మాణం xkb_context *ctx, గుర్తులేని పూర్ణాంకం include_లోతు);

XkbFile *
ProcessIncludeFile(నిర్మాణం xkb_context *ctx,
                   స్థిరరాశి నిర్మాణం parser_keymap_config *config,
                   స్థిరరాశి IncludeStmt *stmt,
                   గణన xkb_file_type file_type, అక్షరం *మార్గం, కొలత_t మార్గం_కొలత);
