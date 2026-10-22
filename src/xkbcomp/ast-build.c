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
    রাশিমালা_Def *রাশিমালা = malloc(আকার(*রাশিমালা));
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;

    রাশিমালা->common.type = op;
    রাশিমালা->common.পরবর্তী = ফাঁকা;

    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_String(xkb_atom_t str)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_STRING_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->string.str = str;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Integer(int64_t ival)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_INTEGER_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->integer.ival = ival;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Float(শূন্য)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_FLOAT_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Boolean(bool set)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_BOOLEAN_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->boolean.set = set;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Name(xkb_atom_t চাবি_name)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYNAME_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->চাবি_name.চাবি_name = চাবি_name;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Sym(xkb_keysym_t keysym)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYSYM_LITERAL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->keysym.keysym = keysym;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Ident(xkb_atom_t ident)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_IDENT);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->ident.ident = ident;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Unary(গণনা stmt_type op, রাশিমালা_Def *child)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(op);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->unary.child = child;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_দ্বিমিক(গণনা stmt_type op, রাশিমালা_Def *বাম, রাশিমালা_Def *ডান)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(op);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;

    রাশিমালা->দ্বিমিক.বাম = বাম;
    রাশিমালা->দ্বিমিক.ডান = ডান;

    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_FIELD_REF);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->field_ref.element = element;
    রাশিমালা->field_ref.field = field;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, রাশিমালা_Def *entry)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ARRAY_REF);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->array_ref.element = element;
    রাশিমালা->array_ref.field = field;
    রাশিমালা->array_ref.entry = entry;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Empty_List(শূন্য)
{
    ফেরত রাশিমালা_Create(STMT_রাশিমালা_EMPTY_LIST);
}

রাশিমালা_Def *
রাশিমালা_Create_Action(xkb_atom_t name, রাশিমালা_Def *args)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ACTION_DECL);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->action.name = name;
    রাশিমালা->action.args = args;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_Action_List(রাশিমালা_Def *actions)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_ACTION_LIST);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    রাশিমালা->actions.actions = actions;
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Create_চাবি_Sym_List(xkb_keysym_t sym)
{
    রাশিমালা_Def *রাশিমালা = রাশিমালা_Create(STMT_রাশিমালা_KEYSYM_LIST);
    যদি (!রাশিমালা)
        ফেরত ফাঁকা;
    darray_init(রাশিমালা->keysym_list.syms);
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(রাশিমালা->keysym_list.syms, sym);
    }
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_Append_চাবি_Sym_List(রাশিমালা_Def *রাশিমালা, xkb_keysym_t sym)
{
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(রাশিমালা->keysym_list.syms, sym);
    }
    ফেরত রাশিমালা;
}

রাশিমালা_Def *
রাশিমালা_চাবি_Sym_List_Append_String(কাঠামো scanner *scanner,
                           রাশিমালা_Def *রাশিমালা, ধ্রুবক অক্ষর *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ধ্রুবক আকার_t len = strlen(string);
    আকার_t idx = 0;
    আকার_t idx_cp = 1;
    যখন (idx < len) {
        আকার_t গুনতি = 0;
        uint32_t cp = utf8_পরবর্তী_code_point(string + idx, len - idx, &গুনতি);
        যদি (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            যাও error;
        }
        ধ্রুবক xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        যদি (sym == XKB_চাবি_No_প্রতীক) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            যাও error;
        }
        darray_append(রাশিমালা->keysym_list.syms, sym);
        idx += গুনতি;
        idx_cp++;
    }
    প্রতিজ্ঞা(string[idx] == '\0');
    ফেরত রাশিমালা;
error:
    FreeStmt((ParseCommon*) রাশিমালা);
    ফেরত ফাঁকা;
}

xkb_keysym_t
KeysymParseString(কাঠামো scanner *scanner, ধ্রুবক অক্ষর *string)
{
    ধ্রুবক আকার_t len = strlen(string);
    যদি (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ফেরত XKB_চাবি_No_প্রতীক;
    }
    আকার_t গুনতি = 0;
    ধ্রুবক uint32_t cp = utf8_পরবর্তী_code_point(string, len, &গুনতি);
    যদি (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ফেরত XKB_চাবি_No_প্রতীক;
    } অন্যথা যদি (গুনতি != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ফেরত XKB_চাবি_No_প্রতীক;
    }
    ধ্রুবক xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    যদি (sym == XKB_চাবি_No_প্রতীক) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ফেরত sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t মান)
{
    KeycodeDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_KEYCODE;
    def->common.পরবর্তী = ফাঁকা;
    def->name = name;
    def->মান = মান;

    ফেরত def;
}

চাবি_Alias_Def *
চাবি_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    চাবি_Alias_Def *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_ALIAS;
    def->common.পরবর্তী = ফাঁকা;
    def->alias = alias;
    def->real = real;

    ফেরত def;
}

VModDef *
VModCreate(xkb_atom_t name, রাশিমালা_Def *মান)
{
    VModDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_VMOD;
    def->common.পরবর্তী = ফাঁকা;
    def->name = name;
    def->মান = মান;

    ফেরত def;
}

VarDef *
VarCreate(রাশিমালা_Def *name, রাশিমালা_Def *মান)
{
    VarDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_VAR;
    def->common.পরবর্তী = ফাঁকা;
    def->name = name;
    def->মান = মান;

    ফেরত def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    রাশিমালা_Def *name, *মান;
    VarDef *def;
    যদি (!(name = রাশিমালা_Create_Ident(ident))) {
        ফেরত ফাঁকা;
    }
    যদি (!(মান = রাশিমালা_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        ফেরত ফাঁকা;
    }
    যদি (!(def = VarCreate(name, মান))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) মান);
        ফেরত ফাঁকা;
    }
    ফেরত def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, রাশিমালা_Def *match)
{
    InterpDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_INTERP;
    def->common.পরবর্তী = ফাঁকা;
    def->sym = sym;
    def->match = match;
    def->def = ফাঁকা;

    ফেরত def;
}

চাবি_Type_Def *
চাবি_Type_Create(xkb_atom_t name, VarDef *body)
{
    চাবি_Type_Def *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_TYPE;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ফেরত def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t চাবি_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_SYMBOLS;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->চাবি_Name = চাবি_Name;
    def->symbols = symbols;

    ফেরত def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, রাশিমালা_Def *val)
{
    GroupCompatDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ফেরত def;
}

ModMapDef *
ModMapCreate(রাশিমালা_Def *modifiers, রাশিমালা_Def *চাবিগুলি)
{
    ModMapDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_MODMAP;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->চাবিগুলি = চাবিগুলি;

    ফেরত def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_LED_MAP;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ফেরত def;
}

LedNameDef *
LedNameCreate(int64_t ndx, রাশিমালা_Def *name, bool আভাসী)
{
    LedNameDef *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = STMT_LED_NAME;
    def->common.পরবর্তী = ফাঁকা;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->আভাসী = আভাসী;

    ফেরত def;
}

UnknownStatement *
UnknownStatementCreate(গণনা stmt_type type, কাঠামো sval name)
{
    UnknownStatement *def = malloc(আকার(*def));
    যদি (!def)
        ফেরত ফাঁকা;

    def->common.type = type;
    def->common.পরবর্তী = ফাঁকা;
    def->name = strndup(name.start, name.len);
    যদি (!def->name) {
        মুক্ত(def);
        ফেরত ফাঁকা;
    }

    ফেরত def;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(কাঠামো xkb_context *ctx, অক্ষর *str, গণনা merge_mode merge)
{
    IncludeStmt *incl, *first;
    অক্ষর *stmt, *tmp;
    অক্ষর nextop;

    incl = first = ফাঁকা;
    tmp = str;
    stmt = strdup_safe(str);
    যখন (tmp && *tmp)
    {
        অক্ষর *file = ফাঁকা, *map = ফাঁকা, *extra_তথ্য = ফাঁকা;

        যদি (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_তথ্য))
            যাও err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        যদি (isempty(file)) {
            মুক্ত(file);
            মুক্ত(map);
            মুক্ত(extra_তথ্য);
            চালু;
        }

        যদি (first == ফাঁকা) {
            first = incl = malloc(আকার(*first));
        } অন্যথা {
            incl->পরবর্তী_incl = malloc(আকার(*first));
            incl = incl->পরবর্তী_incl;
        }

        যদি (!incl) {
            মুক্ত(file);
            মুক্ত(map);
            মুক্ত(extra_তথ্য);
            বিরতি;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.পরবর্তী = ফাঁকা;
        incl->merge = merge;
        incl->stmt = ফাঁকা;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_তথ্য;
        incl->পরবর্তী_incl = ফাঁকা;

        নির্বাচন (nextop) {
        ক্ষেত্র MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            বিরতি;
        ক্ষেত্র MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            বিরতি;
        পূর্বনির্ধারিত:
            merge = MERGE_OVERRIDE;
        }
    }

    যদি (first)
        first->stmt = stmt;
    অন্যথা
        মুক্ত(stmt);

    ফেরত first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    মুক্ত(stmt);
    ফেরত ফাঁকা;
}

XkbFile *
XkbFileCreate(গণনা xkb_file_type type, অক্ষর *name, ParseCommon *defs,
              গণনা xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, আকার(*file));
    যদি (!file)
        ফেরত ফাঁকা;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ফেরত file;
}

XkbFile *
XkbFileFromComponents(কাঠামো xkb_context *ctx,
                      ধ্রুবক কাঠামো xkb_component_names *kkctgs)
{
    অক্ষর *ধ্রুবক components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    গণনা xkb_file_type type;
    IncludeStmt *include = ফাঁকা;
    XkbFile *file = ফাঁকা;
    ParseCommon *defs = ফাঁকা, *defsLast = ফাঁকা;

    জন্য (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        যদি (!include)
            যাও err;

        file = XkbFileCreate(type, ফাঁকা, (ParseCommon *) include, 0);
        যদি (!file) {
            FreeInclude(include);
            যাও err;
        }

        যদি (!defs)
            defsLast = defs = &file->common;
        অন্যথা
            defsLast = defsLast->পরবর্তী = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ফাঁকা, defs, 0);
    যদি (!file)
        যাও err;

    ফেরত file;

err:
    FreeXkbFile((XkbFile *) defs);
    ফেরত ফাঁকা;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *পরবর্তী;

    যখন (incl)
    {
        পরবর্তী = incl->পরবর্তী_incl;

        মুক্ত(incl->file);
        মুক্ত(incl->map);
        মুক্ত(incl->modifier);
        মুক্ত(incl->stmt);

        মুক্ত(incl);
        incl = পরবর্তী;
    }
}

শূন্য
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *পরবর্তী;

    যখন (stmt)
    {
        পরবর্তী = stmt->পরবর্তী;

        নির্বাচন (stmt->type) {
        ক্ষেত্র STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ফাঁকা;
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_NEGATE:
        ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
        ক্ষেত্র STMT_রাশিমালা_NOT:
        ক্ষেত্র STMT_রাশিমালা_INVERT:
            FreeStmt((ParseCommon *) ((রাশিমালা_Unary *) stmt)->child);
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_DIVIDE:
        ক্ষেত্র STMT_রাশিমালা_ADD:
        ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
        ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
        ক্ষেত্র STMT_রাশিমালা_ASSIGN:
            FreeStmt((ParseCommon *) ((রাশিমালা_দ্বিমিক *) stmt)->বাম);
            FreeStmt((ParseCommon *) ((রাশিমালা_দ্বিমিক *) stmt)->ডান);
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_ACTION_DECL:
            FreeStmt((ParseCommon *) ((রাশিমালা_Action *) stmt)->args);
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_ACTION_LIST:
            FreeStmt((ParseCommon *) ((রাশিমালা_Action_List *) stmt)->actions);
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_ARRAY_REF:
            FreeStmt((ParseCommon *) ((রাশিমালা_Array_Ref *) stmt)->entry);
            বিরতি;

        ক্ষেত্র STMT_রাশিমালা_KEYSYM_LIST:
            darray_free(((রাশিমালা_Keysym_List *) stmt)->syms);
            বিরতি;

        ক্ষেত্র STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->মান);
            বিরতি;
        ক্ষেত্র STMT_TYPE:
            FreeStmt((ParseCommon *) ((চাবি_Type_Def *) stmt)->body);
            বিরতি;
        ক্ষেত্র STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            বিরতি;
        ক্ষেত্র STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->মান);
            বিরতি;
        ক্ষেত্র STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            বিরতি;
        ক্ষেত্র STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->চাবিগুলি);
            বিরতি;
        ক্ষেত্র STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            বিরতি;
        ক্ষেত্র STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            বিরতি;
        ক্ষেত্র STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            বিরতি;
        ক্ষেত্র STMT_UNKNOWN_DECLARATION:
        ক্ষেত্র STMT_UNKNOWN_COMPOUND:
            মুক্ত(((UnknownStatement *) stmt)->name);
            বিরতি;
        পূর্বনির্ধারিত:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_অঙ্ক_মানসমূহ == 37 &&
                          STMT_অঙ্ক_মানসমূহ == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            বিরতি;
        }

        মুক্ত(stmt);
        stmt = পরবর্তী;
    }
}

শূন্য
FreeXkbFile(XkbFile *file)
{
    XkbFile *পরবর্তী;

    যখন (file)
    {
        পরবর্তী = (XkbFile *) file->common.পরবর্তী;

        নির্বাচন (file->file_type) {
        ক্ষেত্র FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            বিরতি;

        ক্ষেত্র FILE_TYPE_TYPES:
        ক্ষেত্র FILE_TYPE_COMPAT:
        ক্ষেত্র FILE_TYPE_SYMBOLS:
        ক্ষেত্র FILE_TYPE_KEYCODES:
        ক্ষেত্র FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            বিরতি;

        পূর্বনির্ধারিত:
            বিরতি;
        }

        মুক্ত(file->name);
        মুক্ত(file);
        file = পরবর্তী;
    }
}

স্থির ধ্রুবক অক্ষর *xkb_file_type_strings[FILE_TYPE_অঙ্ক_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ধ্রুবক অক্ষর *
xkb_file_type_to_string(গণনা xkb_file_type type)
{
    যদি (type >= FILE_TYPE_অঙ্ক_ENTRIES)
        ফেরত "unknown";
    ফেরত xkb_file_type_strings[type];
}

স্থির ধ্রুবক অক্ষর *stmt_type_strings[STMT_অঙ্ক_মানসমূহ] = {
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

ধ্রুবক অক্ষর *
stmt_type_to_string(গণনা stmt_type type)
{
    যদি (type >= STMT_অঙ্ক_মানসমূহ)
        ফেরত ফাঁকা;
    ফেরত stmt_type_strings[type];
}

অক্ষর
stmt_type_to_operator_char(গণনা stmt_type type)
{
    নির্বাচন (type) {
    ক্ষেত্র STMT_রাশিমালা_ADD:
        ফেরত '+';
    ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
        ফেরত '-';
    ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
        ফেরত '*';
    ক্ষেত্র STMT_রাশিমালা_DIVIDE:
        ফেরত '/';
    ক্ষেত্র STMT_রাশিমালা_NOT:
        ফেরত '!';
    ক্ষেত্র STMT_রাশিমালা_NEGATE:
        ফেরত '-';
    ক্ষেত্র STMT_রাশিমালা_INVERT:
        ফেরত '~';
    ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
        ফেরত '+';
    পূর্বনির্ধারিত:
        ফেরত '\0';
    }
}
