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

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"
#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "ast-build.h"
#অন্তর্ভাৱ "include.h"
#অন্তর্ভাৱ "keysym.h"
#অন্তর্ভাৱ "utf8-decoding.h"

স্থির ৰাশিমালা_Def *
ৰাশিমালা_Create(গণনা stmt_type op)
{
    ৰাশিমালা_Def *ৰাশিমালা = malloc(পরিমাণ(*ৰাশিমালা));
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;

    ৰাশিমালা->common.type = op;
    ৰাশিমালা->common.next = রিক্ত;

    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_String(xkb_atom_t str)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_STRING_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->string.str = str;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Integer(int64_t ival)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_INTEGER_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->integer.ival = ival;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Float(শূন্য)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_FLOAT_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Boolean(bool set)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_BOOLEAN_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->boolean.set = set;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Name(xkb_atom_t চাবি_name)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_KEYNAME_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->চাবি_name.চাবি_name = চাবি_name;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Sym(xkb_keysym_t keysym)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_KEYSYM_LITERAL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->keysym.keysym = keysym;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Ident(xkb_atom_t ident)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_IDENT);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->ident.ident = ident;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Unary(গণনা stmt_type op, ৰাশিমালা_Def *child)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(op);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->unary.child = child;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_দ্বিমিক(গণনা stmt_type op, ৰাশিমালা_Def *বাম, ৰাশিমালা_Def *ডান)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(op);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;

    ৰাশিমালা->দ্বিমিক.বাম = বাম;
    ৰাশিমালা->দ্বিমিক.ডান = ডান;

    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_FIELD_REF);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->field_ref.element = element;
    ৰাশিমালা->field_ref.field = field;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, ৰাশিমালা_Def *entry)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_ARRAY_REF);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->array_ref.element = element;
    ৰাশিমালা->array_ref.field = field;
    ৰাশিমালা->array_ref.entry = entry;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Empty_List(শূন্য)
{
    ঘূৰাই ৰাশিমালা_Create(STMT_ৰাশিমালা_EMPTY_LIST);
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Action(xkb_atom_t name, ৰাশিমালা_Def *args)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_ACTION_DECL);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->action.name = name;
    ৰাশিমালা->action.args = args;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_Action_List(ৰাশিমালা_Def *actions)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_ACTION_LIST);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    ৰাশিমালা->actions.actions = actions;
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Create_চাবি_Sym_List(xkb_keysym_t sym)
{
    ৰাশিমালা_Def *ৰাশিমালা = ৰাশিমালা_Create(STMT_ৰাশিমালা_KEYSYM_LIST);
    যদি (!ৰাশিমালা)
        ঘূৰাই রিক্ত;
    darray_init(ৰাশিমালা->keysym_list.syms);
    যদি (sym == XKB_চাবি_No_প্ৰতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(ৰাশিমালা->keysym_list.syms, sym);
    }
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_Append_চাবি_Sym_List(ৰাশিমালা_Def *ৰাশিমালা, xkb_keysym_t sym)
{
    যদি (sym == XKB_চাবি_No_প্ৰতীক) {
        /* Discard NoSymbol */
    } অন্যথা {
        darray_append(ৰাশিমালা->keysym_list.syms, sym);
    }
    ঘূৰাই ৰাশিমালা;
}

ৰাশিমালা_Def *
ৰাশিমালা_চাবি_Sym_List_Append_String(সংরচনা scanner *scanner,
                           ৰাশিমালা_Def *ৰাশিমালা, ধ্রুৱ ৱর্ণ *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ধ্রুৱ size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    যেতিয়ালৈ (idx < len) {
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
        ধ্রুৱ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        যদি (sym == XKB_চাবি_No_প্ৰতীক) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            গচ্ছ error;
        }
        darray_append(ৰাশিমালা->keysym_list.syms, sym);
        idx += গুনতি;
        idx_cp++;
    }
    প্রতিজ্ঞা(string[idx] == '\0');
    ঘূৰাই ৰাশিমালা;
error:
    FreeStmt((ParseCommon*) ৰাশিমালা);
    ঘূৰাই রিক্ত;
}

xkb_keysym_t
KeysymParseString(সংরচনা scanner *scanner, ধ্রুৱ ৱর্ণ *string)
{
    ধ্রুৱ size_t len = strlen(string);
    যদি (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ঘূৰাই XKB_চাবি_No_প্ৰতীক;
    }
    size_t গুনতি = 0;
    ধ্রুৱ uint32_t cp = utf8_next_code_point(string, len, &গুনতি);
    যদি (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ঘূৰাই XKB_চাবি_No_প্ৰতীক;
    } অন্যথা যদি (গুনতি != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ঘূৰাই XKB_চাবি_No_প্ৰতীক;
    }
    ধ্রুৱ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    যদি (sym == XKB_চাবি_No_প্ৰতীক) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ঘূৰাই sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t মান)
{
    KeycodeDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_KEYCODE;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    ঘূৰাই def;
}

চাবি_Alias_Def *
চাবি_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    চাবি_Alias_Def *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_ALIAS;
    def->common.next = রিক্ত;
    def->alias = alias;
    def->real = real;

    ঘূৰাই def;
}

VModDef *
VModCreate(xkb_atom_t name, ৰাশিমালা_Def *মান)
{
    VModDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_VMOD;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    ঘূৰাই def;
}

VarDef *
VarCreate(ৰাশিমালা_Def *name, ৰাশিমালা_Def *মান)
{
    VarDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_VAR;
    def->common.next = রিক্ত;
    def->name = name;
    def->মান = মান;

    ঘূৰাই def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    ৰাশিমালা_Def *name, *মান;
    VarDef *def;
    যদি (!(name = ৰাশিমালা_Create_Ident(ident))) {
        ঘূৰাই রিক্ত;
    }
    যদি (!(মান = ৰাশিমালা_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        ঘূৰাই রিক্ত;
    }
    যদি (!(def = VarCreate(name, মান))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) মান);
        ঘূৰাই রিক্ত;
    }
    ঘূৰাই def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ৰাশিমালা_Def *match)
{
    InterpDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_INTERP;
    def->common.next = রিক্ত;
    def->sym = sym;
    def->match = match;
    def->def = রিক্ত;

    ঘূৰাই def;
}

চাবি_Type_Def *
চাবি_Type_Create(xkb_atom_t name, VarDef *body)
{
    চাবি_Type_Def *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_TYPE;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ঘূৰাই def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t চাবি_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_SYMBOLS;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->চাবি_Name = চাবি_Name;
    def->symbols = symbols;

    ঘূৰাই def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, ৰাশিমালা_Def *val)
{
    GroupCompatDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ঘূৰাই def;
}

ModMapDef *
ModMapCreate(ৰাশিমালা_Def *modifiers, ৰাশিমালা_Def *চাবিগুলি)
{
    ModMapDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_MODMAP;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->চাবিগুলি = চাবিগুলি;

    ঘূৰাই def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_LED_MAP;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ঘূৰাই def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ৰাশিমালা_Def *name, bool আভাস)
{
    LedNameDef *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = STMT_LED_NAME;
    def->common.next = রিক্ত;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->আভাস = আভাস;

    ঘূৰাই def;
}

UnknownStatement *
UnknownStatementCreate(গণনা stmt_type type, সংরচনা sval name)
{
    UnknownStatement *def = malloc(পরিমাণ(*def));
    যদি (!def)
        ঘূৰাই রিক্ত;

    def->common.type = type;
    def->common.next = রিক্ত;
    def->name = strndup(name.start, name.len);
    যদি (!def->name) {
        মুক্ত(def);
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই def;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(সংরচনা xkb_context *ctx, ৱর্ণ *str, গণনা merge_mode merge)
{
    IncludeStmt *incl, *first;
    ৱর্ণ *stmt, *tmp;
    ৱর্ণ nextop;

    incl = first = রিক্ত;
    tmp = str;
    stmt = strdup_safe(str);
    যেতিয়ালৈ (tmp && *tmp)
    {
        ৱর্ণ *file = রিক্ত, *map = রিক্ত, *extra_data = রিক্ত;

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
            অনুৱর্ত;
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

        ৱিকল্প (nextop) {
        পক্ষ MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            ভঙ্গ;
        পক্ষ MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            ভঙ্গ;
        স্ৱভাৱ:
            merge = MERGE_OVERRIDE;
        }
    }

    যদি (first)
        first->stmt = stmt;
    অন্যথা
        মুক্ত(stmt);

    ঘূৰাই first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    মুক্ত(stmt);
    ঘূৰাই রিক্ত;
}

XkbFile *
XkbFileCreate(গণনা xkb_file_type type, ৱর্ণ *name, ParseCommon *defs,
              গণনা xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, পরিমাণ(*file));
    যদি (!file)
        ঘূৰাই রিক্ত;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ঘূৰাই file;
}

XkbFile *
XkbFileFromComponents(সংরচনা xkb_context *ctx,
                      ধ্রুৱ সংরচনা xkb_component_names *kkctgs)
{
    ৱর্ণ *ধ্রুৱ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    গণনা xkb_file_type type;
    IncludeStmt *include = রিক্ত;
    XkbFile *file = রিক্ত;
    ParseCommon *defs = রিক্ত, *defsLast = রিক্ত;

    বাবে (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
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

    ঘূৰাই file;

err:
    FreeXkbFile((XkbFile *) defs);
    ঘূৰাই রিক্ত;
}

স্থির শূন্য
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    যেতিয়ালৈ (incl)
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

    যেতিয়ালৈ (stmt)
    {
        next = stmt->next;

        ৱিকল্প (stmt->type) {
        পক্ষ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = রিক্ত;
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_NEGATE:
        পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
        পক্ষ STMT_ৰাশিমালা_NOT:
        পক্ষ STMT_ৰাশিমালা_INVERT:
            FreeStmt((ParseCommon *) ((ৰাশিমালা_Unary *) stmt)->child);
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_DIVIDE:
        পক্ষ STMT_ৰাশিমালা_ADD:
        পক্ষ STMT_ৰাশিমালা_SUBTRACT:
        পক্ষ STMT_ৰাশিমালা_MULTIPLY:
        পক্ষ STMT_ৰাশিমালা_ASSIGN:
            FreeStmt((ParseCommon *) ((ৰাশিমালা_দ্বিমিক *) stmt)->বাম);
            FreeStmt((ParseCommon *) ((ৰাশিমালা_দ্বিমিক *) stmt)->ডান);
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ৰাশিমালা_Action *) stmt)->args);
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ৰাশিমালা_Action_List *) stmt)->actions);
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ৰাশিমালা_Array_Ref *) stmt)->entry);
            ভঙ্গ;

        পক্ষ STMT_ৰাশিমালা_KEYSYM_LIST:
            darray_free(((ৰাশিমালা_Keysym_List *) stmt)->syms);
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
        স্ৱভাৱ:
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

    যেতিয়ালৈ (file)
    {
        next = (XkbFile *) file->common.next;

        ৱিকল্প (file->file_type) {
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

        স্ৱভাৱ:
            ভঙ্গ;
        }

        মুক্ত(file->name);
        মুক্ত(file);
        file = next;
    }
}

স্থির ধ্রুৱ ৱর্ণ *xkb_file_type_strings[FILE_TYPE_অঙ্ক_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ধ্রুৱ ৱর্ণ *
xkb_file_type_to_string(গণনা xkb_file_type type)
{
    যদি (type >= FILE_TYPE_অঙ্ক_ENTRIES)
        ঘূৰাই "unknown";
    ঘূৰাই xkb_file_type_strings[type];
}

স্থির ধ্রুৱ ৱর্ণ *stmt_type_strings[STMT_অঙ্ক_মানসমূহ] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_ৰাশিমালা_STRING_LITERAL] = "string literal expression",
    [STMT_ৰাশিমালা_INTEGER_LITERAL] = "integer literal expression",
    [STMT_ৰাশিমালা_FLOAT_LITERAL] = "float literal expression",
    [STMT_ৰাশিমালা_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_ৰাশিমালা_KEYNAME_LITERAL] = "key name expression",
    [STMT_ৰাশিমালা_KEYSYM_LITERAL] = "keysym expression",
    [STMT_ৰাশিমালা_IDENT] = "identifier expression",
    [STMT_ৰাশিমালা_ACTION_DECL] = "action declaration expression",
    [STMT_ৰাশিমালা_FIELD_REF] = "field reference expression",
    [STMT_ৰাশিমালা_ARRAY_REF] = "array reference expression",
    [STMT_ৰাশিমালা_EMPTY_LIST] = "empty list expression",
    [STMT_ৰাশিমালা_KEYSYM_LIST] = "keysym list expression",
    [STMT_ৰাশিমালা_ACTION_LIST] = "action list expression",
    [STMT_ৰাশিমালা_ADD] = "addition expression",
    [STMT_ৰাশিমালা_SUBTRACT] = "substraction expression",
    [STMT_ৰাশিমালা_MULTIPLY] = "multiplication expression",
    [STMT_ৰাশিমালা_DIVIDE] = "division expression",
    [STMT_ৰাশিমালা_ASSIGN] = "assignment expression",
    [STMT_ৰাশিমালা_NOT] = "logical negation expression",
    [STMT_ৰাশিমালা_NEGATE] = "arithmetic negation expression",
    [STMT_ৰাশিমালা_INVERT] = "bitwise inversion expression",
    [STMT_ৰাশিমালা_UNARY_PLUS] = "unary plus expression",
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

ধ্রুৱ ৱর্ণ *
stmt_type_to_string(গণনা stmt_type type)
{
    যদি (type >= STMT_অঙ্ক_মানসমূহ)
        ঘূৰাই রিক্ত;
    ঘূৰাই stmt_type_strings[type];
}

ৱর্ণ
stmt_type_to_operator_char(গণনা stmt_type type)
{
    ৱিকল্প (type) {
    পক্ষ STMT_ৰাশিমালা_ADD:
        ঘূৰাই '+';
    পক্ষ STMT_ৰাশিমালা_SUBTRACT:
        ঘূৰাই '-';
    পক্ষ STMT_ৰাশিমালা_MULTIPLY:
        ঘূৰাই '*';
    পক্ষ STMT_ৰাশিমালা_DIVIDE:
        ঘূৰাই '/';
    পক্ষ STMT_ৰাশিমালা_NOT:
        ঘূৰাই '!';
    পক্ষ STMT_ৰাশিমালা_NEGATE:
        ঘূৰাই '-';
    পক্ষ STMT_ৰাশিমালা_INVERT:
        ঘূৰাই '~';
    পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
        ঘূৰাই '+';
    স্ৱভাৱ:
        ঘূৰাই '\0';
    }
}
