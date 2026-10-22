/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश "ast.h"
#समावेश "utils.h"
#समावेश "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#घोषणा INCLUDE_MAX_DEPTH 15

#घोषणा MERGE_OVERRIDE_PREFIX '+'
#घोषणा MERGE_AUGMENT_PREFIX  '|'
#घोषणा MERGE_REPLACE_PREFIX  '^'
#घोषणा MERGE_DEFAULT_PREFIX  MERGE_OVERRIDE_PREFIX
#घोषणा is_merge_mode_prefix(ch) \
    ((ch) == MERGE_OVERRIDE_PREFIX || (ch) == MERGE_AUGMENT_PREFIX || \
     (ch) == MERGE_REPLACE_PREFIX)
अटल स्थिर अक्षर MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(अक्षर **str_inout, अक्षर **file_rtrn, अक्षर **map_rtrn,
                अक्षर *nextop_rtrn, अक्षर **extra_तथ्य);

ssize_t
expand_path(काष्ठा xkb_context *ctx, स्थिर अक्षर* parent_file_name,
            स्थिर अक्षर *name, माप_प्रकार name_len, क्रमागत xkb_file_type type,
            अक्षर *buf, माप_प्रकार buf_आकार);

खाता *
FindFileInXkbPath(काष्ठा xkb_context *ctx, स्थिर अक्षर* parent_file_name,
                  स्थिर अक्षर *name, माप_प्रकार name_len, क्रमागत xkb_file_type type,
                  अक्षर *buf, माप_प्रकार buf_आकार, अचिन्हित पूर्णांक *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(काष्ठा xkb_context *ctx, अचिन्हित पूर्णांक include_depth);

XkbFile *
ProcessIncludeFile(काष्ठा xkb_context *ctx,
                   स्थिर काष्ठा parser_keymap_config *config,
                   स्थिर IncludeStmt *stmt,
                   क्रमागत xkb_file_type file_type, अक्षर *path, माप_प्रकार path_आकार);
