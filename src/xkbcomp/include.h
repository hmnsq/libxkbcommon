/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
સ્થિર અચલ અક્ષર MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(અક્ષર **str_inout, અક્ષર **file_rtrn, અક્ષર **map_rtrn,
                અક્ષર *nextop_rtrn, અક્ષર **extra_data);

ssize_t
expand_path(સંરચના xkb_context *ctx, અચલ અક્ષર* parent_file_name,
            અચલ અક્ષર *name, size_t name_len, ગણના xkb_file_type type,
            અક્ષર *buf, size_t buf_size);

FILE *
FindFileInXkbPath(સંરચના xkb_context *ctx, અચલ અક્ષર* parent_file_name,
                  અચલ અક્ષર *name, size_t name_len, ગણના xkb_file_type type,
                  અક્ષર *buf, size_t buf_size, unsigned પૂર્ણાંક *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(સંરચના xkb_context *ctx, unsigned પૂર્ણાંક include_depth);

XkbFile *
ProcessIncludeFile(સંરચના xkb_context *ctx,
                   અચલ સંરચના parser_keymap_config *config,
                   અચલ IncludeStmt *stmt,
                   ગણના xkb_file_type file_type, અક્ષર *path, size_t path_size);
