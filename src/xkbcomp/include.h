/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#رهنمود once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#تعریف_کن INCLUDE_MAX_عمق 15

#تعریف_کن ادغام_OVERRIDE_پیشوند '+'
#تعریف_کن ادغام_AUGMENT_پیشوند  '|'
#تعریف_کن ادغام_REPLACE_پیشوند  '^'
#تعریف_کن ادغام_DEFAULT_پیشوند  ادغام_OVERRIDE_پیشوند
#تعریف_کن is_ادغام_نما_پیشوند(ch) \
    ((ch) == ادغام_OVERRIDE_پیشوند || (ch) == ادغام_AUGMENT_پیشوند || \
     (ch) == ادغام_REPLACE_پیشوند)
ساكن ثابت حرف ادغام_نما_PREFIXES[] =
    { ادغام_OVERRIDE_پیشوند, ادغام_AUGMENT_پیشوند, ادغام_REPLACE_پیشوند, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_داده);

ssize_t
expand_مسیر(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
            ثابت حرف *name, اندازه_t name_len, تعداد xkb_file_type type,
            حرف *buf, اندازه_t buf_اندازه);

FILE *
پیدا_کن_File_درون_Xkb_مسیر(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
                  ثابت حرف *name, اندازه_t name_len, تعداد xkb_file_type type,
                  حرف *buf, اندازه_t buf_اندازه, بیعلامت صحيح *جابه_جایی,
                  bool required);

bool
Exceeds_Include_Max_عمق(بنية xkb_context *ctx, بیعلامت صحيح include_عمق);

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *مسیر, اندازه_t مسیر_اندازه);
