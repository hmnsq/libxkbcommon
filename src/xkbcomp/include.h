/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#define INCLUDE_MAX_DEPTH 15

#define MERGE_OVERRIDE_PREFIX '+'
#define MERGE_AUGMENT_PREFIX  '|'
#define MERGE_REPLACE_PREFIX  '^'
#define MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#define is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
स्थिर स्थिरांक अक्षर MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(अक्षर **str_inout, अक्षर **file_rtrn, अक्षर **map_rtrn,
                अक्षर *nextop_rtrn, अक्षर **extra_data);

ssize_t
expand_path(संरचना xkb_context *ctx, स्थिरांक अक्षर* parent_file_name,
            स्थिरांक अक्षर *name, माप_प्रकार name_len, गणना xkb_file_type type,
            अक्षर *buf, माप_प्रकार buf_size);

खाता *
FindFileInXkbPath(संरचना xkb_context *ctx, स्थिरांक अक्षर* parent_file_name,
                  स्थिरांक अक्षर *name, माप_प्रकार name_len, गणना xkb_file_type type,
                  अक्षर *buf, माप_प्रकार buf_size, अचिन्हित पूर्णांक *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(संरचना xkb_context *ctx, अचिन्हित पूर्णांक include_depth);

XkbFile *
ProcessIncludeFile(संरचना xkb_context *ctx,
                   स्थिरांक संरचना parser_keymap_config *config,
                   स्थिरांक IncludeStmt *stmt,
                   गणना xkb_file_type file_type, अक्षर *path, माप_प्रकार path_size);
