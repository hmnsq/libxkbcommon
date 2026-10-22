/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "scanner-utils.h"

પદાવલિ_Def *
પદાવલિ_Create_String(xkb_atom_t str);

પદાવલિ_Def *
પદાવલિ_Create_Integer(int64_t ival);

પદાવલિ_Def *
પદાવલિ_Create_Float(શૂન્ય);

પદાવલિ_Def *
પદાવલિ_Create_Boolean(bool set);

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Name(xkb_atom_t ચાવી_name);

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Sym(xkb_keysym_t keysym);

પદાવલિ_Def *
પદાવલિ_Create_Ident(xkb_atom_t ident);

પદાવલિ_Def *
પદાવલિ_Create_Unary(ગણના stmt_type op, પદાવલિ_Def *child);

પદાવલિ_Def *
પદાવલિ_Create_દ્વિઅંકી(ગણના stmt_type op, પદાવલિ_Def *ડાબું, પદાવલિ_Def *જમણું);

પદાવલિ_Def *
પદાવલિ_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

પદાવલિ_Def *
પદાવલિ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, પદાવલિ_Def *entry);

પદાવલિ_Def *
પદાવલિ_Empty_List(શૂન્ય);

પદાવલિ_Def *
પદાવલિ_Create_Action(xkb_atom_t name, પદાવલિ_Def *args);

પદાવલિ_Def *
પદાવલિ_Create_Action_List(પદાવલિ_Def *actions);

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Sym_List(xkb_keysym_t sym);

પદાવલિ_Def *
પદાવલિ_Append_ચાવી_Sym_List(પદાવલિ_Def *list, xkb_keysym_t sym);

પદાવલિ_Def *
પદાવલિ_ચાવી_Sym_List_Append_String(સંરચના scanner *param,
                           પદાવલિ_Def *પદાવલિ, અચલ અક્ષર *string);

xkb_keysym_t
KeysymParseString(સંરચના scanner *scanner, અચલ અક્ષર *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t કિંમત);

ચાવી_Alias_Def *
ચાવી_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, પદાવલિ_Def *કિંમત);

VarDef *
VarCreate(પદાવલિ_Def *name, પદાવલિ_Def *કિંમત);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, પદાવલિ_Def *match);

ચાવી_Type_Def *
ચાવી_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ચાવી_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, પદાવલિ_Def *def);

ModMapDef *
ModMapCreate(પદાવલિ_Def *modifiers, પદાવલિ_Def *ચાવીઓ);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, પદાવલિ_Def *name, bool આભાસી);

UnknownStatement *
UnknownStatementCreate(ગણના stmt_type, સંરચના sval name);

IncludeStmt *
IncludeCreate(સંરચના xkb_context *ctx, અક્ષર *str, ગણના merge_mode merge);

XkbFile *
XkbFileCreate(ગણના xkb_file_type type, અક્ષર *name, ParseCommon *defs,
              ગણના xkb_map_flags flags);

શૂન્ય
FreeStmt(ParseCommon *stmt);
