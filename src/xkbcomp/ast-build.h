/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "scanner-utils.h"

கோவை_Def *
கோவை_Create_String(xkb_atom_t str);

கோவை_Def *
கோவை_Create_Integer(int64_t ival);

கோவை_Def *
கோவை_Create_Float(வெறுமை);

கோவை_Def *
கோவை_Create_Boolean(bool set);

கோவை_Def *
கோவை_Create_சாவி_Name(xkb_atom_t சாவி_name);

கோவை_Def *
கோவை_Create_சாவி_Sym(xkb_keysym_t keysym);

கோவை_Def *
கோவை_Create_Ident(xkb_atom_t ident);

கோவை_Def *
கோவை_Create_Unary(எண்ணிக்கை stmt_type op, கோவை_Def *child);

கோவை_Def *
கோவை_Create_இருமம்(எண்ணிக்கை stmt_type op, கோவை_Def *இடது, கோவை_Def *வலது);

கோவை_Def *
கோவை_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

கோவை_Def *
கோவை_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, கோவை_Def *entry);

கோவை_Def *
கோவை_Empty_List(வெறுமை);

கோவை_Def *
கோவை_Create_Action(xkb_atom_t name, கோவை_Def *args);

கோவை_Def *
கோவை_Create_Action_List(கோவை_Def *actions);

கோவை_Def *
கோவை_Create_சாவி_Sym_List(xkb_keysym_t sym);

கோவை_Def *
கோவை_Append_சாவி_Sym_List(கோவை_Def *list, xkb_keysym_t sym);

கோவை_Def *
கோவை_சாவி_Sym_List_Append_String(கட்டமைப்பு scanner *param,
                           கோவை_Def *கோவை, மாறிலி எழுத்து *string);

xkb_keysym_t
KeysymParseString(கட்டமைப்பு scanner *scanner, மாறிலி எழுத்து *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t மதிப்பு);

சாவி_Alias_Def *
சாவி_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, கோவை_Def *மதிப்பு);

VarDef *
VarCreate(கோவை_Def *name, கோவை_Def *மதிப்பு);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, கோவை_Def *match);

சாவி_Type_Def *
சாவி_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t சாவி_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, கோவை_Def *def);

ModMapDef *
ModMapCreate(கோவை_Def *modifiers, கோவை_Def *சாவிகள்);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, கோவை_Def *name, bool மெய்நிகர்);

UnknownStatement *
UnknownStatementCreate(எண்ணிக்கை stmt_type, கட்டமைப்பு sval name);

IncludeStmt *
IncludeCreate(கட்டமைப்பு xkb_context *ctx, எழுத்து *str, எண்ணிக்கை merge_mode merge);

XkbFile *
XkbFileCreate(எண்ணிக்கை xkb_file_type type, எழுத்து *name, ParseCommon *defs,
              எண்ணிக்கை xkb_map_flags flags);

வெறுமை
FreeStmt(ParseCommon *stmt);
