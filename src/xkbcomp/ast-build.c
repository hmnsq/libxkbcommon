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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "ast-build.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "utf8-decoding.h"

স্থির রাশিমালা_Def *
রাশিমালা_Create(গণনা stmt_type op)
{
    রাশিমালা_Def *রাশিমালা = malloc(পরিমাণ(*রাশিমালা));
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;

    রাশিমালা->common.type = op;
    রাশিমালা->common.next = রিক্ত;

    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_String(xkb_atom_t str)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_STRING_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->string.str = str;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Integer(int64_t ival)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_INTEGER_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->integer.ival = ival;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Float(শূন্য)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_FLOAT_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Boolean(bool set)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_BOOLEAN_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->boolean.set = set;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Name(xkb_atom_t চাবি_name)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYNAME_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->চাবি_name.চাবি_name = চাবি_name;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Sym(xkb_keysym_t keysym)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYSYM_LITERAL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->keysym.keysym = keysym;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Ident(xkb_atom_t ident)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_IDENT);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->ident.ident = ident;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Unary(গণনা stmt_type op, রাশিমালা_Def *child)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(op);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->unary.child = child;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_দ্বিমিক(গণনা stmt_type op, রাশিমালা_Def *বাম, রাশিমালা_Def *ডান)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(op);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;

    রাশিমালা->দ্বিমিক.বাম = বাম;
    রাশিমালা->দ্বিমিক.ডান = ডান;

    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_FIELD_REF);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->field_ref.element = element;
    রাশিমালা->field_ref.field = field;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, রাশিমালা_Def *entry)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ARRAY_REF);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->array_ref.element = element;
    রাশিমালা->array_ref.field = field;
    রাশিমালা->array_ref.entry = entry;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Empty_List(শূন্য)
{
    প্রত্যাগম রাশিমালা_Create(STMT_রাশিমালা_EMPTY_LIST);
}

রাশিমালা_Def *
রাশিমালা_Create_Action(xkb_atom_t name, রাশিমালা_Def *args)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ACTION_DECL);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->action.name = name;
    রাশিমালা->action.args = args;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Action_List(রাশিমালা_Def *actions)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ACTION_LIST);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    রাশিমালা->actions.actions = actions;
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Sym_List(xkb_keysym_t sym)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYSYM_LIST);
    যদি (!রাশিমালা)
        প্রত্যাগম রিক্ত;
    darray_init(রাশিমালা->keysym_list.syms);
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(রাশিমালা->keysym_list.syms, sym);
    }
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Append_চাবি_Sym_List(রাশিমালা_Def *রাশিমালা, xkb_keysym_t sym)
{
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(রাশিমালা->keysym_list.syms, sym);
    }
    প্রত্যাগম রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_চাবি_Sym_List_Append_String(সংরচনা scanner *scanner,
                           রাশিমালা_Def *রাশিমালা, ধ্রুব বর্ণ *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ধ্রুব size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    যাবত্ (idx < len) {
        size_t গুনতি = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &গুনতি);
        যদি (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            গচ্ছ error;
        }
        ধ্রুব xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        যদি (sym == XKB_চাবি_No_প্রতীক) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            গচ্ছ error;
        }
        darray_append(রাশিমালা->keysym_list.syms, sym);
        idx += গুনতি;
        idx_cp++;
    }
    প্রতিজ্ঞা(string[idx] == '\0');
    প্রত্যাগম রাশিমালা;
error:
    FreeStmt((ParseCommon*) রাশিমালা);
    প্রত্যাগম রিক্ত;
}

xkb_keysym_t
KeysymParseString(সংরচনা scanner *scanner, ধ্রুব বর্ণ *string)
{
    ধ্রুব size_t len = strlen(string);
    যদি (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    }
    size_t গুনতি = 0;
    ধ্রুব uint32_t cp = utf8_next_code_point(string, len, &গুনতি);
    যদি (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    } অন্যথা যদি (গুনতি != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    }
    ধ্রুব xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    প্রত্যাগম sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t মান)
{
    KeycodeDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_KEYCODE;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    প্রত্যাগম def;
}

চাবি_Alias_Def *
চাবি_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    চাবি_Alias_Def *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_ALIAS;
    def->common.next = রিক্ত;
    def->alias = alias;
    def->real = real;

    প্রত্যাগম def;
}

VModDef *
VModCreate(xkb_atom_t name, রাশিমালা_Def *মান)
{
    VModDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_VMOD;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    প্রত্যাগম def;
}

VarDef *
VarCreate(রাশিমালা_Def *name, রাশিমালা_Def *মান)
{
    VarDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_VAR;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    প্রত্যাগম def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    রাশিমালা_Def *name, *মান;
    VarDef *def;
    যদি (!(name = রাশিমালা_Create_Ident(ident))) {
        প্রত্যাগম রিক্ত;
    }
    যদি (!(মান = রাশিমালা_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        প্রত্যাগম রিক্ত;
    }
    যদি (!(def = VarCreate(name, মান))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) মান);
        প্রত্যাগম রিক্ত;
    }
    প্রত্যাগম def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, রাশিমালা_Def *match)
{
    InterpDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_INTERP;
    def->common.next = রিক্ত;
    def->sym = sym;
    def->match = match;
    def->def = রিক্ত;

    প্রত্যাগম def;
}

চাবি_Type_Def *
চাবি_Type_Create(xkb_atom_t name, VarDef *body)
{
    চাবি_Type_Def *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_TYPE;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    প্রত্যাগম def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t চাবি_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_SYMBOLS;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->চাবি_Name = চাবি_Name;
    def->symbols = symbols;

    প্রত্যাগম def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, রাশিমালা_Def *val)
{
    GroupCompatDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    প্রত্যাগম def;
}

ModMapDef *
ModMapCreate(রাশিমালা_Def *modifiers, রাশিমালা_Def *চাবিগুলি)
{
    ModMapDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_MODMAP;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->চাবিগুলি = চাবিগুলি;

    প্রত্যাগম def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_LED_MAP;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    প্রত্যাগম def;
}

LedNameDef *
LedNameCreate(int64_t ndx, রাশিমালা_Def *name, bool আভাস)
{
    LedNameDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = STMT_LED_NAME;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->আভাস = আভাস;

    প্রত্যাগম def;
}

UnknownStatement *
UnknownStatementCreate(গণনা stmt_type type, সংরচনা sval name)
{
    UnknownStatement *def = malloc(পরিমাণ(*def));
    যদি (!def)
        প্রত্যাগম রিক্ত;

    def->common.type = type;
    def->common.next = রিক্ত;
    def->name = strndup(name.start, name.len);
    যদি (!def->name) {
        মুক্ত(def);
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম def;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(সংরচনা xkb_context *ctx, বর্ণ *str, গণনা merge_mode merge)
{
    IncludeStmt *incl, *first;
    বর্ণ *stmt, *tmp;
    বর্ণ nextop;

    incl = first = রিক্ত;
    tmp = str;
    stmt = strdup_safe(str);
    যাবত্ (tmp && *tmp)
    {
        বর্ণ *file = রিক্ত, *map = রিক্ত, *extra_data = রিক্ত;

        যদি (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            গচ্ছ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        যদি (isempty(file)) {
            মুক্ত(file);
            মুক্ত(map);
            মুক্ত(extra_data);
            অনুবর্ত;
        }

        যদি (first == রিক্ত) {
            first = incl = malloc(পরিমাণ(*first));
        } অন্যথা {
            incl->next_incl = malloc(পরিমাণ(*first));
            incl = incl->next_incl;
        }

        যদি (!incl) {
            মুক্ত(file);
            মুক্ত(map);
            মুক্ত(extra_data);
            ভঙ্গ;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = রিক্ত;
        incl->merge = merge;
        incl->stmt = রিক্ত;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = রিক্ত;

        বিকল্প (nextop) {
        পক্ষ MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            ভঙ্গ;
        পক্ষ MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            ভঙ্গ;
        স্বভাব:
            merge = MERGE_OVERRIDE;
        }
    }

    যদি (first)
        first->stmt = stmt;
    অন্যথা
        মুক্ত(stmt);

    প্রত্যাগম first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    মুক্ত(stmt);
    প্রত্যাগম রিক্ত;
}

XkbFile *
XkbFileCreate(গণনা xkb_file_type type, বর্ণ *name, ParseCommon *defs,
              গণনা xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, পরিমাণ(*file));
    যদি (!file)
        প্রত্যাগম রিক্ত;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    প্রত্যাগম file;
}

XkbFile *
XkbFileFromComponents(সংরচনা xkb_context *ctx,
                      ধ্রুব সংরচনা xkb_component_names *kkctgs)
{
    বর্ণ *ধ্রুব components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    গণনা xkb_file_type type;
    IncludeStmt *include = রিক্ত;
    XkbFile *file = রিক্ত;
    ParseCommon *defs = রিক্ত, *defsLast = রিক্ত;

    প্রতি (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        যদি (!include)
            গচ্ছ err;

        file = XkbFileCreate(type, রিক্ত, (ParseCommon *) include, 0);
        যদি (!file) {
            FreeInclude(include);
            গচ্ছ err;
        }

        যদি (!defs)
            defsLast = defs = &file->common;
        অন্যথা
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, রিক্ত, defs, 0);
    যদি (!file)
        গচ্ছ err;

    প্রত্যাগম file;

err:
    FreeXkbFile((XkbFile *) defs);
    প্রত্যাগম রিক্ত;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    যাবত্ (incl)
    {
        next = incl->next_incl;

        মুক্ত(incl->file);
        মুক্ত(incl->map);
        মুক্ত(incl->modifier);
        মুক্ত(incl->stmt);

        মুক্ত(incl);
        incl = next;
    }
}

শূন্য
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    যাবত্ (stmt)
    {
        next = stmt->next;

        বিকল্প (stmt->type) {
        পক্ষ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = রিক্ত;
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_NEGATE:
        পক্ষ STMT_রাশিমালা_UNARY_PLUS:
        পক্ষ STMT_রাশিমালা_NOT:
        পক্ষ STMT_রাশিমালা_INVERT:
            FreeStmt((ParseCommon *) ((রাশিমালা_Unary *) stmt)->child);
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_DIVIDE:
        পক্ষ STMT_রাশিমালা_ADD:
        পক্ষ STMT_রাশিমালা_SUBTRACT:
        পক্ষ STMT_রাশিমালা_MULTIPLY:
        পক্ষ STMT_রাশিমালা_ASSIGN:
            FreeStmt((ParseCommon *) ((রাশিমালা_দ্বিমিক *) stmt)->বাম);
            FreeStmt((ParseCommon *) ((রাশিমালা_দ্বিমিক *) stmt)->ডান);
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_ACTION_DECL:
            FreeStmt((ParseCommon *) ((রাশিমালা_Action *) stmt)->args);
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_ACTION_LIST:
            FreeStmt((ParseCommon *) ((রাশিমালা_Action_List *) stmt)->actions);
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_ARRAY_REF:
            FreeStmt((ParseCommon *) ((রাশিমালা_Array_Ref *) stmt)->entry);
            ভঙ্গ;

        পক্ষ STMT_রাশিমালা_KEYSYM_LIST:
            darray_free(((রাশিমালা_Keysym_List *) stmt)->syms);
            ভঙ্গ;

        পক্ষ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->মান);
            ভঙ্গ;
        পক্ষ STMT_TYPE:
            FreeStmt((ParseCommon *) ((চাবি_Type_Def *) stmt)->body);
            ভঙ্গ;
        পক্ষ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ভঙ্গ;
        পক্ষ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->মান);
            ভঙ্গ;
        পক্ষ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ভঙ্গ;
        পক্ষ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->চাবিগুলি);
            ভঙ্গ;
        পক্ষ STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            ভঙ্গ;
        পক্ষ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ভঙ্গ;
        পক্ষ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ভঙ্গ;
        পক্ষ STMT_UNKNOWN_DECLARATION:
        পক্ষ STMT_UNKNOWN_COMPOUND:
            মুক্ত(((UnknownStatement *) stmt)->name);
            ভঙ্গ;
        স্বভাব:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_অঙ্ক_মানসমূহ == 37 &&
                          STMT_অঙ্ক_মানসমূহ == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ভঙ্গ;
        }

        মুক্ত(stmt);
        stmt = next;
    }
}

শূন্য
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    যাবত্ (file)
    {
        next = (XkbFile *) file->common.next;

        বিকল্প (file->file_type) {
        পক্ষ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ভঙ্গ;

        পক্ষ FILE_TYPE_TYPES:
        পক্ষ FILE_TYPE_COMPAT:
        পক্ষ FILE_TYPE_SYMBOLS:
        পক্ষ FILE_TYPE_KEYCODES:
        পক্ষ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ভঙ্গ;

        স্বভাব:
            ভঙ্গ;
        }

        মুক্ত(file->name);
        মুক্ত(file);
        file = next;
    }
}

স্থির ধ্রুব বর্ণ *xkb_file_type_strings[FILE_TYPE_অঙ্ক_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ধ্রুব বর্ণ *
xkb_file_type_to_string(গণনা xkb_file_type type)
{
    যদি (type >= FILE_TYPE_অঙ্ক_ENTRIES)
        প্রত্যাগম "unknown";
    প্রত্যাগম xkb_file_type_strings[type];
}

স্থির ধ্রুব বর্ণ *stmt_type_strings[STMT_অঙ্ক_মানসমূহ] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_রাশিমালা_STRING_LITERAL] = "string literal expression",
    [STMT_রাশিমালা_INTEGER_LITERAL] = "integer literal expression",
    [STMT_রাশিমালা_FLOAT_LITERAL] = "float literal expression",
    [STMT_রাশিমালা_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_রাশিমালা_KEYNAME_LITERAL] = "key name expression",
    [STMT_রাশিমালা_KEYSYM_LITERAL] = "keysym expression",
    [STMT_রাশিমালা_IDENT] = "identifier expression",
    [STMT_রাশিমালা_ACTION_DECL] = "action declaration expression",
    [STMT_রাশিমালা_FIELD_REF] = "field reference expression",
    [STMT_রাশিমালা_ARRAY_REF] = "array reference expression",
    [STMT_রাশিমালা_EMPTY_LIST] = "empty list expression",
    [STMT_রাশিমালা_KEYSYM_LIST] = "keysym list expression",
    [STMT_রাশিমালা_ACTION_LIST] = "action list expression",
    [STMT_রাশিমালা_ADD] = "addition expression",
    [STMT_রাশিমালা_SUBTRACT] = "substraction expression",
    [STMT_রাশিমালা_MULTIPLY] = "multiplication expression",
    [STMT_রাশিমালা_DIVIDE] = "division expression",
    [STMT_রাশিমালা_ASSIGN] = "assignment expression",
    [STMT_রাশিমালা_NOT] = "logical negation expression",
    [STMT_রাশিমালা_NEGATE] = "arithmetic negation expression",
    [STMT_রাশিমালা_INVERT] = "bitwise inversion expression",
    [STMT_রাশিমালা_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_অঙ্ক_মানসমূহ == 37 &&
              STMT_অঙ্ক_মানসমূহ == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ধ্রুব বর্ণ *
stmt_type_to_string(গণনা stmt_type type)
{
    যদি (type >= STMT_অঙ্ক_মানসমূহ)
        প্রত্যাগম রিক্ত;
    প্রত্যাগম stmt_type_strings[type];
}

বর্ণ
stmt_type_to_operator_char(গণনা stmt_type type)
{
    বিকল্প (type) {
    পক্ষ STMT_রাশিমালা_ADD:
        প্রত্যাগম '+';
    পক্ষ STMT_রাশিমালা_SUBTRACT:
        প্রত্যাগম '-';
    পক্ষ STMT_রাশিমালা_MULTIPLY:
        প্রত্যাগম '*';
    পক্ষ STMT_রাশিমালা_DIVIDE:
        প্রত্যাগম '/';
    পক্ষ STMT_রাশিমালা_NOT:
        প্রত্যাগম '!';
    পক্ষ STMT_রাশিমালা_NEGATE:
        প্রত্যাগম '-';
    পক্ষ STMT_রাশিমালা_INVERT:
        প্রত্যাগম '~';
    পক্ষ STMT_রাশিমালা_UNARY_PLUS:
        প্রত্যাগম '+';
    স্বভাব:
        প্রত্যাগম '\0';
    }
}
