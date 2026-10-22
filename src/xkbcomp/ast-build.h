/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "ast.h"
#అన్తర్భావ "scanner-utils.h"

సమాసం_Def *
సమాసం_Create_String(xkb_atom_t str);

సమాసం_Def *
సమాసం_Create_Integer(int64_t ival);

సమాసం_Def *
సమాసం_Create_Float(శూన్య);

సమాసం_Def *
సమాసం_Create_Boolean(bool సమితి);

సమాసం_Def *
సమాసం_Create_కీలకం_Name(xkb_atom_t కీలకం_name);

సమాసం_Def *
సమాసం_Create_కీలకం_Sym(xkb_keysym_t keysym);

సమాసం_Def *
సమాసం_Create_Ident(xkb_atom_t ident);

సమాసం_Def *
సమాసం_Create_Unary(గణన stmt_type op, సమాసం_Def *బిడ్డ);

సమాసం_Def *
సమాసం_Create_ద్విమాన(గణన stmt_type op, సమాసం_Def *ఎడమ, సమాసం_Def *కుడి);

సమాసం_Def *
సమాసం_Create_క్షేత్రం_Ref(xkb_atom_t element, xkb_atom_t క్షేత్రం_2);

సమాసం_Def *
సమాసం_Create_Array_Ref(xkb_atom_t element, xkb_atom_t క్షేత్రం_2, సమాసం_Def *నమోదు_2);

సమాసం_Def *
సమాసం_ఖాళీలు_List(శూన్య);

సమాసం_Def *
సమాసం_Create_Action(xkb_atom_t name, సమాసం_Def *వాదనలు);

సమాసం_Def *
సమాసం_Create_Action_List(సమాసం_Def *actions);

సమాసం_Def *
సమాసం_Create_కీలకం_Sym_List(xkb_keysym_t sym);

సమాసం_Def *
సమాసం_Append_కీలకం_Sym_List(సమాసం_Def *list, xkb_keysym_t sym);

సమాసం_Def *
సమాసం_కీలకం_Sym_List_Append_String(నిర్మాణం scanner *param,
                           సమాసం_Def *సమాసం, స్థిరరాశి అక్షరం *string);

xkb_keysym_t
KeysymParseString(నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *string);

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t విలువ);

కీలకం_Alias_Def *
కీలకం_Alias_Create(xkb_atom_t alias, xkb_atom_t real);

VModDef *
VModCreate(xkb_atom_t name, సమాసం_Def *విలువ);

VarDef *
VarCreate(సమాసం_Def *name, సమాసం_Def *విలువ);

VarDef *
BoolVarCreate(xkb_atom_t ident, bool సమితి);

InterpDef *
InterpCreate(xkb_keysym_t sym, సమాసం_Def *సరిపోలిక);

కీలకం_Type_Def *
కీలకం_Type_Create(xkb_atom_t name, VarDef *body);

SymbolsDef *
SymbolsCreate(xkb_atom_t కీలకం_Name, VarDef *symbols);

సమూహం_Compat_Def *
సమూహం_Compat_Create(int64_t సమూహం, సమాసం_Def *def);

ModMapDef *
ModMapCreate(సమాసం_Def *modifiers, సమాసం_Def *కీలకాలు);

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body);

LedNameDef *
LedNameCreate(int64_t ndx, సమాసం_Def *name, bool మాయా);

UnknownStatement *
UnknownStatementCreate(గణన stmt_type, నిర్మాణం sval name);

IncludeStmt *
IncludeCreate(నిర్మాణం xkb_context *ctx, అక్షరం *str, గణన విలీనం_బహుళకం విలీనం);

XkbFile *
XkbFileCreate(గణన xkb_file_type type, అక్షరం *name, ParseCommon *defs,
              గణన xkb_map_flags flags);

శూన్య
FreeStmt(ParseCommon *stmt);
