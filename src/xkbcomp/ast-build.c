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

स्थिर अभिव्यक्ती_Def *
अभिव्यक्ती_Create(गणना stmt_type op)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = malloc(परिमाण(*अभिव्यक्ती));
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;

    अभिव्यक्ती->common.type = op;
    अभिव्यक्ती->common.next = रिक्त;

    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_String(xkb_atom_t str)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_STRING_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->string.str = str;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Integer(int64_t ival)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_INTEGER_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->integer.ival = ival;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Float(शून्य)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_FLOAT_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Boolean(bool set)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_BOOLEAN_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->boolean.set = set;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Name(xkb_atom_t किल्ली_name)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_KEYNAME_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->किल्ली_name.किल्ली_name = किल्ली_name;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Sym(xkb_keysym_t keysym)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_KEYSYM_LITERAL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->keysym.keysym = keysym;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Ident(xkb_atom_t ident)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_IDENT);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->ident.ident = ident;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Unary(गणना stmt_type op, अभिव्यक्ती_Def *child)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(op);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->unary.child = child;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_द्विमान(गणना stmt_type op, अभिव्यक्ती_Def *डावा, अभिव्यक्ती_Def *उजवा)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(op);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;

    अभिव्यक्ती->द्विमान.डावा = डावा;
    अभिव्यक्ती->द्विमान.उजवा = उजवा;

    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_FIELD_REF);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->field_ref.element = element;
    अभिव्यक्ती->field_ref.field = field;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, अभिव्यक्ती_Def *entry)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_ARRAY_REF);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->array_ref.element = element;
    अभिव्यक्ती->array_ref.field = field;
    अभिव्यक्ती->array_ref.entry = entry;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Empty_List(शून्य)
{
    प्रत्यागम अभिव्यक्ती_Create(STMT_अभिव्यक्ती_EMPTY_LIST);
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Action(xkb_atom_t name, अभिव्यक्ती_Def *args)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_ACTION_DECL);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->action.name = name;
    अभिव्यक्ती->action.args = args;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_Action_List(अभिव्यक्ती_Def *actions)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_ACTION_LIST);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    अभिव्यक्ती->actions.actions = actions;
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Create_किल्ली_Sym_List(xkb_keysym_t sym)
{
    अभिव्यक्ती_Def *अभिव्यक्ती = अभिव्यक्ती_Create(STMT_अभिव्यक्ती_KEYSYM_LIST);
    यदि (!अभिव्यक्ती)
        प्रत्यागम रिक्त;
    darray_init(अभिव्यक्ती->keysym_list.syms);
    यदि (sym == XKB_किल्ली_No_चिन्ह) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(अभिव्यक्ती->keysym_list.syms, sym);
    }
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_Append_किल्ली_Sym_List(अभिव्यक्ती_Def *अभिव्यक्ती, xkb_keysym_t sym)
{
    यदि (sym == XKB_किल्ली_No_चिन्ह) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(अभिव्यक्ती->keysym_list.syms, sym);
    }
    प्रत्यागम अभिव्यक्ती;
}

अभिव्यक्ती_Def *
अभिव्यक्ती_किल्ली_Sym_List_Append_String(संरचना scanner *scanner,
                           अभिव्यक्ती_Def *अभिव्यक्ती, ध्रुव वर्ण *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ध्रुव size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    यावत् (idx < len) {
        size_t मोजणी = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &मोजणी);
        यदि (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            गच्छ error;
        }
        ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        यदि (sym == XKB_किल्ली_No_चिन्ह) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            गच्छ error;
        }
        darray_append(अभिव्यक्ती->keysym_list.syms, sym);
        idx += मोजणी;
        idx_cp++;
    }
    प्रतिज्ञा(string[idx] == '\0');
    प्रत्यागम अभिव्यक्ती;
error:
    FreeStmt((ParseCommon*) अभिव्यक्ती);
    प्रत्यागम रिक्त;
}

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, ध्रुव वर्ण *string)
{
    ध्रुव size_t len = strlen(string);
    यदि (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        प्रत्यागम XKB_किल्ली_No_चिन्ह;
    }
    size_t मोजणी = 0;
    ध्रुव uint32_t cp = utf8_next_code_point(string, len, &मोजणी);
    यदि (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        प्रत्यागम XKB_किल्ली_No_चिन्ह;
    } अन्यथा यदि (मोजणी != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        प्रत्यागम XKB_किल्ली_No_चिन्ह;
    }
    ध्रुव xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    यदि (sym == XKB_किल्ली_No_चिन्ह) {
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

किल्ली_Alias_Def *
किल्ली_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    किल्ली_Alias_Def *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_ALIAS;
    def->common.next = रिक्त;
    def->alias = alias;
    def->real = real;

    प्रत्यागम def;
}

VModDef *
VModCreate(xkb_atom_t name, अभिव्यक्ती_Def *मूल्य)
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
VarCreate(अभिव्यक्ती_Def *name, अभिव्यक्ती_Def *मूल्य)
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
    अभिव्यक्ती_Def *name, *मूल्य;
    VarDef *def;
    यदि (!(name = अभिव्यक्ती_Create_Ident(ident))) {
        प्रत्यागम रिक्त;
    }
    यदि (!(मूल्य = अभिव्यक्ती_Create_Boolean(set))) {
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
InterpCreate(xkb_keysym_t sym, अभिव्यक्ती_Def *match)
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

किल्ली_Type_Def *
किल्ली_Type_Create(xkb_atom_t name, VarDef *body)
{
    किल्ली_Type_Def *def = malloc(परिमाण(*def));
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
SymbolsCreate(xkb_atom_t किल्ली_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_SYMBOLS;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->किल्ली_Name = किल्ली_Name;
    def->symbols = symbols;

    प्रत्यागम def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, अभिव्यक्ती_Def *val)
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
ModMapCreate(अभिव्यक्ती_Def *modifiers, अभिव्यक्ती_Def *किल्ल्या)
{
    ModMapDef *def = malloc(परिमाण(*def));
    यदि (!def)
        प्रत्यागम रिक्त;

    def->common.type = STMT_MODMAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->किल्ल्या = किल्ल्या;

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
LedNameCreate(int64_t ndx, अभिव्यक्ती_Def *name, bool आभास)
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

        पक्ष STMT_अभिव्यक्ती_NEGATE:
        पक्ष STMT_अभिव्यक्ती_UNARY_PLUS:
        पक्ष STMT_अभिव्यक्ती_NOT:
        पक्ष STMT_अभिव्यक्ती_INVERT:
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_Unary *) stmt)->child);
            भङ्ग;

        पक्ष STMT_अभिव्यक्ती_DIVIDE:
        पक्ष STMT_अभिव्यक्ती_ADD:
        पक्ष STMT_अभिव्यक्ती_SUBTRACT:
        पक्ष STMT_अभिव्यक्ती_MULTIPLY:
        पक्ष STMT_अभिव्यक्ती_ASSIGN:
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_द्विमान *) stmt)->डावा);
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_द्विमान *) stmt)->उजवा);
            भङ्ग;

        पक्ष STMT_अभिव्यक्ती_ACTION_DECL:
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_Action *) stmt)->args);
            भङ्ग;

        पक्ष STMT_अभिव्यक्ती_ACTION_LIST:
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_Action_List *) stmt)->actions);
            भङ्ग;

        पक्ष STMT_अभिव्यक्ती_ARRAY_REF:
            FreeStmt((ParseCommon *) ((अभिव्यक्ती_Array_Ref *) stmt)->entry);
            भङ्ग;

        पक्ष STMT_अभिव्यक्ती_KEYSYM_LIST:
            darray_free(((अभिव्यक्ती_Keysym_List *) stmt)->syms);
            भङ्ग;

        पक्ष STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->मूल्य);
            भङ्ग;
        पक्ष STMT_TYPE:
            FreeStmt((ParseCommon *) ((किल्ली_Type_Def *) stmt)->body);
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
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->किल्ल्या);
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
            static_assert(STMT_अंक_मूल्ये == 37 &&
                          STMT_अंक_मूल्ये == STMT_UNKNOWN_COMPOUND + 1,
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

स्थिर ध्रुव वर्ण *stmt_type_strings[STMT_अंक_मूल्ये] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_अभिव्यक्ती_STRING_LITERAL] = "string literal expression",
    [STMT_अभिव्यक्ती_INTEGER_LITERAL] = "integer literal expression",
    [STMT_अभिव्यक्ती_FLOAT_LITERAL] = "float literal expression",
    [STMT_अभिव्यक्ती_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_अभिव्यक्ती_KEYNAME_LITERAL] = "key name expression",
    [STMT_अभिव्यक्ती_KEYSYM_LITERAL] = "keysym expression",
    [STMT_अभिव्यक्ती_IDENT] = "identifier expression",
    [STMT_अभिव्यक्ती_ACTION_DECL] = "action declaration expression",
    [STMT_अभिव्यक्ती_FIELD_REF] = "field reference expression",
    [STMT_अभिव्यक्ती_ARRAY_REF] = "array reference expression",
    [STMT_अभिव्यक्ती_EMPTY_LIST] = "empty list expression",
    [STMT_अभिव्यक्ती_KEYSYM_LIST] = "keysym list expression",
    [STMT_अभिव्यक्ती_ACTION_LIST] = "action list expression",
    [STMT_अभिव्यक्ती_ADD] = "addition expression",
    [STMT_अभिव्यक्ती_SUBTRACT] = "substraction expression",
    [STMT_अभिव्यक्ती_MULTIPLY] = "multiplication expression",
    [STMT_अभिव्यक्ती_DIVIDE] = "division expression",
    [STMT_अभिव्यक्ती_ASSIGN] = "assignment expression",
    [STMT_अभिव्यक्ती_NOT] = "logical negation expression",
    [STMT_अभिव्यक्ती_NEGATE] = "arithmetic negation expression",
    [STMT_अभिव्यक्ती_INVERT] = "bitwise inversion expression",
    [STMT_अभिव्यक्ती_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_अंक_मूल्ये == 37 &&
              STMT_अंक_मूल्ये == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ध्रुव वर्ण *
stmt_type_to_string(गणना stmt_type type)
{
    यदि (type >= STMT_अंक_मूल्ये)
        प्रत्यागम रिक्त;
    प्रत्यागम stmt_type_strings[type];
}

वर्ण
stmt_type_to_operator_char(गणना stmt_type type)
{
    विकल्प (type) {
    पक्ष STMT_अभिव्यक्ती_ADD:
        प्रत्यागम '+';
    पक्ष STMT_अभिव्यक्ती_SUBTRACT:
        प्रत्यागम '-';
    पक्ष STMT_अभिव्यक्ती_MULTIPLY:
        प्रत्यागम '*';
    पक्ष STMT_अभिव्यक्ती_DIVIDE:
        प्रत्यागम '/';
    पक्ष STMT_अभिव्यक्ती_NOT:
        प्रत्यागम '!';
    पक्ष STMT_अभिव्यक्ती_NEGATE:
        प्रत्यागम '-';
    पक्ष STMT_अभिव्यक्ती_INVERT:
        प्रत्यागम '~';
    पक्ष STMT_अभिव्यक्ती_UNARY_PLUS:
        प्रत्यागम '+';
    स्वभाव:
        प्रत्यागम '\0';
    }
}
