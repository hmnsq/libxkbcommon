/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
ساكن ثابت حرف MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_data);

ssize_t
expand_path(بنية xkb_context *ctx, ثابت حرف* parent_file_name,
            ثابت حرف *name, size_t name_len, تعداد xkb_file_type type,
            حرف *buf, size_t buf_size);

FILE *
FindFileInXkbPath(بنية xkb_context *ctx, ثابت حرف* parent_file_name,
                  ثابت حرف *name, size_t name_len, تعداد xkb_file_type type,
                  حرف *buf, size_t buf_size, unsigned صحيح *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(بنية xkb_context *ctx, unsigned صحيح include_depth);

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *path, size_t path_size);
