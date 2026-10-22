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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast-build.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8-decoding.h"

ਸਥਿਰ ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create(ਗਿਣਤੀ stmt_type op)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*ਵਿਅੰਜਕ));
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਿਅੰਜਕ->common.type = op;
    ਵਿਅੰਜਕ->common.ਅਗਲਾ = ਨਿਰਾ;

    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_String(xkb_atom_t str)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_STRING_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->string.str = str;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Integer(int64_t ival)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->integer.ival = ival;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Float(ਖਾਲੀ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Boolean(bool ਸੈੱਟ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->boolean.ਸੈੱਟ = ਸੈੱਟ;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name(xkb_atom_t ਕੁੰਜੀ_name)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name = ਕੁੰਜੀ_name;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym(xkb_keysym_t keysym)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->keysym.keysym = keysym;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Ident(xkb_atom_t ident)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_IDENT);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->ident.ident = ident;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Unary(ਗਿਣਤੀ stmt_type op, ਵਿਅੰਜਕ_Def *ਬੱਚਾ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(op);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->unary.ਬੱਚਾ = ਬੱਚਾ;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(ਗਿਣਤੀ stmt_type op, ਵਿਅੰਜਕ_Def *ਖੱਬਾ, ਵਿਅੰਜਕ_Def *ਸੱਜਾ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(op);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਖੱਬਾ = ਖੱਬਾ;
    ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਸੱਜਾ = ਸੱਜਾ;

    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਖੇਤਰ_Ref(xkb_atom_t element, xkb_atom_t ਖੇਤਰ_2)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element = element;
    ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2 = ਖੇਤਰ_2;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ਖੇਤਰ_2, ਵਿਅੰਜਕ_Def *ਇੰਦਰਾਜ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_ARRAY_REF);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->array_ref.element = element;
    ਵਿਅੰਜਕ->array_ref.ਖੇਤਰ_2 = ਖੇਤਰ_2;
    ਵਿਅੰਜਕ->array_ref.ਇੰਦਰਾਜ = ਇੰਦਰਾਜ;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_List(ਖਾਲੀ)
{
    ਵਾਪਸ ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST);
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Action(xkb_atom_t name, ਵਿਅੰਜਕ_Def *ਦਲੀਲਾਂ)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_ACTION_DECL);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->action.name = name;
    ਵਿਅੰਜਕ->action.ਦਲੀਲਾਂ = ਦਲੀਲਾਂ;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_Action_List(ਵਿਅੰਜਕ_Def *actions)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_ACTION_LIST);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਿਅੰਜਕ->actions.actions = actions;
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(xkb_keysym_t sym)
{
    ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create(STMT_ਵਿਅੰਜਕ_KEYSYM_LIST);
    ਜੇ (!ਵਿਅੰਜਕ)
        ਵਾਪਸ ਨਿਰਾ;
    darray_init(ਵਿਅੰਜਕ->keysym_list.syms);
    ਜੇ (sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        /* Discard NoSymbol */
    } ਨਹੀਂਤਾਂ {
        darray_append(ਵਿਅੰਜਕ->keysym_list.syms, sym);
    }
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_Append_ਕੁੰਜੀ_Sym_List(ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, xkb_keysym_t sym)
{
    ਜੇ (sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        /* Discard NoSymbol */
    } ਨਹੀਂਤਾਂ {
        darray_append(ਵਿਅੰਜਕ->keysym_list.syms, sym);
    }
    ਵਾਪਸ ਵਿਅੰਜਕ;
}

ਵਿਅੰਜਕ_Def *
ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(ਬਣਤਰ scanner *scanner,
                           ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, ਅਚਲ ਅੱਖਰ *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ਅਚਲ ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(string);
    ਮਾਪ_t idx = 0;
    ਮਾਪ_t idx_cp = 1;
    ਜਦੋਂ (idx < len) {
        ਮਾਪ_t ਗਿਣਤੀ_2 = 0;
        uint32_t cp = utf8_ਅਗਲਾ_ਕੋਡ_ਬਿੰਦੂ(string + idx, len - idx, &ਗਿਣਤੀ_2);
        ਜੇ (cp == INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            ਜਾਓ error;
        }
        ਅਚਲ xkb_keysym_t sym = xkb_utf32_ਮੰਜ਼ਿਲ_keysym(cp);
        ਜੇ (sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
            scanner_err(scanner, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            ਜਾਓ error;
        }
        darray_append(ਵਿਅੰਜਕ->keysym_list.syms, sym);
        idx += ਗਿਣਤੀ_2;
        idx_cp++;
    }
    ਪ੍ਰਤਿਜ੍ਞਾ(string[idx] == '\0');
    ਵਾਪਸ ਵਿਅੰਜਕ;
error:
    FreeStmt((ParseCommon*) ਵਿਅੰਜਕ);
    ਵਾਪਸ ਨਿਰਾ;
}

xkb_keysym_t
KeysymParseString(ਬਣਤਰ scanner *scanner, ਅਚਲ ਅੱਖਰ *string)
{
    ਅਚਲ ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(string);
    ਜੇ (len == 0) {
        scanner_err(scanner, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    }
    ਮਾਪ_t ਗਿਣਤੀ_2 = 0;
    ਅਚਲ uint32_t cp = utf8_ਅਗਲਾ_ਕੋਡ_ਬਿੰਦੂ(string, len, &ਗਿਣਤੀ_2);
    ਜੇ (cp == INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    } ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    }
    ਅਚਲ xkb_keysym_t sym = xkb_utf32_ਮੰਜ਼ਿਲ_keysym(cp);
    ਜੇ (sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        scanner_err(scanner, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ਵਾਪਸ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ਮੁੱਲ)
{
    KeycodeDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_KEYCODE;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->name = name;
    def->ਮੁੱਲ = ਮੁੱਲ;

    ਵਾਪਸ def;
}

ਕੁੰਜੀ_Alias_Def *
ਕੁੰਜੀ_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ਕੁੰਜੀ_Alias_Def *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_ALIAS;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->alias = alias;
    def->real = real;

    ਵਾਪਸ def;
}

VModDef *
VModCreate(xkb_atom_t name, ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    VModDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_VMOD;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->name = name;
    def->ਮੁੱਲ = ਮੁੱਲ;

    ਵਾਪਸ def;
}

VarDef *
VarCreate(ਵਿਅੰਜਕ_Def *name, ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    VarDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_VAR;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->name = name;
    def->ਮੁੱਲ = ਮੁੱਲ;

    ਵਾਪਸ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool ਸੈੱਟ)
{
    ਵਿਅੰਜਕ_Def *name, *ਮੁੱਲ;
    VarDef *def;
    ਜੇ (!(name = ਵਿਅੰਜਕ_Create_Ident(ident))) {
        ਵਾਪਸ ਨਿਰਾ;
    }
    ਜੇ (!(ਮੁੱਲ = ਵਿਅੰਜਕ_Create_Boolean(ਸੈੱਟ))) {
        FreeStmt((ParseCommon *) name);
        ਵਾਪਸ ਨਿਰਾ;
    }
    ਜੇ (!(def = VarCreate(name, ਮੁੱਲ))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) ਮੁੱਲ);
        ਵਾਪਸ ਨਿਰਾ;
    }
    ਵਾਪਸ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ਵਿਅੰਜਕ_Def *ਮੇਲ)
{
    InterpDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_INTERP;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->sym = sym;
    def->ਮੇਲ = ਮੇਲ;
    def->def = ਨਿਰਾ;

    ਵਾਪਸ def;
}

ਕੁੰਜੀ_Type_Def *
ਕੁੰਜੀ_Type_Create(xkb_atom_t name, VarDef *body)
{
    ਕੁੰਜੀ_Type_Def *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_TYPE;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->name = name;
    def->body = body;

    ਵਾਪਸ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ਕੁੰਜੀ_Name, VarDef *symbols)
{
    SymbolsDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_SYMBOLS;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->ਕੁੰਜੀ_Name = ਕੁੰਜੀ_Name;
    def->symbols = symbols;

    ਵਾਪਸ def;
}

ਸਮੂਹ_Compat_Def *
ਸਮੂਹ_Compat_Create(int64_t ਸਮੂਹ, ਵਿਅੰਜਕ_Def *val)
{
    ਸਮੂਹ_Compat_Def *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_ਸਮੂਹ_COMPAT;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->ਸਮੂਹ = ਸਮੂਹ;
    def->def = val;

    ਵਾਪਸ def;
}

ModMapDef *
ModMapCreate(ਵਿਅੰਜਕ_Def *modifiers, ਵਿਅੰਜਕ_Def *ਕੁੰਜੀਆਂ)
{
    ModMapDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_MODMAP;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->modifiers = modifiers;
    def->ਕੁੰਜੀਆਂ = ਕੁੰਜੀਆਂ;

    ਵਾਪਸ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_LED_MAP;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->name = name;
    def->body = body;

    ਵਾਪਸ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ਵਿਅੰਜਕ_Def *name, bool ਆਭਾਸੀ)
{
    LedNameDef *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = STMT_LED_NAME;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->ਮਿਲਾਓ = ਮਿਲਾਓ_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->ਆਭਾਸੀ = ਆਭਾਸੀ;

    ਵਾਪਸ def;
}

UnknownStatement *
UnknownStatementCreate(ਗਿਣਤੀ stmt_type type, ਬਣਤਰ sval name)
{
    UnknownStatement *def = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*def));
    ਜੇ (!def)
        ਵਾਪਸ ਨਿਰਾ;

    def->common.type = type;
    def->common.ਅਗਲਾ = ਨਿਰਾ;
    def->name = strndup(name.ਸ਼ੁਰੂ, name.len);
    ਜੇ (!def->name) {
        ਮੁਕਤ(def);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ def;
}

ਸਥਿਰ ਖਾਲੀ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ਬਣਤਰ xkb_context *ctx, ਅੱਖਰ *str, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    IncludeStmt *incl, *ਪਹਿਲਾ;
    ਅੱਖਰ *stmt, *tmp;
    ਅੱਖਰ nextop;

    incl = ਪਹਿਲਾ = ਨਿਰਾ;
    tmp = str;
    stmt = strdup_ਸੁਰੱਖਿਅਤ(str);
    ਜਦੋਂ (tmp && *tmp)
    {
        ਅੱਖਰ *file = ਨਿਰਾ, *map = ਨਿਰਾ, *extra_ਅੰਕੜੇ = ਨਿਰਾ;

        ਜੇ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_ਅੰਕੜੇ))
            ਜਾਓ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ਜੇ (isempty(file)) {
            ਮੁਕਤ(file);
            ਮੁਕਤ(map);
            ਮੁਕਤ(extra_ਅੰਕੜੇ);
            ਜਾਰੀ;
        }

        ਜੇ (ਪਹਿਲਾ == ਨਿਰਾ) {
            ਪਹਿਲਾ = incl = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*ਪਹਿਲਾ));
        } ਨਹੀਂਤਾਂ {
            incl->ਅਗਲਾ_incl = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*ਪਹਿਲਾ));
            incl = incl->ਅਗਲਾ_incl;
        }

        ਜੇ (!incl) {
            ਮੁਕਤ(file);
            ਮੁਕਤ(map);
            ਮੁਕਤ(extra_ਅੰਕੜੇ);
            ਰੋਕੋ;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.ਅਗਲਾ = ਨਿਰਾ;
        incl->ਮਿਲਾਓ = ਮਿਲਾਓ;
        incl->stmt = ਨਿਰਾ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_ਅੰਕੜੇ;
        incl->ਅਗਲਾ_incl = ਨਿਰਾ;

        ਚੋਣ (nextop) {
        ਹਾਲਤ ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ:
            ਮਿਲਾਓ = ਮਿਲਾਓ_AUGMENT;
            ਰੋਕੋ;
        ਹਾਲਤ ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ:
            ਮਿਲਾਓ = ਮਿਲਾਓ_REPLACE;
            ਰੋਕੋ;
        ਮੂਲ:
            ਮਿਲਾਓ = ਮਿਲਾਓ_OVERRIDE;
        }
    }

    ਜੇ (ਪਹਿਲਾ)
        ਪਹਿਲਾ->stmt = stmt;
    ਨਹੀਂਤਾਂ
        ਮੁਕਤ(stmt);

    ਵਾਪਸ ਪਹਿਲਾ;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(ਪਹਿਲਾ);
    ਮੁਕਤ(stmt);
    ਵਾਪਸ ਨਿਰਾ;
}

XkbFile *
XkbFileCreate(ਗਿਣਤੀ xkb_file_type type, ਅੱਖਰ *name, ParseCommon *defs,
              ਗਿਣਤੀ xkb_map_flags flags)
{
    XkbFile *file;

    file = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*file));
    ਜੇ (!file)
        ਵਾਪਸ ਨਿਰਾ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ਵਾਪਸ file;
}

XkbFile *
XkbFileFromComponents(ਬਣਤਰ xkb_context *ctx,
                      ਅਚਲ ਬਣਤਰ xkb_component_ਨਾਂ *kkctgs)
{
    ਅੱਖਰ *ਅਚਲ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ਗਿਣਤੀ xkb_file_type type;
    IncludeStmt *include = ਨਿਰਾ;
    XkbFile *file = ਨਿਰਾ;
    ParseCommon *defs = ਨਿਰਾ, *defs_ਆਖਰੀ = ਨਿਰਾ;

    ਲਈ (type = ਪਹਿਲਾ_KEYMAP_FILE_TYPE; type <= ਆਖਰੀ_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], ਮਿਲਾਓ_DEFAULT);
        ਜੇ (!include)
            ਜਾਓ err;

        file = XkbFileCreate(type, ਨਿਰਾ, (ParseCommon *) include, 0);
        ਜੇ (!file) {
            FreeInclude(include);
            ਜਾਓ err;
        }

        ਜੇ (!defs)
            defs_ਆਖਰੀ = defs = &file->common;
        ਨਹੀਂਤਾਂ
            defs_ਆਖਰੀ = defs_ਆਖਰੀ->ਅਗਲਾ = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ਨਿਰਾ, defs, 0);
    ਜੇ (!file)
        ਜਾਓ err;

    ਵਾਪਸ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *ਅਗਲਾ;

    ਜਦੋਂ (incl)
    {
        ਅਗਲਾ = incl->ਅਗਲਾ_incl;

        ਮੁਕਤ(incl->file);
        ਮੁਕਤ(incl->map);
        ਮੁਕਤ(incl->modifier);
        ਮੁਕਤ(incl->stmt);

        ਮੁਕਤ(incl);
        incl = ਅਗਲਾ;
    }
}

ਖਾਲੀ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *ਅਗਲਾ;

    ਜਦੋਂ (stmt)
    {
        ਅਗਲਾ = stmt->ਅਗਲਾ;

        ਚੋਣ (stmt->type) {
        ਹਾਲਤ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ਨਿਰਾ;
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_Unary *) stmt)->ਬੱਚਾ);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ:
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_ਦੁਆਧਾਰੀ *) stmt)->ਖੱਬਾ);
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_ਦੁਆਧਾਰੀ *) stmt)->ਸੱਜਾ);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_Action *) stmt)->ਦਲੀਲਾਂ);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_Action_List *) stmt)->actions);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ਵਿਅੰਜਕ_Array_Ref *) stmt)->ਇੰਦਰਾਜ);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LIST:
            darray_free(((ਵਿਅੰਜਕ_Keysym_List *) stmt)->syms);
            ਰੋਕੋ;

        ਹਾਲਤ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->ਮੁੱਲ);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_TYPE:
            FreeStmt((ParseCommon *) ((ਕੁੰਜੀ_Type_Def *) stmt)->body);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->ਮੇਲ);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->ਮੁੱਲ);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ਕੁੰਜੀਆਂ);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਸਮੂਹ_COMPAT:
            FreeStmt((ParseCommon *) ((ਸਮੂਹ_Compat_Def *) stmt)->def);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_UNKNOWN_DECLARATION:
        ਹਾਲਤ STMT_UNKNOWN_COMPOUND:
            ਮੁਕਤ(((UnknownStatement *) stmt)->name);
            ਰੋਕੋ;
        ਮੂਲ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_ਅੰਕ_ਮੁੱਲਾਂ == 37 &&
                          STMT_ਅੰਕ_ਮੁੱਲਾਂ == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ਰੋਕੋ;
        }

        ਮੁਕਤ(stmt);
        stmt = ਅਗਲਾ;
    }
}

ਖਾਲੀ
FreeXkbFile(XkbFile *file)
{
    XkbFile *ਅਗਲਾ;

    ਜਦੋਂ (file)
    {
        ਅਗਲਾ = (XkbFile *) file->common.ਅਗਲਾ;

        ਚੋਣ (file->file_type) {
        ਹਾਲਤ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ਰੋਕੋ;

        ਹਾਲਤ FILE_TYPE_TYPES:
        ਹਾਲਤ FILE_TYPE_COMPAT:
        ਹਾਲਤ FILE_TYPE_SYMBOLS:
        ਹਾਲਤ FILE_TYPE_KEYCODES:
        ਹਾਲਤ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ਰੋਕੋ;

        ਮੂਲ:
            ਰੋਕੋ;
        }

        ਮੁਕਤ(file->name);
        ਮੁਕਤ(file);
        file = ਅਗਲਾ;
    }
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *xkb_file_type_ਲੜੀਆਂ[FILE_TYPE_ਅੰਕ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_ਨਿਯਮਾਂ] = "rules",
};

ਅਚਲ ਅੱਖਰ *
xkb_file_type_ਮੰਜ਼ਿਲ_string(ਗਿਣਤੀ xkb_file_type type)
{
    ਜੇ (type >= FILE_TYPE_ਅੰਕ_ENTRIES)
        ਵਾਪਸ "unknown";
    ਵਾਪਸ xkb_file_type_ਲੜੀਆਂ[type];
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *stmt_type_ਲੜੀਆਂ[STMT_ਅੰਕ_ਮੁੱਲਾਂ] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_ਵਿਅੰਜਕ_STRING_LITERAL] = "string literal expression",
    [STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL] = "integer literal expression",
    [STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL] = "float literal expression",
    [STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL] = "key name expression",
    [STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL] = "keysym expression",
    [STMT_ਵਿਅੰਜਕ_IDENT] = "identifier expression",
    [STMT_ਵਿਅੰਜਕ_ACTION_DECL] = "action declaration expression",
    [STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF] = "field reference expression",
    [STMT_ਵਿਅੰਜਕ_ARRAY_REF] = "array reference expression",
    [STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST] = "empty list expression",
    [STMT_ਵਿਅੰਜਕ_KEYSYM_LIST] = "keysym list expression",
    [STMT_ਵਿਅੰਜਕ_ACTION_LIST] = "action list expression",
    [STMT_ਵਿਅੰਜਕ_ਜੋੜੋ] = "addition expression",
    [STMT_ਵਿਅੰਜਕ_ਘਟਾਓ] = "substraction expression",
    [STMT_ਵਿਅੰਜਕ_MULTIPLY] = "multiplication expression",
    [STMT_ਵਿਅੰਜਕ_DIVIDE] = "division expression",
    [STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ] = "assignment expression",
    [STMT_ਵਿਅੰਜਕ_NOT] = "logical negation expression",
    [STMT_ਵਿਅੰਜਕ_NEGATE] = "arithmetic negation expression",
    [STMT_ਵਿਅੰਜਕ_INVERT] = "bitwise inversion expression",
    [STMT_ਵਿਅੰਜਕ_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_ਸਮੂਹ_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_ਅੰਕ_ਮੁੱਲਾਂ == 37 &&
              STMT_ਅੰਕ_ਮੁੱਲਾਂ == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ਅਚਲ ਅੱਖਰ *
stmt_type_ਮੰਜ਼ਿਲ_string(ਗਿਣਤੀ stmt_type type)
{
    ਜੇ (type >= STMT_ਅੰਕ_ਮੁੱਲਾਂ)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਾਪਸ stmt_type_ਲੜੀਆਂ[type];
}

ਅੱਖਰ
stmt_type_ਮੰਜ਼ਿਲ_operator_char(ਗਿਣਤੀ stmt_type type)
{
    ਚੋਣ (type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
        ਵਾਪਸ '+';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
        ਵਾਪਸ '-';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
        ਵਾਪਸ '*';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
        ਵਾਪਸ '/';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
        ਵਾਪਸ '!';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
        ਵਾਪਸ '-';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
        ਵਾਪਸ '~';
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
        ਵਾਪਸ '+';
    ਮੂਲ:
        ਵਾਪਸ '\0';
    }
}
