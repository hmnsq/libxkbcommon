/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#تعریف_کړه INCLUDE_MAX_ژوروالی 15

#تعریف_کړه یوځای_OVERRIDE_مختاړی '+'
#تعریف_کړه یوځای_AUGMENT_مختاړی  '|'
#تعریف_کړه یوځای_REPLACE_مختاړی  '^'
#تعریف_کړه یوځای_DEFAULT_مختاړی  یوځای_OVERRIDE_مختاړی
#تعریف_کړه is_یوځای_ډېرپېښ_مختاړی(ch) \
    ((ch) == یوځای_OVERRIDE_مختاړی || (ch) == یوځای_AUGMENT_مختاړی || \
     (ch) == یوځای_REPLACE_مختاړی)
ساكن ثابت حرف یوځای_ډېرپېښ_PREFIXES[] =
    { یوځای_OVERRIDE_مختاړی, یوځای_AUGMENT_مختاړی, یوځای_REPLACE_مختاړی, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_معلومات);

ssize_t
expand_لار(بنية xkb_context *ctx, ثابت حرف* پلار_file_name,
            ثابت حرف *name, اندازه_t name_len, تعداد xkb_file_type type,
            حرف *buf, اندازه_t buf_اندازه);

FILE *
پیدا_کړه_File_دننه_Xkb_لار(بنية xkb_context *ctx, ثابت حرف* پلار_file_name,
                  ثابت حرف *name, اندازه_t name_len, تعداد xkb_file_type type,
                  حرف *buf, اندازه_t buf_اندازه, بینښه صحيح *دننه_واټن,
                  bool required);

bool
Exceeds_Include_Max_ژوروالی(بنية xkb_context *ctx, بینښه صحيح include_ژوروالی);

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *لار, اندازه_t لار_اندازه);
