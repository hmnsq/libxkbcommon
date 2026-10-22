/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

/* DO NOT EDIT DIRECTLY: This file is generated from src/xkbcomp/parser.y. */

/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#నిర్వచితం_కాకపోతే YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED
# define YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED
/* Debug traces.  */
#నిర్వచితం_కాకపోతే YYDEBUG
# define YYDEBUG 0
#అయితే_ముగింపు
#ఒకవేళ YYDEBUG
బాహ్య పూర్ణాంకం _xkbcommon_debug;
#అయితే_ముగింపు
/* "%code requires" blocks.  */
#పంక్తి 14 "src/xkbcomp/parser.y"

#అన్తర్భావ "config.h"

#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "xkbcomp/ast.h"
#అన్తర్భావ "xkbcomp/xkbcomp-priv.h"

#పంక్తి 57 "src/xkbcomp/parser.h"

/* Token kinds.  */
#నిర్వచితం_కాకపోతే YYTOKENTYPE
# define YYTOKENTYPE
  గణన yytokentype
  {
    YYEMPTY = -2,
    చివర_యొక్క_FILE = 0,               /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ERROR_TOK = 255,               /* "invalid token"  */
    XKB_KEYMAP = 1,                /* "xkb_keymap"  */
    XKB_KEYCODES = 2,              /* "xkb_keycodes"  */
    XKB_TYPES = 3,                 /* "xkb_types"  */
    XKB_SYMBOLS = 4,               /* "xkb_symbols"  */
    XKB_COMPATMAP = 5,             /* "xkb_compatibility"  */
    XKB_GEOMETRY = 6,              /* "xkb_geometry"  */
    XKB_SEMANTICS = 7,             /* "xkb_semantics"  */
    XKB_LAYOUT = 8,                /* "xkb_layout"  */
    INCLUDE = 10,                  /* "include"  */
    OVERRIDE = 11,                 /* "override"  */
    AUGMENT = 12,                  /* "augment"  */
    REPLACE = 13,                  /* "replace"  */
    ALTERNATE = 14,                /* "alternate"  */
    VIRTUAL_MODS = 20,             /* "virtual_modifiers"  */
    TYPE = 21,                     /* "type"  */
    INTERPRET = 22,                /* "interpret"  */
    ACTION_TOK = 23,               /* "action"  */
    కీలకం_3 = 24,                      /* "key"  */
    ALIAS = 25,                    /* "alias"  */
    సమూహం_2 = 26,                    /* "group"  */
    MODIFIER_MAP = 27,             /* "modifier_map"  */
    INDICATOR = 28,                /* "indicator"  */
    SHAPE = 29,                    /* "shape"  */
    కీలకాలు_2 = 30,                     /* "keys"  */
    అడ్డువరుస = 31,                      /* "row"  */
    SECTION = 32,                  /* "section"  */
    OVERLAY = 33,                  /* "overlay"  */
    పాఠ్యం = 34,                     /* "text"  */
    OUTLINE = 35,                  /* "outline"  */
    SOLID = 36,                    /* "solid"  */
    LOGO = 37,                     /* "logo"  */
    VIRTUAL = 38,                  /* "virtual"  */
    EQUALS = 40,                   /* "="  */
    PLUS = 41,                     /* "+"  */
    MINUS = 42,                    /* "-"  */
    DIVIDE = 43,                   /* "/"  */
    TIMES = 44,                    /* "*"  */
    OBRACE = 45,                   /* "{"  */
    CBRACE = 46,                   /* "}"  */
    OPAREN = 47,                   /* "("  */
    CPAREN = 48,                   /* ")"  */
    OBRACKET = 49,                 /* "["  */
    CBRACKET = 50,                 /* "]"  */
    DOT = 51,                      /* "."  */
    COMMA = 52,                    /* ","  */
    SEMI = 53,                     /* ";"  */
    EXCLAM = 54,                   /* "!"  */
    INVERT = 55,                   /* "~"  */
    STRING = 60,                   /* "string literal"  */
    DECIMAL_DIGIT = 61,            /* "decimal digit"  */
    INTEGER = 62,                  /* "integer literal"  */
    FLOAT = 63,                    /* "float literal"  */
    IDENT = 64,                    /* "identifier"  */
    KEYNAME = 65,                  /* "key name"  */
    PARTIAL = 70,                  /* "partial"  */
    DEFAULT = 71,                  /* "default"  */
    HIDDEN = 72,                   /* "hidden"  */
    ALPHANUMERIC_కీలకాలు = 73,        /* "alphanumeric_keys"  */
    MODIFIER_కీలకాలు = 74,            /* "modifier_keys"  */
    KEYPAD_కీలకాలు = 75,              /* "keypad_keys"  */
    FUNCTION_కీలకాలు = 76,            /* "function_keys"  */
    ALTERNATE_సమూహం = 77,          /* "alternate_group"  */
    DEPRECATED = 78                /* "deprecated"  */
  };
  రకనిర్వచనం గణన yytokentype yytoken_రకం_t;
#అయితే_ముగింపు

/* Value type.  */
#ఒకవేళ ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
సంఘం YYSTYPE
{
#పంక్తి 177 "src/xkbcomp/parser.y"

        int64_t          అంకె;
        గణన xkb_file_type file_type;
        అక్షరం            *str;
        నిర్మాణం sval     sval;
        xkb_atom_t      atom;
        గణన విలీనం_బహుళకం విలీనం;
        గణన xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        నిర్మాణం { ParseCommon *తల; ParseCommon *చివరి; } anyList;
        uint32_t        no_చిహ్నం_Or_Action_List;
        సమాసం_Def         *సమాసం;
        నిర్మాణం { సమాసం_Def *తల; సమాసం_Def *చివరి; } సమాసం_List;
        VarDef          *var;
        నిర్మాణం { VarDef *తల; VarDef *చివరి; } varList;
        VModDef         *vmod;
        నిర్మాణం { VModDef *తల; VModDef *చివరి; } vmodList;
        InterpDef       *interp;
        కీలకం_Type_Def      *కీలకం_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_ముసుగు_2;
        సమూహం_Compat_Def  *సమూహం_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *కీలకం_సంకేతం;
        కీలకం_Alias_Def     *కీలకం_Alias;
        UnknownStatement *unknown;
        శూన్య            *geom;
        XkbFile         *file;
        నిర్మాణం { XkbFile *తల; XkbFile *చివరి; } fileList;

#పంక్తి 173 "src/xkbcomp/parser.h"

};
రకనిర్వచనం సంఘం YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#అయితే_ముగింపు




పూర్ణాంకం _xkbcommon_parse (నిర్మాణం parser_param *param);


#అయితే_ముగింపు /* !YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED  */
