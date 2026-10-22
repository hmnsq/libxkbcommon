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

ساکن اظہاریہ_Def *
اظہاریہ_Create(شمار stmt_type op)
{
    اظہاریہ_Def *اظہاریہ = حافظہ_دو(حجم(*اظہاریہ));
    اگر (!اظہاریہ)
        واپس تہی;

    اظہاریہ->common.type = op;
    اظہاریہ->common.اگلا = تہی;

    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_String(xkb_atom_t str)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_STRING_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->string.str = str;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Integer(int64_t ival)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_INTEGER_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->integer.ival = ival;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Float(خالی)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_FLOAT_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Boolean(bool سیٹ)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_BOOLEAN_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->boolean.سیٹ = سیٹ;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_کلید_Name(xkb_atom_t کلید_name)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_KEYNAME_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->کلید_name.کلید_name = کلید_name;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_کلید_Sym(xkb_keysym_t keysym)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_KEYSYM_LITERAL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->keysym.keysym = keysym;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Ident(xkb_atom_t ident)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_IDENT);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->ident.ident = ident;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Unary(شمار stmt_type op, اظہاریہ_Def *بچہ)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(op);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->unary.بچہ = بچہ;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_ثنائی(شمار stmt_type op, اظہاریہ_Def *بایاں, اظہاریہ_Def *دایاں)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(op);
    اگر (!اظہاریہ)
        واپس تہی;

    اظہاریہ->ثنائی.بایاں = بایاں;
    اظہاریہ->ثنائی.دایاں = دایاں;

    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_میدان_Ref(xkb_atom_t element, xkb_atom_t میدان_2)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_میدان_REF);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->میدان_ref.element = element;
    اظہاریہ->میدان_ref.میدان_2 = میدان_2;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Array_Ref(xkb_atom_t element, xkb_atom_t میدان_2, اظہاریہ_Def *اندراج_2)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_ARRAY_REF);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->array_ref.element = element;
    اظہاریہ->array_ref.میدان_2 = میدان_2;
    اظہاریہ->array_ref.اندراج_2 = اندراج_2;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_خالی_خانے_List(خالی)
{
    واپس اظہاریہ_Create(STMT_اظہاریہ_خالی_خانے_LIST);
}

اظہاریہ_Def *
اظہاریہ_Create_Action(xkb_atom_t name, اظہاریہ_Def *دلائل)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_ACTION_DECL);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->action.name = name;
    اظہاریہ->action.دلائل = دلائل;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_Action_List(اظہاریہ_Def *actions)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_ACTION_LIST);
    اگر (!اظہاریہ)
        واپس تہی;
    اظہاریہ->actions.actions = actions;
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Create_کلید_Sym_List(xkb_keysym_t sym)
{
    اظہاریہ_Def *اظہاریہ = اظہاریہ_Create(STMT_اظہاریہ_KEYSYM_LIST);
    اگر (!اظہاریہ)
        واپس تہی;
    darray_init(اظہاریہ->keysym_list.syms);
    اگر (sym == XKB_کلید_No_علامت) {
        /* Discard NoSymbol */
    } ورنہ {
        darray_append(اظہاریہ->keysym_list.syms, sym);
    }
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_Append_کلید_Sym_List(اظہاریہ_Def *اظہاریہ, xkb_keysym_t sym)
{
    اگر (sym == XKB_کلید_No_علامت) {
        /* Discard NoSymbol */
    } ورنہ {
        darray_append(اظہاریہ->keysym_list.syms, sym);
    }
    واپس اظہاریہ;
}

اظہاریہ_Def *
اظہاریہ_کلید_Sym_List_Append_String(ساخت scanner *scanner,
                           اظہاریہ_Def *اظہاریہ, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت سائز_t len = لڑی_لمبائی(string);
    سائز_t idx = 0;
    سائز_t idx_cp = 1;
    جب (idx < len) {
        سائز_t گنتی = 0;
        uint32_t cp = utf8_اگلا_رمز_نقطہ(string + idx, len - idx, &گنتی);
        اگر (cp == INVALID_UTF8_رمز_نقطہ) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            جاؤ error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_مقصد_keysym(cp);
        اگر (sym == XKB_کلید_No_علامت) {
            scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            جاؤ error;
        }
        darray_append(اظہاریہ->keysym_list.syms, sym);
        idx += گنتی;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    واپس اظہاریہ;
error:
    FreeStmt((ParseCommon*) اظہاریہ);
    واپس تہی;
}

xkb_keysym_t
KeysymParseString(ساخت scanner *scanner, ثابت حرف *string)
{
    ثابت سائز_t len = لڑی_لمبائی(string);
    اگر (len == 0) {
        scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        واپس XKB_کلید_No_علامت;
    }
    سائز_t گنتی = 0;
    ثابت uint32_t cp = utf8_اگلا_رمز_نقطہ(string, len, &گنتی);
    اگر (cp == INVALID_UTF8_رمز_نقطہ) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        واپس XKB_کلید_No_علامت;
    } ورنہ اگر (گنتی != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        واپس XKB_کلید_No_علامت;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_مقصد_keysym(cp);
    اگر (sym == XKB_کلید_No_علامت) {
        scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    واپس sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قدر)
{
    KeycodeDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_KEYCODE;
    def->common.اگلا = تہی;
    def->name = name;
    def->قدر = قدر;

    واپس def;
}

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    کلید_Alias_Def *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_ALIAS;
    def->common.اگلا = تہی;
    def->alias = alias;
    def->real = real;

    واپس def;
}

VModDef *
VModCreate(xkb_atom_t name, اظہاریہ_Def *قدر)
{
    VModDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_VMOD;
    def->common.اگلا = تہی;
    def->name = name;
    def->قدر = قدر;

    واپس def;
}

VarDef *
VarCreate(اظہاریہ_Def *name, اظہاریہ_Def *قدر)
{
    VarDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_VAR;
    def->common.اگلا = تہی;
    def->name = name;
    def->قدر = قدر;

    واپس def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سیٹ)
{
    اظہاریہ_Def *name, *قدر;
    VarDef *def;
    اگر (!(name = اظہاریہ_Create_Ident(ident))) {
        واپس تہی;
    }
    اگر (!(قدر = اظہاریہ_Create_Boolean(سیٹ))) {
        FreeStmt((ParseCommon *) name);
        واپس تہی;
    }
    اگر (!(def = VarCreate(name, قدر))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) قدر);
        واپس تہی;
    }
    واپس def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, اظہاریہ_Def *مطابقت)
{
    InterpDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_INTERP;
    def->common.اگلا = تہی;
    def->sym = sym;
    def->مطابقت = مطابقت;
    def->def = تہی;

    واپس def;
}

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body)
{
    کلید_Type_Def *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_TYPE;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->name = name;
    def->body = body;

    واپس def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols)
{
    SymbolsDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_SYMBOLS;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->کلید_Name = کلید_Name;
    def->symbols = symbols;

    واپس def;
}

گروہ_Compat_Def *
گروہ_Compat_Create(int64_t گروہ, اظہاریہ_Def *val)
{
    گروہ_Compat_Def *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_گروہ_COMPAT;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->گروہ = گروہ;
    def->def = val;

    واپس def;
}

ModMapDef *
ModMapCreate(اظہاریہ_Def *modifiers, اظہاریہ_Def *کلیدیں)
{
    ModMapDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_MODMAP;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->modifiers = modifiers;
    def->کلیدیں = کلیدیں;

    واپس def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_LED_MAP;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->name = name;
    def->body = body;

    واپس def;
}

LedNameDef *
LedNameCreate(int64_t ndx, اظہاریہ_Def *name, bool مجازی)
{
    LedNameDef *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = STMT_LED_NAME;
    def->common.اگلا = تہی;
    def->ملاؤ = ملاؤ_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->مجازی = مجازی;

    واپس def;
}

UnknownStatement *
UnknownStatementCreate(شمار stmt_type type, ساخت sval name)
{
    UnknownStatement *def = حافظہ_دو(حجم(*def));
    اگر (!def)
        واپس تہی;

    def->common.type = type;
    def->common.اگلا = تہی;
    def->name = strndup(name.آغاز, name.len);
    اگر (!def->name) {
        آزاد(def);
        واپس تہی;
    }

    واپس def;
}

ساکن خالی
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(ساخت xkb_context *ctx, حرف *str, شمار ملاؤ_منوال ملاؤ)
{
    IncludeStmt *incl, *پہلا;
    حرف *stmt, *tmp;
    حرف nextop;

    incl = پہلا = تہی;
    tmp = str;
    stmt = strdup_محفوظ(str);
    جب (tmp && *tmp)
    {
        حرف *file = تہی, *map = تہی, *extra_ڈیٹا = تہی;

        اگر (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_ڈیٹا))
            جاؤ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        اگر (isempty(file)) {
            آزاد(file);
            آزاد(map);
            آزاد(extra_ڈیٹا);
            جاری;
        }

        اگر (پہلا == تہی) {
            پہلا = incl = حافظہ_دو(حجم(*پہلا));
        } ورنہ {
            incl->اگلا_incl = حافظہ_دو(حجم(*پہلا));
            incl = incl->اگلا_incl;
        }

        اگر (!incl) {
            آزاد(file);
            آزاد(map);
            آزاد(extra_ڈیٹا);
            توڑو;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.اگلا = تہی;
        incl->ملاؤ = ملاؤ;
        incl->stmt = تہی;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_ڈیٹا;
        incl->اگلا_incl = تہی;

        انتخاب (nextop) {
        حالت ملاؤ_AUGMENT_سابقہ:
            ملاؤ = ملاؤ_AUGMENT;
            توڑو;
        حالت ملاؤ_REPLACE_سابقہ:
            ملاؤ = ملاؤ_REPLACE;
            توڑو;
        طےشدہ:
            ملاؤ = ملاؤ_OVERRIDE;
        }
    }

    اگر (پہلا)
        پہلا->stmt = stmt;
    ورنہ
        آزاد(stmt);

    واپس پہلا;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(پہلا);
    آزاد(stmt);
    واپس تہی;
}

XkbFile *
XkbFileCreate(شمار xkb_file_type type, حرف *name, ParseCommon *defs,
              شمار xkb_map_flags flags)
{
    XkbFile *file;

    file = صفر_حافظہ_دو(1, حجم(*file));
    اگر (!file)
        واپس تہی;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    واپس file;
}

XkbFile *
XkbFileFromComponents(ساخت xkb_context *ctx,
                      ثابت ساخت xkb_component_نام *kkctgs)
{
    حرف *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    شمار xkb_file_type type;
    IncludeStmt *include = تہی;
    XkbFile *file = تہی;
    ParseCommon *defs = تہی, *defs_آخری = تہی;

    برائے (type = پہلا_KEYMAP_FILE_TYPE; type <= آخری_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], ملاؤ_DEFAULT);
        اگر (!include)
            جاؤ err;

        file = XkbFileCreate(type, تہی, (ParseCommon *) include, 0);
        اگر (!file) {
            FreeInclude(include);
            جاؤ err;
        }

        اگر (!defs)
            defs_آخری = defs = &file->common;
        ورنہ
            defs_آخری = defs_آخری->اگلا = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, تہی, defs, 0);
    اگر (!file)
        جاؤ err;

    واپس file;

err:
    FreeXkbFile((XkbFile *) defs);
    واپس تہی;
}

ساکن خالی
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *اگلا;

    جب (incl)
    {
        اگلا = incl->اگلا_incl;

        آزاد(incl->file);
        آزاد(incl->map);
        آزاد(incl->modifier);
        آزاد(incl->stmt);

        آزاد(incl);
        incl = اگلا;
    }
}

خالی
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *اگلا;

    جب (stmt)
    {
        اگلا = stmt->اگلا;

        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = تہی;
            توڑو;

        حالت STMT_اظہاریہ_NEGATE:
        حالت STMT_اظہاریہ_UNARY_PLUS:
        حالت STMT_اظہاریہ_NOT:
        حالت STMT_اظہاریہ_INVERT:
            FreeStmt((ParseCommon *) ((اظہاریہ_Unary *) stmt)->بچہ);
            توڑو;

        حالت STMT_اظہاریہ_DIVIDE:
        حالت STMT_اظہاریہ_جوڑو:
        حالت STMT_اظہاریہ_تفریق:
        حالت STMT_اظہاریہ_MULTIPLY:
        حالت STMT_اظہاریہ_تفویض:
            FreeStmt((ParseCommon *) ((اظہاریہ_ثنائی *) stmt)->بایاں);
            FreeStmt((ParseCommon *) ((اظہاریہ_ثنائی *) stmt)->دایاں);
            توڑو;

        حالت STMT_اظہاریہ_ACTION_DECL:
            FreeStmt((ParseCommon *) ((اظہاریہ_Action *) stmt)->دلائل);
            توڑو;

        حالت STMT_اظہاریہ_ACTION_LIST:
            FreeStmt((ParseCommon *) ((اظہاریہ_Action_List *) stmt)->actions);
            توڑو;

        حالت STMT_اظہاریہ_ARRAY_REF:
            FreeStmt((ParseCommon *) ((اظہاریہ_Array_Ref *) stmt)->اندراج_2);
            توڑو;

        حالت STMT_اظہاریہ_KEYSYM_LIST:
            darray_free(((اظہاریہ_Keysym_List *) stmt)->syms);
            توڑو;

        حالت STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->قدر);
            توڑو;
        حالت STMT_TYPE:
            FreeStmt((ParseCommon *) ((کلید_Type_Def *) stmt)->body);
            توڑو;
        حالت STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->مطابقت);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            توڑو;
        حالت STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->قدر);
            توڑو;
        حالت STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            توڑو;
        حالت STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->کلیدیں);
            توڑو;
        حالت STMT_گروہ_COMPAT:
            FreeStmt((ParseCommon *) ((گروہ_Compat_Def *) stmt)->def);
            توڑو;
        حالت STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            توڑو;
        حالت STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            توڑو;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            آزاد(((UnknownStatement *) stmt)->name);
            توڑو;
        طےشدہ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_عدد_اقدار == 37 &&
                          STMT_عدد_اقدار == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            توڑو;
        }

        آزاد(stmt);
        stmt = اگلا;
    }
}

خالی
FreeXkbFile(XkbFile *file)
{
    XkbFile *اگلا;

    جب (file)
    {
        اگلا = (XkbFile *) file->common.اگلا;

        انتخاب (file->file_type) {
        حالت FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            توڑو;

        حالت FILE_TYPE_TYPES:
        حالت FILE_TYPE_COMPAT:
        حالت FILE_TYPE_SYMBOLS:
        حالت FILE_TYPE_KEYCODES:
        حالت FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            توڑو;

        طےشدہ:
            توڑو;
        }

        آزاد(file->name);
        آزاد(file);
        file = اگلا;
    }
}

ساکن ثابت حرف *xkb_file_type_لڑیاں[FILE_TYPE_عدد_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_قواعد] = "rules",
};

ثابت حرف *
xkb_file_type_مقصد_string(شمار xkb_file_type type)
{
    اگر (type >= FILE_TYPE_عدد_ENTRIES)
        واپس "unknown";
    واپس xkb_file_type_لڑیاں[type];
}

ساکن ثابت حرف *stmt_type_لڑیاں[STMT_عدد_اقدار] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_اظہاریہ_STRING_LITERAL] = "string literal expression",
    [STMT_اظہاریہ_INTEGER_LITERAL] = "integer literal expression",
    [STMT_اظہاریہ_FLOAT_LITERAL] = "float literal expression",
    [STMT_اظہاریہ_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_اظہاریہ_KEYNAME_LITERAL] = "key name expression",
    [STMT_اظہاریہ_KEYSYM_LITERAL] = "keysym expression",
    [STMT_اظہاریہ_IDENT] = "identifier expression",
    [STMT_اظہاریہ_ACTION_DECL] = "action declaration expression",
    [STMT_اظہاریہ_میدان_REF] = "field reference expression",
    [STMT_اظہاریہ_ARRAY_REF] = "array reference expression",
    [STMT_اظہاریہ_خالی_خانے_LIST] = "empty list expression",
    [STMT_اظہاریہ_KEYSYM_LIST] = "keysym list expression",
    [STMT_اظہاریہ_ACTION_LIST] = "action list expression",
    [STMT_اظہاریہ_جوڑو] = "addition expression",
    [STMT_اظہاریہ_تفریق] = "substraction expression",
    [STMT_اظہاریہ_MULTIPLY] = "multiplication expression",
    [STMT_اظہاریہ_DIVIDE] = "division expression",
    [STMT_اظہاریہ_تفویض] = "assignment expression",
    [STMT_اظہاریہ_NOT] = "logical negation expression",
    [STMT_اظہاریہ_NEGATE] = "arithmetic negation expression",
    [STMT_اظہاریہ_INVERT] = "bitwise inversion expression",
    [STMT_اظہاریہ_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_گروہ_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_عدد_اقدار == 37 &&
              STMT_عدد_اقدار == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_مقصد_string(شمار stmt_type type)
{
    اگر (type >= STMT_عدد_اقدار)
        واپس تہی;
    واپس stmt_type_لڑیاں[type];
}

حرف
stmt_type_مقصد_operator_char(شمار stmt_type type)
{
    انتخاب (type) {
    حالت STMT_اظہاریہ_جوڑو:
        واپس '+';
    حالت STMT_اظہاریہ_تفریق:
        واپس '-';
    حالت STMT_اظہاریہ_MULTIPLY:
        واپس '*';
    حالت STMT_اظہاریہ_DIVIDE:
        واپس '/';
    حالت STMT_اظہاریہ_NOT:
        واپس '!';
    حالت STMT_اظہاریہ_NEGATE:
        واپس '-';
    حالت STMT_اظہاریہ_INVERT:
        واپس '~';
    حالت STMT_اظہاریہ_UNARY_PLUS:
        واپس '+';
    طےشدہ:
        واپس '\0';
    }
}
