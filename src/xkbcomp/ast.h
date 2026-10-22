/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#لارښوونه once

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
    لومړی_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    وروستی_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_قواعد,

    FILE_TYPE_عدد_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_عدد_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_بیان_STRING_LITERAL,
    STMT_بیان_INTEGER_LITERAL,
    STMT_بیان_FLOAT_LITERAL,
    STMT_بیان_BOOLEAN_LITERAL,
    STMT_بیان_KEYNAME_LITERAL,
    STMT_بیان_KEYSYM_LITERAL,
    STMT_بیان_IDENT,
    STMT_بیان_ACTION_DECL,
    STMT_بیان_ډګر_REF,
    STMT_بیان_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_بیان_تشه_LIST,
    STMT_بیان_KEYSYM_LIST,
    STMT_بیان_ACTION_LIST,
    STMT_بیان_جمع_کړه,
    STMT_بیان_کمول,
    STMT_بیان_MULTIPLY,
    STMT_بیان_DIVIDE,
    STMT_بیان_ټاکل,
    STMT_بیان_NOT,
    STMT_بیان_NEGATE,
    STMT_بیان_INVERT,
    STMT_بیان_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_ډله_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_عدد_ارزښتونه
};

تعداد یوځای_ډېرپېښ {
    یوځای_DEFAULT,
    یوځای_AUGMENT,
    یوځای_OVERRIDE,
    یوځای_REPLACE,
    یوځای_ډېرپېښ_عدد_ENTRIES,
};
تعداد { یوځای_ډېرپېښ_MIN_پلنوالی = 3 /* 2 bits + sign */ };
static_assert(یوځای_ډېرپېښ_عدد_ENTRIES - 1 <= (1 << (یوځای_ډېرپېښ_MIN_پلنوالی - 1)) - 1,
              "Cannot encode merge mode");

ثابت حرف *
xkb_file_type_منزل_string(تعداد xkb_file_type type);

ثابت حرف *
stmt_type_منزل_string(تعداد stmt_type type);

حرف
stmt_type_منزل_operator_char(تعداد stmt_type type);

تعريفنوع بنية _ParseCommon  {
    بنية _ParseCommon *بل;
    تعداد stmt_type type;
} ParseCommon;

تعريفنوع بنية _IncludeStmt {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    حرف *stmt;
    حرف *file;
    حرف *map;
    حرف *modifier;
    بنية _IncludeStmt *بل_incl;
} IncludeStmt;

تعريفنوع اتحاد بیان_Def بیان_Def;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} بیان_Ident;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} بیان_String;

تعريفنوع بنية {
    ParseCommon common;
    bool سېټ;
} بیان_Boolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} بیان_Integer;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} بیان_Float;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t کلی_name;
} بیان_کلی_Name;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} بیان_کلی_Sym;

تعريفنوع بنية {
    ParseCommon common;
    بیان_Def *کیڼ;
    بیان_Def *ښی;
} بیان_دوګونی;

تعريفنوع بنية {
    ParseCommon common;
    بیان_Def *ماشوم;
} بیان_Unary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ډګر_2;
} بیان_ډګر_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ډګر_2;
    بیان_Def *ثبت;
} بیان_Array_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    بیان_Def *دلیلونه;
} بیان_Action;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    بیان_Def *actions;
} بیان_Action_List;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} بیان_Keysym_List;

اتحاد بیان_Def {
    ParseCommon common;
    بیان_Ident ident;
    بیان_String string;
    بیان_Boolean boolean;
    بیان_Integer integer;
    بیان_کلی_Name کلی_name;
    بیان_کلی_Sym keysym;
    بیان_دوګونی دوګونی;
    بیان_Unary unary;
    بیان_ډګر_Ref ډګر_ref;
    بیان_Array_Ref array_ref;
    بیان_Action action;
    بیان_Action_List actions;
    بیان_Keysym_List keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    بیان_Def *name;
    بیان_Def *ارزښت;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_atom_t name;
    بیان_Def *ارزښت;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_atom_t name;
    int64_t ارزښت;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_atom_t alias;
    xkb_atom_t real;
} کلی_Alias_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_atom_t name;
    VarDef *body;
} کلی_Type_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_atom_t کلی_Name;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    // NOTE: Can also be “None”, rather than a modifier name.
    بیان_Def *modifiers;
    بیان_Def *کلیګانې;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    int64_t ډله;
    بیان_Def *def;
} ډله_Compat_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    xkb_keysym_t sym;
    بیان_Def *سمون;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
    bool مجازي;
    int64_t ndx;
    بیان_Def *name;
} LedNameDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد یوځای_ډېرپېښ یوځای;
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
