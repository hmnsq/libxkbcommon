/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "darray.h"

എണ്ണൽ xkb_file_type {
    /* Component files, by order of compilation. */
    FILE_TYPE_KEYCODES = 0,
    FILE_TYPE_TYPES = 1,
    FILE_TYPE_COMPAT = 2,
    FILE_TYPE_SYMBOLS = 3,

    /* File types which must be found in a keymap file. */
    FIRST_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    LAST_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_RULES,

    FILE_TYPE_അക്കം_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_അക്കം_ENTRIES
};

എണ്ണൽ stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_വ്യഞ്ജകം_STRING_LITERAL,
    STMT_വ്യഞ്ജകം_INTEGER_LITERAL,
    STMT_വ്യഞ്ജകം_FLOAT_LITERAL,
    STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL,
    STMT_വ്യഞ്ജകം_KEYNAME_LITERAL,
    STMT_വ്യഞ്ജകം_KEYSYM_LITERAL,
    STMT_വ്യഞ്ജകം_IDENT,
    STMT_വ്യഞ്ജകം_ACTION_DECL,
    STMT_വ്യഞ്ജകം_FIELD_REF,
    STMT_വ്യഞ്ജകം_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_വ്യഞ്ജകം_EMPTY_LIST,
    STMT_വ്യഞ്ജകം_KEYSYM_LIST,
    STMT_വ്യഞ്ജകം_ACTION_LIST,
    STMT_വ്യഞ്ജകം_ADD,
    STMT_വ്യഞ്ജകം_SUBTRACT,
    STMT_വ്യഞ്ജകം_MULTIPLY,
    STMT_വ്യഞ്ജകം_DIVIDE,
    STMT_വ്യഞ്ജകം_ASSIGN,
    STMT_വ്യഞ്ജകം_NOT,
    STMT_വ്യഞ്ജകം_NEGATE,
    STMT_വ്യഞ്ജകം_INVERT,
    STMT_വ്യഞ്ജകം_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_GROUP_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_അക്കം_മൂല്യങ്ങൾ
};

എണ്ണൽ merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_അക്കം_ENTRIES,
};
എണ്ണൽ { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_അക്കം_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

സ്ഥിരം അക്ഷരം *
xkb_file_type_to_string(എണ്ണൽ xkb_file_type type);

സ്ഥിരം അക്ഷരം *
stmt_type_to_string(എണ്ണൽ stmt_type type);

അക്ഷരം
stmt_type_to_operator_char(എണ്ണൽ stmt_type type);

തരനിർവചനം ഘടന _ParseCommon  {
    ഘടന _ParseCommon *next;
    എണ്ണൽ stmt_type type;
} ParseCommon;

തരനിർവചനം ഘടന _IncludeStmt {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    അക്ഷരം *stmt;
    അക്ഷരം *file;
    അക്ഷരം *map;
    അക്ഷരം *modifier;
    ഘടന _IncludeStmt *next_incl;
} IncludeStmt;

തരനിർവചനം സംഘം വ്യഞ്ജകം_Def വ്യഞ്ജകം_Def;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t ident;
} വ്യഞ്ജകം_Ident;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t str;
} വ്യഞ്ജകം_String;

തരനിർവചനം ഘടന {
    ParseCommon common;
    bool set;
} വ്യഞ്ജകം_Boolean;

തരനിർവചനം ഘടന {
    ParseCommon common;
    int64_t ival;
} വ്യഞ്ജകം_Integer;

തരനിർവചനം ഘടന {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} വ്യഞ്ജകം_Float;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t താക്കോൽ_name;
} വ്യഞ്ജകം_താക്കോൽ_Name;

തരനിർവചനം ഘടന {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} വ്യഞ്ജകം_താക്കോൽ_Sym;

തരനിർവചനം ഘടന {
    ParseCommon common;
    വ്യഞ്ജകം_Def *ഇടത്;
    വ്യഞ്ജകം_Def *വലത്;
} വ്യഞ്ജകം_ദ്വിമാന;

തരനിർവചനം ഘടന {
    ParseCommon common;
    വ്യഞ്ജകം_Def *child;
} വ്യഞ്ജകം_Unary;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} വ്യഞ്ജകം_Field_Ref;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    വ്യഞ്ജകം_Def *entry;
} വ്യഞ്ജകം_Array_Ref;

തരനിർവചനം ഘടന {
    ParseCommon common;
    xkb_atom_t name;
    വ്യഞ്ജകം_Def *args;
} വ്യഞ്ജകം_Action;

തരനിർവചനം ഘടന {
    ParseCommon common;
    /* List of actions for a single level. */
    വ്യഞ്ജകം_Def *actions;
} വ്യഞ്ജകം_Action_List;

തരനിർവചനം ഘടന {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} വ്യഞ്ജകം_Keysym_List;

സംഘം വ്യഞ്ജകം_Def {
    ParseCommon common;
    വ്യഞ്ജകം_Ident ident;
    വ്യഞ്ജകം_String string;
    വ്യഞ്ജകം_Boolean boolean;
    വ്യഞ്ജകം_Integer integer;
    വ്യഞ്ജകം_താക്കോൽ_Name താക്കോൽ_name;
    വ്യഞ്ജകം_താക്കോൽ_Sym keysym;
    വ്യഞ്ജകം_ദ്വിമാന ദ്വിമാന;
    വ്യഞ്ജകം_Unary unary;
    വ്യഞ്ജകം_Field_Ref field_ref;
    വ്യഞ്ജകം_Array_Ref array_ref;
    വ്യഞ്ജകം_Action action;
    വ്യഞ്ജകം_Action_List actions;
    വ്യഞ്ജകം_Keysym_List keysym_list;
};

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    വ്യഞ്ജകം_Def *name;
    വ്യഞ്ജകം_Def *മൂല്യം;
} VarDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t name;
    വ്യഞ്ജകം_Def *മൂല്യം;
} VModDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t name;
    int64_t മൂല്യം;
} KeycodeDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} താക്കോൽ_Alias_Def;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} താക്കോൽ_Type_Def;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t താക്കോൽ_Name;
    VarDef *symbols;
} SymbolsDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    വ്യഞ്ജകം_Def *modifiers;
    വ്യഞ്ജകം_Def *താക്കോലുകൾ;
} ModMapDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    int64_t group;
    വ്യഞ്ജകം_Def *def;
} GroupCompatDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_keysym_t sym;
    വ്യഞ്ജകം_Def *match;
    VarDef *def;
} InterpDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    bool മായാ;
    int64_t ndx;
    വ്യഞ്ജകം_Def *name;
} LedNameDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    എണ്ണൽ merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

തരനിർവചനം ഘടന {
    ParseCommon common;
    അക്ഷരം *name;
} UnknownStatement;

എണ്ണൽ xkb_map_flags {
    MAP_NO_FLAG = 0,
    MAP_IS_DEFAULT = (1 << 0),
    MAP_IS_PARTIAL = (1 << 1),
    MAP_IS_HIDDEN = (1 << 2),
    MAP_HAS_ALPHANUMERIC = (1 << 3),
    MAP_HAS_MODIFIER = (1 << 4),
    MAP_HAS_KEYPAD = (1 << 5),
    MAP_HAS_FN = (1 << 6),
    MAP_IS_ALTGR = (1 << 7),
    MAP_IS_DEPRECATED = (1 << 8),
};

തരനിർവചനം ഘടന {
    ParseCommon common;
    അക്ഷരം *name;
    ParseCommon *defs;
    എണ്ണൽ xkb_file_type file_type;
    എണ്ണൽ xkb_map_flags flags;
} XkbFile;
