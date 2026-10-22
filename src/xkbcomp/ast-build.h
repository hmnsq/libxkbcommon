/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_String(xkb_atom_t str);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Integer(int64_t ival);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Float(ਖਾਲੀ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Boolean(bool ਸੈੱਟ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name(xkb_atom_t ਕੁੰਜੀ_name);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym(xkb_keysym_t keysym);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Ident(xkb_atom_t ident);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Unary(ਗਿਣਤੀ stmt_type op, ਵਿਅੰਜਕ_Def *ਬੱਚਾ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(ਗਿਣਤੀ stmt_type op, ਵਿਅੰਜਕ_Def *ਖੱਬਾ, ਵਿਅੰਜਕ_Def *ਸੱਜਾ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਖੇਤਰ_Ref(xkb_atom_t element, xkb_atom_t ਖੇਤਰ_2);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ਖੇਤਰ_2, ਵਿਅੰਜਕ_Def *ਇੰਦਰਾਜ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_List(ਖਾਲੀ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Action(xkb_atom_t name, ਵਿਅੰਜਕ_Def *ਦਲੀਲਾਂ);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Action_List(ਵਿਅੰਜਕ_Def *actions);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(xkb_keysym_t sym);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Append_ਕੁੰਜੀ_Sym_List(ਵਿਅੰਜਕ_Def *list, xkb_keysym_t sym);

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(ਬਣਤਰ scanner *param,
                           ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, ਅਚਲ ਅੱਖਰ *string);

xkb_keysym_t
KeysymParseString(ਬਣਤਰ scanner *scanner, ਅਚਲ ਅੱਖਰ *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ਮੁੱਲ);

ਕੁੰਜੀ_Alias_Def *
ਕੁੰਜੀ_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ਵਿਅੰਜਕ_Def *ਮੁੱਲ);

VarDef *
VarCreate(ਵਿਅੰਜਕ_Def *name, ਵਿਅੰਜਕ_Def *ਮੁੱਲ);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool ਸੈੱਟ);

InterpDef *
InterpCreate(xkb_keysym_t sym, ਵਿਅੰਜਕ_Def *ਮੇਲ);

ਕੁੰਜੀ_Type_Def *
ਕੁੰਜੀ_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ਕੁੰਜੀ_Name, VarDef *symbols);

ਸਮੂਹ_Compat_Def *
ਸਮੂਹ_Compat_Create(int64_t ਸਮੂਹ, ਵਿਅੰਜਕ_Def *def);

ModMapDef *
ModMapCreate(ਵਿਅੰਜਕ_Def *modifiers, ਵਿਅੰਜਕ_Def *ਕੁੰਜੀਆਂ);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ਵਿਅੰਜਕ_Def *name, bool ਆਭਾਸੀ);

UnknownStatement *
UnknownStatementCreate(ਗਿਣਤੀ stmt_type, ਬਣਤਰ sval name);

IncludeStmt *
IncludeCreate(ਬਣਤਰ xkb_context *ctx, ਅੱਖਰ *str, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ);

XkbFile *
XkbFileCreate(ਗਿਣਤੀ xkb_file_type type, ਅੱਖਰ *name, ParseCommon *defs,
              ਗਿਣਤੀ xkb_map_flags flags);

ਖਾਲੀ
FreeStmt(ParseCommon *stmt);
