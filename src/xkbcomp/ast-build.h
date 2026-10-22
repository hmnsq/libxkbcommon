/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include "ast.h"
#include "scanner-utils.h"

पाहायाग_Def *
पाहायाग_Create_String(xkb_atom_t str);

पाहायाग_Def *
पाहायाग_Create_Integer(int64_t ival);

पाहायाग_Def *
पाहायाग_Create_Float(void);

पाहायाग_Def *
पाहायाग_Create_Boolean(bool set);

पाहायाग_Def *
पाहायाग_Create_सुसि_Name(xkb_atom_t सुसि_name);

पाहायाग_Def *
पाहायाग_Create_सुसि_Sym(xkb_keysym_t keysym);

पाहायाग_Def *
पाहायाग_Create_Ident(xkb_atom_t ident);

पाहायाग_Def *
पाहायाग_Create_Unary(enum stmt_type op, पाहायाग_Def *child);

पाहायाग_Def *
पाहायाग_Create_दालावाहान(enum stmt_type op, पाहायाग_Def *कालिवा, पाहायाग_Def *कानान);

पाहायाग_Def *
पाहायाग_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

पाहायाग_Def *
पाहायाग_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, पाहायाग_Def *entry);

पाहायाग_Def *
पाहायाग_Empty_List(void);

पाहायाग_Def *
पाहायाग_Create_Action(xkb_atom_t name, पाहायाग_Def *मगा_कातविरान);

पाहायाग_Def *
पाहायाग_Create_Action_List(पाहायाग_Def *actions);

पाहायाग_Def *
पाहायाग_Create_सुसि_Sym_List(xkb_keysym_t sym);

पाहायाग_Def *
पाहायाग_Append_सुसि_Sym_List(पाहायाग_Def *list, xkb_keysym_t sym);

पाहायाग_Def *
पाहायाग_सुसि_Sym_List_Append_String(struct scanner *param,
                           पाहायाग_Def *पाहायाग, const char *string);

xkb_keysym_t
KeysymParseString(struct scanner *scanner, const char *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t हालागा);

सुसि_Alias_Def *
सुसि_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, पाहायाग_Def *हालागा);

VarDef *
VarCreate(पाहायाग_Def *name, पाहायाग_Def *हालागा);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, पाहायाग_Def *match);

सुसि_Type_Def *
सुसि_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t सुसि_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, पाहायाग_Def *def);

ModMapDef *
ModMapCreate(पाहायाग_Def *modifiers, पाहायाग_Def *मगा_सुसि);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, पाहायाग_Def *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(enum stmt_type, struct sval name);

IncludeStmt *
IncludeCreate(struct xkb_context *ctx, char *str, enum merge_mode merge);

XkbFile *
XkbFileCreate(enum xkb_file_type type, char *name, ParseCommon *defs,
              enum xkb_map_flags flags);

void
FreeStmt(ParseCommon *stmt);
