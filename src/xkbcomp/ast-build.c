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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "ast-build.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "utf8-decoding.h"

நிலையான கோவை_Def *
கோவை_Create(எண்ணிக்கை stmt_type op)
{
    கோவை_Def *கோவை = malloc(அளவு(*கோவை));
    எனில் (!கோவை)
        திரும்பு வெற்று;

    கோவை->common.type = op;
    கோவை->common.next = வெற்று;

    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_String(xkb_atom_t str)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_STRING_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->string.str = str;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Integer(int64_t ival)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_INTEGER_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->integer.ival = ival;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Float(வெறுமை)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_FLOAT_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Boolean(bool set)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_BOOLEAN_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->boolean.set = set;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_சாவி_Name(xkb_atom_t சாவி_name)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_KEYNAME_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->சாவி_name.சாவி_name = சாவி_name;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_சாவி_Sym(xkb_keysym_t keysym)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_KEYSYM_LITERAL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->keysym.keysym = keysym;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Ident(xkb_atom_t ident)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_IDENT);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->ident.ident = ident;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Unary(எண்ணிக்கை stmt_type op, கோவை_Def *child)
{
    கோவை_Def *கோவை = கோவை_Create(op);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->unary.child = child;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_இருமம்(எண்ணிக்கை stmt_type op, கோவை_Def *இடது, கோவை_Def *வலது)
{
    கோவை_Def *கோவை = கோவை_Create(op);
    எனில் (!கோவை)
        திரும்பு வெற்று;

    கோவை->இருமம்.இடது = இடது;
    கோவை->இருமம்.வலது = வலது;

    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_FIELD_REF);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->field_ref.element = element;
    கோவை->field_ref.field = field;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, கோவை_Def *entry)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_ARRAY_REF);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->array_ref.element = element;
    கோவை->array_ref.field = field;
    கோவை->array_ref.entry = entry;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Empty_List(வெறுமை)
{
    திரும்பு கோவை_Create(STMT_கோவை_EMPTY_LIST);
}

கோவை_Def *
கோவை_Create_Action(xkb_atom_t name, கோவை_Def *args)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_ACTION_DECL);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->action.name = name;
    கோவை->action.args = args;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_Action_List(கோவை_Def *actions)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_ACTION_LIST);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    கோவை->actions.actions = actions;
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Create_சாவி_Sym_List(xkb_keysym_t sym)
{
    கோவை_Def *கோவை = கோவை_Create(STMT_கோவை_KEYSYM_LIST);
    எனில் (!கோவை)
        திரும்பு வெற்று;
    darray_init(கோவை->keysym_list.syms);
    எனில் (sym == XKB_சாவி_No_குறியீடு) {
        /* Discard NoSymbol */
    } இல்லையெனில் {
        darray_append(கோவை->keysym_list.syms, sym);
    }
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_Append_சாவி_Sym_List(கோவை_Def *கோவை, xkb_keysym_t sym)
{
    எனில் (sym == XKB_சாவி_No_குறியீடு) {
        /* Discard NoSymbol */
    } இல்லையெனில் {
        darray_append(கோவை->keysym_list.syms, sym);
    }
    திரும்பு கோவை;
}

கோவை_Def *
கோவை_சாவி_Sym_List_Append_String(கட்டமைப்பு scanner *scanner,
                           கோவை_Def *கோவை, மாறிலி எழுத்து *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    மாறிலி size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    போது (idx < len) {
        size_t மொத்தம் = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &மொத்தம்);
        எனில் (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            செல் error;
        }
        மாறிலி xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        எனில் (sym == XKB_சாவி_No_குறியீடு) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            செல் error;
        }
        darray_append(கோவை->keysym_list.syms, sym);
        idx += மொத்தம்;
        idx_cp++;
    }
    ப்ரதிஜ்ஞா(string[idx] == '\0');
    திரும்பு கோவை;
error:
    FreeStmt((ParseCommon*) கோவை);
    திரும்பு வெற்று;
}

xkb_keysym_t
KeysymParseString(கட்டமைப்பு scanner *scanner, மாறிலி எழுத்து *string)
{
    மாறிலி size_t len = strlen(string);
    எனில் (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        திரும்பு XKB_சாவி_No_குறியீடு;
    }
    size_t மொத்தம் = 0;
    மாறிலி uint32_t cp = utf8_next_code_point(string, len, &மொத்தம்);
    எனில் (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        திரும்பு XKB_சாவி_No_குறியீடு;
    } இல்லையெனில் எனில் (மொத்தம் != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        திரும்பு XKB_சாவி_No_குறியீடு;
    }
    மாறிலி xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    எனில் (sym == XKB_சாவி_No_குறியீடு) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    திரும்பு sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t மதிப்பு)
{
    KeycodeDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_KEYCODE;
    def->common.next = வெற்று;
    def->name = name;
    def->மதிப்பு = மதிப்பு;

    திரும்பு def;
}

சாவி_Alias_Def *
சாவி_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    சாவி_Alias_Def *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_ALIAS;
    def->common.next = வெற்று;
    def->alias = alias;
    def->real = real;

    திரும்பு def;
}

VModDef *
VModCreate(xkb_atom_t name, கோவை_Def *மதிப்பு)
{
    VModDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_VMOD;
    def->common.next = வெற்று;
    def->name = name;
    def->மதிப்பு = மதிப்பு;

    திரும்பு def;
}

VarDef *
VarCreate(கோவை_Def *name, கோவை_Def *மதிப்பு)
{
    VarDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_VAR;
    def->common.next = வெற்று;
    def->name = name;
    def->மதிப்பு = மதிப்பு;

    திரும்பு def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    கோவை_Def *name, *மதிப்பு;
    VarDef *def;
    எனில் (!(name = கோவை_Create_Ident(ident))) {
        திரும்பு வெற்று;
    }
    எனில் (!(மதிப்பு = கோவை_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        திரும்பு வெற்று;
    }
    எனில் (!(def = VarCreate(name, மதிப்பு))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) மதிப்பு);
        திரும்பு வெற்று;
    }
    திரும்பு def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, கோவை_Def *match)
{
    InterpDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_INTERP;
    def->common.next = வெற்று;
    def->sym = sym;
    def->match = match;
    def->def = வெற்று;

    திரும்பு def;
}

சாவி_Type_Def *
சாவி_Type_Create(xkb_atom_t name, VarDef *body)
{
    சாவி_Type_Def *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_TYPE;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    திரும்பு def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t சாவி_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_SYMBOLS;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->சாவி_Name = சாவி_Name;
    def->symbols = symbols;

    திரும்பு def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, கோவை_Def *val)
{
    GroupCompatDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    திரும்பு def;
}

ModMapDef *
ModMapCreate(கோவை_Def *modifiers, கோவை_Def *சாவிகள்)
{
    ModMapDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_MODMAP;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->சாவிகள் = சாவிகள்;

    திரும்பு def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_LED_MAP;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    திரும்பு def;
}

LedNameDef *
LedNameCreate(int64_t ndx, கோவை_Def *name, bool மெய்நிகர்)
{
    LedNameDef *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = STMT_LED_NAME;
    def->common.next = வெற்று;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->மெய்நிகர் = மெய்நிகர்;

    திரும்பு def;
}

UnknownStatement *
UnknownStatementCreate(எண்ணிக்கை stmt_type type, கட்டமைப்பு sval name)
{
    UnknownStatement *def = malloc(அளவு(*def));
    எனில் (!def)
        திரும்பு வெற்று;

    def->common.type = type;
    def->common.next = வெற்று;
    def->name = strndup(name.start, name.len);
    எனில் (!def->name) {
        விடுவி(def);
        திரும்பு வெற்று;
    }

    திரும்பு def;
}

நிலையான வெறுமை
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(கட்டமைப்பு xkb_context *ctx, எழுத்து *str, எண்ணிக்கை merge_mode merge)
{
    IncludeStmt *incl, *first;
    எழுத்து *stmt, *tmp;
    எழுத்து nextop;

    incl = first = வெற்று;
    tmp = str;
    stmt = strdup_safe(str);
    போது (tmp && *tmp)
    {
        எழுத்து *file = வெற்று, *map = வெற்று, *extra_data = வெற்று;

        எனில் (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            செல் err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        எனில் (isempty(file)) {
            விடுவி(file);
            விடுவி(map);
            விடுவி(extra_data);
            தொடர்;
        }

        எனில் (first == வெற்று) {
            first = incl = malloc(அளவு(*first));
        } இல்லையெனில் {
            incl->next_incl = malloc(அளவு(*first));
            incl = incl->next_incl;
        }

        எனில் (!incl) {
            விடுவி(file);
            விடுவி(map);
            விடுவி(extra_data);
            நிறுத்து;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = வெற்று;
        incl->merge = merge;
        incl->stmt = வெற்று;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = வெற்று;

        தேர்வு (nextop) {
        நிலை MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            நிறுத்து;
        நிலை MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            நிறுத்து;
        இயல்பு:
            merge = MERGE_OVERRIDE;
        }
    }

    எனில் (first)
        first->stmt = stmt;
    இல்லையெனில்
        விடுவி(stmt);

    திரும்பு first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    விடுவி(stmt);
    திரும்பு வெற்று;
}

XkbFile *
XkbFileCreate(எண்ணிக்கை xkb_file_type type, எழுத்து *name, ParseCommon *defs,
              எண்ணிக்கை xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, அளவு(*file));
    எனில் (!file)
        திரும்பு வெற்று;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    திரும்பு file;
}

XkbFile *
XkbFileFromComponents(கட்டமைப்பு xkb_context *ctx,
                      மாறிலி கட்டமைப்பு xkb_component_names *kkctgs)
{
    எழுத்து *மாறிலி components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    எண்ணிக்கை xkb_file_type type;
    IncludeStmt *include = வெற்று;
    XkbFile *file = வெற்று;
    ParseCommon *defs = வெற்று, *defsLast = வெற்று;

    க்கு (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        எனில் (!include)
            செல் err;

        file = XkbFileCreate(type, வெற்று, (ParseCommon *) include, 0);
        எனில் (!file) {
            FreeInclude(include);
            செல் err;
        }

        எனில் (!defs)
            defsLast = defs = &file->common;
        இல்லையெனில்
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, வெற்று, defs, 0);
    எனில் (!file)
        செல் err;

    திரும்பு file;

err:
    FreeXkbFile((XkbFile *) defs);
    திரும்பு வெற்று;
}

நிலையான வெறுமை
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    போது (incl)
    {
        next = incl->next_incl;

        விடுவி(incl->file);
        விடுவி(incl->map);
        விடுவி(incl->modifier);
        விடுவி(incl->stmt);

        விடுவி(incl);
        incl = next;
    }
}

வெறுமை
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    போது (stmt)
    {
        next = stmt->next;

        தேர்வு (stmt->type) {
        நிலை STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = வெற்று;
            நிறுத்து;

        நிலை STMT_கோவை_NEGATE:
        நிலை STMT_கோவை_UNARY_PLUS:
        நிலை STMT_கோவை_NOT:
        நிலை STMT_கோவை_INVERT:
            FreeStmt((ParseCommon *) ((கோவை_Unary *) stmt)->child);
            நிறுத்து;

        நிலை STMT_கோவை_DIVIDE:
        நிலை STMT_கோவை_ADD:
        நிலை STMT_கோவை_SUBTRACT:
        நிலை STMT_கோவை_MULTIPLY:
        நிலை STMT_கோவை_ASSIGN:
            FreeStmt((ParseCommon *) ((கோவை_இருமம் *) stmt)->இடது);
            FreeStmt((ParseCommon *) ((கோவை_இருமம் *) stmt)->வலது);
            நிறுத்து;

        நிலை STMT_கோவை_ACTION_DECL:
            FreeStmt((ParseCommon *) ((கோவை_Action *) stmt)->args);
            நிறுத்து;

        நிலை STMT_கோவை_ACTION_LIST:
            FreeStmt((ParseCommon *) ((கோவை_Action_List *) stmt)->actions);
            நிறுத்து;

        நிலை STMT_கோவை_ARRAY_REF:
            FreeStmt((ParseCommon *) ((கோவை_Array_Ref *) stmt)->entry);
            நிறுத்து;

        நிலை STMT_கோவை_KEYSYM_LIST:
            darray_free(((கோவை_Keysym_List *) stmt)->syms);
            நிறுத்து;

        நிலை STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->மதிப்பு);
            நிறுத்து;
        நிலை STMT_TYPE:
            FreeStmt((ParseCommon *) ((சாவி_Type_Def *) stmt)->body);
            நிறுத்து;
        நிலை STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            நிறுத்து;
        நிலை STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->மதிப்பு);
            நிறுத்து;
        நிலை STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            நிறுத்து;
        நிலை STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->சாவிகள்);
            நிறுத்து;
        நிலை STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            நிறுத்து;
        நிலை STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            நிறுத்து;
        நிலை STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            நிறுத்து;
        நிலை STMT_UNKNOWN_DECLARATION:
        நிலை STMT_UNKNOWN_COMPOUND:
            விடுவி(((UnknownStatement *) stmt)->name);
            நிறுத்து;
        இயல்பு:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_இலக்கம்_மதிப்புகள் == 37 &&
                          STMT_இலக்கம்_மதிப்புகள் == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            நிறுத்து;
        }

        விடுவி(stmt);
        stmt = next;
    }
}

வெறுமை
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    போது (file)
    {
        next = (XkbFile *) file->common.next;

        தேர்வு (file->file_type) {
        நிலை FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            நிறுத்து;

        நிலை FILE_TYPE_TYPES:
        நிலை FILE_TYPE_COMPAT:
        நிலை FILE_TYPE_SYMBOLS:
        நிலை FILE_TYPE_KEYCODES:
        நிலை FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            நிறுத்து;

        இயல்பு:
            நிறுத்து;
        }

        விடுவி(file->name);
        விடுவி(file);
        file = next;
    }
}

நிலையான மாறிலி எழுத்து *xkb_file_type_strings[FILE_TYPE_இலக்கம்_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

மாறிலி எழுத்து *
xkb_file_type_to_string(எண்ணிக்கை xkb_file_type type)
{
    எனில் (type >= FILE_TYPE_இலக்கம்_ENTRIES)
        திரும்பு "unknown";
    திரும்பு xkb_file_type_strings[type];
}

நிலையான மாறிலி எழுத்து *stmt_type_strings[STMT_இலக்கம்_மதிப்புகள்] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_கோவை_STRING_LITERAL] = "string literal expression",
    [STMT_கோவை_INTEGER_LITERAL] = "integer literal expression",
    [STMT_கோவை_FLOAT_LITERAL] = "float literal expression",
    [STMT_கோவை_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_கோவை_KEYNAME_LITERAL] = "key name expression",
    [STMT_கோவை_KEYSYM_LITERAL] = "keysym expression",
    [STMT_கோவை_IDENT] = "identifier expression",
    [STMT_கோவை_ACTION_DECL] = "action declaration expression",
    [STMT_கோவை_FIELD_REF] = "field reference expression",
    [STMT_கோவை_ARRAY_REF] = "array reference expression",
    [STMT_கோவை_EMPTY_LIST] = "empty list expression",
    [STMT_கோவை_KEYSYM_LIST] = "keysym list expression",
    [STMT_கோவை_ACTION_LIST] = "action list expression",
    [STMT_கோவை_ADD] = "addition expression",
    [STMT_கோவை_SUBTRACT] = "substraction expression",
    [STMT_கோவை_MULTIPLY] = "multiplication expression",
    [STMT_கோவை_DIVIDE] = "division expression",
    [STMT_கோவை_ASSIGN] = "assignment expression",
    [STMT_கோவை_NOT] = "logical negation expression",
    [STMT_கோவை_NEGATE] = "arithmetic negation expression",
    [STMT_கோவை_INVERT] = "bitwise inversion expression",
    [STMT_கோவை_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_இலக்கம்_மதிப்புகள் == 37 &&
              STMT_இலக்கம்_மதிப்புகள் == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

மாறிலி எழுத்து *
stmt_type_to_string(எண்ணிக்கை stmt_type type)
{
    எனில் (type >= STMT_இலக்கம்_மதிப்புகள்)
        திரும்பு வெற்று;
    திரும்பு stmt_type_strings[type];
}

எழுத்து
stmt_type_to_operator_char(எண்ணிக்கை stmt_type type)
{
    தேர்வு (type) {
    நிலை STMT_கோவை_ADD:
        திரும்பு '+';
    நிலை STMT_கோவை_SUBTRACT:
        திரும்பு '-';
    நிலை STMT_கோவை_MULTIPLY:
        திரும்பு '*';
    நிலை STMT_கோவை_DIVIDE:
        திரும்பு '/';
    நிலை STMT_கோவை_NOT:
        திரும்பு '!';
    நிலை STMT_கோவை_NEGATE:
        திரும்பு '-';
    நிலை STMT_கோவை_INVERT:
        திரும்பு '~';
    நிலை STMT_கோவை_UNARY_PLUS:
        திரும்பு '+';
    இயல்பு:
        திரும்பு '\0';
    }
}
