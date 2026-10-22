/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"

ಎಣಿಕೆ xkb_file_type {
    /* Component files, by order of compilation. */
    FILE_TYPE_KEYCODES = 0,
    FILE_TYPE_TYPES = 1,
    FILE_TYPE_COMPAT = 2,
    FILE_TYPE_SYMBOLS = 3,

    /* File types which must be found in a keymap file. */
    ಮೊದಲ_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    ಕೊನೆಯ_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_ನಿಯಮಗಳು,

    FILE_TYPE_ಅಂಕೆ_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_ಅಂಕೆ_ENTRIES
};

ಎಣಿಕೆ stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT,
    STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_ಗುಂಪು_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_ಅಂಕೆ_ಮೌಲ್ಯಗಳು
};

ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ {
    ವಿಲೀನ_DEFAULT,
    ವಿಲೀನ_AUGMENT,
    ವಿಲೀನ_OVERRIDE,
    ವಿಲೀನ_REPLACE,
    ವಿಲೀನ_ಬಹುಲಕ_ಅಂಕೆ_ENTRIES,
};
ಎಣಿಕೆ { ವಿಲೀನ_ಬಹುಲಕ_MIN_ಅಗಲ = 3 /* 2 bits + sign */ };
static_assert(ವಿಲೀನ_ಬಹುಲಕ_ಅಂಕೆ_ENTRIES - 1 <= (1 << (ವಿಲೀನ_ಬಹುಲಕ_MIN_ಅಗಲ - 1)) - 1,
              "Cannot encode merge mode");

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_type_ಗಮ್ಯ_string(ಎಣಿಕೆ xkb_file_type type);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
stmt_type_ಗಮ್ಯ_string(ಎಣಿಕೆ stmt_type type);

ಅಕ್ಷರ
stmt_type_ಗಮ್ಯ_operator_char(ಎಣಿಕೆ stmt_type type);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ _ParseCommon  {
    ರಚನೆ _ParseCommon *ಮುಂದಿನ;
    ಎಣಿಕೆ stmt_type type;
} ParseCommon;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ _IncludeStmt {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    ಅಕ್ಷರ *stmt;
    ಅಕ್ಷರ *file;
    ಅಕ್ಷರ *map;
    ಅಕ್ಷರ *modifier;
    ರಚನೆ _IncludeStmt *ಮುಂದಿನ_incl;
} IncludeStmt;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಒಕ್ಕೂಟ ಅಭಿವ್ಯಕ್ತಿ_Def ಅಭಿವ್ಯಕ್ತಿ_Def;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t ident;
} ಅಭಿವ್ಯಕ್ತಿ_Ident;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t str;
} ಅಭಿವ್ಯಕ್ತಿ_String;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    bool ಗಣ;
} ಅಭಿವ್ಯಕ್ತಿ_Boolean;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    int64_t ival;
} ಅಭಿವ್ಯಕ್ತಿ_Integer;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} ಅಭಿವ್ಯಕ್ತಿ_Float;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t ಕೀಲಿ_name;
} ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Name;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಬಲ;
} ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಮಗು;
} ಅಭಿವ್ಯಕ್ತಿ_Unary;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ಕ್ಷೇತ್ರ_2;
} ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_Ref;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ನಮೂದು;
} ಅಭಿವ್ಯಕ್ತಿ_Array_Ref;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    xkb_atom_t name;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ವಾದಗಳು;
} ಅಭಿವ್ಯಕ್ತಿ_Action;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    /* List of actions for a single level. */
    ಅಭಿವ್ಯಕ್ತಿ_Def *actions;
} ಅಭಿವ್ಯಕ್ತಿ_Action_List;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} ಅಭಿವ್ಯಕ್ತಿ_Keysym_List;

ಒಕ್ಕೂಟ ಅಭಿವ್ಯಕ್ತಿ_Def {
    ParseCommon common;
    ಅಭಿವ್ಯಕ್ತಿ_Ident ident;
    ಅಭಿವ್ಯಕ್ತಿ_String string;
    ಅಭಿವ್ಯಕ್ತಿ_Boolean boolean;
    ಅಭಿವ್ಯಕ್ತಿ_Integer integer;
    ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Name ಕೀಲಿ_name;
    ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym keysym;
    ಅಭಿವ್ಯಕ್ತಿ_ದ್ವಿಮಾನ ದ್ವಿಮಾನ;
    ಅಭಿವ್ಯಕ್ತಿ_Unary unary;
    ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_Ref ಕ್ಷೇತ್ರ_ref;
    ಅಭಿವ್ಯಕ್ತಿ_Array_Ref array_ref;
    ಅಭಿವ್ಯಕ್ತಿ_Action action;
    ಅಭಿವ್ಯಕ್ತಿ_Action_List actions;
    ಅಭಿವ್ಯಕ್ತಿ_Keysym_List keysym_list;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    ಅಭಿವ್ಯಕ್ತಿ_Def *name;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ;
} VarDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t name;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ;
} VModDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t name;
    int64_t ಮೌಲ್ಯ;
} KeycodeDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t alias;
    xkb_atom_t real;
} ಕೀಲಿ_Alias_Def;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t name;
    VarDef *body;
} ಕೀಲಿ_Type_Def;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t ಕೀಲಿ_Name;
    VarDef *symbols;
} SymbolsDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    // NOTE: Can also be “None”, rather than a modifier name.
    ಅಭಿವ್ಯಕ್ತಿ_Def *modifiers;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿಗಳು;
} ModMapDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    int64_t ಗುಂಪು;
    ಅಭಿವ್ಯಕ್ತಿ_Def *def;
} ಗುಂಪು_Compat_Def;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_keysym_t sym;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಹೊಂದಿಕೆ;
    VarDef *def;
} InterpDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    bool ವಾಸ್ತವಿಕ;
    int64_t ndx;
    ಅಭಿವ್ಯಕ್ತಿ_Def *name;
} LedNameDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಅಕ್ಷರ *name;
} UnknownStatement;

ಎಣಿಕೆ xkb_map_flags {
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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ParseCommon common;
    ಅಕ್ಷರ *name;
    ParseCommon *defs;
    ಎಣಿಕೆ xkb_file_type file_type;
    ಎಣಿಕೆ xkb_map_flags flags;
} XkbFile;
