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
স্থির ধ্রুব বর্ণ MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(বর্ণ **str_inout, বর্ণ **file_rtrn, বর্ণ **map_rtrn,
                বর্ণ *nextop_rtrn, বর্ণ **extra_data);

ssize_t
expand_path(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ* parent_file_name,
            ধ্রুব বর্ণ *name, size_t name_len, গণনা xkb_file_type type,
            বর্ণ *buf, size_t buf_size);

FILE *
FindFileInXkbPath(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ* parent_file_name,
                  ধ্রুব বর্ণ *name, size_t name_len, গণনা xkb_file_type type,
                  বর্ণ *buf, size_t buf_size, unsigned পূর্ণাঙ্ক *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(সংরচনা xkb_context *ctx, unsigned পূর্ণাঙ্ক include_depth);

XkbFile *
ProcessIncludeFile(সংরচনা xkb_context *ctx,
                   ধ্রুব সংরচনা parser_keymap_config *config,
                   ধ্রুব IncludeStmt *stmt,
                   গণনা xkb_file_type file_type, বর্ণ *path, size_t path_size);
