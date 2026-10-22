/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#هدايت once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#وصف_ڪريو INCLUDE_MAX_اونهائي 15

#وصف_ڪريو ملايو_OVERRIDE_اڳياڙي '+'
#وصف_ڪريو ملايو_AUGMENT_اڳياڙي  '|'
#وصف_ڪريو ملايو_REPLACE_اڳياڙي  '^'
#وصف_ڪريو ملايو_DEFAULT_اڳياڙي  ملايو_OVERRIDE_اڳياڙي
#وصف_ڪريو is_ملايو_گهڻائي_اڳياڙي(ch) \
    ((ch) == ملايو_OVERRIDE_اڳياڙي || (ch) == ملايو_AUGMENT_اڳياڙي || \
     (ch) == ملايو_REPLACE_اڳياڙي)
ساكن ثابت حرف ملايو_گهڻائي_PREFIXES[] =
    { ملايو_OVERRIDE_اڳياڙي, ملايو_AUGMENT_اڳياڙي, ملايو_REPLACE_اڳياڙي, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_معلومات);

ssize_t
expand_رستو(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
            ثابت حرف *name, سائيز_t name_len, تعداد xkb_file_type type,
            حرف *buf, سائيز_t buf_سائيز);

FILE *
ڳوليو_File_اندريون_Xkb_رستو(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
                  ثابت حرف *name, سائيز_t name_len, تعداد xkb_file_type type,
                  حرف *buf, سائيز_t buf_سائيز, بنانشان صحيح *وٿي,
                  bool required);

bool
Exceeds_Include_Max_اونهائي(بنية xkb_context *ctx, بنانشان صحيح include_اونهائي);

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *رستو, سائيز_t رستو_سائيز);
