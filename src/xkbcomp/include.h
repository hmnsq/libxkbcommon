/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
স্থির ধ্রুৱ ৱর্ণ MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(ৱর্ণ **str_inout, ৱর্ণ **file_rtrn, ৱর্ণ **map_rtrn,
                ৱর্ণ *nextop_rtrn, ৱর্ণ **extra_data);

ssize_t
expand_path(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ* parent_file_name,
            ধ্রুৱ ৱর্ণ *name, size_t name_len, গণনা xkb_file_type type,
            ৱর্ণ *buf, size_t buf_size);

FILE *
FindFileInXkbPath(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ* parent_file_name,
                  ধ্রুৱ ৱর্ণ *name, size_t name_len, গণনা xkb_file_type type,
                  ৱর্ণ *buf, size_t buf_size, unsigned পূর্ণাঙ্ক *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(সংরচনা xkb_context *ctx, unsigned পূর্ণাঙ্ক include_depth);

XkbFile *
ProcessIncludeFile(সংরচনা xkb_context *ctx,
                   ধ্রুৱ সংরচনা parser_keymap_config *config,
                   ধ্রুৱ IncludeStmt *stmt,
                   গণনা xkb_file_type file_type, ৱর্ণ *path, size_t path_size);
