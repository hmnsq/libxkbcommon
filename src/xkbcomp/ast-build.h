/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "scanner-utils.h"

व्यंजक_Def *
व्यंजक_Create_String(xkb_atom_t str);

व्यंजक_Def *
व्यंजक_Create_Integer(int64_t ival);

व्यंजक_Def *
व्यंजक_Create_Float(शून्य);

व्यंजक_Def *
व्यंजक_Create_Boolean(bool set);

व्यंजक_Def *
व्यंजक_Create_कुंजी_Name(xkb_atom_t कुंजी_name);

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym(xkb_keysym_t keysym);

व्यंजक_Def *
व्यंजक_Create_Ident(xkb_atom_t ident);

व्यंजक_Def *
व्यंजक_Create_Unary(गणना stmt_type op, व्यंजक_Def *child);

व्यंजक_Def *
व्यंजक_Create_द्विआधारी(गणना stmt_type op, व्यंजक_Def *बायाँ, व्यंजक_Def *दायाँ);

व्यंजक_Def *
व्यंजक_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

व्यंजक_Def *
व्यंजक_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, व्यंजक_Def *entry);

व्यंजक_Def *
व्यंजक_Empty_List(शून्य);

व्यंजक_Def *
व्यंजक_Create_Action(xkb_atom_t name, व्यंजक_Def *args);

व्यंजक_Def *
व्यंजक_Create_Action_List(व्यंजक_Def *actions);

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym_List(xkb_keysym_t sym);

व्यंजक_Def *
व्यंजक_Append_कुंजी_Sym_List(व्यंजक_Def *list, xkb_keysym_t sym);

व्यंजक_Def *
व्यंजक_कुंजी_Sym_List_Append_String(संरचना scanner *param,
                           व्यंजक_Def *व्यंजक, ध्रुव वर्ण *string);

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मूल्य);

कुंजी_Alias_Def *
कुंजी_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, व्यंजक_Def *मूल्य);

VarDef *
VarCreate(व्यंजक_Def *name, व्यंजक_Def *मूल्य);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यंजक_Def *match);

कुंजी_Type_Def *
कुंजी_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t कुंजी_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, व्यंजक_Def *def);

ModMapDef *
ModMapCreate(व्यंजक_Def *modifiers, व्यंजक_Def *कुंजियाँ);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, व्यंजक_Def *name, bool आभास);

UnknownStatement *
UnknownStatementCreate(गणना stmt_type, संरचना sval name);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge);

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags);

शून्य
FreeStmt(ParseCommon *stmt);
