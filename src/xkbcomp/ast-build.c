/*
 * For HPND
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Ran Benita <ran234@gmail.com>
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "ast-build.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "utf8-decoding.h"

സ്ഥിതം വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create(എണ്ണൽ stmt_type op)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = malloc(വലുപ്പം(*വ്യഞ്ജകം));
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;

    വ്യഞ്ജകം->common.type = op;
    വ്യഞ്ജകം->common.next = ശൂന്യ;

    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_String(xkb_atom_t str)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_STRING_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->string.str = str;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Integer(int64_t ival)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_INTEGER_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->integer.ival = ival;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Float(ശൂന്യം)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_FLOAT_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Boolean(bool set)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->boolean.set = set;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Name(xkb_atom_t താക്കോൽ_name)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_KEYNAME_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->താക്കോൽ_name.താക്കോൽ_name = താക്കോൽ_name;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Sym(xkb_keysym_t keysym)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_KEYSYM_LITERAL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->keysym.keysym = keysym;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Ident(xkb_atom_t ident)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_IDENT);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->ident.ident = ident;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Unary(എണ്ണൽ stmt_type op, വ്യഞ്ജകം_Def *child)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(op);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->unary.child = child;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_ദ്വിമാന(എണ്ണൽ stmt_type op, വ്യഞ്ജകം_Def *ഇടത്, വ്യഞ്ജകം_Def *വലത്)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(op);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;

    വ്യഞ്ജകം->ദ്വിമാന.ഇടത് = ഇടത്;
    വ്യഞ്ജകം->ദ്വിമാന.വലത് = വലത്;

    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_FIELD_REF);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->field_ref.element = element;
    വ്യഞ്ജകം->field_ref.field = field;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, വ്യഞ്ജകം_Def *entry)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_ARRAY_REF);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->array_ref.element = element;
    വ്യഞ്ജകം->array_ref.field = field;
    വ്യഞ്ജകം->array_ref.entry = entry;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Empty_List(ശൂന്യം)
{
    തിരികെ വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_EMPTY_LIST);
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Action(xkb_atom_t name, വ്യഞ്ജകം_Def *args)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_ACTION_DECL);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->action.name = name;
    വ്യഞ്ജകം->action.args = args;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_Action_List(വ്യഞ്ജകം_Def *actions)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_ACTION_LIST);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    വ്യഞ്ജകം->actions.actions = actions;
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(xkb_keysym_t sym)
{
    വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create(STMT_വ്യഞ്ജകം_KEYSYM_LIST);
    എങ്കിൽ (!വ്യഞ്ജകം)
        തിരികെ ശൂന്യ;
    darray_init(വ്യഞ്ജകം->keysym_list.syms);
    എങ്കിൽ (sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
        /* Discard NoSymbol */
    } അല്ലെങ്കിൽ {
        darray_append(വ്യഞ്ജകം->keysym_list.syms, sym);
    }
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_Append_താക്കോൽ_Sym_List(വ്യഞ്ജകം_Def *വ്യഞ്ജകം, xkb_keysym_t sym)
{
    എങ്കിൽ (sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
        /* Discard NoSymbol */
    } അല്ലെങ്കിൽ {
        darray_append(വ്യഞ്ജകം->keysym_list.syms, sym);
    }
    തിരികെ വ്യഞ്ജകം;
}

വ്യഞ്ജകം_Def *
വ്യഞ്ജകം_താക്കോൽ_Sym_List_Append_String(ഘടന scanner *scanner,
                           വ്യഞ്ജകം_Def *വ്യഞ്ജകം, സ്ഥിരം അക്ഷരം *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    സ്ഥിരം size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    ആകുമ്പോൾ (idx < len) {
        size_t എണ്ണം = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &എണ്ണം);
        എങ്കിൽ (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            പോകുക error;
        }
        സ്ഥിരം xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        എങ്കിൽ (sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            പോകുക error;
        }
        darray_append(വ്യഞ്ജകം->keysym_list.syms, sym);
        idx += എണ്ണം;
        idx_cp++;
    }
    പ്രതിജ്ഞാ(string[idx] == '\0');
    തിരികെ വ്യഞ്ജകം;
error:
    FreeStmt((ParseCommon*) വ്യഞ്ജകം);
    തിരികെ ശൂന്യ;
}

xkb_keysym_t
KeysymParseString(ഘടന scanner *scanner, സ്ഥിരം അക്ഷരം *string)
{
    സ്ഥിരം size_t len = strlen(string);
    എങ്കിൽ (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    }
    size_t എണ്ണം = 0;
    സ്ഥിരം uint32_t cp = utf8_next_code_point(string, len, &എണ്ണം);
    എങ്കിൽ (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    } അല്ലെങ്കിൽ എങ്കിൽ (എണ്ണം != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    }
    സ്ഥിരം xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    എങ്കിൽ (sym == XKB_താക്കോൽ_No_ചിഹ്നം) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    തിരികെ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t മൂല്യം)
{
    KeycodeDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_KEYCODE;
    def->common.next = ശൂന്യ;
    def->name = name;
    def->മൂല്യം = മൂല്യം;

    തിരികെ def;
}

താക്കോൽ_Alias_Def *
താക്കോൽ_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    താക്കോൽ_Alias_Def *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_ALIAS;
    def->common.next = ശൂന്യ;
    def->alias = alias;
    def->real = real;

    തിരികെ def;
}

VModDef *
VModCreate(xkb_atom_t name, വ്യഞ്ജകം_Def *മൂല്യം)
{
    VModDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_VMOD;
    def->common.next = ശൂന്യ;
    def->name = name;
    def->മൂല്യം = മൂല്യം;

    തിരികെ def;
}

VarDef *
VarCreate(വ്യഞ്ജകം_Def *name, വ്യഞ്ജകം_Def *മൂല്യം)
{
    VarDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_VAR;
    def->common.next = ശൂന്യ;
    def->name = name;
    def->മൂല്യം = മൂല്യം;

    തിരികെ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    വ്യഞ്ജകം_Def *name, *മൂല്യം;
    VarDef *def;
    എങ്കിൽ (!(name = വ്യഞ്ജകം_Create_Ident(ident))) {
        തിരികെ ശൂന്യ;
    }
    എങ്കിൽ (!(മൂല്യം = വ്യഞ്ജകം_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        തിരികെ ശൂന്യ;
    }
    എങ്കിൽ (!(def = VarCreate(name, മൂല്യം))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) മൂല്യം);
        തിരികെ ശൂന്യ;
    }
    തിരികെ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, വ്യഞ്ജകം_Def *match)
{
    InterpDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_INTERP;
    def->common.next = ശൂന്യ;
    def->sym = sym;
    def->match = match;
    def->def = ശൂന്യ;

    തിരികെ def;
}

താക്കോൽ_Type_Def *
താക്കോൽ_Type_Create(xkb_atom_t name, VarDef *body)
{
    താക്കോൽ_Type_Def *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_TYPE;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    തിരികെ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t താക്കോൽ_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_SYMBOLS;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->താക്കോൽ_Name = താക്കോൽ_Name;
    def->symbols = symbols;

    തിരികെ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, വ്യഞ്ജകം_Def *val)
{
    GroupCompatDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    തിരികെ def;
}

ModMapDef *
ModMapCreate(വ്യഞ്ജകം_Def *modifiers, വ്യഞ്ജകം_Def *താക്കോലുകൾ)
{
    ModMapDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_MODMAP;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->താക്കോലുകൾ = താക്കോലുകൾ;

    തിരികെ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_LED_MAP;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    തിരികെ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, വ്യഞ്ജകം_Def *name, bool മായാ)
{
    LedNameDef *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = STMT_LED_NAME;
    def->common.next = ശൂന്യ;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->മായാ = മായാ;

    തിരികെ def;
}

UnknownStatement *
UnknownStatementCreate(എണ്ണൽ stmt_type type, ഘടന sval name)
{
    UnknownStatement *def = malloc(വലുപ്പം(*def));
    എങ്കിൽ (!def)
        തിരികെ ശൂന്യ;

    def->common.type = type;
    def->common.next = ശൂന്യ;
    def->name = strndup(name.start, name.len);
    എങ്കിൽ (!def->name) {
        സ്വതന്ത്രം(def);
        തിരികെ ശൂന്യ;
    }

    തിരികെ def;
}

സ്ഥിതം ശൂന്യം
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ഘടന xkb_context *ctx, അക്ഷരം *str, എണ്ണൽ merge_mode merge)
{
    IncludeStmt *incl, *first;
    അക്ഷരം *stmt, *tmp;
    അക്ഷരം nextop;

    incl = first = ശൂന്യ;
    tmp = str;
    stmt = strdup_safe(str);
    ആകുമ്പോൾ (tmp && *tmp)
    {
        അക്ഷരം *file = ശൂന്യ, *map = ശൂന്യ, *extra_data = ശൂന്യ;

        എങ്കിൽ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            പോകുക err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        എങ്കിൽ (isempty(file)) {
            സ്വതന്ത്രം(file);
            സ്വതന്ത്രം(map);
            സ്വതന്ത്രം(extra_data);
            തുടരുക;
        }

        എങ്കിൽ (first == ശൂന്യ) {
            first = incl = malloc(വലുപ്പം(*first));
        } അല്ലെങ്കിൽ {
            incl->next_incl = malloc(വലുപ്പം(*first));
            incl = incl->next_incl;
        }

        എങ്കിൽ (!incl) {
            സ്വതന്ത്രം(file);
            സ്വതന്ത്രം(map);
            സ്വതന്ത്രം(extra_data);
            നിർത്തുക;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = ശൂന്യ;
        incl->merge = merge;
        incl->stmt = ശൂന്യ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = ശൂന്യ;

        തിരഞ്ഞെടുപ്പ് (nextop) {
        അവസ്ഥ MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            നിർത്തുക;
        അവസ്ഥ MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            merge = MERGE_OVERRIDE;
        }
    }

    എങ്കിൽ (first)
        first->stmt = stmt;
    അല്ലെങ്കിൽ
        സ്വതന്ത്രം(stmt);

    തിരികെ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    സ്വതന്ത്രം(stmt);
    തിരികെ ശൂന്യ;
}

XkbFile *
XkbFileCreate(എണ്ണൽ xkb_file_type type, അക്ഷരം *name, ParseCommon *defs,
              എണ്ണൽ xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, വലുപ്പം(*file));
    എങ്കിൽ (!file)
        തിരികെ ശൂന്യ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    തിരികെ file;
}

XkbFile *
XkbFileFromComponents(ഘടന xkb_context *ctx,
                      സ്ഥിരം ഘടന xkb_component_names *kkctgs)
{
    അക്ഷരം *സ്ഥിരം components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    എണ്ണൽ xkb_file_type type;
    IncludeStmt *include = ശൂന്യ;
    XkbFile *file = ശൂന്യ;
    ParseCommon *defs = ശൂന്യ, *defsLast = ശൂന്യ;

    വേണ്ടി (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        എങ്കിൽ (!include)
            പോകുക err;

        file = XkbFileCreate(type, ശൂന്യ, (ParseCommon *) include, 0);
        എങ്കിൽ (!file) {
            FreeInclude(include);
            പോകുക err;
        }

        എങ്കിൽ (!defs)
            defsLast = defs = &file->common;
        അല്ലെങ്കിൽ
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ശൂന്യ, defs, 0);
    എങ്കിൽ (!file)
        പോകുക err;

    തിരികെ file;

err:
    FreeXkbFile((XkbFile *) defs);
    തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    ആകുമ്പോൾ (incl)
    {
        next = incl->next_incl;

        സ്വതന്ത്രം(incl->file);
        സ്വതന്ത്രം(incl->map);
        സ്വതന്ത്രം(incl->modifier);
        സ്വതന്ത്രം(incl->stmt);

        സ്വതന്ത്രം(incl);
        incl = next;
    }
}

ശൂന്യം
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    ആകുമ്പോൾ (stmt)
    {
        next = stmt->next;

        തിരഞ്ഞെടുപ്പ് (stmt->type) {
        അവസ്ഥ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ശൂന്യ;
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
        അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
        അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
        അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_Unary *) stmt)->child);
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
        അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
        അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
        അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
        അവസ്ഥ STMT_വ്യഞ്ജകം_ASSIGN:
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_ദ്വിമാന *) stmt)->ഇടത്);
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_ദ്വിമാന *) stmt)->വലത്);
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_DECL:
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_Action *) stmt)->args);
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_LIST:
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_Action_List *) stmt)->actions);
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_ARRAY_REF:
            FreeStmt((ParseCommon *) ((വ്യഞ്ജകം_Array_Ref *) stmt)->entry);
            നിർത്തുക;

        അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LIST:
            darray_free(((വ്യഞ്ജകം_Keysym_List *) stmt)->syms);
            നിർത്തുക;

        അവസ്ഥ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->മൂല്യം);
            നിർത്തുക;
        അവസ്ഥ STMT_TYPE:
            FreeStmt((ParseCommon *) ((താക്കോൽ_Type_Def *) stmt)->body);
            നിർത്തുക;
        അവസ്ഥ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            നിർത്തുക;
        അവസ്ഥ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->മൂല്യം);
            നിർത്തുക;
        അവസ്ഥ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            നിർത്തുക;
        അവസ്ഥ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->താക്കോലുകൾ);
            നിർത്തുക;
        അവസ്ഥ STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            നിർത്തുക;
        അവസ്ഥ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            നിർത്തുക;
        അവസ്ഥ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            നിർത്തുക;
        അവസ്ഥ STMT_UNKNOWN_DECLARATION:
        അവസ്ഥ STMT_UNKNOWN_COMPOUND:
            സ്വതന്ത്രം(((UnknownStatement *) stmt)->name);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_അക്കം_മൂല്യങ്ങൾ == 37 &&
                          STMT_അക്കം_മൂല്യങ്ങൾ == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            നിർത്തുക;
        }

        സ്വതന്ത്രം(stmt);
        stmt = next;
    }
}

ശൂന്യം
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    ആകുമ്പോൾ (file)
    {
        next = (XkbFile *) file->common.next;

        തിരഞ്ഞെടുപ്പ് (file->file_type) {
        അവസ്ഥ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            നിർത്തുക;

        അവസ്ഥ FILE_TYPE_TYPES:
        അവസ്ഥ FILE_TYPE_COMPAT:
        അവസ്ഥ FILE_TYPE_SYMBOLS:
        അവസ്ഥ FILE_TYPE_KEYCODES:
        അവസ്ഥ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
        }

        സ്വതന്ത്രം(file->name);
        സ്വതന്ത്രം(file);
        file = next;
    }
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *xkb_file_type_strings[FILE_TYPE_അക്കം_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

സ്ഥിരം അക്ഷരം *
xkb_file_type_to_string(എണ്ണൽ xkb_file_type type)
{
    എങ്കിൽ (type >= FILE_TYPE_അക്കം_ENTRIES)
        തിരികെ "unknown";
    തിരികെ xkb_file_type_strings[type];
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *stmt_type_strings[STMT_അക്കം_മൂല്യങ്ങൾ] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_വ്യഞ്ജകം_STRING_LITERAL] = "string literal expression",
    [STMT_വ്യഞ്ജകം_INTEGER_LITERAL] = "integer literal expression",
    [STMT_വ്യഞ്ജകം_FLOAT_LITERAL] = "float literal expression",
    [STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_വ്യഞ്ജകം_KEYNAME_LITERAL] = "key name expression",
    [STMT_വ്യഞ്ജകം_KEYSYM_LITERAL] = "keysym expression",
    [STMT_വ്യഞ്ജകം_IDENT] = "identifier expression",
    [STMT_വ്യഞ്ജകം_ACTION_DECL] = "action declaration expression",
    [STMT_വ്യഞ്ജകം_FIELD_REF] = "field reference expression",
    [STMT_വ്യഞ്ജകം_ARRAY_REF] = "array reference expression",
    [STMT_വ്യഞ്ജകം_EMPTY_LIST] = "empty list expression",
    [STMT_വ്യഞ്ജകം_KEYSYM_LIST] = "keysym list expression",
    [STMT_വ്യഞ്ജകം_ACTION_LIST] = "action list expression",
    [STMT_വ്യഞ്ജകം_ADD] = "addition expression",
    [STMT_വ്യഞ്ജകം_SUBTRACT] = "substraction expression",
    [STMT_വ്യഞ്ജകം_MULTIPLY] = "multiplication expression",
    [STMT_വ്യഞ്ജകം_DIVIDE] = "division expression",
    [STMT_വ്യഞ്ജകം_ASSIGN] = "assignment expression",
    [STMT_വ്യഞ്ജകം_NOT] = "logical negation expression",
    [STMT_വ്യഞ്ജകം_NEGATE] = "arithmetic negation expression",
    [STMT_വ്യഞ്ജകം_INVERT] = "bitwise inversion expression",
    [STMT_വ്യഞ്ജകം_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_GROUP_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_അക്കം_മൂല്യങ്ങൾ == 37 &&
              STMT_അക്കം_മൂല്യങ്ങൾ == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

സ്ഥിരം അക്ഷരം *
stmt_type_to_string(എണ്ണൽ stmt_type type)
{
    എങ്കിൽ (type >= STMT_അക്കം_മൂല്യങ്ങൾ)
        തിരികെ ശൂന്യ;
    തിരികെ stmt_type_strings[type];
}

അക്ഷരം
stmt_type_to_operator_char(എണ്ണൽ stmt_type type)
{
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
        തിരികെ '+';
    അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
        തിരികെ '-';
    അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
        തിരികെ '*';
    അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
        തിരികെ '/';
    അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
        തിരികെ '!';
    അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
        തിരികെ '-';
    അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
        തിരികെ '~';
    അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
        തിരികെ '+';
    സ്ഥിരസ്ഥിതി:
        തിരികെ '\0';
    }
}
