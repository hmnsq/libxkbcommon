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

#ضمن "config.h"

#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon-keysyms.h"
#ضمن "messages-codes.h"
#ضمن "xkbcomp-priv.h"
#ضمن "ast.h"
#ضمن "ast-build.h"
#ضمن "include.h"
#ضمن "keysym.h"
#ضمن "utf8-decoding.h"

ساكن ExprDef *
ExprCreate(تعداد stmt_type op)
{
    ExprDef *expr = malloc(حجم(*expr));
    ܐܢ (!expr)
        ܦܢܐ عدم;

    expr->common.type = op;
    expr->common.next = عدم;

    ܦܢܐ expr;
}

ExprDef *
ExprCreateString(xkb_atom_t str)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_STRING_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->string.str = str;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateInteger(int64_t ival)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_INTEGER_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->integer.ival = ival;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateFloat(فراغ)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_FLOAT_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateBoolean(bool set)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_BOOLEAN_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->boolean.set = set;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateKeyName(xkb_atom_t key_name)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYNAME_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->key_name.key_name = key_name;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateKeySym(xkb_keysym_t keysym)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYSYM_LITERAL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->keysym.keysym = keysym;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateIdent(xkb_atom_t ident)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_IDENT);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->ident.ident = ident;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateUnary(تعداد stmt_type op, ExprDef *child)
{
    ExprDef *expr = ExprCreate(op);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->unary.child = child;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateBinary(تعداد stmt_type op, ExprDef *left, ExprDef *right)
{
    ExprDef *expr = ExprCreate(op);
    ܐܢ (!expr)
        ܦܢܐ عدم;

    expr->binary.left = left;
    expr->binary.right = right;

    ܦܢܐ expr;
}

ExprDef *
ExprCreateFieldRef(xkb_atom_t element, xkb_atom_t field)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_FIELD_REF);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->field_ref.element = element;
    expr->field_ref.field = field;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateArrayRef(xkb_atom_t element, xkb_atom_t field, ExprDef *entry)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ARRAY_REF);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->array_ref.element = element;
    expr->array_ref.field = field;
    expr->array_ref.entry = entry;
    ܦܢܐ expr;
}

ExprDef *
ExprEmptyList(فراغ)
{
    ܦܢܐ ExprCreate(STMT_EXPR_EMPTY_LIST);
}

ExprDef *
ExprCreateAction(xkb_atom_t name, ExprDef *args)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ACTION_DECL);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->action.name = name;
    expr->action.args = args;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateActionList(ExprDef *actions)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ACTION_LIST);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    expr->actions.actions = actions;
    ܦܢܐ expr;
}

ExprDef *
ExprCreateKeySymList(xkb_keysym_t sym)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYSYM_LIST);
    ܐܢ (!expr)
        ܦܢܐ عدم;
    darray_init(expr->keysym_list.syms);
    ܐܢ (sym == XKB_KEY_NoSymbol) {
        /* Discard NoSymbol */
    } ܐܚܪܢܐ {
        darray_append(expr->keysym_list.syms, sym);
    }
    ܦܢܐ expr;
}

ExprDef *
ExprAppendKeySymList(ExprDef *expr, xkb_keysym_t sym)
{
    ܐܢ (sym == XKB_KEY_NoSymbol) {
        /* Discard NoSymbol */
    } ܐܚܪܢܐ {
        darray_append(expr->keysym_list.syms, sym);
    }
    ܦܢܐ expr;
}

ExprDef *
ExprKeySymListAppendString(بنية scanner *scanner,
                           ExprDef *expr, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    ܟܡܐ (idx < len) {
        size_t count = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &count);
        ܐܢ (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            اذهب error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        ܐܢ (sym == XKB_KEY_NoSymbol) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            اذهب error;
        }
        darray_append(expr->keysym_list.syms, sym);
        idx += count;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    ܦܢܐ expr;
error:
    FreeStmt((ParseCommon*) expr);
    ܦܢܐ عدم;
}

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string)
{
    ثابت size_t len = strlen(string);
    ܐܢ (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ܦܢܐ XKB_KEY_NoSymbol;
    }
    size_t count = 0;
    ثابت uint32_t cp = utf8_next_code_point(string, len, &count);
    ܐܢ (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ܦܢܐ XKB_KEY_NoSymbol;
    } ܐܚܪܢܐ ܐܢ (count != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ܦܢܐ XKB_KEY_NoSymbol;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    ܐܢ (sym == XKB_KEY_NoSymbol) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ܦܢܐ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t value)
{
    KeycodeDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_KEYCODE;
    def->common.next = عدم;
    def->name = name;
    def->value = value;

    ܦܢܐ def;
}

KeyAliasDef *
KeyAliasCreate(xkb_atom_t alias, xkb_atom_t real)
{
    KeyAliasDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_ALIAS;
    def->common.next = عدم;
    def->alias = alias;
    def->real = real;

    ܦܢܐ def;
}

VModDef *
VModCreate(xkb_atom_t name, ExprDef *value)
{
    VModDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_VMOD;
    def->common.next = عدم;
    def->name = name;
    def->value = value;

    ܦܢܐ def;
}

VarDef *
VarCreate(ExprDef *name, ExprDef *value)
{
    VarDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_VAR;
    def->common.next = عدم;
    def->name = name;
    def->value = value;

    ܦܢܐ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    ExprDef *name, *value;
    VarDef *def;
    ܐܢ (!(name = ExprCreateIdent(ident))) {
        ܦܢܐ عدم;
    }
    ܐܢ (!(value = ExprCreateBoolean(set))) {
        FreeStmt((ParseCommon *) name);
        ܦܢܐ عدم;
    }
    ܐܢ (!(def = VarCreate(name, value))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) value);
        ܦܢܐ عدم;
    }
    ܦܢܐ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ExprDef *match)
{
    InterpDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_INTERP;
    def->common.next = عدم;
    def->sym = sym;
    def->match = match;
    def->def = عدم;

    ܦܢܐ def;
}

KeyTypeDef *
KeyTypeCreate(xkb_atom_t name, VarDef *body)
{
    KeyTypeDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_TYPE;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ܦܢܐ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t keyName, VarDef *symbols)
{
    SymbolsDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_SYMBOLS;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->keyName = keyName;
    def->symbols = symbols;

    ܦܢܐ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, ExprDef *val)
{
    GroupCompatDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ܦܢܐ def;
}

ModMapDef *
ModMapCreate(ExprDef *modifiers, ExprDef *keys)
{
    ModMapDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_MODMAP;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->keys = keys;

    ܦܢܐ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_LED_MAP;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ܦܢܐ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ExprDef *name, bool virtual)
{
    LedNameDef *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = STMT_LED_NAME;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->virtual = virtual;

    ܦܢܐ def;
}

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type type, بنية sval name)
{
    UnknownStatement *def = malloc(حجم(*def));
    ܐܢ (!def)
        ܦܢܐ عدم;

    def->common.type = type;
    def->common.next = عدم;
    def->name = strndup(name.start, name.len);
    ܐܢ (!def->name) {
        حرر(def);
        ܦܢܐ عدم;
    }

    ܦܢܐ def;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد merge_mode merge)
{
    IncludeStmt *incl, *first;
    حرف *stmt, *tmp;
    حرف nextop;

    incl = first = عدم;
    tmp = str;
    stmt = strdup_safe(str);
    ܟܡܐ (tmp && *tmp)
    {
        حرف *file = عدم, *map = عدم, *extra_data = عدم;

        ܐܢ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            اذهب err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ܐܢ (isempty(file)) {
            حرر(file);
            حرر(map);
            حرر(extra_data);
            استمر;
        }

        ܐܢ (first == عدم) {
            first = incl = malloc(حجم(*first));
        } ܐܚܪܢܐ {
            incl->next_incl = malloc(حجم(*first));
            incl = incl->next_incl;
        }

        ܐܢ (!incl) {
            حرر(file);
            حرر(map);
            حرر(extra_data);
            اكسر;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = عدم;
        incl->merge = merge;
        incl->stmt = عدم;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = عدم;

        بدل (nextop) {
        حالة MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            اكسر;
        حالة MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            اكسر;
        افتراضي:
            merge = MERGE_OVERRIDE;
        }
    }

    ܐܢ (first)
        first->stmt = stmt;
    ܐܚܪܢܐ
        حرر(stmt);

    ܦܢܐ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    حرر(stmt);
    ܦܢܐ عدم;
}

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, حجم(*file));
    ܐܢ (!file)
        ܦܢܐ عدم;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ܦܢܐ file;
}

XkbFile *
XkbFileFromComponents(بنية xkb_context *ctx,
                      ثابت بنية xkb_component_names *kkctgs)
{
    حرف *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    تعداد xkb_file_type type;
    IncludeStmt *include = عدم;
    XkbFile *file = عدم;
    ParseCommon *defs = عدم, *defsLast = عدم;

    لكل (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        ܐܢ (!include)
            اذهب err;

        file = XkbFileCreate(type, عدم, (ParseCommon *) include, 0);
        ܐܢ (!file) {
            FreeInclude(include);
            اذهب err;
        }

        ܐܢ (!defs)
            defsLast = defs = &file->common;
        ܐܚܪܢܐ
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, عدم, defs, 0);
    ܐܢ (!file)
        اذهب err;

    ܦܢܐ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ܦܢܐ عدم;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    ܟܡܐ (incl)
    {
        next = incl->next_incl;

        حرر(incl->file);
        حرر(incl->map);
        حرر(incl->modifier);
        حرر(incl->stmt);

        حرر(incl);
        incl = next;
    }
}

فراغ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    ܟܡܐ (stmt)
    {
        next = stmt->next;

        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = عدم;
            اكسر;

        حالة STMT_EXPR_NEGATE:
        حالة STMT_EXPR_UNARY_PLUS:
        حالة STMT_EXPR_NOT:
        حالة STMT_EXPR_INVERT:
            FreeStmt((ParseCommon *) ((ExprUnary *) stmt)->child);
            اكسر;

        حالة STMT_EXPR_DIVIDE:
        حالة STMT_EXPR_ADD:
        حالة STMT_EXPR_SUBTRACT:
        حالة STMT_EXPR_MULTIPLY:
        حالة STMT_EXPR_ASSIGN:
            FreeStmt((ParseCommon *) ((ExprBinary *) stmt)->left);
            FreeStmt((ParseCommon *) ((ExprBinary *) stmt)->right);
            اكسر;

        حالة STMT_EXPR_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ExprAction *) stmt)->args);
            اكسر;

        حالة STMT_EXPR_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ExprActionList *) stmt)->actions);
            اكسر;

        حالة STMT_EXPR_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ExprArrayRef *) stmt)->entry);
            اكسر;

        حالة STMT_EXPR_KEYSYM_LIST:
            darray_free(((ExprKeysymList *) stmt)->syms);
            اكسر;

        حالة STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->value);
            اكسر;
        حالة STMT_TYPE:
            FreeStmt((ParseCommon *) ((KeyTypeDef *) stmt)->body);
            اكسر;
        حالة STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            اكسر;
        حالة STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->value);
            اكسر;
        حالة STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            اكسر;
        حالة STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->keys);
            اكسر;
        حالة STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            اكسر;
        حالة STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            اكسر;
        حالة STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            حرر(((UnknownStatement *) stmt)->name);
            اكسر;
        افتراضي:
            {} /* Label followed by declaration requires C23 */
            static_assert(_STMT_NUM_VALUES == 37 &&
                          _STMT_NUM_VALUES == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            اكسر;
        }

        حرر(stmt);
        stmt = next;
    }
}

فراغ
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    ܟܡܐ (file)
    {
        next = (XkbFile *) file->common.next;

        بدل (file->file_type) {
        حالة FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            اكسر;

        حالة FILE_TYPE_TYPES:
        حالة FILE_TYPE_COMPAT:
        حالة FILE_TYPE_SYMBOLS:
        حالة FILE_TYPE_KEYCODES:
        حالة FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            اكسر;

        افتراضي:
            اكسر;
        }

        حرر(file->name);
        حرر(file);
        file = next;
    }
}

ساكن ثابت حرف *xkb_file_type_strings[_FILE_TYPE_NUM_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

ثابت حرف *
xkb_file_type_to_string(تعداد xkb_file_type type)
{
    ܐܢ (type >= _FILE_TYPE_NUM_ENTRIES)
        ܦܢܐ "unknown";
    ܦܢܐ xkb_file_type_strings[type];
}

ساكن ثابت حرف *stmt_type_strings[_STMT_NUM_VALUES] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_EXPR_STRING_LITERAL] = "string literal expression",
    [STMT_EXPR_INTEGER_LITERAL] = "integer literal expression",
    [STMT_EXPR_FLOAT_LITERAL] = "float literal expression",
    [STMT_EXPR_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_EXPR_KEYNAME_LITERAL] = "key name expression",
    [STMT_EXPR_KEYSYM_LITERAL] = "keysym expression",
    [STMT_EXPR_IDENT] = "identifier expression",
    [STMT_EXPR_ACTION_DECL] = "action declaration expression",
    [STMT_EXPR_FIELD_REF] = "field reference expression",
    [STMT_EXPR_ARRAY_REF] = "array reference expression",
    [STMT_EXPR_EMPTY_LIST] = "empty list expression",
    [STMT_EXPR_KEYSYM_LIST] = "keysym list expression",
    [STMT_EXPR_ACTION_LIST] = "action list expression",
    [STMT_EXPR_ADD] = "addition expression",
    [STMT_EXPR_SUBTRACT] = "substraction expression",
    [STMT_EXPR_MULTIPLY] = "multiplication expression",
    [STMT_EXPR_DIVIDE] = "division expression",
    [STMT_EXPR_ASSIGN] = "assignment expression",
    [STMT_EXPR_NOT] = "logical negation expression",
    [STMT_EXPR_NEGATE] = "arithmetic negation expression",
    [STMT_EXPR_INVERT] = "bitwise inversion expression",
    [STMT_EXPR_UNARY_PLUS] = "unary plus expression",
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
static_assert(_STMT_NUM_VALUES == 37 &&
              _STMT_NUM_VALUES == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_to_string(تعداد stmt_type type)
{
    ܐܢ (type >= _STMT_NUM_VALUES)
        ܦܢܐ عدم;
    ܦܢܐ stmt_type_strings[type];
}

حرف
stmt_type_to_operator_char(تعداد stmt_type type)
{
    بدل (type) {
    حالة STMT_EXPR_ADD:
        ܦܢܐ '+';
    حالة STMT_EXPR_SUBTRACT:
        ܦܢܐ '-';
    حالة STMT_EXPR_MULTIPLY:
        ܦܢܐ '*';
    حالة STMT_EXPR_DIVIDE:
        ܦܢܐ '/';
    حالة STMT_EXPR_NOT:
        ܦܢܐ '!';
    حالة STMT_EXPR_NEGATE:
        ܦܢܐ '-';
    حالة STMT_EXPR_INVERT:
        ܦܢܐ '~';
    حالة STMT_EXPR_UNARY_PLUS:
        ܦܢܐ '+';
    افتراضي:
        ܦܢܐ '\0';
    }
}
