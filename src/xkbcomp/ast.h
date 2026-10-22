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

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "atom.h"
#अन्तर्भाव "darray.h"

गणना xkb_file_type {
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

    FILE_TYPE_अङ्कः_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_अङ्कः_ENTRIES
};

गणना stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_व्यञ्जकम्_STRING_LITERAL,
    STMT_व्यञ्जकम्_INTEGER_LITERAL,
    STMT_व्यञ्जकम्_FLOAT_LITERAL,
    STMT_व्यञ्जकम्_BOOLEAN_LITERAL,
    STMT_व्यञ्जकम्_KEYNAME_LITERAL,
    STMT_व्यञ्जकम्_KEYSYM_LITERAL,
    STMT_व्यञ्जकम्_IDENT,
    STMT_व्यञ्जकम्_ACTION_DECL,
    STMT_व्यञ्जकम्_FIELD_REF,
    STMT_व्यञ्जकम्_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_व्यञ्जकम्_EMPTY_LIST,
    STMT_व्यञ्जकम्_KEYSYM_LIST,
    STMT_व्यञ्जकम्_ACTION_LIST,
    STMT_व्यञ्जकम्_ADD,
    STMT_व्यञ्जकम्_SUBTRACT,
    STMT_व्यञ्जकम्_MULTIPLY,
    STMT_व्यञ्जकम्_DIVIDE,
    STMT_व्यञ्जकम्_ASSIGN,
    STMT_व्यञ्जकम्_NOT,
    STMT_व्यञ्जकम्_NEGATE,
    STMT_व्यञ्जकम्_INVERT,
    STMT_व्यञ्जकम्_UNARY_PLUS,
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

    STMT_अङ्कः_मानानि
};

गणना merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_अङ्कः_ENTRIES,
};
गणना { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_अङ्कः_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

ध्रुव वर्ण *
xkb_file_type_to_string(गणना xkb_file_type type);

ध्रुव वर्ण *
stmt_type_to_string(गणना stmt_type type);

वर्ण
stmt_type_to_operator_char(गणना stmt_type type);

प्रकारनिर्देश संरचना _ParseCommon  {
    संरचना _ParseCommon *next;
    गणना stmt_type type;
} ParseCommon;

प्रकारनिर्देश संरचना _IncludeStmt {
    ParseCommon common;
    गणना merge_mode merge;
    वर्ण *stmt;
    वर्ण *file;
    वर्ण *map;
    वर्ण *modifier;
    संरचना _IncludeStmt *next_incl;
} IncludeStmt;

प्रकारनिर्देश सङ्घ व्यञ्जकम्_Def व्यञ्जकम्_Def;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t ident;
} व्यञ्जकम्_Ident;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t str;
} व्यञ्जकम्_String;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    bool set;
} व्यञ्जकम्_Boolean;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    int64_t ival;
} व्यञ्जकम्_Integer;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} व्यञ्जकम्_Float;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t कुञ्चिका_name;
} व्यञ्जकम्_कुञ्चिका_Name;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} व्यञ्जकम्_कुञ्चिका_Sym;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    व्यञ्जकम्_Def *वामः;
    व्यञ्जकम्_Def *दक्षिणः;
} व्यञ्जकम्_द्विमान;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    व्यञ्जकम्_Def *child;
} व्यञ्जकम्_Unary;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} व्यञ्जकम्_Field_Ref;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    व्यञ्जकम्_Def *entry;
} व्यञ्जकम्_Array_Ref;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    xkb_atom_t name;
    व्यञ्जकम्_Def *args;
} व्यञ्जकम्_Action;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    /* List of actions for a single level. */
    व्यञ्जकम्_Def *actions;
} व्यञ्जकम्_Action_List;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} व्यञ्जकम्_Keysym_List;

सङ्घ व्यञ्जकम्_Def {
    ParseCommon common;
    व्यञ्जकम्_Ident ident;
    व्यञ्जकम्_String string;
    व्यञ्जकम्_Boolean boolean;
    व्यञ्जकम्_Integer integer;
    व्यञ्जकम्_कुञ्चिका_Name कुञ्चिका_name;
    व्यञ्जकम्_कुञ्चिका_Sym keysym;
    व्यञ्जकम्_द्विमान द्विमान;
    व्यञ्जकम्_Unary unary;
    व्यञ्जकम्_Field_Ref field_ref;
    व्यञ्जकम्_Array_Ref array_ref;
    व्यञ्जकम्_Action action;
    व्यञ्जकम्_Action_List actions;
    व्यञ्जकम्_Keysym_List keysym_list;
};

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    व्यञ्जकम्_Def *name;
    व्यञ्जकम्_Def *मानम्;
} VarDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    व्यञ्जकम्_Def *मानम्;
} VModDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    int64_t मानम्;
} KeycodeDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} कुञ्चिका_Alias_Def;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} कुञ्चिका_Type_Def;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t कुञ्चिका_Name;
    VarDef *symbols;
} SymbolsDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    व्यञ्जकम्_Def *modifiers;
    व्यञ्जकम्_Def *कुञ्चिकाः;
} ModMapDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    int64_t group;
    व्यञ्जकम्_Def *def;
} GroupCompatDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_keysym_t sym;
    व्यञ्जकम्_Def *match;
    VarDef *def;
} InterpDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    bool आभास;
    int64_t ndx;
    व्यञ्जकम्_Def *name;
} LedNameDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

प्रकारनिर्देश संरचना {
    ParseCommon common;
    वर्ण *name;
} UnknownStatement;

गणना xkb_map_flags {
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

प्रकारनिर्देश संरचना {
    ParseCommon common;
    वर्ण *name;
    ParseCommon *defs;
    गणना xkb_file_type file_type;
    गणना xkb_map_flags flags;
} XkbFile;
