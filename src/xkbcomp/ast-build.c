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

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon-keysyms.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "ast.h"
#అన్తర్భావ "ast-build.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "utf8-decoding.h"

స్థిర సమాసం_Def *
సమాసం_Create(గణన stmt_type op)
{
    సమాసం_Def *సమాసం = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*సమాసం));
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;

    సమాసం->common.type = op;
    సమాసం->common.తదుపరి = ఖాళీ;

    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_String(xkb_atom_t str)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_STRING_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->string.str = str;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Integer(int64_t ival)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_INTEGER_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->integer.ival = ival;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Float(శూన్య)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_FLOAT_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Boolean(bool సమితి)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_BOOLEAN_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->boolean.సమితి = సమితి;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_కీలకం_Name(xkb_atom_t కీలకం_name)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_KEYNAME_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->కీలకం_name.కీలకం_name = కీలకం_name;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_కీలకం_Sym(xkb_keysym_t keysym)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_KEYSYM_LITERAL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->keysym.keysym = keysym;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Ident(xkb_atom_t ident)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_IDENT);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->ident.ident = ident;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Unary(గణన stmt_type op, సమాసం_Def *బిడ్డ)
{
    సమాసం_Def *సమాసం = సమాసం_Create(op);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->unary.బిడ్డ = బిడ్డ;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_ద్విమాన(గణన stmt_type op, సమాసం_Def *ఎడమ, సమాసం_Def *కుడి)
{
    సమాసం_Def *సమాసం = సమాసం_Create(op);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;

    సమాసం->ద్విమాన.ఎడమ = ఎడమ;
    సమాసం->ద్విమాన.కుడి = కుడి;

    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_క్షేత్రం_Ref(xkb_atom_t element, xkb_atom_t క్షేత్రం_2)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_క్షేత్రం_REF);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->క్షేత్రం_ref.element = element;
    సమాసం->క్షేత్రం_ref.క్షేత్రం_2 = క్షేత్రం_2;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Array_Ref(xkb_atom_t element, xkb_atom_t క్షేత్రం_2, సమాసం_Def *నమోదు_2)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_ARRAY_REF);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->array_ref.element = element;
    సమాసం->array_ref.క్షేత్రం_2 = క్షేత్రం_2;
    సమాసం->array_ref.నమోదు_2 = నమోదు_2;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_ఖాళీలు_List(శూన్య)
{
    తిరిగి సమాసం_Create(STMT_సమాసం_ఖాళీలు_LIST);
}

సమాసం_Def *
సమాసం_Create_Action(xkb_atom_t name, సమాసం_Def *వాదనలు)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_ACTION_DECL);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->action.name = name;
    సమాసం->action.వాదనలు = వాదనలు;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_Action_List(సమాసం_Def *actions)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_ACTION_LIST);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    సమాసం->actions.actions = actions;
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Create_కీలకం_Sym_List(xkb_keysym_t sym)
{
    సమాసం_Def *సమాసం = సమాసం_Create(STMT_సమాసం_KEYSYM_LIST);
    ఒకవేళ (!సమాసం)
        తిరిగి ఖాళీ;
    darray_init(సమాసం->keysym_list.syms);
    ఒకవేళ (sym == XKB_కీలకం_No_చిహ్నం) {
        /* Discard NoSymbol */
    } లేకపోతే {
        darray_append(సమాసం->keysym_list.syms, sym);
    }
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_Append_కీలకం_Sym_List(సమాసం_Def *సమాసం, xkb_keysym_t sym)
{
    ఒకవేళ (sym == XKB_కీలకం_No_చిహ్నం) {
        /* Discard NoSymbol */
    } లేకపోతే {
        darray_append(సమాసం->keysym_list.syms, sym);
    }
    తిరిగి సమాసం;
}

సమాసం_Def *
సమాసం_కీలకం_Sym_List_Append_String(నిర్మాణం scanner *scanner,
                           సమాసం_Def *సమాసం, స్థిరరాశి అక్షరం *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    స్థిరరాశి కొలత_t len = అక్షరశ్రేణి_పొడవు(string);
    కొలత_t idx = 0;
    కొలత_t idx_cp = 1;
    అయితే (idx < len) {
        కొలత_t లెక్క = 0;
        uint32_t cp = utf8_తదుపరి_సంకేతం_బిందువు(string + idx, len - idx, &లెక్క);
        ఒకవేళ (cp == INVALID_UTF8_సంకేతం_బిందువు) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            వెళ్ళు error;
        }
        స్థిరరాశి xkb_keysym_t sym = xkb_utf32_గమ్యం_keysym(cp);
        ఒకవేళ (sym == XKB_కీలకం_No_చిహ్నం) {
            scanner_err(scanner, XKB_LOG_సందేశం_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            వెళ్ళు error;
        }
        darray_append(సమాసం->keysym_list.syms, sym);
        idx += లెక్క;
        idx_cp++;
    }
    ప్రతిజ్ఞా(string[idx] == '\0');
    తిరిగి సమాసం;
error:
    FreeStmt((ParseCommon*) సమాసం);
    తిరిగి ఖాళీ;
}

xkb_keysym_t
KeysymParseString(నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *string)
{
    స్థిరరాశి కొలత_t len = అక్షరశ్రేణి_పొడవు(string);
    ఒకవేళ (len == 0) {
        scanner_err(scanner, XKB_LOG_సందేశం_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    }
    కొలత_t లెక్క = 0;
    స్థిరరాశి uint32_t cp = utf8_తదుపరి_సంకేతం_బిందువు(string, len, &లెక్క);
    ఒకవేళ (cp == INVALID_UTF8_సంకేతం_బిందువు) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    } లేకపోతే ఒకవేళ (లెక్క != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    }
    స్థిరరాశి xkb_keysym_t sym = xkb_utf32_గమ్యం_keysym(cp);
    ఒకవేళ (sym == XKB_కీలకం_No_చిహ్నం) {
        scanner_err(scanner, XKB_LOG_సందేశం_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    తిరిగి sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t విలువ)
{
    KeycodeDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_KEYCODE;
    def->common.తదుపరి = ఖాళీ;
    def->name = name;
    def->విలువ = విలువ;

    తిరిగి def;
}

కీలకం_Alias_Def *
కీలకం_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    కీలకం_Alias_Def *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_ALIAS;
    def->common.తదుపరి = ఖాళీ;
    def->alias = alias;
    def->real = real;

    తిరిగి def;
}

VModDef *
VModCreate(xkb_atom_t name, సమాసం_Def *విలువ)
{
    VModDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_VMOD;
    def->common.తదుపరి = ఖాళీ;
    def->name = name;
    def->విలువ = విలువ;

    తిరిగి def;
}

VarDef *
VarCreate(సమాసం_Def *name, సమాసం_Def *విలువ)
{
    VarDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_VAR;
    def->common.తదుపరి = ఖాళీ;
    def->name = name;
    def->విలువ = విలువ;

    తిరిగి def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool సమితి)
{
    సమాసం_Def *name, *విలువ;
    VarDef *def;
    ఒకవేళ (!(name = సమాసం_Create_Ident(ident))) {
        తిరిగి ఖాళీ;
    }
    ఒకవేళ (!(విలువ = సమాసం_Create_Boolean(సమితి))) {
        FreeStmt((ParseCommon *) name);
        తిరిగి ఖాళీ;
    }
    ఒకవేళ (!(def = VarCreate(name, విలువ))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) విలువ);
        తిరిగి ఖాళీ;
    }
    తిరిగి def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, సమాసం_Def *సరిపోలిక)
{
    InterpDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_INTERP;
    def->common.తదుపరి = ఖాళీ;
    def->sym = sym;
    def->సరిపోలిక = సరిపోలిక;
    def->def = ఖాళీ;

    తిరిగి def;
}

కీలకం_Type_Def *
కీలకం_Type_Create(xkb_atom_t name, VarDef *body)
{
    కీలకం_Type_Def *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_TYPE;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->name = name;
    def->body = body;

    తిరిగి def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t కీలకం_Name, VarDef *symbols)
{
    SymbolsDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_SYMBOLS;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->కీలకం_Name = కీలకం_Name;
    def->symbols = symbols;

    తిరిగి def;
}

సమూహం_Compat_Def *
సమూహం_Compat_Create(int64_t సమూహం, సమాసం_Def *val)
{
    సమూహం_Compat_Def *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_సమూహం_COMPAT;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->సమూహం = సమూహం;
    def->def = val;

    తిరిగి def;
}

ModMapDef *
ModMapCreate(సమాసం_Def *modifiers, సమాసం_Def *కీలకాలు)
{
    ModMapDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_MODMAP;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->modifiers = modifiers;
    def->కీలకాలు = కీలకాలు;

    తిరిగి def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_LED_MAP;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->name = name;
    def->body = body;

    తిరిగి def;
}

LedNameDef *
LedNameCreate(int64_t ndx, సమాసం_Def *name, bool మాయా)
{
    LedNameDef *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = STMT_LED_NAME;
    def->common.తదుపరి = ఖాళీ;
    def->విలీనం = విలీనం_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->మాయా = మాయా;

    తిరిగి def;
}

UnknownStatement *
UnknownStatementCreate(గణన stmt_type type, నిర్మాణం sval name)
{
    UnknownStatement *def = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*def));
    ఒకవేళ (!def)
        తిరిగి ఖాళీ;

    def->common.type = type;
    def->common.తదుపరి = ఖాళీ;
    def->name = strndup(name.ప్రారంభం, name.len);
    ఒకవేళ (!def->name) {
        విడుదల(def);
        తిరిగి ఖాళీ;
    }

    తిరిగి def;
}

స్థిర శూన్య
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(నిర్మాణం xkb_context *ctx, అక్షరం *str, గణన విలీనం_బహుళకం విలీనం)
{
    IncludeStmt *incl, *మొదటి;
    అక్షరం *stmt, *tmp;
    అక్షరం nextop;

    incl = మొదటి = ఖాళీ;
    tmp = str;
    stmt = strdup_సురక్షిత(str);
    అయితే (tmp && *tmp)
    {
        అక్షరం *file = ఖాళీ, *map = ఖాళీ, *extra_దత్తాంశం = ఖాళీ;

        ఒకవేళ (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_దత్తాంశం))
            వెళ్ళు err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        ఒకవేళ (isempty(file)) {
            విడుదల(file);
            విడుదల(map);
            విడుదల(extra_దత్తాంశం);
            కొనసాగు;
        }

        ఒకవేళ (మొదటి == ఖాళీ) {
            మొదటి = incl = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*మొదటి));
        } లేకపోతే {
            incl->తదుపరి_incl = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*మొదటి));
            incl = incl->తదుపరి_incl;
        }

        ఒకవేళ (!incl) {
            విడుదల(file);
            విడుదల(map);
            విడుదల(extra_దత్తాంశం);
            ఆపు;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.తదుపరి = ఖాళీ;
        incl->విలీనం = విలీనం;
        incl->stmt = ఖాళీ;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_దత్తాంశం;
        incl->తదుపరి_incl = ఖాళీ;

        ఎంపిక (nextop) {
        స్థితి విలీనం_AUGMENT_ఉపసర్గ:
            విలీనం = విలీనం_AUGMENT;
            ఆపు;
        స్థితి విలీనం_REPLACE_ఉపసర్గ:
            విలీనం = విలీనం_REPLACE;
            ఆపు;
        అప్రమేయ:
            విలీనం = విలీనం_OVERRIDE;
        }
    }

    ఒకవేళ (మొదటి)
        మొదటి->stmt = stmt;
    లేకపోతే
        విడుదల(stmt);

    తిరిగి మొదటి;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(మొదటి);
    విడుదల(stmt);
    తిరిగి ఖాళీ;
}

XkbFile *
XkbFileCreate(గణన xkb_file_type type, అక్షరం *name, ParseCommon *defs,
              గణన xkb_map_flags flags)
{
    XkbFile *file;

    file = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*file));
    ఒకవేళ (!file)
        తిరిగి ఖాళీ;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    తిరిగి file;
}

XkbFile *
XkbFileFromComponents(నిర్మాణం xkb_context *ctx,
                      స్థిరరాశి నిర్మాణం xkb_component_పేర్లు *kkctgs)
{
    అక్షరం *స్థిరరాశి components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    గణన xkb_file_type type;
    IncludeStmt *include = ఖాళీ;
    XkbFile *file = ఖాళీ;
    ParseCommon *defs = ఖాళీ, *defs_చివరి = ఖాళీ;

    కొరకు (type = మొదటి_KEYMAP_FILE_TYPE; type <= చివరి_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], విలీనం_DEFAULT);
        ఒకవేళ (!include)
            వెళ్ళు err;

        file = XkbFileCreate(type, ఖాళీ, (ParseCommon *) include, 0);
        ఒకవేళ (!file) {
            FreeInclude(include);
            వెళ్ళు err;
        }

        ఒకవేళ (!defs)
            defs_చివరి = defs = &file->common;
        లేకపోతే
            defs_చివరి = defs_చివరి->తదుపరి = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, ఖాళీ, defs, 0);
    ఒకవేళ (!file)
        వెళ్ళు err;

    తిరిగి file;

err:
    FreeXkbFile((XkbFile *) defs);
    తిరిగి ఖాళీ;
}

స్థిర శూన్య
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *తదుపరి;

    అయితే (incl)
    {
        తదుపరి = incl->తదుపరి_incl;

        విడుదల(incl->file);
        విడుదల(incl->map);
        విడుదల(incl->modifier);
        విడుదల(incl->stmt);

        విడుదల(incl);
        incl = తదుపరి;
    }
}

శూన్య
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *తదుపరి;

    అయితే (stmt)
    {
        తదుపరి = stmt->తదుపరి;

        ఎంపిక (stmt->type) {
        స్థితి STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = ఖాళీ;
            ఆపు;

        స్థితి STMT_సమాసం_NEGATE:
        స్థితి STMT_సమాసం_UNARY_PLUS:
        స్థితి STMT_సమాసం_NOT:
        స్థితి STMT_సమాసం_INVERT:
            FreeStmt((ParseCommon *) ((సమాసం_Unary *) stmt)->బిడ్డ);
            ఆపు;

        స్థితి STMT_సమాసం_DIVIDE:
        స్థితి STMT_సమాసం_కూడు:
        స్థితి STMT_సమాసం_వ్యవకలనం:
        స్థితి STMT_సమాసం_MULTIPLY:
        స్థితి STMT_సమాసం_కేటాయించు:
            FreeStmt((ParseCommon *) ((సమాసం_ద్విమాన *) stmt)->ఎడమ);
            FreeStmt((ParseCommon *) ((సమాసం_ద్విమాన *) stmt)->కుడి);
            ఆపు;

        స్థితి STMT_సమాసం_ACTION_DECL:
            FreeStmt((ParseCommon *) ((సమాసం_Action *) stmt)->వాదనలు);
            ఆపు;

        స్థితి STMT_సమాసం_ACTION_LIST:
            FreeStmt((ParseCommon *) ((సమాసం_Action_List *) stmt)->actions);
            ఆపు;

        స్థితి STMT_సమాసం_ARRAY_REF:
            FreeStmt((ParseCommon *) ((సమాసం_Array_Ref *) stmt)->నమోదు_2);
            ఆపు;

        స్థితి STMT_సమాసం_KEYSYM_LIST:
            darray_free(((సమాసం_Keysym_List *) stmt)->syms);
            ఆపు;

        స్థితి STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->విలువ);
            ఆపు;
        స్థితి STMT_TYPE:
            FreeStmt((ParseCommon *) ((కీలకం_Type_Def *) stmt)->body);
            ఆపు;
        స్థితి STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->సరిపోలిక);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            ఆపు;
        స్థితి STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->విలువ);
            ఆపు;
        స్థితి STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            ఆపు;
        స్థితి STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->కీలకాలు);
            ఆపు;
        స్థితి STMT_సమూహం_COMPAT:
            FreeStmt((ParseCommon *) ((సమూహం_Compat_Def *) stmt)->def);
            ఆపు;
        స్థితి STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            ఆపు;
        స్థితి STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            ఆపు;
        స్థితి STMT_UNKNOWN_DECLARATION:
        స్థితి STMT_UNKNOWN_COMPOUND:
            విడుదల(((UnknownStatement *) stmt)->name);
            ఆపు;
        అప్రమేయ:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_అంకె_విలువలు == 37 &&
                          STMT_అంకె_విలువలు == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            ఆపు;
        }

        విడుదల(stmt);
        stmt = తదుపరి;
    }
}

శూన్య
FreeXkbFile(XkbFile *file)
{
    XkbFile *తదుపరి;

    అయితే (file)
    {
        తదుపరి = (XkbFile *) file->common.తదుపరి;

        ఎంపిక (file->file_type) {
        స్థితి FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            ఆపు;

        స్థితి FILE_TYPE_TYPES:
        స్థితి FILE_TYPE_COMPAT:
        స్థితి FILE_TYPE_SYMBOLS:
        స్థితి FILE_TYPE_KEYCODES:
        స్థితి FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            ఆపు;

        అప్రమేయ:
            ఆపు;
        }

        విడుదల(file->name);
        విడుదల(file);
        file = తదుపరి;
    }
}

స్థిర స్థిరరాశి అక్షరం *xkb_file_type_అక్షరశ్రేణులు[FILE_TYPE_అంకె_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_నియమాలు] = "rules",
};

స్థిరరాశి అక్షరం *
xkb_file_type_గమ్యం_string(గణన xkb_file_type type)
{
    ఒకవేళ (type >= FILE_TYPE_అంకె_ENTRIES)
        తిరిగి "unknown";
    తిరిగి xkb_file_type_అక్షరశ్రేణులు[type];
}

స్థిర స్థిరరాశి అక్షరం *stmt_type_అక్షరశ్రేణులు[STMT_అంకె_విలువలు] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_సమాసం_STRING_LITERAL] = "string literal expression",
    [STMT_సమాసం_INTEGER_LITERAL] = "integer literal expression",
    [STMT_సమాసం_FLOAT_LITERAL] = "float literal expression",
    [STMT_సమాసం_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_సమాసం_KEYNAME_LITERAL] = "key name expression",
    [STMT_సమాసం_KEYSYM_LITERAL] = "keysym expression",
    [STMT_సమాసం_IDENT] = "identifier expression",
    [STMT_సమాసం_ACTION_DECL] = "action declaration expression",
    [STMT_సమాసం_క్షేత్రం_REF] = "field reference expression",
    [STMT_సమాసం_ARRAY_REF] = "array reference expression",
    [STMT_సమాసం_ఖాళీలు_LIST] = "empty list expression",
    [STMT_సమాసం_KEYSYM_LIST] = "keysym list expression",
    [STMT_సమాసం_ACTION_LIST] = "action list expression",
    [STMT_సమాసం_కూడు] = "addition expression",
    [STMT_సమాసం_వ్యవకలనం] = "substraction expression",
    [STMT_సమాసం_MULTIPLY] = "multiplication expression",
    [STMT_సమాసం_DIVIDE] = "division expression",
    [STMT_సమాసం_కేటాయించు] = "assignment expression",
    [STMT_సమాసం_NOT] = "logical negation expression",
    [STMT_సమాసం_NEGATE] = "arithmetic negation expression",
    [STMT_సమాసం_INVERT] = "bitwise inversion expression",
    [STMT_సమాసం_UNARY_PLUS] = "unary plus expression",
    [STMT_VAR] = "variable definition",
    [STMT_TYPE] = "key type definition",
    [STMT_INTERP] = "symbol interpretation definition",
    [STMT_VMOD] = "virtual modifiers definition",
    [STMT_SYMBOLS] = "key symbols definition",
    [STMT_MODMAP] = "modifier map declaration",
    [STMT_సమూహం_COMPAT] = "group declaration",
    [STMT_LED_MAP] = "indicator map declaration",
    [STMT_LED_NAME] = "indicator name declaration",
    [STMT_UNKNOWN_DECLARATION] = "unknown declaration statement",
    [STMT_UNKNOWN_COMPOUND] = "unknown compound statement",
};
static_assert(STMT_అంకె_విలువలు == 37 &&
              STMT_అంకె_విలువలు == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

స్థిరరాశి అక్షరం *
stmt_type_గమ్యం_string(గణన stmt_type type)
{
    ఒకవేళ (type >= STMT_అంకె_విలువలు)
        తిరిగి ఖాళీ;
    తిరిగి stmt_type_అక్షరశ్రేణులు[type];
}

అక్షరం
stmt_type_గమ్యం_operator_char(గణన stmt_type type)
{
    ఎంపిక (type) {
    స్థితి STMT_సమాసం_కూడు:
        తిరిగి '+';
    స్థితి STMT_సమాసం_వ్యవకలనం:
        తిరిగి '-';
    స్థితి STMT_సమాసం_MULTIPLY:
        తిరిగి '*';
    స్థితి STMT_సమాసం_DIVIDE:
        తిరిగి '/';
    స్థితి STMT_సమాసం_NOT:
        తిరిగి '!';
    స్థితి STMT_సమాసం_NEGATE:
        తిరిగి '-';
    స్థితి STMT_సమాసం_INVERT:
        తిరిగి '~';
    స్థితి STMT_సమాసం_UNARY_PLUS:
        తిరిగి '+';
    అప్రమేయ:
        తిరిగి '\0';
    }
}
