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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "ast-build.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "utf8-decoding.h"

ಸ್ಥಿರ ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create(ಗಣನಾ stmt_type op)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = malloc(ಪರಿಮಾಣ(*ಅಭಿವ್ಯಕ್ತಿ));
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಅಭಿವ್ಯಕ್ತಿ->common.type = op;
    ಅಭಿವ್ಯಕ್ತಿ->common.next = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_String(xkb_atom_t str)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->string.str = str;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Integer(int64_t ival)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->integer.ival = ival;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Float(ಶೂನ್ಯ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(bool set)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->boolean.set = set;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name(xkb_atom_t ಕೀಲಿ_name)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name = ಕೀಲಿ_name;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym(xkb_keysym_t keysym)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->keysym.keysym = keysym;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(xkb_atom_t ident)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->ident.ident = ident;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(ಗಣನಾ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *child)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(op);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->unary.child = child;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(ಗಣನಾ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, ಅಭಿವ್ಯಕ್ತಿ_Def *ಬಲ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(op);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ = ಎಡ;
    ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ = ಬಲ;

    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->field_ref.element = element;
    ಅಭಿವ್ಯಕ್ತಿ->field_ref.field = field;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ಅಭಿವ್ಯಕ್ತಿ_Def *entry)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.element = element;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.field = field;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.entry = entry;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Empty_List(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST);
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *args)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->action.name = name;
    ಅಭಿವ್ಯಕ್ತಿ->action.args = args;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಅಭಿವ್ಯಕ್ತಿ_Def *actions)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅಭಿವ್ಯಕ್ತಿ->actions.actions = actions;
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(xkb_keysym_t sym)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST);
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    darray_init(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms);
    ಯದಿ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        /* Discard NoSymbol */
    } ಅನ್ಯಥಾ {
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List(ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_keysym_t sym)
{
    ಯದಿ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        /* Discard NoSymbol */
    } ಅನ್ಯಥಾ {
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(ಸಂರಚನಾ scanner *scanner,
                           ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಧ್ರುವ ವರ್ಣ *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ಧ್ರುವ size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    ಯಾವತ್ (idx < len) {
        size_t ಎಣಿಕೆ = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &ಎಣಿಕೆ);
        ಯದಿ (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            ಗಚ್ಛ error;
        }
        ಧ್ರುವ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        ಯದಿ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            ಗಚ್ಛ error;
        }
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
        idx += ಎಣಿಕೆ;
        idx_cp++;
    }
    ಪ್ರತಿಜ್ಞಾ(string[idx] == '\0');
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ;
error:
    FreeStmt((ParseCommon*) ಅಭಿವ್ಯಕ್ತಿ);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

xkb_keysym_t
KeysymParseString(ಸಂರಚನಾ scanner *scanner, ಧ್ರುವ ವರ್ಣ *string)
{
    ಧ್ರುವ size_t len = strlen(string);
    ಯದಿ (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    }
    size_t ಎಣಿಕೆ = 0;
    ಧ್ರುವ uint32_t cp = utf8_next_code_point(string, len, &ಎಣಿಕೆ);
    ಯದಿ (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    } ಅನ್ಯಥಾ ಯದಿ (ಎಣಿಕೆ != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    }
    ಧ್ರುವ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    ಯದಿ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ಪ್ರತ್ಯಾಗಮ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ಮೌಲ್ಯ)
{
    KeycodeDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_KEYCODE;
    def->common.next = ರಿಕ್ತ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ def;
}

ಕೀಲಿ_Alias_Def *
ಕೀಲಿ_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ಕೀಲಿ_Alias_Def *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_ALIAS;
    def->common.next = ರಿಕ್ತ;
    def->alias = alias;
    def->real = real;

    ಪ್ರತ್ಯಾಗಮ def;
}

VModDef *
VModCreate(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    VModDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_VMOD;
    def->common.next = ರಿಕ್ತ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ def;
}

VarDef *
VarCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    VarDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_VAR;
    def->common.next = ರಿಕ್ತ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *name, *ಮೌಲ್ಯ;
    VarDef *def;
    ಯದಿ (!(name = ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(ident))) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    ಯದಿ (!(ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    ಯದಿ (!(def = VarCreate(name, ಮೌಲ್ಯ))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) ಮೌಲ್ಯ);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    ಪ್ರತ್ಯಾಗಮ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ಅಭಿವ್ಯಕ್ತಿ_Def *match)
{
    InterpDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_INTERP;
    def->common.next = ರಿಕ್ತ;
    def->sym = sym;
    def->match = match;
    def->def = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ def;
}

ಕೀಲಿ_Type_Def *
ಕೀಲಿ_Type_Create(xkb_atom_t name, VarDef *body)
{
    ಕೀಲಿ_Type_Def *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_TYPE;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ಪ್ರತ್ಯಾಗಮ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ಕೀಲಿ_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_SYMBOLS;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->ಕೀಲಿ_Name = ಕೀಲಿ_Name;
    def->symbols = symbols;

    ಪ್ರತ್ಯಾಗಮ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, ಅಭಿವ್ಯಕ್ತಿ_Def *val)
{
    GroupCompatDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ಪ್ರತ್ಯಾಗಮ def;
}

ModMapDef *
ModMapCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *modifiers, ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿಗಳು)
{
    ModMapDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_MODMAP;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->ಕೀಲಿಗಳು = ಕೀಲಿಗಳು;

    ಪ್ರತ್ಯಾಗಮ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_LED_MAP;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ಪ್ರತ್ಯಾಗಮ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ಅಭಿವ್ಯಕ್ತಿ_Def *name, bool ಆಭಾಸ)
{
    LedNameDef *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = STMT_LED_NAME;
    def->common.next = ರಿಕ್ತ;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->ಆಭಾಸ = ಆಭಾಸ;

    ಪ್ರತ್ಯಾಗಮ def;
}

UnknownStatement *
UnknownStatementCreate(ಗಣನಾ stmt_type type, ಸಂರಚನಾ sval name)
{
    UnknownStatement *def = malloc(ಪರಿಮಾಣ(*def));
    ಯದಿ (!def)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    def->common.type = type;
    def->common.next = ರಿಕ್ತ;
    def->name = strndup(name.start, name.len);
    ಯದಿ (!def->name) {
        ಮುಕ್ತ(def);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ def;
}

ಸ್ಥಿರ ಶೂನ್ಯ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ಸಂರಚನಾ xkb_context *ctx, ವರ್ಣ *str, ಗಣನಾ merge_mode merge)
{
    IncludeStmt *incl, *first;
    ವರ್ಣ *stmt, *tmp;
    ವರ್ಣ nextop;

    incl = first = ರಿಕ್ತ;
    tmp = str;
    stmt = strdup_safe(str);
    ಯಾವತ್ (tmp && *tmp)
    {
        ವರ್ಣ *file = ರಿಕ್ತ, *map = ರಿಕ್ತ, *extra_data = ರಿಕ್ತ;

        ಯದಿ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            ಗಚ್ಛ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ಯದಿ (isempty(file)) {
            ಮುಕ್ತ(file);
            ಮುಕ್ತ(map);
            ಮುಕ್ತ(extra_data);
            ಅನುವರ್ತ;
        }

        ಯದಿ (first == ರಿಕ್ತ) {
            first = incl = malloc(ಪರಿಮಾಣ(*first));
        } ಅನ್ಯಥಾ {
            incl->next_incl = malloc(ಪರಿಮಾಣ(*first));
            incl = incl->next_incl;
        }

        ಯದಿ (!incl) {
            ಮುಕ್ತ(file);
            ಮುಕ್ತ(map);
            ಮುಕ್ತ(extra_data);
            ಭಙ್ಗ;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = ರಿಕ್ತ;
        incl->merge = merge;
        incl->stmt = ರಿಕ್ತ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = ರಿಕ್ತ;

        ವಿಕಲ್ಪ (nextop) {
        ಪಕ್ಷ MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            ಭಙ್ಗ;
        ಪಕ್ಷ MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            merge = MERGE_OVERRIDE;
        }
    }

    ಯದಿ (first)
        first->stmt = stmt;
    ಅನ್ಯಥಾ
        ಮುಕ್ತ(stmt);

    ಪ್ರತ್ಯಾಗಮ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    ಮುಕ್ತ(stmt);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

XkbFile *
XkbFileCreate(ಗಣನಾ xkb_file_type type, ವರ್ಣ *name, ParseCommon *defs,
              ಗಣನಾ xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, ಪರಿಮಾಣ(*file));
    ಯದಿ (!file)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ಪ್ರತ್ಯಾಗಮ file;
}

XkbFile *
XkbFileFromComponents(ಸಂರಚನಾ xkb_context *ctx,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_component_names *kkctgs)
{
    ವರ್ಣ *ಧ್ರುವ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ಗಣನಾ xkb_file_type type;
    IncludeStmt *include = ರಿಕ್ತ;
    XkbFile *file = ರಿಕ್ತ;
    ParseCommon *defs = ರಿಕ್ತ, *defsLast = ರಿಕ್ತ;

    ಪ್ರತಿ (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        ಯದಿ (!include)
            ಗಚ್ಛ err;

        file = XkbFileCreate(type, ರಿಕ್ತ, (ParseCommon *) include, 0);
        ಯದಿ (!file) {
            FreeInclude(include);
            ಗಚ್ಛ err;
        }

        ಯದಿ (!defs)
            defsLast = defs = &file->common;
        ಅನ್ಯಥಾ
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ರಿಕ್ತ, defs, 0);
    ಯದಿ (!file)
        ಗಚ್ಛ err;

    ಪ್ರತ್ಯಾಗಮ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    ಯಾವತ್ (incl)
    {
        next = incl->next_incl;

        ಮುಕ್ತ(incl->file);
        ಮುಕ್ತ(incl->map);
        ಮುಕ್ತ(incl->modifier);
        ಮುಕ್ತ(incl->stmt);

        ಮುಕ್ತ(incl);
        incl = next;
    }
}

ಶೂನ್ಯ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    ಯಾವತ್ (stmt)
    {
        next = stmt->next;

        ವಿಕಲ್ಪ (stmt->type) {
        ಪಕ್ಷ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ರಿಕ್ತ;
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Unary *) stmt)->child);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ *) stmt)->ಎಡ);
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ *) stmt)->ಬಲ);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Action *) stmt)->args);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Action_List *) stmt)->actions);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Array_Ref *) stmt)->entry);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
            darray_free(((ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) stmt)->syms);
            ಭಙ್ಗ;

        ಪಕ್ಷ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->ಮೌಲ್ಯ);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_TYPE:
            FreeStmt((ParseCommon *) ((ಕೀಲಿ_Type_Def *) stmt)->body);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->ಮೌಲ್ಯ);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ಕೀಲಿಗಳು);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_UNKNOWN_DECLARATION:
        ಪಕ್ಷ STMT_UNKNOWN_COMPOUND:
            ಮುಕ್ತ(((UnknownStatement *) stmt)->name);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == 37 &&
                          STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ಭಙ್ಗ;
        }

        ಮುಕ್ತ(stmt);
        stmt = next;
    }
}

ಶೂನ್ಯ
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    ಯಾವತ್ (file)
    {
        next = (XkbFile *) file->common.next;

        ವಿಕಲ್ಪ (file->file_type) {
        ಪಕ್ಷ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ಭಙ್ಗ;

        ಪಕ್ಷ FILE_TYPE_TYPES:
        ಪಕ್ಷ FILE_TYPE_COMPAT:
        ಪಕ್ಷ FILE_TYPE_SYMBOLS:
        ಪಕ್ಷ FILE_TYPE_KEYCODES:
        ಪಕ್ಷ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
        }

        ಮುಕ್ತ(file->name);
        ಮುಕ್ತ(file);
        file = next;
    }
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *xkb_file_type_strings[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ಧ್ರುವ ವರ್ಣ *
xkb_file_type_to_string(ಗಣನಾ xkb_file_type type)
{
    ಯದಿ (type >= FILE_TYPE_ಅಂಕೆ_ENTRIES)
        ಪ್ರತ್ಯಾಗಮ "unknown";
    ಪ್ರತ್ಯಾಗಮ xkb_file_type_strings[type];
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *stmt_type_strings[STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL] = "string literal expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL] = "integer literal expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL] = "float literal expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL] = "key name expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL] = "keysym expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT] = "identifier expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL] = "action declaration expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF] = "field reference expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF] = "array reference expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST] = "empty list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST] = "keysym list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST] = "action list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD] = "addition expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT] = "substraction expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY] = "multiplication expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE] = "division expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN] = "assignment expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT] = "logical negation expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE] = "arithmetic negation expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT] = "bitwise inversion expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == 37 &&
              STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ಧ್ರುವ ವರ್ಣ *
stmt_type_to_string(ಗಣನಾ stmt_type type)
{
    ಯದಿ (type >= STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ stmt_type_strings[type];
}

ವರ್ಣ
stmt_type_to_operator_char(ಗಣನಾ stmt_type type)
{
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
        ಪ್ರತ್ಯಾಗಮ '+';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
        ಪ್ರತ್ಯಾಗಮ '-';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಪ್ರತ್ಯಾಗಮ '*';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಪ್ರತ್ಯಾಗಮ '/';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಪ್ರತ್ಯಾಗಮ '!';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಪ್ರತ್ಯಾಗಮ '-';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
        ಪ್ರತ್ಯಾಗಮ '~';
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಪ್ರತ್ಯಾಗಮ '+';
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ '\0';
    }
}
