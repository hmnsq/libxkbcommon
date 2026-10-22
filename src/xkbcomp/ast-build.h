/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "ast.h"
#شامل "scanner-utils.h"

عبارت_Def *
عبارت_Create_String(xkb_atom_t str);

عبارت_Def *
عبارت_Create_Integer(int64_t ival);

عبارت_Def *
عبارت_Create_Float(تهی);

عبارت_Def *
عبارت_Create_Boolean(bool set);

عبارت_Def *
عبارت_Create_کلید_Name(xkb_atom_t کلید_name);

عبارت_Def *
عبارت_Create_کلید_Sym(xkb_keysym_t keysym);

عبارت_Def *
عبارت_Create_Ident(xkb_atom_t ident);

عبارت_Def *
عبارت_Create_Unary(شمارش stmt_type op, عبارت_Def *child);

عبارت_Def *
عبارت_Create_دودویی(شمارش stmt_type op, عبارت_Def *چپ, عبارت_Def *راست);

عبارت_Def *
عبارت_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

عبارت_Def *
عبارت_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, عبارت_Def *entry);

عبارت_Def *
عبارت_Empty_List(تهی);

عبارت_Def *
عبارت_Create_Action(xkb_atom_t name, عبارت_Def *args);

عبارت_Def *
عبارت_Create_Action_List(عبارت_Def *actions);

عبارت_Def *
عبارت_Create_کلید_Sym_List(xkb_keysym_t sym);

عبارت_Def *
عبارت_Append_کلید_Sym_List(عبارت_Def *list, xkb_keysym_t sym);

عبارت_Def *
عبارت_کلید_Sym_List_Append_String(ساختار scanner *param,
                           عبارت_Def *عبارت, ثابت نویسه *string);

xkb_keysym_t
KeysymParseString(ساختار scanner *scanner, ثابت نویسه *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t مقدار);

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, عبارت_Def *مقدار);

VarDef *
VarCreate(عبارت_Def *name, عبارت_Def *مقدار);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, عبارت_Def *match);

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, عبارت_Def *def);

ModMapDef *
ModMapCreate(عبارت_Def *modifiers, عبارت_Def *کلیدها);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, عبارت_Def *name, bool مجازی);

UnknownStatement *
UnknownStatementCreate(شمارش stmt_type, ساختار sval name);

IncludeStmt *
IncludeCreate(ساختار xkb_context *ctx, نویسه *str, شمارش merge_mode merge);

XkbFile *
XkbFileCreate(شمارش xkb_file_type type, نویسه *name, ParseCommon *defs,
              شمارش xkb_map_flags flags);

تهی
FreeStmt(ParseCommon *stmt);
