/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "ast.h"
#ضمن "scanner-utils.h"

ExprDef *
ExprCreateString(xkb_atom_t str);

ExprDef *
ExprCreateInteger(int64_t ival);

ExprDef *
ExprCreateFloat(فراغ);

ExprDef *
ExprCreateBoolean(bool set);

ExprDef *
ExprCreateKeyName(xkb_atom_t key_name);

ExprDef *
ExprCreateKeySym(xkb_keysym_t keysym);

ExprDef *
ExprCreateIdent(xkb_atom_t ident);

ExprDef *
ExprCreateUnary(تعداد stmt_type op, ExprDef *child);

ExprDef *
ExprCreateBinary(تعداد stmt_type op, ExprDef *left, ExprDef *right);

ExprDef *
ExprCreateFieldRef(xkb_atom_t element, xkb_atom_t field);

ExprDef *
ExprCreateArrayRef(xkb_atom_t element, xkb_atom_t field, ExprDef *entry);

ExprDef *
ExprEmptyList(فراغ);

ExprDef *
ExprCreateAction(xkb_atom_t name, ExprDef *args);

ExprDef *
ExprCreateActionList(ExprDef *actions);

ExprDef *
ExprCreateKeySymList(xkb_keysym_t sym);

ExprDef *
ExprAppendKeySymList(ExprDef *list, xkb_keysym_t sym);

ExprDef *
ExprKeySymListAppendString(بنية scanner *param,
                           ExprDef *expr, ثابت حرف *string);

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t value);

KeyAliasDef *
KeyAliasCreate(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ExprDef *value);

VarDef *
VarCreate(ExprDef *name, ExprDef *value);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, ExprDef *match);

KeyTypeDef *
KeyTypeCreate(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t keyName, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, ExprDef *def);

ModMapDef *
ModMapCreate(ExprDef *modifiers, ExprDef *keys);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ExprDef *name, bool virtual);

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type, بنية sval name);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge);

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags);

فراغ
FreeStmt(ParseCommon *stmt);
