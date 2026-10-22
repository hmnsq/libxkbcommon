/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

تعبير_Def *
تعبير_Create_String(xkb_atom_t str);

تعبير_Def *
تعبير_Create_Integer(int64_t ival);

تعبير_Def *
تعبير_Create_Float(فراغ);

تعبير_Def *
تعبير_Create_Boolean(bool set);

تعبير_Def *
تعبير_Create_مفتاح_Name(xkb_atom_t مفتاح_name);

تعبير_Def *
تعبير_Create_مفتاح_Sym(xkb_keysym_t keysym);

تعبير_Def *
تعبير_Create_Ident(xkb_atom_t ident);

تعبير_Def *
تعبير_Create_Unary(تعداد stmt_type op, تعبير_Def *child);

تعبير_Def *
تعبير_Create_ثنائي(تعداد stmt_type op, تعبير_Def *يسار, تعبير_Def *يمين);

تعبير_Def *
تعبير_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

تعبير_Def *
تعبير_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, تعبير_Def *entry);

تعبير_Def *
تعبير_Empty_List(فراغ);

تعبير_Def *
تعبير_Create_Action(xkb_atom_t name, تعبير_Def *args);

تعبير_Def *
تعبير_Create_Action_List(تعبير_Def *actions);

تعبير_Def *
تعبير_Create_مفتاح_Sym_List(xkb_keysym_t sym);

تعبير_Def *
تعبير_Append_مفتاح_Sym_List(تعبير_Def *list, xkb_keysym_t sym);

تعبير_Def *
تعبير_مفتاح_Sym_List_Append_String(بنية scanner *param,
                           تعبير_Def *تعبير, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قيمة);

مفتاح_Alias_Def *
مفتاح_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, تعبير_Def *قيمة);

VarDef *
VarCreate(تعبير_Def *name, تعبير_Def *قيمة);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, تعبير_Def *match);

مفتاح_Type_Def *
مفتاح_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t مفتاح_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, تعبير_Def *def);

ModMapDef *
ModMapCreate(تعبير_Def *modifiers, تعبير_Def *مفاتيح);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, تعبير_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
