/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "ast.h"
#شامل "utils.h"
#شامل "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
ایستا ثابت نویسه MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(نویسه **str_inout, نویسه **file_rtrn, نویسه **map_rtrn,
                نویسه *nextop_rtrn, نویسه **extra_data);

ssize_t
expand_path(ساختار xkb_context *ctx, ثابت نویسه* parent_file_name,
            ثابت نویسه *name, size_t name_len, شمارش xkb_file_type type,
            نویسه *buf, size_t buf_size);

FILE *
FindFileInXkbPath(ساختار xkb_context *ctx, ثابت نویسه* parent_file_name,
                  ثابت نویسه *name, size_t name_len, شمارش xkb_file_type type,
                  نویسه *buf, size_t buf_size, unsigned صحیح *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(ساختار xkb_context *ctx, unsigned صحیح include_depth);

XkbFile *
ProcessIncludeFile(ساختار xkb_context *ctx,
                   ثابت ساختار parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   شمارش xkb_file_type file_type, نویسه *path, size_t path_size);
