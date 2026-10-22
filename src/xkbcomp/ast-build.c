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

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "ast-build.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "utf8-decoding.h"

स्थिर व्यञ्जकम्_Def *
व्यञ्जकम्_Create(गणना stmt_type op)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = स्मृतिं_देहि(परिमाण(*व्यञ्जकम्));
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;

    व्यञ्जकम्->common.type = op;
    व्यञ्जकम्->common.अग्र = रिक्त;

    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_String(xkb_atom_t str)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_STRING_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->string.str = str;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Integer(int64_t ival)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_INTEGER_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->integer.ival = ival;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Float(शून्य)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_FLOAT_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Boolean(bool समुच्चयः)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_BOOLEAN_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->boolean.समुच्चयः = समुच्चयः;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Name(xkb_atom_t कुञ्चिका_name)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYNAME_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name = कुञ्चिका_name;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym(xkb_keysym_t keysym)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYSYM_LITERAL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->keysym.keysym = keysym;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Ident(xkb_atom_t ident)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_IDENT);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->ident.ident = ident;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Unary(गणना stmt_type op, व्यञ्जकम्_Def *child)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(op);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->unary.child = child;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_द्विमान(गणना stmt_type op, व्यञ्जकम्_Def *वामः, व्यञ्जकम्_Def *दक्षिणः)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(op);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;

    व्यञ्जकम्->द्विमान.वामः = वामः;
    व्यञ्जकम्->द्विमान.दक्षिणः = दक्षिणः;

    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_क्षेत्रम्_Ref(xkb_atom_t element, xkb_atom_t क्षेत्रम्_2)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_क्षेत्रम्_REF);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->क्षेत्रम्_ref.element = element;
    व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2 = क्षेत्रम्_2;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Array_Ref(xkb_atom_t element, xkb_atom_t क्षेत्रम्_2, व्यञ्जकम्_Def *प्रविष्टिः)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ARRAY_REF);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->array_ref.element = element;
    व्यञ्जकम्->array_ref.क्षेत्रम्_2 = क्षेत्रम्_2;
    व्यञ्जकम्->array_ref.प्रविष्टिः = प्रविष्टिः;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_रिक्तानि_List(शून्य)
{
    प्रत्यागम व्यञ्जकम्_Create(STMT_व्यञ्जकम्_रिक्तानि_LIST);
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action(xkb_atom_t name, व्यञ्जकम्_Def *तर्काः)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ACTION_DECL);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->action.name = name;
    व्यञ्जकम्->action.तर्काः = तर्काः;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action_List(व्यञ्जकम्_Def *actions)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ACTION_LIST);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    व्यञ्जकम्->actions.actions = actions;
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym_List(xkb_keysym_t sym)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYSYM_LIST);
    यदि (!व्यञ्जकम्)
        प्रत्यागम रिक्त;
    darray_init(व्यञ्जकम्->keysym_list.syms);
    यदि (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यञ्जकम्->keysym_list.syms, sym);
    }
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Append_कुञ्चिका_Sym_List(व्यञ्जकम्_Def *व्यञ्जकम्, xkb_keysym_t sym)
{
    यदि (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यञ्जकम्->keysym_list.syms, sym);
    }
    प्रत्यागम व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(संरचना scanner *scanner,
                           व्यञ्जकम्_Def *व्यञ्जकम्, ध्रुव वर्ण *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ध्रुव आकार_t len = वाक्यदैर्घ्यम्(string);
    आकार_t idx = 0;
    आकार_t idx_cp = 1;
    यावत् (idx < len) {
        आकार_t गणनम् = 0;
        uint32_t cp = utf8_अग्र_संहिता_point(string + idx, len - idx, &गणनम्);
        यदि (cp == INVALID_UTF8_संहिता_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            गच्छ error;
        }
        ध्रुव xkb_keysym_t sym = xkb_utf32_गन्तव्यम्_keysym(cp);
        यदि (sym == XKB_कुञ्चिका_No_चिह्नम्) {
            scanner_err(scanner, XKB_LOG_सन्देशः_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            गच्छ error;
        }
        darray_append(व्यञ्जकम्->keysym_list.syms, sym);
        idx += गणनम्;
        idx_cp++;
    }
    प्रतिज्ञा(string[idx] == '\0');
    प्रत्यागम व्यञ्जकम्;
error:
    FreeStmt((ParseCommon*) व्यञ्जकम्);
    प्रत्यागम रिक्त;
}

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string)
{
    ध्रुव आकार_t len = वाक्यदैर्घ्यम्(string);
    यदि (len == 0) {
        scanner_err(scanner, XKB_LOG_सन्देशः_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    }
    आकार_t गणनम् = 0;
    ध्रुव uint32_t cp = utf8_अग्र_संहिता_point(string, len, &गणनम्);
    यदि (cp == INVALID_UTF8_संहिता_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    } अन्यथा यदि (गणनम् != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    }
    ध्रुव xkb_keysym_t sym = xkb_utf32_गन्तव्यम्_keysym(cp);
    यदि (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        scanner_err(scanner, XKB_LOG_सन्देशः_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    प्रत्यागम sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मानम्)
{
    KeycodeDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_KEYCODE;
    def->common.अग्र = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    प्रत्यागम def;
}

कुञ्चिका_Alias_Def *
कुञ्चिका_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    कुञ्चिका_Alias_Def *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_ALIAS;
    def->common.अग्र = रिक्त;
    def->alias = alias;
    def->real = real;

    प्रत्यागम def;
}

VModDef *
VModCreate(xkb_atom_t name, व्यञ्जकम्_Def *मानम्)
{
    VModDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_VMOD;
    def->common.अग्र = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    प्रत्यागम def;
}

VarDef *
VarCreate(व्यञ्जकम्_Def *name, व्यञ्जकम्_Def *मानम्)
{
    VarDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_VAR;
    def->common.अग्र = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    प्रत्यागम def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool समुच्चयः)
{
    व्यञ्जकम्_Def *name, *मानम्;
    VarDef *def;
    यदि (!(name = व्यञ्जकम्_Create_Ident(ident))) {
        प्रत्यागम रिक्त;
    }
    यदि (!(मानम् = व्यञ्जकम्_Create_Boolean(समुच्चयः))) {
        FreeStmt((ParseCommon *) name);
        प्रत्यागम रिक्त;
    }
    यदि (!(def = VarCreate(name, मानम्))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) मानम्);
        प्रत्यागम रिक्त;
    }
    प्रत्यागम def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यञ्जकम्_Def *मेलनम्)
{
    InterpDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_INTERP;
    def->common.अग्र = रिक्त;
    def->sym = sym;
    def->मेलनम् = मेलनम्;
    def->def = रिक्त;

    प्रत्यागम def;
}

कुञ्चिका_Type_Def *
कुञ्चिका_Type_Create(xkb_atom_t name, VarDef *body)
{
    कुञ्चिका_Type_Def *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_TYPE;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    प्रत्यागम def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t कुञ्चिका_Name, VarDef *symbols)
{
    SymbolsDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_SYMBOLS;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->कुञ्चिका_Name = कुञ्चिका_Name;
    def->symbols = symbols;

    प्रत्यागम def;
}

गणः_Compat_Def *
गणः_Compat_Create(int64_t गणः, व्यञ्जकम्_Def *val)
{
    गणः_Compat_Def *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_गणः_COMPAT;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->गणः = गणः;
    def->def = val;

    प्रत्यागम def;
}

ModMapDef *
ModMapCreate(व्यञ्जकम्_Def *modifiers, व्यञ्जकम्_Def *कुञ्चिकाः)
{
    ModMapDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_MODMAP;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->कुञ्चिकाः = कुञ्चिकाः;

    प्रत्यागम def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_LED_MAP;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    प्रत्यागम def;
}

LedNameDef *
LedNameCreate(int64_t ndx, व्यञ्जकम्_Def *name, bool आभास)
{
    LedNameDef *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_LED_NAME;
    def->common.अग्र = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->आभास = आभास;

    प्रत्यागम def;
}

UnknownStatement *
UnknownStatementCreate(गणना stmt_type type, संरचना sval name)
{
    UnknownStatement *def = स्मृतिं_देहि(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = type;
    def->common.अग्र = रिक्त;
    def->name = strndup(name.start, name.len);
    यदि (!def->name) {
        मुक्त(def);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम def;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_बहुलकम् merge)
{
    IncludeStmt *incl, *first;
    वर्ण *stmt, *tmp;
    वर्ण nextop;

    incl = first = रिक्त;
    tmp = str;
    stmt = strdup_safe(str);
    यावत् (tmp && *tmp)
    {
        वर्ण *file = रिक्त, *map = रिक्त, *extra_दत्त = रिक्त;

        यदि (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_दत्त))
            गच्छ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        यदि (isempty(file)) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_दत्त);
            अनुवर्त;
        }

        यदि (first == रिक्त) {
            first = incl = स्मृतिं_देहि(परिमाण(*first));
        } अन्यथा {
            incl->अग्र_incl = स्मृतिं_देहि(परिमाण(*first));
            incl = incl->अग्र_incl;
        }

        यदि (!incl) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_दत्त);
            भङ्ग;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.अग्र = रिक्त;
        incl->merge = merge;
        incl->stmt = रिक्त;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_दत्त;
        incl->अग्र_incl = रिक्त;

        विकल्प (nextop) {
        पक्ष MERGE_AUGMENT_उपसर्गः:
            merge = MERGE_AUGMENT;
            भङ्ग;
        पक्ष MERGE_REPLACE_उपसर्गः:
            merge = MERGE_REPLACE;
            भङ्ग;
        स्वभाव:
            merge = MERGE_OVERRIDE;
        }
    }

    यदि (first)
        first->stmt = stmt;
    अन्यथा
        मुक्त(stmt);

    प्रत्यागम first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    मुक्त(stmt);
    प्रत्यागम रिक्त;
}

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags)
{
    XkbFile *file;

    file = शून्यस्मृतिं_देहि(1, परिमाण(*file));
    यदि (!file)
        प्रत्यागम रिक्त;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    प्रत्यागम file;
}

XkbFile *
XkbFileFromComponents(संरचना xkb_context *ctx,
                      ध्रुव संरचना xkb_component_नामानि *kkctgs)
{
    वर्ण *ध्रुव components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    गणना xkb_file_type type;
    IncludeStmt *include = रिक्त;
    XkbFile *file = रिक्त;
    ParseCommon *defs = रिक्त, *defs_चरमः = रिक्त;

    प्रति (type = FIRST_KEYMAP_FILE_TYPE; type <= चरमः_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        यदि (!include)
            गच्छ err;

        file = XkbFileCreate(type, रिक्त, (ParseCommon *) include, 0);
        यदि (!file) {
            FreeInclude(include);
            गच्छ err;
        }

        यदि (!defs)
            defs_चरमः = defs = &file->common;
        अन्यथा
            defs_चरमः = defs_चरमः->अग्र = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, रिक्त, defs, 0);
    यदि (!file)
        गच्छ err;

    प्रत्यागम file;

err:
    FreeXkbFile((XkbFile *) defs);
    प्रत्यागम रिक्त;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *अग्र;

    यावत् (incl)
    {
        अग्र = incl->अग्र_incl;

        मुक्त(incl->file);
        मुक्त(incl->map);
        मुक्त(incl->modifier);
        मुक्त(incl->stmt);

        मुक्त(incl);
        incl = अग्र;
    }
}

शून्य
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *अग्र;

    यावत् (stmt)
    {
        अग्र = stmt->अग्र;

        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = रिक्त;
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_NEGATE:
        पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
        पक्ष STMT_व्यञ्जकम्_NOT:
        पक्ष STMT_व्यञ्जकम्_INVERT:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Unary *) stmt)->child);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_DIVIDE:
        पक्ष STMT_व्यञ्जकम्_सङ्कलय:
        पक्ष STMT_व्यञ्जकम्_व्यवकलय:
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        पक्ष STMT_व्यञ्जकम्_नियोजय:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_द्विमान *) stmt)->वामः);
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_द्विमान *) stmt)->दक्षिणः);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Action *) stmt)->तर्काः);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Action_List *) stmt)->actions);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Array_Ref *) stmt)->प्रविष्टिः);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_KEYSYM_LIST:
            darray_free(((व्यञ्जकम्_Keysym_List *) stmt)->syms);
            भङ्ग;

        पक्ष STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->मानम्);
            भङ्ग;
        पक्ष STMT_TYPE:
            FreeStmt((ParseCommon *) ((कुञ्चिका_Type_Def *) stmt)->body);
            भङ्ग;
        पक्ष STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->मेलनम्);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            भङ्ग;
        पक्ष STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->मानम्);
            भङ्ग;
        पक्ष STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            भङ्ग;
        पक्ष STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->कुञ्चिकाः);
            भङ्ग;
        पक्ष STMT_गणः_COMPAT:
            FreeStmt((ParseCommon *) ((गणः_Compat_Def *) stmt)->def);
            भङ्ग;
        पक्ष STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            भङ्ग;
        पक्ष STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            मुक्त(((UnknownStatement *) stmt)->name);
            भङ्ग;
        स्वभाव:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_अङ्कः_मानानि == 37 &&
                          STMT_अङ्कः_मानानि == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            भङ्ग;
        }

        मुक्त(stmt);
        stmt = अग्र;
    }
}

शून्य
FreeXkbFile(XkbFile *file)
{
    XkbFile *अग्र;

    यावत् (file)
    {
        अग्र = (XkbFile *) file->common.अग्र;

        विकल्प (file->file_type) {
        पक्ष FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            भङ्ग;

        पक्ष FILE_TYPE_TYPES:
        पक्ष FILE_TYPE_COMPAT:
        पक्ष FILE_TYPE_SYMBOLS:
        पक्ष FILE_TYPE_KEYCODES:
        पक्ष FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            भङ्ग;

        स्वभाव:
            भङ्ग;
        }

        मुक्त(file->name);
        मुक्त(file);
        file = अग्र;
    }
}

स्थिर ध्रुव वर्ण *xkb_file_type_वाक्यानि[FILE_TYPE_अङ्कः_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_नियमाः] = "rules",
};

ध्रुव वर्ण *
xkb_file_type_गन्तव्यम्_string(गणना xkb_file_type type)
{
    यदि (type >= FILE_TYPE_अङ्कः_ENTRIES)
        प्रत्यागम "unknown";
    प्रत्यागम xkb_file_type_वाक्यानि[type];
}

स्थिर ध्रुव वर्ण *stmt_type_वाक्यानि[STMT_अङ्कः_मानानि] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_व्यञ्जकम्_STRING_LITERAL] = "string literal expression",
    [STMT_व्यञ्जकम्_INTEGER_LITERAL] = "integer literal expression",
    [STMT_व्यञ्जकम्_FLOAT_LITERAL] = "float literal expression",
    [STMT_व्यञ्जकम्_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_व्यञ्जकम्_KEYNAME_LITERAL] = "key name expression",
    [STMT_व्यञ्जकम्_KEYSYM_LITERAL] = "keysym expression",
    [STMT_व्यञ्जकम्_IDENT] = "identifier expression",
    [STMT_व्यञ्जकम्_ACTION_DECL] = "action declaration expression",
    [STMT_व्यञ्जकम्_क्षेत्रम्_REF] = "field reference expression",
    [STMT_व्यञ्जकम्_ARRAY_REF] = "array reference expression",
    [STMT_व्यञ्जकम्_रिक्तानि_LIST] = "empty list expression",
    [STMT_व्यञ्जकम्_KEYSYM_LIST] = "keysym list expression",
    [STMT_व्यञ्जकम्_ACTION_LIST] = "action list expression",
    [STMT_व्यञ्जकम्_सङ्कलय] = "addition expression",
    [STMT_व्यञ्जकम्_व्यवकलय] = "substraction expression",
    [STMT_व्यञ्जकम्_MULTIPLY] = "multiplication expression",
    [STMT_व्यञ्जकम्_DIVIDE] = "division expression",
    [STMT_व्यञ्जकम्_नियोजय] = "assignment expression",
    [STMT_व्यञ्जकम्_NOT] = "logical negation expression",
    [STMT_व्यञ्जकम्_NEGATE] = "arithmetic negation expression",
    [STMT_व्यञ्जकम्_INVERT] = "bitwise inversion expression",
    [STMT_व्यञ्जकम्_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_गणः_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_अङ्कः_मानानि == 37 &&
              STMT_अङ्कः_मानानि == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ध्रुव वर्ण *
stmt_type_गन्तव्यम्_string(गणना stmt_type type)
{
    यदि (type >= STMT_अङ्कः_मानानि)
        प्रत्यागम रिक्त;
    प्रत्यागम stmt_type_वाक्यानि[type];
}

वर्ण
stmt_type_गन्तव्यम्_operator_char(गणना stmt_type type)
{
    विकल्प (type) {
    पक्ष STMT_व्यञ्जकम्_सङ्कलय:
        प्रत्यागम '+';
    पक्ष STMT_व्यञ्जकम्_व्यवकलय:
        प्रत्यागम '-';
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        प्रत्यागम '*';
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        प्रत्यागम '/';
    पक्ष STMT_व्यञ्जकम्_NOT:
        प्रत्यागम '!';
    पक्ष STMT_व्यञ्जकम्_NEGATE:
        प्रत्यागम '-';
    पक्ष STMT_व्यञ्जकम्_INVERT:
        प्रत्यागम '~';
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
        प्रत्यागम '+';
    स्वभाव:
        प्रत्यागम '\0';
    }
}
