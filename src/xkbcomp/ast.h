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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "darray.h"

எண்ணிக்கை xkb_file_type {
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

    FILE_TYPE_இலக்கம்_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_இலக்கம்_ENTRIES
};

எண்ணிக்கை stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_கோவை_STRING_LITERAL,
    STMT_கோவை_INTEGER_LITERAL,
    STMT_கோவை_FLOAT_LITERAL,
    STMT_கோவை_BOOLEAN_LITERAL,
    STMT_கோவை_KEYNAME_LITERAL,
    STMT_கோவை_KEYSYM_LITERAL,
    STMT_கோவை_IDENT,
    STMT_கோவை_ACTION_DECL,
    STMT_கோவை_FIELD_REF,
    STMT_கோவை_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_கோவை_EMPTY_LIST,
    STMT_கோவை_KEYSYM_LIST,
    STMT_கோவை_ACTION_LIST,
    STMT_கோவை_ADD,
    STMT_கோவை_SUBTRACT,
    STMT_கோவை_MULTIPLY,
    STMT_கோவை_DIVIDE,
    STMT_கோவை_ASSIGN,
    STMT_கோவை_NOT,
    STMT_கோவை_NEGATE,
    STMT_கோவை_INVERT,
    STMT_கோவை_UNARY_PLUS,
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

    STMT_இலக்கம்_மதிப்புகள்
};

எண்ணிக்கை merge_mode {
    MERGE_DEFAULT,
    MERGE_AUGMENT,
    MERGE_OVERRIDE,
    MERGE_REPLACE,
    MERGE_MODE_இலக்கம்_ENTRIES,
};
எண்ணிக்கை { MERGE_MODE_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(MERGE_MODE_இலக்கம்_ENTRIES - 1 <= (1 << (MERGE_MODE_MIN_WIDTH - 1)) - 1,
              "Cannot encode merge mode");

மாறிலி எழுத்து *
xkb_file_type_to_string(எண்ணிக்கை xkb_file_type type);

மாறிலி எழுத்து *
stmt_type_to_string(எண்ணிக்கை stmt_type type);

எழுத்து
stmt_type_to_operator_char(எண்ணிக்கை stmt_type type);

வகைவரை கட்டமைப்பு _ParseCommon  {
    கட்டமைப்பு _ParseCommon *next;
    எண்ணிக்கை stmt_type type;
} ParseCommon;

வகைவரை கட்டமைப்பு _IncludeStmt {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    எழுத்து *stmt;
    எழுத்து *file;
    எழுத்து *map;
    எழுத்து *modifier;
    கட்டமைப்பு _IncludeStmt *next_incl;
} IncludeStmt;

வகைவரை ஒன்றிப்பு கோவை_Def கோவை_Def;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t ident;
} கோவை_Ident;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t str;
} கோவை_String;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    bool set;
} கோவை_Boolean;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    int64_t ival;
} கோவை_Integer;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} கோவை_Float;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t சாவி_name;
} கோவை_சாவி_Name;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} கோவை_சாவி_Sym;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    கோவை_Def *இடது;
    கோவை_Def *வலது;
} கோவை_இருமம்;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    கோவை_Def *child;
} கோவை_Unary;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
} கோவை_Field_Ref;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t field;
    கோவை_Def *entry;
} கோவை_Array_Ref;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    xkb_atom_t name;
    கோவை_Def *args;
} கோவை_Action;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    /* List of actions for a single level. */
    கோவை_Def *actions;
} கோவை_Action_List;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} கோவை_Keysym_List;

ஒன்றிப்பு கோவை_Def {
    ParseCommon common;
    கோவை_Ident ident;
    கோவை_String string;
    கோவை_Boolean boolean;
    கோவை_Integer integer;
    கோவை_சாவி_Name சாவி_name;
    கோவை_சாவி_Sym keysym;
    கோவை_இருமம் இருமம்;
    கோவை_Unary unary;
    கோவை_Field_Ref field_ref;
    கோவை_Array_Ref array_ref;
    கோவை_Action action;
    கோவை_Action_List actions;
    கோவை_Keysym_List keysym_list;
};

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    கோவை_Def *name;
    கோவை_Def *மதிப்பு;
} VarDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t name;
    கோவை_Def *மதிப்பு;
} VModDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t name;
    int64_t மதிப்பு;
} KeycodeDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t alias;
    xkb_atom_t real;
} சாவி_Alias_Def;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} சாவி_Type_Def;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t சாவி_Name;
    VarDef *symbols;
} SymbolsDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    // NOTE: Can also be “None”, rather than a modifier name.
    கோவை_Def *modifiers;
    கோவை_Def *சாவிகள்;
} ModMapDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    int64_t group;
    கோவை_Def *def;
} GroupCompatDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_keysym_t sym;
    கோவை_Def *match;
    VarDef *def;
} InterpDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    bool மெய்நிகர்;
    int64_t ndx;
    கோவை_Def *name;
} LedNameDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எண்ணிக்கை merge_mode merge;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எழுத்து *name;
} UnknownStatement;

எண்ணிக்கை xkb_map_flags {
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

வகைவரை கட்டமைப்பு {
    ParseCommon common;
    எழுத்து *name;
    ParseCommon *defs;
    எண்ணிக்கை xkb_file_type file_type;
    எண்ணிக்கை xkb_map_flags flags;
} XkbFile;
