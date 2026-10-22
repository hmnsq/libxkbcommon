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
    व्यञ्जकम्_Def *व्यञ्जकम् = malloc(परिमाण(*व्यञ्जकम्));
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;

    व्यञ्जकम्->common.type = op;
    व्यञ्जकम्->common.next = रिक्त;

    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_String(xkb_atom_t str)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_STRING_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->string.str = str;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Integer(int64_t ival)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_INTEGER_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->integer.ival = ival;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Float(शून्य)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_FLOAT_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Boolean(bool set)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_BOOLEAN_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->boolean.set = set;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Name(xkb_atom_t कुञ्चिका_name)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYNAME_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name = कुञ्चिका_name;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym(xkb_keysym_t keysym)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYSYM_LITERAL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->keysym.keysym = keysym;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Ident(xkb_atom_t ident)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_IDENT);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->ident.ident = ident;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Unary(गणना stmt_type op, व्यञ्जकम्_Def *child)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(op);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->unary.child = child;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_द्विमान(गणना stmt_type op, व्यञ्जकम्_Def *वामः, व्यञ्जकम्_Def *दक्षिणः)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(op);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;

    व्यञ्जकम्->द्विमान.वामः = वामः;
    व्यञ्जकम्->द्विमान.दक्षिणः = दक्षिणः;

    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_FIELD_REF);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->field_ref.element = element;
    व्यञ्जकम्->field_ref.field = field;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, व्यञ्जकम्_Def *entry)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ARRAY_REF);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->array_ref.element = element;
    व्यञ्जकम्->array_ref.field = field;
    व्यञ्जकम्->array_ref.entry = entry;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Empty_List(शून्य)
{
    पटिनिवत्त व्यञ्जकम्_Create(STMT_व्यञ्जकम्_EMPTY_LIST);
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action(xkb_atom_t name, व्यञ्जकम्_Def *args)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ACTION_DECL);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->action.name = name;
    व्यञ्जकम्->action.args = args;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_Action_List(व्यञ्जकम्_Def *actions)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_ACTION_LIST);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    व्यञ्जकम्->actions.actions = actions;
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Create_कुञ्चिका_Sym_List(xkb_keysym_t sym)
{
    व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create(STMT_व्यञ्जकम्_KEYSYM_LIST);
    सचे (!व्यञ्जकम्)
        पटिनिवत्त रिक्त;
    darray_init(व्यञ्जकम्->keysym_list.syms);
    सचे (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        /* Discard NoSymbol */
    } अञ्ञथा {
        darray_append(व्यञ्जकम्->keysym_list.syms, sym);
    }
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_Append_कुञ्चिका_Sym_List(व्यञ्जकम्_Def *व्यञ्जकम्, xkb_keysym_t sym)
{
    सचे (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        /* Discard NoSymbol */
    } अञ्ञथा {
        darray_append(व्यञ्जकम्->keysym_list.syms, sym);
    }
    पटिनिवत्त व्यञ्जकम्;
}

व्यञ्जकम्_Def *
व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(संरचना scanner *scanner,
                           व्यञ्जकम्_Def *व्यञ्जकम्, ध्रुव वर्ण *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ध्रुव size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    याव (idx < len) {
        size_t गणनम् = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &गणनम्);
        सचे (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            गच्छ error;
        }
        ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        सचे (sym == XKB_कुञ्चिका_No_चिह्नम्) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
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
    पटिनिवत्त व्यञ्जकम्;
error:
    FreeStmt((ParseCommon*) व्यञ्जकम्);
    पटिनिवत्त रिक्त;
}

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string)
{
    ध्रुव size_t len = strlen(string);
    सचे (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    }
    size_t गणनम् = 0;
    ध्रुव uint32_t cp = utf8_next_code_point(string, len, &गणनम्);
    सचे (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    } अञ्ञथा सचे (गणनम् != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    }
    ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    सचे (sym == XKB_कुञ्चिका_No_चिह्नम्) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    पटिनिवत्त sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मानम्)
{
    KeycodeDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_KEYCODE;
    def->common.next = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    पटिनिवत्त def;
}

कुञ्चिका_Alias_Def *
कुञ्चिका_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    कुञ्चिका_Alias_Def *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_ALIAS;
    def->common.next = रिक्त;
    def->alias = alias;
    def->real = real;

    पटिनिवत्त def;
}

VModDef *
VModCreate(xkb_atom_t name, व्यञ्जकम्_Def *मानम्)
{
    VModDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_VMOD;
    def->common.next = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    पटिनिवत्त def;
}

VarDef *
VarCreate(व्यञ्जकम्_Def *name, व्यञ्जकम्_Def *मानम्)
{
    VarDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_VAR;
    def->common.next = रिक्त;
    def->name = name;
    def->मानम् = मानम्;

    पटिनिवत्त def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    व्यञ्जकम्_Def *name, *मानम्;
    VarDef *def;
    सचे (!(name = व्यञ्जकम्_Create_Ident(ident))) {
        पटिनिवत्त रिक्त;
    }
    सचे (!(मानम् = व्यञ्जकम्_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        पटिनिवत्त रिक्त;
    }
    सचे (!(def = VarCreate(name, मानम्))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) मानम्);
        पटिनिवत्त रिक्त;
    }
    पटिनिवत्त def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यञ्जकम्_Def *match)
{
    InterpDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_INTERP;
    def->common.next = रिक्त;
    def->sym = sym;
    def->match = match;
    def->def = रिक्त;

    पटिनिवत्त def;
}

कुञ्चिका_Type_Def *
कुञ्चिका_Type_Create(xkb_atom_t name, VarDef *body)
{
    कुञ्चिका_Type_Def *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_TYPE;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    पटिनिवत्त def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t कुञ्चिका_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_SYMBOLS;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->कुञ्चिका_Name = कुञ्चिका_Name;
    def->symbols = symbols;

    पटिनिवत्त def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, व्यञ्जकम्_Def *val)
{
    GroupCompatDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    पटिनिवत्त def;
}

ModMapDef *
ModMapCreate(व्यञ्जकम्_Def *modifiers, व्यञ्जकम्_Def *कुञ्चिकाः)
{
    ModMapDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_MODMAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->कुञ्चिकाः = कुञ्चिकाः;

    पटिनिवत्त def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_LED_MAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    पटिनिवत्त def;
}

LedNameDef *
LedNameCreate(int64_t ndx, व्यञ्जकम्_Def *name, bool आभास)
{
    LedNameDef *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = STMT_LED_NAME;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->आभास = आभास;

    पटिनिवत्त def;
}

UnknownStatement *
UnknownStatementCreate(गणना stmt_type type, संरचना sval name)
{
    UnknownStatement *def = malloc(परिमाण(*def));
    सचे (!def)
        पटिनिवत्त रिक्त;

    def->common.type = type;
    def->common.next = रिक्त;
    def->name = strndup(name.start, name.len);
    सचे (!def->name) {
        मुक्त(def);
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त def;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge)
{
    IncludeStmt *incl, *first;
    वर्ण *stmt, *tmp;
    वर्ण nextop;

    incl = first = रिक्त;
    tmp = str;
    stmt = strdup_safe(str);
    याव (tmp && *tmp)
    {
        वर्ण *file = रिक्त, *map = रिक्त, *extra_data = रिक्त;

        सचे (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            गच्छ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        सचे (isempty(file)) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_data);
            अनुवर्त;
        }

        सचे (first == रिक्त) {
            first = incl = malloc(परिमाण(*first));
        } अञ्ञथा {
            incl->next_incl = malloc(परिमाण(*first));
            incl = incl->next_incl;
        }

        सचे (!incl) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_data);
            भङ्ग;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = रिक्त;
        incl->merge = merge;
        incl->stmt = रिक्त;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = रिक्त;

        विकल्प (nextop) {
        पक्ष MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            भङ्ग;
        पक्ष MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            भङ्ग;
        स्वभाव:
            merge = MERGE_OVERRIDE;
        }
    }

    सचे (first)
        first->stmt = stmt;
    अञ्ञथा
        मुक्त(stmt);

    पटिनिवत्त first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    मुक्त(stmt);
    पटिनिवत्त रिक्त;
}

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, परिमाण(*file));
    सचे (!file)
        पटिनिवत्त रिक्त;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    पटिनिवत्त file;
}

XkbFile *
XkbFileFromComponents(संरचना xkb_context *ctx,
                      ध्रुव संरचना xkb_component_names *kkctgs)
{
    वर्ण *ध्रुव components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    गणना xkb_file_type type;
    IncludeStmt *include = रिक्त;
    XkbFile *file = रिक्त;
    ParseCommon *defs = रिक्त, *defsLast = रिक्त;

    अत्थाय (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        सचे (!include)
            गच्छ err;

        file = XkbFileCreate(type, रिक्त, (ParseCommon *) include, 0);
        सचे (!file) {
            FreeInclude(include);
            गच्छ err;
        }

        सचे (!defs)
            defsLast = defs = &file->common;
        अञ्ञथा
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, रिक्त, defs, 0);
    सचे (!file)
        गच्छ err;

    पटिनिवत्त file;

err:
    FreeXkbFile((XkbFile *) defs);
    पटिनिवत्त रिक्त;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    याव (incl)
    {
        next = incl->next_incl;

        मुक्त(incl->file);
        मुक्त(incl->map);
        मुक्त(incl->modifier);
        मुक्त(incl->stmt);

        मुक्त(incl);
        incl = next;
    }
}

शून्य
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    याव (stmt)
    {
        next = stmt->next;

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
        पक्ष STMT_व्यञ्जकम्_ADD:
        पक्ष STMT_व्यञ्जकम्_SUBTRACT:
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        पक्ष STMT_व्यञ्जकम्_ASSIGN:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_द्विमान *) stmt)->वामः);
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_द्विमान *) stmt)->दक्षिणः);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Action *) stmt)->args);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Action_List *) stmt)->actions);
            भङ्ग;

        पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
            FreeStmt((ParseCommon *) ((व्यञ्जकम्_Array_Ref *) stmt)->entry);
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
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
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
        पक्ष STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
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
        stmt = next;
    }
}

शून्य
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    याव (file)
    {
        next = (XkbFile *) file->common.next;

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
        file = next;
    }
}

स्थिर ध्रुव वर्ण *xkb_file_type_strings[FILE_TYPE_अङ्कः_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ध्रुव वर्ण *
xkb_file_type_to_string(गणना xkb_file_type type)
{
    सचे (type >= FILE_TYPE_अङ्कः_ENTRIES)
        पटिनिवत्त "unknown";
    पटिनिवत्त xkb_file_type_strings[type];
}

स्थिर ध्रुव वर्ण *stmt_type_strings[STMT_अङ्कः_मानानि] = {
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
    [STMT_व्यञ्जकम्_FIELD_REF] = "field reference expression",
    [STMT_व्यञ्जकम्_ARRAY_REF] = "array reference expression",
    [STMT_व्यञ्जकम्_EMPTY_LIST] = "empty list expression",
    [STMT_व्यञ्जकम्_KEYSYM_LIST] = "keysym list expression",
    [STMT_व्यञ्जकम्_ACTION_LIST] = "action list expression",
    [STMT_व्यञ्जकम्_ADD] = "addition expression",
    [STMT_व्यञ्जकम्_SUBTRACT] = "substraction expression",
    [STMT_व्यञ्जकम्_MULTIPLY] = "multiplication expression",
    [STMT_व्यञ्जकम्_DIVIDE] = "division expression",
    [STMT_व्यञ्जकम्_ASSIGN] = "assignment expression",
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
    [STMT_GROUP_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_अङ्कः_मानानि == 37 &&
              STMT_अङ्कः_मानानि == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ध्रुव वर्ण *
stmt_type_to_string(गणना stmt_type type)
{
    सचे (type >= STMT_अङ्कः_मानानि)
        पटिनिवत्त रिक्त;
    पटिनिवत्त stmt_type_strings[type];
}

वर्ण
stmt_type_to_operator_char(गणना stmt_type type)
{
    विकल्प (type) {
    पक्ष STMT_व्यञ्जकम्_ADD:
        पटिनिवत्त '+';
    पक्ष STMT_व्यञ्जकम्_SUBTRACT:
        पटिनिवत्त '-';
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        पटिनिवत्त '*';
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        पटिनिवत्त '/';
    पक्ष STMT_व्यञ्जकम्_NOT:
        पटिनिवत्त '!';
    पक्ष STMT_व्यञ्जकम्_NEGATE:
        पटिनिवत्त '-';
    पक्ष STMT_व्यञ्जकम्_INVERT:
        पटिनिवत्त '~';
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
        पटिनिवत्त '+';
    स्वभाव:
        पटिनिवत्त '\0';
    }
}
