/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(ವರ್ಣ **str_inout, ವರ್ಣ **file_rtrn, ವರ್ಣ **map_rtrn,
                ವರ್ಣ *nextop_rtrn, ವರ್ಣ **extra_data);

ssize_t
expand_path(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* parent_file_name,
            ಧ್ರುವ ವರ್ಣ *name, size_t name_len, ಗಣನಾ xkb_file_type type,
            ವರ್ಣ *buf, size_t buf_size);

FILE *
FindFileInXkbPath(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* parent_file_name,
                  ಧ್ರುವ ವರ್ಣ *name, size_t name_len, ಗಣನಾ xkb_file_type type,
                  ವರ್ಣ *buf, size_t buf_size, unsigned ಪೂರ್ಣಾಙ್ಕ *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(ಸಂರಚನಾ xkb_context *ctx, unsigned ಪೂರ್ಣಾಙ್ಕ include_depth);

XkbFile *
ProcessIncludeFile(ಸಂರಚನಾ xkb_context *ctx,
                   ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
                   ಧ್ರುವ IncludeStmt *stmt,
                   ಗಣನಾ xkb_file_type file_type, ವರ್ಣ *path, size_t path_size);
