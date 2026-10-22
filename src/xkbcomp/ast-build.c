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

static पाहायाग_Def *
पाहायाग_Create(enum stmt_type op)
{
    पाहायाग_Def *पाहायाग = malloc(sizeof(*पाहायाग));
    if (!पाहायाग)
        return NULL;

    पाहायाग->common.type = op;
    पाहायाग->common.next = NULL;

    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_String(xkb_atom_t str)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_STRING_LITERAL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->string.str = str;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Integer(int64_t ival)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_INTEGER_LITERAL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->integer.ival = ival;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Float(void)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_FLOAT_LITERAL);
    if (!पाहायाग)
        return NULL;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Boolean(bool set)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_BOOLEAN_LITERAL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->boolean.set = set;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_सुसि_Name(xkb_atom_t सुसि_name)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_KEYNAME_LITERAL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->सुसि_name.सुसि_name = सुसि_name;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_सुसि_Sym(xkb_keysym_t keysym)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_KEYSYM_LITERAL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->keysym.keysym = keysym;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Ident(xkb_atom_t ident)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_IDENT);
    if (!पाहायाग)
        return NULL;
    पाहायाग->ident.ident = ident;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Unary(enum stmt_type op, पाहायाग_Def *child)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(op);
    if (!पाहायाग)
        return NULL;
    पाहायाग->unary.child = child;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_दालावाहान(enum stmt_type op, पाहायाग_Def *कालिवा, पाहायाग_Def *कानान)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(op);
    if (!पाहायाग)
        return NULL;

    पाहायाग->दालावाहान.कालिवा = कालिवा;
    पाहायाग->दालावाहान.कानान = कानान;

    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_FIELD_REF);
    if (!पाहायाग)
        return NULL;
    पाहायाग->field_ref.element = element;
    पाहायाग->field_ref.field = field;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, पाहायाग_Def *entry)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_ARRAY_REF);
    if (!पाहायाग)
        return NULL;
    पाहायाग->array_ref.element = element;
    पाहायाग->array_ref.field = field;
    पाहायाग->array_ref.entry = entry;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Empty_List(void)
{
    return पाहायाग_Create(STMT_पाहायाग_EMPTY_LIST);
}

पाहायाग_Def *
पाहायाग_Create_Action(xkb_atom_t name, पाहायाग_Def *मगा_कातविरान)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_ACTION_DECL);
    if (!पाहायाग)
        return NULL;
    पाहायाग->action.name = name;
    पाहायाग->action.मगा_कातविरान = मगा_कातविरान;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_Action_List(पाहायाग_Def *actions)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_ACTION_LIST);
    if (!पाहायाग)
        return NULL;
    पाहायाग->actions.actions = actions;
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Create_सुसि_Sym_List(xkb_keysym_t sym)
{
    पाहायाग_Def *पाहायाग = पाहायाग_Create(STMT_पाहायाग_KEYSYM_LIST);
    if (!पाहायाग)
        return NULL;
    darray_init(पाहायाग->keysym_list.syms);
    if (sym == XKB_सुसि_No_सिम्बोलो) {
        /* Discard NoSymbol */
    } else {
        darray_append(पाहायाग->keysym_list.syms, sym);
    }
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_Append_सुसि_Sym_List(पाहायाग_Def *पाहायाग, xkb_keysym_t sym)
{
    if (sym == XKB_सुसि_No_सिम्बोलो) {
        /* Discard NoSymbol */
    } else {
        darray_append(पाहायाग->keysym_list.syms, sym);
    }
    return पाहायाग;
}

पाहायाग_Def *
पाहायाग_सुसि_Sym_List_Append_String(struct scanner *scanner,
                           पाहायाग_Def *पाहायाग, const char *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    const size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    while (idx < len) {
        size_t बिलांग = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &बिलांग);
        if (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            goto error;
        }
        const xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        if (sym == XKB_सुसि_No_सिम्बोलो) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            goto error;
        }
        darray_append(पाहायाग->keysym_list.syms, sym);
        idx += बिलांग;
        idx_cp++;
    }
    assert(string[idx] == '\0');
    return पाहायाग;
error:
    FreeStmt((ParseCommon*) पाहायाग);
    return NULL;
}

xkb_keysym_t
KeysymParseString(struct scanner *scanner, const char *string)
{
    const size_t len = strlen(string);
    if (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        return XKB_सुसि_No_सिम्बोलो;
    }
    size_t बिलांग = 0;
    const uint32_t cp = utf8_next_code_point(string, len, &बिलांग);
    if (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        return XKB_सुसि_No_सिम्बोलो;
    } else if (बिलांग != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        return XKB_सुसि_No_सिम्बोलो;
    }
    const xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    if (sym == XKB_सुसि_No_सिम्बोलो) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    return sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t हालागा)
{
    KeycodeDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_KEYCODE;
    def->common.next = NULL;
    def->name = name;
    def->हालागा = हालागा;

    return def;
}

सुसि_Alias_Def *
सुसि_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    सुसि_Alias_Def *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_ALIAS;
    def->common.next = NULL;
    def->alias = alias;
    def->real = real;

    return def;
}

VModDef *
VModCreate(xkb_atom_t name, पाहायाग_Def *हालागा)
{
    VModDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_VMOD;
    def->common.next = NULL;
    def->name = name;
    def->हालागा = हालागा;

    return def;
}

VarDef *
VarCreate(पाहायाग_Def *name, पाहायाग_Def *हालागा)
{
    VarDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_VAR;
    def->common.next = NULL;
    def->name = name;
    def->हालागा = हालागा;

    return def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    पाहायाग_Def *name, *हालागा;
    VarDef *def;
    if (!(name = पाहायाग_Create_Ident(ident))) {
        return NULL;
    }
    if (!(हालागा = पाहायाग_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        return NULL;
    }
    if (!(def = VarCreate(name, हालागा))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) हालागा);
        return NULL;
    }
    return def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, पाहायाग_Def *match)
{
    InterpDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_INTERP;
    def->common.next = NULL;
    def->sym = sym;
    def->match = match;
    def->def = NULL;

    return def;
}

सुसि_Type_Def *
सुसि_Type_Create(xkb_atom_t name, VarDef *body)
{
    सुसि_Type_Def *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_TYPE;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    return def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t सुसि_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_SYMBOLS;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->सुसि_Name = सुसि_Name;
    def->symbols = symbols;

    return def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, पाहायाग_Def *val)
{
    GroupCompatDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    return def;
}

ModMapDef *
ModMapCreate(पाहायाग_Def *modifiers, पाहायाग_Def *मगा_सुसि)
{
    ModMapDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_MODMAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->मगा_सुसि = मगा_सुसि;

    return def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_LED_MAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    return def;
}

LedNameDef *
LedNameCreate(int64_t ndx, पाहायाग_Def *name, bool virtual)
{
    LedNameDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_LED_NAME;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->virtual = virtual;

    return def;
}

UnknownStatement *
UnknownStatementCreate(enum stmt_type type, struct sval name)
{
    UnknownStatement *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = type;
    def->common.next = NULL;
    def->name = strndup(name.start, name.len);
    if (!def->name) {
        free(def);
        return NULL;
    }

    return def;
}

static void
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(struct xkb_context *ctx, char *str, enum merge_mode merge)
{
    IncludeStmt *incl, *first;
    char *stmt, *tmp;
    char nextop;

    incl = first = NULL;
    tmp = str;
    stmt = strdup_safe(str);
    while (tmp && *tmp)
    {
        char *file = NULL, *map = NULL, *extra_data = NULL;

        if (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            goto err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        if (isempty(file)) {
            free(file);
            free(map);
            free(extra_data);
            continue;
        }

        if (first == NULL) {
            first = incl = malloc(sizeof(*first));
        } else {
            incl->next_incl = malloc(sizeof(*first));
            incl = incl->next_incl;
        }

        if (!incl) {
            free(file);
            free(map);
            free(extra_data);
            break;
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
        case MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            break;
        case MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            break;
        default:
            merge = MERGE_OVERRIDE;
        }
    }

    if (first)
        first->stmt = stmt;
    else
        free(stmt);

    return first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    free(stmt);
    return NULL;
}

XkbFile *
XkbFileCreate(enum xkb_file_type type, char *name, ParseCommon *defs,
              enum xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, sizeof(*file));
    if (!file)
        return NULL;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    return file;
}

XkbFile *
XkbFileFromComponents(struct xkb_context *ctx,
                      const struct xkb_component_names *kkctgs)
{
    char *const components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    enum xkb_file_type type;
    IncludeStmt *include = NULL;
    XkbFile *file = NULL;
    ParseCommon *defs = NULL, *defsLast = NULL;

    for (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        if (!include)
            goto err;

        file = XkbFileCreate(type, NULL, (ParseCommon *) include, 0);
        if (!file) {
            FreeInclude(include);
            goto err;
        }

        if (!defs)
            defsLast = defs = &file->common;
        else
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, NULL, defs, 0);
    if (!file)
        goto err;

    return file;

err:
    FreeXkbFile((XkbFile *) defs);
    return NULL;
}

static void
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    while (incl)
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

    while (stmt)
    {
        next = stmt->next;

        switch (stmt->type) {
        case STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = NULL;
            break;

        case STMT_पाहायाग_NEGATE:
        case STMT_पाहायाग_UNARY_PLUS:
        case STMT_पाहायाग_NOT:
        case STMT_पाहायाग_INVERT:
            FreeStmt((ParseCommon *) ((पाहायाग_Unary *) stmt)->child);
            break;

        case STMT_पाहायाग_DIVIDE:
        case STMT_पाहायाग_ADD:
        case STMT_पाहायाग_SUBTRACT:
        case STMT_पाहायाग_MULTIPLY:
        case STMT_पाहायाग_ASSIGN:
            FreeStmt((ParseCommon *) ((पाहायाग_दालावाहान *) stmt)->कालिवा);
            FreeStmt((ParseCommon *) ((पाहायाग_दालावाहान *) stmt)->कानान);
            break;

        case STMT_पाहायाग_ACTION_DECL:
            FreeStmt((ParseCommon *) ((पाहायाग_Action *) stmt)->मगा_कातविरान);
            break;

        case STMT_पाहायाग_ACTION_LIST:
            FreeStmt((ParseCommon *) ((पाहायाग_Action_List *) stmt)->actions);
            break;

        case STMT_पाहायाग_ARRAY_REF:
            FreeStmt((ParseCommon *) ((पाहायाग_Array_Ref *) stmt)->entry);
            break;

        case STMT_पाहायाग_KEYSYM_LIST:
            darray_free(((पाहायाग_Keysym_List *) stmt)->syms);
            break;

        case STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->हालागा);
            break;
        case STMT_TYPE:
            FreeStmt((ParseCommon *) ((सुसि_Type_Def *) stmt)->body);
            break;
        case STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            break;
        case STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->हालागा);
            break;
        case STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            break;
        case STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->मगा_सुसि);
            break;
        case STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            break;
        case STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            break;
        case STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            break;
        case STMT_UNKNOWN_DECLARATION:
        case STMT_UNKNOWN_COMPOUND:
            free(((UnknownStatement *) stmt)->name);
            break;
        default:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_नुमेरो_मगा_हालागा == 37 &&
                          STMT_नुमेरो_मगा_हालागा == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            break;
        }

        free(stmt);
        stmt = next;
    }
}

void
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    while (file)
    {
        next = (XkbFile *) file->common.next;

        switch (file->file_type) {
        case FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            break;

        case FILE_TYPE_TYPES:
        case FILE_TYPE_COMPAT:
        case FILE_TYPE_SYMBOLS:
        case FILE_TYPE_KEYCODES:
        case FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            break;

        default:
            break;
        }

        free(file->name);
        free(file);
        file = next;
    }
}

static const char *xkb_file_type_strings[FILE_TYPE_नुमेरो_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

const char *
xkb_file_type_to_string(enum xkb_file_type type)
{
    if (type >= FILE_TYPE_नुमेरो_ENTRIES)
        return "unknown";
    return xkb_file_type_strings[type];
}

static const char *stmt_type_strings[STMT_नुमेरो_मगा_हालागा] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_पाहायाग_STRING_LITERAL] = "string literal expression",
    [STMT_पाहायाग_INTEGER_LITERAL] = "integer literal expression",
    [STMT_पाहायाग_FLOAT_LITERAL] = "float literal expression",
    [STMT_पाहायाग_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_पाहायाग_KEYNAME_LITERAL] = "key name expression",
    [STMT_पाहायाग_KEYSYM_LITERAL] = "keysym expression",
    [STMT_पाहायाग_IDENT] = "identifier expression",
    [STMT_पाहायाग_ACTION_DECL] = "action declaration expression",
    [STMT_पाहायाग_FIELD_REF] = "field reference expression",
    [STMT_पाहायाग_ARRAY_REF] = "array reference expression",
    [STMT_पाहायाग_EMPTY_LIST] = "empty list expression",
    [STMT_पाहायाग_KEYSYM_LIST] = "keysym list expression",
    [STMT_पाहायाग_ACTION_LIST] = "action list expression",
    [STMT_पाहायाग_ADD] = "addition expression",
    [STMT_पाहायाग_SUBTRACT] = "substraction expression",
    [STMT_पाहायाग_MULTIPLY] = "multiplication expression",
    [STMT_पाहायाग_DIVIDE] = "division expression",
    [STMT_पाहायाग_ASSIGN] = "assignment expression",
    [STMT_पाहायाग_NOT] = "logical negation expression",
    [STMT_पाहायाग_NEGATE] = "arithmetic negation expression",
    [STMT_पाहायाग_INVERT] = "bitwise inversion expression",
    [STMT_पाहायाग_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_नुमेरो_मगा_हालागा == 37 &&
              STMT_नुमेरो_मगा_हालागा == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

const char *
stmt_type_to_string(enum stmt_type type)
{
    if (type >= STMT_नुमेरो_मगा_हालागा)
        return NULL;
    return stmt_type_strings[type];
}

char
stmt_type_to_operator_char(enum stmt_type type)
{
    switch (type) {
    case STMT_पाहायाग_ADD:
        return '+';
    case STMT_पाहायाग_SUBTRACT:
        return '-';
    case STMT_पाहायाग_MULTIPLY:
        return '*';
    case STMT_पाहायाग_DIVIDE:
        return '/';
    case STMT_पाहायाग_NOT:
        return '!';
    case STMT_पाहायाग_NEGATE:
        return '-';
    case STMT_पाहायाग_INVERT:
        return '~';
    case STMT_पाहायाग_UNARY_PLUS:
        return '+';
    default:
        return '\0';
    }
}
