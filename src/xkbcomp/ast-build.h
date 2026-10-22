/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "scanner-utils.h"

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_String(xkb_atom_t str);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Integer(int64_t ival);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Float(शून्य);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Boolean(bool set);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Name(xkb_atom_t कुञ्चिका_name);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym(xkb_keysym_t keysym);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Ident(xkb_atom_t ident);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Unary(गणना stmt_type op, व्यञ्जकम्_Def *child);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_द्विमान(गणना stmt_type op, व्यञ्जकम्_Def *वामः, व्यञ्जकम्_Def *दक्षिणः);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, व्यञ्जकम्_Def *entry);

व्यञ्जकम्_Def *
व्यञ्जकम्_Empty_List(शून्य);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action(xkb_atom_t name, व्यञ्जकम्_Def *args);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action_List(व्यञ्जकम्_Def *actions);

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym_List(xkb_keysym_t sym);

व्यञ्जकम्_Def *
व्यञ्जकम्_Append_कुञ्चिका_Sym_List(व्यञ्जकम्_Def *list, xkb_keysym_t sym);

व्यञ्जकम्_Def *
व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(संरचना scanner *param,
                           व्यञ्जकम्_Def *व्यञ्जकम्, ध्रुव वर्ण *string);

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मानम्);

कुञ्चिका_Alias_Def *
कुञ्चिका_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, व्यञ्जकम्_Def *मानम्);

VarDef *
VarCreate(व्यञ्जकम्_Def *name, व्यञ्जकम्_Def *मानम्);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यञ्जकम्_Def *match);

कुञ्चिका_Type_Def *
कुञ्चिका_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t कुञ्चिका_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, व्यञ्जकम्_Def *def);

ModMapDef *
ModMapCreate(व्यञ्जकम्_Def *modifiers, व्यञ्जकम्_Def *कुञ्चिकाः);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, व्यञ्जकम्_Def *name, bool आभास);

UnknownStatement *
UnknownStatementCreate(गणना stmt_type, संरचना sval name);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge);

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags);

शून्य
FreeStmt(ParseCommon *stmt);
