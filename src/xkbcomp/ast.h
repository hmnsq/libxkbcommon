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

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "darray.h"

গণনা xkb_file_type {
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

    FILE_TYPE_অঙ্ক_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_অঙ্ক_ENTRIES
};

গণনা stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_ৰাশিমালা_STRING_LITERAL,
    STMT_ৰাশিমালা_INTEGER_LITERAL,
    STMT_ৰাশিমালা_FLOAT_LITERAL,
    STMT_ৰাশিমালা_BOOLEAN_LITERAL,
    STMT_ৰাশিমালা_KEYNAME_LITERAL,
    STMT_ৰাশিমালা_KEYSYM_LITERAL,
    STMT_ৰাশিমালা_IDENT,
    STMT_ৰাশিমালা_ACTION_DECL,
    STMT_ৰাশিমালা_FIELD_REF,
    STMT_ৰাশিমালা_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_ৰাশিমালা_EMPTY_LIST,
    STMT_ৰাশিমালা_KEYSYM_LIST,
    STMT_ৰাশিমালা_ACTION_LIST,
    STMT_ৰাশিমালা_ADD,
    STMT_ৰাশিমালা_SUBTRACT,
    STMT_ৰাশিমালা_MULTIPLY,
    STMT_ৰাশিমালা_DIVIDE,
    STMT_ৰাশিমালা_ASSIGN,
    STMT_ৰাশিমালা_NOT,
    STMT_ৰাশিমালা_NEGATE,
    STMT_ৰাশিমালা_INVERT,
    STMT_ৰাশিমালা_UNARY_PLUS,
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

    STMT_অঙ্ক_মানসমূহ
};

গণনা merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_অঙ্ক_ENTRIES,
};
গণনা { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_অঙ্ক_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

ধ্রুৱ ৱর্ণ *
xkb_file_type_to_string(গণনা xkb_file_type type);

ধ্রুৱ ৱর্ণ *
stmt_type_to_string(গণনা stmt_type type);

ৱর্ণ
stmt_type_to_operator_char(গণনা stmt_type type);

প্রকারনির্দেশ সংরচনা _ParseCommon  {
    সংরচনা _ParseCommon *next;
    গণনা stmt_type type;
} ParseCommon;

প্রকারনির্দেশ সংরচনা _IncludeStmt {
    ParseCommon common;
    গণনা merge_mode merge;
    ৱর্ণ *stmt;
    ৱর্ণ *file;
    ৱর্ণ *map;
    ৱর্ণ *modifier;
    সংরচনা _IncludeStmt *next_incl;
} IncludeStmt;

প্রকারনির্দেশ সঙ্ঘ ৰাশিমালা_Def ৰাশিমালা_Def;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t ident;
} ৰাশিমালা_Ident;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t str;
} ৰাশিমালা_String;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    bool set;
} ৰাশিমালা_Boolean;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    int64_t ival;
} ৰাশিমালা_Integer;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} ৰাশিমালা_Float;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t চাবি_name;
} ৰাশিমালা_চাবি_Name;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} ৰাশিমালা_চাবি_Sym;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    ৰাশিমালা_Def *বাম;
    ৰাশিমালা_Def *ডান;
} ৰাশিমালা_দ্বিমিক;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    ৰাশিমালা_Def *child;
} ৰাশিমালা_Unary;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} ৰাশিমালা_Field_Ref;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    ৰাশিমালা_Def *entry;
} ৰাশিমালা_Array_Ref;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    xkb_atom_t name;
    ৰাশিমালা_Def *args;
} ৰাশিমালা_Action;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    /* List of actions for a single level. */
    ৰাশিমালা_Def *actions;
} ৰাশিমালা_Action_List;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} ৰাশিমালা_Keysym_List;

সঙ্ঘ ৰাশিমালা_Def {
    ParseCommon common;
    ৰাশিমালা_Ident ident;
    ৰাশিমালা_String string;
    ৰাশিমালা_Boolean boolean;
    ৰাশিমালা_Integer integer;
    ৰাশিমালা_চাবি_Name চাবি_name;
    ৰাশিমালা_চাবি_Sym keysym;
    ৰাশিমালা_দ্বিমিক দ্বিমিক;
    ৰাশিমালা_Unary unary;
    ৰাশিমালা_Field_Ref field_ref;
    ৰাশিমালা_Array_Ref array_ref;
    ৰাশিমালা_Action action;
    ৰাশিমালা_Action_List actions;
    ৰাশিমালা_Keysym_List keysym_list;
};

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    ৰাশিমালা_Def *name;
    ৰাশিমালা_Def *মান;
} VarDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t name;
    ৰাশিমালা_Def *মান;
} VModDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t name;
    int64_t মান;
} KeycodeDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} চাবি_Alias_Def;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} চাবি_Type_Def;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t চাবি_Name;
    VarDef *symbols;
} SymbolsDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    ৰাশিমালা_Def *modifiers;
    ৰাশিমালা_Def *চাবিগুলি;
} ModMapDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    int64_t group;
    ৰাশিমালা_Def *def;
} GroupCompatDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_keysym_t sym;
    ৰাশিমালা_Def *match;
    VarDef *def;
} InterpDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    bool আভাস;
    int64_t ndx;
    ৰাশিমালা_Def *name;
} LedNameDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    গণনা merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    ৱর্ণ *name;
} UnknownStatement;

গণনা xkb_map_flags {
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

প্রকারনির্দেশ সংরচনা {
    ParseCommon common;
    ৱর্ণ *name;
    ParseCommon *defs;
    গণনা xkb_file_type file_type;
    গণনা xkb_map_flags flags;
} XkbFile;
