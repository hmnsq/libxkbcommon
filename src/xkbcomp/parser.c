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
#घोषणा YYBISON 30802

/* Bison version string.  */
#घोषणा YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#घोषणा YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#घोषणा YYPURE 1

/* Push parsers.  */
#घोषणा YYPUSH 0

/* Pull parsers.  */
#घोषणा YYPULL 1


/* Substitute the variable and function names.  */
#घोषणा yyparse         _xkbcommon_parse
#घोषणा yylex           _xkbcommon_lex
#घोषणा yyerror         _xkbcommon_error
#घोषणा yydebug         _xkbcommon_debug
#घोषणा yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#पंक्ति 22 "src/xkbcomp/parser.y"

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "keysym.h"
#समावेश "xkbcomp/xkbcomp-priv.h"
#समावेश "xkbcomp/parser-priv.h"
#समावेश "xkbcomp/ast-build.h"

काष्ठा parser_param {
    काष्ठा xkb_context *ctx;
    काष्ठा scanner *scanner;
    XkbFile *rtrn;
    काष्ठा parser_keymap_config config;
    bool more_maps;
};

#घोषणा parser_log_with_code(param, level, verbosity, log_संदेश_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_संदेश_id, fmt,\
                          ##__VA_ARGS__)

#घोषणा parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#घोषणा parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#घोषणा parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

अटल व्योम
_xkbcommon_error(काष्ठा parser_param *param, स्थिर अक्षर *संदेश)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", संदेश);
}

अटल bool
resolve_keysym(काष्ठा parser_param *param, काष्ठा sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    अगर (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_कुंजी_No_प्रतीक;
        वापस true;
    }

    अगर (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_कुंजी_Void_प्रतीक;
        वापस true;
    }

    /* xkb_keysym_from_name needs a C string. */
    अक्षर buf[XKB_KEYSYM_NAME_MAX_आकार];
    अगर (name.len >= माप(buf)) {
        वापस false;
    }
    स_नकल(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    अगर (sym != XKB_कुंजी_No_प्रतीक) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        वापस true;
    }

    वापस false;
}

#घोषणा param_scanner param->scanner

#पंक्ति 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  अन्यथा
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  अगर defined __cplusplus
#   अगर 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   अन्यथा
#    define YY_NULLPTR 0
#   endif
#  अन्यथा
#   define YY_NULLPTR ((व्योम*)0)
#  endif
# endif

#समावेश "parser.h"
/* Symbol kind.  */
क्रमागत yysymbol_kind_t
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
  YYSYMBOL_कुंजी = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_GROUP = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_कुंजियाँ = 27,                      /* "keys"  */
  YYSYMBOL_ROW = 28,                       /* "row"  */
  YYSYMBOL_SECTION = 29,                   /* "section"  */
  YYSYMBOL_OVERLAY = 30,                   /* "overlay"  */
  YYSYMBOL_TEXT = 31,                      /* "text"  */
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
  YYSYMBOL_ALPHANUMERIC_कुंजियाँ = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_कुंजियाँ = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_कुंजियाँ = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_कुंजियाँ = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_GROUP = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_XkbCompositeMap = 69,           /* XkbCompositeMap  */
  YYSYMBOL_XkbCompositeType = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_कुंजी_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_कुंजी_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_InterpretMatch = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_कुंजी_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_कुंजी_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_प्रतीक_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_GroupCompatDecl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_कुंजी_Or_कुंजी_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_कुंजी_Or_कुंजी_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_SectionBodyItem = 106,          /* SectionBodyItem  */
  YYSYMBOL_RowBody = 107,                  /* RowBody  */
  YYSYMBOL_RowBodyItem = 108,              /* RowBodyItem  */
  YYSYMBOL_कुंजियाँ_2 = 109,                     /*कुंजियाँ*/
  YYSYMBOL_कुंजी_2 = 110,                      /*कुंजी*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_कुंजी_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_कुंजी = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_OutlineInList = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_FieldSpec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_OptMergeMode = 122,             /* OptMergeMode  */
  YYSYMBOL_MergeMode = 123,                /* MergeMode  */
  YYSYMBOL_व्यंजक_List = 124,                 /* ExprList  */
  YYSYMBOL_व्यंजक = 125,                     /*व्यंजक*/
  YYSYMBOL_Term = 126,                     /* Term  */
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_NonEmptyActions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_कुंजी_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_कुंजी_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_Empty_कुंजी_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_कुंजी_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_कुंजी_Sym = 139,                   /* KeySym  */
  YYSYMBOL_कुंजी_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_संख्या = 141,             /* SignedNumber  */
  YYSYMBOL_संख्या = 142,                   /*संख्या*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_कुंजी_Code = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
प्रकार क्रमागत yysymbol_kind_t yysymbol_kind_t;




#अगर_घोषित लघु
# undef लघु
#पूर्ण_अगर

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#अगर_अघोषित __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# अगर defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#पूर्ण_अगर

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#अगर_घोषित __INT_LEAST8_MAX__
प्रकार __INT_LEAST8_TYPE__ yytype_int8;
#या_अगर defined YY_STDINT_H
प्रकार int_least8_t yytype_int8;
#अन्यथा
प्रकार चिन्हित अक्षर yytype_int8;
#पूर्ण_अगर

#अगर_घोषित __INT_LEAST16_MAX__
प्रकार __INT_LEAST16_TYPE__ yytype_int16;
#या_अगर defined YY_STDINT_H
प्रकार int_least16_t yytype_int16;
#अन्यथा
प्रकार लघु yytype_int16;
#पूर्ण_अगर

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#अगर_घोषित __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#पूर्ण_अगर

#अगर defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
प्रकार __UINT_LEAST8_TYPE__ yytype_uint8;
#या_अगर (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= पूर्णांक_उच्च)
प्रकार uint_least8_t yytype_uint8;
#या_अगर !defined __UINT_LEAST8_MAX__ && अच_अक्षर_उच्च <= पूर्णांक_उच्च
प्रकार अचिन्हित अक्षर yytype_uint8;
#अन्यथा
प्रकार लघु yytype_uint8;
#पूर्ण_अगर

#अगर defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
प्रकार __UINT_LEAST16_TYPE__ yytype_uint16;
#या_अगर (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= पूर्णांक_उच्च)
प्रकार uint_least16_t yytype_uint16;
#या_अगर !defined __UINT_LEAST16_MAX__ && अच_लघु_उच्च <= पूर्णांक_उच्च
प्रकार अचिन्हित लघु yytype_uint16;
#अन्यथा
प्रकार पूर्णांक yytype_uint16;
#पूर्ण_अगर

#अगर_अघोषित YYPTRDIFF_T
# अगर defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef सूचक_भेद_प्रकार
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T सूचक_भेद_प्रकार
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# अन्यथा
#  define YYPTRDIFF_T दीर्घ
#  define YYPTRDIFF_MAXIMUM दीर्घ_उच्च
# endif
#पूर्ण_अगर

#अगर_अघोषित YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined माप_प्रकार
#  define YYSIZE_T माप_प्रकार
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T माप_प्रकार
# अन्यथा
#  define YYSIZE_T अचिन्हित
# endif
#पूर्ण_अगर

#घोषणा YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#घोषणा YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, माप (X))


/* Stored state numbers (used for stacks). */
प्रकार yytype_int16 yy_state_t;

/* State numbers in computations.  */
प्रकार पूर्णांक yy_state_fast_t;

#अगर_अघोषित YY_
# अगर defined YYENABLE_NLS && YYENABLE_NLS
#  अगर ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#पूर्ण_अगर


#अगर_अघोषित YY_ATTRIBUTE_PURE
# अगर defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# अन्यथा
#  define YY_ATTRIBUTE_PURE
# endif
#पूर्ण_अगर

#अगर_अघोषित YY_ATTRIBUTE_UNUSED
# अगर defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# अन्यथा
#  define YY_ATTRIBUTE_UNUSED
# endif
#पूर्ण_अगर

/* Suppress unused-variable warnings by "using" E.  */
#अगर ! defined lint || defined __GNUC__
# define YY_USE(E) ((व्योम) (E))
#अन्यथा
# define YY_USE(E) /* empty */
#पूर्ण_अगर

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#अगर defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# अगर __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# अन्यथा
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#अन्यथा
# define YY_INITIAL_मूल्य(मूल्य_2) मूल्य_2
#पूर्ण_अगर
#अगर_अघोषित YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#पूर्ण_अगर
#अगर_अघोषित YY_INITIAL_मूल्य
# define YY_INITIAL_मूल्य(मूल्य_2) /* Nothing. */
#पूर्ण_अगर

#अगर defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#पूर्ण_अगर
#अगर_अघोषित YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#पूर्ण_अगर


#घोषणा YY_ASSERT(E) ((व्योम) (0 && (E)))

#अगर 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  अगर YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   अन्यथा
#    define YYSTACK_ALLOC alloca
#    अगर ! defined _ALLOCA_H && ! defined निकास_सफल
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef निकास_सफल
#      define निकास_सफल 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) करो { /* empty */; } जबतक (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# अन्यथा
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  अगर (defined __cplusplus && ! defined निकास_सफल \
       && ! ((defined YYMALLOC || defined दो_स्मृति) \
             && (defined YYFREE || defined मुक्त)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef निकास_सफल
#    define निकास_सफल 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC दो_स्मृति
#   अगर ! defined दो_स्मृति && ! defined निकास_सफल
व्योम *दो_स्मृति (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE मुक्त
#   अगर ! defined मुक्त && ! defined निकास_सफल
व्योम मुक्त (व्योम *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#पूर्ण_अगर /* 1 */

#अगर (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
जोड़ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (जोड़ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ढेर_alloc, ढेर_2)                           \
    करो                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ढेर_alloc, ढेर_2, yysize);                    \
        ढेर_2 = &yyptr->ढेर_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ढेर_2) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    जबतक (0)

#पूर्ण_अगर

#अगर defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  अगर defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, गिनती_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (गिनती_2)) * माप (*(Src)))
#  अन्यथा
#   define YYCOPY(Dst, Src, गिनती_2)              \
      करो                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          क्रम (yyi = 0; yyi < (गिनती_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      जबतक (0)
#  endif
# endif
#पूर्ण_अगर /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#घोषणा YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#घोषणा YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#घोषणा YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#घोषणा YYNNTS  83
/* YYNRULES -- Number of rules.  */
#घोषणा YYNRULES  221
/* YYNSTATES -- Number of states.  */
#घोषणा YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#घोषणा YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#घोषणा YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
अटल स्थिर yytype_int8 yytranslate[] =
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

#अगर YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
अटल स्थिर yytype_int16 yyrline[] =
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
#पूर्ण_अगर

/** Accessing symbol of state STATE.  */
#घोषणा YY_ACCESSING_प्रतीक(State) YY_CAST (yysymbol_kind_t, yystos[State])

#अगर 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
अटल स्थिर अक्षर *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

अटल स्थिर अक्षर *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  अटल स्थिर अक्षर *स्थिर yy_sname[] =
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
  वापस yy_sname[yysymbol];
}
#पूर्ण_अगर

#घोषणा YYPACT_NINF (-225)

#घोषणा yypact_मूल्य_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#घोषणा YYTABLE_NINF (-217)

#घोषणा yytable_मूल्य_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
अटल स्थिर yytype_int16 yypact[] =
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
अटल स्थिर yytype_uint8 yydefact[] =
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
अटल स्थिर yytype_int16 yypgoto[] =
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
अटल स्थिर yytype_int16 yydefgoto[] =
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
अटल स्थिर yytype_int16 yytable[] =
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

अटल स्थिर yytype_int16 yycheck[] =
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
अटल स्थिर yytype_uint8 yystos[] =
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
अटल स्थिर yytype_uint8 yyr1[] =
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
अटल स्थिर yytype_int8 yyr2[] =
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


क्रमागत { YYENOMEM = -2 };

#घोषणा yyerrok         (yyerrstatus = 0)
#घोषणा yyclearin       (yychar = YYEMPTY)

#घोषणा YYACCEPT        जाओ yyacceptlab
#घोषणा YYABORT         जाओ yyabortlab
#घोषणा YYERROR         जाओ yyerrorlab
#घोषणा YYNOMEM         जाओ yyexhaustedlab


#घोषणा YYRECOVERING()  (!!yyerrstatus)

#घोषणा YYBACKUP(Token, मूल्य_2)                                    \
  करो                                                              \
    अगर (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (मूल्य_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        जाओ yybackup;                                            \
      }                                                           \
    अन्यथा                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  जबतक (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#घोषणा YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#अगर YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF ख_लिखो
# endif

# define YYDPRINTF(Args)                        \
करो {                                            \
  अगर (yydebug)                                  \
    YYFPRINTF Args;                             \
} जबतक (0)




# define YY_प्रतीक_PRINT(Title, Kind, मूल्य_2, Location)                    \
करो {                                                                      \
  अगर (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (मानक_त्रुटि, "%s ", Title);                                   \
      yy_प्रतीक_print (मानक_त्रुटि,                                            \
                  Kind, मूल्य_2, param); \
      YYFPRINTF (मानक_त्रुटि, "\n");                                           \
    }                                                                     \
} जबतक (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

अटल व्योम
yy_प्रतीक_मूल्य_print (खाता *yyo,
                       yysymbol_kind_t yykind, YYSTYPE स्थिर * स्थिर yyvaluep, काष्ठा parser_param *param)
{
  खाता *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  अगर (!yyvaluep)
    वापस;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

अटल व्योम
yy_प्रतीक_print (खाता *yyo,
                 yysymbol_kind_t yykind, YYSTYPE स्थिर * स्थिर yyvaluep, काष्ठा parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_प्रतीक_मूल्य_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

अटल व्योम
yy_ढेर_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (मानक_त्रुटि, "Stack now");
  क्रम (; yybottom <= yytop; yybottom++)
    {
      पूर्णांक yybot = *yybottom;
      YYFPRINTF (मानक_त्रुटि, " %d", yybot);
    }
  YYFPRINTF (मानक_त्रुटि, "\n");
}

# define YY_ढेर_PRINT(Bottom, शीर्ष_2)                            \
करो {                                                            \
  अगर (yydebug)                                                  \
    yy_ढेर_print ((Bottom), (शीर्ष_2));                           \
} जबतक (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

अटल व्योम
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 पूर्णांक yyrule, काष्ठा parser_param *param)
{
  पूर्णांक yylno = yyrline[yyrule];
  पूर्णांक yynrhs = yyr2[yyrule];
  पूर्णांक yyi;
  YYFPRINTF (मानक_त्रुटि, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  क्रम (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (मानक_त्रुटि, "   $%d = ", yyi + 1);
      yy_प्रतीक_print (मानक_त्रुटि,
                       YY_ACCESSING_प्रतीक (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (मानक_त्रुटि, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
करो {                                    \
  अगर (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, param); \
} जबतक (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
पूर्णांक yydebug;
#अन्यथा /* !YYDEBUG */
# define YYDPRINTF(Args) ((व्योम) 0)
# define YY_प्रतीक_PRINT(Title, Kind, मूल्य_2, Location)
# define YY_ढेर_PRINT(Bottom, शीर्ष_2)
# define YY_REDUCE_PRINT(Rule)
#पूर्ण_अगर /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#अगर_अघोषित YYINITDEPTH
# define YYINITDEPTH 200
#पूर्ण_अगर

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#अगर_अघोषित YYMAXDEPTH
# define YYMAXDEPTH 10000
#पूर्ण_अगर


/* Context of a parse error.  */
प्रकार काष्ठा
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
अटल पूर्णांक
yypcontext_expected_tokens (स्थिर yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], पूर्णांक yyargn)
{
  /* Actual size of YYARG. */
  पूर्णांक yycount = 0;
  पूर्णांक yyn = yypact[+*yyctx->yyssp];
  अगर (!yypact_मूल्य_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      पूर्णांक yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      पूर्णांक yychecklim = YYLAST - yyn + 1;
      पूर्णांक yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      पूर्णांक yyx;
      क्रम (yyx = yyxbegin; yyx < yyxend; ++yyx)
        अगर (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_मूल्य_is_error (yytable[yyx + yyn]))
          {
            अगर (!yyarg)
              ++yycount;
            अन्यथा अगर (yycount == yyargn)
              वापस 0;
            अन्यथा
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  अगर (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  वापस yycount;
}




#अगर_अघोषित yystrlen
# अगर defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, म_माप (S)))
# अन्यथा
/* Return the length of YYSTR.  */
अटल YYPTRDIFF_T
yystrlen (स्थिर अक्षर *yystr)
{
  YYPTRDIFF_T yylen;
  क्रम (yylen = 0; yystr[yylen]; yylen++)
    जारी;
  वापस yylen;
}
# endif
#पूर्ण_अगर

#अगर_अघोषित yystpcpy
# अगर defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# अन्यथा
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
अटल अक्षर *
yystpcpy (अक्षर *yydest, स्थिर अक्षर *yysrc)
{
  अक्षर *yyd = yydest;
  स्थिर अक्षर *yys = yysrc;

  जबतक ((*yyd++ = *yys++) != '\0')
    जारी;

  वापस yyd - 1;
}
# endif
#पूर्ण_अगर



अटल पूर्णांक
yy_syntax_error_arguments (स्थिर yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], पूर्णांक yyargn)
{
  /* Actual size of YYARG. */
  पूर्णांक yycount = 0;
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
  अगर (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      पूर्णांक yyn;
      अगर (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      अगर (yyn == YYENOMEM)
        वापस YYENOMEM;
      अन्यथा
        yycount += yyn;
    }
  वापस yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
अटल पूर्णांक
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, अक्षर **yymsg,
                स्थिर yypcontext_t *yyctx)
{
  क्रमागत { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  स्थिर अक्षर *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  पूर्णांक yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  अगर (yycount == YYENOMEM)
    वापस YYENOMEM;

  चयन (yycount)
    {
#घोषणा YYCASE_(N, S)                       \
      हाल N:                               \
        yyformat = S;                       \
        अवरोध
    शेष: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#अघोषित YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    पूर्णांक yyi;
    क्रम (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        अगर (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        अन्यथा
          वापस YYENOMEM;
      }
  }

  अगर (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      अगर (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      वापस -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    अक्षर *yyp = *yymsg;
    पूर्णांक yyi = 0;
    जबतक ((*yyp = *yyformat) != '\0')
      अगर (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      अन्यथा
        {
          ++yyp;
          ++yyformat;
        }
  }
  वापस 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

अटल व्योम
yydestruct (स्थिर अक्षर *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, काष्ठा parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  अगर (!yymsg)
    yymsg = "Deleting";
  YY_प्रतीक_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  चयन (yykind)
    {
    हाल YYSYMBOL_STRING: /* "string literal"  */
#पंक्ति 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पंक्ति 1640 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_XkbFile: /* XkbFile  */
#पंक्ति 257 "src/xkbcomp/parser.y"
            { अगर (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पंक्ति 1646 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_XkbCompositeMap: /* XkbCompositeMap  */
#पंक्ति 257 "src/xkbcomp/parser.y"
            { अगर (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पंक्ति 1652 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#पंक्ति 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).शीर्ष); }
#पंक्ति 1658 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#पंक्ति 257 "src/xkbcomp/parser.y"
            { अगर (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पंक्ति 1664 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_DeclList: /* DeclList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).शीर्ष); }
#पंक्ति 1670 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Decl: /* Decl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#पंक्ति 1676 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_VarDecl: /* VarDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#पंक्ति 1682 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Name_Decl: /* KeyNameDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुंजी_Code)); }
#पंक्ति 1688 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Alias_Decl: /* KeyAliasDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुंजी_Alias)); }
#पंक्ति 1694 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_VModDecl: /* VModDecl  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).शीर्ष); }
#पंक्ति 1700 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_VModDefList: /* VModDefList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).शीर्ष); }
#पंक्ति 1706 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_VModDef: /* VModDef  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#पंक्ति 1712 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#पंक्ति 1718 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_InterpretMatch: /* InterpretMatch  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#पंक्ति 1724 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_VarDeclList: /* VarDeclList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पंक्ति 1730 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Type_Decl: /* KeyTypeDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुंजी_Type)); }
#पंक्ति 1736 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#पंक्ति 1742 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पंक्ति 1748 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पंक्ति 1754 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#पंक्ति 1760 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Multi_कुंजी_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1766 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_GroupCompatDecl: /* GroupCompatDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).groupCompat)); }
#पंक्ति 1772 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).modMask)); }
#पंक्ति 1778 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Or_कुंजी_Sym_List: /* KeyOrKeySymList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक_List).शीर्ष); }
#पंक्ति 1784 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Or_कुंजी_Sym: /* KeyOrKeySym  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1790 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#पंक्ति 1796 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#पंक्ति 1802 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_CoordList: /* CoordList  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1808 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Coord: /* Coord  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1814 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_व्यंजक_List: /* ExprList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक_List).शीर्ष); }
#पंक्ति 1820 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_व्यंजक: /*व्यंजक*/
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1826 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Term: /* Term  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1832 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_MultiActionList: /* MultiActionList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक_List).शीर्ष); }
#पंक्ति 1838 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_ActionList: /* ActionList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक_List).शीर्ष); }
#पंक्ति 1844 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_NonEmptyActions: /* NonEmptyActions  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1850 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Actions: /* Actions  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1856 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Action: /* Action  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1862 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Lhs: /* Lhs  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1868 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_OptTerminal: /* OptTerminal  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1874 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Terminal: /* Terminal  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1880 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Multi_कुंजी_Sym_List: /* MultiKeySymList  */
#पंक्ति 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक_List).शीर्ष); }
#पंक्ति 1886 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Sym_List: /* KeySymList  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1892 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_Non_Empty_कुंजी_Syms: /* NonEmptyKeySyms  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1898 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_कुंजी_Syms: /* KeySyms  */
#पंक्ति 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यंजक)); }
#पंक्ति 1904 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_OptMapName: /* OptMapName  */
#पंक्ति 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पंक्ति 1910 "src/xkbcomp/parser.c"
        अवरोध;

    हाल YYSYMBOL_MapName: /* MapName  */
#पंक्ति 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पंक्ति 1916 "src/xkbcomp/parser.c"
        अवरोध;

      शेष:
        अवरोध;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

पूर्णांक
yyparse (काष्ठा parser_param *param)
{
/* Lookahead token kind.  */
पूर्णांक yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_मूल्य (अटल YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_मूल्य (= yyval_default);

    /* Number of syntax errors so far.  */
    पूर्णांक yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    पूर्णांक yyerrstatus = 0;

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

  पूर्णांक yyn;
  /* The return value of yyparse.  */
  पूर्णांक yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  अक्षर yymsgbuf[128];
  अक्षर *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = माप yymsgbuf;

#घोषणा YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  पूर्णांक yylen = 0;

  YYDPRINTF ((मानक_त्रुटि, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  जाओ yysetstate;


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
  YYDPRINTF ((मानक_त्रुटि, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_ढेर_PRINT (yyss, yyssp);

  अगर (yyss + yystacksize - 1 <= yyssp)
#अगर !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#अन्यथा
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# अगर defined yyoverflow
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
# अन्यथा /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      अगर (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      अगर (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        जोड़ yyalloc *yyptr =
          YY_CAST (जोड़ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        अगर (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        अगर (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((मानक_त्रुटि, "Stack size increased to %ld\n",
                  YY_CAST (दीर्घ, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      अगर (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#पूर्ण_अगर /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  अगर (yystate == YYFINAL)
    YYACCEPT;

  जाओ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  अगर (yypact_मूल्य_is_default (yyn))
    जाओ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  अगर (yychar == YYEMPTY)
    {
      YYDPRINTF ((मानक_त्रुटि, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  अगर (yychar <= END_OF_FILE)
    {
      yychar = END_OF_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((मानक_त्रुटि, "Now at end of input.\n"));
    }
  अन्यथा अगर (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      जाओ yyerrlab1;
    }
  अन्यथा
    {
      yytoken = YYTRANSLATE (yychar);
      YY_प्रतीक_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  अगर (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    जाओ yydefault;
  yyn = yytable[yyn];
  अगर (yyn <= 0)
    {
      अगर (yytable_मूल्य_is_error (yyn))
        जाओ yyerrlab;
      yyn = -yyn;
      जाओ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  अगर (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_प्रतीक_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  जाओ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  अगर (yyn == 0)
    जाओ yyerrlab;
  जाओ yyreduce;


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
  चयन (yyn)
    {
  हाल 2: /* XkbFile: XkbCompositeMap  */
#पंक्ति 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->more_maps = !!param->rtrn; (व्योम) yynerrs; }
#पंक्ति 2195 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 3: /* XkbFile: XkbMapConfig  */
#पंक्ति 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->more_maps = !!param->rtrn; YYACCEPT; }
#पंक्ति 2201 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 4: /* XkbFile: "end of file"  */
#पंक्ति 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = शून्य; param->more_maps = false; }
#पंक्ति 2207 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#पंक्ति 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).शीर्ष, (yyvsp[-6].mapFlags)); }
#पंक्ति 2213 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 6: /* XkbCompositeType: "xkb_keymap"  */
#पंक्ति 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पंक्ति 2219 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 7: /* XkbCompositeType: "xkb_semantics"  */
#पंक्ति 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पंक्ति 2225 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 8: /* XkbCompositeType: "xkb_layout"  */
#पंक्ति 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पंक्ति 2231 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#पंक्ति 298 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[0].file)) {
                                अगर ((yyvsp[-1].fileList).शीर्ष) {
                                    (yyval.fileList).शीर्ष = (yyvsp[-1].fileList).शीर्ष;
                                    (yyval.fileList).last->common.अग्र = &(yyvsp[0].file)->common;
                                    (yyval.fileList).last = (yyvsp[0].file);
                                } अन्यथा {
                                    (yyval.fileList).शीर्ष = (yyval.fileList).last = (yyvsp[0].file);
                                }
                            }
                        }
#पंक्ति 2247 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 10: /* XkbMapConfigList: %empty  */
#पंक्ति 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).शीर्ष = (yyval.fileList).last = शून्य; }
#पंक्ति 2253 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#पंक्ति 315 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).शीर्ष, (yyvsp[-6].mapFlags));
                        }
#पंक्ति 2266 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 12: /* FileType: "xkb_keycodes"  */
#पंक्ति 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#पंक्ति 2272 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 13: /* FileType: "xkb_types"  */
#पंक्ति 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#पंक्ति 2278 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 14: /* FileType: "xkb_compatibility"  */
#पंक्ति 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#पंक्ति 2284 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 15: /* FileType: "xkb_symbols"  */
#पंक्ति 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#पंक्ति 2290 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 16: /* FileType: "xkb_geometry"  */
#पंक्ति 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#पंक्ति 2296 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 17: /* OptFlags: Flags  */
#पंक्ति 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#पंक्ति 2302 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 18: /* OptFlags: %empty  */
#पंक्ति 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#पंक्ति 2308 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 19: /* Flags: Flags Flag  */
#पंक्ति 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#पंक्ति 2314 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 20: /* Flags: Flag  */
#पंक्ति 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#पंक्ति 2320 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 21: /* Flag: "partial"  */
#पंक्ति 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#पंक्ति 2326 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 22: /* Flag: "default"  */
#पंक्ति 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#पंक्ति 2332 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 23: /* Flag: "hidden"  */
#पंक्ति 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#पंक्ति 2338 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 24: /* Flag: "alphanumeric_keys"  */
#पंक्ति 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#पंक्ति 2344 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 25: /* Flag: "modifier_keys"  */
#पंक्ति 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#पंक्ति 2350 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 26: /* Flag: "keypad_keys"  */
#पंक्ति 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#पंक्ति 2356 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 27: /* Flag: "function_keys"  */
#पंक्ति 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#पंक्ति 2362 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 28: /* Flag: "alternate_group"  */
#पंक्ति 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#पंक्ति 2368 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 29: /* Flag: "deprecated"  */
#पंक्ति 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#पंक्ति 2374 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 30: /* Flag: "identifier"  */
#पंक्ति 350 "src/xkbcomp/parser.y"
                        {
                            स्थिर bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (अचिन्हित)(yyvsp[0].sval).len, (yyvsp[0].sval).start,
                                (error ? "" : "; ignored")
                            );
                            अगर (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#पंक्ति 2393 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 31: /* DeclList: DeclList Decl  */
#पंक्ति 367 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[0].any)) {
                                अगर ((yyvsp[-1].anyList).शीर्ष) {
                                    (yyval.anyList).शीर्ष = (yyvsp[-1].anyList).शीर्ष; (yyvsp[-1].anyList).last->अग्र = (yyvsp[0].any); (yyval.anyList).last = (yyvsp[0].any);
                                } अन्यथा {
                                    (yyval.anyList).शीर्ष = (yyval.anyList).last = (yyvsp[0].any);
                                }
                            }
                        }
#पंक्ति 2407 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#पंक्ति 382 "src/xkbcomp/parser.y"
                        {
                            क्रम (VModDef *vmod = (yyvsp[0].vmodList).शीर्ष; vmod; vmod = (VModDef *) vmod->common.अग्र)
                                vmod->merge = (yyvsp[-1].merge);
                            अगर ((yyvsp[-2].anyList).शीर्ष) {
                                (yyval.anyList).शीर्ष = (yyvsp[-2].anyList).शीर्ष; (yyvsp[-2].anyList).last->अग्र = &(yyvsp[0].vmodList).शीर्ष->common; (yyval.anyList).last = &(yyvsp[0].vmodList).last->common;
                            } अन्यथा {
                                (yyval.anyList).शीर्ष = &(yyvsp[0].vmodList).शीर्ष->common; (yyval.anyList).last = &(yyvsp[0].vmodList).last->common;
                            }
                        }
#पंक्ति 2421 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 33: /* DeclList: %empty  */
#पंक्ति 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).शीर्ष = (yyval.anyList).last = शून्य; }
#पंक्ति 2427 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 34: /* Decl: OptMergeMode VarDecl  */
#पंक्ति 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#पंक्ति 2436 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 35: /* Decl: OptMergeMode InterpretDecl  */
#पंक्ति 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#पंक्ति 2445 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 36: /* Decl: OptMergeMode KeyNameDecl  */
#पंक्ति 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुंजी_Code)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुंजी_Code);
                        }
#पंक्ति 2454 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 37: /* Decl: OptMergeMode KeyAliasDecl  */
#पंक्ति 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुंजी_Alias)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुंजी_Alias);
                        }
#पंक्ति 2463 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 38: /* Decl: OptMergeMode KeyTypeDecl  */
#पंक्ति 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुंजी_Type)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुंजी_Type);
                        }
#पंक्ति 2472 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 39: /* Decl: OptMergeMode SymbolsDecl  */
#पंक्ति 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#पंक्ति 2481 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 40: /* Decl: OptMergeMode ModMapDecl  */
#पंक्ति 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].modMask)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].modMask);
                        }
#पंक्ति 2490 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 41: /* Decl: OptMergeMode GroupCompatDecl  */
#पंक्ति 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].groupCompat)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].groupCompat);
                        }
#पंक्ति 2499 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 42: /* Decl: OptMergeMode LedMapDecl  */
#पंक्ति 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#पंक्ति 2508 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 43: /* Decl: OptMergeMode LedNameDecl  */
#पंक्ति 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#पंक्ति 2517 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 44: /* Decl: OptMergeMode ShapeDecl  */
#पंक्ति 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = शून्य; }
#पंक्ति 2523 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 45: /* Decl: OptMergeMode SectionDecl  */
#पंक्ति 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = शून्य; }
#पंक्ति 2529 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 46: /* Decl: OptMergeMode DoodadDecl  */
#पंक्ति 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = शून्य; }
#पंक्ति 2535 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 47: /* Decl: OptMergeMode UnknownDecl  */
#पंक्ति 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#पंक्ति 2541 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#पंक्ति 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#पंक्ति 2547 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 49: /* Decl: MergeMode "string literal"  */
#पंक्ति 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].merge));
                            मुक्त((yyvsp[0].str));
                        }
#पंक्ति 2556 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 50: /* VarDecl: Lhs "=" Expr ";"  */
#पंक्ति 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].व्यंजक), (yyvsp[-1].व्यंजक)); }
#पंक्ति 2562 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 51: /* VarDecl: Ident ";"  */
#पंक्ति 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#पंक्ति 2568 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 52: /* VarDecl: "!" Ident ";"  */
#पंक्ति 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#पंक्ति 2574 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#पंक्ति 468 "src/xkbcomp/parser.y"
                        { (yyval.कुंजी_Code) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].अंक)); }
#पंक्ति 2580 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#पंक्ति 472 "src/xkbcomp/parser.y"
                        { (yyval.कुंजी_Alias) = कुंजी_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#पंक्ति 2586 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#पंक्ति 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#पंक्ति 2592 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 56: /* VModDefList: VModDefList "," VModDef  */
#पंक्ति 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).शीर्ष = (yyvsp[-2].vmodList).शीर्ष; (yyval.vmodList).last->common.अग्र = &(yyvsp[0].vmod)->common; (yyval.vmodList).last = (yyvsp[0].vmod); }
#पंक्ति 2598 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 57: /* VModDefList: VModDef  */
#पंक्ति 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).शीर्ष = (yyval.vmodList).last = (yyvsp[0].vmod); }
#पंक्ति 2604 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 58: /* VModDef: Ident  */
#पंक्ति 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), शून्य); }
#पंक्ति 2610 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 59: /* VModDef: Ident "=" Expr  */
#पंक्ति 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].व्यंजक)); }
#पंक्ति 2616 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#पंक्ति 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).शीर्ष; (yyval.interp) = (yyvsp[-4].interp); }
#पंक्ति 2622 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 61: /* InterpretMatch: KeySym "+" Expr  */
#पंक्ति 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].व्यंजक)); }
#पंक्ति 2628 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 62: /* InterpretMatch: KeySym  */
#पंक्ति 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), शून्य); }
#पंक्ति 2634 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 63: /* VarDeclList: VarDeclList VarDecl  */
#पंक्ति 504 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[0].var)) {
                                अगर ((yyvsp[-1].varList).शीर्ष) {
                                    (yyval.varList).शीर्ष = (yyvsp[-1].varList).शीर्ष;
                                    (yyval.varList).last->common.अग्र = &(yyvsp[0].var)->common;
                                    (yyval.varList).last = (yyvsp[0].var);
                                } अन्यथा {
                                    (yyval.varList).शीर्ष = (yyval.varList).last = (yyvsp[0].var);
                                }
                            }
                        }
#पंक्ति 2650 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 64: /* VarDeclList: %empty  */
#पंक्ति 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyval.varList).last = शून्य; }
#पंक्ति 2656 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#पंक्ति 521 "src/xkbcomp/parser.y"
                        { (yyval.कुंजी_Type) = कुंजी_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पंक्ति 2662 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#पंक्ति 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पंक्ति 2668 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 67: /* OptSymbolsBody: SymbolsBody  */
#पंक्ति 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#पंक्ति 2674 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 68: /* OptSymbolsBody: %empty  */
#पंक्ति 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).शीर्ष = (yyval.varList).last = शून्य; }
#पंक्ति 2680 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#पंक्ति 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyvsp[-2].varList).शीर्ष; (yyval.varList).last->common.अग्र = &(yyvsp[0].var)->common; (yyval.varList).last = (yyvsp[0].var); }
#पंक्ति 2686 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 70: /* SymbolsBody: SymbolsVarDecl  */
#पंक्ति 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyval.varList).last = (yyvsp[0].var); }
#पंक्ति 2692 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#पंक्ति 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 2698 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#पंक्ति 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 2704 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 73: /* SymbolsVarDecl: Ident  */
#पंक्ति 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#पंक्ति 2710 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 74: /* SymbolsVarDecl: "!" Ident  */
#पंक्ति 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#पंक्ति 2716 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#पंक्ति 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(शून्य, (yyvsp[0].व्यंजक)); }
#पंक्ति 2722 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#पंक्ति 560 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[-1].व्यंजक_List).शीर्ष; }
#पंक्ति 2728 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#पंक्ति 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            काष्ठा {व्यंजक_Def *शीर्ष; व्यंजक_Def *last;} list = {
                                .शीर्ष = (yyvsp[-1].व्यंजक_List).शीर्ष, .last = (yyvsp[-1].व्यंजक_List).last
                            };
                            क्रम (uint32_t k = 0; k < (yyvsp[-3].no_प्रतीक_Or_Action_List); k++) {
                                व्यंजक_Def* स्थिर syms =
                                    व्यंजक_Create_कुंजी_Sym_List(XKB_कुंजी_No_प्रतीक);
                                अगर (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = syms;
                            }
                            (yyval.व्यंजक) = list.शीर्ष;
                        }
#पंक्ति 2750 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#पंक्ति 580 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[-1].व्यंजक_List).शीर्ष; }
#पंक्ति 2756 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#पंक्ति 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            काष्ठा {व्यंजक_Def *शीर्ष; व्यंजक_Def *last;} list = {
                                .शीर्ष = (yyvsp[-1].व्यंजक_List).शीर्ष, .last = (yyvsp[-1].व्यंजक_List).last
                            };
                            क्रम (uint32_t k = 0; k < (yyvsp[-3].no_प्रतीक_Or_Action_List); k++) {
                                व्यंजक_Def* स्थिर acts = व्यंजक_Create_Action_List(शून्य);
                                अगर (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = acts;
                            }
                            (yyval.व्यंजक) = list.शीर्ष;
                        }
#पंक्ति 2777 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#पंक्ति 604 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Empty_List(); }
#पंक्ति 2783 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#पंक्ति 610 "src/xkbcomp/parser.y"
                        { (yyval.no_प्रतीक_Or_Action_List) = (yyvsp[-3].no_प्रतीक_Or_Action_List) + 1; }
#पंक्ति 2789 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 82: /* NoSymbolOrActionList: "{" "}"  */
#पंक्ति 612 "src/xkbcomp/parser.y"
                        { (yyval.no_प्रतीक_Or_Action_List) = 1; }
#पंक्ति 2795 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 83: /* NoSymbolOrActionList: %empty  */
#पंक्ति 613 "src/xkbcomp/parser.y"
                        { (yyval.no_प्रतीक_Or_Action_List) = 0; }
#पंक्ति 2801 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#पंक्ति 617 "src/xkbcomp/parser.y"
                        { (yyval.groupCompat) = GroupCompatCreate((yyvsp[-3].अंक), (yyvsp[-1].व्यंजक)); }
#पंक्ति 2807 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#पंक्ति 621 "src/xkbcomp/parser.y"
                        {
                            अगर (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                (yyvsp[-4].व्यंजक)->common.type != STMT_व्यंजक_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].व्यंजक));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].व्यंजक_List).शीर्ष);
                                    YYERROR;
                            }
                            (yyval.modMask) = ModMapCreate((yyvsp[-4].व्यंजक), (yyvsp[-2].व्यंजक_List).शीर्ष);
                        }
#पंक्ति 2826 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#पंक्ति 638 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyvsp[-2].व्यंजक_List).शीर्ष; (yyval.व्यंजक_List).last->common.अग्र = &(yyvsp[0].व्यंजक)->common; (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 2832 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 87: /* KeyOrKeySymList: KeyOrKeySym  */
#पंक्ति 640 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 2838 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 88: /* KeyOrKeySym: "key name"  */
#पंक्ति 644 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Name((yyvsp[0].atom)); }
#पंक्ति 2844 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 89: /* KeyOrKeySym: KeySym  */
#पंक्ति 646 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Sym((yyvsp[0].keysym)); }
#पंक्ति 2850 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#पंक्ति 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पंक्ति 2856 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#पंक्ति 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].अंक), (yyvsp[-1].व्यंजक), false); }
#पंक्ति 2862 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#पंक्ति 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].अंक), (yyvsp[-1].व्यंजक), true); }
#पंक्ति 2868 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#पंक्ति 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].व्यंजक));
                            FreeStmt((ParseCommon *) (yyvsp[-1].व्यंजक));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#पंक्ति 2878 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#पंक्ति 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].व्यंजक));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).शीर्ष);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#पंक्ति 2888 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#पंक्ति 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2894 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#पंक्ति 679 "src/xkbcomp/parser.y"
                        { (व्योम) (yyvsp[-2].व्यंजक); (yyval.geom) = शून्य; }
#पंक्ति 2900 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#पंक्ति 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2906 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 98: /* SectionBody: SectionBody SectionBodyItem  */
#पंक्ति 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = शून्य;}
#पंक्ति 2912 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 99: /* SectionBody: SectionBodyItem  */
#पंक्ति 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = शून्य; }
#पंक्ति 2918 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#पंक्ति 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2924 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 101: /* SectionBodyItem: VarDecl  */
#पंक्ति 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = शून्य; }
#पंक्ति 2930 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 102: /* SectionBodyItem: DoodadDecl  */
#पंक्ति 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2936 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 103: /* SectionBodyItem: LedMapDecl  */
#पंक्ति 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = शून्य; }
#पंक्ति 2942 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 104: /* SectionBodyItem: OverlayDecl  */
#पंक्ति 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2948 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 105: /* RowBody: RowBody RowBodyItem  */
#पंक्ति 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = शून्य;}
#पंक्ति 2954 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 106: /* RowBody: RowBodyItem  */
#पंक्ति 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = शून्य; }
#पंक्ति 2960 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#पंक्ति 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = शून्य; }
#पंक्ति 2966 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 108: /* RowBodyItem: VarDecl  */
#पंक्ति 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = शून्य; }
#पंक्ति 2972 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 109: /* Keys: Keys "," Key  */
#पंक्ति 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = शून्य; }
#पंक्ति 2978 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 110: /* Keys: Key  */
#पंक्ति 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = शून्य; }
#पंक्ति 2984 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 111: /* Key: "key name"  */
#पंक्ति 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 2990 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 112: /* Key: "{" ExprList "}"  */
#पंक्ति 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].व्यंजक_List).शीर्ष); (yyval.geom) = शून्य; }
#पंक्ति 2996 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#पंक्ति 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 3002 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#पंक्ति 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = शून्य; }
#पंक्ति 3008 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 115: /* OverlayKeyList: OverlayKey  */
#पंक्ति 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = शून्य; }
#पंक्ति 3014 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 116: /* OverlayKey: "key name" "=" "key name"  */
#पंक्ति 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = शून्य; }
#पंक्ति 3020 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 117: /* OutlineList: OutlineList "," OutlineInList  */
#पंक्ति 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य;}
#पंक्ति 3026 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 118: /* OutlineList: OutlineInList  */
#पंक्ति 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = शून्य; }
#पंक्ति 3032 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 119: /* OutlineInList: "{" CoordList "}"  */
#पंक्ति 739 "src/xkbcomp/parser.y"
                        { (व्योम) (yyvsp[-1].व्यंजक); (yyval.geom) = शून्य; }
#पंक्ति 3038 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#पंक्ति 741 "src/xkbcomp/parser.y"
                        { (व्योम) (yyvsp[-1].व्यंजक); (yyval.geom) = शून्य; }
#पंक्ति 3044 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 121: /* OutlineInList: Ident "=" Expr  */
#पंक्ति 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].व्यंजक)); (yyval.geom) = शून्य; }
#पंक्ति 3050 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 122: /* CoordList: CoordList "," Coord  */
#पंक्ति 747 "src/xkbcomp/parser.y"
                        { (व्योम) (yyvsp[-2].व्यंजक); (व्योम) (yyvsp[0].व्यंजक); (yyval.व्यंजक) = शून्य; }
#पंक्ति 3056 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 123: /* CoordList: Coord  */
#पंक्ति 749 "src/xkbcomp/parser.y"
                        { (व्योम) (yyvsp[0].व्यंजक); (yyval.व्यंजक) = शून्य; }
#पंक्ति 3062 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#पंक्ति 753 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = शून्य; }
#पंक्ति 3068 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#पंक्ति 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).शीर्ष); (yyval.geom) = शून्य; }
#पंक्ति 3074 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 126: /* DoodadType: "text"  */
#पंक्ति 760 "src/xkbcomp/parser.y"
                                { (yyval.अंक) = 0; }
#पंक्ति 3080 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 127: /* DoodadType: "outline"  */
#पंक्ति 761 "src/xkbcomp/parser.y"
                                { (yyval.अंक) = 0; }
#पंक्ति 3086 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 128: /* DoodadType: "solid"  */
#पंक्ति 762 "src/xkbcomp/parser.y"
                                { (yyval.अंक) = 0; }
#पंक्ति 3092 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 129: /* DoodadType: "logo"  */
#पंक्ति 763 "src/xkbcomp/parser.y"
                                { (yyval.अंक) = 0; }
#पंक्ति 3098 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 130: /* FieldSpec: Ident  */
#पंक्ति 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#पंक्ति 3104 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 131: /* FieldSpec: Element  */
#पंक्ति 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#पंक्ति 3110 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 132: /* Element: "action"  */
#पंक्ति 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#पंक्ति 3116 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 133: /* Element: "interpret"  */
#पंक्ति 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#पंक्ति 3122 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 134: /* Element: "type"  */
#पंक्ति 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#पंक्ति 3128 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 135: /* Element: "key"  */
#पंक्ति 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#पंक्ति 3134 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 136: /* Element: "group"  */
#पंक्ति 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#पंक्ति 3140 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 137: /* Element: "modifier_map"  */
#पंक्ति 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#पंक्ति 3146 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 138: /* Element: "indicator"  */
#पंक्ति 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#पंक्ति 3152 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 139: /* Element: "shape"  */
#पंक्ति 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#पंक्ति 3158 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 140: /* Element: "row"  */
#पंक्ति 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#पंक्ति 3164 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 141: /* Element: "section"  */
#पंक्ति 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#पंक्ति 3170 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 142: /* Element: "text"  */
#पंक्ति 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#पंक्ति 3176 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 143: /* OptMergeMode: MergeMode  */
#पंक्ति 794 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = (yyvsp[0].merge); }
#पंक्ति 3182 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 144: /* OptMergeMode: %empty  */
#पंक्ति 795 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_DEFAULT; }
#पंक्ति 3188 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 145: /* MergeMode: "include"  */
#पंक्ति 798 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_DEFAULT; }
#पंक्ति 3194 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 146: /* MergeMode: "augment"  */
#पंक्ति 799 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_AUGMENT; }
#पंक्ति 3200 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 147: /* MergeMode: "override"  */
#पंक्ति 800 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_OVERRIDE; }
#पंक्ति 3206 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 148: /* MergeMode: "replace"  */
#पंक्ति 801 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_REPLACE; }
#पंक्ति 3212 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 149: /* MergeMode: "alternate"  */
#पंक्ति 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_MESSAGE_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.merge) = MERGE_DEFAULT;
                }
#पंक्ति 3226 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 150: /* ExprList: ExprList "," Expr  */
#पंक्ति 815 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[0].व्यंजक)) {
                                अगर ((yyvsp[-2].व्यंजक_List).शीर्ष) {
                                    (yyval.व्यंजक_List).शीर्ष = (yyvsp[-2].व्यंजक_List).शीर्ष;
                                    (yyval.व्यंजक_List).last->common.अग्र = &(yyvsp[0].व्यंजक)->common;
                                    (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक);
                                } अन्यथा {
                                    (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक);
                                }
                            }
                        }
#पंक्ति 3242 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 151: /* ExprList: Expr  */
#पंक्ति 827 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3248 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 152: /* ExprList: %empty  */
#पंक्ति 828 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = शून्य; }
#पंक्ति 3254 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 153: /* Expr: Expr "/" Expr  */
#पंक्ति 832 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_द्विआधारी(STMT_व्यंजक_DIVIDE, (yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 3260 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 154: /* Expr: Expr "+" Expr  */
#पंक्ति 834 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_द्विआधारी(STMT_व्यंजक_ADD, (yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 3266 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 155: /* Expr: Expr "-" Expr  */
#पंक्ति 836 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_द्विआधारी(STMT_व्यंजक_SUBTRACT, (yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 3272 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 156: /* Expr: Expr "*" Expr  */
#पंक्ति 838 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_द्विआधारी(STMT_व्यंजक_MULTIPLY, (yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 3278 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 157: /* Expr: Lhs "=" Expr  */
#पंक्ति 840 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_द्विआधारी(STMT_व्यंजक_ASSIGN, (yyvsp[-2].व्यंजक), (yyvsp[0].व्यंजक)); }
#पंक्ति 3284 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 158: /* Expr: Term  */
#पंक्ति 842 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3290 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 159: /* Term: "-" Term  */
#पंक्ति 846 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Unary(STMT_व्यंजक_NEGATE, (yyvsp[0].व्यंजक)); }
#पंक्ति 3296 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 160: /* Term: "+" Term  */
#पंक्ति 848 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Unary(STMT_व्यंजक_UNARY_PLUS, (yyvsp[0].व्यंजक)); }
#पंक्ति 3302 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 161: /* Term: "!" Term  */
#पंक्ति 850 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Unary(STMT_व्यंजक_NOT, (yyvsp[0].व्यंजक)); }
#पंक्ति 3308 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 162: /* Term: "~" Term  */
#पंक्ति 852 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Unary(STMT_व्यंजक_INVERT, (yyvsp[0].व्यंजक)); }
#पंक्ति 3314 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 163: /* Term: Lhs  */
#पंक्ति 854 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3320 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 164: /* Term: FieldSpec "(" ExprList ")"  */
#पंक्ति 856 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Action((yyvsp[-3].atom), (yyvsp[-1].व्यंजक_List).शीर्ष); }
#पंक्ति 3326 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 165: /* Term: Actions  */
#पंक्ति 858 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3332 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 166: /* Term: Terminal  */
#पंक्ति 860 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3338 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 167: /* Term: "(" Expr ")"  */
#पंक्ति 862 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[-1].व्यंजक); }
#पंक्ति 3344 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 168: /* MultiActionList: MultiActionList "," Action  */
#पंक्ति 866 "src/xkbcomp/parser.y"
                        {
                            व्यंजक_Def *व्यंजक = व्यंजक_Create_Action_List((yyvsp[0].व्यंजक));
                            (yyval.व्यंजक_List) = (yyvsp[-2].व्यंजक_List);
                            (yyval.व्यंजक_List).last->common.अग्र = &व्यंजक->common; (yyval.व्यंजक_List).last = व्यंजक;
                        }
#पंक्ति 3354 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 169: /* MultiActionList: MultiActionList "," Actions  */
#पंक्ति 872 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List) = (yyvsp[-2].व्यंजक_List); (yyval.व्यंजक_List).last->common.अग्र = &(yyvsp[0].व्यंजक)->common; (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3360 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 170: /* MultiActionList: Action  */
#पंक्ति 874 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = व्यंजक_Create_Action_List((yyvsp[0].व्यंजक)); }
#पंक्ति 3366 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 171: /* MultiActionList: NonEmptyActions  */
#पंक्ति 876 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3372 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 172: /* ActionList: ActionList "," Action  */
#पंक्ति 880 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List) = (yyvsp[-2].व्यंजक_List); (yyval.व्यंजक_List).last->common.अग्र = &(yyvsp[0].व्यंजक)->common; (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3378 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 173: /* ActionList: Action  */
#पंक्ति 882 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3384 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 174: /* NonEmptyActions: "{" ActionList "}"  */
#पंक्ति 886 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Action_List((yyvsp[-1].व्यंजक_List).शीर्ष); }
#पंक्ति 3390 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 175: /* Actions: NonEmptyActions  */
#पंक्ति 890 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3396 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 176: /* Actions: "{" "}"  */
#पंक्ति 892 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Action_List(शून्य); }
#पंक्ति 3402 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 177: /* Action: FieldSpec "(" ExprList ")"  */
#पंक्ति 896 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Action((yyvsp[-3].atom), (yyvsp[-1].व्यंजक_List).शीर्ष); }
#पंक्ति 3408 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 178: /* Lhs: FieldSpec  */
#पंक्ति 900 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Ident((yyvsp[0].atom)); }
#पंक्ति 3414 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 179: /* Lhs: FieldSpec "." FieldSpec  */
#पंक्ति 902 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Field_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#पंक्ति 3420 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 180: /* Lhs: FieldSpec "[" Expr "]"  */
#पंक्ति 904 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].व्यंजक)); }
#पंक्ति 3426 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#पंक्ति 906 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].व्यंजक)); }
#पंक्ति 3432 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 182: /* OptTerminal: Terminal  */
#पंक्ति 910 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3438 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 183: /* OptTerminal: %empty  */
#पंक्ति 911 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = शून्य; }
#पंक्ति 3444 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 184: /* Terminal: String  */
#पंक्ति 915 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_String((yyvsp[0].atom)); }
#पंक्ति 3450 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 185: /* Terminal: Integer  */
#पंक्ति 917 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Integer((yyvsp[0].अंक)); }
#पंक्ति 3456 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 186: /* Terminal: Float  */
#पंक्ति 919 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_Float(/* Discard $1 */); }
#पंक्ति 3462 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 187: /* Terminal: "key name"  */
#पंक्ति 921 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Name((yyvsp[0].atom)); }
#पंक्ति 3468 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#पंक्ति 925 "src/xkbcomp/parser.y"
                        {
                            व्यंजक_Def *व्यंजक = व्यंजक_Create_कुंजी_Sym_List((yyvsp[0].keysym));
                            (yyval.व्यंजक_List) = (yyvsp[-2].व्यंजक_List);
                            (yyval.व्यंजक_List).last->common.अग्र = &व्यंजक->common; (yyval.व्यंजक_List).last = व्यंजक;
                        }
#पंक्ति 3478 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#पंक्ति 931 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List) = (yyvsp[-2].व्यंजक_List); (yyval.व्यंजक_List).last->common.अग्र = &(yyvsp[0].व्यंजक)->common; (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3484 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 190: /* MultiKeySymList: KeySymLit  */
#पंक्ति 933 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = व्यंजक_Create_कुंजी_Sym_List((yyvsp[0].keysym)); }
#पंक्ति 3490 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 191: /* MultiKeySymList: NonEmptyKeySyms  */
#पंक्ति 935 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक_List).शीर्ष = (yyval.व्यंजक_List).last = (yyvsp[0].व्यंजक); }
#पंक्ति 3496 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 192: /* KeySymList: KeySymList "," KeySymLit  */
#पंक्ति 939 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Append_कुंजी_Sym_List((yyvsp[-2].व्यंजक), (yyvsp[0].keysym)); }
#पंक्ति 3502 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 193: /* KeySymList: KeySymList "," "string literal"  */
#पंक्ति 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यंजक) = व्यंजक_कुंजी_Sym_List_Append_String(param->scanner, (yyvsp[-2].व्यंजक), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                        }
#पंक्ति 3513 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 194: /* KeySymList: KeySymLit  */
#पंक्ति 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Sym_List((yyvsp[0].keysym));
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                        }
#पंक्ति 3523 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 195: /* KeySymList: "string literal"  */
#पंक्ति 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Sym_List(XKB_कुंजी_No_प्रतीक);
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                            (yyval.व्यंजक) = व्यंजक_कुंजी_Sym_List_Append_String(param->scanner, (yyval.व्यंजक), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                        }
#पंक्ति 3537 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#पंक्ति 966 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[-1].व्यंजक); }
#पंक्ति 3543 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 197: /* NonEmptyKeySyms: "string literal"  */
#पंक्ति 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Sym_List(XKB_कुंजी_No_प्रतीक);
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                            (yyval.व्यंजक) = व्यंजक_कुंजी_Sym_List_Append_String(param->scanner, (yyval.व्यंजक), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            अगर (!(yyval.व्यंजक))
                                YYERROR;
                        }
#पंक्ति 3557 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 198: /* KeySyms: NonEmptyKeySyms  */
#पंक्ति 980 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = (yyvsp[0].व्यंजक); }
#पंक्ति 3563 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 199: /* KeySyms: "{" "}"  */
#पंक्ति 982 "src/xkbcomp/parser.y"
                        { (yyval.व्यंजक) = व्यंजक_Create_कुंजी_Sym_List(XKB_कुंजी_No_प्रतीक); }
#पंक्ति 3569 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 200: /* KeySym: KeySymLit  */
#पंक्ति 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#पंक्ति 3575 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 201: /* KeySym: "string literal"  */
#पंक्ति 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            अगर ((yyval.keysym) == XKB_कुंजी_No_प्रतीक)
                                YYERROR;
                        }
#पंक्ति 3586 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 202: /* KeySymLit: "identifier"  */
#पंक्ति 997 "src/xkbcomp/parser.y"
                        {
                            अगर (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (अचिन्हित पूर्णांक) (yyvsp[0].sval).len, (yyvsp[0].sval).start
                                );
                                (yyval.keysym) = XKB_कुंजी_No_प्रतीक;
                            }
                        }
#पंक्ति 3602 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 203: /* KeySymLit: "section"  */
#पंक्ति 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_कुंजी_section; }
#पंक्ति 3608 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 204: /* KeySymLit: "decimal digit"  */
#पंक्ति 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_कुंजी_0 + (xkb_keysym_t) (yyvsp[0].अंक);
                        }
#पंक्ति 3621 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 205: /* KeySymLit: "integer literal"  */
#पंक्ति 1020 "src/xkbcomp/parser.y"
                        {
                            अगर ((yyvsp[0].अंक) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].अंक), (yyvsp[0].अंक)
                                );
                                (yyval.keysym) = XKB_कुंजी_No_प्रतीक;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            अन्यथा {
                                /* Any other numeric value */
                                अगर ((yyvsp[0].अंक) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].अंक);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), शून्य, (yyval.keysym), "%#06"PRIx32, "");
                                } अन्यथा {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].अंक), (yyvsp[0].अंक)
                                    );
                                    (yyval.keysym) = XKB_कुंजी_No_प्रतीक;
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
                                    (yyvsp[0].अंक), (yyvsp[0].अंक)
                                );
                            }
                        }
#पंक्ति 3683 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 206: /* SignedNumber: "-" Number  */
#पंक्ति 1079 "src/xkbcomp/parser.y"
                                        { (yyval.अंक) = -(yyvsp[0].अंक); }
#पंक्ति 3689 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 207: /* SignedNumber: Number  */
#पंक्ति 1080 "src/xkbcomp/parser.y"
                                        { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3695 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 208: /* Number: "float literal"  */
#पंक्ति 1083 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3701 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 209: /* Number: "decimal digit"  */
#पंक्ति 1084 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3707 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 210: /* Number: "integer literal"  */
#पंक्ति 1085 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3713 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 211: /* Float: "float literal"  */
#पंक्ति 1088 "src/xkbcomp/parser.y"
                                { (yyval.अंक) = 0; }
#पंक्ति 3719 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 212: /* Integer: "integer literal"  */
#पंक्ति 1091 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3725 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 213: /* Integer: "decimal digit"  */
#पंक्ति 1092 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3731 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 214: /* KeyCode: "integer literal"  */
#पंक्ति 1095 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3737 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 215: /* KeyCode: "decimal digit"  */
#पंक्ति 1096 "src/xkbcomp/parser.y"
                                      { (yyval.अंक) = (yyvsp[0].अंक); }
#पंक्ति 3743 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 216: /* Ident: "identifier"  */
#पंक्ति 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).start, (yyvsp[0].sval).len); }
#पंक्ति 3749 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 217: /* Ident: "default"  */
#पंक्ति 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#पंक्ति 3755 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 218: /* String: "string literal"  */
#पंक्ति 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), म_माप((yyvsp[0].str))); मुक्त((yyvsp[0].str)); }
#पंक्ति 3761 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 219: /* OptMapName: MapName  */
#पंक्ति 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#पंक्ति 3767 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 220: /* OptMapName: %empty  */
#पंक्ति 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = शून्य; }
#पंक्ति 3773 "src/xkbcomp/parser.c"
    अवरोध;

  हाल 221: /* MapName: "string literal"  */
#पंक्ति 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#पंक्ति 3779 "src/xkbcomp/parser.c"
    अवरोध;


#पंक्ति 3783 "src/xkbcomp/parser.c"

      शेष: अवरोध;
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
  YY_प्रतीक_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    स्थिर पूर्णांक yylhs = yyr1[yyn] - YYNTOKENS;
    स्थिर पूर्णांक yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  जाओ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  अगर (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        अक्षर स्थिर *yymsgp = YY_("syntax error");
        पूर्णांक yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        अगर (yysyntax_error_status == 0)
          yymsgp = yymsg;
        अन्यथा अगर (yysyntax_error_status == -1)
          {
            अगर (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (अक्षर *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            अगर (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            अन्यथा
              {
                yymsg = yymsgbuf;
                yymsg_alloc = माप yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        अगर (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  अगर (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      अगर (yychar <= END_OF_FILE)
        {
          /* Return failure if at end of input.  */
          अगर (yychar == END_OF_FILE)
            YYABORT;
        }
      अन्यथा
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  जाओ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  अगर (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ढेर_PRINT (yyss, yyssp);
  yystate = *yyssp;
  जाओ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  क्रम (;;)
    {
      yyn = yypact[yystate];
      अगर (!yypact_मूल्य_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          अगर (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              अगर (0 < yyn)
                अवरोध;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      अगर (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_प्रतीक (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ढेर_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_प्रतीक_PRINT ("Shifting", YY_ACCESSING_प्रतीक (yyn), yyvsp, yylsp);

  yystate = yyn;
  जाओ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  जाओ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  जाओ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  जाओ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  अगर (yychar != YYEMPTY)
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
  YY_ढेर_PRINT (yyss, yyssp);
  जबतक (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_प्रतीक (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#अगर_अघोषित yyoverflow
  अगर (yyss != yyssa)
    YYSTACK_FREE (yyss);
#पूर्ण_अगर
  अगर (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  वापस yyresult;
}

#पंक्ति 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(काष्ठा xkb_context *ctx, स्थिर काष्ठा parser_keymap_config *config,
      काष्ठा scanner *scanner, स्थिर अक्षर *map)
{
    पूर्णांक ret;
    XkbFile *first = शून्य;
    काष्ठा parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = शून्य,
        .config = *config,
        .more_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    जबतक ((ret = yyparse(&param)) == 0 && param.more_maps) {
        अगर (map) {
            अगर (streq_not_null(map, param.rtrn->name))
                वापस param.rtrn;
            अन्यथा
                FreeXkbFile(param.rtrn);
        }
        अन्यथा {
            अगर (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                वापस param.rtrn;
            }
            अन्यथा अगर (!first) {
                first = param.rtrn;
            }
            अन्यथा {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = शून्य;
    }

    अगर (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        वापस शून्य;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    अगर (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    वापस first;
}

/* Parse the next section */
bool
parse_अग्र(काष्ठा xkb_context *ctx,
           स्थिर काष्ठा parser_keymap_config *config,
           काष्ठा scanner *scanner, XkbFile **xkb_file)
{
    पूर्णांक ret;
    काष्ठा parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = शून्य,
        .config = *config,
        .more_maps = false,
    };

    अगर ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        वापस true;
    } अन्यथा {
        FreeXkbFile(param.rtrn);
        *xkb_file = शून्य;
        वापस (ret == 0);
    }
}
