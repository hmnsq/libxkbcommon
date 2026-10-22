/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_String(xkb_atom_t str);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Integer(int64_t ival);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Float(ଶୂନ୍ୟ);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Boolean(bool set);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Name(xkb_atom_t ଚାବି_name);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Sym(xkb_keysym_t keysym);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Ident(xkb_atom_t ident);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Unary(ଗଣନା stmt_type op, ପଦସମଷ୍ଟି_Def *child);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(ଗଣନା stmt_type op, ପଦସମଷ୍ଟି_Def *ବାମ, ପଦସମଷ୍ଟି_Def *ଡାହାଣ);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ପଦସମଷ୍ଟି_Def *entry);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Empty_List(ଶୂନ୍ୟ);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Action(xkb_atom_t name, ପଦସମଷ୍ଟି_Def *args);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Action_List(ପଦସମଷ୍ଟି_Def *actions);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(xkb_keysym_t sym);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Append_ଚାବି_Sym_List(ପଦସମଷ୍ଟି_Def *list, xkb_keysym_t sym);

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_ଚାବି_Sym_List_Append_String(ସଂରଚନା scanner *param,
                           ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, ଅଚଳ ଅକ୍ଷର *string);

xkb_keysym_t
KeysymParseString(ସଂରଚନା scanner *scanner, ଅଚଳ ଅକ୍ଷର *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ମୂଲ୍ୟ);

ଚାବି_Alias_Def *
ଚାବି_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ);

VarDef *
VarCreate(ପଦସମଷ୍ଟି_Def *name, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, ପଦସମଷ୍ଟି_Def *match);

ଚାବି_Type_Def *
ଚାବି_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ଚାବି_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, ପଦସମଷ୍ଟି_Def *def);

ModMapDef *
ModMapCreate(ପଦସମଷ୍ଟି_Def *modifiers, ପଦସମଷ୍ଟି_Def *ଚାବିଗୁଡ଼ିକ);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ପଦସମଷ୍ଟି_Def *name, bool ଆଭାସୀ);

UnknownStatement *
UnknownStatementCreate(ଗଣନା stmt_type, ସଂରଚନା sval name);

IncludeStmt *
IncludeCreate(ସଂରଚନା xkb_context *ctx, ଅକ୍ଷର *str, ଗଣନା merge_mode merge);

XkbFile *
XkbFileCreate(ଗଣନା xkb_file_type type, ଅକ୍ଷର *name, ParseCommon *defs,
              ଗଣନା xkb_map_flags flags);

ଶୂନ୍ୟ
FreeStmt(ParseCommon *stmt);
