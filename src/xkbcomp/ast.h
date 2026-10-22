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

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل "atom.h"
#شامل "darray.h"

شمارش xkb_file_type {
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

    FILE_TYPE_رقم_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_رقم_ENTRIES
};

شمارش stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_عبارت_STRING_LITERAL,
    STMT_عبارت_INTEGER_LITERAL,
    STMT_عبارت_FLOAT_LITERAL,
    STMT_عبارت_BOOLEAN_LITERAL,
    STMT_عبارت_KEYNAME_LITERAL,
    STMT_عبارت_KEYSYM_LITERAL,
    STMT_عبارت_IDENT,
    STMT_عبارت_ACTION_DECL,
    STMT_عبارت_FIELD_REF,
    STMT_عبارت_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_عبارت_EMPTY_LIST,
    STMT_عبارت_KEYSYM_LIST,
    STMT_عبارت_ACTION_LIST,
    STMT_عبارت_ADD,
    STMT_عبارت_SUBTRACT,
    STMT_عبارت_MULTIPLY,
    STMT_عبارت_DIVIDE,
    STMT_عبارت_ASSIGN,
    STMT_عبارت_NOT,
    STMT_عبارت_NEGATE,
    STMT_عبارت_INVERT,
    STMT_عبارت_UNARY_PLUS,
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

    STMT_رقم_مقادیر
};

شمارش merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_رقم_ENTRIES,
};
شمارش { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_رقم_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

ثابت نویسه *
xkb_file_type_to_string(شمارش xkb_file_type type);

ثابت نویسه *
stmt_type_to_string(شمارش stmt_type type);

نویسه
stmt_type_to_operator_char(شمارش stmt_type type);

تعریفنوع ساختار _ParseCommon  {
    ساختار _ParseCommon *next;
    شمارش stmt_type type;
} ParseCommon;

تعریفنوع ساختار _IncludeStmt {
    ParseCommon common;
    شمارش merge_mode merge;
    نویسه *stmt;
    نویسه *file;
    نویسه *map;
    نویسه *modifier;
    ساختار _IncludeStmt *next_incl;
} IncludeStmt;

تعریفنوع اجتماع عبارت_Def عبارت_Def;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t ident;
} عبارت_Ident;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t str;
} عبارت_String;

تعریفنوع ساختار {
    ParseCommon common;
    bool set;
} عبارت_Boolean;

تعریفنوع ساختار {
    ParseCommon common;
    int64_t ival;
} عبارت_Integer;

تعریفنوع ساختار {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} عبارت_Float;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t کلید_name;
} عبارت_کلید_Name;

تعریفنوع ساختار {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} عبارت_کلید_Sym;

تعریفنوع ساختار {
    ParseCommon common;
    عبارت_Def *چپ;
    عبارت_Def *راست;
} عبارت_دودویی;

تعریفنوع ساختار {
    ParseCommon common;
    عبارت_Def *child;
} عبارت_Unary;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} عبارت_Field_Ref;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    عبارت_Def *entry;
} عبارت_Array_Ref;

تعریفنوع ساختار {
    ParseCommon common;
    xkb_atom_t name;
    عبارت_Def *args;
} عبارت_Action;

تعریفنوع ساختار {
    ParseCommon common;
    /* List of actions for a single level. */
    عبارت_Def *actions;
} عبارت_Action_List;

تعریفنوع ساختار {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} عبارت_Keysym_List;

اجتماع عبارت_Def {
    ParseCommon common;
    عبارت_Ident ident;
    عبارت_String string;
    عبارت_Boolean boolean;
    عبارت_Integer integer;
    عبارت_کلید_Name کلید_name;
    عبارت_کلید_Sym keysym;
    عبارت_دودویی دودویی;
    عبارت_Unary unary;
    عبارت_Field_Ref field_ref;
    عبارت_Array_Ref array_ref;
    عبارت_Action action;
    عبارت_Action_List actions;
    عبارت_Keysym_List keysym_list;
};

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    عبارت_Def *name;
    عبارت_Def *مقدار;
} VarDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t name;
    عبارت_Def *مقدار;
} VModDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t name;
    int64_t مقدار;
} KeycodeDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} کلید_Alias_Def;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} کلید_Type_Def;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t کلید_Name;
    VarDef *symbols;
} SymbolsDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    عبارت_Def *modifiers;
    عبارت_Def *کلیدها;
} ModMapDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    int64_t group;
    عبارت_Def *def;
} GroupCompatDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_keysym_t sym;
    عبارت_Def *match;
    VarDef *def;
} InterpDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    bool مجازی;
    int64_t ndx;
    عبارت_Def *name;
} LedNameDef;

تعریفنوع ساختار {
    ParseCommon common;
    شمارش merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

تعریفنوع ساختار {
    ParseCommon common;
    نویسه *name;
} UnknownStatement;

شمارش xkb_map_flags {
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

تعریفنوع ساختار {
    ParseCommon common;
    نویسه *name;
    ParseCommon *defs;
    شمارش xkb_file_type file_type;
    شمارش xkb_map_flags flags;
} XkbFile;
