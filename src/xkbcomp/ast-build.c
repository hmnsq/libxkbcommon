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

स्थिर व्यंजक_Def *
व्यंजक_Create(गणना stmt_type op)
{
    व्यंजक_Def *व्यंजक = malloc(परिमाण(*व्यंजक));
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;

    व्यंजक->common.type = op;
    व्यंजक->common.next = रिक्त;

    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_String(xkb_atom_t str)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_STRING_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->string.str = str;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Integer(int64_t ival)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_INTEGER_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->integer.ival = ival;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Float(शून्य)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_FLOAT_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Boolean(bool set)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_BOOLEAN_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->boolean.set = set;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Name(xkb_atom_t कुंजी_name)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYNAME_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->कुंजी_name.कुंजी_name = कुंजी_name;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym(xkb_keysym_t keysym)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYSYM_LITERAL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->keysym.keysym = keysym;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Ident(xkb_atom_t ident)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_IDENT);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->ident.ident = ident;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Unary(गणना stmt_type op, व्यंजक_Def *child)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(op);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->unary.child = child;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_द्विआधारी(गणना stmt_type op, व्यंजक_Def *बायाँ, व्यंजक_Def *दायाँ)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(op);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;

    व्यंजक->द्विआधारी.बायाँ = बायाँ;
    व्यंजक->द्विआधारी.दायाँ = दायाँ;

    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_FIELD_REF);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->field_ref.element = element;
    व्यंजक->field_ref.field = field;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, व्यंजक_Def *entry)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ARRAY_REF);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->array_ref.element = element;
    व्यंजक->array_ref.field = field;
    व्यंजक->array_ref.entry = entry;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Empty_List(शून्य)
{
    प्रत्यागम व्यंजक_Create(STMT_व्यंजक_EMPTY_LIST);
}

व्यंजक_Def *
व्यंजक_Create_Action(xkb_atom_t name, व्यंजक_Def *args)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ACTION_DECL);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->action.name = name;
    व्यंजक->action.args = args;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_Action_List(व्यंजक_Def *actions)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_ACTION_LIST);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    व्यंजक->actions.actions = actions;
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Create_कुंजी_Sym_List(xkb_keysym_t sym)
{
    व्यंजक_Def *व्यंजक = व्यंजक_Create(STMT_व्यंजक_KEYSYM_LIST);
    यदि (!व्यंजक)
        प्रत्यागम रिक्त;
    darray_init(व्यंजक->keysym_list.syms);
    यदि (sym == XKB_कुंजी_No_प्रतीक) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यंजक->keysym_list.syms, sym);
    }
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_Append_कुंजी_Sym_List(व्यंजक_Def *व्यंजक, xkb_keysym_t sym)
{
    यदि (sym == XKB_कुंजी_No_प्रतीक) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(व्यंजक->keysym_list.syms, sym);
    }
    प्रत्यागम व्यंजक;
}

व्यंजक_Def *
व्यंजक_कुंजी_Sym_List_Append_String(संरचना scanner *scanner,
                           व्यंजक_Def *व्यंजक, ध्रुव वर्ण *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ध्रुव size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    यावत् (idx < len) {
        size_t गिनती = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &गिनती);
        यदि (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            गच्छ error;
        }
        ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        यदि (sym == XKB_कुंजी_No_प्रतीक) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            गच्छ error;
        }
        darray_append(व्यंजक->keysym_list.syms, sym);
        idx += गिनती;
        idx_cp++;
    }
    प्रतिज्ञा(string[idx] == '\0');
    प्रत्यागम व्यंजक;
error:
    FreeStmt((ParseCommon*) व्यंजक);
    प्रत्यागम रिक्त;
}

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string)
{
    ध्रुव size_t len = strlen(string);
    यदि (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        प्रत्यागम XKB_कुंजी_No_प्रतीक;
    }
    size_t गिनती = 0;
    ध्रुव uint32_t cp = utf8_next_code_point(string, len, &गिनती);
    यदि (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        प्रत्यागम XKB_कुंजी_No_प्रतीक;
    } अन्यथा यदि (गिनती != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        प्रत्यागम XKB_कुंजी_No_प्रतीक;
    }
    ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    यदि (sym == XKB_कुंजी_No_प्रतीक) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    प्रत्यागम sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मूल्य)
{
    KeycodeDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_KEYCODE;
    def->common.next = रिक्त;
    def->name = name;
    def->मूल्य = मूल्य;

    प्रत्यागम def;
}

कुंजी_Alias_Def *
कुंजी_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    कुंजी_Alias_Def *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_ALIAS;
    def->common.next = रिक्त;
    def->alias = alias;
    def->real = real;

    प्रत्यागम def;
}

VModDef *
VModCreate(xkb_atom_t name, व्यंजक_Def *मूल्य)
{
    VModDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_VMOD;
    def->common.next = रिक्त;
    def->name = name;
    def->मूल्य = मूल्य;

    प्रत्यागम def;
}

VarDef *
VarCreate(व्यंजक_Def *name, व्यंजक_Def *मूल्य)
{
    VarDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_VAR;
    def->common.next = रिक्त;
    def->name = name;
    def->मूल्य = मूल्य;

    प्रत्यागम def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    व्यंजक_Def *name, *मूल्य;
    VarDef *def;
    यदि (!(name = व्यंजक_Create_Ident(ident))) {
        प्रत्यागम रिक्त;
    }
    यदि (!(मूल्य = व्यंजक_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        प्रत्यागम रिक्त;
    }
    यदि (!(def = VarCreate(name, मूल्य))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) मूल्य);
        प्रत्यागम रिक्त;
    }
    प्रत्यागम def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, व्यंजक_Def *match)
{
    InterpDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_INTERP;
    def->common.next = रिक्त;
    def->sym = sym;
    def->match = match;
    def->def = रिक्त;

    प्रत्यागम def;
}

कुंजी_Type_Def *
कुंजी_Type_Create(xkb_atom_t name, VarDef *body)
{
    कुंजी_Type_Def *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_TYPE;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    प्रत्यागम def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t कुंजी_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_SYMBOLS;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->कुंजी_Name = कुंजी_Name;
    def->symbols = symbols;

    प्रत्यागम def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, व्यंजक_Def *val)
{
    GroupCompatDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    प्रत्यागम def;
}

ModMapDef *
ModMapCreate(व्यंजक_Def *modifiers, व्यंजक_Def *कुंजियाँ)
{
    ModMapDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_MODMAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->कुंजियाँ = कुंजियाँ;

    प्रत्यागम def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_LED_MAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    प्रत्यागम def;
}

LedNameDef *
LedNameCreate(int64_t ndx, व्यंजक_Def *name, bool आभास)
{
    LedNameDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_LED_NAME;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->आभास = आभास;

    प्रत्यागम def;
}

UnknownStatement *
UnknownStatementCreate(गणना stmt_type type, संरचना sval name)
{
    UnknownStatement *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = type;
    def->common.next = रिक्त;
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
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge)
{
    IncludeStmt *incl, *first;
    वर्ण *stmt, *tmp;
    वर्ण nextop;

    incl = first = रिक्त;
    tmp = str;
    stmt = strdup_safe(str);
    यावत् (tmp && *tmp)
    {
        वर्ण *file = रिक्त, *map = रिक्त, *extra_data = रिक्त;

        यदि (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
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
            मुक्त(extra_data);
            अनुवर्त;
        }

        यदि (first == रिक्त) {
            first = incl = malloc(परिमाण(*first));
        } अन्यथा {
            incl->next_incl = malloc(परिमाण(*first));
            incl = incl->next_incl;
        }

        यदि (!incl) {
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

    file = calloc(1, परिमाण(*file));
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

    प्रति (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        यदि (!include)
            गच्छ err;

        file = XkbFileCreate(type, रिक्त, (ParseCommon *) include, 0);
        यदि (!file) {
            FreeInclude(include);
            गच्छ err;
        }

        यदि (!defs)
            defsLast = defs = &file->common;
        अन्यथा
            defsLast = defsLast->next = &file->common;
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
    IncludeStmt *next;

    यावत् (incl)
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

    यावत् (stmt)
    {
        next = stmt->next;

        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = रिक्त;
            भङ्ग;

        पक्ष STMT_व्यंजक_NEGATE:
        पक्ष STMT_व्यंजक_UNARY_PLUS:
        पक्ष STMT_व्यंजक_NOT:
        पक्ष STMT_व्यंजक_INVERT:
            FreeStmt((ParseCommon *) ((व्यंजक_Unary *) stmt)->child);
            भङ्ग;

        पक्ष STMT_व्यंजक_DIVIDE:
        पक्ष STMT_व्यंजक_ADD:
        पक्ष STMT_व्यंजक_SUBTRACT:
        पक्ष STMT_व्यंजक_MULTIPLY:
        पक्ष STMT_व्यंजक_ASSIGN:
            FreeStmt((ParseCommon *) ((व्यंजक_द्विआधारी *) stmt)->बायाँ);
            FreeStmt((ParseCommon *) ((व्यंजक_द्विआधारी *) stmt)->दायाँ);
            भङ्ग;

        पक्ष STMT_व्यंजक_ACTION_DECL:
            FreeStmt((ParseCommon *) ((व्यंजक_Action *) stmt)->args);
            भङ्ग;

        पक्ष STMT_व्यंजक_ACTION_LIST:
            FreeStmt((ParseCommon *) ((व्यंजक_Action_List *) stmt)->actions);
            भङ्ग;

        पक्ष STMT_व्यंजक_ARRAY_REF:
            FreeStmt((ParseCommon *) ((व्यंजक_Array_Ref *) stmt)->entry);
            भङ्ग;

        पक्ष STMT_व्यंजक_KEYSYM_LIST:
            darray_free(((व्यंजक_Keysym_List *) stmt)->syms);
            भङ्ग;

        पक्ष STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->मूल्य);
            भङ्ग;
        पक्ष STMT_TYPE:
            FreeStmt((ParseCommon *) ((कुंजी_Type_Def *) stmt)->body);
            भङ्ग;
        पक्ष STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            भङ्ग;
        पक्ष STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->मूल्य);
            भङ्ग;
        पक्ष STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            भङ्ग;
        पक्ष STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->कुंजियाँ);
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
            static_assert(STMT_अंक_मूल्यावली == 37 &&
                          STMT_अंक_मूल्यावली == STMT_UNKNOWN_COMPOUND + 1,
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

    यावत् (file)
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

स्थिर ध्रुव वर्ण *xkb_file_type_strings[FILE_TYPE_अंक_ENTRIES] = {
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
    यदि (type >= FILE_TYPE_अंक_ENTRIES)
        प्रत्यागम "unknown";
    प्रत्यागम xkb_file_type_strings[type];
}

स्थिर ध्रुव वर्ण *stmt_type_strings[STMT_अंक_मूल्यावली] = {
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

ध्रुव वर्ण *
stmt_type_to_string(गणना stmt_type type)
{
    यदि (type >= STMT_अंक_मूल्यावली)
        प्रत्यागम रिक्त;
    प्रत्यागम stmt_type_strings[type];
}

वर्ण
stmt_type_to_operator_char(गणना stmt_type type)
{
    विकल्प (type) {
    पक्ष STMT_व्यंजक_ADD:
        प्रत्यागम '+';
    पक्ष STMT_व्यंजक_SUBTRACT:
        प्रत्यागम '-';
    पक्ष STMT_व्यंजक_MULTIPLY:
        प्रत्यागम '*';
    पक्ष STMT_व्यंजक_DIVIDE:
        प्रत्यागम '/';
    पक्ष STMT_व्यंजक_NOT:
        प्रत्यागम '!';
    पक्ष STMT_व्यंजक_NEGATE:
        प्रत्यागम '-';
    पक्ष STMT_व्यंजक_INVERT:
        प्रत्यागम '~';
    पक्ष STMT_व्यंजक_UNARY_PLUS:
        प्रत्यागम '+';
    स्वभाव:
        प्रत्यागम '\0';
    }
}
