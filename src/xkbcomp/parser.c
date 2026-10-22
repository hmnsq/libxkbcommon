/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

/* DO NOT EDIT DIRECTLY: This file is generated from src/xkbcomp/parser.y. */

/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#నిర్వచించు YYBISON 30802

/* Bison version string.  */
#నిర్వచించు YYBISON_సంస్కరణ "3.8.2"

/* Skeleton name.  */
#నిర్వచించు YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#నిర్వచించు YYPURE 1

/* Push parsers.  */
#నిర్వచించు YYPUSH 0

/* Pull parsers.  */
#నిర్వచించు YYPULL 1


/* Substitute the variable and function names.  */
#నిర్వచించు yyparse         _xkbcommon_parse
#నిర్వచించు yylex           _xkbcommon_lex
#నిర్వచించు yyerror         _xkbcommon_error
#నిర్వచించు yydebug         _xkbcommon_debug
#నిర్వచించు yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#పంక్తి 22 "src/xkbcomp/parser.y"

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "keysym.h"
#అన్తర్భావ "xkbcomp/xkbcomp-priv.h"
#అన్తర్భావ "xkbcomp/parser-priv.h"
#అన్తర్భావ "xkbcomp/ast-build.h"

నిర్మాణం parser_param {
    నిర్మాణం xkb_context *ctx;
    నిర్మాణం scanner *scanner;
    XkbFile *rtrn;
    నిర్మాణం parser_keymap_config config;
    bool మరిన్ని_maps;
};

#నిర్వచించు parser_log_with_సంకేతం(param, level, verbosity, log_సందేశం_id, fmt, ...)   \
    scanner_log_with_సంకేతం((param)->scanner, level, verbosity, log_సందేశం_id, fmt,\
                          ##__VA_ARGS__)

#నిర్వచించు parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#నిర్వచించు parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#నిర్వచించు parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

స్థిర శూన్య
_xkbcommon_error(నిర్మాణం parser_param *param, స్థిరరాశి అక్షరం *సందేశం)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", సందేశం);
}

స్థిర bool
resolve_keysym(నిర్మాణం parser_param *param, నిర్మాణం sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ఒకవేళ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_కీలకం_No_చిహ్నం;
        తిరిగి true;
    }

    ఒకవేళ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_కీలకం_Void_చిహ్నం;
        తిరిగి true;
    }

    /* xkb_keysym_from_name needs a C string. */
    అక్షరం buf[XKB_KEYSYM_NAME_MAX_కొలత];
    ఒకవేళ (name.len >= పరిమాణం(buf)) {
        తిరిగి false;
    }
    జ్ఞాపకం_నకలు(buf, name.ప్రారంభం, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ఒకవేళ (sym != XKB_కీలకం_No_చిహ్నం) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        తిరిగి true;
    }

    తిరిగి false;
}

#నిర్వచించు param_scanner param->scanner

#పంక్తి 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  లేకపోతే
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  ఒకవేళ defined __cplusplus
#   ఒకవేళ 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   లేకపోతే
#    define YY_NULLPTR 0
#   endif
#  లేకపోతే
#   define YY_NULLPTR ((శూన్య*)0)
#  endif
# endif

#అన్తర్భావ "parser.h"
/* Symbol kind.  */
గణన yysymbol_రకం_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ERROR_TOK = 3,                  /* "invalid token"  */
  YYSYMBOL_XKB_KEYMAP = 4,                 /* "xkb_keymap"  */
  YYSYMBOL_XKB_KEYCODES = 5,               /* "xkb_keycodes"  */
  YYSYMBOL_XKB_TYPES = 6,                  /* "xkb_types"  */
  YYSYMBOL_XKB_SYMBOLS = 7,                /* "xkb_symbols"  */
  YYSYMBOL_XKB_COMPATMAP = 8,              /* "xkb_compatibility"  */
  YYSYMBOL_XKB_GEOMETRY = 9,               /* "xkb_geometry"  */
  YYSYMBOL_XKB_SEMANTICS = 10,             /* "xkb_semantics"  */
  YYSYMBOL_XKB_LAYOUT = 11,                /* "xkb_layout"  */
  YYSYMBOL_INCLUDE = 12,                   /* "include"  */
  YYSYMBOL_OVERRIDE = 13,                  /* "override"  */
  YYSYMBOL_AUGMENT = 14,                   /* "augment"  */
  YYSYMBOL_REPLACE = 15,                   /* "replace"  */
  YYSYMBOL_ALTERNATE = 16,                 /* "alternate"  */
  YYSYMBOL_VIRTUAL_MODS = 17,              /* "virtual_modifiers"  */
  YYSYMBOL_TYPE = 18,                      /* "type"  */
  YYSYMBOL_INTERPRET = 19,                 /* "interpret"  */
  YYSYMBOL_ACTION_TOK = 20,                /* "action"  */
  YYSYMBOL_కీలకం = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_సమూహం = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_కీలకాలు = 27,                      /* "keys"  */
  YYSYMBOL_అడ్డువరుస = 28,                       /* "row"  */
  YYSYMBOL_SECTION = 29,                   /* "section"  */
  YYSYMBOL_OVERLAY = 30,                   /* "overlay"  */
  YYSYMBOL_పాఠ్యం = 31,                      /* "text"  */
  YYSYMBOL_OUTLINE = 32,                   /* "outline"  */
  YYSYMBOL_SOLID = 33,                     /* "solid"  */
  YYSYMBOL_LOGO = 34,                      /* "logo"  */
  YYSYMBOL_VIRTUAL = 35,                   /* "virtual"  */
  YYSYMBOL_EQUALS = 36,                    /* "="  */
  YYSYMBOL_PLUS = 37,                      /* "+"  */
  YYSYMBOL_MINUS = 38,                     /* "-"  */
  YYSYMBOL_DIVIDE = 39,                    /* "/"  */
  YYSYMBOL_TIMES = 40,                     /* "*"  */
  YYSYMBOL_OBRACE = 41,                    /* "{"  */
  YYSYMBOL_CBRACE = 42,                    /* "}"  */
  YYSYMBOL_OPAREN = 43,                    /* "("  */
  YYSYMBOL_CPAREN = 44,                    /* ")"  */
  YYSYMBOL_OBRACKET = 45,                  /* "["  */
  YYSYMBOL_CBRACKET = 46,                  /* "]"  */
  YYSYMBOL_DOT = 47,                       /* "."  */
  YYSYMBOL_COMMA = 48,                     /* ","  */
  YYSYMBOL_SEMI = 49,                      /* ";"  */
  YYSYMBOL_EXCLAM = 50,                    /* "!"  */
  YYSYMBOL_INVERT = 51,                    /* "~"  */
  YYSYMBOL_STRING = 52,                    /* "string literal"  */
  YYSYMBOL_DECIMAL_DIGIT = 53,             /* "decimal digit"  */
  YYSYMBOL_INTEGER = 54,                   /* "integer literal"  */
  YYSYMBOL_FLOAT = 55,                     /* "float literal"  */
  YYSYMBOL_IDENT = 56,                     /* "identifier"  */
  YYSYMBOL_KEYNAME = 57,                   /* "key name"  */
  YYSYMBOL_PARTIAL = 58,                   /* "partial"  */
  YYSYMBOL_DEFAULT = 59,                   /* "default"  */
  YYSYMBOL_HIDDEN = 60,                    /* "hidden"  */
  YYSYMBOL_ALPHANUMERIC_కీలకాలు = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_కీలకాలు = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_కీలకాలు = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_కీలకాలు = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_సమూహం = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_Xkb_సంయుక్త_Map = 69,           /* XkbCompositeMap  */
  YYSYMBOL_Xkb_సంయుక్త_Type = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_కీలకం_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_కీలకం_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_సరిపోలిక = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_కీలకం_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_కీలకం_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_చిహ్నం_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_సమూహం_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_కీలకం_Or_కీలకం_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_కీలకం_Or_కీలకం_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_Section_Body_అంశం = 106,          /* SectionBodyItem  */
  YYSYMBOL_అడ్డువరుస_Body = 107,                  /* RowBody  */
  YYSYMBOL_అడ్డువరుస_Body_అంశం = 108,              /* RowBodyItem  */
  YYSYMBOL_కీలకాలు_2 = 109,                     /*కీలకాలు*/
  YYSYMBOL_కీలకం_2 = 110,                      /*కీలకం*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_కీలకం_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_కీలకం = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_లోపల_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_క్షేత్రం_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_విలీనం_బహుళకం = 122,             /* OptMergeMode  */
  YYSYMBOL_విలీనం_బహుళకం = 123,                /* MergeMode  */
  YYSYMBOL_సమాసం_List = 124,                 /* ExprList  */
  YYSYMBOL_సమాసం = 125,                     /*సమాసం*/
  YYSYMBOL_పదం = 126,                     /*పదం*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_ఖాళీలు_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_కీలకం_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_కీలకం_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_ఖాళీలు_కీలకం_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_కీలకం_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_కీలకం_Sym = 139,                   /* KeySym  */
  YYSYMBOL_కీలకం_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_సంఖ్య = 141,             /* SignedNumber  */
  YYSYMBOL_సంఖ్య = 142,                   /*సంఖ్య*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_కీలకం_సంకేతం = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
రకనిర్వచనం గణన yysymbol_రకం_t yysymbol_రకం_t;




#నిర్వచితమైతే పొట్టి
# undef పొట్టి
#అయితే_ముగింపు

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#నిర్వచితం_కాకపోతే __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# ఒకవేళ defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#అయితే_ముగింపు

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#నిర్వచితమైతే __INT_LEAST8_MAX__
రకనిర్వచనం __INT_LEAST8_TYPE__ yytype_int8;
#లేదావేళ defined YY_STDINT_H
రకనిర్వచనం int_least8_t yytype_int8;
#లేకపోతే
రకనిర్వచనం గుర్తుగల అక్షరం yytype_int8;
#అయితే_ముగింపు

#నిర్వచితమైతే __INT_LEAST16_MAX__
రకనిర్వచనం __INT_LEAST16_TYPE__ yytype_int16;
#లేదావేళ defined YY_STDINT_H
రకనిర్వచనం int_least16_t yytype_int16;
#లేకపోతే
రకనిర్వచనం పొట్టి yytype_int16;
#అయితే_ముగింపు

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#నిర్వచితమైతే __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#అయితే_ముగింపు

#ఒకవేళ defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
రకనిర్వచనం __UINT_LEAST8_TYPE__ yytype_uint8;
#లేదావేళ (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= గరిష్ఠ_పూర్ణాంకం)
రకనిర్వచనం uint_least8_t yytype_uint8;
#లేదావేళ !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= గరిష్ఠ_పూర్ణాంకం
రకనిర్వచనం గుర్తులేని అక్షరం yytype_uint8;
#లేకపోతే
రకనిర్వచనం పొట్టి yytype_uint8;
#అయితే_ముగింపు

#ఒకవేళ defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
రకనిర్వచనం __UINT_LEAST16_TYPE__ yytype_uint16;
#లేదావేళ (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= గరిష్ఠ_పూర్ణాంకం)
రకనిర్వచనం uint_least16_t yytype_uint16;
#లేదావేళ !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= గరిష్ఠ_పూర్ణాంకం
రకనిర్వచనం గుర్తులేని పొట్టి yytype_uint16;
#లేకపోతే
రకనిర్వచనం పూర్ణాంకం yytype_uint16;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే YYPTRDIFF_T
# ఒకవేళ defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_గరిష్ఠ __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_గరిష్ఠ PTRDIFF_MAX
# లేకపోతే
#  define YYPTRDIFF_T దీర్ఘ
#  define YYPTRDIFF_గరిష్ఠ LONG_MAX
# endif
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined కొలత_t
#  define YYSIZE_T కొలత_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T కొలత_t
# లేకపోతే
#  define YYSIZE_T గుర్తులేని
# endif
#అయితే_ముగింపు

#నిర్వచించు YYSIZE_గరిష్ఠ                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_గరిష్ఠ < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_గరిష్ఠ                         \
            : YY_CAST (YYSIZE_T, -1)))

#నిర్వచించు YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, పరిమాణం (X))


/* Stored state numbers (used for stacks). */
రకనిర్వచనం yytype_int16 yy_state_t;

/* State numbers in computations.  */
రకనిర్వచనం పూర్ణాంకం yy_state_fast_t;

#నిర్వచితం_కాకపోతే YY_
# ఒకవేళ defined YYENABLE_NLS && YYENABLE_NLS
#  ఒకవేళ ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే YY_ATTRIBUTE_శుద్ధ
# ఒకవేళ defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_శుద్ధ __attribute__ ((__pure__))
# లేకపోతే
#  define YY_ATTRIBUTE_శుద్ధ
# endif
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే YY_ATTRIBUTE_వాడని
# ఒకవేళ defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_వాడని __attribute__ ((__unused__))
# లేకపోతే
#  define YY_ATTRIBUTE_వాడని
# endif
#అయితే_ముగింపు

/* Suppress unused-variable warnings by "using" E.  */
#ఒకవేళ ! defined lint || defined __GNUC__
# define YY_USE(E) ((శూన్య) (E))
#లేకపోతే
# define YY_USE(E) /*ఖాళీలు*/
#అయితే_ముగింపు

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#ఒకవేళ defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# ఒకవేళ __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# లేకపోతే
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_చివర      \
    _Pragma ("GCC diagnostic pop")
#లేకపోతే
# define YY_INITIAL_విలువ(విలువ_2) విలువ_2
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_చివర
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే YY_INITIAL_విలువ
# define YY_INITIAL_విలువ(విలువ_2) /* Nothing. */
#అయితే_ముగింపు

#ఒకవేళ defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_చివర            \
    _Pragma ("GCC diagnostic pop")
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_చివర
#అయితే_ముగింపు


#నిర్వచించు YY_ASSERT(E) ((శూన్య) (0 && (E)))

#ఒకవేళ 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  ఒకవేళ YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   లేకపోతే
#    define YYSTACK_ALLOC alloca
#    ఒకవేళ ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) చేయి { /*ఖాళీలు*/; } అయితే (0)
#  ifndef YYSTACK_ALLOC_గరిష్ఠ
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_గరిష్ఠ 4032 /* reasonable circa 2006 */
#  endif
# లేకపోతే
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_గరిష్ఠ
#   define YYSTACK_ALLOC_గరిష్ఠ YYSIZE_గరిష్ఠ
#  endif
#  ఒకవేళ (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined జ్ఞాపకం_ఇవ్వు) \
             && (defined YYFREE || defined విడుదల)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC జ్ఞాపకం_ఇవ్వు
#   ఒకవేళ ! defined జ్ఞాపకం_ఇవ్వు && ! defined EXIT_SUCCESS
శూన్య *జ్ఞాపకం_ఇవ్వు (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE విడుదల
#   ఒకవేళ ! defined విడుదల && ! defined EXIT_SUCCESS
శూన్య విడుదల (శూన్య *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#అయితే_ముగింపు /* 1 */

#ఒకవేళ (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
సంఘం yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_గరిష్ఠ (YYSIZEOF (సంఘం yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_బైట్లు(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_గరిష్ఠ)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(దొంతర_alloc, దొంతర_2)                           \
    చేయి                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->దొంతర_alloc, దొంతర_2, yysize);                    \
        దొంతర_2 = &yyptr->దొంతర_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*దొంతర_2) + YYSTACK_GAP_గరిష్ఠ; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    అయితే (0)

#అయితే_ముగింపు

#ఒకవేళ defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  ఒకవేళ defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, లెక్క_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (లెక్క_2)) * పరిమాణం (*(Src)))
#  లేకపోతే
#   define YYCOPY(Dst, Src, లెక్క_2)              \
      చేయి                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          కొరకు (yyi = 0; yyi < (లెక్క_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      అయితే (0)
#  endif
# endif
#అయితే_ముగింపు /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#నిర్వచించు YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#నిర్వచించు YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#నిర్వచించు YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#నిర్వచించు YYNNTS  83
/* YYNRULES -- Number of rules.  */
#నిర్వచించు YYNRULES  221
/* YYNSTATES -- Number of states.  */
#నిర్వచించు YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#నిర్వచించు YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#నిర్వచించు YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_రకం_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
స్థిర స్థిరరాశి yytype_int8 yytranslate[] =
{
       0,     4,     5,     6,     7,     8,     9,    10,    11,     2,
      12,    13,    14,    15,    16,     2,     2,     2,     2,     2,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,     2,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     2,     2,     2,     2,
      52,    53,    54,    55,    56,    57,     2,     2,     2,     2,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     1,     2
};

#ఒకవేళ YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
స్థిర స్థిరరాశి yytype_int16 yyrline[] =
{
       0,   275,   275,   277,   279,   283,   289,   290,   291,   297,
     309,   312,   325,   326,   327,   328,   329,   332,   333,   336,
     337,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   366,   381,   391,   394,   400,   405,   410,   415,   420,
     425,   430,   435,   440,   445,   446,   447,   448,   450,   452,
     459,   461,   463,   467,   471,   475,   479,   481,   485,   487,
     491,   497,   499,   503,   515,   518,   524,   530,   531,   534,
     536,   540,   541,   542,   543,   544,   559,   561,   579,   581,
     603,   609,   611,   613,   616,   620,   637,   639,   643,   645,
     649,   653,   655,   659,   668,   676,   678,   682,   686,   687,
     690,   692,   694,   696,   698,   702,   703,   706,   707,   711,
     712,   715,   717,   721,   725,   726,   729,   732,   734,   738,
     740,   742,   746,   748,   752,   756,   760,   761,   762,   763,
     766,   767,   770,   772,   774,   776,   778,   780,   782,   784,
     786,   788,   790,   794,   795,   798,   799,   800,   801,   802,
     814,   826,   828,   831,   833,   835,   837,   839,   841,   845,
     847,   849,   851,   853,   855,   857,   859,   861,   865,   871,
     873,   875,   879,   881,   885,   889,   891,   895,   899,   901,
     903,   905,   909,   911,   914,   916,   918,   920,   924,   930,
     932,   934,   938,   940,   947,   953,   965,   967,   979,   981,
     985,   987,   996,  1009,  1010,  1019,  1079,  1080,  1083,  1084,
    1085,  1088,  1091,  1092,  1095,  1096,  1099,  1100,  1103,  1106,
    1107,  1110
};
#అయితే_ముగింపు

/** Accessing symbol of state STATE.  */
#నిర్వచించు YY_ACCESSING_చిహ్నం(State) YY_CAST (yysymbol_రకం_t, yystos[State])

#ఒకవేళ 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
స్థిర స్థిరరాశి అక్షరం *yysymbol_name (yysymbol_రకం_t yysymbol) YY_ATTRIBUTE_వాడని;

స్థిర స్థిరరాశి అక్షరం *
yysymbol_name (yysymbol_రకం_t yysymbol)
{
  స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి yy_sname[] =
  {
  "end of file", "error", "invalid token", "invalid token", "xkb_keymap",
  "xkb_keycodes", "xkb_types", "xkb_symbols", "xkb_compatibility",
  "xkb_geometry", "xkb_semantics", "xkb_layout", "include", "override",
  "augment", "replace", "alternate", "virtual_modifiers", "type",
  "interpret", "action", "key", "alias", "group", "modifier_map",
  "indicator", "shape", "keys", "row", "section", "overlay", "text",
  "outline", "solid", "logo", "virtual", "=", "+", "-", "/", "*", "{", "}",
  "(", ")", "[", "]", ".", ",", ";", "!", "~", "string literal",
  "decimal digit", "integer literal", "float literal", "identifier",
  "key name", "partial", "default", "hidden", "alphanumeric_keys",
  "modifier_keys", "keypad_keys", "function_keys", "alternate_group",
  "deprecated", "$accept", "XkbFile", "XkbCompositeMap",
  "XkbCompositeType", "XkbMapConfigList", "XkbMapConfig", "FileType",
  "OptFlags", "Flags", "Flag", "DeclList", "Decl", "VarDecl",
  "KeyNameDecl", "KeyAliasDecl", "VModDecl", "VModDefList", "VModDef",
  "InterpretDecl", "InterpretMatch", "VarDeclList", "KeyTypeDecl",
  "SymbolsDecl", "OptSymbolsBody", "SymbolsBody", "SymbolsVarDecl",
  "MultiKeySymOrActionList", "NoSymbolOrActionList", "GroupCompatDecl",
  "ModMapDecl", "KeyOrKeySymList", "KeyOrKeySym", "LedMapDecl",
  "LedNameDecl", "UnknownDecl", "UnknownCompoundStatementDecl",
  "ShapeDecl", "SectionDecl", "SectionBody", "SectionBodyItem", "RowBody",
  "RowBodyItem", "Keys", "Key", "OverlayDecl", "OverlayKeyList",
  "OverlayKey", "OutlineList", "OutlineInList", "CoordList", "Coord",
  "DoodadDecl", "DoodadType", "FieldSpec", "Element", "OptMergeMode",
  "MergeMode", "ExprList", "Expr", "Term", "MultiActionList", "ActionList",
  "NonEmptyActions", "Actions", "Action", "Lhs", "OptTerminal", "Terminal",
  "MultiKeySymList", "KeySymList", "NonEmptyKeySyms", "KeySyms", "KeySym",
  "KeySymLit", "SignedNumber", "Number", "Float", "Integer", "KeyCode",
  "Ident", "String", "OptMapName", "MapName", YY_NULLPTR
  };
  తిరిగి yy_sname[yysymbol];
}
#అయితే_ముగింపు

#నిర్వచించు YYPACT_NINF (-225)

#నిర్వచించు yypact_విలువ_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#నిర్వచించు YYTABLE_NINF (-217)

#నిర్వచించు yytable_విలువ_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
స్థిర స్థిరరాశి yytype_int16 yypact[] =
{
     113,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,    26,  -225,  -225,   530,   537,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,    -2,    -2,  -225,
    -225,    42,  -225,    76,  -225,  -225,   241,    18,    10,  -225,
     429,  -225,  -225,  -225,  -225,  -225,    48,  -225,    23,    75,
    -225,  -225,   140,    88,    79,  -225,    96,   100,   114,   348,
     309,    88,  -225,    88,   161,  -225,  -225,  -225,   167,   140,
     203,   180,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,    88,
     141,  -225,   190,   200,  -225,  -225,   -36,  -225,   215,  -225,
     218,  -225,  -225,  -225,  -225,  -225,   228,   242,  -225,   244,
     258,  -225,  -225,   278,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,   348,   348,   843,   348,   348,   348,  -225,
    -225,   313,   430,  -225,  -225,  -225,   283,  -225,  -225,  -225,
    -225,  -225,   286,   247,   290,   291,   114,   279,   297,   303,
     179,   300,   348,   886,   348,  -225,   140,  -225,   348,  -225,
    -225,   348,   657,   293,   348,  -225,  -225,  -225,  -225,   314,
      52,  -225,   385,  -225,  -225,   348,   348,   348,   348,   348,
      72,   348,   348,  -225,   -21,   474,   334,  -225,  -225,   348,
    -225,  -225,   326,  -225,   144,   333,   -22,  -225,   438,   672,
     714,   438,   491,   140,   338,   339,  -225,  -225,   352,    56,
     341,   235,   348,  -225,   886,  -225,   -23,   438,   269,   269,
    -225,  -225,  -225,    94,  -225,  -225,   438,   287,   729,   347,
      37,    97,  -225,    99,  -225,   376,    88,   379,    88,  -225,
    -225,   432,  -225,  -225,  -225,   348,   771,   315,  -225,   786,
    -225,   348,  -225,   344,  -225,   368,   383,   533,  -225,   387,
     158,   181,  -225,  -225,   191,  -225,  -225,  -225,   399,   657,
     292,  -225,  -225,    68,  -225,  -225,   348,   405,    72,  -225,
     410,   107,   329,  -225,  -225,  -225,   384,  -225,   423,   -37,
     424,   347,   390,    43,   398,   431,  -225,   357,   434,  -225,
     435,   252,  -225,  -225,  -225,  -225,   108,  -225,  -225,   553,
    -225,   872,  -225,    66,  -225,  -225,  -225,   438,  -225,   438,
    -225,  -225,  -225,  -225,  -225,    37,  -225,  -225,  -225,  -225,
     828,   438,   440,  -225,   615,  -225,   422,  -225,  -225,  -225,
    -225,  -225,  -225,   166,   595,   194,   216,  -225,  -225,   182,
    -225,  -225,  -225,   439,   143,    46,   437,  -225,   451,   155,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,   348,
    -225,   175,  -225,  -225,   433,   447,   422,   183,   452,    46,
    -225,  -225,  -225,  -225,  -225,  -225
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
స్థిర స్థిరరాశి yytype_uint8 yydefact[] =
{
      18,     4,    30,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     0,     2,     3,     0,    17,    20,     1,     6,
      12,    13,    15,    14,    16,     7,     8,   220,   220,    19,
     221,     0,   219,     0,    10,    33,    18,   144,     0,     9,
       0,   145,   147,   146,   148,   149,     0,    31,     0,   143,
       5,    11,     0,   134,   133,   132,   135,     0,   136,   137,
     138,   139,   140,   141,   142,   127,   128,   129,     0,     0,
     216,     0,   217,    34,    36,    37,    32,    35,    38,    39,
      41,    40,    42,    43,    47,    48,    44,    45,    46,     0,
     178,   131,     0,   130,    49,   216,     0,    57,    58,   218,
       0,   203,   201,   204,   205,   202,     0,    62,   200,     0,
       0,   213,   212,     0,   134,   133,   135,   136,   137,   138,
     139,   141,   142,     0,     0,     0,     0,     0,     0,   211,
     187,   178,     0,   158,   175,   165,   163,   166,   186,   185,
     130,   184,     0,     0,     0,     0,     0,     0,     0,   182,
       0,     0,     0,     0,     0,    51,     0,    55,     0,    64,
      64,     0,    68,     0,     0,   160,   163,   159,   176,     0,
       0,   173,     0,   161,   162,   152,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,    52,    64,     0,
     215,   214,     0,    64,     0,   179,     0,    56,    59,     0,
       0,    61,    83,     0,     0,    67,    70,    75,     0,   130,
       0,     0,   152,   174,     0,   167,     0,   151,   154,   155,
     153,   156,    88,     0,    87,    89,   157,     0,     0,     0,
       0,     0,   118,     0,   123,     0,   138,   140,     0,   101,
     103,     0,    99,   104,   102,     0,     0,     0,    53,     0,
     180,     0,    50,     0,    63,     0,   203,     0,   197,   202,
       0,     0,   171,   170,     0,   191,   190,    74,     0,     0,
       0,    54,    84,     0,   172,   164,     0,     0,     0,    91,
       0,     0,     0,   209,   210,   208,     0,   207,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    93,
       0,     0,    65,    60,    82,   195,     0,   194,    80,     0,
      78,     0,    76,     0,    66,    69,    72,    71,   177,   150,
      85,    86,    90,   119,   206,     0,    95,   117,    96,   122,
       0,   121,     0,   108,     0,   106,     0,    97,    92,    94,
     125,   181,   196,     0,     0,     0,     0,   169,   168,     0,
     198,   189,   188,     0,     0,     0,     0,   105,     0,     0,
     115,   193,   192,    81,    79,    77,   199,   124,   120,   152,
     111,     0,   110,   100,     0,     0,     0,     0,     0,     0,
     116,   113,   114,   112,   107,   109
};

/* YYPGOTO[NTERM-NUM].  */
స్థిర స్థిరరాశి yytype_int16 yypgoto[] =
{
    -225,  -225,  -225,  -225,  -225,   453,  -225,   477,  -225,   502,
    -225,  -225,   -47,  -225,  -225,  -225,  -225,   365,  -225,  -225,
    -107,  -225,  -225,  -225,  -225,   254,   255,  -225,  -225,  -225,
    -225,   248,   479,  -225,  -225,  -225,  -225,  -225,  -225,   288,
    -225,   197,  -225,   149,  -225,  -225,   170,  -225,   253,  -224,
     257,   501,  -225,   -48,  -225,  -225,  -225,  -206,    89,   153,
     246,  -225,  -199,   249,  -200,   -39,  -225,   493,   256,  -225,
     267,  -225,   512,  -179,   243,   285,  -225,   -50,  -225,   -41,
     -24,   541,  -225
};

/* YYDEFGOTO[NTERM-NUM].  */
స్థిర స్థిరరాశి yytype_int16 yydefgoto[] =
{
       0,    12,    13,    27,    36,    14,    28,    15,    16,    17,
      37,    47,   254,    74,    75,    76,    96,    97,    77,   106,
     199,    78,    79,   204,   205,   206,   207,   260,    80,    81,
     223,   224,   240,    83,    84,    85,    86,    87,   241,   242,
     334,   335,   371,   372,   243,   359,   360,   231,   232,   233,
     234,   244,    89,   131,    91,    48,    49,   216,   217,   133,
     261,   170,   134,   135,   171,   136,   148,   137,   264,   306,
     265,   351,   225,   108,   286,   287,   138,   139,   192,   140,
     141,    31,    32
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
స్థిర స్థిరరాశి yytype_int16 yytable[] =
{
      90,    73,   263,   262,   229,   281,   273,    93,   113,    92,
     142,    98,   156,   157,   274,   176,   177,   178,   179,    95,
     229,   275,    72,   266,   230,   276,    18,   252,   147,   100,
      41,    42,    43,    44,    45,    95,   143,   144,    72,   145,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      30,    62,    63,   200,    64,    65,    66,    67,    68,    50,
      46,   114,   115,    55,   116,   151,   117,   118,   119,   120,
     332,    62,   121,    69,   122,   282,   228,   169,   307,    70,
      71,   246,    72,    34,   166,   166,   249,   369,   166,   166,
     283,   284,   285,    69,   213,   101,   186,    51,   -73,    95,
     214,   101,    72,   370,   -73,   195,   354,   349,   101,   263,
     262,   348,   318,     1,    90,    98,   276,    35,   258,   103,
     104,   209,   105,   208,   102,   103,   104,    94,   105,   222,
     266,   102,   103,   104,   352,   105,   277,    90,   239,   288,
      99,   290,   278,   235,    93,   289,    92,   291,   132,   323,
     342,    90,    90,   109,   169,   291,   343,   110,    93,    93,
      92,    92,   267,   377,   362,   307,   169,   111,   112,     2,
     307,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      90,   176,   177,   178,   179,   368,   152,    93,   153,    92,
     250,   291,   146,    90,   239,   101,    95,   375,    90,    72,
      93,    90,    92,   376,   308,    93,   309,    92,    93,   169,
      92,   101,   143,  -126,   294,   172,   150,   378,   361,   103,
     104,    90,   105,   379,   366,   383,   154,   310,   209,   311,
     208,   276,   190,   191,   305,   103,   104,   312,   105,   313,
     364,   194,   311,   196,  -183,    90,   333,   198,   235,   155,
     201,   158,    93,   211,    92,    99,   111,   112,   129,   159,
     130,   169,   365,   169,   313,   218,   219,   220,   221,   160,
     226,   227,   176,   177,   178,   179,   165,   167,   247,   161,
     173,   174,   169,    38,   272,   162,    90,   333,   183,   176,
     177,   178,   179,    93,   163,    92,   169,     2,   341,     3,
       4,     5,     6,     7,     8,     9,    10,    11,   178,   179,
     114,   115,    55,   116,   164,   117,   118,   119,   120,   181,
      62,   121,   182,   122,   176,   177,   178,   179,   187,   123,
     124,   184,   185,   125,   297,   126,   279,   202,   188,   189,
     301,   193,   127,   128,    99,   111,   112,   129,    95,   130,
     210,    72,   176,   177,   178,   179,   175,   212,   152,   317,
     153,    99,   111,   112,   299,   319,   114,   115,    55,   116,
     245,   117,   118,   119,   120,   248,    62,   121,   251,   122,
     268,   331,   283,   284,   285,   123,   124,   269,   270,   125,
     271,   126,   230,   302,   176,   177,   178,   179,   127,   128,
      99,   111,   112,   129,    95,   130,   338,    72,   114,   115,
      55,   116,   292,   117,   118,   119,   120,   303,    62,   121,
     293,   122,   176,   177,   178,   179,  -141,   123,   124,   215,
    -216,   330,   325,   126,    20,    21,    22,    23,    24,   336,
     127,   128,    99,   111,   112,   129,    95,   130,   314,    72,
     114,   115,    55,   116,   320,   117,   118,   236,   120,   322,
     237,   121,   238,    64,    65,    66,    67,   176,   177,   178,
     179,   180,   326,   328,   295,   176,   177,   178,   179,   358,
     337,   355,    69,   339,   340,   367,   373,   374,    95,    39,
     380,    72,   114,   115,    55,   116,   381,   117,   118,   236,
     120,   384,   237,   121,   238,    64,    65,    66,    67,   114,
     115,    55,   116,    40,   117,   118,   119,   120,    29,    62,
     256,   197,   122,   315,    69,   316,   321,    82,   385,   296,
      95,   357,   257,    72,    19,    20,    21,    22,    23,    24,
      25,    26,   327,   258,   103,   104,   382,   259,   329,    88,
      72,   114,   115,    55,   116,   345,   117,   118,   119,   120,
     347,    62,   256,   149,   122,   346,   107,   324,   353,    33,
       0,   114,   115,    55,   116,   304,   117,   118,   119,   120,
     350,    62,   256,     0,   122,   305,   103,   104,     0,   259,
       0,     0,    72,     2,   344,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,   258,   103,   104,     0,   259,
       0,     0,    72,   114,   115,    55,   116,     0,   117,   118,
     119,   120,     0,    62,   256,     0,   122,     0,     0,     0,
       0,     0,     0,   114,   115,    55,   116,   363,   117,   118,
     119,   120,   332,    62,   121,     0,   122,   305,   103,   104,
       0,   259,     0,     0,    72,     0,     0,   356,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
       0,    95,     0,     0,    72,   114,   115,    55,   116,     0,
     117,   118,   119,   120,     0,    62,   121,     0,   122,     0,
     114,   115,    55,   116,     0,   117,   118,   119,   120,     0,
      62,   121,   202,   122,     0,     0,     0,   203,     0,     0,
       0,     0,     0,    95,   253,     0,    72,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,     0,    95,     0,
       0,    72,   114,   115,    55,   116,     0,   117,   118,   119,
     120,     0,    62,   121,     0,   122,     0,   114,   115,    55,
     116,     0,   117,   118,   119,   120,   255,    62,   121,     0,
     122,     0,     0,     0,    69,     0,     0,     0,     0,     0,
      95,   280,     0,    72,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,    95,     0,     0,    72,   114,
     115,    55,   116,     0,   117,   118,   119,   120,     0,    62,
     121,     0,   122,     0,   114,   115,    55,   116,     0,   117,
     118,   119,   120,   298,    62,   121,     0,   122,     0,     0,
       0,    69,     0,     0,     0,     0,     0,    95,   300,     0,
      72,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,     0,    95,     0,     0,    72,   114,   115,    55,   116,
       0,   117,   118,   119,   120,     0,    62,   121,     0,   122,
       0,   114,   115,    55,   116,     0,   117,   118,   119,   120,
     168,    62,   121,   230,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,   168,     0,    72,     0,     0,
     114,   115,    55,   116,     0,   117,   118,   119,   120,    95,
      62,   121,    72,   122,   114,   115,    55,   116,     0,   117,
     118,   119,   120,   125,    62,   121,     0,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    72
};

స్థిర స్థిరరాశి yytype_int16 yycheck[] =
{
      48,    48,   202,   202,    41,   229,   212,    48,    58,    48,
      60,    52,    48,    49,   214,    37,    38,    39,    40,    56,
      41,    44,    59,   202,    45,    48,     0,    49,    69,    53,
      12,    13,    14,    15,    16,    56,    60,    61,    59,    63,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      52,    28,    29,   160,    31,    32,    33,    34,    35,    49,
      42,    18,    19,    20,    21,    89,    23,    24,    25,    26,
      27,    28,    29,    50,    31,    38,   183,   125,   257,    56,
      57,   188,    59,    41,   123,   124,   193,    41,   127,   128,
      53,    54,    55,    50,    42,    29,   146,    49,    42,    56,
      48,    29,    59,    57,    48,   153,   330,    41,    29,   309,
     309,   311,    44,     0,   162,   156,    48,    41,    52,    53,
      54,   162,    56,   162,    52,    53,    54,    52,    56,    57,
     309,    52,    53,    54,   313,    56,    42,   185,   185,    42,
      52,    42,    48,   184,   185,    48,   185,    48,    59,    42,
      42,   199,   200,    57,   202,    48,    48,    57,   199,   200,
     199,   200,   203,   369,   343,   344,   214,    53,    54,    56,
     349,    58,    59,    60,    61,    62,    63,    64,    65,    66,
     228,    37,    38,    39,    40,    42,    45,   228,    47,   228,
      46,    48,    25,   241,   241,    29,    56,    42,   246,    59,
     241,   249,   241,    48,    46,   246,    48,   246,   249,   257,
     249,    29,   236,    52,   238,   126,    36,    42,    52,    53,
      54,   269,    56,    48,    42,    42,    36,    46,   269,    48,
     269,    48,    53,    54,    52,    53,    54,    46,    56,    48,
      46,   152,    48,   154,    41,   293,   293,   158,   289,    49,
     161,    36,   293,   164,   293,    52,    53,    54,    55,    41,
      57,   309,    46,   311,    48,   176,   177,   178,   179,    41,
     181,   182,    37,    38,    39,    40,   123,   124,   189,    37,
     127,   128,   330,    42,    49,    41,   334,   334,    41,    37,
      38,    39,    40,   334,    36,   334,   344,    56,    46,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    39,    40,
      18,    19,    20,    21,    36,    23,    24,    25,    26,    36,
      28,    29,    36,    31,    37,    38,    39,    40,    49,    37,
      38,    41,    41,    41,   245,    43,    49,    45,    41,    36,
     251,    41,    50,    51,    52,    53,    54,    55,    56,    57,
      57,    59,    37,    38,    39,    40,    43,    43,    45,   270,
      47,    52,    53,    54,    49,   276,    18,    19,    20,    21,
      36,    23,    24,    25,    26,    49,    28,    29,    45,    31,
      42,   292,    53,    54,    55,    37,    38,    48,    36,    41,
      49,    43,    45,    49,    37,    38,    39,    40,    50,    51,
      52,    53,    54,    55,    56,    57,    49,    59,    18,    19,
      20,    21,    36,    23,    24,    25,    26,    49,    28,    29,
      41,    31,    37,    38,    39,    40,    43,    37,    38,    44,
      43,    41,    48,    43,     5,     6,     7,     8,     9,    41,
      50,    51,    52,    53,    54,    55,    56,    57,    49,    59,
      18,    19,    20,    21,    49,    23,    24,    25,    26,    49,
      28,    29,    30,    31,    32,    33,    34,    37,    38,    39,
      40,    41,    49,    49,    42,    37,    38,    39,    40,    57,
      49,    41,    50,    49,    49,    46,    49,    36,    56,    36,
      57,    59,    18,    19,    20,    21,    49,    23,    24,    25,
      26,    49,    28,    29,    30,    31,    32,    33,    34,    18,
      19,    20,    21,    36,    23,    24,    25,    26,    16,    28,
      29,   156,    31,   269,    50,   270,   278,    48,   379,   241,
      56,   334,    41,    59,     4,     5,     6,     7,     8,     9,
      10,    11,   289,    52,    53,    54,   376,    56,   291,    48,
      59,    18,    19,    20,    21,   309,    23,    24,    25,    26,
     311,    28,    29,    70,    31,   309,    54,   282,   325,    28,
      -1,    18,    19,    20,    21,    42,    23,    24,    25,    26,
     313,    28,    29,    -1,    31,    52,    53,    54,    -1,    56,
      -1,    -1,    59,    56,    41,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    52,    53,    54,    -1,    56,
      -1,    -1,    59,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    42,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    52,    53,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    -1,    28,    29,    -1,    31,    -1,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    45,    31,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    56,    42,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    -1,    31,    -1,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    42,    28,    29,    -1,
      31,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      56,    42,    -1,    59,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    28,
      29,    -1,    31,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    42,    28,    29,    -1,    31,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    42,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    59,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    -1,    31,
      -1,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      42,    28,    29,    45,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    42,    -1,    59,    -1,    -1,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    56,
      28,    29,    59,    31,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    41,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    59
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
స్థిర స్థిరరాశి yytype_uint8 yystos[] =
{
       0,     0,    56,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    68,    69,    72,    74,    75,    76,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    70,    73,    76,
      52,   148,   149,   148,    41,    41,    71,    77,    42,    72,
      74,    12,    13,    14,    15,    16,    42,    78,   122,   123,
      49,    49,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    28,    29,    31,    32,    33,    34,    35,    50,
      56,    57,    59,    79,    80,    81,    82,    85,    88,    89,
      95,    96,    99,   100,   101,   102,   103,   104,   118,   119,
     120,   121,   132,   146,    52,    56,    83,    84,   146,    52,
     147,    29,    52,    53,    54,    56,    86,   139,   140,    57,
      57,    53,    54,   144,    18,    19,    21,    23,    24,    25,
      26,    29,    31,    37,    38,    41,    43,    50,    51,    55,
      57,   120,   125,   126,   129,   130,   132,   134,   143,   144,
     146,   147,   144,   147,   147,   147,    25,   146,   133,   134,
      36,   147,    45,    47,    36,    49,    48,    49,    36,    41,
      41,    37,    41,    36,    36,   126,   132,   126,    42,   120,
     128,   131,   125,   126,   126,    43,    37,    38,    39,    40,
      41,    36,    36,    41,    41,    41,   144,    49,    41,    36,
      53,    54,   145,    41,   125,   120,   125,    84,   125,    87,
      87,   125,    45,    50,    90,    91,    92,    93,   132,   146,
      57,   125,    43,    42,    48,    44,   124,   125,   125,   125,
     125,   125,    57,    97,    98,   139,   125,   125,    87,    41,
      45,   114,   115,   116,   117,   146,    25,    28,    30,    79,
      99,   105,   106,   111,   118,    36,    87,   125,    49,    87,
      46,    45,    49,    42,    79,    42,    29,    41,    52,    56,
      94,   127,   129,   131,   135,   137,   140,   146,    42,    48,
      36,    49,    49,   124,   131,    44,    48,    42,    48,    49,
      42,   116,    38,    53,    54,    55,   141,   142,    42,    48,
      42,    48,    36,    41,   147,    42,   106,   125,    42,    49,
      42,   125,    49,    49,    42,    52,   136,   140,    46,    48,
      46,    48,    46,    48,    49,    92,    93,   125,    44,   125,
      49,    98,    49,    42,   142,    48,    49,   115,    49,   117,
      41,   125,    27,    79,   107,   108,    41,    49,    49,    49,
      49,    46,    42,    48,    41,   127,   135,   130,   131,    41,
     137,   138,   140,   141,   116,    41,    42,   108,    57,   112,
     113,    52,   140,    42,    46,    46,    42,    46,    42,    41,
      57,   109,   110,    49,    36,    42,    48,   124,    42,    48,
      57,    49,   113,    42,    49,   110
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
స్థిర స్థిరరాశి yytype_uint8 yyr1[] =
{
       0,    67,    68,    68,    68,    69,    70,    70,    70,    71,
      71,    72,    73,    73,    73,    73,    73,    74,    74,    75,
      75,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    77,    77,    77,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      79,    79,    79,    80,    81,    82,    83,    83,    84,    84,
      85,    86,    86,    87,    87,    88,    89,    90,    90,    91,
      91,    92,    92,    92,    92,    92,    93,    93,    93,    93,
      93,    94,    94,    94,    95,    96,    97,    97,    98,    98,
      99,   100,   100,   101,   102,   103,   103,   104,   105,   105,
     106,   106,   106,   106,   106,   107,   107,   108,   108,   109,
     109,   110,   110,   111,   112,   112,   113,   114,   114,   115,
     115,   115,   116,   116,   117,   118,   119,   119,   119,   119,
     120,   120,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   122,   122,   123,   123,   123,   123,   123,
     124,   124,   124,   125,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   127,   127,
     127,   127,   128,   128,   129,   130,   130,   131,   132,   132,
     132,   132,   133,   133,   134,   134,   134,   134,   135,   135,
     135,   135,   136,   136,   136,   136,   137,   137,   138,   138,
     139,   139,   140,   140,   140,   140,   141,   141,   142,   142,
     142,   143,   144,   144,   145,   145,   146,   146,   147,   148,
     148,   149
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
స్థిర స్థిరరాశి yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     7,     1,     1,     1,     2,
       0,     7,     1,     1,     1,     1,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       4,     2,     3,     4,     5,     3,     3,     1,     1,     3,
       6,     3,     1,     2,     0,     6,     6,     1,     0,     3,
       1,     3,     3,     1,     2,     1,     3,     5,     3,     5,
       3,     4,     2,     0,     5,     6,     3,     1,     1,     1,
       6,     5,     6,     5,     6,     6,     6,     6,     2,     1,
       5,     1,     1,     1,     1,     2,     1,     5,     1,     3,
       1,     1,     3,     6,     3,     1,     3,     3,     1,     3,
       5,     3,     3,     1,     5,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       3,     1,     0,     3,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     1,     4,     1,     1,     3,     3,     3,
       1,     1,     3,     1,     3,     1,     2,     4,     1,     3,
       4,     6,     1,     0,     1,     1,     1,     1,     3,     3,
       1,     1,     3,     3,     1,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1
};


గణన { YYENOMEM = -2 };

#నిర్వచించు yyerrok         (yyerrstatus = 0)
#నిర్వచించు yyclearin       (yychar = YYEMPTY)

#నిర్వచించు YYACCEPT        వెళ్ళు yyacceptlab
#నిర్వచించు YYABORT         వెళ్ళు yyabortlab
#నిర్వచించు YYERROR         వెళ్ళు yyerrorlab
#నిర్వచించు YYNOMEM         వెళ్ళు yyexhaustedlab


#నిర్వచించు YYRECOVERING()  (!!yyerrstatus)

#నిర్వచించు YYBACKUP(Token, విలువ_2)                                    \
  చేయి                                                              \
    ఒకవేళ (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (విలువ_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        వెళ్ళు yybackup;                                            \
      }                                                           \
    లేకపోతే                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  అయితే (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#నిర్వచించు YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#ఒకవేళ YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(వాదనలు_2)                        \
చేయి {                                            \
  ఒకవేళ (yydebug)                                  \
    YYFPRINTF వాదనలు_2;                             \
} అయితే (0)




# define YY_చిహ్నం_PRINT(Title, రకం_2, విలువ_2, Location)                    \
చేయి {                                                                      \
  ఒకవేళ (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_చిహ్నం_print (stderr,                                            \
                  రకం_2, విలువ_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} అయితే (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

స్థిర శూన్య
yy_చిహ్నం_విలువ_print (FILE *yyo,
                       yysymbol_రకం_t yykind, YYSTYPE స్థిరరాశి * స్థిరరాశి yyvaluep, నిర్మాణం parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  ఒకవేళ (!yyvaluep)
    తిరిగి;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_చివర
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

స్థిర శూన్య
yy_చిహ్నం_print (FILE *yyo,
                 yysymbol_రకం_t yykind, YYSTYPE స్థిరరాశి * స్థిరరాశి yyvaluep, నిర్మాణం parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_చిహ్నం_విలువ_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

స్థిర శూన్య
yy_దొంతర_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  కొరకు (; yybottom <= yytop; yybottom++)
    {
      పూర్ణాంకం yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_దొంతర_PRINT(Bottom, శిఖరం)                            \
చేయి {                                                            \
  ఒకవేళ (yydebug)                                                  \
    yy_దొంతర_print ((Bottom), (శిఖరం));                           \
} అయితే (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

స్థిర శూన్య
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 పూర్ణాంకం yyrule, నిర్మాణం parser_param *param)
{
  పూర్ణాంకం yylno = yyrline[yyrule];
  పూర్ణాంకం yynrhs = yyr2[yyrule];
  పూర్ణాంకం yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  కొరకు (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_చిహ్నం_print (stderr,
                       YY_ACCESSING_చిహ్నం (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(నియమం)          \
చేయి {                                    \
  ఒకవేళ (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, నియమం, param); \
} అయితే (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
పూర్ణాంకం yydebug;
#లేకపోతే /* !YYDEBUG */
# define YYDPRINTF(వాదనలు_2) ((శూన్య) 0)
# define YY_చిహ్నం_PRINT(Title, రకం_2, విలువ_2, Location)
# define YY_దొంతర_PRINT(Bottom, శిఖరం)
# define YY_REDUCE_PRINT(నియమం)
#అయితే_ముగింపు /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#నిర్వచితం_కాకపోతే YYINITDEPTH
# define YYINITDEPTH 200
#అయితే_ముగింపు

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#నిర్వచితం_కాకపోతే YYMAXDEPTH
# define YYMAXDEPTH 10000
#అయితే_ముగింపు


/* Context of a parse error.  */
రకనిర్వచనం నిర్మాణం
{
  yy_state_t *yyssp;
  yysymbol_రకం_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
స్థిర పూర్ణాంకం
yypcontext_expected_టోకెన్లు (స్థిరరాశి yypcontext_t *yyctx,
                            yysymbol_రకం_t yyarg[], పూర్ణాంకం yyargn)
{
  /* Actual size of YYARG. */
  పూర్ణాంకం yycount = 0;
  పూర్ణాంకం yyn = yypact[+*yyctx->yyssp];
  ఒకవేళ (!yypact_విలువ_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      పూర్ణాంకం yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      పూర్ణాంకం yychecklim = YYLAST - yyn + 1;
      పూర్ణాంకం yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      పూర్ణాంకం yyx;
      కొరకు (yyx = yyxbegin; yyx < yyxend; ++yyx)
        ఒకవేళ (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_విలువ_is_error (yytable[yyx + yyn]))
          {
            ఒకవేళ (!yyarg)
              ++yycount;
            లేకపోతే ఒకవేళ (yycount == yyargn)
              తిరిగి 0;
            లేకపోతే
              yyarg[yycount++] = YY_CAST (yysymbol_రకం_t, yyx);
          }
    }
  ఒకవేళ (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  తిరిగి yycount;
}




#నిర్వచితం_కాకపోతే yystrlen
# ఒకవేళ defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, అక్షరశ్రేణి_పొడవు (S)))
# లేకపోతే
/* Return the length of YYSTR.  */
స్థిర YYPTRDIFF_T
yystrlen (స్థిరరాశి అక్షరం *yystr)
{
  YYPTRDIFF_T yylen;
  కొరకు (yylen = 0; yystr[yylen]; yylen++)
    కొనసాగు;
  తిరిగి yylen;
}
# endif
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే yystpcpy
# ఒకవేళ defined __GLIBC__ && defined _STRING_H && defined GNU_ఆధారం
#  define yystpcpy stpcpy
# లేకపోతే
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
స్థిర అక్షరం *
yystpcpy (అక్షరం *yydest, స్థిరరాశి అక్షరం *yysrc)
{
  అక్షరం *yyd = yydest;
  స్థిరరాశి అక్షరం *yys = yysrc;

  అయితే ((*yyd++ = *yys++) != '\0')
    కొనసాగు;

  తిరిగి yyd - 1;
}
# endif
#అయితే_ముగింపు



స్థిర పూర్ణాంకం
yy_syntax_error_arguments (స్థిరరాశి yypcontext_t *yyctx,
                           yysymbol_రకం_t yyarg[], పూర్ణాంకం yyargn)
{
  /* Actual size of YYARG. */
  పూర్ణాంకం yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  ఒకవేళ (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      పూర్ణాంకం yyn;
      ఒకవేళ (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_టోకెన్లు (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      ఒకవేళ (yyn == YYENOMEM)
        తిరిగి YYENOMEM;
      లేకపోతే
        yycount += yyn;
    }
  తిరిగి yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
స్థిర పూర్ణాంకం
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, అక్షరం **yymsg,
                స్థిరరాశి yypcontext_t *yyctx)
{
  గణన { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  స్థిరరాశి అక్షరం *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_రకం_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  పూర్ణాంకం yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  ఒకవేళ (yycount == YYENOMEM)
    తిరిగి YYENOMEM;

  ఎంపిక (yycount)
    {
#నిర్వచించు YYCASE_(N, S)                       \
      స్థితి N:                               \
        yyformat = S;                       \
        ఆపు
    అప్రమేయ: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#నిర్వచనం_రద్దు YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    పూర్ణాంకం yyi;
    కొరకు (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        ఒకవేళ (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_గరిష్ఠ)
          yysize = yysize1;
        లేకపోతే
          తిరిగి YYENOMEM;
      }
  }

  ఒకవేళ (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      ఒకవేళ (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_గరిష్ఠ))
        *yymsg_alloc = YYSTACK_ALLOC_గరిష్ఠ;
      తిరిగి -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    అక్షరం *yyp = *yymsg;
    పూర్ణాంకం yyi = 0;
    అయితే ((*yyp = *yyformat) != '\0')
      ఒకవేళ (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      లేకపోతే
        {
          ++yyp;
          ++yyformat;
        }
  }
  తిరిగి 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

స్థిర శూన్య
yydestruct (స్థిరరాశి అక్షరం *yymsg,
            yysymbol_రకం_t yykind, YYSTYPE *yyvaluep, నిర్మాణం parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  ఒకవేళ (!yymsg)
    yymsg = "Deleting";
  YY_చిహ్నం_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ఎంపిక (yykind)
    {
    స్థితి YYSYMBOL_STRING: /* "string literal"  */
#పంక్తి 259 "src/xkbcomp/parser.y"
            { విడుదల(((*yyvaluep).str)); }
#పంక్తి 1640 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_XkbFile: /* XkbFile  */
#పంక్తి 257 "src/xkbcomp/parser.y"
            { ఒకవేళ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#పంక్తి 1646 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Xkb_సంయుక్త_Map: /* XkbCompositeMap  */
#పంక్తి 257 "src/xkbcomp/parser.y"
            { ఒకవేళ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#పంక్తి 1652 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#పంక్తి 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).తల); }
#పంక్తి 1658 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#పంక్తి 257 "src/xkbcomp/parser.y"
            { ఒకవేళ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#పంక్తి 1664 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_DeclList: /* DeclList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).తల); }
#పంక్తి 1670 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Decl: /* Decl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#పంక్తి 1676 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_VarDecl: /* VarDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#పంక్తి 1682 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Name_Decl: /* KeyNameDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).కీలకం_సంకేతం)); }
#పంక్తి 1688 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Alias_Decl: /* KeyAliasDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).కీలకం_Alias)); }
#పంక్తి 1694 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_VModDecl: /* VModDecl  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).తల); }
#పంక్తి 1700 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_VModDefList: /* VModDefList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).తల); }
#పంక్తి 1706 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_VModDef: /* VModDef  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#పంక్తి 1712 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#పంక్తి 1718 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Interpret_సరిపోలిక: /* InterpretMatch  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#పంక్తి 1724 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_VarDeclList: /* VarDeclList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).తల); }
#పంక్తి 1730 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Type_Decl: /* KeyTypeDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).కీలకం_Type)); }
#పంక్తి 1736 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#పంక్తి 1742 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).తల); }
#పంక్తి 1748 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).తల); }
#పంక్తి 1754 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#పంక్తి 1760 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Multi_కీలకం_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1766 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_సమూహం_Compat_Decl: /* GroupCompatDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమూహం_Compat)); }
#పంక్తి 1772 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_ముసుగు_2)); }
#పంక్తి 1778 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Or_కీలకం_Sym_List: /* KeyOrKeySymList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం_List).తల); }
#పంక్తి 1784 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Or_కీలకం_Sym: /* KeyOrKeySym  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1790 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#పంక్తి 1796 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#పంక్తి 1802 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_CoordList: /* CoordList  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1808 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Coord: /* Coord  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1814 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_సమాసం_List: /* ExprList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం_List).తల); }
#పంక్తి 1820 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_సమాసం: /*సమాసం*/
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1826 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_పదం: /*పదం*/
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1832 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_MultiActionList: /* MultiActionList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం_List).తల); }
#పంక్తి 1838 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_ActionList: /* ActionList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం_List).తల); }
#పంక్తి 1844 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Non_ఖాళీలు_Actions: /* NonEmptyActions  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1850 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Actions: /* Actions  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1856 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Action: /* Action  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1862 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Lhs: /* Lhs  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1868 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_OptTerminal: /* OptTerminal  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1874 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Terminal: /* Terminal  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1880 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Multi_కీలకం_Sym_List: /* MultiKeySymList  */
#పంక్తి 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం_List).తల); }
#పంక్తి 1886 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Sym_List: /* KeySymList  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1892 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_Non_ఖాళీలు_కీలకం_Syms: /* NonEmptyKeySyms  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1898 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_కీలకం_Syms: /* KeySyms  */
#పంక్తి 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).సమాసం)); }
#పంక్తి 1904 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_OptMapName: /* OptMapName  */
#పంక్తి 259 "src/xkbcomp/parser.y"
            { విడుదల(((*yyvaluep).str)); }
#పంక్తి 1910 "src/xkbcomp/parser.c"
        ఆపు;

    స్థితి YYSYMBOL_MapName: /* MapName  */
#పంక్తి 259 "src/xkbcomp/parser.y"
            { విడుదల(((*yyvaluep).str)); }
#పంక్తి 1916 "src/xkbcomp/parser.c"
        ఆపు;

      అప్రమేయ:
        ఆపు;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_చివర
}






/*----------.
| yyparse.  |
`----------*/

పూర్ణాంకం
yyparse (నిర్మాణం parser_param *param)
{
/* Lookahead token kind.  */
పూర్ణాంకం yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_విలువ (స్థిర YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_విలువ (= yyval_default);

    /* Number of syntax errors so far.  */
    పూర్ణాంకం yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    పూర్ణాంకం yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  పూర్ణాంకం yyn;
  /* The return value of yyparse.  */
  పూర్ణాంకం yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_రకం_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  అక్షరం yymsgbuf[128];
  అక్షరం *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = పరిమాణం yymsgbuf;

#నిర్వచించు YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  పూర్ణాంకం yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  వెళ్ళు yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_చివర
  YY_దొంతర_PRINT (yyss, yyssp);

  ఒకవేళ (yyss + yystacksize - 1 <= yyssp)
#ఒకవేళ !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#లేకపోతే
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# ఒకవేళ defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# లేకపోతే /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      ఒకవేళ (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      ఒకవేళ (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        సంఘం yyalloc *yyptr =
          YY_CAST (సంఘం yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_బైట్లు (yystacksize))));
        ఒకవేళ (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        ఒకవేళ (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (దీర్ఘ, yystacksize)));
      YY_IGNORE_USELESS_CAST_చివర

      ఒకవేళ (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#అయితే_ముగింపు /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  ఒకవేళ (yystate == YYFINAL)
    YYACCEPT;

  వెళ్ళు yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  ఒకవేళ (yypact_విలువ_is_default (yyn))
    వెళ్ళు yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  ఒకవేళ (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  ఒకవేళ (yychar <= చివర_యొక్క_FILE)
    {
      yychar = చివర_యొక్క_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  లేకపోతే ఒకవేళ (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      వెళ్ళు yyerrlab1;
    }
  లేకపోతే
    {
      yytoken = YYTRANSLATE (yychar);
      YY_చిహ్నం_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  ఒకవేళ (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    వెళ్ళు yydefault;
  yyn = yytable[yyn];
  ఒకవేళ (yyn <= 0)
    {
      ఒకవేళ (yytable_విలువ_is_error (yyn))
        వెళ్ళు yyerrlab;
      yyn = -yyn;
      వెళ్ళు yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  ఒకవేళ (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_చిహ్నం_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_చివర

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  వెళ్ళు yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  ఒకవేళ (yyn == 0)
    వెళ్ళు yyerrlab;
  వెళ్ళు yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  ఎంపిక (yyn)
    {
  స్థితి 2: /* XkbFile: XkbCompositeMap  */
#పంక్తి 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->మరిన్ని_maps = !!param->rtrn; (శూన్య) yynerrs; }
#పంక్తి 2195 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 3: /* XkbFile: XkbMapConfig  */
#పంక్తి 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->మరిన్ని_maps = !!param->rtrn; YYACCEPT; }
#పంక్తి 2201 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 4: /* XkbFile: "end of file"  */
#పంక్తి 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = ఖాళీ; param->మరిన్ని_maps = false; }
#పంక్తి 2207 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#పంక్తి 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).తల, (yyvsp[-6].mapFlags)); }
#పంక్తి 2213 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 6: /* XkbCompositeType: "xkb_keymap"  */
#పంక్తి 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#పంక్తి 2219 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 7: /* XkbCompositeType: "xkb_semantics"  */
#పంక్తి 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#పంక్తి 2225 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 8: /* XkbCompositeType: "xkb_layout"  */
#పంక్తి 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#పంక్తి 2231 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#పంక్తి 298 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[0].file)) {
                                ఒకవేళ ((yyvsp[-1].fileList).తల) {
                                    (yyval.fileList).తల = (yyvsp[-1].fileList).తల;
                                    (yyval.fileList).చివరి->common.తదుపరి = &(yyvsp[0].file)->common;
                                    (yyval.fileList).చివరి = (yyvsp[0].file);
                                } లేకపోతే {
                                    (yyval.fileList).తల = (yyval.fileList).చివరి = (yyvsp[0].file);
                                }
                            }
                        }
#పంక్తి 2247 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 10: /* XkbMapConfigList: %empty  */
#పంక్తి 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).తల = (yyval.fileList).చివరి = ఖాళీ; }
#పంక్తి 2253 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#పంక్తి 315 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            సురక్షిత_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).తల, (yyvsp[-6].mapFlags));
                        }
#పంక్తి 2266 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 12: /* FileType: "xkb_keycodes"  */
#పంక్తి 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#పంక్తి 2272 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 13: /* FileType: "xkb_types"  */
#పంక్తి 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#పంక్తి 2278 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 14: /* FileType: "xkb_compatibility"  */
#పంక్తి 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#పంక్తి 2284 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 15: /* FileType: "xkb_symbols"  */
#పంక్తి 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#పంక్తి 2290 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 16: /* FileType: "xkb_geometry"  */
#పంక్తి 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#పంక్తి 2296 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 17: /* OptFlags: Flags  */
#పంక్తి 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#పంక్తి 2302 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 18: /* OptFlags: %empty  */
#పంక్తి 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#పంక్తి 2308 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 19: /* Flags: Flags Flag  */
#పంక్తి 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#పంక్తి 2314 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 20: /* Flags: Flag  */
#పంక్తి 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#పంక్తి 2320 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 21: /* Flag: "partial"  */
#పంక్తి 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#పంక్తి 2326 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 22: /* Flag: "default"  */
#పంక్తి 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#పంక్తి 2332 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 23: /* Flag: "hidden"  */
#పంక్తి 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#పంక్తి 2338 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 24: /* Flag: "alphanumeric_keys"  */
#పంక్తి 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#పంక్తి 2344 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 25: /* Flag: "modifier_keys"  */
#పంక్తి 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#పంక్తి 2350 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 26: /* Flag: "keypad_keys"  */
#పంక్తి 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#పంక్తి 2356 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 27: /* Flag: "function_keys"  */
#పంక్తి 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#పంక్తి 2362 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 28: /* Flag: "alternate_group"  */
#పంక్తి 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#పంక్తి 2368 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 29: /* Flag: "deprecated"  */
#పంక్తి 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#పంక్తి 2374 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 30: /* Flag: "identifier"  */
#పంక్తి 350 "src/xkbcomp/parser.y"
                        {
                            స్థిరరాశి bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_సంకేతం(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_కనీస,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (గుర్తులేని)(yyvsp[0].sval).len, (yyvsp[0].sval).ప్రారంభం,
                                (error ? "" : "; ignored")
                            );
                            ఒకవేళ (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#పంక్తి 2393 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 31: /* DeclList: DeclList Decl  */
#పంక్తి 367 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[0].any)) {
                                ఒకవేళ ((yyvsp[-1].anyList).తల) {
                                    (yyval.anyList).తల = (yyvsp[-1].anyList).తల; (yyvsp[-1].anyList).చివరి->తదుపరి = (yyvsp[0].any); (yyval.anyList).చివరి = (yyvsp[0].any);
                                } లేకపోతే {
                                    (yyval.anyList).తల = (yyval.anyList).చివరి = (yyvsp[0].any);
                                }
                            }
                        }
#పంక్తి 2407 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#పంక్తి 382 "src/xkbcomp/parser.y"
                        {
                            కొరకు (VModDef *vmod = (yyvsp[0].vmodList).తల; vmod; vmod = (VModDef *) vmod->common.తదుపరి)
                                vmod->విలీనం = (yyvsp[-1].విలీనం);
                            ఒకవేళ ((yyvsp[-2].anyList).తల) {
                                (yyval.anyList).తల = (yyvsp[-2].anyList).తల; (yyvsp[-2].anyList).చివరి->తదుపరి = &(yyvsp[0].vmodList).తల->common; (yyval.anyList).చివరి = &(yyvsp[0].vmodList).చివరి->common;
                            } లేకపోతే {
                                (yyval.anyList).తల = &(yyvsp[0].vmodList).తల->common; (yyval.anyList).చివరి = &(yyvsp[0].vmodList).చివరి->common;
                            }
                        }
#పంక్తి 2421 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 33: /* DeclList: %empty  */
#పంక్తి 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).తల = (yyval.anyList).చివరి = ఖాళీ; }
#పంక్తి 2427 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 34: /* Decl: OptMergeMode VarDecl  */
#పంక్తి 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#పంక్తి 2436 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 35: /* Decl: OptMergeMode InterpretDecl  */
#పంక్తి 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#పంక్తి 2445 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 36: /* Decl: OptMergeMode KeyNameDecl  */
#పంక్తి 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].కీలకం_సంకేతం)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].కీలకం_సంకేతం);
                        }
#పంక్తి 2454 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 37: /* Decl: OptMergeMode KeyAliasDecl  */
#పంక్తి 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].కీలకం_Alias)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].కీలకం_Alias);
                        }
#పంక్తి 2463 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 38: /* Decl: OptMergeMode KeyTypeDecl  */
#పంక్తి 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].కీలకం_Type)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].కీలకం_Type);
                        }
#పంక్తి 2472 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 39: /* Decl: OptMergeMode SymbolsDecl  */
#పంక్తి 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#పంక్తి 2481 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 40: /* Decl: OptMergeMode ModMapDecl  */
#పంక్తి 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_ముసుగు_2)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_ముసుగు_2);
                        }
#పంక్తి 2490 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 41: /* Decl: OptMergeMode GroupCompatDecl  */
#పంక్తి 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].సమూహం_Compat)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].సమూహం_Compat);
                        }
#పంక్తి 2499 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 42: /* Decl: OptMergeMode LedMapDecl  */
#పంక్తి 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#పంక్తి 2508 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 43: /* Decl: OptMergeMode LedNameDecl  */
#పంక్తి 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->విలీనం = (yyvsp[-1].విలీనం);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#పంక్తి 2517 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 44: /* Decl: OptMergeMode ShapeDecl  */
#పంక్తి 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ఖాళీ; }
#పంక్తి 2523 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 45: /* Decl: OptMergeMode SectionDecl  */
#పంక్తి 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ఖాళీ; }
#పంక్తి 2529 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 46: /* Decl: OptMergeMode DoodadDecl  */
#పంక్తి 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ఖాళీ; }
#పంక్తి 2535 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 47: /* Decl: OptMergeMode UnknownDecl  */
#పంక్తి 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#పంక్తి 2541 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#పంక్తి 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#పంక్తి 2547 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 49: /* Decl: MergeMode "string literal"  */
#పంక్తి 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].విలీనం));
                            విడుదల((yyvsp[0].str));
                        }
#పంక్తి 2556 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 50: /* VarDecl: Lhs "=" Expr ";"  */
#పంక్తి 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].సమాసం), (yyvsp[-1].సమాసం)); }
#పంక్తి 2562 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 51: /* VarDecl: Ident ";"  */
#పంక్తి 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#పంక్తి 2568 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 52: /* VarDecl: "!" Ident ";"  */
#పంక్తి 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#పంక్తి 2574 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#పంక్తి 468 "src/xkbcomp/parser.y"
                        { (yyval.కీలకం_సంకేతం) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].అంకె)); }
#పంక్తి 2580 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#పంక్తి 472 "src/xkbcomp/parser.y"
                        { (yyval.కీలకం_Alias) = కీలకం_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#పంక్తి 2586 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#పంక్తి 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#పంక్తి 2592 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 56: /* VModDefList: VModDefList "," VModDef  */
#పంక్తి 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).తల = (yyvsp[-2].vmodList).తల; (yyval.vmodList).చివరి->common.తదుపరి = &(yyvsp[0].vmod)->common; (yyval.vmodList).చివరి = (yyvsp[0].vmod); }
#పంక్తి 2598 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 57: /* VModDefList: VModDef  */
#పంక్తి 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).తల = (yyval.vmodList).చివరి = (yyvsp[0].vmod); }
#పంక్తి 2604 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 58: /* VModDef: Ident  */
#పంక్తి 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), ఖాళీ); }
#పంక్తి 2610 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 59: /* VModDef: Ident "=" Expr  */
#పంక్తి 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].సమాసం)); }
#పంక్తి 2616 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#పంక్తి 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).తల; (yyval.interp) = (yyvsp[-4].interp); }
#పంక్తి 2622 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 61: /* InterpretMatch: KeySym "+" Expr  */
#పంక్తి 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].సమాసం)); }
#పంక్తి 2628 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 62: /* InterpretMatch: KeySym  */
#పంక్తి 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), ఖాళీ); }
#పంక్తి 2634 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 63: /* VarDeclList: VarDeclList VarDecl  */
#పంక్తి 504 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[0].var)) {
                                ఒకవేళ ((yyvsp[-1].varList).తల) {
                                    (yyval.varList).తల = (yyvsp[-1].varList).తల;
                                    (yyval.varList).చివరి->common.తదుపరి = &(yyvsp[0].var)->common;
                                    (yyval.varList).చివరి = (yyvsp[0].var);
                                } లేకపోతే {
                                    (yyval.varList).తల = (yyval.varList).చివరి = (yyvsp[0].var);
                                }
                            }
                        }
#పంక్తి 2650 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 64: /* VarDeclList: %empty  */
#పంక్తి 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).తల = (yyval.varList).చివరి = ఖాళీ; }
#పంక్తి 2656 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#పంక్తి 521 "src/xkbcomp/parser.y"
                        { (yyval.కీలకం_Type) = కీలకం_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).తల); }
#పంక్తి 2662 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#పంక్తి 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).తల); }
#పంక్తి 2668 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 67: /* OptSymbolsBody: SymbolsBody  */
#పంక్తి 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#పంక్తి 2674 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 68: /* OptSymbolsBody: %empty  */
#పంక్తి 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).తల = (yyval.varList).చివరి = ఖాళీ; }
#పంక్తి 2680 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#పంక్తి 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).తల = (yyvsp[-2].varList).తల; (yyval.varList).చివరి->common.తదుపరి = &(yyvsp[0].var)->common; (yyval.varList).చివరి = (yyvsp[0].var); }
#పంక్తి 2686 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 70: /* SymbolsBody: SymbolsVarDecl  */
#పంక్తి 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).తల = (yyval.varList).చివరి = (yyvsp[0].var); }
#పంక్తి 2692 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#పంక్తి 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 2698 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#పంక్తి 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 2704 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 73: /* SymbolsVarDecl: Ident  */
#పంక్తి 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#పంక్తి 2710 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 74: /* SymbolsVarDecl: "!" Ident  */
#పంక్తి 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#పంక్తి 2716 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#పంక్తి 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(ఖాళీ, (yyvsp[0].సమాసం)); }
#పంక్తి 2722 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#పంక్తి 560 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[-1].సమాసం_List).తల; }
#పంక్తి 2728 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#పంక్తి 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            నిర్మాణం {సమాసం_Def *తల; సమాసం_Def *చివరి;} list = {
                                .తల = (yyvsp[-1].సమాసం_List).తల, .చివరి = (yyvsp[-1].సమాసం_List).చివరి
                            };
                            కొరకు (uint32_t k = 0; k < (yyvsp[-3].no_చిహ్నం_Or_Action_List); k++) {
                                సమాసం_Def* స్థిరరాశి syms =
                                    సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                                ఒకవేళ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.తదుపరి = &list.తల->common;
                                list.తల = syms;
                            }
                            (yyval.సమాసం) = list.తల;
                        }
#పంక్తి 2750 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#పంక్తి 580 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[-1].సమాసం_List).తల; }
#పంక్తి 2756 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#పంక్తి 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            నిర్మాణం {సమాసం_Def *తల; సమాసం_Def *చివరి;} list = {
                                .తల = (yyvsp[-1].సమాసం_List).తల, .చివరి = (yyvsp[-1].సమాసం_List).చివరి
                            };
                            కొరకు (uint32_t k = 0; k < (yyvsp[-3].no_చిహ్నం_Or_Action_List); k++) {
                                సమాసం_Def* స్థిరరాశి acts = సమాసం_Create_Action_List(ఖాళీ);
                                ఒకవేళ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.తదుపరి = &list.తల->common;
                                list.తల = acts;
                            }
                            (yyval.సమాసం) = list.తల;
                        }
#పంక్తి 2777 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#పంక్తి 604 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_ఖాళీలు_List(); }
#పంక్తి 2783 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#పంక్తి 610 "src/xkbcomp/parser.y"
                        { (yyval.no_చిహ్నం_Or_Action_List) = (yyvsp[-3].no_చిహ్నం_Or_Action_List) + 1; }
#పంక్తి 2789 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 82: /* NoSymbolOrActionList: "{" "}"  */
#పంక్తి 612 "src/xkbcomp/parser.y"
                        { (yyval.no_చిహ్నం_Or_Action_List) = 1; }
#పంక్తి 2795 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 83: /* NoSymbolOrActionList: %empty  */
#పంక్తి 613 "src/xkbcomp/parser.y"
                        { (yyval.no_చిహ్నం_Or_Action_List) = 0; }
#పంక్తి 2801 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#పంక్తి 617 "src/xkbcomp/parser.y"
                        { (yyval.సమూహం_Compat) = సమూహం_Compat_Create((yyvsp[-3].అంకె), (yyvsp[-1].సమాసం)); }
#పంక్తి 2807 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#పంక్తి 621 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ (param->config.format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1 &&
                                (yyvsp[-4].సమాసం)->common.type != STMT_సమాసం_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ముసుగు,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].సమాసం));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].సమాసం_List).తల);
                                    YYERROR;
                            }
                            (yyval.mod_ముసుగు_2) = ModMapCreate((yyvsp[-4].సమాసం), (yyvsp[-2].సమాసం_List).తల);
                        }
#పంక్తి 2826 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#పంక్తి 638 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyvsp[-2].సమాసం_List).తల; (yyval.సమాసం_List).చివరి->common.తదుపరి = &(yyvsp[0].సమాసం)->common; (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 2832 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 87: /* KeyOrKeySymList: KeyOrKeySym  */
#పంక్తి 640 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 2838 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 88: /* KeyOrKeySym: "key name"  */
#పంక్తి 644 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_కీలకం_Name((yyvsp[0].atom)); }
#పంక్తి 2844 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 89: /* KeyOrKeySym: KeySym  */
#పంక్తి 646 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_కీలకం_Sym((yyvsp[0].keysym)); }
#పంక్తి 2850 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#పంక్తి 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).తల); }
#పంక్తి 2856 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#పంక్తి 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].అంకె), (yyvsp[-1].సమాసం), false); }
#పంక్తి 2862 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#పంక్తి 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].అంకె), (yyvsp[-1].సమాసం), true); }
#పంక్తి 2868 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#పంక్తి 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].సమాసం));
                            FreeStmt((ParseCommon *) (yyvsp[-1].సమాసం));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#పంక్తి 2878 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#పంక్తి 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].సమాసం));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).తల);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#పంక్తి 2888 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#పంక్తి 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2894 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#పంక్తి 679 "src/xkbcomp/parser.y"
                        { (శూన్య) (yyvsp[-2].సమాసం); (yyval.geom) = ఖాళీ; }
#పంక్తి 2900 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#పంక్తి 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2906 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 98: /* SectionBody: SectionBody SectionBodyItem  */
#పంక్తి 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ఖాళీ;}
#పంక్తి 2912 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 99: /* SectionBody: SectionBodyItem  */
#పంక్తి 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2918 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#పంక్తి 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2924 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 101: /* SectionBodyItem: VarDecl  */
#పంక్తి 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ఖాళీ; }
#పంక్తి 2930 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 102: /* SectionBodyItem: DoodadDecl  */
#పంక్తి 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2936 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 103: /* SectionBodyItem: LedMapDecl  */
#పంక్తి 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = ఖాళీ; }
#పంక్తి 2942 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 104: /* SectionBodyItem: OverlayDecl  */
#పంక్తి 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2948 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 105: /* RowBody: RowBody RowBodyItem  */
#పంక్తి 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ఖాళీ;}
#పంక్తి 2954 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 106: /* RowBody: RowBodyItem  */
#పంక్తి 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ఖాళీ; }
#పంక్తి 2960 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#పంక్తి 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = ఖాళీ; }
#పంక్తి 2966 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 108: /* RowBodyItem: VarDecl  */
#పంక్తి 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ఖాళీ; }
#పంక్తి 2972 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 109: /* Keys: Keys "," Key  */
#పంక్తి 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ఖాళీ; }
#పంక్తి 2978 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 110: /* Keys: Key  */
#పంక్తి 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ఖాళీ; }
#పంక్తి 2984 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 111: /* Key: "key name"  */
#పంక్తి 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 2990 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 112: /* Key: "{" ExprList "}"  */
#పంక్తి 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].సమాసం_List).తల); (yyval.geom) = ఖాళీ; }
#పంక్తి 2996 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#పంక్తి 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 3002 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#పంక్తి 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 3008 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 115: /* OverlayKeyList: OverlayKey  */
#పంక్తి 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 3014 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 116: /* OverlayKey: "key name" "=" "key name"  */
#పంక్తి 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 3020 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 117: /* OutlineList: OutlineList "," OutlineInList  */
#పంక్తి 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ;}
#పంక్తి 3026 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 118: /* OutlineList: OutlineInList  */
#పంక్తి 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ఖాళీ; }
#పంక్తి 3032 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 119: /* OutlineInList: "{" CoordList "}"  */
#పంక్తి 739 "src/xkbcomp/parser.y"
                        { (శూన్య) (yyvsp[-1].సమాసం); (yyval.geom) = ఖాళీ; }
#పంక్తి 3038 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#పంక్తి 741 "src/xkbcomp/parser.y"
                        { (శూన్య) (yyvsp[-1].సమాసం); (yyval.geom) = ఖాళీ; }
#పంక్తి 3044 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 121: /* OutlineInList: Ident "=" Expr  */
#పంక్తి 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].సమాసం)); (yyval.geom) = ఖాళీ; }
#పంక్తి 3050 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 122: /* CoordList: CoordList "," Coord  */
#పంక్తి 747 "src/xkbcomp/parser.y"
                        { (శూన్య) (yyvsp[-2].సమాసం); (శూన్య) (yyvsp[0].సమాసం); (yyval.సమాసం) = ఖాళీ; }
#పంక్తి 3056 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 123: /* CoordList: Coord  */
#పంక్తి 749 "src/xkbcomp/parser.y"
                        { (శూన్య) (yyvsp[0].సమాసం); (yyval.సమాసం) = ఖాళీ; }
#పంక్తి 3062 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#పంక్తి 753 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = ఖాళీ; }
#పంక్తి 3068 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#పంక్తి 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).తల); (yyval.geom) = ఖాళీ; }
#పంక్తి 3074 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 126: /* DoodadType: "text"  */
#పంక్తి 760 "src/xkbcomp/parser.y"
                                { (yyval.అంకె) = 0; }
#పంక్తి 3080 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 127: /* DoodadType: "outline"  */
#పంక్తి 761 "src/xkbcomp/parser.y"
                                { (yyval.అంకె) = 0; }
#పంక్తి 3086 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 128: /* DoodadType: "solid"  */
#పంక్తి 762 "src/xkbcomp/parser.y"
                                { (yyval.అంకె) = 0; }
#పంక్తి 3092 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 129: /* DoodadType: "logo"  */
#పంక్తి 763 "src/xkbcomp/parser.y"
                                { (yyval.అంకె) = 0; }
#పంక్తి 3098 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 130: /* FieldSpec: Ident  */
#పంక్తి 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#పంక్తి 3104 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 131: /* FieldSpec: Element  */
#పంక్తి 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#పంక్తి 3110 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 132: /* Element: "action"  */
#పంక్తి 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#పంక్తి 3116 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 133: /* Element: "interpret"  */
#పంక్తి 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#పంక్తి 3122 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 134: /* Element: "type"  */
#పంక్తి 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#పంక్తి 3128 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 135: /* Element: "key"  */
#పంక్తి 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#పంక్తి 3134 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 136: /* Element: "group"  */
#పంక్తి 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#పంక్తి 3140 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 137: /* Element: "modifier_map"  */
#పంక్తి 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#పంక్తి 3146 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 138: /* Element: "indicator"  */
#పంక్తి 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#పంక్తి 3152 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 139: /* Element: "shape"  */
#పంక్తి 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#పంక్తి 3158 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 140: /* Element: "row"  */
#పంక్తి 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#పంక్తి 3164 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 141: /* Element: "section"  */
#పంక్తి 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#పంక్తి 3170 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 142: /* Element: "text"  */
#పంక్తి 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#పంక్తి 3176 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 143: /* OptMergeMode: MergeMode  */
#పంక్తి 794 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = (yyvsp[0].విలీనం); }
#పంక్తి 3182 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 144: /* OptMergeMode: %empty  */
#పంక్తి 795 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = విలీనం_DEFAULT; }
#పంక్తి 3188 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 145: /* MergeMode: "include"  */
#పంక్తి 798 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = విలీనం_DEFAULT; }
#పంక్తి 3194 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 146: /* MergeMode: "augment"  */
#పంక్తి 799 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = విలీనం_AUGMENT; }
#పంక్తి 3200 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 147: /* MergeMode: "override"  */
#పంక్తి 800 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = విలీనం_OVERRIDE; }
#పంక్తి 3206 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 148: /* MergeMode: "replace"  */
#పంక్తి 801 "src/xkbcomp/parser.y"
                                        { (yyval.విలీనం) = విలీనం_REPLACE; }
#పంక్తి 3212 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 149: /* MergeMode: "alternate"  */
#పంక్తి 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_సందేశం_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.విలీనం) = విలీనం_DEFAULT;
                }
#పంక్తి 3226 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 150: /* ExprList: ExprList "," Expr  */
#పంక్తి 815 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[0].సమాసం)) {
                                ఒకవేళ ((yyvsp[-2].సమాసం_List).తల) {
                                    (yyval.సమాసం_List).తల = (yyvsp[-2].సమాసం_List).తల;
                                    (yyval.సమాసం_List).చివరి->common.తదుపరి = &(yyvsp[0].సమాసం)->common;
                                    (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం);
                                } లేకపోతే {
                                    (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం);
                                }
                            }
                        }
#పంక్తి 3242 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 151: /* ExprList: Expr  */
#పంక్తి 827 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3248 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 152: /* ExprList: %empty  */
#పంక్తి 828 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = ఖాళీ; }
#పంక్తి 3254 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 153: /* Expr: Expr "/" Expr  */
#పంక్తి 832 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_ద్విమాన(STMT_సమాసం_DIVIDE, (yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 3260 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 154: /* Expr: Expr "+" Expr  */
#పంక్తి 834 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_ద్విమాన(STMT_సమాసం_కూడు, (yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 3266 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 155: /* Expr: Expr "-" Expr  */
#పంక్తి 836 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_ద్విమాన(STMT_సమాసం_వ్యవకలనం, (yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 3272 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 156: /* Expr: Expr "*" Expr  */
#పంక్తి 838 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_ద్విమాన(STMT_సమాసం_MULTIPLY, (yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 3278 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 157: /* Expr: Lhs "=" Expr  */
#పంక్తి 840 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_ద్విమాన(STMT_సమాసం_కేటాయించు, (yyvsp[-2].సమాసం), (yyvsp[0].సమాసం)); }
#పంక్తి 3284 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 158: /* Expr: Term  */
#పంక్తి 842 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3290 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 159: /* Term: "-" Term  */
#పంక్తి 846 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Unary(STMT_సమాసం_NEGATE, (yyvsp[0].సమాసం)); }
#పంక్తి 3296 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 160: /* Term: "+" Term  */
#పంక్తి 848 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Unary(STMT_సమాసం_UNARY_PLUS, (yyvsp[0].సమాసం)); }
#పంక్తి 3302 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 161: /* Term: "!" Term  */
#పంక్తి 850 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Unary(STMT_సమాసం_NOT, (yyvsp[0].సమాసం)); }
#పంక్తి 3308 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 162: /* Term: "~" Term  */
#పంక్తి 852 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Unary(STMT_సమాసం_INVERT, (yyvsp[0].సమాసం)); }
#పంక్తి 3314 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 163: /* Term: Lhs  */
#పంక్తి 854 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3320 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 164: /* Term: FieldSpec "(" ExprList ")"  */
#పంక్తి 856 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Action((yyvsp[-3].atom), (yyvsp[-1].సమాసం_List).తల); }
#పంక్తి 3326 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 165: /* Term: Actions  */
#పంక్తి 858 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3332 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 166: /* Term: Terminal  */
#పంక్తి 860 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3338 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 167: /* Term: "(" Expr ")"  */
#పంక్తి 862 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[-1].సమాసం); }
#పంక్తి 3344 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 168: /* MultiActionList: MultiActionList "," Action  */
#పంక్తి 866 "src/xkbcomp/parser.y"
                        {
                            సమాసం_Def *సమాసం = సమాసం_Create_Action_List((yyvsp[0].సమాసం));
                            (yyval.సమాసం_List) = (yyvsp[-2].సమాసం_List);
                            (yyval.సమాసం_List).చివరి->common.తదుపరి = &సమాసం->common; (yyval.సమాసం_List).చివరి = సమాసం;
                        }
#పంక్తి 3354 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 169: /* MultiActionList: MultiActionList "," Actions  */
#పంక్తి 872 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List) = (yyvsp[-2].సమాసం_List); (yyval.సమాసం_List).చివరి->common.తదుపరి = &(yyvsp[0].సమాసం)->common; (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3360 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 170: /* MultiActionList: Action  */
#పంక్తి 874 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = సమాసం_Create_Action_List((yyvsp[0].సమాసం)); }
#పంక్తి 3366 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 171: /* MultiActionList: NonEmptyActions  */
#పంక్తి 876 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3372 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 172: /* ActionList: ActionList "," Action  */
#పంక్తి 880 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List) = (yyvsp[-2].సమాసం_List); (yyval.సమాసం_List).చివరి->common.తదుపరి = &(yyvsp[0].సమాసం)->common; (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3378 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 173: /* ActionList: Action  */
#పంక్తి 882 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3384 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 174: /* NonEmptyActions: "{" ActionList "}"  */
#పంక్తి 886 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Action_List((yyvsp[-1].సమాసం_List).తల); }
#పంక్తి 3390 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 175: /* Actions: NonEmptyActions  */
#పంక్తి 890 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3396 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 176: /* Actions: "{" "}"  */
#పంక్తి 892 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Action_List(ఖాళీ); }
#పంక్తి 3402 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 177: /* Action: FieldSpec "(" ExprList ")"  */
#పంక్తి 896 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Action((yyvsp[-3].atom), (yyvsp[-1].సమాసం_List).తల); }
#పంక్తి 3408 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 178: /* Lhs: FieldSpec  */
#పంక్తి 900 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Ident((yyvsp[0].atom)); }
#పంక్తి 3414 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 179: /* Lhs: FieldSpec "." FieldSpec  */
#పంక్తి 902 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_క్షేత్రం_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#పంక్తి 3420 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 180: /* Lhs: FieldSpec "[" Expr "]"  */
#పంక్తి 904 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].సమాసం)); }
#పంక్తి 3426 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#పంక్తి 906 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].సమాసం)); }
#పంక్తి 3432 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 182: /* OptTerminal: Terminal  */
#పంక్తి 910 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3438 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 183: /* OptTerminal: %empty  */
#పంక్తి 911 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = ఖాళీ; }
#పంక్తి 3444 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 184: /* Terminal: String  */
#పంక్తి 915 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_String((yyvsp[0].atom)); }
#పంక్తి 3450 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 185: /* Terminal: Integer  */
#పంక్తి 917 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Integer((yyvsp[0].అంకె)); }
#పంక్తి 3456 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 186: /* Terminal: Float  */
#పంక్తి 919 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_Float(/* Discard $1 */); }
#పంక్తి 3462 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 187: /* Terminal: "key name"  */
#పంక్తి 921 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_కీలకం_Name((yyvsp[0].atom)); }
#పంక్తి 3468 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#పంక్తి 925 "src/xkbcomp/parser.y"
                        {
                            సమాసం_Def *సమాసం = సమాసం_Create_కీలకం_Sym_List((yyvsp[0].keysym));
                            (yyval.సమాసం_List) = (yyvsp[-2].సమాసం_List);
                            (yyval.సమాసం_List).చివరి->common.తదుపరి = &సమాసం->common; (yyval.సమాసం_List).చివరి = సమాసం;
                        }
#పంక్తి 3478 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#పంక్తి 931 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List) = (yyvsp[-2].సమాసం_List); (yyval.సమాసం_List).చివరి->common.తదుపరి = &(yyvsp[0].సమాసం)->common; (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3484 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 190: /* MultiKeySymList: KeySymLit  */
#పంక్తి 933 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = సమాసం_Create_కీలకం_Sym_List((yyvsp[0].keysym)); }
#పంక్తి 3490 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 191: /* MultiKeySymList: NonEmptyKeySyms  */
#పంక్తి 935 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం_List).తల = (yyval.సమాసం_List).చివరి = (yyvsp[0].సమాసం); }
#పంక్తి 3496 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 192: /* KeySymList: KeySymList "," KeySymLit  */
#పంక్తి 939 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Append_కీలకం_Sym_List((yyvsp[-2].సమాసం), (yyvsp[0].keysym)); }
#పంక్తి 3502 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 193: /* KeySymList: KeySymList "," "string literal"  */
#పంక్తి 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.సమాసం) = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, (yyvsp[-2].సమాసం), (yyvsp[0].str));
                            విడుదల((yyvsp[0].str));
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                        }
#పంక్తి 3513 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 194: /* KeySymList: KeySymLit  */
#పంక్తి 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.సమాసం) = సమాసం_Create_కీలకం_Sym_List((yyvsp[0].keysym));
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                        }
#పంక్తి 3523 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 195: /* KeySymList: "string literal"  */
#పంక్తి 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.సమాసం) = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                            (yyval.సమాసం) = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, (yyval.సమాసం), (yyvsp[0].str));
                            విడుదల((yyvsp[0].str));
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                        }
#పంక్తి 3537 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#పంక్తి 966 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[-1].సమాసం); }
#పంక్తి 3543 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 197: /* NonEmptyKeySyms: "string literal"  */
#పంక్తి 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.సమాసం) = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                            (yyval.సమాసం) = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, (yyval.సమాసం), (yyvsp[0].str));
                            విడుదల((yyvsp[0].str));
                            ఒకవేళ (!(yyval.సమాసం))
                                YYERROR;
                        }
#పంక్తి 3557 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 198: /* KeySyms: NonEmptyKeySyms  */
#పంక్తి 980 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = (yyvsp[0].సమాసం); }
#పంక్తి 3563 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 199: /* KeySyms: "{" "}"  */
#పంక్తి 982 "src/xkbcomp/parser.y"
                        { (yyval.సమాసం) = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం); }
#పంక్తి 3569 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 200: /* KeySym: KeySymLit  */
#పంక్తి 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#పంక్తి 3575 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 201: /* KeySym: "string literal"  */
#పంక్తి 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            విడుదల((yyvsp[0].str));
                            ఒకవేళ ((yyval.keysym) == XKB_కీలకం_No_చిహ్నం)
                                YYERROR;
                        }
#పంక్తి 3586 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 202: /* KeySymLit: "identifier"  */
#పంక్తి 997 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (గుర్తులేని పూర్ణాంకం) (yyvsp[0].sval).len, (yyvsp[0].sval).ప్రారంభం
                                );
                                (yyval.keysym) = XKB_కీలకం_No_చిహ్నం;
                            }
                        }
#పంక్తి 3602 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 203: /* KeySymLit: "section"  */
#పంక్తి 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_కీలకం_section; }
#పంక్తి 3608 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 204: /* KeySymLit: "decimal digit"  */
#పంక్తి 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_కీలకం_0 + (xkb_keysym_t) (yyvsp[0].అంకె);
                        }
#పంక్తి 3621 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 205: /* KeySymLit: "integer literal"  */
#పంక్తి 1020 "src/xkbcomp/parser.y"
                        {
                            ఒకవేళ ((yyvsp[0].అంకె) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].అంకె), (yyvsp[0].అంకె)
                                );
                                (yyval.keysym) = XKB_కీలకం_No_చిహ్నం;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            లేకపోతే {
                                /* Any other numeric value */
                                ఒకవేళ ((yyvsp[0].అంకె) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].అంకె);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), ఖాళీ, (yyval.keysym), "%#06"PRIx32, "");
                                } లేకపోతే {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].అంకె), (yyvsp[0].అంకె)
                                    );
                                    (yyval.keysym) = XKB_కీలకం_No_చిహ్నం;
                                }
                                parser_vrb(
                                    /*
                                     * Require an extra high verbosity, because
                                     * keysyms are formatted as number unless
                                     * enabling pretty-pretting for the
                                     * serialization.
                                     */
                                    param, XKB_LOG_VERBOSITY_COMPREHENSIVE,
                                    XKB_WARNING_NUMERIC_KEYSYM,
                                    "numeric keysym \"%#06"PRIx64"\" (%"PRId64")",
                                    (yyvsp[0].అంకె), (yyvsp[0].అంకె)
                                );
                            }
                        }
#పంక్తి 3683 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 206: /* SignedNumber: "-" Number  */
#పంక్తి 1079 "src/xkbcomp/parser.y"
                                        { (yyval.అంకె) = -(yyvsp[0].అంకె); }
#పంక్తి 3689 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 207: /* SignedNumber: Number  */
#పంక్తి 1080 "src/xkbcomp/parser.y"
                                        { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3695 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 208: /* Number: "float literal"  */
#పంక్తి 1083 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3701 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 209: /* Number: "decimal digit"  */
#పంక్తి 1084 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3707 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 210: /* Number: "integer literal"  */
#పంక్తి 1085 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3713 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 211: /* Float: "float literal"  */
#పంక్తి 1088 "src/xkbcomp/parser.y"
                                { (yyval.అంకె) = 0; }
#పంక్తి 3719 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 212: /* Integer: "integer literal"  */
#పంక్తి 1091 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3725 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 213: /* Integer: "decimal digit"  */
#పంక్తి 1092 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3731 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 214: /* KeyCode: "integer literal"  */
#పంక్తి 1095 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3737 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 215: /* KeyCode: "decimal digit"  */
#పంక్తి 1096 "src/xkbcomp/parser.y"
                                      { (yyval.అంకె) = (yyvsp[0].అంకె); }
#పంక్తి 3743 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 216: /* Ident: "identifier"  */
#పంక్తి 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).ప్రారంభం, (yyvsp[0].sval).len); }
#పంక్తి 3749 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 217: /* Ident: "default"  */
#పంక్తి 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#పంక్తి 3755 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 218: /* String: "string literal"  */
#పంక్తి 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), అక్షరశ్రేణి_పొడవు((yyvsp[0].str))); విడుదల((yyvsp[0].str)); }
#పంక్తి 3761 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 219: /* OptMapName: MapName  */
#పంక్తి 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#పంక్తి 3767 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 220: /* OptMapName: %empty  */
#పంక్తి 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = ఖాళీ; }
#పంక్తి 3773 "src/xkbcomp/parser.c"
    ఆపు;

  స్థితి 221: /* MapName: "string literal"  */
#పంక్తి 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#పంక్తి 3779 "src/xkbcomp/parser.c"
    ఆపు;


#పంక్తి 3783 "src/xkbcomp/parser.c"

      అప్రమేయ: ఆపు;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_చిహ్నం_PRINT ("-> $$ =", YY_CAST (yysymbol_రకం_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    స్థిరరాశి పూర్ణాంకం yylhs = yyr1[yyn] - YYNTOKENS;
    స్థిరరాశి పూర్ణాంకం yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  వెళ్ళు yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  ఒకవేళ (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        అక్షరం స్థిరరాశి *yymsgp = YY_("syntax error");
        పూర్ణాంకం yysyntax_error_స్థితి;
        yysyntax_error_స్థితి = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        ఒకవేళ (yysyntax_error_స్థితి == 0)
          yymsgp = yymsg;
        లేకపోతే ఒకవేళ (yysyntax_error_స్థితి == -1)
          {
            ఒకవేళ (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (అక్షరం *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            ఒకవేళ (yymsg)
              {
                yysyntax_error_స్థితి
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            లేకపోతే
              {
                yymsg = yymsgbuf;
                yymsg_alloc = పరిమాణం yymsgbuf;
                yysyntax_error_స్థితి = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        ఒకవేళ (yysyntax_error_స్థితి == YYENOMEM)
          YYNOMEM;
      }
    }

  ఒకవేళ (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      ఒకవేళ (yychar <= చివర_యొక్క_FILE)
        {
          /* Return failure if at end of input.  */
          ఒకవేళ (yychar == చివర_యొక్క_FILE)
            YYABORT;
        }
      లేకపోతే
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  వెళ్ళు yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  ఒకవేళ (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_దొంతర_PRINT (yyss, yyssp);
  yystate = *yyssp;
  వెళ్ళు yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  కొరకు (;;)
    {
      yyn = yypact[yystate];
      ఒకవేళ (!yypact_విలువ_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          ఒకవేళ (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              ఒకవేళ (0 < yyn)
                ఆపు;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      ఒకవేళ (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_చిహ్నం (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_దొంతర_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_చివర


  /* Shift the error token.  */
  YY_చిహ్నం_PRINT ("Shifting", YY_ACCESSING_చిహ్నం (yyn), yyvsp, yylsp);

  yystate = yyn;
  వెళ్ళు yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  వెళ్ళు yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  వెళ్ళు yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  వెళ్ళు yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  ఒకవేళ (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, param);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_దొంతర_PRINT (yyss, yyssp);
  అయితే (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_చిహ్నం (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#నిర్వచితం_కాకపోతే yyoverflow
  ఒకవేళ (yyss != yyssa)
    YYSTACK_FREE (yyss);
#అయితే_ముగింపు
  ఒకవేళ (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  తిరిగి yyresult;
}

#పంక్తి 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(నిర్మాణం xkb_context *ctx, స్థిరరాశి నిర్మాణం parser_keymap_config *config,
      నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *map)
{
    పూర్ణాంకం ret;
    XkbFile *మొదటి = ఖాళీ;
    నిర్మాణం parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ఖాళీ,
        .config = *config,
        .మరిన్ని_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    అయితే ((ret = yyparse(&param)) == 0 && param.మరిన్ని_maps) {
        ఒకవేళ (map) {
            ఒకవేళ (streq_not_null(map, param.rtrn->name))
                తిరిగి param.rtrn;
            లేకపోతే
                FreeXkbFile(param.rtrn);
        }
        లేకపోతే {
            ఒకవేళ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(మొదటి);
                తిరిగి param.rtrn;
            }
            లేకపోతే ఒకవేళ (!మొదటి) {
                మొదటి = param.rtrn;
            }
            లేకపోతే {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ఖాళీ;
    }

    ఒకవేళ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(మొదటి);
        FreeXkbFile(param.rtrn);
        తిరిగి ఖాళీ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ఒకవేళ (మొదటి && మొదటి->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, సురక్షిత_map_name(మొదటి->name));

    తిరిగి మొదటి;
}

/* Parse the next section */
bool
parse_తదుపరి(నిర్మాణం xkb_context *ctx,
           స్థిరరాశి నిర్మాణం parser_keymap_config *config,
           నిర్మాణం scanner *scanner, XkbFile **xkb_file)
{
    పూర్ణాంకం ret;
    నిర్మాణం parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ఖాళీ,
        .config = *config,
        .మరిన్ని_maps = false,
    };

    ఒకవేళ ((ret = yyparse(&param)) == 0 && param.మరిన్ని_maps) {
        *xkb_file = param.rtrn;
        తిరిగి true;
    } లేకపోతే {
        FreeXkbFile(param.rtrn);
        *xkb_file = ఖాళీ;
        తిరిగి (ret == 0);
    }
}
