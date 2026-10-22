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

#include "config.h"

#include <stdint.h>
#include <string.h>

#include "xkbcommon/xkbcommon-keysyms.h"
#include "messages-codes.h"
#include "xkbcomp-priv.h"
#include "ast.h"
#include "ast-build.h"
#include "include.h"
#include "keysym.h"
#include "utf8-decoding.h"

static ExprDef *
ExprCreate(ga_nanaa stmt_type op)
{
    ExprDef *expr = malloc(parimaa_na(*expr));
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;

    expr->common.type = op;
    expr->common.next = NULL;

    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateString(xkb_atom_t str)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_STRING_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->string.str = str;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateInteger(int64_t ival)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_INTEGER_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->integer.ival = ival;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateFloat(void)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_FLOAT_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateBoolean(bool set)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_BOOLEAN_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->boolean.set = set;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateKeyName(xkb_atom_t key_name)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYNAME_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->key_name.key_name = key_name;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateKeySym(xkb_keysym_t keysym)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYSYM_LITERAL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->keysym.keysym = keysym;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateIdent(xkb_atom_t ident)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_IDENT);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->ident.ident = ident;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateUnary(ga_nanaa stmt_type op, ExprDef *child)
{
    ExprDef *expr = ExprCreate(op);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->unary.child = child;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateBinary(ga_nanaa stmt_type op, ExprDef *left, ExprDef *right)
{
    ExprDef *expr = ExprCreate(op);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;

    expr->binary.left = left;
    expr->binary.right = right;

    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateFieldRef(xkb_atom_t element, xkb_atom_t field)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_FIELD_REF);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->field_ref.element = element;
    expr->field_ref.field = field;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateArrayRef(xkb_atom_t element, xkb_atom_t field, ExprDef *entry)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ARRAY_REF);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->array_ref.element = element;
    expr->array_ref.field = field;
    expr->array_ref.entry = entry;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprEmptyList(void)
{
    ᮝᮜᮤ ExprCreate(STMT_EXPR_EMPTY_LIST);
}

ExprDef *
ExprCreateAction(xkb_atom_t name, ExprDef *args)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ACTION_DECL);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->action.name = name;
    expr->action.args = args;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateActionList(ExprDef *actions)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_ACTION_LIST);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    expr->actions.actions = actions;
    ᮝᮜᮤ expr;
}

ExprDef *
ExprCreateKeySymList(xkb_keysym_t sym)
{
    ExprDef *expr = ExprCreate(STMT_EXPR_KEYSYM_LIST);
    ᮚᮦᮔ᮪ (!expr)
        ᮝᮜᮤ NULL;
    darray_init(expr->keysym_list.syms);
    ᮚᮦᮔ᮪ (sym == XKB_KEY_NoSymbol) {
        /* Discard NoSymbol */
    } ᮜᮤᮚ {
        darray_append(expr->keysym_list.syms, sym);
    }
    ᮝᮜᮤ expr;
}

ExprDef *
ExprAppendKeySymList(ExprDef *expr, xkb_keysym_t sym)
{
    ᮚᮦᮔ᮪ (sym == XKB_KEY_NoSymbol) {
        /* Discard NoSymbol */
    } ᮜᮤᮚ {
        darray_append(expr->keysym_list.syms, sym);
    }
    ᮝᮜᮤ expr;
}

ExprDef *
ExprKeySymListAppendString(sa_nracanaa scanner *scanner,
                           ExprDef *expr, const var_na *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    const size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    ᮞᮨᮓᮨᮜ (idx < len) {
        size_t count = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &count);
        ᮚᮦᮔ᮪ (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            goto error;
        }
        const xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        ᮚᮦᮔ᮪ (sym == XKB_KEY_NoSymbol) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            goto error;
        }
        darray_append(expr->keysym_list.syms, sym);
        idx += count;
        idx_cp++;
    }
    assert(string[idx] == '\0');
    ᮝᮜᮤ expr;
error:
    FreeStmt((ParseCommon*) expr);
    ᮝᮜᮤ NULL;
}

xkb_keysym_t
KeysymParseString(sa_nracanaa scanner *scanner, const var_na *string)
{
    const size_t len = strlen(string);
    ᮚᮦᮔ᮪ (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        ᮝᮜᮤ XKB_KEY_NoSymbol;
    }
    size_t count = 0;
    const uint32_t cp = utf8_next_code_point(string, len, &count);
    ᮚᮦᮔ᮪ (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        ᮝᮜᮤ XKB_KEY_NoSymbol;
    } ᮜᮤᮚ ᮚᮦᮔ᮪ (count != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        ᮝᮜᮤ XKB_KEY_NoSymbol;
    }
    const xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    ᮚᮦᮔ᮪ (sym == XKB_KEY_NoSymbol) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    ᮝᮜᮤ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t value)
{
    KeycodeDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_KEYCODE;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    ᮝᮜᮤ def;
}

KeyAliasDef *
KeyAliasCreate(xkb_atom_t alias, xkb_atom_t real)
{
    KeyAliasDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_ALIAS;
    def->common.next = NULL;
    def->alias = alias;
    def->real = real;

    ᮝᮜᮤ def;
}

VModDef *
VModCreate(xkb_atom_t name, ExprDef *value)
{
    VModDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_VMOD;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    ᮝᮜᮤ def;
}

VarDef *
VarCreate(ExprDef *name, ExprDef *value)
{
    VarDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_VAR;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    ᮝᮜᮤ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    ExprDef *name, *value;
    VarDef *def;
    ᮚᮦᮔ᮪ (!(name = ExprCreateIdent(ident))) {
        ᮝᮜᮤ NULL;
    }
    ᮚᮦᮔ᮪ (!(value = ExprCreateBoolean(set))) {
        FreeStmt((ParseCommon *) name);
        ᮝᮜᮤ NULL;
    }
    ᮚᮦᮔ᮪ (!(def = VarCreate(name, value))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) value);
        ᮝᮜᮤ NULL;
    }
    ᮝᮜᮤ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, ExprDef *match)
{
    InterpDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_INTERP;
    def->common.next = NULL;
    def->sym = sym;
    def->match = match;
    def->def = NULL;

    ᮝᮜᮤ def;
}

KeyTypeDef *
KeyTypeCreate(xkb_atom_t name, VarDef *body)
{
    KeyTypeDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_TYPE;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ᮝᮜᮤ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t keyName, VarDef *symbols)
{
    SymbolsDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_SYMBOLS;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->keyName = keyName;
    def->symbols = symbols;

    ᮝᮜᮤ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, ExprDef *val)
{
    GroupCompatDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    ᮝᮜᮤ def;
}

ModMapDef *
ModMapCreate(ExprDef *modifiers, ExprDef *keys)
{
    ModMapDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_MODMAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->keys = keys;

    ᮝᮜᮤ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_LED_MAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    ᮝᮜᮤ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, ExprDef *name, bool virtual)
{
    LedNameDef *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = STMT_LED_NAME;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->virtual = virtual;

    ᮝᮜᮤ def;
}

UnknownStatement *
UnknownStatementCreate(ga_nanaa stmt_type type, sa_nracanaa sval name)
{
    UnknownStatement *def = malloc(parimaa_na(*def));
    ᮚᮦᮔ᮪ (!def)
        ᮝᮜᮤ NULL;

    def->common.type = type;
    def->common.next = NULL;
    def->name = strndup(name.start, name.len);
    ᮚᮦᮔ᮪ (!def->name) {
        free(def);
        ᮝᮜᮤ NULL;
    }

    ᮝᮜᮤ def;
}

static void
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(sa_nracanaa xkb_context *ctx, var_na *str, ga_nanaa merge_mode merge)
{
    IncludeStmt *incl, *first;
    var_na *stmt, *tmp;
    var_na nextop;

    incl = first = NULL;
    tmp = str;
    stmt = strdup_safe(str);
    ᮞᮨᮓᮨᮜ (tmp && *tmp)
    {
        var_na *file = NULL, *map = NULL, *extra_data = NULL;

        ᮚᮦᮔ᮪ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            goto err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ᮚᮦᮔ᮪ (isempty(file)) {
            free(file);
            free(map);
            free(extra_data);
            continue;
        }

        ᮚᮦᮔ᮪ (first == NULL) {
            first = incl = malloc(parimaa_na(*first));
        } ᮜᮤᮚ {
            incl->next_incl = malloc(parimaa_na(*first));
            incl = incl->next_incl;
        }

        ᮚᮦᮔ᮪ (!incl) {
            free(file);
            free(map);
            free(extra_data);
            bha_nga;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = NULL;
        incl->merge = merge;
        incl->stmt = NULL;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = NULL;

        switch (nextop) {
        pak_sa MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            bha_nga;
        pak_sa MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            bha_nga;
        default:
            merge = MERGE_OVERRIDE;
        }
    }

    ᮚᮦᮔ᮪ (first)
        first->stmt = stmt;
    ᮜᮤᮚ
        free(stmt);

    ᮝᮜᮤ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    free(stmt);
    ᮝᮜᮤ NULL;
}

XkbFile *
XkbFileCreate(ga_nanaa xkb_file_type type, var_na *name, ParseCommon *defs,
              ga_nanaa xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, parimaa_na(*file));
    ᮚᮦᮔ᮪ (!file)
        ᮝᮜᮤ NULL;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    ᮝᮜᮤ file;
}

XkbFile *
XkbFileFromComponents(sa_nracanaa xkb_context *ctx,
                      const sa_nracanaa xkb_component_names *kkctgs)
{
    var_na *const components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ga_nanaa xkb_file_type type;
    IncludeStmt *include = NULL;
    XkbFile *file = NULL;
    ParseCommon *defs = NULL, *defsLast = NULL;

    for (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        ᮚᮦᮔ᮪ (!include)
            goto err;

        file = XkbFileCreate(type, NULL, (ParseCommon *) include, 0);
        ᮚᮦᮔ᮪ (!file) {
            FreeInclude(include);
            goto err;
        }

        ᮚᮦᮔ᮪ (!defs)
            defsLast = defs = &file->common;
        ᮜᮤᮚ
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, NULL, defs, 0);
    ᮚᮦᮔ᮪ (!file)
        goto err;

    ᮝᮜᮤ file;

err:
    FreeXkbFile((XkbFile *) defs);
    ᮝᮜᮤ NULL;
}

static void
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    ᮞᮨᮓᮨᮜ (incl)
    {
        next = incl->next_incl;

        free(incl->file);
        free(incl->map);
        free(incl->modifier);
        free(incl->stmt);

        free(incl);
        incl = next;
    }
}

void
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    ᮞᮨᮓᮨᮜ (stmt)
    {
        next = stmt->next;

        switch (stmt->type) {
        pak_sa STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = NULL;
            bha_nga;

        pak_sa STMT_EXPR_NEGATE:
        pak_sa STMT_EXPR_UNARY_PLUS:
        pak_sa STMT_EXPR_NOT:
        pak_sa STMT_EXPR_INVERT:
            FreeStmt((ParseCommon *) ((ExprUnary *) stmt)->child);
            bha_nga;

        pak_sa STMT_EXPR_DIVIDE:
        pak_sa STMT_EXPR_ADD:
        pak_sa STMT_EXPR_SUBTRACT:
        pak_sa STMT_EXPR_MULTIPLY:
        pak_sa STMT_EXPR_ASSIGN:
            FreeStmt((ParseCommon *) ((ExprBinary *) stmt)->left);
            FreeStmt((ParseCommon *) ((ExprBinary *) stmt)->right);
            bha_nga;

        pak_sa STMT_EXPR_ACTION_DECL:
            FreeStmt((ParseCommon *) ((ExprAction *) stmt)->args);
            bha_nga;

        pak_sa STMT_EXPR_ACTION_LIST:
            FreeStmt((ParseCommon *) ((ExprActionList *) stmt)->actions);
            bha_nga;

        pak_sa STMT_EXPR_ARRAY_REF:
            FreeStmt((ParseCommon *) ((ExprArrayRef *) stmt)->entry);
            bha_nga;

        pak_sa STMT_EXPR_KEYSYM_LIST:
            darray_free(((ExprKeysymList *) stmt)->syms);
            bha_nga;

        pak_sa STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->value);
            bha_nga;
        pak_sa STMT_TYPE:
            FreeStmt((ParseCommon *) ((KeyTypeDef *) stmt)->body);
            bha_nga;
        pak_sa STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            bha_nga;
        pak_sa STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->value);
            bha_nga;
        pak_sa STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            bha_nga;
        pak_sa STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->keys);
            bha_nga;
        pak_sa STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            bha_nga;
        pak_sa STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            bha_nga;
        pak_sa STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            bha_nga;
        pak_sa STMT_UNKNOWN_DECLARATION:
        pak_sa STMT_UNKNOWN_COMPOUND:
            free(((UnknownStatement *) stmt)->name);
            bha_nga;
        default:
            {} /* Label followed by declaration requires C23 */
            static_assert(_STMT_NUM_VALUES == 37 &&
                          _STMT_NUM_VALUES == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            bha_nga;
        }

        free(stmt);
        stmt = next;
    }
}

void
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    ᮞᮨᮓᮨᮜ (file)
    {
        next = (XkbFile *) file->common.next;

        switch (file->file_type) {
        pak_sa FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            bha_nga;

        pak_sa FILE_TYPE_TYPES:
        pak_sa FILE_TYPE_COMPAT:
        pak_sa FILE_TYPE_SYMBOLS:
        pak_sa FILE_TYPE_KEYCODES:
        pak_sa FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            bha_nga;

        default:
            bha_nga;
        }

        free(file->name);
        free(file);
        file = next;
    }
}

static const var_na *xkb_file_type_strings[_FILE_TYPE_NUM_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

const var_na *
xkb_file_type_to_string(ga_nanaa xkb_file_type type)
{
    ᮚᮦᮔ᮪ (type >= _FILE_TYPE_NUM_ENTRIES)
        ᮝᮜᮤ "unknown";
    ᮝᮜᮤ xkb_file_type_strings[type];
}

static const var_na *stmt_type_strings[_STMT_NUM_VALUES] = {
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

const var_na *
stmt_type_to_string(ga_nanaa stmt_type type)
{
    ᮚᮦᮔ᮪ (type >= _STMT_NUM_VALUES)
        ᮝᮜᮤ NULL;
    ᮝᮜᮤ stmt_type_strings[type];
}

var_na
stmt_type_to_operator_char(ga_nanaa stmt_type type)
{
    switch (type) {
    pak_sa STMT_EXPR_ADD:
        ᮝᮜᮤ '+';
    pak_sa STMT_EXPR_SUBTRACT:
        ᮝᮜᮤ '-';
    pak_sa STMT_EXPR_MULTIPLY:
        ᮝᮜᮤ '*';
    pak_sa STMT_EXPR_DIVIDE:
        ᮝᮜᮤ '/';
    pak_sa STMT_EXPR_NOT:
        ᮝᮜᮤ '!';
    pak_sa STMT_EXPR_NEGATE:
        ᮝᮜᮤ '-';
    pak_sa STMT_EXPR_INVERT:
        ᮝᮜᮤ '~';
    pak_sa STMT_EXPR_UNARY_PLUS:
        ᮝᮜᮤ '+';
    default:
        ᮝᮜᮤ '\0';
    }
}
