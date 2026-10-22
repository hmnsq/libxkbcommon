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
स्थिर अचर वर्ण MERGE_MODE_PREFIXES[] =
    { MERGE_OVERRIDE_PREFIX, MERGE_AUGMENT_PREFIX, MERGE_REPLACE_PREFIX, 0 };


bool
ParseIncludeMap(वर्ण **str_inout, वर्ण **file_rtrn, वर्ण **map_rtrn,
                वर्ण *nextop_rtrn, वर्ण **extra_data);

ssize_t
expand_path(संरचना xkb_context *ctx, अचर वर्ण* parent_file_name,
            अचर वर्ण *name, माप_प्रकार name_len, गणना xkb_file_type type,
            वर्ण *buf, माप_प्रकार buf_size);

खाता *
FindFileInXkbPath(संरचना xkb_context *ctx, अचर वर्ण* parent_file_name,
                  अचर वर्ण *name, माप_प्रकार name_len, गणना xkb_file_type type,
                  वर्ण *buf, माप_प्रकार buf_size, अचिन्हित पूर्णाङ्क *offset,
                  bool required);

bool
ExceedsIncludeMaxDepth(संरचना xkb_context *ctx, अचिन्हित पूर्णाङ्क include_depth);

XkbFile *
ProcessIncludeFile(संरचना xkb_context *ctx,
                   अचर संरचना parser_keymap_config *config,
                   अचर IncludeStmt *stmt,
                   गणना xkb_file_type file_type, वर्ण *path, माप_प्रकार path_size);
