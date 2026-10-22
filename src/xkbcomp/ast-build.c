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

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "ast-build.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "utf8-decoding.h"

स्थिर अभिव्यक्ति_Def *
अभिव्यक्ति_Create(गणना stmt_type op)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = दो_स्मृति(परिमाण(*अभिव्यक्ति));
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;

    अभिव्यक्ति->common.type = op;
    अभिव्यक्ति->common.next = रिक्त;

    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_String(xkb_atom_t str)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_STRING_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->string.str = str;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Integer(int64_t ival)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_INTEGER_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->integer.ival = ival;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Float(शून्य)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_FLOAT_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Boolean(bool set)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_BOOLEAN_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->boolean.set = set;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Name(xkb_atom_t साँचो_name)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_KEYNAME_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->साँचो_name.साँचो_name = साँचो_name;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Sym(xkb_keysym_t keysym)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_KEYSYM_LITERAL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->keysym.keysym = keysym;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Ident(xkb_atom_t ident)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_IDENT);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->ident.ident = ident;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Unary(गणना stmt_type op, अभिव्यक्ति_Def *child)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(op);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->unary.child = child;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_द्विआधारी(गणना stmt_type op, अभिव्यक्ति_Def *बायाँ, अभिव्यक्ति_Def *दायाँ)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(op);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;

    अभिव्यक्ति->द्विआधारी.बायाँ = बायाँ;
    अभिव्यक्ति->द्विआधारी.दायाँ = दायाँ;

    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Field_Ref(xkb_atom_t element, xkb_atom_t field)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_FIELD_REF);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->field_ref.element = element;
    अभिव्यक्ति->field_ref.field = field;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Array_Ref(xkb_atom_t element, xkb_atom_t field, अभिव्यक्ति_Def *entry)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_ARRAY_REF);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->array_ref.element = element;
    अभिव्यक्ति->array_ref.field = field;
    अभिव्यक्ति->array_ref.entry = entry;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Empty_List(शून्य)
{
    फर्काऊ अभिव्यक्ति_Create(STMT_अभिव्यक्ति_EMPTY_LIST);
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Action(xkb_atom_t name, अभिव्यक्ति_Def *args)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_ACTION_DECL);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->action.name = name;
    अभिव्यक्ति->action.args = args;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_Action_List(अभिव्यक्ति_Def *actions)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_ACTION_LIST);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    अभिव्यक्ति->actions.actions = actions;
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Create_साँचो_Sym_List(xkb_keysym_t sym)
{
    अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create(STMT_अभिव्यक्ति_KEYSYM_LIST);
    यदि (!अभिव्यक्ति)
        फर्काऊ रिक्त;
    darray_init(अभिव्यक्ति->keysym_list.syms);
    यदि (sym == XKB_साँचो_No_चिह्न) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(अभिव्यक्ति->keysym_list.syms, sym);
    }
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_Append_साँचो_Sym_List(अभिव्यक्ति_Def *अभिव्यक्ति, xkb_keysym_t sym)
{
    यदि (sym == XKB_साँचो_No_चिह्न) {
        /* Discard NoSymbol */
    } अन्यथा {
        darray_append(अभिव्यक्ति->keysym_list.syms, sym);
    }
    फर्काऊ अभिव्यक्ति;
}

अभिव्यक्ति_Def *
अभिव्यक्ति_साँचो_Sym_List_Append_String(संरचना scanner *scanner,
                           अभिव्यक्ति_Def *अभिव्यक्ति, अचर वर्ण *string)
{
    /* TODO: use strnlen with max len = 4 * MAX_KEYSYMS_LIST_LENGTH */
    अचर माप_प्रकार len = म_माप(string);
    माप_प्रकार idx = 0;
    माप_प्रकार idx_cp = 1;
    जबसम्म (idx < len) {
        माप_प्रकार गन्ती = 0;
        uint32_t cp = utf8_next_code_point(string + idx, len - idx, &गन्ती);
        यदि (cp == INVALID_UTF8_CODE_POINT) {
            scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                        "Cannot convert string to keysyms: "
                        "Invalid UTF-8 encoding starting at byte position %zu "
                        "(code point position: %zu).",
                        idx + 1, idx_cp);
            गच्छ error;
        }
        अचर xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
        यदि (sym == XKB_साँचो_No_चिह्न) {
            scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                        "Cannot convert string to keysyms: Unicode code point "
                        "U+04%"PRIX32" has no keysym equivalent"
                        "(byte position: %zu, code point position: %zu).",
                        cp, idx + 1, idx_cp);
            गच्छ error;
        }
        darray_append(अभिव्यक्ति->keysym_list.syms, sym);
        idx += गन्ती;
        idx_cp++;
    }
    प्रतिज्ञा(string[idx] == '\0');
    फर्काऊ अभिव्यक्ति;
error:
    FreeStmt((ParseCommon*) अभिव्यक्ति);
    फर्काऊ रिक्त;
}

xkb_keysym_t
KeysymParseString(संरचना scanner *scanner, अचर वर्ण *string)
{
    अचर माप_प्रकार len = म_माप(string);
    यदि (len == 0) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: empty string.");
        फर्काऊ XKB_साँचो_No_चिह्न;
    }
    माप_प्रकार गन्ती = 0;
    अचर uint32_t cp = utf8_next_code_point(string, len, &गन्ती);
    यदि (cp == INVALID_UTF8_CODE_POINT) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Invalid UTF-8 encoding.");
        फर्काऊ XKB_साँचो_No_चिह्न;
    } अन्यथा यदि (गन्ती != len) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "Cannot convert string to single keysym: "
                    "Expected a single Unicode code point, got: \"%s\".",
                    string);
        फर्काऊ XKB_साँचो_No_चिह्न;
    }
    अचर xkb_keysym_t sym = xkb_utf32_to_keysym(cp);
    यदि (sym == XKB_साँचो_No_चिह्न) {
        scanner_err(scanner, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot convert string to single keysym: Unicode "
                    "code point U+%04"PRIX32" has no keysym equivalent.",
                    cp);
    }
    फर्काऊ sym;
}

KeycodeDef *
KeycodeCreate(xkb_atom_t name, int64_t मान)
{
    KeycodeDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_KEYCODE;
    def->common.next = रिक्त;
    def->name = name;
    def->मान = मान;

    फर्काऊ def;
}

साँचो_Alias_Def *
साँचो_Alias_Create(xkb_atom_t alias, xkb_atom_t real)
{
    साँचो_Alias_Def *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_ALIAS;
    def->common.next = रिक्त;
    def->alias = alias;
    def->real = real;

    फर्काऊ def;
}

VModDef *
VModCreate(xkb_atom_t name, अभिव्यक्ति_Def *मान)
{
    VModDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_VMOD;
    def->common.next = रिक्त;
    def->name = name;
    def->मान = मान;

    फर्काऊ def;
}

VarDef *
VarCreate(अभिव्यक्ति_Def *name, अभिव्यक्ति_Def *मान)
{
    VarDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_VAR;
    def->common.next = रिक्त;
    def->name = name;
    def->मान = मान;

    फर्काऊ def;
}

VarDef *
BoolVarCreate(xkb_atom_t ident, bool set)
{
    अभिव्यक्ति_Def *name, *मान;
    VarDef *def;
    यदि (!(name = अभिव्यक्ति_Create_Ident(ident))) {
        फर्काऊ रिक्त;
    }
    यदि (!(मान = अभिव्यक्ति_Create_Boolean(set))) {
        FreeStmt((ParseCommon *) name);
        फर्काऊ रिक्त;
    }
    यदि (!(def = VarCreate(name, मान))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) मान);
        फर्काऊ रिक्त;
    }
    फर्काऊ def;
}

InterpDef *
InterpCreate(xkb_keysym_t sym, अभिव्यक्ति_Def *match)
{
    InterpDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_INTERP;
    def->common.next = रिक्त;
    def->sym = sym;
    def->match = match;
    def->def = रिक्त;

    फर्काऊ def;
}

साँचो_Type_Def *
साँचो_Type_Create(xkb_atom_t name, VarDef *body)
{
    साँचो_Type_Def *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_TYPE;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    फर्काऊ def;
}

SymbolsDef *
SymbolsCreate(xkb_atom_t साँचो_Name, VarDef *symbols)
{
    SymbolsDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_SYMBOLS;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->साँचो_Name = साँचो_Name;
    def->symbols = symbols;

    फर्काऊ def;
}

GroupCompatDef *
GroupCompatCreate(int64_t group, अभिव्यक्ति_Def *val)
{
    GroupCompatDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    फर्काऊ def;
}

ModMapDef *
ModMapCreate(अभिव्यक्ति_Def *modifiers, अभिव्यक्ति_Def *साँचोहरू)
{
    ModMapDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_MODMAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->modifiers = modifiers;
    def->साँचोहरू = साँचोहरू;

    फर्काऊ def;
}

LedMapDef *
LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_LED_MAP;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    फर्काऊ def;
}

LedNameDef *
LedNameCreate(int64_t ndx, अभिव्यक्ति_Def *name, bool आभास)
{
    LedNameDef *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = STMT_LED_NAME;
    def->common.next = रिक्त;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->आभास = आभास;

    फर्काऊ def;
}

UnknownStatement *
UnknownStatementCreate(गणना stmt_type type, संरचना sval name)
{
    UnknownStatement *def = दो_स्मृति(परिमाण(*def));
    यदि (!def)
        फर्काऊ रिक्त;

    def->common.type = type;
    def->common.next = रिक्त;
    def->name = strndup(name.start, name.len);
    यदि (!def->name) {
        मुक्त(def);
        फर्काऊ रिक्त;
    }

    फर्काऊ def;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl);

IncludeStmt *
IncludeCreate(संरचना xkb_context *ctx, वर्ण *str, गणना merge_mode merge)
{
    IncludeStmt *incl, *first;
    वर्ण *stmt, *tmp;
    वर्ण nextop;

    incl = first = रिक्त;
    tmp = str;
    stmt = strdup_safe(str);
    जबसम्म (tmp && *tmp)
    {
        वर्ण *file = रिक्त, *map = रिक्त, *extra_data = रिक्त;

        यदि (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            गच्छ err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        यदि (isempty(file)) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_data);
            जारी;
        }

        यदि (first == रिक्त) {
            first = incl = दो_स्मृति(परिमाण(*first));
        } अन्यथा {
            incl->next_incl = दो_स्मृति(परिमाण(*first));
            incl = incl->next_incl;
        }

        यदि (!incl) {
            मुक्त(file);
            मुक्त(map);
            मुक्त(extra_data);
            रोक;
        }

        incl->common.type = STMT_INCLUDE;
        incl->common.next = रिक्त;
        incl->merge = merge;
        incl->stmt = रिक्त;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = रिक्त;

        विकल्प (nextop) {
        पक्ष MERGE_AUGMENT_PREFIX:
            merge = MERGE_AUGMENT;
            रोक;
        पक्ष MERGE_REPLACE_PREFIX:
            merge = MERGE_REPLACE;
            रोक;
        स्वभाव:
            merge = MERGE_OVERRIDE;
        }
    }

    यदि (first)
        first->stmt = stmt;
    अन्यथा
        मुक्त(stmt);

    फर्काऊ first;

err:
    log_err(ctx, XKB_ERROR_INVALID_INCLUDE_STATEMENT,
            "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    मुक्त(stmt);
    फर्काऊ रिक्त;
}

XkbFile *
XkbFileCreate(गणना xkb_file_type type, वर्ण *name, ParseCommon *defs,
              गणना xkb_map_flags flags)
{
    XkbFile *file;

    file = सुस्मृति(1, परिमाण(*file));
    यदि (!file)
        फर्काऊ रिक्त;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name;
    file->defs = defs;
    file->flags = flags;

    फर्काऊ file;
}

XkbFile *
XkbFileFromComponents(संरचना xkb_context *ctx,
                      अचर संरचना xkb_component_names *kkctgs)
{
    वर्ण *अचर components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compatibility, kkctgs->symbols,
    };
    गणना xkb_file_type type;
    IncludeStmt *include = रिक्त;
    XkbFile *file = रिक्त;
    ParseCommon *defs = रिक्त, *defsLast = रिक्त;

    लागि (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        यदि (!include)
            गच्छ err;

        file = XkbFileCreate(type, रिक्त, (ParseCommon *) include, 0);
        यदि (!file) {
            FreeInclude(include);
            गच्छ err;
        }

        यदि (!defs)
            defsLast = defs = &file->common;
        अन्यथा
            defsLast = defsLast->next = &file->common;
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, रिक्त, defs, 0);
    यदि (!file)
        गच्छ err;

    फर्काऊ file;

err:
    FreeXkbFile((XkbFile *) defs);
    फर्काऊ रिक्त;
}

स्थिर शून्य
FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    जबसम्म (incl)
    {
        next = incl->next_incl;

        मुक्त(incl->file);
        मुक्त(incl->map);
        मुक्त(incl->modifier);
        मुक्त(incl->stmt);

        मुक्त(incl);
        incl = next;
    }
}

शून्य
FreeStmt(ParseCommon *stmt)
{
    ParseCommon *next;

    जबसम्म (stmt)
    {
        next = stmt->next;

        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            FreeInclude((IncludeStmt *) stmt);
            /* stmt is already free'd here. */
            stmt = रिक्त;
            रोक;

        पक्ष STMT_अभिव्यक्ति_NEGATE:
        पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
        पक्ष STMT_अभिव्यक्ति_NOT:
        पक्ष STMT_अभिव्यक्ति_INVERT:
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_Unary *) stmt)->child);
            रोक;

        पक्ष STMT_अभिव्यक्ति_DIVIDE:
        पक्ष STMT_अभिव्यक्ति_ADD:
        पक्ष STMT_अभिव्यक्ति_SUBTRACT:
        पक्ष STMT_अभिव्यक्ति_MULTIPLY:
        पक्ष STMT_अभिव्यक्ति_ASSIGN:
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_द्विआधारी *) stmt)->बायाँ);
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_द्विआधारी *) stmt)->दायाँ);
            रोक;

        पक्ष STMT_अभिव्यक्ति_ACTION_DECL:
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_Action *) stmt)->args);
            रोक;

        पक्ष STMT_अभिव्यक्ति_ACTION_LIST:
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_Action_List *) stmt)->actions);
            रोक;

        पक्ष STMT_अभिव्यक्ति_ARRAY_REF:
            FreeStmt((ParseCommon *) ((अभिव्यक्ति_Array_Ref *) stmt)->entry);
            रोक;

        पक्ष STMT_अभिव्यक्ति_KEYSYM_LIST:
            darray_free(((अभिव्यक्ति_Keysym_List *) stmt)->syms);
            रोक;

        पक्ष STMT_VAR:
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->name);
            FreeStmt((ParseCommon *) ((VarDef *) stmt)->मान);
            रोक;
        पक्ष STMT_TYPE:
            FreeStmt((ParseCommon *) ((साँचो_Type_Def *) stmt)->body);
            रोक;
        पक्ष STMT_INTERP:
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->match);
            FreeStmt((ParseCommon *) ((InterpDef *) stmt)->def);
            रोक;
        पक्ष STMT_VMOD:
            FreeStmt((ParseCommon *) ((VModDef *) stmt)->मान);
            रोक;
        पक्ष STMT_SYMBOLS:
            FreeStmt((ParseCommon *) ((SymbolsDef *) stmt)->symbols);
            रोक;
        पक्ष STMT_MODMAP:
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->modifiers);
            FreeStmt((ParseCommon *) ((ModMapDef *) stmt)->साँचोहरू);
            रोक;
        पक्ष STMT_GROUP_COMPAT:
            FreeStmt((ParseCommon *) ((GroupCompatDef *) stmt)->def);
            रोक;
        पक्ष STMT_LED_MAP:
            FreeStmt((ParseCommon *) ((LedMapDef *) stmt)->body);
            रोक;
        पक्ष STMT_LED_NAME:
            FreeStmt((ParseCommon *) ((LedNameDef *) stmt)->name);
            रोक;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            मुक्त(((UnknownStatement *) stmt)->name);
            रोक;
        स्वभाव:
            {} /* Label followed by declaration requires C23 */
            static_assert(STMT_अङ्क_मानहरू == 37 &&
                          STMT_अङ्क_मानहरू == STMT_UNKNOWN_COMPOUND + 1,
                          "Missing statement type");
            रोक;
        }

        मुक्त(stmt);
        stmt = next;
    }
}

शून्य
FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    जबसम्म (file)
    {
        next = (XkbFile *) file->common.next;

        विकल्प (file->file_type) {
        पक्ष FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            रोक;

        पक्ष FILE_TYPE_TYPES:
        पक्ष FILE_TYPE_COMPAT:
        पक्ष FILE_TYPE_SYMBOLS:
        पक्ष FILE_TYPE_KEYCODES:
        पक्ष FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            रोक;

        स्वभाव:
            रोक;
        }

        मुक्त(file->name);
        मुक्त(file);
        file = next;
    }
}

स्थिर अचर वर्ण *xkb_file_type_strings[FILE_TYPE_अङ्क_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "xkb_keycodes",
    [FILE_TYPE_TYPES] = "xkb_types",
    [FILE_TYPE_COMPAT] = "xkb_compatibility",
    [FILE_TYPE_SYMBOLS] = "xkb_symbols",
    [FILE_TYPE_GEOMETRY] = "xkb_geometry",
    [FILE_TYPE_KEYMAP] = "xkb_keymap",
    [FILE_TYPE_RULES] = "rules",
};

अचर वर्ण *
xkb_file_type_to_string(गणना xkb_file_type type)
{
    यदि (type >= FILE_TYPE_अङ्क_ENTRIES)
        फर्काऊ "unknown";
    फर्काऊ xkb_file_type_strings[type];
}

स्थिर अचर वर्ण *stmt_type_strings[STMT_अङ्क_मानहरू] = {
    [STMT_UNKNOWN] = "unknown statement",
    [STMT_INCLUDE] = "include statement",
    [STMT_KEYCODE] = "key name definition",
    [STMT_ALIAS] = "key alias definition",
    [STMT_अभिव्यक्ति_STRING_LITERAL] = "string literal expression",
    [STMT_अभिव्यक्ति_INTEGER_LITERAL] = "integer literal expression",
    [STMT_अभिव्यक्ति_FLOAT_LITERAL] = "float literal expression",
    [STMT_अभिव्यक्ति_BOOLEAN_LITERAL] = "boolean literal expression",
    [STMT_अभिव्यक्ति_KEYNAME_LITERAL] = "key name expression",
    [STMT_अभिव्यक्ति_KEYSYM_LITERAL] = "keysym expression",
    [STMT_अभिव्यक्ति_IDENT] = "identifier expression",
    [STMT_अभिव्यक्ति_ACTION_DECL] = "action declaration expression",
    [STMT_अभिव्यक्ति_FIELD_REF] = "field reference expression",
    [STMT_अभिव्यक्ति_ARRAY_REF] = "array reference expression",
    [STMT_अभिव्यक्ति_EMPTY_LIST] = "empty list expression",
    [STMT_अभिव्यक्ति_KEYSYM_LIST] = "keysym list expression",
    [STMT_अभिव्यक्ति_ACTION_LIST] = "action list expression",
    [STMT_अभिव्यक्ति_ADD] = "addition expression",
    [STMT_अभिव्यक्ति_SUBTRACT] = "substraction expression",
    [STMT_अभिव्यक्ति_MULTIPLY] = "multiplication expression",
    [STMT_अभिव्यक्ति_DIVIDE] = "division expression",
    [STMT_अभिव्यक्ति_ASSIGN] = "assignment expression",
    [STMT_अभिव्यक्ति_NOT] = "logical negation expression",
    [STMT_अभिव्यक्ति_NEGATE] = "arithmetic negation expression",
    [STMT_अभिव्यक्ति_INVERT] = "bitwise inversion expression",
    [STMT_अभिव्यक्ति_UNARY_PLUS] = "unary plus expression",
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
static_assert(STMT_अङ्क_मानहरू == 37 &&
              STMT_अङ्क_मानहरू == STMT_UNKNOWN_COMPOUND + 1,
              "Missing statement type");

अचर वर्ण *
stmt_type_to_string(गणना stmt_type type)
{
    यदि (type >= STMT_अङ्क_मानहरू)
        फर्काऊ रिक्त;
    फर्काऊ stmt_type_strings[type];
}

वर्ण
stmt_type_to_operator_char(गणना stmt_type type)
{
    विकल्प (type) {
    पक्ष STMT_अभिव्यक्ति_ADD:
        फर्काऊ '+';
    पक्ष STMT_अभिव्यक्ति_SUBTRACT:
        फर्काऊ '-';
    पक्ष STMT_अभिव्यक्ति_MULTIPLY:
        फर्काऊ '*';
    पक्ष STMT_अभिव्यक्ति_DIVIDE:
        फर्काऊ '/';
    पक्ष STMT_अभिव्यक्ति_NOT:
        फर्काऊ '!';
    पक्ष STMT_अभिव्यक्ति_NEGATE:
        फर्काऊ '-';
    पक्ष STMT_अभिव्यक्ति_INVERT:
        फर्काऊ '~';
    पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
        फर्काऊ '+';
    स्वभाव:
        फर्काऊ '\0';
    }
}
