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

ساكن تعبير_Def *
تعبير_Create(تعداد stmt_type op)
{
    تعبير_Def *تعبير = malloc(حجم(*تعبير));
    إذا (!تعبير)
        أرجع عدم;

    تعبير->common.type = op;
    تعبير->common.next = عدم;

    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_String(xkb_atom_t str)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_STRING_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->string.str = str;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Integer(int64_t ival)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_INTEGER_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->integer.ival = ival;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Float(فراغ)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_FLOAT_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Boolean(bool set)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_BOOLEAN_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->boolean.set = set;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_مفتاح_Name(xkb_atom_t مفتاح_name)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_KEYNAME_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->مفتاح_name.مفتاح_name = مفتاح_name;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_مفتاح_Sym(xkb_keysym_t keysym)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_KEYSYM_LITERAL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->keysym.keysym = keysym;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Ident(xkb_atom_t ident)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_IDENT);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->ident.ident = ident;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Unary(تعداد stmt_type op, تعبير_Def *child)
{
    تعبير_Def *تعبير = تعبير_Create(op);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->unary.child = child;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_ثنائي(تعداد stmt_type op, تعبير_Def *يسار, تعبير_Def *يمين)
{
    تعبير_Def *تعبير = تعبير_Create(op);
    إذا (!تعبير)
        أرجع عدم;

    تعبير->ثنائي.يسار = يسار;
    تعبير->ثنائي.يمين = يمين;

    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_FIELD_REF);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->field_ref.element = element;
    تعبير->field_ref.field = field;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, تعبير_Def *entry)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_ARRAY_REF);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->array_ref.element = element;
    تعبير->array_ref.field = field;
    تعبير->array_ref.entry = entry;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Empty_List(فراغ)
{
    أرجع تعبير_Create(STMT_تعبير_EMPTY_LIST);
}

تعبير_Def *
تعبير_Create_Action(xkb_atom_t name, تعبير_Def *وسائط)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_ACTION_DECL);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->action.name = name;
    تعبير->action.وسائط = وسائط;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_Action_List(تعبير_Def *actions)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_ACTION_LIST);
    إذا (!تعبير)
        أرجع عدم;
    تعبير->actions.actions = actions;
    أرجع تعبير;
}

تعبير_Def *
تعبير_Create_مفتاح_Sym_List(xkb_keysym_t sym)
{
    تعبير_Def *تعبير = تعبير_Create(STMT_تعبير_KEYSYM_LIST);
    إذا (!تعبير)
        أرجع عدم;
    darray_init(تعبير->keysym_list.syms);
    إذا (sym == XKB_مفتاح_No_رمز) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(تعبير->keysym_list.syms, sym);
    }
    أرجع تعبير;
}

تعبير_Def *
تعبير_Append_مفتاح_Sym_List(تعبير_Def *تعبير, xkb_keysym_t sym)
{
    إذا (sym == XKB_مفتاح_No_رمز) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(تعبير->keysym_list.syms, sym);
    }
    أرجع تعبير;
}

تعبير_Def *
تعبير_مفتاح_Sym_List_Append_String(بنية scanner *scanner,
                           تعبير_Def *تعبير, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت size_t len = strlen(string);
    size_t idx = 0;
    size_t idx_cp = 1;
    طالما (idx < len) {
        size_t عدد = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &عدد);
        إذا (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            اذهب error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        إذا (sym == XKB_مفتاح_No_رمز) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            اذهب error;
        }
        darray_append(تعبير->keysym_list.syms, sym);
        idx += عدد;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    أرجع تعبير;
error:
    FreeStmt((ParseCommon*) تعبير);
    أرجع عدم;
}

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string)
{
    ثابت size_t len = strlen(string);
    إذا (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        أرجع XKB_مفتاح_No_رمز;
    }
    size_t عدد = 0;
    ثابت uint32_t cp = utf8_next_code_point(string, len, &عدد);
    إذا (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        أرجع XKB_مفتاح_No_رمز;
    } وإلا إذا (عدد != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        أرجع XKB_مفتاح_No_رمز;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    إذا (sym == XKB_مفتاح_No_رمز) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    أرجع sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قيمة)
{
    KeycodeDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_KEYCODE;
    def->common.next = عدم;
    def->name = name;
    def->قيمة = قيمة;

    أرجع def;
}

مفتاح_Alias_Def *
مفتاح_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    مفتاح_Alias_Def *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_ALIAS;
    def->common.next = عدم;
    def->alias = alias;
    def->real = real;

    أرجع def;
}

VModDef *
VModCreate(xkb_atom_t name, تعبير_Def *قيمة)
{
    VModDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VMOD;
    def->common.next = عدم;
    def->name = name;
    def->قيمة = قيمة;

    أرجع def;
}

VarDef *
VarCreate(تعبير_Def *name, تعبير_Def *قيمة)
{
    VarDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VAR;
    def->common.next = عدم;
    def->name = name;
    def->قيمة = قيمة;

    أرجع def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    تعبير_Def *name, *قيمة;
    VarDef *def;
    إذا (!(name = تعبير_Create_Ident(ident))) {
        أرجع عدم;
    }
    إذا (!(قيمة = تعبير_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        أرجع عدم;
    }
    إذا (!(def = VarCreate(name, قيمة))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) قيمة);
        أرجع عدم;
    }
    أرجع def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, تعبير_Def *match)
{
    InterpDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_INTERP;
    def->common.next = عدم;
    def->sym = sym;
    def->match = match;
    def->def = عدم;

    أرجع def;
}

مفتاح_Type_Def *
مفتاح_Type_Create(xkb_atom_t name, VarDef *body)
{
    مفتاح_Type_Def *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_TYPE;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t مفتاح_Name, VarDef *symbols)
{
    SymbolsDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_SYMBOLS;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->مفتاح_Name = مفتاح_Name;
    def->symbols = symbols;

    أرجع def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, تعبير_Def *val)
{
    GroupCompatDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    أرجع def;
}

ModMapDef *
ModMapCreate(تعبير_Def *modifiers, تعبير_Def *مفاتيح)
{
    ModMapDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_MODMAP;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->مفاتيح = مفاتيح;

    أرجع def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_MAP;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

LedNameDef *
LedNameCreate(int64_t ndx, تعبير_Def *name, bool virtual)
{
    LedNameDef *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_NAME;
    def->common.next = عدم;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->virtual = virtual;

    أرجع def;
}

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type type, بنية sval name)
{
    UnknownStatement *def = malloc(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = type;
    def->common.next = عدم;
    def->name = strndup(name.start, name.len);
    إذا (!def->name) {
        حرر(def);
        أرجع عدم;
    }

    أرجع def;
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
    طالما (tmp && *tmp)
    {
        حرف *file = عدم, *map = عدم, *extra_data = عدم;

        إذا (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            اذهب err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        إذا (isempty(file)) {
            حرر(file);
            حرر(map);
            حرر(extra_data);
            استمر;
        }

        إذا (first == عدم) {
            first = incl = malloc(حجم(*first));
        } وإلا {
            incl->next_incl = malloc(حجم(*first));
            incl = incl->next_incl;
        }

        إذا (!incl) {
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

    إذا (first)
        first->stmt = stmt;
    وإلا
        حرر(stmt);

    أرجع first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    حرر(stmt);
    أرجع عدم;
}

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, حجم(*file));
    إذا (!file)
        أرجع عدم;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    أرجع file;
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
        إذا (!include)
            اذهب err;

        file = XkbFileCreate(type, عدم, (ParseCommon *) include, 0);
        إذا (!file) {
            FreeInclude(include);
            اذهب err;
        }

        إذا (!defs)
            defsLast = defs = &file->common;
        وإلا
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, عدم, defs, 0);
    إذا (!file)
        اذهب err;

    أرجع file;

err:
    FreeXkbFile((XkbFile *) defs);
    أرجع عدم;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    طالما (incl)
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

    طالما (stmt)
    {
        next = stmt->next;

        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = عدم;
            اكسر;

        حالة STMT_تعبير_NEGATE:
        حالة STMT_تعبير_UNARY_PLUS:
        حالة STMT_تعبير_NOT:
        حالة STMT_تعبير_INVERT:
            FreeStmt((ParseCommon *) ((تعبير_Unary *) stmt)->child);
            اكسر;

        حالة STMT_تعبير_DIVIDE:
        حالة STMT_تعبير_ADD:
        حالة STMT_تعبير_SUBTRACT:
        حالة STMT_تعبير_MULTIPLY:
        حالة STMT_تعبير_ASSIGN:
            FreeStmt((ParseCommon *) ((تعبير_ثنائي *) stmt)->يسار);
            FreeStmt((ParseCommon *) ((تعبير_ثنائي *) stmt)->يمين);
            اكسر;

        حالة STMT_تعبير_ACTION_DECL:
            FreeStmt((ParseCommon *) ((تعبير_Action *) stmt)->وسائط);
            اكسر;

        حالة STMT_تعبير_ACTION_LIST:
            FreeStmt((ParseCommon *) ((تعبير_Action_List *) stmt)->actions);
            اكسر;

        حالة STMT_تعبير_ARRAY_REF:
            FreeStmt((ParseCommon *) ((تعبير_Array_Ref *) stmt)->entry);
            اكسر;

        حالة STMT_تعبير_KEYSYM_LIST:
            darray_free(((تعبير_Keysym_List *) stmt)->syms);
            اكسر;

        حالة STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->قيمة);
            اكسر;
        حالة STMT_TYPE:
            FreeStmt((ParseCommon *) ((مفتاح_Type_Def *) stmt)->body);
            اكسر;
        حالة STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            اكسر;
        حالة STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->قيمة);
            اكسر;
        حالة STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            اكسر;
        حالة STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->مفاتيح);
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
            static_assert(STMT_رقم_قيم == 37 &&
                          STMT_رقم_قيم == STMT_UNKNOWN_COMPOUND + 1,
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

    طالما (file)
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

ساكن ثابت حرف *xkb_file_type_strings[FILE_TYPE_رقم_ENTRIES] = {
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
    إذا (type >= FILE_TYPE_رقم_ENTRIES)
        أرجع "unknown";
    أرجع xkb_file_type_strings[type];
}

ساكن ثابت حرف *stmt_type_strings[STMT_رقم_قيم] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_تعبير_STRING_LITERAL] = "string literal expression",
    [STMT_تعبير_INTEGER_LITERAL] = "integer literal expression",
    [STMT_تعبير_FLOAT_LITERAL] = "float literal expression",
    [STMT_تعبير_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_تعبير_KEYNAME_LITERAL] = "key name expression",
    [STMT_تعبير_KEYSYM_LITERAL] = "keysym expression",
    [STMT_تعبير_IDENT] = "identifier expression",
    [STMT_تعبير_ACTION_DECL] = "action declaration expression",
    [STMT_تعبير_FIELD_REF] = "field reference expression",
    [STMT_تعبير_ARRAY_REF] = "array reference expression",
    [STMT_تعبير_EMPTY_LIST] = "empty list expression",
    [STMT_تعبير_KEYSYM_LIST] = "keysym list expression",
    [STMT_تعبير_ACTION_LIST] = "action list expression",
    [STMT_تعبير_ADD] = "addition expression",
    [STMT_تعبير_SUBTRACT] = "substraction expression",
    [STMT_تعبير_MULTIPLY] = "multiplication expression",
    [STMT_تعبير_DIVIDE] = "division expression",
    [STMT_تعبير_ASSIGN] = "assignment expression",
    [STMT_تعبير_NOT] = "logical negation expression",
    [STMT_تعبير_NEGATE] = "arithmetic negation expression",
    [STMT_تعبير_INVERT] = "bitwise inversion expression",
    [STMT_تعبير_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_رقم_قيم == 37 &&
              STMT_رقم_قيم == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_to_string(تعداد stmt_type type)
{
    إذا (type >= STMT_رقم_قيم)
        أرجع عدم;
    أرجع stmt_type_strings[type];
}

حرف
stmt_type_to_operator_char(تعداد stmt_type type)
{
    بدل (type) {
    حالة STMT_تعبير_ADD:
        أرجع '+';
    حالة STMT_تعبير_SUBTRACT:
        أرجع '-';
    حالة STMT_تعبير_MULTIPLY:
        أرجع '*';
    حالة STMT_تعبير_DIVIDE:
        أرجع '/';
    حالة STMT_تعبير_NOT:
        أرجع '!';
    حالة STMT_تعبير_NEGATE:
        أرجع '-';
    حالة STMT_تعبير_INVERT:
        أرجع '~';
    حالة STMT_تعبير_UNARY_PLUS:
        أرجع '+';
    افتراضي:
        أرجع '\0';
    }
}
