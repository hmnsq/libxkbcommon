/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#رهنمود once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

عبارت_Def *
عبارت_Create_String(xkb_atom_t str);

عبارت_Def *
عبارت_Create_Integer(int64_t ival);

عبارت_Def *
عبارت_Create_Float(فراغ);

عبارت_Def *
عبارت_Create_Boolean(bool مجموعه);

عبارت_Def *
عبارت_Create_کلید_Name(xkb_atom_t کلید_name);

عبارت_Def *
عبارت_Create_کلید_Sym(xkb_keysym_t keysym);

عبارت_Def *
عبارت_Create_Ident(xkb_atom_t ident);

عبارت_Def *
عبارت_Create_Unary(تعداد stmt_type op, عبارت_Def *فرزند);

عبارت_Def *
عبارت_Create_دودویی(تعداد stmt_type op, عبارت_Def *چپ, عبارت_Def *راست);

عبارت_Def *
عبارت_Create_فیلد_Ref(xkb_atom_t element, xkb_atom_t فیلد_2);

عبارت_Def *
عبارت_Create_Array_Ref(xkb_atom_t element, xkb_atom_t فیلد_2, عبارت_Def *مدخل);

عبارت_Def *
عبارت_خالی_List(فراغ);

عبارت_Def *
عبارت_Create_Action(xkb_atom_t name, عبارت_Def *آرگومانها);

عبارت_Def *
عبارت_Create_Action_List(عبارت_Def *actions);

عبارت_Def *
عبارت_Create_کلید_Sym_List(xkb_keysym_t sym);

عبارت_Def *
عبارت_Append_کلید_Sym_List(عبارت_Def *list, xkb_keysym_t sym);

عبارت_Def *
عبارت_کلید_Sym_List_Append_String(بنية scanner *param,
                           عبارت_Def *عبارت, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t مقدار);

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, عبارت_Def *مقدار);

VarDef *
VarCreate(عبارت_Def *name, عبارت_Def *مقدار);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool مجموعه);

InterpDef *
InterpCreate(xkb_keysym_t sym, عبارت_Def *تطابق);

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols);

گروه_Compat_Def *
گروه_Compat_Create(int64_t گروه, عبارت_Def *def);

ModMapDef *
ModMapCreate(عبارت_Def *modifiers, عبارت_Def *کلیدها);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, عبارت_Def *name, bool مجازی);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد ادغام_نما ادغام);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
