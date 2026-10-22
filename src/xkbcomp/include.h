/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#تعریف_کرو INCLUDE_MAX_ڈونگھائی 15

#تعریف_کرو ملاؤ_OVERRIDE_اگیتر '+'
#تعریف_کرو ملاؤ_AUGMENT_اگیتر  '|'
#تعریف_کرو ملاؤ_REPLACE_اگیتر  '^'
#تعریف_کرو ملاؤ_DEFAULT_اگیتر  ملاؤ_OVERRIDE_اگیتر
#تعریف_کرو is_ملاؤ_منوال_اگیتر(ch) \
    ((ch) == ملاؤ_OVERRIDE_اگیتر || (ch) == ملاؤ_AUGMENT_اگیتر || \
     (ch) == ملاؤ_REPLACE_اگیتر)
ساكن ثابت حرف ملاؤ_منوال_PREFIXES[] =
    { ملاؤ_OVERRIDE_اگیتر, ملاؤ_AUGMENT_اگیتر, ملاؤ_REPLACE_اگیتر, 0 };


bool
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_انکڑے);

ssize_t
expand_رستہ(بنية xkb_context *ctx, ثابت حرف* ماپا_file_name,
            ثابت حرف *name, ناپ_t name_len, تعداد xkb_file_type type,
            حرف *buf, ناپ_t buf_ناپ);

FILE *
لبھو_File_وچ_Xkb_رستہ(بنية xkb_context *ctx, ثابت حرف* ماپا_file_name,
                  ثابت حرف *name, ناپ_t name_len, تعداد xkb_file_type type,
                  حرف *buf, ناپ_t buf_ناپ, بنانشانی صحيح *وتھ,
                  bool required);

bool
Exceeds_Include_Max_ڈونگھائی(بنية xkb_context *ctx, بنانشانی صحيح include_ڈونگھائی);

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *رستہ, ناپ_t رستہ_ناپ);
