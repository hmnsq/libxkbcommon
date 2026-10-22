/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "scanner-utils.h"

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_String(xkb_atom_t str);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Integer(int64_t ival);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Float(शून्य);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Boolean(bool set);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Name(xkb_atom_t साँचो_name);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Sym(xkb_keysym_t keysym);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Ident(xkb_atom_t ident);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Unary(गणना stmt_type op, अभिव्यक्ति_Def *child);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_द्विआधारी(गणना stmt_type op, अभिव्यक्ति_Def *बायाँ, अभिव्यक्ति_Def *दायाँ);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, अभिव्यक्ति_Def *entry);

अभिव्यक्ति_Def *
अभिव्यक्ति_Empty_List(शून्य);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Action(xkb_atom_t name, अभिव्यक्ति_Def *args);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Action_List(अभिव्यक्ति_Def *actions);

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Sym_List(xkb_keysym_t sym);

अभिव्यक्ति_Def *
अभिव्यक्ति_Append_साँचो_Sym_List(अभिव्यक्ति_Def *list, xkb_keysym_t sym);

अभिव्यक्ति_Def *
अभिव्यक्ति_साँचो_Sym_List_Append_String(संरचना scanner *param,
                           अभिव्यक्ति_Def *अभिव्यक्ति, अचर वर्ण *string);

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, अचर वर्ण *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मान);

साँचो_Alias_Def *
साँचो_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, अभिव्यक्ति_Def *मान);

VarDef *
VarCreate(अभिव्यक्ति_Def *name, अभिव्यक्ति_Def *मान);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, अभिव्यक्ति_Def *match);

साँचो_Type_Def *
साँचो_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t साँचो_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, अभिव्यक्ति_Def *def);

ModMapDef *
ModMapCreate(अभिव्यक्ति_Def *modifiers, अभिव्यक्ति_Def *साँचोहरू);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, अभिव्यक्ति_Def *name, bool आभास);

UnknownStatement *
UnknownStatementCreate(गणना stmt_type, संरचना sval name);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge);

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags);

शून्य
FreeStmt(ParseCommon *stmt);
