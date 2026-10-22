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

    FILE_TYPE_ژمارە_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_ژمارە_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_دەربڕین_STRING_LITERAL,
    STMT_دەربڕین_INTEGER_LITERAL,
    STMT_دەربڕین_FLOAT_LITERAL,
    STMT_دەربڕین_BOOLEAN_LITERAL,
    STMT_دەربڕین_KEYNAME_LITERAL,
    STMT_دەربڕین_KEYSYM_LITERAL,
    STMT_دەربڕین_IDENT,
    STMT_دەربڕین_ACTION_DECL,
    STMT_دەربڕین_FIELD_REF,
    STMT_دەربڕین_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_دەربڕین_EMPTY_LIST,
    STMT_دەربڕین_KEYSYM_LIST,
    STMT_دەربڕین_ACTION_LIST,
    STMT_دەربڕین_ADD,
    STMT_دەربڕین_SUBTRACT,
    STMT_دەربڕین_MULTIPLY,
    STMT_دەربڕین_DIVIDE,
    STMT_دەربڕین_ASSIGN,
    STMT_دەربڕین_NOT,
    STMT_دەربڕین_NEGATE,
    STMT_دەربڕین_INVERT,
    STMT_دەربڕین_UNARY_PLUS,
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

    STMT_ژمارە_بەهاکان
};

تعداد merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_ژمارە_ENTRIES,
};
تعداد { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_ژمارە_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
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

تعريفنوع اتحاد دەربڕین_Def دەربڕین_Def;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} دەربڕین_Ident;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} دەربڕین_String;

تعريفنوع بنية {
    ParseCommon common;
    bool set;
} دەربڕین_Boolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} دەربڕین_Integer;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} دەربڕین_Float;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t کلیل_name;
} دەربڕین_کلیل_Name;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} دەربڕین_کلیل_Sym;

تعريفنوع بنية {
    ParseCommon common;
    دەربڕین_Def *چەپ;
    دەربڕین_Def *ڕاست;
} دەربڕین_دووانەیی;

تعريفنوع بنية {
    ParseCommon common;
    دەربڕین_Def *child;
} دەربڕین_Unary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} دەربڕین_Field_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    دەربڕین_Def *entry;
} دەربڕین_Array_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    دەربڕین_Def *args;
} دەربڕین_Action;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    دەربڕین_Def *actions;
} دەربڕین_Action_List;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} دەربڕین_Keysym_List;

اتحاد دەربڕین_Def {
    ParseCommon common;
    دەربڕین_Ident ident;
    دەربڕین_String string;
    دەربڕین_Boolean boolean;
    دەربڕین_Integer integer;
    دەربڕین_کلیل_Name کلیل_name;
    دەربڕین_کلیل_Sym keysym;
    دەربڕین_دووانەیی دووانەیی;
    دەربڕین_Unary unary;
    دەربڕین_Field_Ref field_ref;
    دەربڕین_Array_Ref array_ref;
    دەربڕین_Action action;
    دەربڕین_Action_List actions;
    دەربڕین_Keysym_List keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    دەربڕین_Def *name;
    دەربڕین_Def *بەها;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    دەربڕین_Def *بەها;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    int64_t بەها;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} کلیل_Alias_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} کلیل_Type_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_atom_t کلیل_Name;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    دەربڕین_Def *modifiers;
    دەربڕین_Def *کلیلەکان;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    int64_t group;
    دەربڕین_Def *def;
} GroupCompatDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    xkb_keysym_t sym;
    دەربڕین_Def *match;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد merge_mode merge;
    bool virtual;
    int64_t ndx;
    دەربڕین_Def *name;
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
