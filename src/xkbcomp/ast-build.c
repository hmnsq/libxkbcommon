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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "ast-build.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "utf8-decoding.h"

ସ୍ଥିର ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create(ଗଣନା stmt_type op)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = malloc(ଆକାର(*ପଦସମଷ୍ଟି));
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;

    ପଦସମଷ୍ଟି->common.type = op;
    ପଦସମଷ୍ଟି->common.next = ଖାଲି;

    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_String(xkb_atom_t str)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_STRING_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->string.str = str;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Integer(int64_t ival)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->integer.ival = ival;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Float(ଶୂନ୍ୟ)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Boolean(bool set)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->boolean.set = set;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Name(xkb_atom_t ଚାବି_name)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->ଚାବି_name.ଚାବି_name = ଚାବି_name;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Sym(xkb_keysym_t keysym)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->keysym.keysym = keysym;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Ident(xkb_atom_t ident)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_IDENT);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->ident.ident = ident;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Unary(ଗଣନା stmt_type op, ପଦସମଷ୍ଟି_Def *child)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(op);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->unary.child = child;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(ଗଣନା stmt_type op, ପଦସମଷ୍ଟି_Def *ବାମ, ପଦସମଷ୍ଟି_Def *ଡାହାଣ)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(op);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;

    ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ବାମ = ବାମ;
    ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ଡାହାଣ = ଡାହାଣ;

    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_FIELD_REF);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->field_ref.element = element;
    ପଦସମଷ୍ଟି->field_ref.field = field;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ପଦସମଷ୍ଟି_Def *entry)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_ARRAY_REF);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->array_ref.element = element;
    ପଦସମଷ୍ଟି->array_ref.field = field;
    ପଦସମଷ୍ଟି->array_ref.entry = entry;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Empty_List(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_EMPTY_LIST);
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Action(xkb_atom_t name, ପଦସମଷ୍ଟି_Def *args)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_ACTION_DECL);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->action.name = name;
    ପଦସମଷ୍ଟି->action.args = args;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_Action_List(ପଦସମଷ୍ଟି_Def *actions)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_ACTION_LIST);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    ପଦସମଷ୍ଟି->actions.actions = actions;
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(xkb_keysym_t sym)
{
    ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create(STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST);
    ଯଦି (!ପଦସମଷ୍ଟି)
        ଫେରନ୍ତୁ ଖାଲି;
    darray_init(ପଦସମଷ୍ଟି->keysym_list.syms);
    ଯଦି (sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
        /* Discard NoSymbol */
    } ଅନ୍ୟଥା {
        darray_append(ପଦସମଷ୍ଟି->keysym_list.syms, sym);
    }
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_Append_ଚାବି_Sym_List(ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, xkb_keysym_t sym)
{
    ଯଦି (sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
        /* Discard NoSymbol */
    } ଅନ୍ୟଥା {
        darray_append(ପଦସମଷ୍ଟି->keysym_list.syms, sym);
    }
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
}

ପଦସମଷ୍ଟି_Def *
ପଦସମଷ୍ଟି_ଚାବି_Sym_List_Append_String(ସଂରଚନା scanner *scanner,
                           ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, ଅଚଳ ଅକ୍ଷର *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ଅଚଳ size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    ଯେତେବେଳେ (idx < len) {
        size_t ଗଣନା_2 = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &ଗଣନା_2);
        ଯଦି (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            ଯାଅ error;
        }
        ଅଚଳ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        ଯଦି (sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            ଯାଅ error;
        }
        darray_append(ପଦସମଷ୍ଟି->keysym_list.syms, sym);
        idx += ଗଣନା_2;
        idx_cp++;
    }
    ପ୍ରତିଜ୍ଞା(string[idx] == '\0');
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି;
error:
    FreeStmt((ParseCommon*) ପଦସମଷ୍ଟି);
    ଫେରନ୍ତୁ ଖାଲି;
}

xkb_keysym_t
KeysymParseString(ସଂରଚନା scanner *scanner, ଅଚଳ ଅକ୍ଷର *string)
{
    ଅଚଳ size_t len = strlen(string);
    ଯଦି (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    }
    size_t ଗଣନା_2 = 0;
    ଅଚଳ uint32_t cp = utf8_next_code_point(string, len, &ଗଣନା_2);
    ଯଦି (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    } ଅନ୍ୟଥା ଯଦି (ଗଣନା_2 != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    }
    ଅଚଳ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    ଯଦି (sym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ଫେରନ୍ତୁ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ମୂଲ୍ୟ)
{
    KeycodeDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_KEYCODE;
    def->common.next = ଖାଲି;
    def->name = name;
    def->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ def;
}

ଚାବି_Alias_Def *
ଚାବି_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ଚାବି_Alias_Def *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_ALIAS;
    def->common.next = ଖାଲି;
    def->alias = alias;
    def->real = real;

    ଫେରନ୍ତୁ def;
}

VModDef *
VModCreate(xkb_atom_t name, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    VModDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_VMOD;
    def->common.next = ଖାଲି;
    def->name = name;
    def->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ def;
}

VarDef *
VarCreate(ପଦସମଷ୍ଟି_Def *name, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    VarDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_VAR;
    def->common.next = ଖାଲି;
    def->name = name;
    def->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    ପଦସମଷ୍ଟି_Def *name, *ମୂଲ୍ୟ;
    VarDef *def;
    ଯଦି (!(name = ପଦସମଷ୍ଟି_Create_Ident(ident))) {
        ଫେରନ୍ତୁ ଖାଲି;
    }
    ଯଦି (!(ମୂଲ୍ୟ = ପଦସମଷ୍ଟି_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        ଫେରନ୍ତୁ ଖାଲି;
    }
    ଯଦି (!(def = VarCreate(name, ମୂଲ୍ୟ))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) ମୂଲ୍ୟ);
        ଫେରନ୍ତୁ ଖାଲି;
    }
    ଫେରନ୍ତୁ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ପଦସମଷ୍ଟି_Def *match)
{
    InterpDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_INTERP;
    def->common.next = ଖାଲି;
    def->sym = sym;
    def->match = match;
    def->def = ଖାଲି;

    ଫେରନ୍ତୁ def;
}

ଚାବି_Type_Def *
ଚାବି_Type_Create(xkb_atom_t name, VarDef *body)
{
    ଚାବି_Type_Def *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_TYPE;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ଫେରନ୍ତୁ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ଚାବି_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_SYMBOLS;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->ଚାବି_Name = ଚାବି_Name;
    def->symbols = symbols;

    ଫେରନ୍ତୁ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, ପଦସମଷ୍ଟି_Def *val)
{
    GroupCompatDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ଫେରନ୍ତୁ def;
}

ModMapDef *
ModMapCreate(ପଦସମଷ୍ଟି_Def *modifiers, ପଦସମଷ୍ଟି_Def *ଚାବିଗୁଡ଼ିକ)
{
    ModMapDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_MODMAP;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->ଚାବିଗୁଡ଼ିକ = ଚାବିଗୁଡ଼ିକ;

    ଫେରନ୍ତୁ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_LED_MAP;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ଫେରନ୍ତୁ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ପଦସମଷ୍ଟି_Def *name, bool ଆଭାସୀ)
{
    LedNameDef *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = STMT_LED_NAME;
    def->common.next = ଖାଲି;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->ଆଭାସୀ = ଆଭାସୀ;

    ଫେରନ୍ତୁ def;
}

UnknownStatement *
UnknownStatementCreate(ଗଣନା stmt_type type, ସଂରଚନା sval name)
{
    UnknownStatement *def = malloc(ଆକାର(*def));
    ଯଦି (!def)
        ଫେରନ୍ତୁ ଖାଲି;

    def->common.type = type;
    def->common.next = ଖାଲି;
    def->name = strndup(name.start, name.len);
    ଯଦି (!def->name) {
        ମୁକ୍ତ(def);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ def;
}

ସ୍ଥିର ଶୂନ୍ୟ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ସଂରଚନା xkb_context *ctx, ଅକ୍ଷର *str, ଗଣନା merge_mode merge)
{
    IncludeStmt *incl, *first;
    ଅକ୍ଷର *stmt, *tmp;
    ଅକ୍ଷର nextop;

    incl = first = ଖାଲି;
    tmp = str;
    stmt = strdup_safe(str);
    ଯେତେବେଳେ (tmp && *tmp)
    {
        ଅକ୍ଷର *file = ଖାଲି, *map = ଖାଲି, *extra_data = ଖାଲି;

        ଯଦି (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            ଯାଅ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ଯଦି (isempty(file)) {
            ମୁକ୍ତ(file);
            ମୁକ୍ତ(map);
            ମୁକ୍ତ(extra_data);
            ଜାରି;
        }

        ଯଦି (first == ଖାଲି) {
            first = incl = malloc(ଆକାର(*first));
        } ଅନ୍ୟଥା {
            incl->next_incl = malloc(ଆକାର(*first));
            incl = incl->next_incl;
        }

        ଯଦି (!incl) {
            ମୁକ୍ତ(file);
            ମୁକ୍ତ(map);
            ମୁକ୍ତ(extra_data);
            ଭାଙ୍ଗ;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = ଖାଲି;
        incl->merge = merge;
        incl->stmt = ଖାଲି;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = ଖାଲି;

        ବାଛ (nextop) {
        ସ୍ଥିତି MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            merge = MERGE_OVERRIDE;
        }
    }

    ଯଦି (first)
        first->stmt = stmt;
    ଅନ୍ୟଥା
        ମୁକ୍ତ(stmt);

    ଫେରନ୍ତୁ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    ମୁକ୍ତ(stmt);
    ଫେରନ୍ତୁ ଖାଲି;
}

XkbFile *
XkbFileCreate(ଗଣନା xkb_file_type type, ଅକ୍ଷର *name, ParseCommon *defs,
              ଗଣନା xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, ଆକାର(*file));
    ଯଦି (!file)
        ଫେରନ୍ତୁ ଖାଲି;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ଫେରନ୍ତୁ file;
}

XkbFile *
XkbFileFromComponents(ସଂରଚନା xkb_context *ctx,
                      ଅଚଳ ସଂରଚନା xkb_component_names *kkctgs)
{
    ଅକ୍ଷର *ଅଚଳ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ଗଣନା xkb_file_type type;
    IncludeStmt *include = ଖାଲି;
    XkbFile *file = ଖାଲି;
    ParseCommon *defs = ଖାଲି, *defsLast = ଖାଲି;

    ପାଇଁ (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        ଯଦି (!include)
            ଯାଅ err;

        file = XkbFileCreate(type, ଖାଲି, (ParseCommon *) include, 0);
        ଯଦି (!file) {
            FreeInclude(include);
            ଯାଅ err;
        }

        ଯଦି (!defs)
            defsLast = defs = &file->common;
        ଅନ୍ୟଥା
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ଖାଲି, defs, 0);
    ଯଦି (!file)
        ଯାଅ err;

    ଫେରନ୍ତୁ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    ଯେତେବେଳେ (incl)
    {
        next = incl->next_incl;

        ମୁକ୍ତ(incl->file);
        ମୁକ୍ତ(incl->map);
        ମୁକ୍ତ(incl->modifier);
        ମୁକ୍ତ(incl->stmt);

        ମୁକ୍ତ(incl);
        incl = next;
    }
}

ଶୂନ୍ୟ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    ଯେତେବେଳେ (stmt)
    {
        next = stmt->next;

        ବାଛ (stmt->type) {
        ସ୍ଥିତି STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ଖାଲି;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_Unary *) stmt)->child);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ASSIGN:
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_ଦ୍ୱିମିକ *) stmt)->ବାମ);
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_ଦ୍ୱିମିକ *) stmt)->ଡାହାଣ);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_Action *) stmt)->args);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_Action_List *) stmt)->actions);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ପଦସମଷ୍ଟି_Array_Ref *) stmt)->entry);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST:
            darray_free(((ପଦସମଷ୍ଟି_Keysym_List *) stmt)->syms);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->ମୂଲ୍ୟ);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_TYPE:
            FreeStmt((ParseCommon *) ((ଚାବି_Type_Def *) stmt)->body);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->ମୂଲ୍ୟ);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ଚାବିଗୁଡ଼ିକ);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_UNKNOWN_DECLARATION:
        ସ୍ଥିତି STMT_UNKNOWN_COMPOUND:
            ମୁକ୍ତ(((UnknownStatement *) stmt)->name);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ == 37 &&
                          STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ଭାଙ୍ଗ;
        }

        ମୁକ୍ତ(stmt);
        stmt = next;
    }
}

ଶୂନ୍ୟ
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    ଯେତେବେଳେ (file)
    {
        next = (XkbFile *) file->common.next;

        ବାଛ (file->file_type) {
        ସ୍ଥିତି FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ଭାଙ୍ଗ;

        ସ୍ଥିତି FILE_TYPE_TYPES:
        ସ୍ଥିତି FILE_TYPE_COMPAT:
        ସ୍ଥିତି FILE_TYPE_SYMBOLS:
        ସ୍ଥିତି FILE_TYPE_KEYCODES:
        ସ୍ଥିତି FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
        }

        ମୁକ୍ତ(file->name);
        ମୁକ୍ତ(file);
        file = next;
    }
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *xkb_file_type_strings[FILE_TYPE_ଅଙ୍କ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ଅଚଳ ଅକ୍ଷର *
xkb_file_type_to_string(ଗଣନା xkb_file_type type)
{
    ଯଦି (type >= FILE_TYPE_ଅଙ୍କ_ENTRIES)
        ଫେରନ୍ତୁ "unknown";
    ଫେରନ୍ତୁ xkb_file_type_strings[type];
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *stmt_type_strings[STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_ପଦସମଷ୍ଟି_STRING_LITERAL] = "string literal expression",
    [STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL] = "integer literal expression",
    [STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL] = "float literal expression",
    [STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL] = "key name expression",
    [STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL] = "keysym expression",
    [STMT_ପଦସମଷ୍ଟି_IDENT] = "identifier expression",
    [STMT_ପଦସମଷ୍ଟି_ACTION_DECL] = "action declaration expression",
    [STMT_ପଦସମଷ୍ଟି_FIELD_REF] = "field reference expression",
    [STMT_ପଦସମଷ୍ଟି_ARRAY_REF] = "array reference expression",
    [STMT_ପଦସମଷ୍ଟି_EMPTY_LIST] = "empty list expression",
    [STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST] = "keysym list expression",
    [STMT_ପଦସମଷ୍ଟି_ACTION_LIST] = "action list expression",
    [STMT_ପଦସମଷ୍ଟି_ADD] = "addition expression",
    [STMT_ପଦସମଷ୍ଟି_SUBTRACT] = "substraction expression",
    [STMT_ପଦସମଷ୍ଟି_MULTIPLY] = "multiplication expression",
    [STMT_ପଦସମଷ୍ଟି_DIVIDE] = "division expression",
    [STMT_ପଦସମଷ୍ଟି_ASSIGN] = "assignment expression",
    [STMT_ପଦସମଷ୍ଟି_NOT] = "logical negation expression",
    [STMT_ପଦସମଷ୍ଟି_NEGATE] = "arithmetic negation expression",
    [STMT_ପଦସମଷ୍ଟି_INVERT] = "bitwise inversion expression",
    [STMT_ପଦସମଷ୍ଟି_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ == 37 &&
              STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ଅଚଳ ଅକ୍ଷର *
stmt_type_to_string(ଗଣନା stmt_type type)
{
    ଯଦି (type >= STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ)
        ଫେରନ୍ତୁ ଖାଲି;
    ଫେରନ୍ତୁ stmt_type_strings[type];
}

ଅକ୍ଷର
stmt_type_to_operator_char(ଗଣନା stmt_type type)
{
    ବାଛ (type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
        ଫେରନ୍ତୁ '+';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
        ଫେରନ୍ତୁ '-';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
        ଫେରନ୍ତୁ '*';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
        ଫେରନ୍ତୁ '/';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
        ଫେରନ୍ତୁ '!';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
        ଫେରନ୍ତୁ '-';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
        ଫେରନ୍ତୁ '~';
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
        ଫେରନ୍ତୁ '+';
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ '\0';
    }
}
