/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#అన్తర్భావ "atom.h"
#అన్తర్భావ "darray.h"

గణన xkb_file_type {
    /* Component files, by order of compilation. */
    FILE_TYPE_KEYCODES = 0,
    FILE_TYPE_TYPES = 1,
    FILE_TYPE_COMPAT = 2,
    FILE_TYPE_SYMBOLS = 3,

    /* File types which must be found in a keymap file. */
    మొదటి_KEYMAP_FILE_TYPE = FILE_TYPE_KEYCODES,
    చివరి_KEYMAP_FILE_TYPE = FILE_TYPE_SYMBOLS,

    /* Geometry is not compiled any more. */
    FILE_TYPE_GEOMETRY = 4,

    /* A top level file which includes the above files. */
    FILE_TYPE_KEYMAP,


    /* This one doesn't mix with the others, but useful here as well. */
    FILE_TYPE_నియమాలు,

    FILE_TYPE_అంకె_ENTRIES,
    FILE_TYPE_INVALID = FILE_TYPE_అంకె_ENTRIES
};

గణన stmt_type {
    STMT_UNKNOWN = 0,
    STMT_INCLUDE,
    STMT_KEYCODE,
    STMT_ALIAS,
    STMT_సమాసం_STRING_LITERAL,
    STMT_సమాసం_INTEGER_LITERAL,
    STMT_సమాసం_FLOAT_LITERAL,
    STMT_సమాసం_BOOLEAN_LITERAL,
    STMT_సమాసం_KEYNAME_LITERAL,
    STMT_సమాసం_KEYSYM_LITERAL,
    STMT_సమాసం_IDENT,
    STMT_సమాసం_ACTION_DECL,
    STMT_సమాసం_క్షేత్రం_REF,
    STMT_సమాసం_ARRAY_REF,
    /* Needed because of the ambiguity between keysym and action empty lists */
    STMT_సమాసం_ఖాళీలు_LIST,
    STMT_సమాసం_KEYSYM_LIST,
    STMT_సమాసం_ACTION_LIST,
    STMT_సమాసం_కూడు,
    STMT_సమాసం_వ్యవకలనం,
    STMT_సమాసం_MULTIPLY,
    STMT_సమాసం_DIVIDE,
    STMT_సమాసం_కేటాయించు,
    STMT_సమాసం_NOT,
    STMT_సమాసం_NEGATE,
    STMT_సమాసం_INVERT,
    STMT_సమాసం_UNARY_PLUS,
    STMT_VAR,
    STMT_TYPE,
    STMT_INTERP,
    STMT_VMOD,
    STMT_SYMBOLS,
    STMT_MODMAP,
    STMT_సమూహం_COMPAT,
    STMT_LED_MAP,
    STMT_LED_NAME,
    STMT_UNKNOWN_DECLARATION,
    STMT_UNKNOWN_COMPOUND,

    STMT_అంకె_విలువలు
};

గణన విలీనం_బహుళకం {
    విలీనం_DEFAULT,
    విలీనం_AUGMENT,
    విలీనం_OVERRIDE,
    విలీనం_REPLACE,
    విలీనం_బహుళకం_అంకె_ENTRIES,
};
గణన { విలీనం_బహుళకం_MIN_వెడల్పు = 3 /* 2 bits + sign */ };
static_assert(విలీనం_బహుళకం_అంకె_ENTRIES - 1 <= (1 << (విలీనం_బహుళకం_MIN_వెడల్పు - 1)) - 1,
              "Cannot encode merge mode");

స్థిరరాశి అక్షరం *
xkb_file_type_గమ్యం_string(గణన xkb_file_type type);

స్థిరరాశి అక్షరం *
stmt_type_గమ్యం_string(గణన stmt_type type);

అక్షరం
stmt_type_గమ్యం_operator_char(గణన stmt_type type);

రకనిర్వచనం నిర్మాణం _ParseCommon  {
    నిర్మాణం _ParseCommon *తదుపరి;
    గణన stmt_type type;
} ParseCommon;

రకనిర్వచనం నిర్మాణం _IncludeStmt {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    అక్షరం *stmt;
    అక్షరం *file;
    అక్షరం *map;
    అక్షరం *modifier;
    నిర్మాణం _IncludeStmt *తదుపరి_incl;
} IncludeStmt;

రకనిర్వచనం సంఘం సమాసం_Def సమాసం_Def;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t ident;
} సమాసం_Ident;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t str;
} సమాసం_String;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    bool సమితి;
} సమాసం_Boolean;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    int64_t ival;
} సమాసం_Integer;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    /* We don't support floats, but we still represnt them in the AST, in
     * order to provide proper error messages. */
} సమాసం_Float;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t కీలకం_name;
} సమాసం_కీలకం_Name;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    /* Keysyms are resolved early, so this might contain `NoSymbol` in case of
     * failure, to enable error recovery. */
    xkb_keysym_t keysym;
} సమాసం_కీలకం_Sym;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    సమాసం_Def *ఎడమ;
    సమాసం_Def *కుడి;
} సమాసం_ద్విమాన;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    సమాసం_Def *బిడ్డ;
} సమాసం_Unary;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t క్షేత్రం_2;
} సమాసం_క్షేత్రం_Ref;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t element;
    xkb_atom_t క్షేత్రం_2;
    సమాసం_Def *నమోదు_2;
} సమాసం_Array_Ref;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    xkb_atom_t name;
    సమాసం_Def *వాదనలు;
} సమాసం_Action;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    /* List of actions for a single level. */
    సమాసం_Def *actions;
} సమాసం_Action_List;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    /* List of keysym for a single level. */
    darray(xkb_keysym_t) syms;
} సమాసం_Keysym_List;

సంఘం సమాసం_Def {
    ParseCommon common;
    సమాసం_Ident ident;
    సమాసం_String string;
    సమాసం_Boolean boolean;
    సమాసం_Integer integer;
    సమాసం_కీలకం_Name కీలకం_name;
    సమాసం_కీలకం_Sym keysym;
    సమాసం_ద్విమాన ద్విమాన;
    సమాసం_Unary unary;
    సమాసం_క్షేత్రం_Ref క్షేత్రం_ref;
    సమాసం_Array_Ref array_ref;
    సమాసం_Action action;
    సమాసం_Action_List actions;
    సమాసం_Keysym_List keysym_list;
};

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    సమాసం_Def *name;
    సమాసం_Def *విలువ;
} VarDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t name;
    సమాసం_Def *విలువ;
} VModDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t name;
    int64_t విలువ;
} KeycodeDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t alias;
    xkb_atom_t real;
} కీలకం_Alias_Def;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t name;
    VarDef *body;
} కీలకం_Type_Def;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t కీలకం_Name;
    VarDef *symbols;
} SymbolsDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    // NOTE: Can also be “None”, rather than a modifier name.
    సమాసం_Def *modifiers;
    సమాసం_Def *కీలకాలు;
} ModMapDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    int64_t సమూహం;
    సమాసం_Def *def;
} సమూహం_Compat_Def;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_keysym_t sym;
    సమాసం_Def *సరిపోలిక;
    VarDef *def;
} InterpDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    bool మాయా;
    int64_t ndx;
    సమాసం_Def *name;
} LedNameDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    గణన విలీనం_బహుళకం విలీనం;
    xkb_atom_t name;
    VarDef *body;
} LedMapDef;

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    అక్షరం *name;
} UnknownStatement;

గణన xkb_map_flags {
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

రకనిర్వచనం నిర్మాణం {
    ParseCommon common;
    అక్షరం *name;
    ParseCommon *defs;
    గణన xkb_file_type file_type;
    గణన xkb_map_flags flags;
} XkbFile;
