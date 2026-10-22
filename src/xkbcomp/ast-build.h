/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "scanner-utils.h"

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_String(xkb_atom_t str);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Integer(int64_t ival);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Float(शून्य);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Boolean(bool set);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Name(xkb_atom_t किल्ली_name);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Sym(xkb_keysym_t keysym);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Ident(xkb_atom_t ident);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Unary(गणना stmt_type op, अभिव्यक्ती_Def *child);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_द्विमान(गणना stmt_type op, अभिव्यक्ती_Def *डावा, अभिव्यक्ती_Def *उजवा);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, अभिव्यक्ती_Def *entry);

अभिव्यक्ती_Def *
अभिव्यक्ती_Empty_List(शून्य);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Action(xkb_atom_t name, अभिव्यक्ती_Def *args);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Action_List(अभिव्यक्ती_Def *actions);

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Sym_List(xkb_keysym_t sym);

अभिव्यक्ती_Def *
अभिव्यक्ती_Append_किल्ली_Sym_List(अभिव्यक्ती_Def *list, xkb_keysym_t sym);

अभिव्यक्ती_Def *
अभिव्यक्ती_किल्ली_Sym_List_Append_String(संरचना scanner *param,
                           अभिव्यक्ती_Def *अभिव्यक्ती, ध्रुव वर्ण *string);

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मूल्य);

किल्ली_Alias_Def *
किल्ली_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, अभिव्यक्ती_Def *मूल्य);

VarDef *
VarCreate(अभिव्यक्ती_Def *name, अभिव्यक्ती_Def *मूल्य);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, अभिव्यक्ती_Def *match);

किल्ली_Type_Def *
किल्ली_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t किल्ली_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, अभिव्यक्ती_Def *def);

ModMapDef *
ModMapCreate(अभिव्यक्ती_Def *modifiers, अभिव्यक्ती_Def *किल्ल्या);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, अभिव्यक्ती_Def *name, bool आभास);

UnknownStatement *
UnknownStatementCreate(गणना stmt_type, संरचना sval name);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge);

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags);

शून्य
FreeStmt(ParseCommon *stmt);
