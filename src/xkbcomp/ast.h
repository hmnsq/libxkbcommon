/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل "atom.h"
#شامل "darray.h"

شمار xkb_file_type {
    /* Component files, by order of compilation. */
    FILE_TYPE_KEYCODES = 0,
    FILE_TYPE_TYPES = 1,
    FILE_TYPE_COMPAT = 2,
    FILE_TYPE_SYMBOLS = 3,

    /* File types which must be found in a keymap file. */
    پہلا_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    آخری_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_قواعد,

    FILE_TYPE_عدد_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_عدد_ENTRIES
};

شمار stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_اظہاریہ_STRING_LITERAL,
    STMT_اظہاریہ_INTEGER_LITERAL,
    STMT_اظہاریہ_FLOAT_LITERAL,
    STMT_اظہاریہ_BOOLEAN_LITERAL,
    STMT_اظہاریہ_KEYNAME_LITERAL,
    STMT_اظہاریہ_KEYSYM_LITERAL,
    STMT_اظہاریہ_IDENT,
    STMT_اظہاریہ_ACTION_DECL,
    STMT_اظہاریہ_میدان_REF,
    STMT_اظہاریہ_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_اظہاریہ_خالی_خانے_LIST,
    STMT_اظہاریہ_KEYSYM_LIST,
    STMT_اظہاریہ_ACTION_LIST,
    STMT_اظہاریہ_جوڑو,
    STMT_اظہاریہ_تفریق,
    STMT_اظہاریہ_MULTIPLY,
    STMT_اظہاریہ_DIVIDE,
    STMT_اظہاریہ_تفویض,
    STMT_اظہاریہ_NOT,
    STMT_اظہاریہ_NEGATE,
    STMT_اظہاریہ_INVERT,
    STMT_اظہاریہ_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_گروہ_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_عدد_اقدار
};

شمار ملاؤ_منوال {
    ملاؤ_DEFAULT,
    ملاؤ_AUGMENT,
    ملاؤ_OVERRIDE,
    ملاؤ_REPLACE,
    ملاؤ_منوال_عدد_ENTRIES,
};
شمار { ملاؤ_منوال_MIN_چوڑائی = 3 /* 2 bits + sign */ };
static_assert(ملاؤ_منوال_عدد_ENTRIES - 1 <= (1 << (ملاؤ_منوال_MIN_چوڑائی - 1)) - 1,
              "Cannot encode merge mode");

ثابت حرف *
xkb_file_type_مقصد_string(شمار xkb_file_type type);

ثابت حرف *
stmt_type_مقصد_string(شمار stmt_type type);

حرف
stmt_type_مقصد_operator_char(شمار stmt_type type);

قسمتعریف ساخت _ParseCommon  {
    ساخت _ParseCommon *اگلا;
    شمار stmt_type type;
} ParseCommon;

قسمتعریف ساخت _IncludeStmt {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    حرف *stmt;
    حرف *file;
    حرف *map;
    حرف *modifier;
    ساخت _IncludeStmt *اگلا_incl;
} IncludeStmt;

قسمتعریف اتحاد اظہاریہ_Def اظہاریہ_Def;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t ident;
} اظہاریہ_Ident;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t str;
} اظہاریہ_String;

قسمتعریف ساخت {
    ParseCommon common;
    bool سیٹ;
} اظہاریہ_Boolean;

قسمتعریف ساخت {
    ParseCommon common;
    int64_t ival;
} اظہاریہ_Integer;

قسمتعریف ساخت {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} اظہاریہ_Float;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t کلید_name;
} اظہاریہ_کلید_Name;

قسمتعریف ساخت {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} اظہاریہ_کلید_Sym;

قسمتعریف ساخت {
    ParseCommon common;
    اظہاریہ_Def *بایاں;
    اظہاریہ_Def *دایاں;
} اظہاریہ_ثنائی;

قسمتعریف ساخت {
    ParseCommon common;
    اظہاریہ_Def *بچہ;
} اظہاریہ_Unary;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t میدان_2;
} اظہاریہ_میدان_Ref;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t میدان_2;
    اظہاریہ_Def *اندراج_2;
} اظہاریہ_Array_Ref;

قسمتعریف ساخت {
    ParseCommon common;
    xkb_atom_t name;
    اظہاریہ_Def *دلائل;
} اظہاریہ_Action;

قسمتعریف ساخت {
    ParseCommon common;
    /* List of actions for a single level. */
    اظہاریہ_Def *actions;
} اظہاریہ_Action_List;

قسمتعریف ساخت {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} اظہاریہ_Keysym_List;

اتحاد اظہاریہ_Def {
    ParseCommon common;
    اظہاریہ_Ident ident;
    اظہاریہ_String string;
    اظہاریہ_Boolean boolean;
    اظہاریہ_Integer integer;
    اظہاریہ_کلید_Name کلید_name;
    اظہاریہ_کلید_Sym keysym;
    اظہاریہ_ثنائی ثنائی;
    اظہاریہ_Unary unary;
    اظہاریہ_میدان_Ref میدان_ref;
    اظہاریہ_Array_Ref array_ref;
    اظہاریہ_Action action;
    اظہاریہ_Action_List actions;
    اظہاریہ_Keysym_List keysym_list;
};

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    اظہاریہ_Def *name;
    اظہاریہ_Def *قدر;
} VarDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t name;
    اظہاریہ_Def *قدر;
} VModDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t name;
    int64_t قدر;
} KeycodeDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t alias;
    xkb_atom_t real;
} کلید_Alias_Def;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t name;
    VarDef *body;
} کلید_Type_Def;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t کلید_Name;
    VarDef *symbols;
} SymbolsDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    // NOTE: Can also be “None”, rather than a modifier name.
    اظہاریہ_Def *modifiers;
    اظہاریہ_Def *کلیدیں;
} ModMapDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    int64_t گروہ;
    اظہاریہ_Def *def;
} گروہ_Compat_Def;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_keysym_t sym;
    اظہاریہ_Def *مطابقت;
    VarDef *def;
} InterpDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    bool مجازی;
    int64_t ndx;
    اظہاریہ_Def *name;
} LedNameDef;

قسمتعریف ساخت {
    ParseCommon common;
    شمار ملاؤ_منوال ملاؤ;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

قسمتعریف ساخت {
    ParseCommon common;
    حرف *name;
} UnknownStatement;

شمار xkb_map_flags {
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

قسمتعریف ساخت {
    ParseCommon common;
    حرف *name;
    ParseCommon *defs;
    شمار xkb_file_type file_type;
    شمار xkb_map_flags flags;
} XkbFile;
