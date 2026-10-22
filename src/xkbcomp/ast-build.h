/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

اظہاریہ_Def *
اظہاریہ_Create_String(xkb_atom_t str);

اظہاریہ_Def *
اظہاریہ_Create_Integer(int64_t ival);

اظہاریہ_Def *
اظہاریہ_Create_Float(فراغ);

اظہاریہ_Def *
اظہاریہ_Create_Boolean(bool سیٹ);

اظہاریہ_Def *
اظہاریہ_Create_کلید_Name(xkb_atom_t کلید_name);

اظہاریہ_Def *
اظہاریہ_Create_کلید_Sym(xkb_keysym_t keysym);

اظہاریہ_Def *
اظہاریہ_Create_Ident(xkb_atom_t ident);

اظہاریہ_Def *
اظہاریہ_Create_Unary(تعداد stmt_type op, اظہاریہ_Def *بچہ);

اظہاریہ_Def *
اظہاریہ_Create_ثنائی(تعداد stmt_type op, اظہاریہ_Def *بایاں, اظہاریہ_Def *دایاں);

اظہاریہ_Def *
اظہاریہ_Create_میدان_Ref(xkb_atom_t element, xkb_atom_t میدان_2);

اظہاریہ_Def *
اظہاریہ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t میدان_2, اظہاریہ_Def *اندراج);

اظہاریہ_Def *
اظہاریہ_خالی_تھاواں_List(فراغ);

اظہاریہ_Def *
اظہاریہ_Create_Action(xkb_atom_t name, اظہاریہ_Def *دلیلاں);

اظہاریہ_Def *
اظہاریہ_Create_Action_List(اظہاریہ_Def *actions);

اظہاریہ_Def *
اظہاریہ_Create_کلید_Sym_List(xkb_keysym_t sym);

اظہاریہ_Def *
اظہاریہ_Append_کلید_Sym_List(اظہاریہ_Def *list, xkb_keysym_t sym);

اظہاریہ_Def *
اظہاریہ_کلید_Sym_List_Append_String(بنية scanner *param,
                           اظہاریہ_Def *اظہاریہ, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قدر);

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, اظہاریہ_Def *قدر);

VarDef *
VarCreate(اظہاریہ_Def *name, اظہاریہ_Def *قدر);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سیٹ);

InterpDef *
InterpCreate(xkb_keysym_t sym, اظہاریہ_Def *میل);

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols);

گروہ_Compat_Def *
گروہ_Compat_Create(int64_t گروہ, اظہاریہ_Def *def);

ModMapDef *
ModMapCreate(اظہاریہ_Def *modifiers, اظہاریہ_Def *کلیدیں);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, اظہاریہ_Def *name, bool مجازی);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد ملاؤ_منوال ملاؤ);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
