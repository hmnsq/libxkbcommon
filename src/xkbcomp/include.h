/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#شامل "config.h"

#شامل "ast.h"
#شامل "utils.h"
#شامل "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#تعریف_کرو INCLUDE_MAX_گہرائی 15

#تعریف_کرو ملاؤ_OVERRIDE_سابقہ '+'
#تعریف_کرو ملاؤ_AUGMENT_سابقہ  '|'
#تعریف_کرو ملاؤ_REPLACE_سابقہ  '^'
#تعریف_کرو ملاؤ_DEFAULT_سابقہ  ملاؤ_OVERRIDE_سابقہ
#تعریف_کرو is_ملاؤ_منوال_سابقہ(ch) \
    ((ch) == ملاؤ_OVERRIDE_سابقہ || (ch) == ملاؤ_AUGMENT_سابقہ || \
     (ch) == ملاؤ_REPLACE_سابقہ)
ساکن ثابت حرف ملاؤ_منوال_PREFIXES[] =
    { ملاؤ_OVERRIDE_سابقہ, ملاؤ_AUGMENT_سابقہ, ملاؤ_REPLACE_سابقہ, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_ڈیٹا);

ssize_t
expand_راستہ(ساخت xkb_context *ctx, ثابت حرف* والد_file_name,
            ثابت حرف *name, سائز_t name_len, شمار xkb_file_type type,
            حرف *buf, سائز_t buf_سائز);

FILE *
ڈھونڈو_File_اندر_Xkb_راستہ(ساخت xkb_context *ctx, ثابت حرف* والد_file_name,
                  ثابت حرف *name, سائز_t name_len, شمار xkb_file_type type,
                  حرف *buf, سائز_t buf_سائز, غیرعلامتی صحیح *انحراف,
                  bool required);

bool
Exceeds_Include_Max_گہرائی(ساخت xkb_context *ctx, غیرعلامتی صحیح include_گہرائی);

XkbFile *
ProcessIncludeFile(ساخت xkb_context *ctx,
                   ثابت ساخت parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   شمار xkb_file_type file_type, حرف *راستہ, سائز_t راستہ_سائز);
