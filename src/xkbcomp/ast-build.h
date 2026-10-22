/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#هدايت once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

اظهار_Def *
اظهار_Create_String(xkb_atom_t str);

اظهار_Def *
اظهار_Create_Integer(int64_t ival);

اظهار_Def *
اظهار_Create_Float(فراغ);

اظهار_Def *
اظهار_Create_Boolean(bool سيٽ);

اظهار_Def *
اظهار_Create_ڪنجي_Name(xkb_atom_t ڪنجي_name);

اظهار_Def *
اظهار_Create_ڪنجي_Sym(xkb_keysym_t keysym);

اظهار_Def *
اظهار_Create_Ident(xkb_atom_t ident);

اظهار_Def *
اظهار_Create_Unary(تعداد stmt_type op, اظهار_Def *ٻار);

اظهار_Def *
اظهار_Create_ٻٽو(تعداد stmt_type op, اظهار_Def *کاٻو, اظهار_Def *ساڄو);

اظهار_Def *
اظهار_Create_ميدان_Ref(xkb_atom_t element, xkb_atom_t ميدان_2);

اظهار_Def *
اظهار_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ميدان_2, اظهار_Def *داخلا);

اظهار_Def *
اظهار_خالي_جايون_List(فراغ);

اظهار_Def *
اظهار_Create_Action(xkb_atom_t name, اظهار_Def *دليلون);

اظهار_Def *
اظهار_Create_Action_List(اظهار_Def *actions);

اظهار_Def *
اظهار_Create_ڪنجي_Sym_List(xkb_keysym_t sym);

اظهار_Def *
اظهار_Append_ڪنجي_Sym_List(اظهار_Def *list, xkb_keysym_t sym);

اظهار_Def *
اظهار_ڪنجي_Sym_List_Append_String(بنية scanner *param,
                           اظهار_Def *اظهار, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قدر);

ڪنجي_Alias_Def *
ڪنجي_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, اظهار_Def *قدر);

VarDef *
VarCreate(اظهار_Def *name, اظهار_Def *قدر);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سيٽ);

InterpDef *
InterpCreate(xkb_keysym_t sym, اظهار_Def *ميلاپ);

ڪنجي_Type_Def *
ڪنجي_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ڪنجي_Name, VarDef *symbols);

گروهه_Compat_Def *
گروهه_Compat_Create(int64_t گروهه, اظهار_Def *def);

ModMapDef *
ModMapCreate(اظهار_Def *modifiers, اظهار_Def *ڪنجيون);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, اظهار_Def *name, bool مجازي);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد ملايو_گهڻائي ملايو);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
