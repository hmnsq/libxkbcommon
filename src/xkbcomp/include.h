/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
സ്ഥിതം സ്ഥിരം അക്ഷരം MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(അക്ഷരം **str_inout, അക്ഷരം **file_rtrn, അക്ഷരം **map_rtrn,
                അക്ഷരം *nextop_rtrn, അക്ഷരം **extra_data);

ssize_t
expand_path(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* parent_file_name,
            സ്ഥിരം അക്ഷരം *name, size_t name_len, എണ്ണൽ xkb_file_type type,
            അക്ഷരം *buf, size_t buf_size);

FILE *
FindFileInXkbPath(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* parent_file_name,
                  സ്ഥിരം അക്ഷരം *name, size_t name_len, എണ്ണൽ xkb_file_type type,
                  അക്ഷരം *buf, size_t buf_size, unsigned പൂർണ്ണസംഖ്യ *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(ഘടന xkb_context *ctx, unsigned പൂർണ്ണസംഖ്യ include_depth);

XkbFile *
ProcessIncludeFile(ഘടന xkb_context *ctx,
                   സ്ഥിരം ഘടന parser_keymap_config *config,
                   സ്ഥിരം IncludeStmt *stmt,
                   എണ്ണൽ xkb_file_type file_type, അക്ഷരം *path, size_t path_size);
