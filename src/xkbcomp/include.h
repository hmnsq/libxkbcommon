/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
স্থির ধ্রুবক অক্ষর MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(অক্ষর **str_inout, অক্ষর **file_rtrn, অক্ষর **map_rtrn,
                অক্ষর *nextop_rtrn, অক্ষর **extra_তথ্য);

ssize_t
expand_path(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর* parent_file_name,
            ধ্রুবক অক্ষর *name, আকার_t name_len, গণনা xkb_file_type type,
            অক্ষর *buf, আকার_t buf_আকার);

FILE *
FindFileInXkbPath(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর* parent_file_name,
                  ধ্রুবক অক্ষর *name, আকার_t name_len, গণনা xkb_file_type type,
                  অক্ষর *buf, আকার_t buf_আকার, unsigned পূর্ণসংখ্যা *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(কাঠামো xkb_context *ctx, unsigned পূর্ণসংখ্যা include_depth);

XkbFile *
ProcessIncludeFile(কাঠামো xkb_context *ctx,
                   ধ্রুবক কাঠামো parser_keymap_config *config,
                   ধ্রুবক IncludeStmt *stmt,
                   গণনা xkb_file_type file_type, অক্ষর *path, আকার_t path_আকার);
