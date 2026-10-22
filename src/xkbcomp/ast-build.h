/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "scanner-utils.h"

ৰাশিমালা_Def *
ৰাশিমালা_Create_String(xkb_atom_t str);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Integer(int64_t ival);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Float(শূন্য);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Boolean(bool set);

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Name(xkb_atom_t চাবি_name);

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Sym(xkb_keysym_t keysym);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Ident(xkb_atom_t ident);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Unary(গণনা stmt_type op, ৰাশিমালা_Def *child);

ৰাশিমালা_Def *
ৰাশিমালা_Create_দ্বিমিক(গণনা stmt_type op, ৰাশিমালা_Def *বাম, ৰাশিমালা_Def *ডান);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ৰাশিমালা_Def *entry);

ৰাশিমালা_Def *
ৰাশিমালা_Empty_List(শূন্য);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Action(xkb_atom_t name, ৰাশিমালা_Def *args);

ৰাশিমালা_Def *
ৰাশিমালা_Create_Action_List(ৰাশিমালা_Def *actions);

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Sym_List(xkb_keysym_t sym);

ৰাশিমালা_Def *
ৰাশিমালা_Append_চাবি_Sym_List(ৰাশিমালা_Def *list, xkb_keysym_t sym);

ৰাশিমালা_Def *
ৰাশিমালা_চাবি_Sym_List_Append_String(সংরচনা scanner *param,
                           ৰাশিমালা_Def *ৰাশিমালা, ধ্রুৱ ৱর্ণ *string);

xkb_keysym_t
KeysymParseString(সংরচনা scanner *scanner, ধ্রুৱ ৱর্ণ *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t মান);

চাবি_Alias_Def *
চাবি_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ৰাশিমালা_Def *মান);

VarDef *
VarCreate(ৰাশিমালা_Def *name, ৰাশিমালা_Def *মান);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, ৰাশিমালা_Def *match);

চাবি_Type_Def *
চাবি_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t চাবি_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, ৰাশিমালা_Def *def);

ModMapDef *
ModMapCreate(ৰাশিমালা_Def *modifiers, ৰাশিমালা_Def *চাবিগুলি);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ৰাশিমালা_Def *name, bool আভাস);

UnknownStatement *
UnknownStatementCreate(গণনা stmt_type, সংরচনা sval name);

IncludeStmt *
IncludeCreate(সংরচনা xkb_context *ctx, ৱর্ণ *str, গণনা merge_mode merge);

XkbFile *
XkbFileCreate(গণনা xkb_file_type type, ৱর্ণ *name, ParseCommon *defs,
              গণনা xkb_map_flags flags);

শূন্য
FreeStmt(ParseCommon *stmt);
