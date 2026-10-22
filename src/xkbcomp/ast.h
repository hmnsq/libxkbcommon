/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#هدايت once

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
    پهريون_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    آخري_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_قاعدا,

    FILE_TYPE_انگ_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_انگ_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_اظهار_STRING_LITERAL,
    STMT_اظهار_INTEGER_LITERAL,
    STMT_اظهار_FLOAT_LITERAL,
    STMT_اظهار_BOOLEAN_LITERAL,
    STMT_اظهار_KEYNAME_LITERAL,
    STMT_اظهار_KEYSYM_LITERAL,
    STMT_اظهار_IDENT,
    STMT_اظهار_ACTION_DECL,
    STMT_اظهار_ميدان_REF,
    STMT_اظهار_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_اظهار_خالي_جايون_LIST,
    STMT_اظهار_KEYSYM_LIST,
    STMT_اظهار_ACTION_LIST,
    STMT_اظهار_جوڙيو,
    STMT_اظهار_ڪٽيو,
    STMT_اظهار_MULTIPLY,
    STMT_اظهار_DIVIDE,
    STMT_اظهار_سونپيو,
    STMT_اظهار_NOT,
    STMT_اظهار_NEGATE,
    STMT_اظهار_INVERT,
    STMT_اظهار_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_گروهه_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_انگ_قدرون
};

تعداد ملايو_گهڻائي {
    ملايو_DEFAULT,
    ملايو_AUGMENT,
    ملايو_OVERRIDE,
    ملايو_REPLACE,
    ملايو_گهڻائي_انگ_ENTRIES,
};
تعداد { ملايو_گهڻائي_MIN_ويڪر = 3 /* 2 bits + sign */ };
static_assert(ملايو_گهڻائي_انگ_ENTRIES - 1 <= (1 << (ملايو_گهڻائي_MIN_ويڪر - 1)) - 1,
              "Cannot encode merge mode");

ثابت حرف *
xkb_file_type_منزل_string(تعداد xkb_file_type type);

ثابت حرف *
stmt_type_منزل_string(تعداد stmt_type type);

حرف
stmt_type_منزل_operator_char(تعداد stmt_type type);

تعريفنوع بنية _ParseCommon  {
    بنية _ParseCommon *ايندڙ;
    تعداد stmt_type type;
} ParseCommon;

تعريفنوع بنية _IncludeStmt {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    حرف *stmt;
    حرف *file;
    حرف *map;
    حرف *modifier;
    بنية _IncludeStmt *ايندڙ_incl;
} IncludeStmt;

تعريفنوع اتحاد اظهار_Def اظهار_Def;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} اظهار_Ident;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} اظهار_String;

تعريفنوع بنية {
    ParseCommon common;
    bool سيٽ;
} اظهار_Boolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} اظهار_Integer;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} اظهار_Float;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ڪنجي_name;
} اظهار_ڪنجي_Name;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} اظهار_ڪنجي_Sym;

تعريفنوع بنية {
    ParseCommon common;
    اظهار_Def *کاٻو;
    اظهار_Def *ساڄو;
} اظهار_ٻٽو;

تعريفنوع بنية {
    ParseCommon common;
    اظهار_Def *ٻار;
} اظهار_Unary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ميدان_2;
} اظهار_ميدان_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t ميدان_2;
    اظهار_Def *داخلا;
} اظهار_Array_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    اظهار_Def *دليلون;
} اظهار_Action;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    اظهار_Def *actions;
} اظهار_Action_List;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} اظهار_Keysym_List;

اتحاد اظهار_Def {
    ParseCommon common;
    اظهار_Ident ident;
    اظهار_String string;
    اظهار_Boolean boolean;
    اظهار_Integer integer;
    اظهار_ڪنجي_Name ڪنجي_name;
    اظهار_ڪنجي_Sym keysym;
    اظهار_ٻٽو ٻٽو;
    اظهار_Unary unary;
    اظهار_ميدان_Ref ميدان_ref;
    اظهار_Array_Ref array_ref;
    اظهار_Action action;
    اظهار_Action_List actions;
    اظهار_Keysym_List keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    اظهار_Def *name;
    اظهار_Def *قدر;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_atom_t name;
    اظهار_Def *قدر;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_atom_t name;
    int64_t قدر;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_atom_t alias;
    xkb_atom_t real;
} ڪنجي_Alias_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_atom_t name;
    VarDef *body;
} ڪنجي_Type_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_atom_t ڪنجي_Name;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    // NOTE: Can also be “None”, rather than a modifier name.
    اظهار_Def *modifiers;
    اظهار_Def *ڪنجيون;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    int64_t گروهه;
    اظهار_Def *def;
} گروهه_Compat_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    xkb_keysym_t sym;
    اظهار_Def *ميلاپ;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
    bool مجازي;
    int64_t ndx;
    اظهار_Def *name;
} LedNameDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ملايو_گهڻائي ملايو;
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
