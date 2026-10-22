/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "scanner-utils.h"

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_String(xkb_atom_t str);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Integer(int64_t ival);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Float(ശൂന്യം);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Boolean(bool set);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Name(xkb_atom_t താക്കോൽ_name);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Sym(xkb_keysym_t keysym);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Ident(xkb_atom_t ident);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Unary(എണ്ണൽ stmt_type op, വ്യഞ്ജകം_Def *child);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_ദ്വിമാന(എണ്ണൽ stmt_type op, വ്യഞ്ജകം_Def *ഇടത്, വ്യഞ്ജകം_Def *വലത്);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, വ്യഞ്ജകം_Def *entry);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Empty_List(ശൂന്യം);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Action(xkb_atom_t name, വ്യഞ്ജകം_Def *args);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Action_List(വ്യഞ്ജകം_Def *actions);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(xkb_keysym_t sym);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Append_താക്കോൽ_Sym_List(വ്യഞ്ജകം_Def *list, xkb_keysym_t sym);

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_താക്കോൽ_Sym_List_Append_String(ഘടന scanner *param,
                           വ്യഞ്ജകം_Def *വ്യഞ്ജകം, സ്ഥിരം അക്ഷരം *string);

xkb_keysym_t
KeysymParseString(ഘടന scanner *scanner, സ്ഥിരം അക്ഷരം *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t മൂല്യം);

താക്കോൽ_Alias_Def *
താക്കോൽ_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, വ്യഞ്ജകം_Def *മൂല്യം);

VarDef *
VarCreate(വ്യഞ്ജകം_Def *name, വ്യഞ്ജകം_Def *മൂല്യം);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, വ്യഞ്ജകം_Def *match);

താക്കോൽ_Type_Def *
താക്കോൽ_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t താക്കോൽ_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, വ്യഞ്ജകം_Def *def);

ModMapDef *
ModMapCreate(വ്യഞ്ജകം_Def *modifiers, വ്യഞ്ജകം_Def *താക്കോലുകൾ);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, വ്യഞ്ജകം_Def *name, bool മായാ);

UnknownStatement *
UnknownStatementCreate(എണ്ണൽ stmt_type, ഘടന sval name);

IncludeStmt *
IncludeCreate(ഘടന xkb_context *ctx, അക്ഷരം *str, എണ്ണൽ merge_mode merge);

XkbFile *
XkbFileCreate(എണ്ണൽ xkb_file_type type, അക്ഷരം *name, ParseCommon *defs,
              എണ്ണൽ xkb_map_flags flags);

ശൂന്യം
FreeStmt(ParseCommon *stmt);
