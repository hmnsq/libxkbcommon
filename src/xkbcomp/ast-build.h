/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_String(xkb_atom_t str);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Integer(int64_t ival);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Float(ಶೂನ್ಯ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(bool set);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name(xkb_atom_t ಕೀಲಿ_name);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym(xkb_keysym_t keysym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(xkb_atom_t ident);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(ಗಣನಾ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *child);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(ಗಣನಾ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, ಅಭಿವ್ಯಕ್ತಿ_Def *ಬಲ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ಅಭಿವ್ಯಕ್ತಿ_Def *entry);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Empty_List(ಶೂನ್ಯ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *args);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಅಭಿವ್ಯಕ್ತಿ_Def *actions);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(xkb_keysym_t sym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List(ಅಭಿವ್ಯಕ್ತಿ_Def *list, xkb_keysym_t sym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(ಸಂರಚನಾ scanner *param,
                           ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಧ್ರುವ ವರ್ಣ *string);

xkb_keysym_t
KeysymParseString(ಸಂರಚನಾ scanner *scanner, ಧ್ರುವ ವರ್ಣ *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ಮೌಲ್ಯ);

ಕೀಲಿ_Alias_Def *
ಕೀಲಿ_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ);

VarDef *
VarCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set);

InterpDef *
InterpCreate(xkb_keysym_t sym, ಅಭಿವ್ಯಕ್ತಿ_Def *match);

ಕೀಲಿ_Type_Def *
ಕೀಲಿ_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ಕೀಲಿ_Name, VarDef *symbols);

GroupCompatDef *
GroupCompatCreate(int64_t group, ಅಭಿವ್ಯಕ್ತಿ_Def *def);

ModMapDef *
ModMapCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *modifiers, ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿಗಳು);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ಅಭಿವ್ಯಕ್ತಿ_Def *name, bool ಆಭಾಸ);

UnknownStatement *
UnknownStatementCreate(ಗಣನಾ stmt_type, ಸಂರಚನಾ sval name);

IncludeStmt *
IncludeCreate(ಸಂರಚನಾ xkb_context *ctx, ವರ್ಣ *str, ಗಣನಾ merge_mode merge);

XkbFile *
XkbFileCreate(ಗಣನಾ xkb_file_type type, ವರ್ಣ *name, ParseCommon *defs,
              ಗಣನಾ xkb_map_flags flags);

ಶೂನ್ಯ
FreeStmt(ParseCommon *stmt);
