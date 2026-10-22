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

ساكن اظهار_Def *
اظهار_Create(تعداد stmt_type op)
{
    اظهار_Def *اظهار = يادگيري_ڏيو(حجم(*اظهار));
    إذا (!اظهار)
        أرجع عدم;

    اظهار->common.type = op;
    اظهار->common.ايندڙ = عدم;

    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_String(xkb_atom_t str)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_STRING_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->string.str = str;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Integer(int64_t ival)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_INTEGER_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->integer.ival = ival;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Float(فراغ)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_FLOAT_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Boolean(bool سيٽ)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_BOOLEAN_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->boolean.سيٽ = سيٽ;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_ڪنجي_Name(xkb_atom_t ڪنجي_name)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_KEYNAME_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->ڪنجي_name.ڪنجي_name = ڪنجي_name;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_ڪنجي_Sym(xkb_keysym_t keysym)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_KEYSYM_LITERAL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->keysym.keysym = keysym;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Ident(xkb_atom_t ident)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_IDENT);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->ident.ident = ident;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Unary(تعداد stmt_type op, اظهار_Def *ٻار)
{
    اظهار_Def *اظهار = اظهار_Create(op);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->unary.ٻار = ٻار;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_ٻٽو(تعداد stmt_type op, اظهار_Def *کاٻو, اظهار_Def *ساڄو)
{
    اظهار_Def *اظهار = اظهار_Create(op);
    إذا (!اظهار)
        أرجع عدم;

    اظهار->ٻٽو.کاٻو = کاٻو;
    اظهار->ٻٽو.ساڄو = ساڄو;

    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_ميدان_Ref(xkb_atom_t element, xkb_atom_t ميدان_2)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_ميدان_REF);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->ميدان_ref.element = element;
    اظهار->ميدان_ref.ميدان_2 = ميدان_2;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ميدان_2, اظهار_Def *داخلا)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_ARRAY_REF);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->array_ref.element = element;
    اظهار->array_ref.ميدان_2 = ميدان_2;
    اظهار->array_ref.داخلا = داخلا;
    أرجع اظهار;
}

اظهار_Def *
اظهار_خالي_جايون_List(فراغ)
{
    أرجع اظهار_Create(STMT_اظهار_خالي_جايون_LIST);
}

اظهار_Def *
اظهار_Create_Action(xkb_atom_t name, اظهار_Def *دليلون)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_ACTION_DECL);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->action.name = name;
    اظهار->action.دليلون = دليلون;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_Action_List(اظهار_Def *actions)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_ACTION_LIST);
    إذا (!اظهار)
        أرجع عدم;
    اظهار->actions.actions = actions;
    أرجع اظهار;
}

اظهار_Def *
اظهار_Create_ڪنجي_Sym_List(xkb_keysym_t sym)
{
    اظهار_Def *اظهار = اظهار_Create(STMT_اظهار_KEYSYM_LIST);
    إذا (!اظهار)
        أرجع عدم;
    darray_init(اظهار->keysym_list.syms);
    إذا (sym == XKB_ڪنجي_No_نشان) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(اظهار->keysym_list.syms, sym);
    }
    أرجع اظهار;
}

اظهار_Def *
اظهار_Append_ڪنجي_Sym_List(اظهار_Def *اظهار, xkb_keysym_t sym)
{
    إذا (sym == XKB_ڪنجي_No_نشان) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(اظهار->keysym_list.syms, sym);
    }
    أرجع اظهار;
}

اظهار_Def *
اظهار_ڪنجي_Sym_List_Append_String(بنية scanner *scanner,
                           اظهار_Def *اظهار, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت سائيز_t len = لڙي_ڊيگهه(string);
    سائيز_t idx = 0;
    سائيز_t idx_cp = 1;
    طالما (idx < len) {
        سائيز_t ڳڻپ = 0;
        uint32_t cp = utf8_ايندڙ_ڪوڊ_نقطو(string + idx, len - idx, &ڳڻپ);
        إذا (cp == INVALID_UTF8_ڪوڊ_نقطو) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            اذهب error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_منزل_keysym(cp);
        إذا (sym == XKB_ڪنجي_No_نشان) {
            scanner_err(scanner, XKB_LOG_پيغام_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            اذهب error;
        }
        darray_append(اظهار->keysym_list.syms, sym);
        idx += ڳڻپ;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    أرجع اظهار;
error:
    FreeStmt((ParseCommon*) اظهار);
    أرجع عدم;
}

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string)
{
    ثابت سائيز_t len = لڙي_ڊيگهه(string);
    إذا (len == 0) {
        scanner_err(scanner, XKB_LOG_پيغام_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        أرجع XKB_ڪنجي_No_نشان;
    }
    سائيز_t ڳڻپ = 0;
    ثابت uint32_t cp = utf8_ايندڙ_ڪوڊ_نقطو(string, len, &ڳڻپ);
    إذا (cp == INVALID_UTF8_ڪوڊ_نقطو) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        أرجع XKB_ڪنجي_No_نشان;
    } وإلا إذا (ڳڻپ != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        أرجع XKB_ڪنجي_No_نشان;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_منزل_keysym(cp);
    إذا (sym == XKB_ڪنجي_No_نشان) {
        scanner_err(scanner, XKB_LOG_پيغام_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    أرجع sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t قدر)
{
    KeycodeDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_KEYCODE;
    def->common.ايندڙ = عدم;
    def->name = name;
    def->قدر = قدر;

    أرجع def;
}

ڪنجي_Alias_Def *
ڪنجي_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    ڪنجي_Alias_Def *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_ALIAS;
    def->common.ايندڙ = عدم;
    def->alias = alias;
    def->real = real;

    أرجع def;
}

VModDef *
VModCreate(xkb_atom_t name, اظهار_Def *قدر)
{
    VModDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VMOD;
    def->common.ايندڙ = عدم;
    def->name = name;
    def->قدر = قدر;

    أرجع def;
}

VarDef *
VarCreate(اظهار_Def *name, اظهار_Def *قدر)
{
    VarDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VAR;
    def->common.ايندڙ = عدم;
    def->name = name;
    def->قدر = قدر;

    أرجع def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سيٽ)
{
    اظهار_Def *name, *قدر;
    VarDef *def;
    إذا (!(name = اظهار_Create_Ident(ident))) {
        أرجع عدم;
    }
    إذا (!(قدر = اظهار_Create_Boolean(سيٽ))) {
        FreeStmt((ParseCommon *) name);
        أرجع عدم;
    }
    إذا (!(def = VarCreate(name, قدر))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) قدر);
        أرجع عدم;
    }
    أرجع def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, اظهار_Def *ميلاپ)
{
    InterpDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_INTERP;
    def->common.ايندڙ = عدم;
    def->sym = sym;
    def->ميلاپ = ميلاپ;
    def->def = عدم;

    أرجع def;
}

ڪنجي_Type_Def *
ڪنجي_Type_Create(xkb_atom_t name, VarDef *body)
{
    ڪنجي_Type_Def *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_TYPE;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t ڪنجي_Name, VarDef *symbols)
{
    SymbolsDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_SYMBOLS;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->ڪنجي_Name = ڪنجي_Name;
    def->symbols = symbols;

    أرجع def;
}

گروهه_Compat_Def *
گروهه_Compat_Create(int64_t گروهه, اظهار_Def *val)
{
    گروهه_Compat_Def *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_گروهه_COMPAT;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->گروهه = گروهه;
    def->def = val;

    أرجع def;
}

ModMapDef *
ModMapCreate(اظهار_Def *modifiers, اظهار_Def *ڪنجيون)
{
    ModMapDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_MODMAP;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->modifiers = modifiers;
    def->ڪنجيون = ڪنجيون;

    أرجع def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_MAP;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

LedNameDef *
LedNameCreate(int64_t ndx, اظهار_Def *name, bool مجازي)
{
    LedNameDef *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_NAME;
    def->common.ايندڙ = عدم;
    def->ملايو = ملايو_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->مجازي = مجازي;

    أرجع def;
}

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type type, بنية sval name)
{
    UnknownStatement *def = يادگيري_ڏيو(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = type;
    def->common.ايندڙ = عدم;
    def->name = strndup(name.شروعات, name.len);
    إذا (!def->name) {
        حرر(def);
        أرجع عدم;
    }

    أرجع def;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد ملايو_گهڻائي ملايو)
{
    IncludeStmt *incl, *پهريون;
    حرف *stmt, *tmp;
    حرف nextop;

    incl = پهريون = عدم;
    tmp = str;
    stmt = strdup_محفوظ(str);
    طالما (tmp && *tmp)
    {
        حرف *file = عدم, *map = عدم, *extra_معلومات = عدم;

        إذا (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_معلومات))
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
            حرر(extra_معلومات);
            استمر;
        }

        إذا (پهريون == عدم) {
            پهريون = incl = يادگيري_ڏيو(حجم(*پهريون));
        } وإلا {
            incl->ايندڙ_incl = يادگيري_ڏيو(حجم(*پهريون));
            incl = incl->ايندڙ_incl;
        }

        إذا (!incl) {
            حرر(file);
            حرر(map);
            حرر(extra_معلومات);
            اكسر;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.ايندڙ = عدم;
        incl->ملايو = ملايو;
        incl->stmt = عدم;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_معلومات;
        incl->ايندڙ_incl = عدم;

        بدل (nextop) {
        حالة ملايو_AUGMENT_اڳياڙي:
            ملايو = ملايو_AUGMENT;
            اكسر;
        حالة ملايو_REPLACE_اڳياڙي:
            ملايو = ملايو_REPLACE;
            اكسر;
        افتراضي:
            ملايو = ملايو_OVERRIDE;
        }
    }

    إذا (پهريون)
        پهريون->stmt = stmt;
    وإلا
        حرر(stmt);

    أرجع پهريون;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(پهريون);
    حرر(stmt);
    أرجع عدم;
}

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags)
{
    XkbFile *file;

    file = صفر_يادگيري_ڏيو(1, حجم(*file));
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
                      ثابت بنية xkb_component_نالا *kkctgs)
{
    حرف *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    تعداد xkb_file_type type;
    IncludeStmt *include = عدم;
    XkbFile *file = عدم;
    ParseCommon *defs = عدم, *defs_آخري = عدم;

    لكل (type = پهريون_KEYMAP_FILE_TYPE; type <= آخري_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], ملايو_DEFAULT);
        إذا (!include)
            اذهب err;

        file = XkbFileCreate(type, عدم, (ParseCommon *) include, 0);
        إذا (!file) {
            FreeInclude(include);
            اذهب err;
        }

        إذا (!defs)
            defs_آخري = defs = &file->common;
        وإلا
            defs_آخري = defs_آخري->ايندڙ = &file->common;
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
    IncludeStmt *ايندڙ;

    طالما (incl)
    {
        ايندڙ = incl->ايندڙ_incl;

        حرر(incl->file);
        حرر(incl->map);
        حرر(incl->modifier);
        حرر(incl->stmt);

        حرر(incl);
        incl = ايندڙ;
    }
}

فراغ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *ايندڙ;

    طالما (stmt)
    {
        ايندڙ = stmt->ايندڙ;

        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = عدم;
            اكسر;

        حالة STMT_اظهار_NEGATE:
        حالة STMT_اظهار_UNARY_PLUS:
        حالة STMT_اظهار_NOT:
        حالة STMT_اظهار_INVERT:
            FreeStmt((ParseCommon *) ((اظهار_Unary *) stmt)->ٻار);
            اكسر;

        حالة STMT_اظهار_DIVIDE:
        حالة STMT_اظهار_جوڙيو:
        حالة STMT_اظهار_ڪٽيو:
        حالة STMT_اظهار_MULTIPLY:
        حالة STMT_اظهار_سونپيو:
            FreeStmt((ParseCommon *) ((اظهار_ٻٽو *) stmt)->کاٻو);
            FreeStmt((ParseCommon *) ((اظهار_ٻٽو *) stmt)->ساڄو);
            اكسر;

        حالة STMT_اظهار_ACTION_DECL:
            FreeStmt((ParseCommon *) ((اظهار_Action *) stmt)->دليلون);
            اكسر;

        حالة STMT_اظهار_ACTION_LIST:
            FreeStmt((ParseCommon *) ((اظهار_Action_List *) stmt)->actions);
            اكسر;

        حالة STMT_اظهار_ARRAY_REF:
            FreeStmt((ParseCommon *) ((اظهار_Array_Ref *) stmt)->داخلا);
            اكسر;

        حالة STMT_اظهار_KEYSYM_LIST:
            darray_free(((اظهار_Keysym_List *) stmt)->syms);
            اكسر;

        حالة STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->قدر);
            اكسر;
        حالة STMT_TYPE:
            FreeStmt((ParseCommon *) ((ڪنجي_Type_Def *) stmt)->body);
            اكسر;
        حالة STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->ميلاپ);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            اكسر;
        حالة STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->قدر);
            اكسر;
        حالة STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            اكسر;
        حالة STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->ڪنجيون);
            اكسر;
        حالة STMT_گروهه_COMPAT:
            FreeStmt((ParseCommon *) ((گروهه_Compat_Def *) stmt)->def);
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
            static_assert(STMT_انگ_قدرون == 37 &&
                          STMT_انگ_قدرون == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            اكسر;
        }

        حرر(stmt);
        stmt = ايندڙ;
    }
}

فراغ
FreeXkbFile(XkbFile *file)
{
    XkbFile *ايندڙ;

    طالما (file)
    {
        ايندڙ = (XkbFile *) file->common.ايندڙ;

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
        file = ايندڙ;
    }
}

ساكن ثابت حرف *xkb_file_type_لڙيون[FILE_TYPE_انگ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_قاعدا] = "rules",
};

ثابت حرف *
xkb_file_type_منزل_string(تعداد xkb_file_type type)
{
    إذا (type >= FILE_TYPE_انگ_ENTRIES)
        أرجع "unknown";
    أرجع xkb_file_type_لڙيون[type];
}

ساكن ثابت حرف *stmt_type_لڙيون[STMT_انگ_قدرون] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_اظهار_STRING_LITERAL] = "string literal expression",
    [STMT_اظهار_INTEGER_LITERAL] = "integer literal expression",
    [STMT_اظهار_FLOAT_LITERAL] = "float literal expression",
    [STMT_اظهار_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_اظهار_KEYNAME_LITERAL] = "key name expression",
    [STMT_اظهار_KEYSYM_LITERAL] = "keysym expression",
    [STMT_اظهار_IDENT] = "identifier expression",
    [STMT_اظهار_ACTION_DECL] = "action declaration expression",
    [STMT_اظهار_ميدان_REF] = "field reference expression",
    [STMT_اظهار_ARRAY_REF] = "array reference expression",
    [STMT_اظهار_خالي_جايون_LIST] = "empty list expression",
    [STMT_اظهار_KEYSYM_LIST] = "keysym list expression",
    [STMT_اظهار_ACTION_LIST] = "action list expression",
    [STMT_اظهار_جوڙيو] = "addition expression",
    [STMT_اظهار_ڪٽيو] = "substraction expression",
    [STMT_اظهار_MULTIPLY] = "multiplication expression",
    [STMT_اظهار_DIVIDE] = "division expression",
    [STMT_اظهار_سونپيو] = "assignment expression",
    [STMT_اظهار_NOT] = "logical negation expression",
    [STMT_اظهار_NEGATE] = "arithmetic negation expression",
    [STMT_اظهار_INVERT] = "bitwise inversion expression",
    [STMT_اظهار_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_گروهه_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_انگ_قدرون == 37 &&
              STMT_انگ_قدرون == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_منزل_string(تعداد stmt_type type)
{
    إذا (type >= STMT_انگ_قدرون)
        أرجع عدم;
    أرجع stmt_type_لڙيون[type];
}

حرف
stmt_type_منزل_operator_char(تعداد stmt_type type)
{
    بدل (type) {
    حالة STMT_اظهار_جوڙيو:
        أرجع '+';
    حالة STMT_اظهار_ڪٽيو:
        أرجع '-';
    حالة STMT_اظهار_MULTIPLY:
        أرجع '*';
    حالة STMT_اظهار_DIVIDE:
        أرجع '/';
    حالة STMT_اظهار_NOT:
        أرجع '!';
    حالة STMT_اظهار_NEGATE:
        أرجع '-';
    حالة STMT_اظهار_INVERT:
        أرجع '~';
    حالة STMT_اظهار_UNARY_PLUS:
        أرجع '+';
    افتراضي:
        أرجع '\0';
    }
}
