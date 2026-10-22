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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"

ଗଣନା xkb_file_type {
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

    FILE_TYPE_ଅଙ୍କ_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_ଅଙ୍କ_ENTRIES
};

ଗଣନା stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_ପଦସମଷ୍ଟି_STRING_LITERAL,
    STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL,
    STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL,
    STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL,
    STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL,
    STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL,
    STMT_ପଦସମଷ୍ଟି_IDENT,
    STMT_ପଦସମଷ୍ଟି_ACTION_DECL,
    STMT_ପଦସମଷ୍ଟି_FIELD_REF,
    STMT_ପଦସମଷ୍ଟି_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_ପଦସମଷ୍ଟି_EMPTY_LIST,
    STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST,
    STMT_ପଦସମଷ୍ଟି_ACTION_LIST,
    STMT_ପଦସମଷ୍ଟି_ADD,
    STMT_ପଦସମଷ୍ଟି_SUBTRACT,
    STMT_ପଦସମଷ୍ଟି_MULTIPLY,
    STMT_ପଦସମଷ୍ଟି_DIVIDE,
    STMT_ପଦସମଷ୍ଟି_ASSIGN,
    STMT_ପଦସମଷ୍ଟି_NOT,
    STMT_ପଦସମଷ୍ଟି_NEGATE,
    STMT_ପଦସମଷ୍ଟି_INVERT,
    STMT_ପଦସମଷ୍ଟି_UNARY_PLUS,
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

    STMT_ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ
};

ଗଣନା merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_ଅଙ୍କ_ENTRIES,
};
ଗଣନା { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_ଅଙ୍କ_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

ଅଚଳ ଅକ୍ଷର *
xkb_file_type_to_string(ଗଣନା xkb_file_type type);

ଅଚଳ ଅକ୍ଷର *
stmt_type_to_string(ଗଣନା stmt_type type);

ଅକ୍ଷର
stmt_type_to_operator_char(ଗଣନା stmt_type type);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା _ParseCommon  {
    ସଂରଚନା _ParseCommon *next;
    ଗଣନା stmt_type type;
} ParseCommon;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା _IncludeStmt {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    ଅକ୍ଷର *stmt;
    ଅକ୍ଷର *file;
    ଅକ୍ଷର *map;
    ଅକ୍ଷର *modifier;
    ସଂରଚନା _IncludeStmt *next_incl;
} IncludeStmt;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂଘ ପଦସମଷ୍ଟି_Def ପଦସମଷ୍ଟି_Def;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t ident;
} ପଦସମଷ୍ଟି_Ident;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t str;
} ପଦସମଷ୍ଟି_String;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    bool set;
} ପଦସମଷ୍ଟି_Boolean;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    int64_t ival;
} ପଦସମଷ୍ଟି_Integer;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} ପଦସମଷ୍ଟି_Float;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t ଚାବି_name;
} ପଦସମଷ୍ଟି_ଚାବି_Name;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} ପଦସମଷ୍ଟି_ଚାବି_Sym;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ପଦସମଷ୍ଟି_Def *ବାମ;
    ପଦସମଷ୍ଟି_Def *ଡାହାଣ;
} ପଦସମଷ୍ଟି_ଦ୍ୱିମିକ;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ପଦସମଷ୍ଟି_Def *child;
} ପଦସମଷ୍ଟି_Unary;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} ପଦସମଷ୍ଟି_Field_Ref;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    ପଦସମଷ୍ଟି_Def *entry;
} ପଦସମଷ୍ଟି_Array_Ref;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    xkb_atom_t name;
    ପଦସମଷ୍ଟି_Def *args;
} ପଦସମଷ୍ଟି_Action;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    /* List of actions for a single level. */
    ପଦସମଷ୍ଟି_Def *actions;
} ପଦସମଷ୍ଟି_Action_List;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} ପଦସମଷ୍ଟି_Keysym_List;

ସଂଘ ପଦସମଷ୍ଟି_Def {
    ParseCommon common;
    ପଦସମଷ୍ଟି_Ident ident;
    ପଦସମଷ୍ଟି_String string;
    ପଦସମଷ୍ଟି_Boolean boolean;
    ପଦସମଷ୍ଟି_Integer integer;
    ପଦସମଷ୍ଟି_ଚାବି_Name ଚାବି_name;
    ପଦସମଷ୍ଟି_ଚାବି_Sym keysym;
    ପଦସମଷ୍ଟି_ଦ୍ୱିମିକ ଦ୍ୱିମିକ;
    ପଦସମଷ୍ଟି_Unary unary;
    ପଦସମଷ୍ଟି_Field_Ref field_ref;
    ପଦସମଷ୍ଟି_Array_Ref array_ref;
    ପଦସମଷ୍ଟି_Action action;
    ପଦସମଷ୍ଟି_Action_List actions;
    ପଦସମଷ୍ଟି_Keysym_List keysym_list;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    ପଦସମଷ୍ଟି_Def *name;
    ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ;
} VarDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t name;
    ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ;
} VModDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t name;
    int64_t ମୂଲ୍ୟ;
} KeycodeDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} ଚାବି_Alias_Def;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} ଚାବି_Type_Def;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t ଚାବି_Name;
    VarDef *symbols;
} SymbolsDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    ପଦସମଷ୍ଟି_Def *modifiers;
    ପଦସମଷ୍ଟି_Def *ଚାବିଗୁଡ଼ିକ;
} ModMapDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    int64_t group;
    ପଦସମଷ୍ଟି_Def *def;
} GroupCompatDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_keysym_t sym;
    ପଦସମଷ୍ଟି_Def *match;
    VarDef *def;
} InterpDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    bool ଆଭାସୀ;
    int64_t ndx;
    ପଦସମଷ୍ଟି_Def *name;
} LedNameDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଗଣନା merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଅକ୍ଷର *name;
} UnknownStatement;

ଗଣନା xkb_map_flags {
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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ParseCommon common;
    ଅକ୍ଷର *name;
    ParseCommon *defs;
    ଗଣନା xkb_file_type file_type;
    ଗଣନା xkb_map_flags flags;
} XkbFile;
