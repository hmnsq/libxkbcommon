/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#परिभाषय INCLUDE_MAX_गभीरता 15

#परिभाषय MERGE_OVERRIDE_उपसर्गः '+'
#परिभाषय MERGE_AUGMENT_उपसर्गः  '|'
#परिभाषय MERGE_REPLACE_उपसर्गः  '^'
#परिभाषय MERGE_DEFAULT_उपसर्गः  MERGE_OVERRIDE_उपसर्गः
#परिभाषय is_merge_बहुलकम्_उपसर्गः(ch) \
    ((ch) == MERGE_OVERRIDE_उपसर्गः || (ch) == MERGE_AUGMENT_उपसर्गः || \
     (ch) == MERGE_REPLACE_उपसर्गः)
स्थिर ध्रुव वर्ण MERGE_बहुलकम्_PREFIXES[] =
    { MERGE_OVERRIDE_उपसर्गः, MERGE_AUGMENT_उपसर्गः, MERGE_REPLACE_उपसर्गः, 0 };


bool
ParseIncludeMap(वर्ण **str_inout, वर्ण **file_rtrn, वर्ण **map_rtrn,
                वर्ण *nextop_rtrn, वर्ण **extra_दत्त);

ssize_t
expand_मार्गः(संरचना xkb_context *ctx, ध्रुव वर्ण* parent_file_name,
            ध्रुव वर्ण *name, आकार_t name_len, गणना xkb_file_type type,
            वर्ण *buf, आकार_t buf_आकार);

FILE *
Find_File_अन्तर्_Xkb_मार्गः(संरचना xkb_context *ctx, ध्रुव वर्ण* parent_file_name,
                  ध्रुव वर्ण *name, आकार_t name_len, गणना xkb_file_type type,
                  वर्ण *buf, आकार_t buf_आकार, अचिह्नित पूर्णाङ्क *विस्थापनम्,
                  bool required);

bool
Exceeds_Include_Max_गभीरता(संरचना xkb_context *ctx, अचिह्नित पूर्णाङ्क include_गभीरता);

XkbFile *
ProcessIncludeFile(संरचना xkb_context *ctx,
                   ध्रुव संरचना parser_keymap_config *config,
                   ध्रुव IncludeStmt *stmt,
                   गणना xkb_file_type file_type, वर्ण *मार्गः, आकार_t मार्गः_आकार);
