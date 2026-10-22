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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon-keysyms.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "ast-build.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "utf8-decoding.h"

સ્થિર પદાવલિ_Def *
પદાવલિ_Create(ગણના stmt_type op)
{
    પદાવલિ_Def *પદાવલિ = malloc(કદ(*પદાવલિ));
    જો (!પદાવલિ)
        પાછા ખાલી;

    પદાવલિ->common.type = op;
    પદાવલિ->common.next = ખાલી;

    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_String(xkb_atom_t str)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_STRING_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->string.str = str;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Integer(int64_t ival)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_INTEGER_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->integer.ival = ival;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Float(શૂન્ય)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_FLOAT_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Boolean(bool set)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_BOOLEAN_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->boolean.set = set;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Name(xkb_atom_t ચાવી_name)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_KEYNAME_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->ચાવી_name.ચાવી_name = ચાવી_name;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Sym(xkb_keysym_t keysym)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_KEYSYM_LITERAL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->keysym.keysym = keysym;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Ident(xkb_atom_t ident)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_IDENT);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->ident.ident = ident;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Unary(ગણના stmt_type op, પદાવલિ_Def *child)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(op);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->unary.child = child;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_દ્વિઅંકી(ગણના stmt_type op, પદાવલિ_Def *ડાબું, પદાવલિ_Def *જમણું)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(op);
    જો (!પદાવલિ)
        પાછા ખાલી;

    પદાવલિ->દ્વિઅંકી.ડાબું = ડાબું;
    પદાવલિ->દ્વિઅંકી.જમણું = જમણું;

    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_FIELD_REF);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->field_ref.element = element;
    પદાવલિ->field_ref.field = field;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, પદાવલિ_Def *entry)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_ARRAY_REF);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->array_ref.element = element;
    પદાવલિ->array_ref.field = field;
    પદાવલિ->array_ref.entry = entry;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Empty_List(શૂન્ય)
{
    પાછા પદાવલિ_Create(STMT_પદાવલિ_EMPTY_LIST);
}

પદાવલિ_Def *
પદાવલિ_Create_Action(xkb_atom_t name, પદાવલિ_Def *args)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_ACTION_DECL);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->action.name = name;
    પદાવલિ->action.args = args;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_Action_List(પદાવલિ_Def *actions)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_ACTION_LIST);
    જો (!પદાવલિ)
        પાછા ખાલી;
    પદાવલિ->actions.actions = actions;
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Create_ચાવી_Sym_List(xkb_keysym_t sym)
{
    પદાવલિ_Def *પદાવલિ = પદાવલિ_Create(STMT_પદાવલિ_KEYSYM_LIST);
    જો (!પદાવલિ)
        પાછા ખાલી;
    darray_init(પદાવલિ->keysym_list.syms);
    જો (sym == XKB_ચાવી_No_પ્રતીક) {
        /* Discard NoSymbol */
    } નહીંતર {
        darray_append(પદાવલિ->keysym_list.syms, sym);
    }
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_Append_ચાવી_Sym_List(પદાવલિ_Def *પદાવલિ, xkb_keysym_t sym)
{
    જો (sym == XKB_ચાવી_No_પ્રતીક) {
        /* Discard NoSymbol */
    } નહીંતર {
        darray_append(પદાવલિ->keysym_list.syms, sym);
    }
    પાછા પદાવલિ;
}

પદાવલિ_Def *
પદાવલિ_ચાવી_Sym_List_Append_String(સંરચના scanner *scanner,
                           પદાવલિ_Def *પદાવલિ, અચલ અક્ષર *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    અચલ size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    જ્યારે (idx < len) {
        size_t ગણતરી = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &ગણતરી);
        જો (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            જાઓ error;
        }
        અચલ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        જો (sym == XKB_ચાવી_No_પ્રતીક) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            જાઓ error;
        }
        darray_append(પદાવલિ->keysym_list.syms, sym);
        idx += ગણતરી;
        idx_cp++;
    }
    પ્રતિજ્ઞા(string[idx] == '\0');
    પાછા પદાવલિ;
error:
    FreeStmt((ParseCommon*) પદાવલિ);
    પાછા ખાલી;
}

xkb_keysym_t
KeysymParseString(સંરચના scanner *scanner, અચલ અક્ષર *string)
{
    અચલ size_t len = strlen(string);
    જો (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        પાછા XKB_ચાવી_No_પ્રતીક;
    }
    size_t ગણતરી = 0;
    અચલ uint32_t cp = utf8_next_code_point(string, len, &ગણતરી);
    જો (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        પાછા XKB_ચાવી_No_પ્રતીક;
    } નહીંતર જો (ગણતરી != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        પાછા XKB_ચાવી_No_પ્રતીક;
    }
    અચલ xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    જો (sym == XKB_ચાવી_No_પ્રતીક) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    પાછા sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t કિંમત)
{
    KeycodeDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_KEYCODE;
    def->common.next = ખાલી;
    def->name = name;
    def->કિંમત = કિંમત;

    પાછા def;
}

ચાવી_Alias_Def *
ચાવી_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ચાવી_Alias_Def *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_ALIAS;
    def->common.next = ખાલી;
    def->alias = alias;
    def->real = real;

    પાછા def;
}

VModDef *
VModCreate(xkb_atom_t name, પદાવલિ_Def *કિંમત)
{
    VModDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_VMOD;
    def->common.next = ખાલી;
    def->name = name;
    def->કિંમત = કિંમત;

    પાછા def;
}

VarDef *
VarCreate(પદાવલિ_Def *name, પદાવલિ_Def *કિંમત)
{
    VarDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_VAR;
    def->common.next = ખાલી;
    def->name = name;
    def->કિંમત = કિંમત;

    પાછા def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    પદાવલિ_Def *name, *કિંમત;
    VarDef *def;
    જો (!(name = પદાવલિ_Create_Ident(ident))) {
        પાછા ખાલી;
    }
    જો (!(કિંમત = પદાવલિ_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        પાછા ખાલી;
    }
    જો (!(def = VarCreate(name, કિંમત))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) કિંમત);
        પાછા ખાલી;
    }
    પાછા def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, પદાવલિ_Def *match)
{
    InterpDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_INTERP;
    def->common.next = ખાલી;
    def->sym = sym;
    def->match = match;
    def->def = ખાલી;

    પાછા def;
}

ચાવી_Type_Def *
ચાવી_Type_Create(xkb_atom_t name, VarDef *body)
{
    ચાવી_Type_Def *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_TYPE;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    પાછા def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ચાવી_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_SYMBOLS;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->ચાવી_Name = ચાવી_Name;
    def->symbols = symbols;

    પાછા def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, પદાવલિ_Def *val)
{
    GroupCompatDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    પાછા def;
}

ModMapDef *
ModMapCreate(પદાવલિ_Def *modifiers, પદાવલિ_Def *ચાવીઓ)
{
    ModMapDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_MODMAP;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->ચાવીઓ = ચાવીઓ;

    પાછા def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_LED_MAP;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    પાછા def;
}

LedNameDef *
LedNameCreate(int64_t ndx, પદાવલિ_Def *name, bool આભાસી)
{
    LedNameDef *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = STMT_LED_NAME;
    def->common.next = ખાલી;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->આભાસી = આભાસી;

    પાછા def;
}

UnknownStatement *
UnknownStatementCreate(ગણના stmt_type type, સંરચના sval name)
{
    UnknownStatement *def = malloc(કદ(*def));
    જો (!def)
        પાછા ખાલી;

    def->common.type = type;
    def->common.next = ખાલી;
    def->name = strndup(name.start, name.len);
    જો (!def->name) {
        મુક્ત(def);
        પાછા ખાલી;
    }

    પાછા def;
}

સ્થિર શૂન્ય
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(સંરચના xkb_context *ctx, અક્ષર *str, ગણના merge_mode merge)
{
    IncludeStmt *incl, *first;
    અક્ષર *stmt, *tmp;
    અક્ષર nextop;

    incl = first = ખાલી;
    tmp = str;
    stmt = strdup_safe(str);
    જ્યારે (tmp && *tmp)
    {
        અક્ષર *file = ખાલી, *map = ખાલી, *extra_data = ખાલી;

        જો (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            જાઓ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        જો (isempty(file)) {
            મુક્ત(file);
            મુક્ત(map);
            મુક્ત(extra_data);
            ચાલુ;
        }

        જો (first == ખાલી) {
            first = incl = malloc(કદ(*first));
        } નહીંતર {
            incl->next_incl = malloc(કદ(*first));
            incl = incl->next_incl;
        }

        જો (!incl) {
            મુક્ત(file);
            મુક્ત(map);
            મુક્ત(extra_data);
            તોડો;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = ખાલી;
        incl->merge = merge;
        incl->stmt = ખાલી;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = ખાલી;

        પસંદગી (nextop) {
        સ્થિતિ MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            તોડો;
        સ્થિતિ MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            તોડો;
        મૂળભૂત:
            merge = MERGE_OVERRIDE;
        }
    }

    જો (first)
        first->stmt = stmt;
    નહીંતર
        મુક્ત(stmt);

    પાછા first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    મુક્ત(stmt);
    પાછા ખાલી;
}

XkbFile *
XkbFileCreate(ગણના xkb_file_type type, અક્ષર *name, ParseCommon *defs,
              ગણના xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, કદ(*file));
    જો (!file)
        પાછા ખાલી;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    પાછા file;
}

XkbFile *
XkbFileFromComponents(સંરચના xkb_context *ctx,
                      અચલ સંરચના xkb_component_names *kkctgs)
{
    અક્ષર *અચલ components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    ગણના xkb_file_type type;
    IncludeStmt *include = ખાલી;
    XkbFile *file = ખાલી;
    ParseCommon *defs = ખાલી, *defsLast = ખાલી;

    માટે (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        જો (!include)
            જાઓ err;

        file = XkbFileCreate(type, ખાલી, (ParseCommon *) include, 0);
        જો (!file) {
            FreeInclude(include);
            જાઓ err;
        }

        જો (!defs)
            defsLast = defs = &file->common;
        નહીંતર
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ખાલી, defs, 0);
    જો (!file)
        જાઓ err;

    પાછા file;

err:
    FreeXkbFile((XkbFile *) defs);
    પાછા ખાલી;
}

સ્થિર શૂન્ય
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    જ્યારે (incl)
    {
        next = incl->next_incl;

        મુક્ત(incl->file);
        મુક્ત(incl->map);
        મુક્ત(incl->modifier);
        મુક્ત(incl->stmt);

        મુક્ત(incl);
        incl = next;
    }
}

શૂન્ય
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    જ્યારે (stmt)
    {
        next = stmt->next;

        પસંદગી (stmt->type) {
        સ્થિતિ STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ખાલી;
            તોડો;

        સ્થિતિ STMT_પદાવલિ_NEGATE:
        સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
        સ્થિતિ STMT_પદાવલિ_NOT:
        સ્થિતિ STMT_પદાવલિ_INVERT:
            FreeStmt((ParseCommon *) ((પદાવલિ_Unary *) stmt)->child);
            તોડો;

        સ્થિતિ STMT_પદાવલિ_DIVIDE:
        સ્થિતિ STMT_પદાવલિ_ADD:
        સ્થિતિ STMT_પદાવલિ_SUBTRACT:
        સ્થિતિ STMT_પદાવલિ_MULTIPLY:
        સ્થિતિ STMT_પદાવલિ_ASSIGN:
            FreeStmt((ParseCommon *) ((પદાવલિ_દ્વિઅંકી *) stmt)->ડાબું);
            FreeStmt((ParseCommon *) ((પદાવલિ_દ્વિઅંકી *) stmt)->જમણું);
            તોડો;

        સ્થિતિ STMT_પદાવલિ_ACTION_DECL:
            FreeStmt((ParseCommon *) ((પદાવલિ_Action *) stmt)->args);
            તોડો;

        સ્થિતિ STMT_પદાવલિ_ACTION_LIST:
            FreeStmt((ParseCommon *) ((પદાવલિ_Action_List *) stmt)->actions);
            તોડો;

        સ્થિતિ STMT_પદાવલિ_ARRAY_REF:
            FreeStmt((ParseCommon *) ((પદાવલિ_Array_Ref *) stmt)->entry);
            તોડો;

        સ્થિતિ STMT_પદાવલિ_KEYSYM_LIST:
            darray_free(((પદાવલિ_Keysym_List *) stmt)->syms);
            તોડો;

        સ્થિતિ STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->કિંમત);
            તોડો;
        સ્થિતિ STMT_TYPE:
            FreeStmt((ParseCommon *) ((ચાવી_Type_Def *) stmt)->body);
            તોડો;
        સ્થિતિ STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            તોડો;
        સ્થિતિ STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->કિંમત);
            તોડો;
        સ્થિતિ STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            તોડો;
        સ્થિતિ STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ચાવીઓ);
            તોડો;
        સ્થિતિ STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            તોડો;
        સ્થિતિ STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            તોડો;
        સ્થિતિ STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            તોડો;
        સ્થિતિ STMT_UNKNOWN_DECLARATION:
        સ્થિતિ STMT_UNKNOWN_COMPOUND:
            મુક્ત(((UnknownStatement *) stmt)->name);
            તોડો;
        મૂળભૂત:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_અંક_કિંમતો == 37 &&
                          STMT_અંક_કિંમતો == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            તોડો;
        }

        મુક્ત(stmt);
        stmt = next;
    }
}

શૂન્ય
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    જ્યારે (file)
    {
        next = (XkbFile *) file->common.next;

        પસંદગી (file->file_type) {
        સ્થિતિ FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            તોડો;

        સ્થિતિ FILE_TYPE_TYPES:
        સ્થિતિ FILE_TYPE_COMPAT:
        સ્થિતિ FILE_TYPE_SYMBOLS:
        સ્થિતિ FILE_TYPE_KEYCODES:
        સ્થિતિ FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            તોડો;

        મૂળભૂત:
            તોડો;
        }

        મુક્ત(file->name);
        મુક્ત(file);
        file = next;
    }
}

સ્થિર અચલ અક્ષર *xkb_file_type_strings[FILE_TYPE_અંક_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

અચલ અક્ષર *
xkb_file_type_to_string(ગણના xkb_file_type type)
{
    જો (type >= FILE_TYPE_અંક_ENTRIES)
        પાછા "unknown";
    પાછા xkb_file_type_strings[type];
}

સ્થિર અચલ અક્ષર *stmt_type_strings[STMT_અંક_કિંમતો] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_પદાવલિ_STRING_LITERAL] = "string literal expression",
    [STMT_પદાવલિ_INTEGER_LITERAL] = "integer literal expression",
    [STMT_પદાવલિ_FLOAT_LITERAL] = "float literal expression",
    [STMT_પદાવલિ_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_પદાવલિ_KEYNAME_LITERAL] = "key name expression",
    [STMT_પદાવલિ_KEYSYM_LITERAL] = "keysym expression",
    [STMT_પદાવલિ_IDENT] = "identifier expression",
    [STMT_પદાવલિ_ACTION_DECL] = "action declaration expression",
    [STMT_પદાવલિ_FIELD_REF] = "field reference expression",
    [STMT_પદાવલિ_ARRAY_REF] = "array reference expression",
    [STMT_પદાવલિ_EMPTY_LIST] = "empty list expression",
    [STMT_પદાવલિ_KEYSYM_LIST] = "keysym list expression",
    [STMT_પદાવલિ_ACTION_LIST] = "action list expression",
    [STMT_પદાવલિ_ADD] = "addition expression",
    [STMT_પદાવલિ_SUBTRACT] = "substraction expression",
    [STMT_પદાવલિ_MULTIPLY] = "multiplication expression",
    [STMT_પદાવલિ_DIVIDE] = "division expression",
    [STMT_પદાવલિ_ASSIGN] = "assignment expression",
    [STMT_પદાવલિ_NOT] = "logical negation expression",
    [STMT_પદાવલિ_NEGATE] = "arithmetic negation expression",
    [STMT_પદાવલિ_INVERT] = "bitwise inversion expression",
    [STMT_પદાવલિ_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_અંક_કિંમતો == 37 &&
              STMT_અંક_કિંમતો == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

અચલ અક્ષર *
stmt_type_to_string(ગણના stmt_type type)
{
    જો (type >= STMT_અંક_કિંમતો)
        પાછા ખાલી;
    પાછા stmt_type_strings[type];
}

અક્ષર
stmt_type_to_operator_char(ગણના stmt_type type)
{
    પસંદગી (type) {
    સ્થિતિ STMT_પદાવલિ_ADD:
        પાછા '+';
    સ્થિતિ STMT_પદાવલિ_SUBTRACT:
        પાછા '-';
    સ્થિતિ STMT_પદાવલિ_MULTIPLY:
        પાછા '*';
    સ્થિતિ STMT_પદાવલિ_DIVIDE:
        પાછા '/';
    સ્થિતિ STMT_પદાવલિ_NOT:
        પાછા '!';
    સ્થિતિ STMT_પદાવલિ_NEGATE:
        પાછા '-';
    સ્થિતિ STMT_પદાવલિ_INVERT:
        પાછા '~';
    સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
        પાછા '+';
    મૂળભૂત:
        પાછા '\0';
    }
}
