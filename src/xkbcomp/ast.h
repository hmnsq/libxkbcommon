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

    FILE_TYPE_अंक_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_अंक_ENTRIES
};

गणना stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_अभिव्यक्ती_STRING_LITERAL,
    STMT_अभिव्यक्ती_INTEGER_LITERAL,
    STMT_अभिव्यक्ती_FLOAT_LITERAL,
    STMT_अभिव्यक्ती_BOOLEAN_LITERAL,
    STMT_अभिव्यक्ती_KEYNAME_LITERAL,
    STMT_अभिव्यक्ती_KEYSYM_LITERAL,
    STMT_अभिव्यक्ती_IDENT,
    STMT_अभिव्यक्ती_ACTION_DECL,
    STMT_अभिव्यक्ती_FIELD_REF,
    STMT_अभिव्यक्ती_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_अभिव्यक्ती_EMPTY_LIST,
    STMT_अभिव्यक्ती_KEYSYM_LIST,
    STMT_अभिव्यक्ती_ACTION_LIST,
    STMT_अभिव्यक्ती_ADD,
    STMT_अभिव्यक्ती_SUBTRACT,
    STMT_अभिव्यक्ती_MULTIPLY,
    STMT_अभिव्यक्ती_DIVIDE,
    STMT_अभिव्यक्ती_ASSIGN,
    STMT_अभिव्यक्ती_NOT,
    STMT_अभिव्यक्ती_NEGATE,
    STMT_अभिव्यक्ती_INVERT,
    STMT_अभिव्यक्ती_UNARY_PLUS,
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

    STMT_अंक_मूल्ये
};

गणना merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_अंक_ENTRIES,
};
गणना { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_अंक_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

स्थिरांक अक्षर *
xkb_file_type_to_string(गणना xkb_file_type type);

स्थिरांक अक्षर *
stmt_type_to_string(गणना stmt_type type);

अक्षर
stmt_type_to_operator_char(गणना stmt_type type);

प्रकारव्याख्या संरचना _ParseCommon  {
    संरचना _ParseCommon *next;
    गणना stmt_type type;
} ParseCommon;

प्रकारव्याख्या संरचना _IncludeStmt {
    ParseCommon common;
    गणना merge_mode merge;
    अक्षर *stmt;
    अक्षर *file;
    अक्षर *map;
    अक्षर *modifier;
    संरचना _IncludeStmt *next_incl;
} IncludeStmt;

प्रकारव्याख्या संघ अभिव्यक्ती_Def अभिव्यक्ती_Def;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t ident;
} अभिव्यक्ती_Ident;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t str;
} अभिव्यक्ती_String;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    bool set;
} अभिव्यक्ती_Boolean;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    int64_t ival;
} अभिव्यक्ती_Integer;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} अभिव्यक्ती_Float;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t किल्ली_name;
} अभिव्यक्ती_किल्ली_Name;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} अभिव्यक्ती_किल्ली_Sym;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    अभिव्यक्ती_Def *डावा;
    अभिव्यक्ती_Def *उजवा;
} अभिव्यक्ती_द्विमान;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    अभिव्यक्ती_Def *child;
} अभिव्यक्ती_Unary;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} अभिव्यक्ती_Field_Ref;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    अभिव्यक्ती_Def *entry;
} अभिव्यक्ती_Array_Ref;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    xkb_atom_t name;
    अभिव्यक्ती_Def *args;
} अभिव्यक्ती_Action;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    /* List of actions for a single level. */
    अभिव्यक्ती_Def *actions;
} अभिव्यक्ती_Action_List;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} अभिव्यक्ती_Keysym_List;

संघ अभिव्यक्ती_Def {
    ParseCommon common;
    अभिव्यक्ती_Ident ident;
    अभिव्यक्ती_String string;
    अभिव्यक्ती_Boolean boolean;
    अभिव्यक्ती_Integer integer;
    अभिव्यक्ती_किल्ली_Name किल्ली_name;
    अभिव्यक्ती_किल्ली_Sym keysym;
    अभिव्यक्ती_द्विमान द्विमान;
    अभिव्यक्ती_Unary unary;
    अभिव्यक्ती_Field_Ref field_ref;
    अभिव्यक्ती_Array_Ref array_ref;
    अभिव्यक्ती_Action action;
    अभिव्यक्ती_Action_List actions;
    अभिव्यक्ती_Keysym_List keysym_list;
};

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    अभिव्यक्ती_Def *name;
    अभिव्यक्ती_Def *मूल्य;
} VarDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    अभिव्यक्ती_Def *मूल्य;
} VModDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    int64_t मूल्य;
} KeycodeDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} किल्ली_Alias_Def;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} किल्ली_Type_Def;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t किल्ली_Name;
    VarDef *symbols;
} SymbolsDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    अभिव्यक्ती_Def *modifiers;
    अभिव्यक्ती_Def *किल्ल्या;
} ModMapDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    int64_t group;
    अभिव्यक्ती_Def *def;
} GroupCompatDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_keysym_t sym;
    अभिव्यक्ती_Def *match;
    VarDef *def;
} InterpDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    bool आभासी;
    int64_t ndx;
    अभिव्यक्ती_Def *name;
} LedNameDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    गणना merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

प्रकारव्याख्या संरचना {
    ParseCommon common;
    अक्षर *name;
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

प्रकारव्याख्या संरचना {
    ParseCommon common;
    अक्षर *name;
    ParseCommon *defs;
    गणना xkb_file_type file_type;
    गणना xkb_map_flags flags;
} XkbFile;
