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

    FILE_TYPE_اڠک_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_اڠک_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_اوڠکاڤن_STRING_LITERAL,
    STMT_اوڠکاڤن_INTEGER_LITERAL,
    STMT_اوڠکاڤن_FLOAT_LITERAL,
    STMT_اوڠکاڤن_BOOLEAN_LITERAL,
    STMT_اوڠکاڤن_KEYNAME_LITERAL,
    STMT_اوڠکاڤن_KEYSYM_LITERAL,
    STMT_اوڠکاڤن_IDENT,
    STMT_اوڠکاڤن_ACTION_DECL,
    STMT_اوڠکاڤن_FIELD_REF,
    STMT_اوڠکاڤن_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_اوڠکاڤن_EMPTY_LIST,
    STMT_اوڠکاڤن_KEYSYM_LIST,
    STMT_اوڠکاڤن_ACTION_LIST,
    STMT_اوڠکاڤن_ADD,
    STMT_اوڠکاڤن_SUBTRACT,
    STMT_اوڠکاڤن_MULTIPLY,
    STMT_اوڠکاڤن_DIVIDE,
    STMT_اوڠکاڤن_ASSIGN,
    STMT_اوڠکاڤن_NOT,
    STMT_اوڠکاڤن_NEGATE,
    STMT_اوڠکاڤن_INVERT,
    STMT_اوڠکاڤن_UNARY_PLUS,
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

    STMT_اڠک_نيلاي_سموا
};

تعداد merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_اڠک_ENTRIES,
};
تعداد { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_اڠک_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
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

تعريفنوع اتحاد اوڠکاڤن_Def اوڠکاڤن_Def;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} اوڠکاڤن_Ident;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} اوڠکاڤن_String;

تعريفنوع بنية {
    ParseCommon common;
    bool set;
} اوڠکاڤن_Boolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} اوڠکاڤن_Integer;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} اوڠکاڤن_Float;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t کونچي_name;
} اوڠکاڤن_کونچي_Name;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} اوڠکاڤن_کونچي_Sym;

تعريفنوع بنية {
    ParseCommon common;
    اوڠکاڤن_Def *کيري;
    اوڠکاڤن_Def *کانن;
} اوڠکاڤن_بيناري;

تعريفنوع بنية {
    ParseCommon common;
    اوڠکاڤن_Def *child;
} اوڠکاڤن_Unary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} اوڠکاڤن_Field_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    اوڠکاڤن_Def *entry;
} اوڠکاڤن_Array_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    اوڠکاڤن_Def *args;
} اوڠکاڤن_Action;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    اوڠکاڤن_Def *actions;
} اوڠکاڤن_Action_List;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} اوڠکاڤن_Keysym_List;

اتحاد اوڠکاڤن_Def {
    ParseCommon common;
    اوڠکاڤن_Ident ident;
    اوڠکاڤن_String string;
    اوڠکاڤن_Boolean boolean;
    اوڠکاڤن_Integer integer;
    اوڠکاڤن_کونچي_Name کونچي_name;
    اوڠکاڤن_کونچي_Sym keysym;
    اوڠکاڤن_بيناري بيناري;
    اوڠکاڤن_Unary unary;
    اوڠکاڤن_Field_Ref field_ref;
    اوڠکاڤن_Array_Ref array_ref;
    اوڠکاڤن_Action action;
    اوڠکاڤن_Action_List actions;
    اوڠکاڤن_Keysym_List keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    اوڠکاڤن_Def *name;
    اوڠکاڤن_Def *نيلاي;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    اوڠکاڤن_Def *نيلاي;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    int64_t نيلاي;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} کونچي_Alias_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} کونچي_Type_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t کونچي_Name;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    اوڠکاڤن_Def *modifiers;
    اوڠکاڤن_Def *کونچي_سموا;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    int64_t group;
    اوڠکاڤن_Def *def;
} GroupCompatDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_keysym_t sym;
    اوڠکاڤن_Def *match;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    bool virtual;
    int64_t ndx;
    اوڠکاڤن_Def *name;
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
