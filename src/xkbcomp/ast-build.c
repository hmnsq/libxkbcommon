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

ساكن عبارت_Def *
عبارت_Create(تعداد stmt_type op)
{
    عبارت_Def *عبارت = حافظه_بده(حجم(*عبارت));
    إذا (!عبارت)
        أرجع عدم;

    عبارت->common.type = op;
    عبارت->common.بعدی = عدم;

    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_String(xkb_atom_t str)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_STRING_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->string.str = str;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Integer(int64_t ival)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_INTEGER_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->integer.ival = ival;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Float(فراغ)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_FLOAT_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Boolean(bool مجموعه)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_BOOLEAN_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->boolean.مجموعه = مجموعه;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Name(xkb_atom_t کلید_name)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYNAME_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->کلید_name.کلید_name = کلید_name;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Sym(xkb_keysym_t keysym)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYSYM_LITERAL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->keysym.keysym = keysym;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Ident(xkb_atom_t ident)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_IDENT);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->ident.ident = ident;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Unary(تعداد stmt_type op, عبارت_Def *فرزند)
{
    عبارت_Def *عبارت = عبارت_Create(op);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->unary.فرزند = فرزند;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_دودویی(تعداد stmt_type op, عبارت_Def *چپ, عبارت_Def *راست)
{
    عبارت_Def *عبارت = عبارت_Create(op);
    إذا (!عبارت)
        أرجع عدم;

    عبارت->دودویی.چپ = چپ;
    عبارت->دودویی.راست = راست;

    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_فیلد_Ref(xkb_atom_t element, xkb_atom_t فیلد_2)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_فیلد_REF);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->فیلد_ref.element = element;
    عبارت->فیلد_ref.فیلد_2 = فیلد_2;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Array_Ref(xkb_atom_t element, xkb_atom_t فیلد_2, عبارت_Def *مدخل)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ARRAY_REF);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->array_ref.element = element;
    عبارت->array_ref.فیلد_2 = فیلد_2;
    عبارت->array_ref.مدخل = مدخل;
    أرجع عبارت;
}

عبارت_Def *
عبارت_خالی_List(فراغ)
{
    أرجع عبارت_Create(STMT_عبارت_خالی_LIST);
}

عبارت_Def *
عبارت_Create_Action(xkb_atom_t name, عبارت_Def *آرگومانها)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ACTION_DECL);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->action.name = name;
    عبارت->action.آرگومانها = آرگومانها;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_Action_List(عبارت_Def *actions)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_ACTION_LIST);
    إذا (!عبارت)
        أرجع عدم;
    عبارت->actions.actions = actions;
    أرجع عبارت;
}

عبارت_Def *
عبارت_Create_کلید_Sym_List(xkb_keysym_t sym)
{
    عبارت_Def *عبارت = عبارت_Create(STMT_عبارت_KEYSYM_LIST);
    إذا (!عبارت)
        أرجع عدم;
    darray_init(عبارت->keysym_list.syms);
    إذا (sym == XKB_کلید_No_نماد) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(عبارت->keysym_list.syms, sym);
    }
    أرجع عبارت;
}

عبارت_Def *
عبارت_Append_کلید_Sym_List(عبارت_Def *عبارت, xkb_keysym_t sym)
{
    إذا (sym == XKB_کلید_No_نماد) {
        /* Discard NoSymbol */
    } وإلا {
        darray_append(عبارت->keysym_list.syms, sym);
    }
    أرجع عبارت;
}

عبارت_Def *
عبارت_کلید_Sym_List_Append_String(بنية scanner *scanner,
                           عبارت_Def *عبارت, ثابت حرف *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    ثابت اندازه_t len = طول_رشته(string);
    اندازه_t idx = 0;
    اندازه_t idx_cp = 1;
    طالما (idx < len) {
        اندازه_t شمار = 0;
        uint32_t cp = utf8_بعدی_کد_نقطه(string + idx, len - idx, &شمار);
        إذا (cp == INVALID_UTF8_کد_نقطه) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            اذهب error;
        }
        ثابت xkb_keysym_t sym = xkb_utf32_مقصد_keysym(cp);
        إذا (sym == XKB_کلید_No_نماد) {
            scanner_err(scanner, XKB_LOG_پیام_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            اذهب error;
        }
        darray_append(عبارت->keysym_list.syms, sym);
        idx += شمار;
        idx_cp++;
    }
    أكد(string[idx] == '\0');
    أرجع عبارت;
error:
    FreeStmt((ParseCommon*) عبارت);
    أرجع عدم;
}

xkb_keysym_t
KeysymParseString(بنية scanner *scanner, ثابت حرف *string)
{
    ثابت اندازه_t len = طول_رشته(string);
    إذا (len == 0) {
        scanner_err(scanner, XKB_LOG_پیام_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        أرجع XKB_کلید_No_نماد;
    }
    اندازه_t شمار = 0;
    ثابت uint32_t cp = utf8_بعدی_کد_نقطه(string, len, &شمار);
    إذا (cp == INVALID_UTF8_کد_نقطه) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        أرجع XKB_کلید_No_نماد;
    } وإلا إذا (شمار != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        أرجع XKB_کلید_No_نماد;
    }
    ثابت xkb_keysym_t sym = xkb_utf32_مقصد_keysym(cp);
    إذا (sym == XKB_کلید_No_نماد) {
        scanner_err(scanner, XKB_LOG_پیام_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    أرجع sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t مقدار)
{
    KeycodeDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_KEYCODE;
    def->common.بعدی = عدم;
    def->name = name;
    def->مقدار = مقدار;

    أرجع def;
}

کلید_Alias_Def *
کلید_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    کلید_Alias_Def *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_ALIAS;
    def->common.بعدی = عدم;
    def->alias = alias;
    def->real = real;

    أرجع def;
}

VModDef *
VModCreate(xkb_atom_t name, عبارت_Def *مقدار)
{
    VModDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VMOD;
    def->common.بعدی = عدم;
    def->name = name;
    def->مقدار = مقدار;

    أرجع def;
}

VarDef *
VarCreate(عبارت_Def *name, عبارت_Def *مقدار)
{
    VarDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_VAR;
    def->common.بعدی = عدم;
    def->name = name;
    def->مقدار = مقدار;

    أرجع def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool مجموعه)
{
    عبارت_Def *name, *مقدار;
    VarDef *def;
    إذا (!(name = عبارت_Create_Ident(ident))) {
        أرجع عدم;
    }
    إذا (!(مقدار = عبارت_Create_Boolean(مجموعه))) {
        FreeStmt((ParseCommon *) name);
        أرجع عدم;
    }
    إذا (!(def = VarCreate(name, مقدار))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) مقدار);
        أرجع عدم;
    }
    أرجع def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, عبارت_Def *تطابق)
{
    InterpDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_INTERP;
    def->common.بعدی = عدم;
    def->sym = sym;
    def->تطابق = تطابق;
    def->def = عدم;

    أرجع def;
}

کلید_Type_Def *
کلید_Type_Create(xkb_atom_t name, VarDef *body)
{
    کلید_Type_Def *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_TYPE;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t کلید_Name, VarDef *symbols)
{
    SymbolsDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_SYMBOLS;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->کلید_Name = کلید_Name;
    def->symbols = symbols;

    أرجع def;
}

گروه_Compat_Def *
گروه_Compat_Create(int64_t گروه, عبارت_Def *val)
{
    گروه_Compat_Def *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_گروه_COMPAT;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->گروه = گروه;
    def->def = val;

    أرجع def;
}

ModMapDef *
ModMapCreate(عبارت_Def *modifiers, عبارت_Def *کلیدها)
{
    ModMapDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_MODMAP;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->modifiers = modifiers;
    def->کلیدها = کلیدها;

    أرجع def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_MAP;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->name = name;
    def->body = body;

    أرجع def;
}

LedNameDef *
LedNameCreate(int64_t ndx, عبارت_Def *name, bool مجازی)
{
    LedNameDef *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = STMT_LED_NAME;
    def->common.بعدی = عدم;
    def->ادغام = ادغام_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->مجازی = مجازی;

    أرجع def;
}

UnknownStatement *
UnknownStatementCreate(تعداد stmt_type type, بنية sval name)
{
    UnknownStatement *def = حافظه_بده(حجم(*def));
    إذا (!def)
        أرجع عدم;

    def->common.type = type;
    def->common.بعدی = عدم;
    def->name = strndup(name.شروع, name.len);
    إذا (!def->name) {
        حرر(def);
        أرجع عدم;
    }

    أرجع def;
}

ساكن فراغ
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(بنية xkb_context *ctx, حرف *str, تعداد ادغام_نما ادغام)
{
    IncludeStmt *incl, *اولی;
    حرف *stmt, *tmp;
    حرف nextop;

    incl = اولی = عدم;
    tmp = str;
    stmt = strdup_امن(str);
    طالما (tmp && *tmp)
    {
        حرف *file = عدم, *map = عدم, *extra_داده = عدم;

        إذا (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_داده))
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
            حرر(extra_داده);
            استمر;
        }

        إذا (اولی == عدم) {
            اولی = incl = حافظه_بده(حجم(*اولی));
        } وإلا {
            incl->بعدی_incl = حافظه_بده(حجم(*اولی));
            incl = incl->بعدی_incl;
        }

        إذا (!incl) {
            حرر(file);
            حرر(map);
            حرر(extra_داده);
            اكسر;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.بعدی = عدم;
        incl->ادغام = ادغام;
        incl->stmt = عدم;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_داده;
        incl->بعدی_incl = عدم;

        بدل (nextop) {
        حالة ادغام_AUGMENT_پیشوند:
            ادغام = ادغام_AUGMENT;
            اكسر;
        حالة ادغام_REPLACE_پیشوند:
            ادغام = ادغام_REPLACE;
            اكسر;
        افتراضي:
            ادغام = ادغام_OVERRIDE;
        }
    }

    إذا (اولی)
        اولی->stmt = stmt;
    وإلا
        حرر(stmt);

    أرجع اولی;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(اولی);
    حرر(stmt);
    أرجع عدم;
}

XkbFile *
XkbFileCreate(تعداد xkb_file_type type, حرف *name, ParseCommon *defs,
              تعداد xkb_map_flags flags)
{
    XkbFile *file;

    file = حافظه_صفر_بده(1, حجم(*file));
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
                      ثابت بنية xkb_component_نامها *kkctgs)
{
    حرف *ثابت components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    تعداد xkb_file_type type;
    IncludeStmt *include = عدم;
    XkbFile *file = عدم;
    ParseCommon *defs = عدم, *defs_آخرین = عدم;

    لكل (type = اولی_KEYMAP_FILE_TYPE; type <= آخرین_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], ادغام_DEFAULT);
        إذا (!include)
            اذهب err;

        file = XkbFileCreate(type, عدم, (ParseCommon *) include, 0);
        إذا (!file) {
            FreeInclude(include);
            اذهب err;
        }

        إذا (!defs)
            defs_آخرین = defs = &file->common;
        وإلا
            defs_آخرین = defs_آخرین->بعدی = &file->common;
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
    IncludeStmt *بعدی;

    طالما (incl)
    {
        بعدی = incl->بعدی_incl;

        حرر(incl->file);
        حرر(incl->map);
        حرر(incl->modifier);
        حرر(incl->stmt);

        حرر(incl);
        incl = بعدی;
    }
}

فراغ
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *بعدی;

    طالما (stmt)
    {
        بعدی = stmt->بعدی;

        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = عدم;
            اكسر;

        حالة STMT_عبارت_NEGATE:
        حالة STMT_عبارت_UNARY_PLUS:
        حالة STMT_عبارت_NOT:
        حالة STMT_عبارت_INVERT:
            FreeStmt((ParseCommon *) ((عبارت_Unary *) stmt)->فرزند);
            اكسر;

        حالة STMT_عبارت_DIVIDE:
        حالة STMT_عبارت_جمع_کن:
        حالة STMT_عبارت_تفریق:
        حالة STMT_عبارت_MULTIPLY:
        حالة STMT_عبارت_تخصیص_بده:
            FreeStmt((ParseCommon *) ((عبارت_دودویی *) stmt)->چپ);
            FreeStmt((ParseCommon *) ((عبارت_دودویی *) stmt)->راست);
            اكسر;

        حالة STMT_عبارت_ACTION_DECL:
            FreeStmt((ParseCommon *) ((عبارت_Action *) stmt)->آرگومانها);
            اكسر;

        حالة STMT_عبارت_ACTION_LIST:
            FreeStmt((ParseCommon *) ((عبارت_Action_List *) stmt)->actions);
            اكسر;

        حالة STMT_عبارت_ARRAY_REF:
            FreeStmt((ParseCommon *) ((عبارت_Array_Ref *) stmt)->مدخل);
            اكسر;

        حالة STMT_عبارت_KEYSYM_LIST:
            darray_free(((عبارت_Keysym_List *) stmt)->syms);
            اكسر;

        حالة STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->مقدار);
            اكسر;
        حالة STMT_TYPE:
            FreeStmt((ParseCommon *) ((کلید_Type_Def *) stmt)->body);
            اكسر;
        حالة STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->تطابق);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            اكسر;
        حالة STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->مقدار);
            اكسر;
        حالة STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            اكسر;
        حالة STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->کلیدها);
            اكسر;
        حالة STMT_گروه_COMPAT:
            FreeStmt((ParseCommon *) ((گروه_Compat_Def *) stmt)->def);
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
            static_assert(STMT_رقم_مقادیر == 37 &&
                          STMT_رقم_مقادیر == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            اكسر;
        }

        حرر(stmt);
        stmt = بعدی;
    }
}

فراغ
FreeXkbFile(XkbFile *file)
{
    XkbFile *بعدی;

    طالما (file)
    {
        بعدی = (XkbFile *) file->common.بعدی;

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
        file = بعدی;
    }
}

ساكن ثابت حرف *xkb_file_type_رشتهها[FILE_TYPE_رقم_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_قواعد] = "rules",
};

ثابت حرف *
xkb_file_type_مقصد_string(تعداد xkb_file_type type)
{
    إذا (type >= FILE_TYPE_رقم_ENTRIES)
        أرجع "unknown";
    أرجع xkb_file_type_رشتهها[type];
}

ساكن ثابت حرف *stmt_type_رشتهها[STMT_رقم_مقادیر] = {
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
    [STMT_عبارت_فیلد_REF] = "field reference expression",
    [STMT_عبارت_ARRAY_REF] = "array reference expression",
    [STMT_عبارت_خالی_LIST] = "empty list expression",
    [STMT_عبارت_KEYSYM_LIST] = "keysym list expression",
    [STMT_عبارت_ACTION_LIST] = "action list expression",
    [STMT_عبارت_جمع_کن] = "addition expression",
    [STMT_عبارت_تفریق] = "substraction expression",
    [STMT_عبارت_MULTIPLY] = "multiplication expression",
    [STMT_عبارت_DIVIDE] = "division expression",
    [STMT_عبارت_تخصیص_بده] = "assignment expression",
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
    [STMT_گروه_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_رقم_مقادیر == 37 &&
              STMT_رقم_مقادیر == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

ثابت حرف *
stmt_type_مقصد_string(تعداد stmt_type type)
{
    إذا (type >= STMT_رقم_مقادیر)
        أرجع عدم;
    أرجع stmt_type_رشتهها[type];
}

حرف
stmt_type_مقصد_operator_char(تعداد stmt_type type)
{
    بدل (type) {
    حالة STMT_عبارت_جمع_کن:
        أرجع '+';
    حالة STMT_عبارت_تفریق:
        أرجع '-';
    حالة STMT_عبارت_MULTIPLY:
        أرجع '*';
    حالة STMT_عبارت_DIVIDE:
        أرجع '/';
    حالة STMT_عبارت_NOT:
        أرجع '!';
    حالة STMT_عبارت_NEGATE:
        أرجع '-';
    حالة STMT_عبارت_INVERT:
        أرجع '~';
    حالة STMT_عبارت_UNARY_PLUS:
        أرجع '+';
    افتراضي:
        أرجع '\0';
    }
}
