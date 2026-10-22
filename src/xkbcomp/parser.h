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

#اگر_غیر_تعریف_شدہ YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED
# define YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED
/* Debug traces.  */
#اگر_غیر_تعریف_شدہ YYDEBUG
# define YYDEBUG 0
#اگر_ختم
#اگر YYDEBUG
خارجی صحیح _xkbcommon_debug;
#اگر_ختم
/* "%code requires" blocks.  */
#سطر 14 "src/xkbcomp/parser.y"

#شامل "config.h"

#شامل "scanner-utils.h"
#شامل "xkbcomp/ast.h"
#شامل "xkbcomp/xkbcomp-priv.h"

#سطر 57 "src/xkbcomp/parser.h"

/* Token kinds.  */
#اگر_غیر_تعریف_شدہ YYTOKENTYPE
# define YYTOKENTYPE
  شمار yytokentype
  {
    YYEMPTY = -2,
    انتہا_کا_FILE = 0,               /* "end of file"  */
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
    کلید_3 = 24,                      /* "key"  */
    ALIAS = 25,                    /* "alias"  */
    گروہ_2 = 26,                    /* "group"  */
    MODIFIER_MAP = 27,             /* "modifier_map"  */
    INDICATOR = 28,                /* "indicator"  */
    SHAPE = 29,                    /* "shape"  */
    کلیدیں_2 = 30,                     /* "keys"  */
    صف = 31,                      /* "row"  */
    SECTION = 32,                  /* "section"  */
    OVERLAY = 33,                  /* "overlay"  */
    متن = 34,                     /* "text"  */
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
    ALPHANUMERIC_کلیدیں = 73,        /* "alphanumeric_keys"  */
    MODIFIER_کلیدیں = 74,            /* "modifier_keys"  */
    KEYPAD_کلیدیں = 75,              /* "keypad_keys"  */
    FUNCTION_کلیدیں = 76,            /* "function_keys"  */
    ALTERNATE_گروہ = 77,          /* "alternate_group"  */
    DEPRECATED = 78                /* "deprecated"  */
  };
  قسمتعریف شمار yytokentype yytoken_قسم_t;
#اگر_ختم

/* Value type.  */
#اگر ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
اتحاد YYSTYPE
{
#سطر 177 "src/xkbcomp/parser.y"

        int64_t          عدد;
        شمار xkb_file_type file_type;
        حرف            *str;
        ساخت sval     sval;
        xkb_atom_t      atom;
        شمار ملاؤ_منوال ملاؤ;
        شمار xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ساخت { ParseCommon *سر; ParseCommon *آخری; } anyList;
        uint32_t        no_علامت_Or_Action_List;
        اظہاریہ_Def         *اظہاریہ;
        ساخت { اظہاریہ_Def *سر; اظہاریہ_Def *آخری; } اظہاریہ_List;
        VarDef          *var;
        ساخت { VarDef *سر; VarDef *آخری; } varList;
        VModDef         *vmod;
        ساخت { VModDef *سر; VModDef *آخری; } vmodList;
        InterpDef       *interp;
        کلید_Type_Def      *کلید_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_نقاب_2;
        گروہ_Compat_Def  *گروہ_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *کلید_رمز;
        کلید_Alias_Def     *کلید_Alias;
        UnknownStatement *unknown;
        خالی            *geom;
        XkbFile         *file;
        ساخت { XkbFile *سر; XkbFile *آخری; } fileList;

#سطر 173 "src/xkbcomp/parser.h"

};
قسمتعریف اتحاد YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#اگر_ختم




صحیح _xkbcommon_parse (ساخت parser_param *param);


#اگر_ختم /* !YY__XKBCOMMON_SRC_XKBCOMP_PARSER_H_INCLUDED  */
