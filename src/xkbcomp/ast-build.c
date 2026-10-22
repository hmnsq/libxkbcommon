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

ساكن بیان_Def *
بیان_Create(تعداد stmt_type op)
{
    بیان_Def *بیان = حافظه_ورکړه(حجم(*بیان));
    إذا (!بیان)
        أرجع عدم;

    بیان->common.type = op;
    بیان->common.بل = عدم;

    أرجع بیان;
}

بیان_Def *
بیان_Create_String(xkb_atom_t str)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_STRING_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    بیان->string.str = str;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Integer(int64_t ival)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_INTEGER_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    بیان->integer.ival = ival;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Float(فراغ)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_FLOAT_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Boolean(bool سېټ)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_BOOLEAN_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    بیان->boolean.سېټ = سېټ;
    أرجع بیان;
}

بیان_Def *
بیان_Create_کلی_Name(xkb_atom_t کلی_name)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_KEYNAME_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    بیان->کلی_name.کلی_name = کلی_name;
    أرجع بیان;
}

بیان_Def *
بیان_Create_کلی_Sym(xkb_keysym_t keysym)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_KEYSYM_LITERAL);
    إذا (!بیان)
        أرجع عدم;
    بیان->keysym.keysym = keysym;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Ident(xkb_atom_t ident)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_IDENT);
    إذا (!بیان)
        أرجع عدم;
    بیان->ident.ident = ident;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Unary(تعداد stmt_type op, بیان_Def *ماشوم)
{
    بیان_Def *بیان = بیان_Create(op);
    إذا (!بیان)
        أرجع عدم;
    بیان->unary.ماشوم = ماشوم;
    أرجع بیان;
}

بیان_Def *
بیان_Create_دوګونی(تعداد stmt_type op, بیان_Def *کیڼ, بیان_Def *ښی)
{
    بیان_Def *بیان = بیان_Create(op);
    إذا (!بیان)
        أرجع عدم;

    بیان->دوګونی.کیڼ = کیڼ;
    بیان->دوګونی.ښی = ښی;

    أرجع بیان;
}

بیان_Def *
بیان_Create_ډګر_Ref(xkb_atom_t element, xkb_atom_t ډګر_2)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_ډګر_REF);
    إذا (!بیان)
        أرجع عدم;
    بیان->ډګر_ref.element = element;
    بیان->ډګر_ref.ډګر_2 = ډګر_2;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Array_Ref(xkb_atom_t element, xkb_atom_t ډګر_2, بیان_Def *ثبت)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_ARRAY_REF);
    إذا (!بیان)
        أرجع عدم;
    بیان->array_ref.element = element;
    بیان->array_ref.ډګر_2 = ډګر_2;
    بیان->array_ref.ثبت = ثبت;
    أرجع بیان;
}

بیان_Def *
بیان_تشه_List(فراغ)
{
    أرجع بیان_Create(STMT_بیان_تشه_LIST);
}

بیان_Def *
بیان_Create_Action(xkb_atom_t name, بیان_Def *دلیلونه)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_ACTION_DECL);
    إذا (!بیان)
        أرجع عدم;
    بیان->action.name = name;
    بیان->action.دلیلونه = دلیلونه;
    أرجع بیان;
}

بیان_Def *
بیان_Create_Action_List(بیان_Def *actions)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_ACTION_LIST);
    إذا (!بیان)
        أرجع عدم;
    بیان->actions.actions = actions;
    أرجع بیان;
}

بیان_Def *
بیان_Create_کلی_Sym_List(xkb_keysym_t sym)
{
    بیان_Def *بیان = بیان_Create(STMT_بیان_KEYSYM_LIST);
    إذا (!بیان)
        أرجع عدم;
    darray_init(بیان->keysym_list.syms);
    إذا (sym == XKB_کلی_No_نښه) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(بیان->keysym_list.syms, sym);
    }
    أرجع بیان;
}

بیان_Def *
بیان_Append_کلی_Sym_List(بیان_Def *بیان, xkb_keysym_t sym)
{
    إذا (sym == XKB_کلی_No_نښه) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(بیان->keysym_list.syms, sym);
    }
    أرجع بیان;
}

بیان_Def *
بیان_کلی_Sym_List_Append_String(بنية scanner *scanner,
                           بیان_Def *بیان, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت اندازه_t len = مزي_اوږدوالی(string);
    اندازه_t idx = 0;
    اندازه_t idx_cp = 1;
    طالما (idx < len) {
        اندازه_t شمېر = 0;
        uint32_t cp = utf8_بل_کوډ_ټکی(string + idx, len - idx, &شمېر);
        إذا (cp == INVALID_UTF8_کوډ_ټکی) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            اذهب error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_منزل_keysym(cp);
        إذا (sym == XKB_کلی_No_نښه) {
            scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            اذهب error;
        }
        darray_append(بیان->keysym_list.syms, sym);
        idx += شمېر;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    أرجع بیان;
error:
    FreeStmt((ParseCommon*) بیان);
    أرجع عدم;
}

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string)
{
    ثابت اندازه_t len = مزي_اوږدوالی(string);
    إذا (len == 0) {
        scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        أرجع XKB_کلی_No_نښه;
    }
    اندازه_t شمېر = 0;
    ثابت uint32_t cp = utf8_بل_کوډ_ټکی(string, len, &شمېر);
    إذا (cp == INVALID_UTF8_کوډ_ټکی) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        أرجع XKB_کلی_No_نښه;
    } وإلا إذا (شمېر != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        أرجع XKB_کلی_No_نښه;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_منزل_keysym(cp);
    إذا (sym == XKB_کلی_No_نښه) {
        scanner_err(scanner, XKB_LOG_پیغام_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    أرجع sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t ارزښت)
{
    KeycodeDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_KEYCODE;
    def->common.بل = عدم;
    def->name = name;
    def->ارزښت = ارزښت;

    أرجع def;
}

کلی_Alias_Def *
کلی_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    کلی_Alias_Def *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_ALIAS;
    def->common.بل = عدم;
    def->alias = alias;
    def->real = real;

    أرجع def;
}

VModDef *
VModCreate(xkb_atom_t name, بیان_Def *ارزښت)
{
    VModDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VMOD;
    def->common.بل = عدم;
    def->name = name;
    def->ارزښت = ارزښت;

    أرجع def;
}

VarDef *
VarCreate(بیان_Def *name, بیان_Def *ارزښت)
{
    VarDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VAR;
    def->common.بل = عدم;
    def->name = name;
    def->ارزښت = ارزښت;

    أرجع def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool سېټ)
{
    بیان_Def *name, *ارزښت;
    VarDef *def;
    إذا (!(name = بیان_Create_Ident(ident))) {
        أرجع عدم;
    }
    إذا (!(ارزښت = بیان_Create_Boolean(سېټ))) {
        FreeStmt((ParseCommon *) name);
        أرجع عدم;
    }
    إذا (!(def = VarCreate(name, ارزښت))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) ارزښت);
        أرجع عدم;
    }
    أرجع def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, بیان_Def *سمون)
{
    InterpDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_INTERP;
    def->common.بل = عدم;
    def->sym = sym;
    def->سمون = سمون;
    def->def = عدم;

    أرجع def;
}

کلی_Type_Def *
کلی_Type_Create(xkb_atom_t name, VarDef *body)
{
    کلی_Type_Def *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_TYPE;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t کلی_Name, VarDef *symbols)
{
    SymbolsDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_SYMBOLS;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->کلی_Name = کلی_Name;
    def->symbols = symbols;

    أرجع def;
}

ډله_Compat_Def *
ډله_Compat_Create(int64_t ډله, بیان_Def *val)
{
    ډله_Compat_Def *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_ډله_COMPAT;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->ډله = ډله;
    def->def = val;

    أرجع def;
}

ModMapDef *
ModMapCreate(بیان_Def *modifiers, بیان_Def *کلیګانې)
{
    ModMapDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_MODMAP;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->modifiers = modifiers;
    def->کلیګانې = کلیګانې;

    أرجع def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_MAP;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

LedNameDef *
LedNameCreate(int64_t ndx, بیان_Def *name, bool مجازي)
{
    LedNameDef *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_NAME;
    def->common.بل = عدم;
    def->یوځای = یوځای_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->مجازي = مجازي;

    أرجع def;
}

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type type, بنية sval name)
{
    UnknownStatement *def = حافظه_ورکړه(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = type;
    def->common.بل = عدم;
    def->name = strndup(name.پیل, name.len);
    إذا (!def->name) {
        حرر(def);
        أرجع عدم;
    }

    أرجع def;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد یوځای_ډېرپېښ یوځای)
{
    IncludeStmt *incl, *لومړی;
    حرف *stmt, *tmp;
    حرف nextop;

    incl = لومړی = عدم;
    tmp = str;
    stmt = strdup_خوندي(str);
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

        إذا (لومړی == عدم) {
            لومړی = incl = حافظه_ورکړه(حجم(*لومړی));
        } وإلا {
            incl->بل_incl = حافظه_ورکړه(حجم(*لومړی));
            incl = incl->بل_incl;
        }

        إذا (!incl) {
            حرر(file);
            حرر(map);
            حرر(extra_معلومات);
            اكسر;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.بل = عدم;
        incl->یوځای = یوځای;
        incl->stmt = عدم;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_معلومات;
        incl->بل_incl = عدم;

        بدل (nextop) {
        حالة یوځای_AUGMENT_مختاړی:
            یوځای = یوځای_AUGMENT;
            اكسر;
        حالة یوځای_REPLACE_مختاړی:
            یوځای = یوځای_REPLACE;
            اكسر;
        افتراضي:
            یوځای = یوځای_OVERRIDE;
        }
    }

    إذا (لومړی)
        لومړی->stmt = stmt;
    وإلا
        حرر(stmt);

    أرجع لومړی;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(لومړی);
    حرر(stmt);
    أرجع عدم;
}

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags)
{
    XkbFile *file;

    file = حافظه_صفر_ورکړه(1, حجم(*file));
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
                      ثابت بنية xkb_component_نومونه *kkctgs)
{
    حرف *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    تعداد xkb_file_type type;
    IncludeStmt *include = عدم;
    XkbFile *file = عدم;
    ParseCommon *defs = عدم, *defs_وروستی = عدم;

    لكل (type = لومړی_KEYMAP_FILE_TYPE; type <= وروستی_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], یوځای_DEFAULT);
        إذا (!include)
            اذهب err;

        file = XkbFileCreate(type, عدم, (ParseCommon *) include, 0);
        إذا (!file) {
            FreeInclude(include);
            اذهب err;
        }

        إذا (!defs)
            defs_وروستی = defs = &file->common;
        وإلا
            defs_وروستی = defs_وروستی->بل = &file->common;
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
    IncludeStmt *بل;

    طالما (incl)
    {
        بل = incl->بل_incl;

        حرر(incl->file);
        حرر(incl->map);
        حرر(incl->modifier);
        حرر(incl->stmt);

        حرر(incl);
        incl = بل;
    }
}

فراغ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *بل;

    طالما (stmt)
    {
        بل = stmt->بل;

        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = عدم;
            اكسر;

        حالة STMT_بیان_NEGATE:
        حالة STMT_بیان_UNARY_PLUS:
        حالة STMT_بیان_NOT:
        حالة STMT_بیان_INVERT:
            FreeStmt((ParseCommon *) ((بیان_Unary *) stmt)->ماشوم);
            اكسر;

        حالة STMT_بیان_DIVIDE:
        حالة STMT_بیان_جمع_کړه:
        حالة STMT_بیان_کمول:
        حالة STMT_بیان_MULTIPLY:
        حالة STMT_بیان_ټاکل:
            FreeStmt((ParseCommon *) ((بیان_دوګونی *) stmt)->کیڼ);
            FreeStmt((ParseCommon *) ((بیان_دوګونی *) stmt)->ښی);
            اكسر;

        حالة STMT_بیان_ACTION_DECL:
            FreeStmt((ParseCommon *) ((بیان_Action *) stmt)->دلیلونه);
            اكسر;

        حالة STMT_بیان_ACTION_LIST:
            FreeStmt((ParseCommon *) ((بیان_Action_List *) stmt)->actions);
            اكسر;

        حالة STMT_بیان_ARRAY_REF:
            FreeStmt((ParseCommon *) ((بیان_Array_Ref *) stmt)->ثبت);
            اكسر;

        حالة STMT_بیان_KEYSYM_LIST:
            darray_free(((بیان_Keysym_List *) stmt)->syms);
            اكسر;

        حالة STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->ارزښت);
            اكسر;
        حالة STMT_TYPE:
            FreeStmt((ParseCommon *) ((کلی_Type_Def *) stmt)->body);
            اكسر;
        حالة STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->سمون);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            اكسر;
        حالة STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->ارزښت);
            اكسر;
        حالة STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            اكسر;
        حالة STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->کلیګانې);
            اكسر;
        حالة STMT_ډله_COMPAT:
            FreeStmt((ParseCommon *) ((ډله_Compat_Def *) stmt)->def);
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
            static_assert(STMT_عدد_ارزښتونه == 37 &&
                          STMT_عدد_ارزښتونه == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            اكسر;
        }

        حرر(stmt);
        stmt = بل;
    }
}

فراغ
FreeXkbFile(XkbFile *file)
{
    XkbFile *بل;

    طالما (file)
    {
        بل = (XkbFile *) file->common.بل;

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
        file = بل;
    }
}

ساكن ثابت حرف *xkb_file_type_مزي_ګانې[FILE_TYPE_عدد_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_قواعد] = "rules",
};

ثابت حرف *
xkb_file_type_منزل_string(تعداد xkb_file_type type)
{
    إذا (type >= FILE_TYPE_عدد_ENTRIES)
        أرجع "unknown";
    أرجع xkb_file_type_مزي_ګانې[type];
}

ساكن ثابت حرف *stmt_type_مزي_ګانې[STMT_عدد_ارزښتونه] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_بیان_STRING_LITERAL] = "string literal expression",
    [STMT_بیان_INTEGER_LITERAL] = "integer literal expression",
    [STMT_بیان_FLOAT_LITERAL] = "float literal expression",
    [STMT_بیان_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_بیان_KEYNAME_LITERAL] = "key name expression",
    [STMT_بیان_KEYSYM_LITERAL] = "keysym expression",
    [STMT_بیان_IDENT] = "identifier expression",
    [STMT_بیان_ACTION_DECL] = "action declaration expression",
    [STMT_بیان_ډګر_REF] = "field reference expression",
    [STMT_بیان_ARRAY_REF] = "array reference expression",
    [STMT_بیان_تشه_LIST] = "empty list expression",
    [STMT_بیان_KEYSYM_LIST] = "keysym list expression",
    [STMT_بیان_ACTION_LIST] = "action list expression",
    [STMT_بیان_جمع_کړه] = "addition expression",
    [STMT_بیان_کمول] = "substraction expression",
    [STMT_بیان_MULTIPLY] = "multiplication expression",
    [STMT_بیان_DIVIDE] = "division expression",
    [STMT_بیان_ټاکل] = "assignment expression",
    [STMT_بیان_NOT] = "logical negation expression",
    [STMT_بیان_NEGATE] = "arithmetic negation expression",
    [STMT_بیان_INVERT] = "bitwise inversion expression",
    [STMT_بیان_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_ډله_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_عدد_ارزښتونه == 37 &&
              STMT_عدد_ارزښتونه == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_منزل_string(تعداد stmt_type type)
{
    إذا (type >= STMT_عدد_ارزښتونه)
        أرجع عدم;
    أرجع stmt_type_مزي_ګانې[type];
}

حرف
stmt_type_منزل_operator_char(تعداد stmt_type type)
{
    بدل (type) {
    حالة STMT_بیان_جمع_کړه:
        أرجع '+';
    حالة STMT_بیان_کمول:
        أرجع '-';
    حالة STMT_بیان_MULTIPLY:
        أرجع '*';
    حالة STMT_بیان_DIVIDE:
        أرجع '/';
    حالة STMT_بیان_NOT:
        أرجع '!';
    حالة STMT_بیان_NEGATE:
        أرجع '-';
    حالة STMT_بیان_INVERT:
        أرجع '~';
    حالة STMT_بیان_UNARY_PLUS:
        أرجع '+';
    افتراضي:
        أرجع '\0';
    }
}
