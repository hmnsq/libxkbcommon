/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
நிலையான மாறிலி எழுத்து MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(எழுத்து **str_inout, எழுத்து **file_rtrn, எழுத்து **map_rtrn,
                எழுத்து *nextop_rtrn, எழுத்து **extra_data);

ssize_t
expand_path(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* parent_file_name,
            மாறிலி எழுத்து *name, size_t name_len, எண்ணிக்கை xkb_file_type type,
            எழுத்து *buf, size_t buf_size);

FILE *
FindFileInXkbPath(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* parent_file_name,
                  மாறிலி எழுத்து *name, size_t name_len, எண்ணிக்கை xkb_file_type type,
                  எழுத்து *buf, size_t buf_size, unsigned முழு *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(கட்டமைப்பு xkb_context *ctx, unsigned முழு include_depth);

XkbFile *
ProcessIncludeFile(கட்டமைப்பு xkb_context *ctx,
                   மாறிலி கட்டமைப்பு parser_keymap_config *config,
                   மாறிலி IncludeStmt *stmt,
                   எண்ணிக்கை xkb_file_type file_type, எழுத்து *path, size_t path_size);
