/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

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
ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(bool ಗಣ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name(xkb_atom_t ಕೀಲಿ_name);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym(xkb_keysym_t keysym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(xkb_atom_t ident);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(ಎಣಿಕೆ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮಗು);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(ಎಣಿಕೆ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, ಅಭಿವ್ಯಕ್ತಿ_Def *ಬಲ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕ್ಷೇತ್ರ_Ref(xkb_atom_t element, xkb_atom_t ಕ್ಷೇತ್ರ_2);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ಕ್ಷೇತ್ರ_2, ಅಭಿವ್ಯಕ್ತಿ_Def *ನಮೂದು);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_List(ಶೂನ್ಯ);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ವಾದಗಳು);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಅಭಿವ್ಯಕ್ತಿ_Def *actions);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(xkb_keysym_t sym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List(ಅಭಿವ್ಯಕ್ತಿ_Def *list, xkb_keysym_t sym);

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(ರಚನೆ scanner *param,
                           ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string);

xkb_keysym_t
KeysymParseString(ರಚನೆ scanner *scanner, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ಮೌಲ್ಯ);

ಕೀಲಿ_Alias_Def *
ಕೀಲಿ_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ);

VarDef *
VarCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool ಗಣ);

InterpDef *
InterpCreate(xkb_keysym_t sym, ಅಭಿವ್ಯಕ್ತಿ_Def *ಹೊಂದಿಕೆ);

ಕೀಲಿ_Type_Def *
ಕೀಲಿ_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t ಕೀಲಿ_Name, VarDef *symbols);

ಗುಂಪು_Compat_Def *
ಗುಂಪು_Compat_Create(int64_t ಗುಂಪು, ಅಭಿವ್ಯಕ್ತಿ_Def *def);

ModMapDef *
ModMapCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *modifiers, ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿಗಳು);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, ಅಭಿವ್ಯಕ್ತಿ_Def *name, bool ವಾಸ್ತವಿಕ);

UnknownStatement *
UnknownStatementCreate(ಎಣಿಕೆ stmt_type, ರಚನೆ sval name);

IncludeStmt *
IncludeCreate(ರಚನೆ xkb_context *ctx, ಅಕ್ಷರ *str, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ);

XkbFile *
XkbFileCreate(ಎಣಿಕೆ xkb_file_type type, ಅಕ್ಷರ *name, ParseCommon *defs,
              ಎಣಿಕೆ xkb_map_flags flags);

ಶೂನ್ಯ
FreeStmt(ParseCommon *stmt);
