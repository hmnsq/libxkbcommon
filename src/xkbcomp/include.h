/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

/* Reasonable threshold, with plenty of margin for keymaps in the wild */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_MAX_ਡੂੰਘਾਈ 15

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ '+'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ  '|'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ  '^'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_DEFAULT_ਅਗੇਤਰ  ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(ch) \
    ((ch) == ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ || (ch) == ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ || \
     (ch) == ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ)
ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਮਿਲਾਓ_ਬਹੁਲਕ_PREFIXES[] =
    { ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ, ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ, ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ, 0 };


bool
ParseIncludeMap(ਅੱਖਰ **str_inout, ਅੱਖਰ **file_rtrn, ਅੱਖਰ **map_rtrn,
                ਅੱਖਰ *nextop_rtrn, ਅੱਖਰ **extra_ਅੰਕੜੇ);

ssize_t
expand_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* ਮਾਪਾ_file_name,
            ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t name_len, ਗਿਣਤੀ xkb_file_type type,
            ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ);

FILE *
ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* ਮਾਪਾ_file_name,
                  ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t name_len, ਗਿਣਤੀ xkb_file_type type,
                  ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਵਿੱਥ,
                  bool required);

bool
Exceeds_Include_Max_ਡੂੰਘਾਈ(ਬਣਤਰ xkb_context *ctx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ);

XkbFile *
ProcessIncludeFile(ਬਣਤਰ xkb_context *ctx,
                   ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
                   ਅਚਲ IncludeStmt *stmt,
                   ਗਿਣਤੀ xkb_file_type file_type, ਅੱਖਰ *ਰਸਤਾ, ਮਾਪ_t ਰਸਤਾ_ਮਾਪ);
