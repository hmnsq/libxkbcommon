/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(ଅକ୍ଷର **str_inout, ଅକ୍ଷର **file_rtrn, ଅକ୍ଷର **map_rtrn,
                ଅକ୍ଷର *nextop_rtrn, ଅକ୍ଷର **extra_data);

ssize_t
expand_path(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର* parent_file_name,
            ଅଚଳ ଅକ୍ଷର *name, size_t name_len, ଗଣନା xkb_file_type type,
            ଅକ୍ଷର *buf, size_t buf_size);

FILE *
FindFileInXkbPath(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର* parent_file_name,
                  ଅଚଳ ଅକ୍ଷର *name, size_t name_len, ଗଣନା xkb_file_type type,
                  ଅକ୍ଷର *buf, size_t buf_size, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(ସଂରଚନା xkb_context *ctx, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth);

XkbFile *
ProcessIncludeFile(ସଂରଚନା xkb_context *ctx,
                   ଅଚଳ ସଂରଚନା parser_keymap_config *config,
                   ଅଚଳ IncludeStmt *stmt,
                   ଗଣନା xkb_file_type file_type, ଅକ୍ଷର *path, size_t path_size);
