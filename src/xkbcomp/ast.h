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

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "atom.h"
#ضمن "darray.h"

تعداد xkb_file_type {
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

    _FILE_TYPE_NUM_ENTRIES,
    FILE_TYPE_INVALID = _FILE_TYPE_NUM_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_EXPR_STRING_LITERAL,
    STMT_EXPR_INTEGER_LITERAL,
    STMT_EXPR_FLOAT_LITERAL,
    STMT_EXPR_BOOLEAN_LITERAL,
    STMT_EXPR_KEYNAME_LITERAL,
    STMT_EXPR_KEYSYM_LITERAL,
    STMT_EXPR_IDENT,
    STMT_EXPR_ACTION_DECL,
    STMT_EXPR_FIELD_REF,
    STMT_EXPR_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_EXPR_EMPTY_LIST,
    STMT_EXPR_KEYSYM_LIST,
    STMT_EXPR_ACTION_LIST,
    STMT_EXPR_ADD,
    STMT_EXPR_SUBTRACT,
    STMT_EXPR_MULTIPLY,
    STMT_EXPR_DIVIDE,
    STMT_EXPR_ASSIGN,
    STMT_EXPR_NOT,
    STMT_EXPR_NEGATE,
    STMT_EXPR_INVERT,
    STMT_EXPR_UNARY_PLUS,
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

    _STMT_NUM_VALUES
};

تعداد merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    _MERGE_MODE_NUM_ENTRIES,
};
تعداد { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(_MERGE_MODE_NUM_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

ثابت حرف *
xkb_file_type_to_string(تعداد xkb_file_type type);

ثابت حرف *
stmt_type_to_string(تعداد stmt_type type);

حرف
stmt_type_to_operator_char(تعداد stmt_type type);

تعريفنوع بنية _ParseCommon  {
    بنية _ParseCommon *next;
    تعداد stmt_type type;
} ParseCommon;

تعريفنوع بنية _IncludeStmt {
    ParseCommon common;
    تعداد merge_mode merge;
    حرف *stmt;
    حرف *file;
    حرف *map;
    حرف *modifier;
    بنية _IncludeStmt *next_incl;
} IncludeStmt;

تعريفنوع اتحاد ExprDef ExprDef;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} ExprIdent;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} ExprString;

تعريفنوع بنية {
    ParseCommon common;
    bool set;
} ExprBoolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} ExprInteger;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} ExprFloat;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t key_name;
} ExprKeyName;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} ExprKeySym;

تعريفنوع بنية {
    ParseCommon common;
    ExprDef *left;
    ExprDef *right;
} ExprBinary;

تعريفنوع بنية {
    ParseCommon common;
    ExprDef *child;
} ExprUnary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} ExprFieldRef;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    ExprDef *entry;
} ExprArrayRef;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    ExprDef *args;
} ExprAction;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    ExprDef *actions;
} ExprActionList;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} ExprKeysymList;

اتحاد ExprDef {
    ParseCommon common;
    ExprIdent ident;
    ExprString string;
    ExprBoolean boolean;
    ExprInteger integer;
    ExprKeyName key_name;
    ExprKeySym keysym;
    ExprBinary binary;
    ExprUnary unary;
    ExprFieldRef field_ref;
    ExprArrayRef array_ref;
    ExprAction action;
    ExprActionList actions;
    ExprKeysymList keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    ExprDef *name;
    ExprDef *value;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    ExprDef *value;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    int64_t value;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} KeyAliasDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} KeyTypeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t keyName;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    ExprDef *modifiers;
    ExprDef *keys;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    int64_t group;
    ExprDef *def;
} GroupCompatDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_keysym_t sym;
    ExprDef *match;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    bool virtual;
    int64_t ndx;
    ExprDef *name;
} LedNameDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

تعريفنوع بنية {
    ParseCommon common;
    حرف *name;
} UnknownStatement;

تعداد xkb_map_flags {
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

تعريفنوع بنية {
    ParseCommon common;
    حرف *name;
    ParseCommon *defs;
    تعداد xkb_file_type file_type;
    تعداد xkb_map_flags flags;
} XkbFile;
