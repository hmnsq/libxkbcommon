/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

فرچ_Def *
فرچ_Create_String(xkb_atom_t str);

فرچ_Def *
فرچ_Create_Integer(int64_t ival);

فرچ_Def *
فرچ_Create_Float(فراغ);

فرچ_Def *
فرچ_Create_Boolean(bool set);

فرچ_Def *
فرچ_Create_مبودي_Name(xkb_atom_t مبودي_name);

فرچ_Def *
فرچ_Create_مبودي_Sym(xkb_keysym_t keysym);

فرچ_Def *
فرچ_Create_Ident(xkb_atom_t ident);

فرچ_Def *
فرچ_Create_Unary(تعداد stmt_type op, فرچ_Def *child);

فرچ_Def *
فرچ_Create_نبيو(تعداد stmt_type op, فرچ_Def *هغو, فرچ_Def *دام);

فرچ_Def *
فرچ_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

فرچ_Def *
فرچ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, فرچ_Def *entry);

فرچ_Def *
فرچ_Empty_List(فراغ);

فرچ_Def *
فرچ_Create_Action(xkb_atom_t name, فرچ_Def *حجوجي);

فرچ_Def *
فرچ_Create_Action_List(فرچ_Def *actions);

فرچ_Def *
فرچ_Create_مبودي_Sym_List(xkb_keysym_t sym);

فرچ_Def *
فرچ_Append_مبودي_Sym_List(فرچ_Def *list, xkb_keysym_t sym);

فرچ_Def *
فرچ_مبودي_Sym_List_Append_String(بنية scanner *param,
                           فرچ_Def *فرچ, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t درج);

مبودي_Alias_Def *
مبودي_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, فرچ_Def *درج);

VarDef *
VarCreate(فرچ_Def *name, فرچ_Def *درج);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, فرچ_Def *match);

مبودي_Type_Def *
مبودي_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t مبودي_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, فرچ_Def *def);

ModMapDef *
ModMapCreate(فرچ_Def *modifiers, فرچ_Def *مبوداي);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, فرچ_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
