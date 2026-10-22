/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#رهنمود once

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
    اولی_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    آخرین_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_قواعد,

    FILE_TYPE_رقم_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_رقم_ENTRIES
};

تعداد stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_عبارت_STRING_LITERAL,
    STMT_عبارت_INTEGER_LITERAL,
    STMT_عبارت_FLOAT_LITERAL,
    STMT_عبارت_BOOLEAN_LITERAL,
    STMT_عبارت_KEYNAME_LITERAL,
    STMT_عبارت_KEYSYM_LITERAL,
    STMT_عبارت_IDENT,
    STMT_عبارت_ACTION_DECL,
    STMT_عبارت_فیلد_REF,
    STMT_عبارت_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_عبارت_خالی_LIST,
    STMT_عبارت_KEYSYM_LIST,
    STMT_عبارت_ACTION_LIST,
    STMT_عبارت_جمع_کن,
    STMT_عبارت_تفریق,
    STMT_عبارت_MULTIPLY,
    STMT_عبارت_DIVIDE,
    STMT_عبارت_تخصیص_بده,
    STMT_عبارت_NOT,
    STMT_عبارت_NEGATE,
    STMT_عبارت_INVERT,
    STMT_عبارت_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_گروه_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_رقم_مقادیر
};

تعداد ادغام_نما {
    ادغام_DEFAULT,
    ادغام_AUGMENT,
    ادغام_OVERRIDE,
    ادغام_REPLACE,
    ادغام_نما_رقم_ENTRIES,
};
تعداد { ادغام_نما_MIN_عرض = 3 /* 2 bits + sign */ };
static_assert(ادغام_نما_رقم_ENTRIES - 1 <= (1 << (ادغام_نما_MIN_عرض - 1)) - 1,
              "Cannot encode merge mode");

ثابت حرف *
xkb_file_type_مقصد_string(تعداد xkb_file_type type);

ثابت حرف *
stmt_type_مقصد_string(تعداد stmt_type type);

حرف
stmt_type_مقصد_operator_char(تعداد stmt_type type);

تعريفنوع بنية _ParseCommon  {
    بنية _ParseCommon *بعدی;
    تعداد stmt_type type;
} ParseCommon;

تعريفنوع بنية _IncludeStmt {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    حرف *stmt;
    حرف *file;
    حرف *map;
    حرف *modifier;
    بنية _IncludeStmt *بعدی_incl;
} IncludeStmt;

تعريفنوع اتحاد عبارت_Def عبارت_Def;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t ident;
} عبارت_Ident;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t str;
} عبارت_String;

تعريفنوع بنية {
    ParseCommon common;
    bool مجموعه;
} عبارت_Boolean;

تعريفنوع بنية {
    ParseCommon common;
    int64_t ival;
} عبارت_Integer;

تعريفنوع بنية {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} عبارت_Float;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t کلید_name;
} عبارت_کلید_Name;

تعريفنوع بنية {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} عبارت_کلید_Sym;

تعريفنوع بنية {
    ParseCommon common;
    عبارت_Def *چپ;
    عبارت_Def *راست;
} عبارت_دودویی;

تعريفنوع بنية {
    ParseCommon common;
    عبارت_Def *فرزند;
} عبارت_Unary;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t فیلد_2;
} عبارت_فیلد_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t فیلد_2;
    عبارت_Def *مدخل;
} عبارت_Array_Ref;

تعريفنوع بنية {
    ParseCommon common;
    xkb_atom_t name;
    عبارت_Def *آرگومانها;
} عبارت_Action;

تعريفنوع بنية {
    ParseCommon common;
    /* List of actions for a single level. */
    عبارت_Def *actions;
} عبارت_Action_List;

تعريفنوع بنية {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} عبارت_Keysym_List;

اتحاد عبارت_Def {
    ParseCommon common;
    عبارت_Ident ident;
    عبارت_String string;
    عبارت_Boolean boolean;
    عبارت_Integer integer;
    عبارت_کلید_Name کلید_name;
    عبارت_کلید_Sym keysym;
    عبارت_دودویی دودویی;
    عبارت_Unary unary;
    عبارت_فیلد_Ref فیلد_ref;
    عبارت_Array_Ref array_ref;
    عبارت_Action action;
    عبارت_Action_List actions;
    عبارت_Keysym_List keysym_list;
};

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    عبارت_Def *name;
    عبارت_Def *مقدار;
} VarDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_atom_t name;
    عبارت_Def *مقدار;
} VModDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_atom_t name;
    int64_t مقدار;
} KeycodeDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_atom_t alias;
    xkb_atom_t real;
} کلید_Alias_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_atom_t name;
    VarDef *body;
} کلید_Type_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_atom_t کلید_Name;
    VarDef *symbols;
} SymbolsDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    // NOTE: Can also be “None”, rather than a modifier name.
    عبارت_Def *modifiers;
    عبارت_Def *کلیدها;
} ModMapDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    int64_t گروه;
    عبارت_Def *def;
} گروه_Compat_Def;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    xkb_keysym_t sym;
    عبارت_Def *تطابق;
    VarDef *def;
} InterpDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
    bool مجازی;
    int64_t ndx;
    عبارت_Def *name;
} LedNameDef;

تعريفنوع بنية {
    ParseCommon common;
    تعداد ادغام_نما ادغام;
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
