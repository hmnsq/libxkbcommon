/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

اوڠکاڤن_Def *
اوڠکاڤن_Create_String(xkb_atom_t str);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Integer(int64_t ival);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Float(فراغ);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Boolean(bool set);

اوڠکاڤن_Def *
اوڠکاڤن_Create_کونچي_Name(xkb_atom_t کونچي_name);

اوڠکاڤن_Def *
اوڠکاڤن_Create_کونچي_Sym(xkb_keysym_t keysym);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Ident(xkb_atom_t ident);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Unary(تعداد stmt_type op, اوڠکاڤن_Def *child);

اوڠکاڤن_Def *
اوڠکاڤن_Create_بيناري(تعداد stmt_type op, اوڠکاڤن_Def *کيري, اوڠکاڤن_Def *کانن);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, اوڠکاڤن_Def *entry);

اوڠکاڤن_Def *
اوڠکاڤن_Empty_List(فراغ);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Action(xkb_atom_t name, اوڠکاڤن_Def *args);

اوڠکاڤن_Def *
اوڠکاڤن_Create_Action_List(اوڠکاڤن_Def *actions);

اوڠکاڤن_Def *
اوڠکاڤن_Create_کونچي_Sym_List(xkb_keysym_t sym);

اوڠکاڤن_Def *
اوڠکاڤن_Append_کونچي_Sym_List(اوڠکاڤن_Def *list, xkb_keysym_t sym);

اوڠکاڤن_Def *
اوڠکاڤن_کونچي_Sym_List_Append_String(بنية scanner *param,
                           اوڠکاڤن_Def *اوڠکاڤن, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t نيلاي);

کونچي_Alias_Def *
کونچي_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, اوڠکاڤن_Def *نيلاي);

VarDef *
VarCreate(اوڠکاڤن_Def *name, اوڠکاڤن_Def *نيلاي);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, اوڠکاڤن_Def *match);

کونچي_Type_Def *
کونچي_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کونچي_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, اوڠکاڤن_Def *def);

ModMapDef *
ModMapCreate(اوڠکاڤن_Def *modifiers, اوڠکاڤن_Def *کونچي_سموا);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, اوڠکاڤن_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
