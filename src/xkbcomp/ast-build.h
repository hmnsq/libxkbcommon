/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

ئىپادە_Def *
ئىپادە_Create_String(xkb_atom_t str);

ئىپادە_Def *
ئىپادە_Create_Integer(int64_t ival);

ئىپادە_Def *
ئىپادە_Create_Float(فراغ);

ئىپادە_Def *
ئىپادە_Create_Boolean(bool set);

ئىپادە_Def *
ئىپادە_Create_ئاچقۇچ_Name(xkb_atom_t ئاچقۇچ_name);

ئىپادە_Def *
ئىپادە_Create_ئاچقۇچ_Sym(xkb_keysym_t keysym);

ئىپادە_Def *
ئىپادە_Create_Ident(xkb_atom_t ident);

ئىپادە_Def *
ئىپادە_Create_Unary(تعداد stmt_type op, ئىپادە_Def *child);

ئىپادە_Def *
ئىپادە_Create_ئىككىلىك(تعداد stmt_type op, ئىپادە_Def *سول, ئىپادە_Def *ئوڭ);

ئىپادە_Def *
ئىپادە_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

ئىپادە_Def *
ئىپادە_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ئىپادە_Def *entry);

ئىپادە_Def *
ئىپادە_Empty_List(فراغ);

ئىپادە_Def *
ئىپادە_Create_Action(xkb_atom_t name, ئىپادە_Def *args);

ئىپادە_Def *
ئىپادە_Create_Action_List(ئىپادە_Def *actions);

ئىپادە_Def *
ئىپادە_Create_ئاچقۇچ_Sym_List(xkb_keysym_t sym);

ئىپادە_Def *
ئىپادە_Append_ئاچقۇچ_Sym_List(ئىپادە_Def *list, xkb_keysym_t sym);

ئىپادە_Def *
ئىپادە_ئاچقۇچ_Sym_List_Append_String(بنية scanner *param,
                           ئىپادە_Def *ئىپادە, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قىممەت);

ئاچقۇچ_Alias_Def *
ئاچقۇچ_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ئىپادە_Def *قىممەت);

VarDef *
VarCreate(ئىپادە_Def *name, ئىپادە_Def *قىممەت);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, ئىپادە_Def *match);

ئاچقۇچ_Type_Def *
ئاچقۇچ_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ئاچقۇچ_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, ئىپادە_Def *def);

ModMapDef *
ModMapCreate(ئىپادە_Def *modifiers, ئىپادە_Def *ئاچقۇچلار);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ئىپادە_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
