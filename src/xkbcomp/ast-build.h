/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

بیان_Def *
بیان_Create_String(xkb_atom_t str);

بیان_Def *
بیان_Create_Integer(int64_t ival);

بیان_Def *
بیان_Create_Float(فراغ);

بیان_Def *
بیان_Create_Boolean(bool سېټ);

بیان_Def *
بیان_Create_کلی_Name(xkb_atom_t کلی_name);

بیان_Def *
بیان_Create_کلی_Sym(xkb_keysym_t keysym);

بیان_Def *
بیان_Create_Ident(xkb_atom_t ident);

بیان_Def *
بیان_Create_Unary(تعداد stmt_type op, بیان_Def *ماشوم);

بیان_Def *
بیان_Create_دوګونی(تعداد stmt_type op, بیان_Def *کیڼ, بیان_Def *ښی);

بیان_Def *
بیان_Create_ډګر_Ref(xkb_atom_t element, xkb_atom_t ډګر_2);

بیان_Def *
بیان_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ډګر_2, بیان_Def *ثبت);

بیان_Def *
بیان_تشه_List(فراغ);

بیان_Def *
بیان_Create_Action(xkb_atom_t name, بیان_Def *دلیلونه);

بیان_Def *
بیان_Create_Action_List(بیان_Def *actions);

بیان_Def *
بیان_Create_کلی_Sym_List(xkb_keysym_t sym);

بیان_Def *
بیان_Append_کلی_Sym_List(بیان_Def *list, xkb_keysym_t sym);

بیان_Def *
بیان_کلی_Sym_List_Append_String(بنية scanner *param,
                           بیان_Def *بیان, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ارزښت);

کلی_Alias_Def *
کلی_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, بیان_Def *ارزښت);

VarDef *
VarCreate(بیان_Def *name, بیان_Def *ارزښت);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سېټ);

InterpDef *
InterpCreate(xkb_keysym_t sym, بیان_Def *سمون);

کلی_Type_Def *
کلی_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کلی_Name, VarDef *symbols);

ډله_Compat_Def *
ډله_Compat_Create(int64_t ډله, بیان_Def *def);

ModMapDef *
ModMapCreate(بیان_Def *modifiers, بیان_Def *کلیګانې);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, بیان_Def *name, bool مجازي);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد یوځای_ډېرپېښ یوځای);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
