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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "darray.h"

ગણના xkb_file_type {
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

    FILE_TYPE_અંક_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_અંક_ENTRIES
};

ગણના stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_પદાવલિ_STRING_LITERAL,
    STMT_પદાવલિ_INTEGER_LITERAL,
    STMT_પદાવલિ_FLOAT_LITERAL,
    STMT_પદાવલિ_BOOLEAN_LITERAL,
    STMT_પદાવલિ_KEYNAME_LITERAL,
    STMT_પદાવલિ_KEYSYM_LITERAL,
    STMT_પદાવલિ_IDENT,
    STMT_પદાવલિ_ACTION_DECL,
    STMT_પદાવલિ_FIELD_REF,
    STMT_પદાવલિ_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_પદાવલિ_EMPTY_LIST,
    STMT_પદાવલિ_KEYSYM_LIST,
    STMT_પદાવલિ_ACTION_LIST,
    STMT_પદાવલિ_ADD,
    STMT_પદાવલિ_SUBTRACT,
    STMT_પદાવલિ_MULTIPLY,
    STMT_પદાવલિ_DIVIDE,
    STMT_પદાવલિ_ASSIGN,
    STMT_પદાવલિ_NOT,
    STMT_પદાવલિ_NEGATE,
    STMT_પદાવલિ_INVERT,
    STMT_પદાવલિ_UNARY_PLUS,
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

    STMT_અંક_કિંમતો
};

ગણના merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_અંક_ENTRIES,
};
ગણના { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_અંક_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

અચલ અક્ષર *
xkb_file_type_to_string(ગણના xkb_file_type type);

અચલ અક્ષર *
stmt_type_to_string(ગણના stmt_type type);

અક્ષર
stmt_type_to_operator_char(ગણના stmt_type type);

પ્રકારવ્યાખ્યા સંરચના _ParseCommon  {
    સંરચના _ParseCommon *next;
    ગણના stmt_type type;
} ParseCommon;

પ્રકારવ્યાખ્યા સંરચના _IncludeStmt {
    ParseCommon common;
    ગણના merge_mode merge;
    અક્ષર *stmt;
    અક્ષર *file;
    અક્ષર *map;
    અક્ષર *modifier;
    સંરચના _IncludeStmt *next_incl;
} IncludeStmt;

પ્રકારવ્યાખ્યા સંઘ પદાવલિ_Def પદાવલિ_Def;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t ident;
} પદાવલિ_Ident;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t str;
} પદાવલિ_String;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    bool set;
} પદાવલિ_Boolean;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    int64_t ival;
} પદાવલિ_Integer;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} પદાવલિ_Float;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t ચાવી_name;
} પદાવલિ_ચાવી_Name;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} પદાવલિ_ચાવી_Sym;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    પદાવલિ_Def *ડાબું;
    પદાવલિ_Def *જમણું;
} પદાવલિ_દ્વિઅંકી;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    પદાવલિ_Def *child;
} પદાવલિ_Unary;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} પદાવલિ_Field_Ref;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    પદાવલિ_Def *entry;
} પદાવલિ_Array_Ref;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    xkb_atom_t name;
    પદાવલિ_Def *args;
} પદાવલિ_Action;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    /* List of actions for a single level. */
    પદાવલિ_Def *actions;
} પદાવલિ_Action_List;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} પદાવલિ_Keysym_List;

સંઘ પદાવલિ_Def {
    ParseCommon common;
    પદાવલિ_Ident ident;
    પદાવલિ_String string;
    પદાવલિ_Boolean boolean;
    પદાવલિ_Integer integer;
    પદાવલિ_ચાવી_Name ચાવી_name;
    પદાવલિ_ચાવી_Sym keysym;
    પદાવલિ_દ્વિઅંકી દ્વિઅંકી;
    પદાવલિ_Unary unary;
    પદાવલિ_Field_Ref field_ref;
    પદાવલિ_Array_Ref array_ref;
    પદાવલિ_Action action;
    પદાવલિ_Action_List actions;
    પદાવલિ_Keysym_List keysym_list;
};

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    પદાવલિ_Def *name;
    પદાવલિ_Def *કિંમત;
} VarDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t name;
    પદાવલિ_Def *કિંમત;
} VModDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t name;
    int64_t કિંમત;
} KeycodeDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} ચાવી_Alias_Def;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} ચાવી_Type_Def;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t ચાવી_Name;
    VarDef *symbols;
} SymbolsDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    પદાવલિ_Def *modifiers;
    પદાવલિ_Def *ચાવીઓ;
} ModMapDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    int64_t group;
    પદાવલિ_Def *def;
} GroupCompatDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_keysym_t sym;
    પદાવલિ_Def *match;
    VarDef *def;
} InterpDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    bool આભાસી;
    int64_t ndx;
    પદાવલિ_Def *name;
} LedNameDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    ગણના merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    અક્ષર *name;
} UnknownStatement;

ગણના xkb_map_flags {
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

પ્રકારવ્યાખ્યા સંરચના {
    ParseCommon common;
    અક્ષર *name;
    ParseCommon *defs;
    ગણના xkb_file_type file_type;
    ગણના xkb_map_flags flags;
} XkbFile;
