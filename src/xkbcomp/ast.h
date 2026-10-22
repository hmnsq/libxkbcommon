/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"

ਗਿਣਤੀ xkb_file_type {
    /* Component files, by order of compilation. */
    FILE_TYPE_KEYCODES = 0,
    FILE_TYPE_TYPES = 1,
    FILE_TYPE_COMPAT = 2,
    FILE_TYPE_SYMBOLS = 3,

    /* File types which must be found in a keymap file. */
    ਪਹਿਲਾ_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    ਆਖਰੀ_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_ਨਿਯਮਾਂ,

    FILE_TYPE_ਅੰਕ_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_ਅੰਕ_ENTRIES
};

ਗਿਣਤੀ stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_ਵਿਅੰਜਕ_STRING_LITERAL,
    STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL,
    STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL,
    STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL,
    STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL,
    STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL,
    STMT_ਵਿਅੰਜਕ_IDENT,
    STMT_ਵਿਅੰਜਕ_ACTION_DECL,
    STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF,
    STMT_ਵਿਅੰਜਕ_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST,
    STMT_ਵਿਅੰਜਕ_KEYSYM_LIST,
    STMT_ਵਿਅੰਜਕ_ACTION_LIST,
    STMT_ਵਿਅੰਜਕ_ਜੋੜੋ,
    STMT_ਵਿਅੰਜਕ_ਘਟਾਓ,
    STMT_ਵਿਅੰਜਕ_MULTIPLY,
    STMT_ਵਿਅੰਜਕ_DIVIDE,
    STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ,
    STMT_ਵਿਅੰਜਕ_NOT,
    STMT_ਵਿਅੰਜਕ_NEGATE,
    STMT_ਵਿਅੰਜਕ_INVERT,
    STMT_ਵਿਅੰਜਕ_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_ਸਮੂਹ_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_ਅੰਕ_ਮੁੱਲਾਂ
};

ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ {
    ਮਿਲਾਓ_DEFAULT,
    ਮਿਲਾਓ_AUGMENT,
    ਮਿਲਾਓ_OVERRIDE,
    ਮਿਲਾਓ_REPLACE,
    ਮਿਲਾਓ_ਬਹੁਲਕ_ਅੰਕ_ENTRIES,
};
ਗਿਣਤੀ { ਮਿਲਾਓ_ਬਹੁਲਕ_MIN_ਚੌੜਾਈ = 3 /* 2 bits + sign */ };
static_assert(ਮਿਲਾਓ_ਬਹੁਲਕ_ਅੰਕ_ENTRIES - 1 <= (1 << (ਮਿਲਾਓ_ਬਹੁਲਕ_MIN_ਚੌੜਾਈ - 1)) - 1,
              "Cannot encode merge mode");

ਅਚਲ ਅੱਖਰ *
xkb_file_type_ਮੰਜ਼ਿਲ_string(ਗਿਣਤੀ xkb_file_type type);

ਅਚਲ ਅੱਖਰ *
stmt_type_ਮੰਜ਼ਿਲ_string(ਗਿਣਤੀ stmt_type type);

ਅੱਖਰ
stmt_type_ਮੰਜ਼ਿਲ_operator_char(ਗਿਣਤੀ stmt_type type);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ _ParseCommon  {
    ਬਣਤਰ _ParseCommon *ਅਗਲਾ;
    ਗਿਣਤੀ stmt_type type;
} ParseCommon;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ _IncludeStmt {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    ਅੱਖਰ *stmt;
    ਅੱਖਰ *file;
    ਅੱਖਰ *map;
    ਅੱਖਰ *modifier;
    ਬਣਤਰ _IncludeStmt *ਅਗਲਾ_incl;
} IncludeStmt;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੰਘ ਵਿਅੰਜਕ_Def ਵਿਅੰਜਕ_Def;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t ident;
} ਵਿਅੰਜਕ_Ident;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t str;
} ਵਿਅੰਜਕ_String;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    bool ਸੈੱਟ;
} ਵਿਅੰਜਕ_Boolean;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    int64_t ival;
} ਵਿਅੰਜਕ_Integer;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} ਵਿਅੰਜਕ_Float;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t ਕੁੰਜੀ_name;
} ਵਿਅੰਜਕ_ਕੁੰਜੀ_Name;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਵਿਅੰਜਕ_Def *ਖੱਬਾ;
    ਵਿਅੰਜਕ_Def *ਸੱਜਾ;
} ਵਿਅੰਜਕ_ਦੁਆਧਾਰੀ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਵਿਅੰਜਕ_Def *ਬੱਚਾ;
} ਵਿਅੰਜਕ_Unary;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ਖੇਤਰ_2;
} ਵਿਅੰਜਕ_ਖੇਤਰ_Ref;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *ਇੰਦਰਾਜ;
} ਵਿਅੰਜਕ_Array_Ref;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    xkb_atom_t name;
    ਵਿਅੰਜਕ_Def *ਦਲੀਲਾਂ;
} ਵਿਅੰਜਕ_Action;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    /* List of actions for a single level. */
    ਵਿਅੰਜਕ_Def *actions;
} ਵਿਅੰਜਕ_Action_List;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} ਵਿਅੰਜਕ_Keysym_List;

ਸੰਘ ਵਿਅੰਜਕ_Def {
    ParseCommon common;
    ਵਿਅੰਜਕ_Ident ident;
    ਵਿਅੰਜਕ_String string;
    ਵਿਅੰਜਕ_Boolean boolean;
    ਵਿਅੰਜਕ_Integer integer;
    ਵਿਅੰਜਕ_ਕੁੰਜੀ_Name ਕੁੰਜੀ_name;
    ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym keysym;
    ਵਿਅੰਜਕ_ਦੁਆਧਾਰੀ ਦੁਆਧਾਰੀ;
    ਵਿਅੰਜਕ_Unary unary;
    ਵਿਅੰਜਕ_ਖੇਤਰ_Ref ਖੇਤਰ_ref;
    ਵਿਅੰਜਕ_Array_Ref array_ref;
    ਵਿਅੰਜਕ_Action action;
    ਵਿਅੰਜਕ_Action_List actions;
    ਵਿਅੰਜਕ_Keysym_List keysym_list;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    ਵਿਅੰਜਕ_Def *name;
    ਵਿਅੰਜਕ_Def *ਮੁੱਲ;
} VarDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t name;
    ਵਿਅੰਜਕ_Def *ਮੁੱਲ;
} VModDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t name;
    int64_t ਮੁੱਲ;
} KeycodeDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t alias;
    xkb_atom_t real;
} ਕੁੰਜੀ_Alias_Def;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t name;
    VarDef *body;
} ਕੁੰਜੀ_Type_Def;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t ਕੁੰਜੀ_Name;
    VarDef *symbols;
} SymbolsDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    // NOTE: Can also be “None”, rather than a modifier name.
    ਵਿਅੰਜਕ_Def *modifiers;
    ਵਿਅੰਜਕ_Def *ਕੁੰਜੀਆਂ;
} ModMapDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    int64_t ਸਮੂਹ;
    ਵਿਅੰਜਕ_Def *def;
} ਸਮੂਹ_Compat_Def;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_keysym_t sym;
    ਵਿਅੰਜਕ_Def *ਮੇਲ;
    VarDef *def;
} InterpDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    bool ਆਭਾਸੀ;
    int64_t ndx;
    ਵਿਅੰਜਕ_Def *name;
} LedNameDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਅੱਖਰ *name;
} UnknownStatement;

ਗਿਣਤੀ xkb_map_flags {
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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ParseCommon common;
    ਅੱਖਰ *name;
    ParseCommon *defs;
    ਗਿਣਤੀ xkb_file_type file_type;
    ਗਿਣਤੀ xkb_map_flags flags;
} XkbFile;
