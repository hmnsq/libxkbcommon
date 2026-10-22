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

#شامل "config.h"

#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "messages-codes.h"
#شامل "xkbcomp-priv.h"
#شامل "ast.h"
#شامل "ast-build.h"
#شامل "include.h"
#شامل "keysym.h"
#شامل "utf8-decoding.h"

ایستا عبارت_Def *
عبارت_Create(شمارش stmt_type op)
{
    عبارت_Def *عبارت = malloc(اندازه(*عبارت));
    اگر (!عبارت)
        بازگشت پوچ;

    عبارت->common.type = op;
    عبارت->common.next = پوچ;

    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_String(xkb_atom_t str)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_STRING_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->string.str = str;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Integer(int64_t ival)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_INTEGER_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->integer.ival = ival;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Float(تهی)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_FLOAT_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Boolean(bool set)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_BOOLEAN_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->boolean.set = set;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Name(xkb_atom_t کلید_name)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYNAME_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->کلید_name.کلید_name = کلید_name;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Sym(xkb_keysym_t keysym)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYSYM_LITERAL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->keysym.keysym = keysym;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Ident(xkb_atom_t ident)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_IDENT);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->ident.ident = ident;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Unary(شمارش stmt_type op, عبارت_Def *child)
{
    عبارت_Def *عبارت = عبارت_Create(op);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->unary.child = child;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_دودویی(شمارش stmt_type op, عبارت_Def *چپ, عبارت_Def *راست)
{
    عبارت_Def *عبارت = عبارت_Create(op);
    اگر (!عبارت)
        بازگشت پوچ;

    عبارت->دودویی.چپ = چپ;
    عبارت->دودویی.راست = راست;

    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_FIELD_REF);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->field_ref.element = element;
    عبارت->field_ref.field = field;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, عبارت_Def *entry)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ARRAY_REF);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->array_ref.element = element;
    عبارت->array_ref.field = field;
    عبارت->array_ref.entry = entry;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Empty_List(تهی)
{
    بازگشت عبارت_Create(STMT_عبارت_EMPTY_LIST);
}

عبارت_Def *
عبارت_Create_Action(xkb_atom_t name, عبارت_Def *args)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ACTION_DECL);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->action.name = name;
    عبارت->action.args = args;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_Action_List(عبارت_Def *actions)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ACTION_LIST);
    اگر (!عبارت)
        بازگشت پوچ;
    عبارت->actions.actions = actions;
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Sym_List(xkb_keysym_t sym)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYSYM_LIST);
    اگر (!عبارت)
        بازگشت پوچ;
    darray_init(عبارت->keysym_list.syms);
    اگر (sym == XKB_کلید_No_نماد) {
        /* Discard NoSymbol */
    } وگرنه {
        darray_append(عبارت->keysym_list.syms, sym);
    }
    بازگشت عبارت;
}

عبارت_Def *
عبارت_Append_کلید_Sym_List(عبارت_Def *عبارت, xkb_keysym_t sym)
{
    اگر (sym == XKB_کلید_No_نماد) {
        /* Discard NoSymbol */
    } وگرنه {
        darray_append(عبارت->keysym_list.syms, sym);
    }
    بازگشت عبارت;
}

عبارت_Def *
عبارت_کلید_Sym_List_Append_String(ساختار scanner *scanner,
                           عبارت_Def *عبارت, ثابت نویسه *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    وقتی (idx < len) {
        size_t شمار = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &شمار);
        اگر (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            برو error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        اگر (sym == XKB_کلید_No_نماد) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            برو error;
        }
        darray_append(عبارت->keysym_list.syms, sym);
        idx += شمار;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    بازگشت عبارت;
error:
    FreeStmt((ParseCommon*) عبارت);
    بازگشت پوچ;
}

xkb_keysym_t
KeysymParseString(ساختار scanner *scanner, ثابت نویسه *string)
{
    ثابت size_t len = strlen(string);
    اگر (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        بازگشت XKB_کلید_No_نماد;
    }
    size_t شمار = 0;
    ثابت uint32_t cp = utf8_next_code_point(string, len, &شمار);
    اگر (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        بازگشت XKB_کلید_No_نماد;
    } وگرنه اگر (شمار != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        بازگشت XKB_کلید_No_نماد;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    اگر (sym == XKB_کلید_No_نماد) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    بازگشت sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t مقدار)
{
    KeycodeDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_KEYCODE;
    def->common.next = پوچ;
    def->name = name;
    def->مقدار = مقدار;

    بازگشت def;
}

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    کلید_Alias_Def *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_ALIAS;
    def->common.next = پوچ;
    def->alias = alias;
    def->real = real;

    بازگشت def;
}

VModDef *
VModCreate(xkb_atom_t name, عبارت_Def *مقدار)
{
    VModDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_VMOD;
    def->common.next = پوچ;
    def->name = name;
    def->مقدار = مقدار;

    بازگشت def;
}

VarDef *
VarCreate(عبارت_Def *name, عبارت_Def *مقدار)
{
    VarDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_VAR;
    def->common.next = پوچ;
    def->name = name;
    def->مقدار = مقدار;

    بازگشت def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    عبارت_Def *name, *مقدار;
    VarDef *def;
    اگر (!(name = عبارت_Create_Ident(ident))) {
        بازگشت پوچ;
    }
    اگر (!(مقدار = عبارت_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        بازگشت پوچ;
    }
    اگر (!(def = VarCreate(name, مقدار))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) مقدار);
        بازگشت پوچ;
    }
    بازگشت def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, عبارت_Def *match)
{
    InterpDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_INTERP;
    def->common.next = پوچ;
    def->sym = sym;
    def->match = match;
    def->def = پوچ;

    بازگشت def;
}

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body)
{
    کلید_Type_Def *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_TYPE;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    بازگشت def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_SYMBOLS;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->کلید_Name = کلید_Name;
    def->symbols = symbols;

    بازگشت def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, عبارت_Def *val)
{
    GroupCompatDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    بازگشت def;
}

ModMapDef *
ModMapCreate(عبارت_Def *modifiers, عبارت_Def *کلیدها)
{
    ModMapDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_MODMAP;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->کلیدها = کلیدها;

    بازگشت def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_LED_MAP;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    بازگشت def;
}

LedNameDef *
LedNameCreate(int64_t ndx, عبارت_Def *name, bool مجازی)
{
    LedNameDef *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = STMT_LED_NAME;
    def->common.next = پوچ;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->مجازی = مجازی;

    بازگشت def;
}

UnknownStatement *
UnknownStatementCreate(شمارش stmt_type type, ساختار sval name)
{
    UnknownStatement *def = malloc(اندازه(*def));
    اگر (!def)
        بازگشت پوچ;

    def->common.type = type;
    def->common.next = پوچ;
    def->name = strndup(name.start, name.len);
    اگر (!def->name) {
        آزاد(def);
        بازگشت پوچ;
    }

    بازگشت def;
}

ایستا تهی
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ساختار xkb_context *ctx, نویسه *str, شمارش merge_mode merge)
{
    IncludeStmt *incl, *first;
    نویسه *stmt, *tmp;
    نویسه nextop;

    incl = first = پوچ;
    tmp = str;
    stmt = strdup_safe(str);
    وقتی (tmp && *tmp)
    {
        نویسه *file = پوچ, *map = پوچ, *extra_data = پوچ;

        اگر (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            برو err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        اگر (isempty(file)) {
            آزاد(file);
            آزاد(map);
            آزاد(extra_data);
            ادامه;
        }

        اگر (first == پوچ) {
            first = incl = malloc(اندازه(*first));
        } وگرنه {
            incl->next_incl = malloc(اندازه(*first));
            incl = incl->next_incl;
        }

        اگر (!incl) {
            آزاد(file);
            آزاد(map);
            آزاد(extra_data);
            بشکن;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = پوچ;
        incl->merge = merge;
        incl->stmt = پوچ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = پوچ;

        انتخاب (nextop) {
        حالت MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            بشکن;
        حالت MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            بشکن;
        پیشفرض:
            merge = MERGE_OVERRIDE;
        }
    }

    اگر (first)
        first->stmt = stmt;
    وگرنه
        آزاد(stmt);

    بازگشت first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    آزاد(stmt);
    بازگشت پوچ;
}

XkbFile *
XkbFileCreate(شمارش xkb_file_type type, نویسه *name, ParseCommon *defs,
              شمارش xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, اندازه(*file));
    اگر (!file)
        بازگشت پوچ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    بازگشت file;
}

XkbFile *
XkbFileFromComponents(ساختار xkb_context *ctx,
                      ثابت ساختار xkb_component_names *kkctgs)
{
    نویسه *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    شمارش xkb_file_type type;
    IncludeStmt *include = پوچ;
    XkbFile *file = پوچ;
    ParseCommon *defs = پوچ, *defsLast = پوچ;

    برای (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        اگر (!include)
            برو err;

        file = XkbFileCreate(type, پوچ, (ParseCommon *) include, 0);
        اگر (!file) {
            FreeInclude(include);
            برو err;
        }

        اگر (!defs)
            defsLast = defs = &file->common;
        وگرنه
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, پوچ, defs, 0);
    اگر (!file)
        برو err;

    بازگشت file;

err:
    FreeXkbFile((XkbFile *) defs);
    بازگشت پوچ;
}

ایستا تهی
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    وقتی (incl)
    {
        next = incl->next_incl;

        آزاد(incl->file);
        آزاد(incl->map);
        آزاد(incl->modifier);
        آزاد(incl->stmt);

        آزاد(incl);
        incl = next;
    }
}

تهی
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    وقتی (stmt)
    {
        next = stmt->next;

        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = پوچ;
            بشکن;

        حالت STMT_عبارت_NEGATE:
        حالت STMT_عبارت_UNARY_PLUS:
        حالت STMT_عبارت_NOT:
        حالت STMT_عبارت_INVERT:
            FreeStmt((ParseCommon *) ((عبارت_Unary *) stmt)->child);
            بشکن;

        حالت STMT_عبارت_DIVIDE:
        حالت STMT_عبارت_ADD:
        حالت STMT_عبارت_SUBTRACT:
        حالت STMT_عبارت_MULTIPLY:
        حالت STMT_عبارت_ASSIGN:
            FreeStmt((ParseCommon *) ((عبارت_دودویی *) stmt)->چپ);
            FreeStmt((ParseCommon *) ((عبارت_دودویی *) stmt)->راست);
            بشکن;

        حالت STMT_عبارت_ACTION_DECL:
            FreeStmt((ParseCommon *) ((عبارت_Action *) stmt)->args);
            بشکن;

        حالت STMT_عبارت_ACTION_LIST:
            FreeStmt((ParseCommon *) ((عبارت_Action_List *) stmt)->actions);
            بشکن;

        حالت STMT_عبارت_ARRAY_REF:
            FreeStmt((ParseCommon *) ((عبارت_Array_Ref *) stmt)->entry);
            بشکن;

        حالت STMT_عبارت_KEYSYM_LIST:
            darray_free(((عبارت_Keysym_List *) stmt)->syms);
            بشکن;

        حالت STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->مقدار);
            بشکن;
        حالت STMT_TYPE:
            FreeStmt((ParseCommon *) ((کلید_Type_Def *) stmt)->body);
            بشکن;
        حالت STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            بشکن;
        حالت STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->مقدار);
            بشکن;
        حالت STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            بشکن;
        حالت STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->کلیدها);
            بشکن;
        حالت STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            بشکن;
        حالت STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            بشکن;
        حالت STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            بشکن;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            آزاد(((UnknownStatement *) stmt)->name);
            بشکن;
        پیشفرض:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_رقم_مقادیر == 37 &&
                          STMT_رقم_مقادیر == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            بشکن;
        }

        آزاد(stmt);
        stmt = next;
    }
}

تهی
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    وقتی (file)
    {
        next = (XkbFile *) file->common.next;

        انتخاب (file->file_type) {
        حالت FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            بشکن;

        حالت FILE_TYPE_TYPES:
        حالت FILE_TYPE_COMPAT:
        حالت FILE_TYPE_SYMBOLS:
        حالت FILE_TYPE_KEYCODES:
        حالت FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            بشکن;

        پیشفرض:
            بشکن;
        }

        آزاد(file->name);
        آزاد(file);
        file = next;
    }
}

ایستا ثابت نویسه *xkb_file_type_strings[FILE_TYPE_رقم_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ثابت نویسه *
xkb_file_type_to_string(شمارش xkb_file_type type)
{
    اگر (type >= FILE_TYPE_رقم_ENTRIES)
        بازگشت "unknown";
    بازگشت xkb_file_type_strings[type];
}

ایستا ثابت نویسه *stmt_type_strings[STMT_رقم_مقادیر] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_عبارت_STRING_LITERAL] = "string literal expression",
    [STMT_عبارت_INTEGER_LITERAL] = "integer literal expression",
    [STMT_عبارت_FLOAT_LITERAL] = "float literal expression",
    [STMT_عبارت_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_عبارت_KEYNAME_LITERAL] = "key name expression",
    [STMT_عبارت_KEYSYM_LITERAL] = "keysym expression",
    [STMT_عبارت_IDENT] = "identifier expression",
    [STMT_عبارت_ACTION_DECL] = "action declaration expression",
    [STMT_عبارت_FIELD_REF] = "field reference expression",
    [STMT_عبارت_ARRAY_REF] = "array reference expression",
    [STMT_عبارت_EMPTY_LIST] = "empty list expression",
    [STMT_عبارت_KEYSYM_LIST] = "keysym list expression",
    [STMT_عبارت_ACTION_LIST] = "action list expression",
    [STMT_عبارت_ADD] = "addition expression",
    [STMT_عبارت_SUBTRACT] = "substraction expression",
    [STMT_عبارت_MULTIPLY] = "multiplication expression",
    [STMT_عبارت_DIVIDE] = "division expression",
    [STMT_عبارت_ASSIGN] = "assignment expression",
    [STMT_عبارت_NOT] = "logical negation expression",
    [STMT_عبارت_NEGATE] = "arithmetic negation expression",
    [STMT_عبارت_INVERT] = "bitwise inversion expression",
    [STMT_عبارت_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_رقم_مقادیر == 37 &&
              STMT_رقم_مقادیر == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت نویسه *
stmt_type_to_string(شمارش stmt_type type)
{
    اگر (type >= STMT_رقم_مقادیر)
        بازگشت پوچ;
    بازگشت stmt_type_strings[type];
}

نویسه
stmt_type_to_operator_char(شمارش stmt_type type)
{
    انتخاب (type) {
    حالت STMT_عبارت_ADD:
        بازگشت '+';
    حالت STMT_عبارت_SUBTRACT:
        بازگشت '-';
    حالت STMT_عبارت_MULTIPLY:
        بازگشت '*';
    حالت STMT_عبارت_DIVIDE:
        بازگشت '/';
    حالت STMT_عبارت_NOT:
        بازگشت '!';
    حالت STMT_عبارت_NEGATE:
        بازگشت '-';
    حالت STMT_عبارت_INVERT:
        بازگشت '~';
    حالت STMT_عبارت_UNARY_PLUS:
        بازگشت '+';
    پیشفرض:
        بازگشت '\0';
    }
}
