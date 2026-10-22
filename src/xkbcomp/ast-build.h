/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

دەربڕین_Def *
دەربڕین_Create_String(xkb_atom_t str);

دەربڕین_Def *
دەربڕین_Create_Integer(int64_t ival);

دەربڕین_Def *
دەربڕین_Create_Float(فراغ);

دەربڕین_Def *
دەربڕین_Create_Boolean(bool set);

دەربڕین_Def *
دەربڕین_Create_کلیل_Name(xkb_atom_t کلیل_name);

دەربڕین_Def *
دەربڕین_Create_کلیل_Sym(xkb_keysym_t keysym);

دەربڕین_Def *
دەربڕین_Create_Ident(xkb_atom_t ident);

دەربڕین_Def *
دەربڕین_Create_Unary(تعداد stmt_type op, دەربڕین_Def *child);

دەربڕین_Def *
دەربڕین_Create_دووانەیی(تعداد stmt_type op, دەربڕین_Def *چەپ, دەربڕین_Def *ڕاست);

دەربڕین_Def *
دەربڕین_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

دەربڕین_Def *
دەربڕین_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, دەربڕین_Def *entry);

دەربڕین_Def *
دەربڕین_Empty_List(فراغ);

دەربڕین_Def *
دەربڕین_Create_Action(xkb_atom_t name, دەربڕین_Def *args);

دەربڕین_Def *
دەربڕین_Create_Action_List(دەربڕین_Def *actions);

دەربڕین_Def *
دەربڕین_Create_کلیل_Sym_List(xkb_keysym_t sym);

دەربڕین_Def *
دەربڕین_Append_کلیل_Sym_List(دەربڕین_Def *list, xkb_keysym_t sym);

دەربڕین_Def *
دەربڕین_کلیل_Sym_List_Append_String(بنية scanner *param,
                           دەربڕین_Def *دەربڕین, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t بەها);

کلیل_Alias_Def *
کلیل_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, دەربڕین_Def *بەها);

VarDef *
VarCreate(دەربڕین_Def *name, دەربڕین_Def *بەها);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, دەربڕین_Def *match);

کلیل_Type_Def *
کلیل_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t کلیل_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, دەربڕین_Def *def);

ModMapDef *
ModMapCreate(دەربڕین_Def *modifiers, دەربڕین_Def *کلیلەکان);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, دەربڕین_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
