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

#समावेश "config.h"

#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon-keysyms.h"
#समावेश "messages-codes.h"
#समावेश "xkbcomp-priv.h"
#समावेश "ast.h"
#समावेश "ast-build.h"
#समावेश "include.h"
#समावेश "keysym.h"
#समावेश "utf8-decoding.h"

अटल व्यंजक_Def *
व्यंजक_Create(क्रमागत stmt_type op)
{
    व्यंजक_Def *व्यंजक = दो_स्मृति(माप(*व्यंजक));
    अगर (!व्यंजक)
        वापस शून्य;

    व्यंजक->common.type = op;
    व्यंजक->common.अग्र = शून्य;

    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_String(xkb_atom_t str)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_STRING_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->string.str = str;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Integer(int64_t ival)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_INTEGER_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->integer.ival = ival;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Float(व्योम)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_FLOAT_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Boolean(bool set)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_BOOLEAN_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->boolean.set = set;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Name(xkb_atom_t कुंजी_name)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYNAME_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->कुंजी_name.कुंजी_name = कुंजी_name;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym(xkb_keysym_t keysym)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYSYM_LITERAL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->keysym.keysym = keysym;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Ident(xkb_atom_t ident)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_IDENT);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->ident.ident = ident;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Unary(क्रमागत stmt_type op, व्यंजक_Def *child)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(op);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->unary.child = child;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_द्विआधारी(क्रमागत stmt_type op, व्यंजक_Def *बायाँ, व्यंजक_Def *दायाँ)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(op);
    अगर (!व्यंजक)
        वापस शून्य;

    व्यंजक->द्विआधारी.बायाँ = बायाँ;
    व्यंजक->द्विआधारी.दायाँ = दायाँ;

    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_FIELD_REF);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->field_ref.element = element;
    व्यंजक->field_ref.field = field;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, व्यंजक_Def *entry)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ARRAY_REF);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->array_ref.element = element;
    व्यंजक->array_ref.field = field;
    व्यंजक->array_ref.entry = entry;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Empty_List(व्योम)
{
    वापस व्यंजक_Create(STMT_व्यंजक_EMPTY_LIST);
}

व्यंजक_Def *
व्यंजक_Create_Action(xkb_atom_t name, व्यंजक_Def *args)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ACTION_DECL);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->action.name = name;
    व्यंजक->action.args = args;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Action_List(व्यंजक_Def *actions)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ACTION_LIST);
    अगर (!व्यंजक)
        वापस शून्य;
    व्यंजक->actions.actions = actions;
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym_List(xkb_keysym_t sym)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYSYM_LIST);
    अगर (!व्यंजक)
        वापस शून्य;
    darray_init(व्यंजक->keysym_list.syms);
    अगर (sym == XKB_कुंजी_No_प्रतीक) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यंजक->keysym_list.syms, sym);
    }
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_Append_कुंजी_Sym_List(व्यंजक_Def *व्यंजक, xkb_keysym_t sym)
{
    अगर (sym == XKB_कुंजी_No_प्रतीक) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यंजक->keysym_list.syms, sym);
    }
    वापस व्यंजक;
}

व्यंजक_Def *
व्यंजक_कुंजी_Sym_List_Append_String(काष्ठा scanner *scanner,
                           व्यंजक_Def *व्यंजक, स्थिर अक्षर *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    स्थिर माप_प्रकार len = म_माप(string);
    माप_प्रकार idx = 0;
    माप_प्रकार idx_cp = 1;
    जबतक (idx < len) {
        माप_प्रकार गिनती = 0;
        uint32_t cp = utf8_अग्र_code_point(string + idx, len - idx, &गिनती);
        अगर (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            जाओ error;
        }
        स्थिर xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        अगर (sym == XKB_कुंजी_No_प्रतीक) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            जाओ error;
        }
        darray_append(व्यंजक->keysym_list.syms, sym);
        idx += गिनती;
        idx_cp++;
    }
    निश्चित(string[idx] == '\0');
    वापस व्यंजक;
error:
    FreeStmt((ParseCommon*) व्यंजक);
    वापस शून्य;
}

xkb_keysym_t
KeysymParseString(काष्ठा scanner *scanner, स्थिर अक्षर *string)
{
    स्थिर माप_प्रकार len = म_माप(string);
    अगर (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        वापस XKB_कुंजी_No_प्रतीक;
    }
    माप_प्रकार गिनती = 0;
    स्थिर uint32_t cp = utf8_अग्र_code_point(string, len, &गिनती);
    अगर (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        वापस XKB_कुंजी_No_प्रतीक;
    } अन्यथा अगर (गिनती != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        वापस XKB_कुंजी_No_प्रतीक;
    }
    स्थिर xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    अगर (sym == XKB_कुंजी_No_प्रतीक) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    वापस sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मूल्य)
{
    KeycodeDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_KEYCODE;
    def->common.अग्र = शून्य;
    def->name = name;
    def->मूल्य = मूल्य;

    वापस def;
}

कुंजी_Alias_Def *
कुंजी_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    कुंजी_Alias_Def *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_ALIAS;
    def->common.अग्र = शून्य;
    def->alias = alias;
    def->real = real;

    वापस def;
}

VModDef *
VModCreate(xkb_atom_t name, व्यंजक_Def *मूल्य)
{
    VModDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_VMOD;
    def->common.अग्र = शून्य;
    def->name = name;
    def->मूल्य = मूल्य;

    वापस def;
}

VarDef *
VarCreate(व्यंजक_Def *name, व्यंजक_Def *मूल्य)
{
    VarDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_VAR;
    def->common.अग्र = शून्य;
    def->name = name;
    def->मूल्य = मूल्य;

    वापस def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    व्यंजक_Def *name, *मूल्य;
    VarDef *def;
    अगर (!(name = व्यंजक_Create_Ident(ident))) {
        वापस शून्य;
    }
    अगर (!(मूल्य = व्यंजक_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        वापस शून्य;
    }
    अगर (!(def = VarCreate(name, मूल्य))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) मूल्य);
        वापस शून्य;
    }
    वापस def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यंजक_Def *match)
{
    InterpDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_INTERP;
    def->common.अग्र = शून्य;
    def->sym = sym;
    def->match = match;
    def->def = शून्य;

    वापस def;
}

कुंजी_Type_Def *
कुंजी_Type_Create(xkb_atom_t name, VarDef *body)
{
    कुंजी_Type_Def *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_TYPE;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    वापस def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t कुंजी_Name, VarDef *symbols)
{
    SymbolsDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_SYMBOLS;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->कुंजी_Name = कुंजी_Name;
    def->symbols = symbols;

    वापस def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, व्यंजक_Def *val)
{
    GroupCompatDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    वापस def;
}

ModMapDef *
ModMapCreate(व्यंजक_Def *modifiers, व्यंजक_Def *कुंजियाँ)
{
    ModMapDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_MODMAP;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->कुंजियाँ = कुंजियाँ;

    वापस def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_LED_MAP;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    वापस def;
}

LedNameDef *
LedNameCreate(int64_t ndx, व्यंजक_Def *name, bool भव)
{
    LedNameDef *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = STMT_LED_NAME;
    def->common.अग्र = शून्य;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->भव = भव;

    वापस def;
}

UnknownStatement *
UnknownStatementCreate(क्रमागत stmt_type type, काष्ठा sval name)
{
    UnknownStatement *def = दो_स्मृति(माप(*def));
    अगर (!def)
        वापस शून्य;

    def->common.type = type;
    def->common.अग्र = शून्य;
    def->name = strndup(name.start, name.len);
    अगर (!def->name) {
        मुक्त(def);
        वापस शून्य;
    }

    वापस def;
}

अटल व्योम
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(काष्ठा xkb_context *ctx, अक्षर *str, क्रमागत merge_mode merge)
{
    IncludeStmt *incl, *first;
    अक्षर *stmt, *tmp;
    अक्षर nextop;

    incl = first = शून्य;
    tmp = str;
    stmt = strdup_safe(str);
    जबतक (tmp && *tmp)
    {
        अक्षर *file = शून्य, *map = शून्य, *extra_तथ्य = शून्य;

        अगर (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_तथ्य))
            जाओ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        अगर (isempty(file)) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_तथ्य);
            जारी;
        }

        अगर (first == शून्य) {
            first = incl = दो_स्मृति(माप(*first));
        } अन्यथा {
            incl->अग्र_incl = दो_स्मृति(माप(*first));
            incl = incl->अग्र_incl;
        }

        अगर (!incl) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_तथ्य);
            अवरोध;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.अग्र = शून्य;
        incl->merge = merge;
        incl->stmt = शून्य;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_तथ्य;
        incl->अग्र_incl = शून्य;

        चयन (nextop) {
        हाल MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            अवरोध;
        हाल MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            अवरोध;
        शेष:
            merge = MERGE_OVERRIDE;
        }
    }

    अगर (first)
        first->stmt = stmt;
    अन्यथा
        मुक्त(stmt);

    वापस first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    मुक्त(stmt);
    वापस शून्य;
}

XkbFile *
XkbFileCreate(क्रमागत xkb_file_type type, अक्षर *name, ParseCommon *defs,
              क्रमागत xkb_map_flags flags)
{
    XkbFile *file;

    file = सुस्मृति(1, माप(*file));
    अगर (!file)
        वापस शून्य;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    वापस file;
}

XkbFile *
XkbFileFromComponents(काष्ठा xkb_context *ctx,
                      स्थिर काष्ठा xkb_component_names *kkctgs)
{
    अक्षर *स्थिर components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    क्रमागत xkb_file_type type;
    IncludeStmt *include = शून्य;
    XkbFile *file = शून्य;
    ParseCommon *defs = शून्य, *defsLast = शून्य;

    क्रम (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        अगर (!include)
            जाओ err;

        file = XkbFileCreate(type, शून्य, (ParseCommon *) include, 0);
        अगर (!file) {
            FreeInclude(include);
            जाओ err;
        }

        अगर (!defs)
            defsLast = defs = &file->common;
        अन्यथा
            defsLast = defsLast->अग्र = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, शून्य, defs, 0);
    अगर (!file)
        जाओ err;

    वापस file;

err:
    FreeXkbFile((XkbFile *) defs);
    वापस शून्य;
}

अटल व्योम
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *अग्र;

    जबतक (incl)
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

व्योम
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *अग्र;

    जबतक (stmt)
    {
        अग्र = stmt->अग्र;

        चयन (stmt->type) {
        हाल STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = शून्य;
            अवरोध;

        हाल STMT_व्यंजक_NEGATE:
        हाल STMT_व्यंजक_UNARY_PLUS:
        हाल STMT_व्यंजक_NOT:
        हाल STMT_व्यंजक_INVERT:
            FreeStmt((ParseCommon *) ((व्यंजक_Unary *) stmt)->child);
            अवरोध;

        हाल STMT_व्यंजक_DIVIDE:
        हाल STMT_व्यंजक_ADD:
        हाल STMT_व्यंजक_SUBTRACT:
        हाल STMT_व्यंजक_MULTIPLY:
        हाल STMT_व्यंजक_ASSIGN:
            FreeStmt((ParseCommon *) ((व्यंजक_द्विआधारी *) stmt)->बायाँ);
            FreeStmt((ParseCommon *) ((व्यंजक_द्विआधारी *) stmt)->दायाँ);
            अवरोध;

        हाल STMT_व्यंजक_ACTION_DECL:
            FreeStmt((ParseCommon *) ((व्यंजक_Action *) stmt)->args);
            अवरोध;

        हाल STMT_व्यंजक_ACTION_LIST:
            FreeStmt((ParseCommon *) ((व्यंजक_Action_List *) stmt)->actions);
            अवरोध;

        हाल STMT_व्यंजक_ARRAY_REF:
            FreeStmt((ParseCommon *) ((व्यंजक_Array_Ref *) stmt)->entry);
            अवरोध;

        हाल STMT_व्यंजक_KEYSYM_LIST:
            darray_free(((व्यंजक_Keysym_List *) stmt)->syms);
            अवरोध;

        हाल STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->मूल्य);
            अवरोध;
        हाल STMT_TYPE:
            FreeStmt((ParseCommon *) ((कुंजी_Type_Def *) stmt)->body);
            अवरोध;
        हाल STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            अवरोध;
        हाल STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->मूल्य);
            अवरोध;
        हाल STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            अवरोध;
        हाल STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->कुंजियाँ);
            अवरोध;
        हाल STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            अवरोध;
        हाल STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            अवरोध;
        हाल STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            अवरोध;
        हाल STMT_UNKNOWN_DECLARATION:
        हाल STMT_UNKNOWN_COMPOUND:
            मुक्त(((UnknownStatement *) stmt)->name);
            अवरोध;
        शेष:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_अंक_मूल्यावली == 37 &&
                          STMT_अंक_मूल्यावली == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            अवरोध;
        }

        मुक्त(stmt);
        stmt = अग्र;
    }
}

व्योम
FreeXkbFile(XkbFile *file)
{
    XkbFile *अग्र;

    जबतक (file)
    {
        अग्र = (XkbFile *) file->common.अग्र;

        चयन (file->file_type) {
        हाल FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            अवरोध;

        हाल FILE_TYPE_TYPES:
        हाल FILE_TYPE_COMPAT:
        हाल FILE_TYPE_SYMBOLS:
        हाल FILE_TYPE_KEYCODES:
        हाल FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            अवरोध;

        शेष:
            अवरोध;
        }

        मुक्त(file->name);
        मुक्त(file);
        file = अग्र;
    }
}

अटल स्थिर अक्षर *xkb_file_type_strings[FILE_TYPE_अंक_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

स्थिर अक्षर *
xkb_file_type_to_string(क्रमागत xkb_file_type type)
{
    अगर (type >= FILE_TYPE_अंक_ENTRIES)
        वापस "unknown";
    वापस xkb_file_type_strings[type];
}

अटल स्थिर अक्षर *stmt_type_strings[STMT_अंक_मूल्यावली] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_व्यंजक_STRING_LITERAL] = "string literal expression",
    [STMT_व्यंजक_INTEGER_LITERAL] = "integer literal expression",
    [STMT_व्यंजक_FLOAT_LITERAL] = "float literal expression",
    [STMT_व्यंजक_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_व्यंजक_KEYNAME_LITERAL] = "key name expression",
    [STMT_व्यंजक_KEYSYM_LITERAL] = "keysym expression",
    [STMT_व्यंजक_IDENT] = "identifier expression",
    [STMT_व्यंजक_ACTION_DECL] = "action declaration expression",
    [STMT_व्यंजक_FIELD_REF] = "field reference expression",
    [STMT_व्यंजक_ARRAY_REF] = "array reference expression",
    [STMT_व्यंजक_EMPTY_LIST] = "empty list expression",
    [STMT_व्यंजक_KEYSYM_LIST] = "keysym list expression",
    [STMT_व्यंजक_ACTION_LIST] = "action list expression",
    [STMT_व्यंजक_ADD] = "addition expression",
    [STMT_व्यंजक_SUBTRACT] = "substraction expression",
    [STMT_व्यंजक_MULTIPLY] = "multiplication expression",
    [STMT_व्यंजक_DIVIDE] = "division expression",
    [STMT_व्यंजक_ASSIGN] = "assignment expression",
    [STMT_व्यंजक_NOT] = "logical negation expression",
    [STMT_व्यंजक_NEGATE] = "arithmetic negation expression",
    [STMT_व्यंजक_INVERT] = "bitwise inversion expression",
    [STMT_व्यंजक_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_अंक_मूल्यावली == 37 &&
              STMT_अंक_मूल्यावली == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

स्थिर अक्षर *
stmt_type_to_string(क्रमागत stmt_type type)
{
    अगर (type >= STMT_अंक_मूल्यावली)
        वापस शून्य;
    वापस stmt_type_strings[type];
}

अक्षर
stmt_type_to_operator_char(क्रमागत stmt_type type)
{
    चयन (type) {
    हाल STMT_व्यंजक_ADD:
        वापस '+';
    हाल STMT_व्यंजक_SUBTRACT:
        वापस '-';
    हाल STMT_व्यंजक_MULTIPLY:
        वापस '*';
    हाल STMT_व्यंजक_DIVIDE:
        वापस '/';
    हाल STMT_व्यंजक_NOT:
        वापस '!';
    हाल STMT_व्यंजक_NEGATE:
        वापस '-';
    हाल STMT_व्यंजक_INVERT:
        वापस '~';
    हाल STMT_व्यंजक_UNARY_PLUS:
        वापस '+';
    शेष:
        वापस '\0';
    }
}
