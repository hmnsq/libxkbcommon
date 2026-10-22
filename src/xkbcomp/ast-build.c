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
ಅಭಿವ್ಯಕ್ತಿ_Create(ಎಣಿಕೆ stmt_type op)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*ಅಭಿವ್ಯಕ್ತಿ));
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಅಭಿವ್ಯಕ್ತಿ->common.type = op;
    ಅಭಿವ್ಯಕ್ತಿ->common.ಮುಂದಿನ = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_String(xkb_atom_t str)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->string.str = str;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Integer(int64_t ival)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->integer.ival = ival;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Float(ಶೂನ್ಯ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(bool ಗಣ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->boolean.ಗಣ = ಗಣ;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name(xkb_atom_t ಕೀಲಿ_name)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name = ಕೀಲಿ_name;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym(xkb_keysym_t keysym)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->keysym.keysym = keysym;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(xkb_atom_t ident)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->ident.ident = ident;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(ಎಣಿಕೆ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮಗು)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(op);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು = ಮಗು;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(ಎಣಿಕೆ stmt_type op, ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, ಅಭಿವ್ಯಕ್ತಿ_Def *ಬಲ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(op);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ = ಎಡ;
    ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ = ಬಲ;

    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕ್ಷೇತ್ರ_Ref(xkb_atom_t element, xkb_atom_t ಕ್ಷೇತ್ರ_2)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element = element;
    ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2 = ಕ್ಷೇತ್ರ_2;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ಕ್ಷೇತ್ರ_2, ಅಭಿವ್ಯಕ್ತಿ_Def *ನಮೂದು)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.element = element;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.ಕ್ಷೇತ್ರ_2 = ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ->array_ref.ನಮೂದು = ನಮೂದು;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_List(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST);
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ವಾದಗಳು)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->action.name = name;
    ಅಭಿವ್ಯಕ್ತಿ->action.ವಾದಗಳು = ವಾದಗಳು;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಅಭಿವ್ಯಕ್ತಿ_Def *actions)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಅಭಿವ್ಯಕ್ತಿ->actions.actions = actions;
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(xkb_keysym_t sym)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST);
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    darray_init(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms);
    ಆದರೆ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        /* Discard NoSymbol */
    } ಇಲ್ಲದಿದ್ದರೆ {
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
    }
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List(ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_keysym_t sym)
{
    ಆದರೆ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        /* Discard NoSymbol */
    } ಇಲ್ಲದಿದ್ದರೆ {
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
    }
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
}

ಅಭಿವ್ಯಕ್ತಿ_Def *
ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(ರಚನೆ scanner *scanner,
                           ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string);
    ಗಾತ್ರ_t idx = 0;
    ಗಾತ್ರ_t idx_cp = 1;
    ಆಗ (idx < len) {
        ಗಾತ್ರ_t ಎಣಿಕೆ_2 = 0;
        uint32_t cp = utf8_ಮುಂದಿನ_ಸಂಕೇತ_ಬಿಂದು(string + idx, len - idx, &ಎಣಿಕೆ_2);
        ಆದರೆ (cp == INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            ಹೋಗು error;
        }
        ಸ್ಥಿರಾಂಕ xkb_keysym_t sym = xkb_utf32_ಗಮ್ಯ_keysym(cp);
        ಆದರೆ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(scanner, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            ಹೋಗು error;
        }
        darray_append(ಅಭಿವ್ಯಕ್ತಿ->keysym_list.syms, sym);
        idx += ಎಣಿಕೆ_2;
        idx_cp++;
    }
    ಪ್ರತಿಜ್ಞಾ(string[idx] == '\0');
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ;
error:
    FreeStmt((ParseCommon*) ಅಭಿವ್ಯಕ್ತಿ);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

xkb_keysym_t
KeysymParseString(ರಚನೆ scanner *scanner, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string)
{
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string);
    ಆದರೆ (len == 0) {
        scanner_err(scanner, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    }
    ಗಾತ್ರ_t ಎಣಿಕೆ_2 = 0;
    ಸ್ಥಿರಾಂಕ uint32_t cp = utf8_ಮುಂದಿನ_ಸಂಕೇತ_ಬಿಂದು(string, len, &ಎಣಿಕೆ_2);
    ಆದರೆ (cp == INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಣಿಕೆ_2 != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    }
    ಸ್ಥಿರಾಂಕ xkb_keysym_t sym = xkb_utf32_ಗಮ್ಯ_keysym(cp);
    ಆದರೆ (sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        scanner_err(scanner, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ಹಿಂತಿರುಗಿ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ಮೌಲ್ಯ)
{
    KeycodeDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_KEYCODE;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ def;
}

ಕೀಲಿ_Alias_Def *
ಕೀಲಿ_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ಕೀಲಿ_Alias_Def *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_ALIAS;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->alias = alias;
    def->real = real;

    ಹಿಂತಿರುಗಿ def;
}

VModDef *
VModCreate(xkb_atom_t name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    VModDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_VMOD;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ def;
}

VarDef *
VarCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *name, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    VarDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_VAR;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->name = name;
    def->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool ಗಣ)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def *name, *ಮೌಲ್ಯ;
    VarDef *def;
    ಆದರೆ (!(name = ಅಭಿವ್ಯಕ್ತಿ_Create_Ident(ident))) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ಆದರೆ (!(ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_Boolean(ಗಣ))) {
        FreeStmt((ParseCommon *) name);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ಆದರೆ (!(def = VarCreate(name, ಮೌಲ್ಯ))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) ಮೌಲ್ಯ);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ಹಿಂತಿರುಗಿ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ಅಭಿವ್ಯಕ್ತಿ_Def *ಹೊಂದಿಕೆ)
{
    InterpDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_INTERP;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->sym = sym;
    def->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ;
    def->def = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ def;
}

ಕೀಲಿ_Type_Def *
ಕೀಲಿ_Type_Create(xkb_atom_t name, VarDef *body)
{
    ಕೀಲಿ_Type_Def *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_TYPE;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->name = name;
    def->body = body;

    ಹಿಂತಿರುಗಿ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ಕೀಲಿ_Name, VarDef *symbols)
{
    SymbolsDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_SYMBOLS;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->ಕೀಲಿ_Name = ಕೀಲಿ_Name;
    def->symbols = symbols;

    ಹಿಂತಿರುಗಿ def;
}

ಗುಂಪು_Compat_Def *
ಗುಂಪು_Compat_Create(int64_t ಗುಂಪು, ಅಭಿವ್ಯಕ್ತಿ_Def *val)
{
    ಗುಂಪು_Compat_Def *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_ಗುಂಪು_COMPAT;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->ಗುಂಪು = ಗುಂಪು;
    def->def = val;

    ಹಿಂತಿರುಗಿ def;
}

ModMapDef *
ModMapCreate(ಅಭಿವ್ಯಕ್ತಿ_Def *modifiers, ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿಗಳು)
{
    ModMapDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_MODMAP;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->modifiers = modifiers;
    def->ಕೀಲಿಗಳು = ಕೀಲಿಗಳು;

    ಹಿಂತಿರುಗಿ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_LED_MAP;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->name = name;
    def->body = body;

    ಹಿಂತಿರುಗಿ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ಅಭಿವ್ಯಕ್ತಿ_Def *name, bool ವಾಸ್ತವಿಕ)
{
    LedNameDef *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = STMT_LED_NAME;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->ವಿಲೀನ = ವಿಲೀನ_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->ವಾಸ್ತವಿಕ = ವಾಸ್ತವಿಕ;

    ಹಿಂತಿರುಗಿ def;
}

UnknownStatement *
UnknownStatementCreate(ಎಣಿಕೆ stmt_type type, ರಚನೆ sval name)
{
    UnknownStatement *def = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*def));
    ಆದರೆ (!def)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    def->common.type = type;
    def->common.ಮುಂದಿನ = ಖಾಲಿ;
    def->name = strndup(name.ಆರಂಭ, name.len);
    ಆದರೆ (!def->name) {
        ಬಿಡುಗಡೆ(def);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ def;
}

ಸ್ಥಿರ ಶೂನ್ಯ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ರಚನೆ xkb_context *ctx, ಅಕ್ಷರ *str, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    IncludeStmt *incl, *ಮೊದಲ;
    ಅಕ್ಷರ *stmt, *tmp;
    ಅಕ್ಷರ nextop;

    incl = ಮೊದಲ = ಖಾಲಿ;
    tmp = str;
    stmt = strdup_ಸುರಕ್ಷಿತ(str);
    ಆಗ (tmp && *tmp)
    {
        ಅಕ್ಷರ *file = ಖಾಲಿ, *map = ಖಾಲಿ, *extra_ದತ್ತಾಂಶ = ಖಾಲಿ;

        ಆದರೆ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_ದತ್ತಾಂಶ))
            ಹೋಗು err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ಆದರೆ (isempty(file)) {
            ಬಿಡುಗಡೆ(file);
            ಬಿಡುಗಡೆ(map);
            ಬಿಡುಗಡೆ(extra_ದತ್ತಾಂಶ);
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ಮೊದಲ == ಖಾಲಿ) {
            ಮೊದಲ = incl = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*ಮೊದಲ));
        } ಇಲ್ಲದಿದ್ದರೆ {
            incl->ಮುಂದಿನ_incl = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*ಮೊದಲ));
            incl = incl->ಮುಂದಿನ_incl;
        }

        ಆದರೆ (!incl) {
            ಬಿಡುಗಡೆ(file);
            ಬಿಡುಗಡೆ(map);
            ಬಿಡುಗಡೆ(extra_ದತ್ತಾಂಶ);
            ನಿಲ್ಲಿಸು;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.ಮುಂದಿನ = ಖಾಲಿ;
        incl->ವಿಲೀನ = ವಿಲೀನ;
        incl->stmt = ಖಾಲಿ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_ದತ್ತಾಂಶ;
        incl->ಮುಂದಿನ_incl = ಖಾಲಿ;

        ಆಯ್ಕೆ (nextop) {
        ಸ್ಥಿತಿ ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ:
            ವಿಲೀನ = ವಿಲೀನ_AUGMENT;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ:
            ವಿಲೀನ = ವಿಲೀನ_REPLACE;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ವಿಲೀನ = ವಿಲೀನ_OVERRIDE;
        }
    }

    ಆದರೆ (ಮೊದಲ)
        ಮೊದಲ->stmt = stmt;
    ಇಲ್ಲದಿದ್ದರೆ
        ಬಿಡುಗಡೆ(stmt);

    ಹಿಂತಿರುಗಿ ಮೊದಲ;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(ಮೊದಲ);
    ಬಿಡುಗಡೆ(stmt);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

XkbFile *
XkbFileCreate(ಎಣಿಕೆ xkb_file_type type, ಅಕ್ಷರ *name, ParseCommon *defs,
              ಎಣಿಕೆ xkb_map_flags flags)
{
    XkbFile *file;

    file = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*file));
    ಆದರೆ (!file)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ಹಿಂತಿರುಗಿ file;
}

XkbFile *
XkbFileFromComponents(ರಚನೆ xkb_context *ctx,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_component_ಹೆಸರುಗಳು *kkctgs)
{
    ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ಎಣಿಕೆ xkb_file_type type;
    IncludeStmt *include = ಖಾಲಿ;
    XkbFile *file = ಖಾಲಿ;
    ParseCommon *defs = ಖಾಲಿ, *defs_ಕೊನೆಯ = ಖಾಲಿ;

    ಗಾಗಿ (type = ಮೊದಲ_KEYMAP_FILE_TYPE; type <= ಕೊನೆಯ_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], ವಿಲೀನ_DEFAULT);
        ಆದರೆ (!include)
            ಹೋಗು err;

        file = XkbFileCreate(type, ಖಾಲಿ, (ParseCommon *) include, 0);
        ಆದರೆ (!file) {
            FreeInclude(include);
            ಹೋಗು err;
        }

        ಆದರೆ (!defs)
            defs_ಕೊನೆಯ = defs = &file->common;
        ಇಲ್ಲದಿದ್ದರೆ
            defs_ಕೊನೆಯ = defs_ಕೊನೆಯ->ಮುಂದಿನ = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ಖಾಲಿ, defs, 0);
    ಆದರೆ (!file)
        ಹೋಗು err;

    ಹಿಂತಿರುಗಿ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *ಮುಂದಿನ;

    ಆಗ (incl)
    {
        ಮುಂದಿನ = incl->ಮುಂದಿನ_incl;

        ಬಿಡುಗಡೆ(incl->file);
        ಬಿಡುಗಡೆ(incl->map);
        ಬಿಡುಗಡೆ(incl->modifier);
        ಬಿಡುಗಡೆ(incl->stmt);

        ಬಿಡುಗಡೆ(incl);
        incl = ಮುಂದಿನ;
    }
}

ಶೂನ್ಯ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *ಮುಂದಿನ;

    ಆಗ (stmt)
    {
        ಮುಂದಿನ = stmt->ಮುಂದಿನ;

        ಆಯ್ಕೆ (stmt->type) {
        ಸ್ಥಿತಿ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ಖಾಲಿ;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Unary *) stmt)->ಮಗು);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ *) stmt)->ಎಡ);
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ *) stmt)->ಬಲ);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Action *) stmt)->ವಾದಗಳು);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Action_List *) stmt)->actions);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ಅಭಿವ್ಯಕ್ತಿ_Array_Ref *) stmt)->ನಮೂದು);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
            darray_free(((ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) stmt)->syms);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->ಮೌಲ್ಯ);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_TYPE:
            FreeStmt((ParseCommon *) ((ಕೀಲಿ_Type_Def *) stmt)->body);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->ಹೊಂದಿಕೆ);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->ಮೌಲ್ಯ);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ಕೀಲಿಗಳು);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಗುಂಪು_COMPAT:
            FreeStmt((ParseCommon *) ((ಗುಂಪು_Compat_Def *) stmt)->def);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_UNKNOWN_DECLARATION:
        ಸ್ಥಿತಿ STMT_UNKNOWN_COMPOUND:
            ಬಿಡುಗಡೆ(((UnknownStatement *) stmt)->name);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == 37 &&
                          STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ನಿಲ್ಲಿಸು;
        }

        ಬಿಡುಗಡೆ(stmt);
        stmt = ಮುಂದಿನ;
    }
}

ಶೂನ್ಯ
FreeXkbFile(XkbFile *file)
{
    XkbFile *ಮುಂದಿನ;

    ಆಗ (file)
    {
        ಮುಂದಿನ = (XkbFile *) file->common.ಮುಂದಿನ;

        ಆಯ್ಕೆ (file->file_type) {
        ಸ್ಥಿತಿ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FILE_TYPE_TYPES:
        ಸ್ಥಿತಿ FILE_TYPE_COMPAT:
        ಸ್ಥಿತಿ FILE_TYPE_SYMBOLS:
        ಸ್ಥಿತಿ FILE_TYPE_KEYCODES:
        ಸ್ಥಿತಿ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
        }

        ಬಿಡುಗಡೆ(file->name);
        ಬಿಡುಗಡೆ(file);
        file = ಮುಂದಿನ;
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xkb_file_type_ಅಕ್ಷರಸರಣಿಗಳು[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_ನಿಯಮಗಳು] = "rules",
};

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_type_ಗಮ್ಯ_string(ಎಣಿಕೆ xkb_file_type type)
{
    ಆದರೆ (type >= FILE_TYPE_ಅಂಕೆ_ENTRIES)
        ಹಿಂತಿರುಗಿ "unknown";
    ಹಿಂತಿರುಗಿ xkb_file_type_ಅಕ್ಷರಸರಣಿಗಳು[type];
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stmt_type_ಅಕ್ಷರಸರಣಿಗಳು[STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು] = {
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
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF] = "field reference expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF] = "array reference expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST] = "empty list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST] = "keysym list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST] = "action list expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು] = "addition expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ] = "substraction expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY] = "multiplication expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE] = "division expression",
    [STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು] = "assignment expression",
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
    [STMT_ಗುಂಪು_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == 37 &&
              STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
stmt_type_ಗಮ್ಯ_string(ಎಣಿಕೆ stmt_type type)
{
    ಆದರೆ (type >= STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ stmt_type_ಅಕ್ಷರಸರಣಿಗಳು[type];
}

ಅಕ್ಷರ
stmt_type_ಗಮ್ಯ_operator_char(ಎಣಿಕೆ stmt_type type)
{
    ಆಯ್ಕೆ (type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
        ಹಿಂತಿರುಗಿ '+';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
        ಹಿಂತಿರುಗಿ '-';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಹಿಂತಿರುಗಿ '*';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಹಿಂತಿರುಗಿ '/';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಹಿಂತಿರುಗಿ '!';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಹಿಂತಿರುಗಿ '-';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
        ಹಿಂತಿರುಗಿ '~';
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಹಿಂತಿರುಗಿ '+';
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ '\0';
    }
}
